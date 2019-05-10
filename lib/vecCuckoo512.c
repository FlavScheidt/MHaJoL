#include "vecCuckooFilter.h"

const uint64_t permTB[256] = {0x0706050403020100ull,
     0x0007060504030201ull, 0x0107060504030200ull, 0x0001070605040302ull,
     0x0207060504030100ull, 0x0002070605040301ull, 0x0102070605040300ull,
     0x0001020706050403ull, 0x0307060504020100ull, 0x0003070605040201ull,
     0x0103070605040200ull, 0x0001030706050402ull, 0x0203070605040100ull,
     0x0002030706050401ull, 0x0102030706050400ull, 0x0001020307060504ull,
     0x0407060503020100ull, 0x0004070605030201ull, 0x0104070605030200ull,
     0x0001040706050302ull, 0x0204070605030100ull, 0x0002040706050301ull,
     0x0102040706050300ull, 0x0001020407060503ull, 0x0304070605020100ull,
     0x0003040706050201ull, 0x0103040706050200ull, 0x0001030407060502ull,
     0x0203040706050100ull, 0x0002030407060501ull, 0x0102030407060500ull,
     0x0001020304070605ull, 0x0507060403020100ull, 0x0005070604030201ull,
     0x0105070604030200ull, 0x0001050706040302ull, 0x0205070604030100ull,
     0x0002050706040301ull, 0x0102050706040300ull, 0x0001020507060403ull,
     0x0305070604020100ull, 0x0003050706040201ull, 0x0103050706040200ull,
     0x0001030507060402ull, 0x0203050706040100ull, 0x0002030507060401ull,
     0x0102030507060400ull, 0x0001020305070604ull, 0x0405070603020100ull,
     0x0004050706030201ull, 0x0104050706030200ull, 0x0001040507060302ull,
     0x0204050706030100ull, 0x0002040507060301ull, 0x0102040507060300ull,
     0x0001020405070603ull, 0x0304050706020100ull, 0x0003040507060201ull,
     0x0103040507060200ull, 0x0001030405070602ull, 0x0203040507060100ull,
     0x0002030405070601ull, 0x0102030405070600ull, 0x0001020304050706ull,
     0x0607050403020100ull, 0x0006070504030201ull, 0x0106070504030200ull,
     0x0001060705040302ull, 0x0206070504030100ull, 0x0002060705040301ull,
     0x0102060705040300ull, 0x0001020607050403ull, 0x0306070504020100ull,
     0x0003060705040201ull, 0x0103060705040200ull, 0x0001030607050402ull,
     0x0203060705040100ull, 0x0002030607050401ull, 0x0102030607050400ull,
     0x0001020306070504ull, 0x0406070503020100ull, 0x0004060705030201ull,
     0x0104060705030200ull, 0x0001040607050302ull, 0x0204060705030100ull,
     0x0002040607050301ull, 0x0102040607050300ull, 0x0001020406070503ull,
     0x0304060705020100ull, 0x0003040607050201ull, 0x0103040607050200ull,
     0x0001030406070502ull, 0x0203040607050100ull, 0x0002030406070501ull,
     0x0102030406070500ull, 0x0001020304060705ull, 0x0506070403020100ull,
     0x0005060704030201ull, 0x0105060704030200ull, 0x0001050607040302ull,
     0x0205060704030100ull, 0x0002050607040301ull, 0x0102050607040300ull,
     0x0001020506070403ull, 0x0305060704020100ull, 0x0003050607040201ull,
     0x0103050607040200ull, 0x0001030506070402ull, 0x0203050607040100ull,
     0x0002030506070401ull, 0x0102030506070400ull, 0x0001020305060704ull,
     0x0405060703020100ull, 0x0004050607030201ull, 0x0104050607030200ull,
     0x0001040506070302ull, 0x0204050607030100ull, 0x0002040506070301ull,
     0x0102040506070300ull, 0x0001020405060703ull, 0x0304050607020100ull,
     0x0003040506070201ull, 0x0103040506070200ull, 0x0001030405060702ull,
     0x0203040506070100ull, 0x0002030405060701ull, 0x0102030405060700ull,
     0x0001020304050607ull, 0x0706050403020100ull, 0x0007060504030201ull,
     0x0107060504030200ull, 0x0001070605040302ull, 0x0207060504030100ull,
     0x0002070605040301ull, 0x0102070605040300ull, 0x0001020706050403ull,
     0x0307060504020100ull, 0x0003070605040201ull, 0x0103070605040200ull,
     0x0001030706050402ull, 0x0203070605040100ull, 0x0002030706050401ull,
     0x0102030706050400ull, 0x0001020307060504ull, 0x0407060503020100ull,
     0x0004070605030201ull, 0x0104070605030200ull, 0x0001040706050302ull,
     0x0204070605030100ull, 0x0002040706050301ull, 0x0102040706050300ull,
     0x0001020407060503ull, 0x0304070605020100ull, 0x0003040706050201ull,
     0x0103040706050200ull, 0x0001030407060502ull, 0x0203040706050100ull,
     0x0002030407060501ull, 0x0102030407060500ull, 0x0001020304070605ull,
     0x0507060403020100ull, 0x0005070604030201ull, 0x0105070604030200ull,
     0x0001050706040302ull, 0x0205070604030100ull, 0x0002050706040301ull,
     0x0102050706040300ull, 0x0001020507060403ull, 0x0305070604020100ull,
     0x0003050706040201ull, 0x0103050706040200ull, 0x0001030507060402ull,
     0x0203050706040100ull, 0x0002030507060401ull, 0x0102030507060400ull,
     0x0001020305070604ull, 0x0405070603020100ull, 0x0004050706030201ull,
     0x0104050706030200ull, 0x0001040507060302ull, 0x0204050706030100ull,
     0x0002040507060301ull, 0x0102040507060300ull, 0x0001020405070603ull,
     0x0304050706020100ull, 0x0003040507060201ull, 0x0103040507060200ull,
     0x0001030405070602ull, 0x0203040507060100ull, 0x0002030405070601ull,
     0x0102030405070600ull, 0x0001020304050706ull, 0x0607050403020100ull,
     0x0006070504030201ull, 0x0106070504030200ull, 0x0001060705040302ull,
     0x0206070504030100ull, 0x0002060705040301ull, 0x0102060705040300ull,
     0x0001020607050403ull, 0x0306070504020100ull, 0x0003060705040201ull,
     0x0103060705040200ull, 0x0001030607050402ull, 0x0203060705040100ull,
     0x0002030607050401ull, 0x0102030607050400ull, 0x0001020306070504ull,
     0x0406070503020100ull, 0x0004060705030201ull, 0x0104060705030200ull,
     0x0001040607050302ull, 0x0204060705030100ull, 0x0002040607050301ull,
     0x0102040607050300ull, 0x0001020406070503ull, 0x0304060705020100ull,
     0x0003040607050201ull, 0x0103040607050200ull, 0x0001030406070502ull,
     0x0203040607050100ull, 0x0002030406070501ull, 0x0102030406070500ull,
     0x0001020304060705ull, 0x0506070403020100ull, 0x0005060704030201ull,
     0x0105060704030200ull, 0x0001050607040302ull, 0x0205060704030100ull,
     0x0002050607040301ull, 0x0102050607040300ull, 0x0001020506070403ull,
     0x0305060704020100ull, 0x0003050607040201ull, 0x0103050607040200ull,
     0x0001030506070402ull, 0x0203050607040100ull, 0x0002030506070401ull,
     0x0102030506070400ull, 0x0001020305060704ull, 0x0405060703020100ull,
     0x0004050607030201ull, 0x0104050607030200ull, 0x0001040506070302ull,
     0x0204050607030100ull, 0x0002040506070301ull, 0x0102040506070300ull,
     0x0001020405060703ull, 0x0304050607020100ull, 0x0003040506070201ull,
     0x0103040506070200ull, 0x0001030405060702ull, 0x0203040506070100ull,
     0x0002030405060701ull, 0x0102030405060700ull, 0x0001020304050607ull};


inline void vividGenerate(column_orders * c_orders)
{
	int key[tamOrders];
	int shiftIndex;

	//Masks
	__mmask8 loadMask;
	__mmask8 storeMask;
	__mmask8 remotionMask;
	__mmask8 table1Mask;
	__mmask8 table2Mask;

	//Vectors
	__m256i keysVector; //Vector of keys
	__m256i temporaryVector; //Auxiliary vector used to load new keys
	__m256i permutationMask; //Mask to shuffle new and old keys
	__m256i hashedVector; //Hashed keys
	__m256i hopsVector; 
	__m256i valuesVector;

	//Auxiliary
	__m256i oneVector 	= _mm256_set1_epi32(1);
	__m256i zeroVector 	= _mm256_set1_epi32(0);

	__m128i permMask;

	__m256i tableSizeVector = _mm256_set1_epi32(TAB_SIZE-1);
	// __m256i thresholdVector = _mm256_set1_epi32(VCUCKOO_MAX_TRY);

	//Initiate vectors and masks
	loadMask 		= _cvtu32_mask8(255);
	table1Mask		= _cvtu32_mask8(255);
	table2Mask		= _cvtu32_mask8(0);

	hopsVector 		= _mm256_cmpeq_epi32(oneVector, oneVector);

	//Performance counter variables
	clock_t init, end;

	//Other variables
	size_t tuples = 0;
	size_t index;
	int threshold = 0;

	for (unsigned int i=0; i<tamOrders;i++)
		key[i] = c_orders[i].O_CUSTKEY;

	init = clock();
	// likwid_markerStartRegion("Generation");

	while (tuples <= tamOrders-8)
	{
		threshold++;
		/******************************************
			PHASE 1 - THE LOAD
			Load the new items using the loadMask
		******************************************/
		// temporaryVector = _mm256_maskz_and_epi32(loadMask, oneVector, oneVector);
		// temporaryVector = _mm256_maskload_epi32(&key[tuples], temporaryVector);
		// keysVector		= _mm256_mask_or_epi32(keysVector, loadMask, temporaryVector, zeroVector);
		keysVector = _mm256_mask_expandloadu_epi32 (keysVector, loadMask, &key[tuples]);

		//Number of keys loaded to set the new tuples value
		index 	= _cvtmask8_u32(loadMask);
		tuples += _mm_popcnt_u64(index);

		/*******************************************
			PHASE 2 - THE HASH
		*******************************************/
		hashedVector = _mm256_fnv1a_epi32(keysVector);
		hashedVector = _mm256_maskz_and_epi32 (table1Mask, hashedVector, tableSizeVector);

		temporaryVector = _mm256_murmur3_epi32(keysVector, 0x0D50064F7);
		temporaryVector = _mm256_and_si256(temporaryVector, tableSizeVector);
		hashedVector 	= _mm256_mask_add_epi32(hashedVector, table2Mask, temporaryVector, tableSizeVector);

		/*******************************************
			PHASE 3 - THE RETRIEVAL
			Load the cuckoo table values and check for zeros and duplicated values
		*******************************************/
		valuesVector = _mm256_i32gather_epi32(cuckoo, hashedVector, 4);

		/*******************************************
			PHASE 4 - THE DUPLICATES AND THE ZEROS
			Check if the values are already there
		*******************************************/
		//Duplicates
		remotionMask = _mm256_movepi32_mask(_mm256_cmpeq_epi32(keysVector, valuesVector));

		//Zeros
		loadMask 	= _mm256_movepi32_mask(_mm256_cmpeq_epi32(valuesVector, zeroVector));

		//Remove duplicates from the loadMask and set the store mask where there is no repeated key
		loadMask 	= _kor_mask8(loadMask, remotionMask);
		storeMask 	= _knot_mask8(remotionMask);

		/*******************************************
			PHASE 5 - THE HOPS CALCULATION 
			Calculates the hops, who's gonna be stored on the OHT and who goes to each table
		*******************************************/
		//Calculates the hops removing the ones who reached the threshold

		//Did someone reach the threshold?
		// ohtMask = _mm256_cmpgt_epi32(hopsVector, thresholdVector);
		//Set zero where the threshold has been reached 
		// hopsVector = _mm256_castps_si256(_mm256_andnot_ps(_mm256_castsi256_ps(ohtMask), _mm256_castsi256_ps(hopsVector)));
		//Set zero where a new key must be load
		hopsVector = _mm256_mask_add_epi32(oneVector, loadMask, hopsVector, oneVector);

		//Shifts to set where the number of hops are odd
		temporaryVector 	= _mm256_slli_epi32(hopsVector, 31);
		temporaryVector 	= _mm256_srli_epi32(temporaryVector, 31);

		table1Mask 			= _mm256_movepi32_mask(temporaryVector);
		table2Mask 			= _knot_mask8(table1Mask);

		//Even # of hops goes to tb2, odd # to tb1

		/*******************************************
			PHASE 6 - THE NEW MASKS 
		*******************************************/
		//If the key goes to the OHT, the position is free to hold a new key from the relation

		//Same as the load, but respecting the positions that must be stored on the t1 on the next iteration
		table1Mask = _kor_mask8(remotionMask, table1Mask);
		table2Mask = _kor_mask8(remotionMask, table2Mask);

		/*******************************************
			PHASE 7 - THE STORE
			Almost everyone goes to the cuckoo table... except the ones that reached the threshold, those must be stored on the OHT
		*******************************************/
		_mm256_mask_i32scatter_epi32(cuckoo, storeMask,hashedVector, keysVector, 4);

		/*******************************************
			PHASE 8 - THE SHUFFLE
		*******************************************/
		shiftIndex 			= _cvtmask8_u32(loadMask);
		permMask 			= _mm_loadl_epi64((__m128i*) &permTB[shiftIndex ^ 255]);
		permutationMask 	= _mm256_cvtepi8_epi32(permMask);

		keysVector = _mm256_permutevar8x32_epi32(valuesVector, permutationMask);

		//Shuffle Masks
		temporaryVector 	= _mm256_maskz_and_epi32(loadMask, oneVector, oneVector);
		temporaryVector 	= _mm256_slli_epi32(temporaryVector, 31);
		temporaryVector 	= _mm256_permutevar8x32_epi32(temporaryVector, permutationMask);
		loadMask 			= _mm256_movepi32_mask(temporaryVector);

		temporaryVector 	= _mm256_maskz_and_epi32(table1Mask, oneVector, oneVector);
		temporaryVector 	= _mm256_slli_epi32(temporaryVector, 31);
		temporaryVector 	= _mm256_permutevar8x32_epi32(temporaryVector, permutationMask);
		table1Mask 			= _mm256_movepi32_mask(temporaryVector);

		table2Mask 			= _knot_mask8(table1Mask);
	}

	// likwid_markerStopRegion("Generation");
	end = clock();
	printf("Generation %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return;	
}

inline int vividLookUp(__m256i key)
{
	__m256i hash1;
	__m256i hash2;
	__m256i table1;
	__m256i table2;

	__m256i tableSizeVector = _mm256_set1_epi32(TAB_SIZE-1);
	__m256i oneVector 		= _mm256_set1_epi32(0);
	__m256i zeroVector 		= _mm256_set1_epi32(0);
	oneVector 				= _mm256_cmpeq_epi32(oneVector, zeroVector);

	uint32_t found = 0;

	hash1 = _mm256_fnv1a_epi32(key);
	hash1 = _mm256_and_si256(hash1, tableSizeVector);

	hash2 = _mm256_murmur3_epi32(key, 0x0D50064F7);
	hash2 = _mm256_and_si256(hash2, tableSizeVector);
	hash2 = _mm256_add_epi32(hash2, tableSizeVector);

	table1 = _mm256_i32gather_epi32(cuckoo, hash1, 4);
	table2 = _mm256_i32gather_epi32(cuckoo, hash2, 4);

	hash1 = _mm256_cmpeq_epi32(key, table1);
	hash2 = _mm256_cmpeq_epi32(key, table2);

	hash1 = _mm256_or_si256(hash1, hash2);

	// ohtMask = _mm256_cmpeq_epi32(hash1, zeroVector);
	// ohtMask = _mm256_cmpeq_epi32(ohtMask, zeroVector);

	hash1 = _mm256_cmpeq_epi32(hash1, zeroVector);

	found = _cvtmask8_u32(_mm256_movepi32_mask(hash1));

	// return found;
	return (((found<<31)>>31)+((found<<30)>>31)+((found<<29)>>31)+((found<<28)>>31)+((found<<27)>>31)+((found<<26)>>31)+((found<<25)>>31)+((found<<24)>>31));
}

int vividJoin(column_customer * c_customer, column_orders * c_orders)
{
	clock_t init, end;
	uint32_t index=0;
	int customer[tamCustomer];

	__m256i keys;

	__m256i oneVector = _mm256_set1_epi32(0);
	__m256i zeroVector = _mm256_set1_epi32(0);
	oneVector = _mm256_cmpeq_epi32(oneVector, zeroVector);

	HOPS = 0;			//hops for a given key
	HOPSGENERAL = 0;	//general hops in average
	OCC = 0;			//occupation	
	DUP = 0;			//duplicated keys
	SUC = 0; 			//successfull insertion on the first try
	HOPED = 0;			//Succesfull insertion with hops
	OHTOCC = 0;

	for (int i=0; i<REAL_TAB_SIZE; i++)
		cuckoo[i]=0;

	vividGenerate(c_orders);

	for (int i=0; i<REAL_TAB_SIZE; i++)
		if (cuckoo[i] != 0)
			OCC++;

	for (unsigned int i=0; i<tamCustomer;i++)
		customer[i] = c_customer[i].C_CUSTKEY;

	init=clock();
	// likwid_markerStartRegion("Core");
	for (unsigned int i=0; i<tamCustomer; i=i+8)
	{
		keys = _mm256_maskload_epi32(&customer[i], oneVector);
		index += vividLookUp(keys);
	}
	// likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	printf("Cuckoo Occupation %d\n", OCC);

    return index; 
}
