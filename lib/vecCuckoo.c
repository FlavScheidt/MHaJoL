#include "vecCuckoo.h"

inline void * vecCuckooInsert(void * keys)
{
	int key[8];
	int hashed[8];

	ptrKeysArray keysArrayCT = (ptrKeysArray) keys;
	_mm256_maskstore_epi32 (&key[0], keysArrayCT->mask, keysArrayCT->keys);
	_mm256_maskstore_epi32 (&hashed[0], keysArrayCT->mask, keysArrayCT->hashed);

	pthread_mutex_lock(&lockCuckoo);
	for (int i=0; i<8; i++)
	{
		if (((uint32_t)key[i]) != 0)
			cuckoo[(uint32_t)hashed[i]] = (uint32_t) key[i];
	}
	for (int i = 0; i<8; i++)
		printf("%d %d %d\n", hashed[i], key[i], cuckoo[(uint32_t)hashed[i]]);
	pthread_mutex_unlock(&lockCuckoo);

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

const uint64_t perm[256] = {0x0706050403020100ull,
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

inline void vecCuckooGenerate(column_orders * c_orders)
{
	pthread_t thread[MAX_THREADS];
	void *status;
	int threadIndex=-1;

	int key[tamOrders];
	int shiftIndex;
	//Vectors and masks
	__m256i loadMask; //Mask used to load new keys
	__m256i keysVector; //Vector of keys
	__m256i temporaryVector; //Auxiliary vector used to load new keys
	__m256i permutationMask; //Mask to shuffle new and old keys
	__m256i hashedVector; //Hashed keys
	__m256i table1Mask; //Keys to be inserted on table1
	__m256i table2Mask; //Keys to be inserted on table2
	__m256i remotionMask; //Mask to set duplicated keys
	__m256i hopsVector;

	//Auxiliary Masks
	__m256i mask_1 = _mm256_set1_epi32(1);
	__m256i mask_0 = _mm256_set1_epi32(0);

	keysArray keysArrayCT;
	ptrKeysArray ptrArray =  &keysArrayCT;
	keysArrayOHT keysArrayOHT;
	ptrKeysArrayOHT ptrArrayOHT =  &keysArrayOHT;

	__m128i permMask;

	__m256i tableSizeVector = _mm256_set1_epi32(TAB_SIZE-1);
	__m256i thresholdVector = _mm256_set1_epi32(VCUCKOO_MAX_TRY);

	//Initiate vectors 4and masks
	loadMask = _mm256_cmpeq_epi32(mask_1, mask_1);
	keysVector = _mm256_cmpeq_epi32(mask_0, mask_1);
	temporaryVector = _mm256_cmpeq_epi32(mask_0, mask_1);
	permutationMask =  _mm256_cmpeq_epi32(mask_0, mask_1);
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

	if (pthread_mutex_init(&lockCuckoo, NULL) != 0)
	{
 		fprintf(stderr, "Mutex not created.\n");
 		exit(EXIT_FAILURE);
 	}

	init = clock();

	keysVector = _mm256_maskload_epi32(&key[tuples], loadMask);
	tuples = 8;

	hashedVector = _mm256_fnv1a_epi32(keysVector);
	hashedVector = _mm256_and_si256(hashedVector, tableSizeVector);

	hopsVector = _mm256_andnot_si256(hopsVector, mask_1);
	table1Mask = _mm256_andnot_si256(table1Mask, mask_1);
	table2Mask = _mm256_andnot_si256(table1Mask, mask_1);

	keysArrayCT.keys = _mm256_or_si256(keysVector, mask_0);
	keysArrayCT.hashed = _mm256_or_si256(hashedVector, mask_0);
	keysArrayCT.mask = _mm256_or_si256(loadMask, mask_0);

	if (pthread_create(&thread[0],NULL,vecCuckooInsert,(void *)ptrArray) != 0)
	{
	    perror("pthread_create() error");
	    exit(1);
	}

	while (tuples <= tamOrders-8)
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
		tuples +=  _mm_popcnt_u64(index);


		/*******************************************
			PHASE 2 - THE HASH
		*******************************************/
		hashedVector = _mm256_fnv1a_epi32(keysVector);
		hashedVector = _mm256_and_si256(hashedVector, tableSizeVector);
		hashedVector = _mm256_and_si256(hashedVector, table1Mask);

		temporaryVector = _mm256_murmur3_epi32(keysVector, 0x0D50064F7);
		temporaryVector = _mm256_and_si256(temporaryVector, tableSizeVector);
		temporaryVector = _mm256_and_si256(temporaryVector, table2Mask);

		hashedVector = _mm256_or_si256(hashedVector, temporaryVector);

		/*******************************************
			PHASE 3 - THE RETRIEVAL
			Load the cuckoo table values and check for zeros and duplicated values
		*******************************************/
		if (pthread_join(thread[0], &status) != 0) 
		{
		    perror("pthread_join() error");
		    exit(3);
		}

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
		table1Mask = _mm256_srli_epi32(table1Mask, 31);
		table2Mask = _mm256_andnot_si256(table1Mask, mask_1);

		/*******************************************
			PHASE 4 - THE STORE
			Almost everyone goes to the cuckoo table... except the ones that reached the threshold, those must be stored on the OHT
		*******************************************/
		keysArrayCT.keys = _mm256_or_si256(keysVector, mask_0);
		keysArrayCT.hashed = _mm256_or_si256(hashedVector, mask_0);
		keysArrayCT.mask = _mm256_or_si256(loadMask, mask_0);

		if(pthread_create(&thread[0],NULL,vecCuckooInsert,(void *)ptrArray)!=0)
		{
		    perror("pthread_create() error");
		    exit(1);
		}

		keysArrayOHT.keys = _mm256_or_si256(keysVector, mask_0);
		keysArrayOHT.mask = _mm256_or_si256(loadMask, mask_0);

		if (pthread_create(&thread[1], NULL, vecCuckooInsertOHT, (void *) ptrArrayOHT)!= 0) 
		{
		    perror("pthread_create() error");
		    exit(1);
		}

		/*******************************************
			PHASE 4 - THE SHUFFLE
		*******************************************/
		shiftIndex = _mm256_movemask_ps(_mm256_castsi256_ps(loadMask));
		permMask = _mm_loadl_epi64((__m128i*) &perm[shiftIndex ^ 255]);
		permutationMask = _mm256_cvtepi8_epi32(permMask);
		loadMask = _mm256_permutevar8x32_epi32(loadMask, permutationMask);
		keysVector = _mm256_permutevar8x32_epi32(keysVector, permutationMask);
		table1Mask = _mm256_permutevar8x32_epi32(table1Mask, permutationMask);
		table2Mask = _mm256_permutevar8x32_epi32(table2Mask, permutationMask);
	}

	end = clock();
	printf("Generation %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	pthread_mutex_destroy(&lockCuckoo);
	return;
	
}

inline int vecCuckooLookUpOHT(__m256i key, __m256i hash1)
{
	return 0;
}

inline int vecCuckooLookUp(__m256i key)
{
	__m256i hash1;
	__m256i hash2;
	__m256i table1;
	__m256i table2;

	__m256i tableSizeVector = _mm256_set1_epi32(TAB_SIZE-1);
	__m256i mask_1 = _mm256_set1_epi32(1);

	int found = 0;
	int ohtFound=0;

	hash1 = _mm256_fnv1a_epi32(key);
	hash1 = _mm256_and_si256(hash1, tableSizeVector);

	hash2 = _mm256_murmur3_epi32(key, 0x0D50064F7);
	hash2 = _mm256_and_si256(hash2, tableSizeVector);

	hash1 = _mm256_or_si256(hash1, hash2);

	table1 = _mm256_i32gather_epi32(cuckoo, hash1, 4);
	table2 = _mm256_i32gather_epi32(cuckoo, hash2, 4);

	hash1 = _mm256_cmpeq_epi32(key, table1);
	hash2 = _mm256_cmpeq_epi32(key, table2);

	hash1 = _mm256_or_si256(hash1, hash2);

	//Invert, cause its a NOT IN and then see if we need
	hash1 = _mm256_andnot_si256(hash1, mask_1);
	if (_mm256_testz_si256(hash1, mask_1) == 0)
		ohtFound=vecCuckooLookUpOHT(key, hash1);

	found = _mm256_movemask_ps(_mm256_castsi256_ps(hash1));
	return (((found<<31)>>31)+((found<<30)>>31)+((found<<29)>>31)+((found<<28)>>31)+((found<<27)>>31)+((found<<26)>>31)+((found<<25)>>31)+((found<<24)>>31))+ohtFound;

}

int vecCuckooJoin(column_customer * c_customer, column_orders * c_orders)
{
	clock_t init, end;
	uint32_t index=0;
	int customer[tamCustomer];

	__m256i keys;

	__m256i mask_1 = _mm256_set1_epi32(1);


	HOPS = 0;			//hops for a given key
	HOPSGENERAL = 0;	//general hops in average
	OCC = 0;			//occupation	
	DUP = 0;			//duplicated keys
	SUC = 0; 			//successfull insertion on the first try
	HOPED = 0;			//Succesfull insertion with hops

	for (int i=0; i<REAL_TAB_SIZE; i++)
		cuckoo[i]=0;

	vecCuckooGenerate(c_orders);

	// for (int i=0; i<REAL_TAB_SIZE; i++)
	// 	printf("%d\n", cuckoo[i]);

	for (int i=0; i<tamCustomer;i++)
		customer[i] = c_customer[i].C_CUSTKEY;

	init=clock();
	likwid_markerStartRegion("Core");
	for (int i=0; i<tamCustomer; i=i+8)
	{
		keys = _mm256_maskload_epi32(&customer[i], mask_1);
		index += vecCuckooLookUp(keys);
	}
	likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

    return index; 
}
