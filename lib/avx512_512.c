#include "ViViD.h"

inline void vivid512Generate(column_orders * c_orders)
{
	int key[tamOrders];
	// int shiftIndex;

	//Masks
	__mmask16 loadMask;
	__mmask16 storeMask;
	__mmask16 remotionMask;
	__mmask16 table1Mask;
	__mmask16 table2Mask;

	//Vectors
	__m512i keysVector; //Vector of keys
	__m512i temporaryVector; //Auxiliary vector used to load new keys
	__m512i hashedVector; //Hashed keys
	__m512i hopsVector; 
	__m512i valuesVector;

	//Auxiliary
	__m512i oneVector 		= _mm512_set1_epi32(1);
	__m512i allOneVector 	= _mm512_set1_epi32(0xFFFFFFFF);
	__m512i zeroVector 		= _mm512_set1_epi32(0);

	__m512i hashedAuxVector;
	__m512i valuesAuxVector;

	__mmask16 remotionAuxMask;

	__m512i tableSizeVector = _mm512_set1_epi32(TAB_SIZE-1);

	//Initiate vectors and masks
	loadMask 		= _cvtu32_mask16(65535);
	table1Mask		= _cvtu32_mask16(65535);
	table2Mask		= _cvtu32_mask16(0);

	hopsVector 		= _mm512_set1_epi32(1);

	//Performance counter variables
	clock_t init, end;

	//Other variables
	size_t tuples = 0;
	size_t index;

	for (unsigned int i=0; i<tamOrders;i++)
		key[i] = c_orders[i].O_CUSTKEY;

	init = clock();
	// likwid_markerStartRegion("Generation");

	while (tuples <= tamOrders)
	{
		/******************************************
			PHASE 1 - THE LOAD
			Load the new items using the loadMask
		******************************************/
		//Load functions from AVX-512 dont work - gcc problem?? Try using expandloadu
		// temporaryVector = _mm512_maskz_and_epi32(loadMask, allOneVector, allOneVector);
		temporaryVector = _mm512_maskz_expandloadu_epi32(loadMask,&key[tuples]);
		keysVector		= _mm512_mask_or_epi32(keysVector, loadMask, temporaryVector, zeroVector);

		//Number of keys loaded to set the new tuples value
		index 	= _cvtmask16_u32(loadMask);
		tuples += _mm_popcnt_u64(index);

		/*******************************************
			PHASE 2 - THE HASH
		*******************************************/
		hashedVector 	= _mm512_fnv1a_epi32(keysVector);
		hashedVector 	= _mm512_and_si512 (hashedVector, tableSizeVector);

		temporaryVector = _mm512_murmur3_epi32(keysVector, 0x0D50064F7);
		temporaryVector = _mm512_and_si512(temporaryVector, tableSizeVector);
		temporaryVector = _mm512_add_epi32(temporaryVector, tableSizeVector);

		hashedVector 	= _mm512_mask_and_epi32(temporaryVector, table1Mask, hashedVector, allOneVector);
		hashedAuxVector = _mm512_mask_and_epi32(temporaryVector, table2Mask, hashedVector, allOneVector);

		/*******************************************
			PHASE 3 - THE RETRIEVAL
			Load the cuckoo table values and check for zeros and duplicated values
		*******************************************/
		valuesVector 	= _mm512_i32gather_epi32(hashedVector, (int const*)cuckoo, 4);
		valuesAuxVector = _mm512_i32gather_epi32(hashedAuxVector, (int const*)cuckoo, 4);

		/*******************************************
			PHASE 4 - THE DUPLICATES AND THE ZEROS
			Check if the values are already there
		*******************************************/
		//Duplicates -> need to search on BOTH tables
		remotionAuxMask = _mm512_cmpeq_epi32_mask(keysVector, valuesAuxVector);
		remotionMask 	= _mm512_cmpeq_epi32_mask(keysVector, valuesVector);

		remotionMask	= _kor_mask16(remotionAuxMask, remotionMask);

		//Zeros
		loadMask 	= _mm512_cmpeq_epi32_mask(valuesVector, zeroVector);

		//Remove duplicates from the loadMask and set the store mask where there is no repeated key
		loadMask 	= _kor_mask16(loadMask, remotionMask);
		storeMask 	= _knot_mask16(remotionMask);

		/*******************************************
			PHASE 5 - THE HOPS CALCULATION 
			Calculates the hops, who's gonna be stored on the OHT and who goes to each table
		*******************************************/
		//Set zero where a new key must be load
		hopsVector 			= _mm512_mask_add_epi32(oneVector, _knot_mask16(loadMask), hopsVector, oneVector);

		//Shifts to set where the number of hops are odd
		temporaryVector 	= _mm512_slli_epi32(hopsVector, 31);

		table1Mask 			= _mm512_movepi32_mask(temporaryVector);
		table2Mask 			= _knot_mask16(table1Mask);

		//Even # of hops goes to tb2, odd # to tb1

		/*******************************************
			PHASE 6 - THE NEW MASKS 
		*******************************************/
		//Same as the load, but respecting the positions that must be stored on the t1 on the next iteration
		table1Mask = _kor_mask16(remotionMask, table1Mask);
		table2Mask = _kor_mask16(remotionMask, table2Mask);

		/*******************************************
			PHASE 7 - THE STORE
			Almost everyone goes to the cuckoo table... except the ones that reached the threshold, those must be stored on the OHT
		*******************************************/
		_mm512_mask_i32scatter_epi32(cuckoo, storeMask,hashedVector, keysVector, 4);

		/*******************************************
			PHASE 8 - THE SHUFFLE
		*******************************************/
		// shiftIndex 			= _cvtmask16_u32(loadMask);
		// permMask 			= _mm_loadl_epi64((__m128i*) &permTB[shiftIndex ^ 65535]);
		// permutationMask 	= _mm512_cvtepi16_epi32(permMask);

		// keysVector 			= _mm512_permutexvar_epi32(permutationMask,valuesVector);
		keysVector			= _mm512_mask_compress_epi32(valuesVector, loadMask, zeroVector);
		hopsVector			= _mm512_mask_compress_epi32(hopsVector, loadMask, zeroVector);

		//Shuffle Masks
		temporaryVector 	= _mm512_maskz_and_epi32(table1Mask, allOneVector, allOneVector);
		temporaryVector		= _mm512_mask_compress_epi32(temporaryVector, loadMask, zeroVector);
		table1Mask 			= _mm512_movepi32_mask(temporaryVector);

		temporaryVector 	= _mm512_maskz_and_epi32(table2Mask, allOneVector, allOneVector);
		temporaryVector		= _mm512_mask_compress_epi32(temporaryVector, loadMask, zeroVector);
		table2Mask 			= _mm512_movepi32_mask(temporaryVector);

		temporaryVector 	= _mm512_maskz_and_epi32(loadMask, allOneVector, allOneVector);
		temporaryVector		= _mm512_mask_compress_epi32(temporaryVector, loadMask, zeroVector);
		loadMask 			= _mm512_movepi32_mask(temporaryVector);

	}

	// likwid_markerStopRegion("Generation");
	end = clock();
	printf("Generation %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return;	
}

inline int vivid512LookUp(__m512i key)
{
	__m512i hash1;
	__m512i hash2;
	__m512i table1;
	__m512i table2;

	__mmask16 found1;
	__mmask16 found2;

	__m512i tableSizeVector = _mm512_set1_epi32(TAB_SIZE-1);

	uint32_t found = 0;

	hash1 = _mm512_fnv1a_epi32(key);
	hash1 = _mm512_and_si512(hash1, tableSizeVector);

	hash2 = _mm512_murmur3_epi32(key, 0x0D50064F7);
	hash2 = _mm512_and_si512(hash2, tableSizeVector);
	hash2 = _mm512_add_epi32(hash2, tableSizeVector);

	table1 = _mm512_i32gather_epi32(hash1, (int const*)cuckoo, 4);
	table2 = _mm512_i32gather_epi32(hash2, (int const*)cuckoo, 4);

	found1 = _mm512_cmpeq_epi32_mask(key, table1);
	found2 = _mm512_cmpeq_epi32_mask(key, table2);

	found = _mm512_mask2int(_kor_mask16(found1, found2));

	return popCounter(found);
}

int vivid512Join(column_customer * c_customer, column_orders * c_orders)
{
	clock_t init, end;
	uint32_t index=0;
	int customer[tamCustomer];

	__m512i keys;

	for (int i=0; i<REAL_TAB_SIZE; i++)
		cuckoo[i]=0;

	vivid512Generate(c_orders);

	for (unsigned int i=0; i<tamCustomer;i++)
		customer[i] = c_customer[i].C_CUSTKEY;

	init=clock();
	// likwid_markerStartRegion("Core");
	for (unsigned int i=0; i<tamCustomer; i=i+8)
	{
		keys = _mm512_load_epi32(&customer[i]);
		index += vivid512LookUp(keys);
	}
	// likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

    return index; 
}
