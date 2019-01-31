#include "vecCuckoo.h"

inline void * vecCuckooInsert(void * keys)
{
	int key[8];
	int hashed[8];

	ptrKeysArray keysArrayCT = (ptrKeysArray) keys;
	_mm256_maskstore_epi32 (&key[0], keysArrayCT->mask, keysArrayCT->keys);
	_mm256_maskstore_epi32 (&hashed[0], keysArrayCT->mask, keysArrayCT->hashed);

	for (int i=0; i<8; i++)
	{
		if (key[i] != 0)
			cuckoo[hashed[i]] = key[i];
	}

	pthread_exit(NULL);
}

inline void * vecCuckooInsertOHT(void * keys)
{
	// ptrkeysArraOHT keysArraOHT = (ptrkeysArraOHT) keys;

	// for (int i=0; i<8; i++)
	// {
	//  	if (keysArraOHT->key[i] != 0)
	//  		OHT[] = keysArrayCT->key[i];
	// }

	pthread_exit(NULL);
}


inline void vecCuckooGenerate(column_orders * c_orders)
{
	pthread_t thread;
	int key[tamOrders];
	//Vectors and masks
	__m256i loadMask; //Mask used to load new keys
	__m256i keysVector; //Vector of keys
	__m256i temporaryVector; //Auxiliary vector used to load new keys
	// __m256i permutationMask; //Mask to shuffle new and old keys
	__m256i hashedVector; //Hashed keys
	__m256i table1Mask; //Keys to be inserted on table1
	__m256i table2Mask; //Keys to be inserted on table2
	__m256i remotionMask; //Mask to set duplicated keys
	__m256i hopsVector;

	//Auxiliary Masks
	__m256i mask_1 = _mm256_set1_epi32(1);
	__m256i mask_0 = _mm256_set1_epi32(0);

	// __m256i * ptr;
	// __m256i *ohtPtr;
	keysArray keysArrayCT;
	ptrKeysArray ptrArray =  &keysArrayCT;
	keysArrayOHT keysArrayOHT;
	ptrKeysArrayOHT ptrArrayOHT =  &keysArrayOHT;
	// ptrKeysArrayOHT keysOHT = (ptrKeysArrayOHT) malloc(sizeof(keysArrayOHT));
	// int insertValues[8];
	// int insertKeys[8];

	// __m128i permMask;

	__m256i tableSizeVector = _mm256_set1_epi32(TAB_SIZE-1);
	__m256i thresholdVector = _mm256_set1_epi32(VCUCKOO_MAX_TRY);

	//Initiate vectors 4and masks
	loadMask = _mm256_cmpeq_epi32(mask_1, mask_1);
	keysVector = _mm256_cmpeq_epi32(mask_0, mask_1);
	temporaryVector = _mm256_cmpeq_epi32(mask_0, mask_1);
	/* permutationMask,*/ 
	hashedVector = _mm256_cmpeq_epi32(mask_1, mask_1);
	table1Mask = _mm256_cmpeq_epi32(mask_1, mask_1);
	table2Mask =_mm256_cmpeq_epi32(mask_0, mask_1);
	hopsVector = _mm256_cmpeq_epi32(mask_1, mask_1);
	remotionMask = _mm256_cmpeq_epi32(mask_0, mask_1);

	//Performance counter variables
	clock_t init, end;

	//Other variables
	size_t tuples = 0;
	size_t index;

	for (int i=0; i<tamOrders;i++)
		key[i] = c_orders[i].O_CUSTKEY;

	init = clock();
	do 
	{
		/******************************************
			PHASE 1 - THE LOAD
			Load the new items using the loadMask
		******************************************/
		temporaryVector = _mm256_maskload_epi32(&key[tuples], loadMask);
		
		keysVector = _mm256_andnot_si256(loadMask, keysVector);
		keysVector = _mm256_or_si256(keysVector, temporaryVector);

		//Number of keys loaded to set the new tuples value
		index = _mm256_movemask_ps(_mm256_castsi256_ps(loadMask));
		tuples +=  _mm_popcnt_u64(index)-1;


		/*******************************************
			PHASE 2 - THE HASH
		*******************************************/
		hashedVector = _mm256_and_si256(keysVector, table1Mask);
		hashedVector = _mm256_fnv1a_epi32(hashedVector);
		hashedVector = _mm256_and_si256(hashedVector, tableSizeVector);

		temporaryVector = _mm256_and_si256(keysVector, table2Mask);
		temporaryVector = _mm256_murmur3_epi32(temporaryVector, 0x0D50064F7);
		temporaryVector = _mm256_and_si256(temporaryVector, tableSizeVector);

		//We need to sum the tables size to get the indexes of the second table, but we then to stay zero if they are not set for table 2
		remotionMask = _mm256_add_epi32(temporaryVector, tableSizeVector);
		temporaryVector = _mm256_cmpeq_epi32(temporaryVector, remotionMask);
		temporaryVector = _mm256_andnot_si256(temporaryVector, mask_1);
		temporaryVector = _mm256_mul_epi32(temporaryVector, remotionMask);

		hashedVector = _mm256_or_si256(hashedVector, temporaryVector);

		/*******************************************
			PHASE 3 - THE RETRIEVAL
			Load the cuckoo table values and check for zeros and duplicated values
		*******************************************/
		temporaryVector = _mm256_i32gather_epi32(cuckoo, hashedVector, 4);
		remotionMask = _mm256_cmpeq_epi32(keysVector, temporaryVector);
		loadMask = _mm256_cmpeq_epi32(temporaryVector, mask_0);
		loadMask = _mm256_xor_si256(loadMask, remotionMask);

		/*******************************************
			PHASE 4 - THE HOPS CALCULATION
			Calculates the hops, who's gonna be stored on the OHT and who goes to each table
		*******************************************/
		remotionMask = _mm256_cmpeq_epi32(hopsVector, thresholdVector);
		remotionMask = _mm256_andnot_si256(remotionMask, mask_1);
		hopsVector = _mm256_and_si256(hopsVector, remotionMask);
		hopsVector = _mm256_andnot_si256(loadMask, hopsVector);
		hopsVector = _mm256_add_epi32(hopsVector, mask_1);
		table1Mask = _mm256_slli_epi32(hopsVector, 31);
		table1Mask = _mm256_srli_epi32(hopsVector, 31);
		table2Mask = _mm256_andnot_si256(table1Mask, mask_1);

		/*******************************************
			PHASE 4 - THE STORE
			Almost everyone goes to the cuckoo table... except the ones that reached the threshold, those must be stored on the OHT
		*******************************************/
		keysArrayCT.keys = _mm256_or_si256(keysVector, mask_0);
		keysArrayCT.hashed = _mm256_or_si256(hashedVector, mask_0);
		keysArrayCT.mask = _mm256_or_si256(loadMask, mask_0);

		pthread_create(&thread, NULL, vecCuckooInsert, (void *) ptrArray);

		keysArrayOHT.keys = _mm256_or_si256(keysVector, mask_0);
		keysArrayOHT.mask = _mm256_or_si256(loadMask, mask_0);

		pthread_create(&thread, NULL, vecCuckooInsertOHT, (void *) ptrArrayOHT);

		// _mm256_maskstore_epi32 (keysArrayCT->hashed, remotionMask, hashedVector);
		// _mm256_maskstore_epi32 (keysArrayCT->keys, loadMask, keysVector);
		// _mm256_maskstore_epi32 (insertKeys, loadMask, hashedVector);
		// _mm256_maskstore_epi32 (insertValues, loadMask, keysVector);

		// for (int i=0; i<8; i++)
		// {
		// 	if (insertValues[i] != 0)
		// 		cuckoo[insertKeys[i]] = insertValues[i];
		// }

		// // pthread_create(&thread, NULL, vecCuckooInsert, (void *) keysArrayCT);

		// remotionMask = _mm256_andnot_si256(remotionMask, mask_1);
		// if (_mm256_testz_si256(remotionMask, mask_1))
		// {
		// 	_mm256_maskstore_epi32 (insertValues, remotionMask, hashedVector);
		// 	// _mm256_maskstore_epi32 (keysOHT->keys, remotionMask, hashedVector);
		// 	// pthread_create(&thread, NULL, vecCuckooInsertOHT, (void *) keysOHT);
		// }
		// end = clock();
		// printf("Iteration %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// table1Mask = _mm256__si256(table1Mask, loadMask);
		// // table1Mask = _mm256_and_si256(table1Mask, remotionMask);
		// // table1Mask = _mm256_andnot_si256(table1Mask, mask_1);

		// table2Mask = _mm256_xor_si256(table2Mask, loadMask);
		// // table2Mask = _mm256_and_si256(table2Mask, remotionMask);
		// table2Mask = _mm256_andnot_si256(table2Mask, mask_1);
		
		// Invert values on the tables mask's
		// table1Mask = _mm256_andnot_si256(table1Mask, mask_1);
		// table2Mask = _mm256_andnot_si256(table2Mask, mask_1);

		// //Move new keys to keys vector
		// keysVector = _mm256_or_si256(keysVector, temporaryVector);

		//Shuffle vector
		// shiftIndex = _mm256_movemask_ps(_mm256_castsi256_ps(loadMask));
		// permMask = _mm_loadl_epi64((__m128i*) &perm[shiftIndex ^ 255]);
		// permutationMask = mm256_cvtepi8_epi32(permMask);
		// loadMask = _mm256_permutevar8x32_epi32(loadMask, permutationMask);
		// keysVector = _mm256_permutevar8x32_epi32(keysVector, permutationMask);
		// table1Mask = _mm256_permutevar8x32_epi32(table1Mask, permutationMask);
		// table2Mask = _mm256_permutevar8x32_epi32(table2Mask, permutationMask);
	} while (tuples <= tamOrders-8);
	end = clock();
	printf("Generation %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	
}

int vecCuckooJoin(column_customer * c_customer, column_orders * c_orders)
{
	// clock_t init, end;
	uint32_t index=0;

	HOPS = 0;			//hops for a given key
	HOPSGENERAL = 0;	//general hops in average
	OCC = 0;			//occupation	
	DUP = 0;			//duplicated keys
	SUC = 0; 			//successfull insertion on the first try
	HOPED = 0;			//Succesfull insertion with hops

	vecCuckooGenerate(c_orders);

	// init=clock();
	// likwid_markerStartRegion("Core");
	// for (int i=0; i<tamCustomer; i++)
	// {
	// 	if (vecCuckooLookUp(c_customer[i].C_CUSTKEY) == 0)
	// 		index++;
	// }
	// likwid_markerStopRegion("Core");
	// end=clock();

	// printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

    return index; 
}
