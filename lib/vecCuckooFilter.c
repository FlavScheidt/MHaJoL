#include "vecCuckooFilter.h"

inline void cViViDGenerateFilter(column_orders * c_orders)
{
	alignas(32) int cuckooKey[8];
	alignas(32) int cuckooHashed[8];
	alignas(32) uint8_t fingerprint[8];

	int key[tamOrders];
	int shiftIndex;

	//Vectors and masks
	__m256i loadMask; //Mask used to load new keys
	__m256i keysVector; //Vector of keys
	__m256i temporaryVector; //Auxiliary vector used to load new keys
	__m256i  temporaryMask;
	__m256i fingerprintVector;
	__m256i permutationMask; //Mask to shuffle new and old keys
	__m256i bucketVector; //Bucket
	__m256i positionVector; //Position
	__m256i shiftVector; //Shifts to be performed to insert and lookup
	__m256i table1Mask; //Keys to be inserted on table1
	__m256i table2Mask; //Keys to be stored on table2
	__m256i remotionMask;
	__m256i hopsVector; 
	__m256i table1Values;
	__m256i table2Values;
	__m256i storeMask;
	// __m256i ohtMask;
	__m256i mask_1 = _mm256_set1_epi32(1);
	__m256i mask_0 = _mm256_set1_epi32(0);

	__m128i permMask;

	__m256i tableSizeVector = _mm256_set1_epi32(FILTER_SIZE-1);
	// __m256i thresholdVector = _mm256_set1_epi32(VCUCKOO_MAX_TRY);

	//Initiate vectors 4and masks
	loadMask = _mm256_cmpeq_epi32(mask_1, mask_1);
	keysVector = _mm256_cmpeq_epi32(mask_0, mask_1);
	temporaryVector = _mm256_cmpeq_epi32(mask_0, mask_1);
	permutationMask =  _mm256_cmpeq_epi32(mask_0, mask_1);
	bucketVector = _mm256_cmpeq_epi32(mask_1, mask_1);
	positionVector = _mm256_cmpeq_epi32(mask_1, mask_1);
	shiftVector = _mm256_cmpeq_epi32(mask_1, mask_1); 
	table1Mask = _mm256_cmpeq_epi32(mask_1, mask_1);
	table2Mask =_mm256_cmpeq_epi32(mask_0, mask_1);
	hopsVector = _mm256_cmpeq_epi32(mask_1, mask_1);
	remotionMask = _mm256_cmpeq_epi32(mask_0, mask_1);
	// ohtMask = _mm256_cmpeq_epi32(mask_0, mask_1);
	table1Values =_mm256_cmpeq_epi32(mask_0, mask_1);
	table2Values =_mm256_cmpeq_epi32(mask_0, mask_1);
	storeMask = _mm256_cmpeq_epi32(mask_1, mask_1);

	//Performance counter variables
	clock_t init, end;

	//Other variables
	size_t tuples = 0;
	size_t index;
	int threshold = 0;
	for (unsigned int i=0; i<tamOrders;i++)
		key[i] = c_orders[i].O_CUSTKEY;

	init = clock();
	likwid_markerStartRegion("Generation");

	/******************************************
		First Iteration
	******************************************/
	//Load
	keysVector = _mm256_maskload_epi32(&key[tuples], loadMask);
	tuples = 8;

	//Fingerprint
	fingerprintVector = _mm256_fnv1a_epi32(keysVector);
	temporaryMask = _mm256_set1_epi32(255);
	fingerprintVector = _mm256_and_si256(fingerprintVector, temporaryMask); //Truncate to get the last 8 bits

	//Hash to get the bucket
	bucketVector = _mm256_fnv1a_epi32(keysVector); //hash
	bucketVector = _mm256_and_si256(bucketVector, tableSizeVector);//Limit the hash to the size of the table

	//Hash to get position into the bucket
	//No mod instruction! Let's explicitly do this -> hash-((hash/5)*5)
	temporaryMask = _mm256_set1_epi32(5);
	positionVector = _mm256_castps_si256(_mm256_round_ps(_mm256_div_ps(_mm256_castsi256_ps(bucketVector), _mm256_castsi256_ps(temporaryMask)),(_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC))); //hash/5
	positionVector = _mm256_mul_epi32(bucketVector, temporaryMask); // (hash/5)*5
	positionVector = _mm256_sub_epi32(temporaryMask, temporaryVector); //hash - ((hash/5)*5)

	//Retrieve Buckets
	table1Values = _mm256_i32gather_epi32(filter, bucketVector, 4);

	//Setup store masks for the positions
	// temporaryVector = _mm256_cmpeq_epi32(mask_0, mask_0);
	// temporaryMask = _mm256_set1_epi32(FINGERPRINT_SIZE);
	// temporaryMask = _mm256_mul_epi32(temporaryMask, positionVector);
	// temporaryMask = _mm256_sub_epi32(temporaryMask, mask_1);

	// temporaryVector = _mm256_srav_epi32(temporaryVector, temporaryMask);
	// temporaryVector = _mm256_slli_epi32(temporaryVector, 3*FINGERPRINT_SIZE);
	// temporaryMask = _mm256_set1_epi32(FINGERPRINT_SIZE);

	// remotionMask = _mm256_set1_epi32(4);
	// temporaryMask = _mm256_mul_epi32(temporaryMask, positionVector);
	// temporaryMask = _mm256_sub_epi32(remotionMask, temporaryMask);

	// temporaryVector = _mm256_srav_epi32(temporaryVector, temporaryMask);
	// temporaryVector = _mm256_cmpeq_epi8(temporaryVector, mask_0);

	//Move fingerprints to the desired position
	

	//Clean up memory to store
	for (int i=0; i<8; i++)
	{
		cuckooKey[i] = 0;
		cuckooHashed[i] = 0;
	}

	//

	_mm256_maskstore_epi32(&cuckooKey[0], storeMask, keysVector);
	_mm256_maskstore_epi32(&cuckooHashed[0], storeMask, bucketVector);

	for (int i=0; i<8; i++)
	{
		if (((uint32_t)cuckooKey[i]) != 0)
			cuckoo[(uint32_t)cuckooHashed[i]] = (uint32_t) cuckooKey[i];
	}

}
// inline int vecInsertFilter(unsigned int key, int tamOrders)
// {
// 	char str[10];

// 	uint32_t olderCuckoo;
// 	uint32_t aux;
// 	uint16_t bigDesloc;
// 	uint16_t littleDesloc;

// 	uint32_t index0;
// 	uint32_t index1;

// 	//Filter Phase
// 	uint32_t vIndex0;
// 	uint32_t iIndex0;
	
// 	uint32_t vIndex1;
// 	uint32_t iIndex1;

// 	index0 = HASH_FILTER0;
// 	olderCuckoo = HASH_FINGERPRINT;
// 	sprintf(str, "%d", olderCuckoo);
// 	index1 = HASH_FILTER1;

// 	vIndex0 = (uint32_t) index0/5; //vector index
// 	iIndex0 = index0%5; //bucket index inside 32 bits word
		
// 	vIndex1 = (uint32_t) index1/5; //vector index
// 	iIndex1 = index1%5; //bucket index inside 32 bits word

// 	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo)) ||
// 			(((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo)))
// 		return 3;

// 	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 	{
// 		filter0CCT[vIndex0] = filter0CCT[vIndex0]
// 				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) ;
// 		return 1;
// 	}

// 	if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 	{
// 		filter1CCT[vIndex1] = filter1CCT[vIndex1]
// 				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) ;
// 		return 1;
// 	}

// 	for (int j=0; j<CCT_THRESHOLD; j++)
// 	{
// 		//Always insert on the first table
// 		aux = (filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE)) >> 56;
// 		bigDesloc = (4+iIndex0)*CCT_FINGERPRINT_SIZE;
// 		littleDesloc = (5-iIndex0)*CCT_FINGERPRINT_SIZE;
// 		if (iIndex0 == 4)
// 			filter0CCT[vIndex0] = ((filter0CCT[vIndex0] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter0CCT[vIndex0] >> bigDesloc) << bigDesloc);
// 		else
// 			filter0CCT[vIndex0] = ((filter0CCT[vIndex0] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter0CCT[vIndex0] >> bigDesloc) << bigDesloc);
// 		olderCuckoo = aux;

// 		sprintf(str, "%d", olderCuckoo);
// 		index1 = HASH_FILTER1;
		
// 		vIndex1 = (uint32_t) index1/5; //vector index
// 		iIndex1 = index1%5; //bucket index inside 32 bits word	

// 		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == olderCuckoo))
// 			return 3;

// 		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 		{
// 			filter1CCT[vIndex1] = filter1CCT[vIndex1]
// 					| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) ;
// 			return 1;
// 		}

// 		aux = (filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE)) >> 56;
// 		bigDesloc = (4+iIndex1)*CCT_FINGERPRINT_SIZE;
// 		littleDesloc = (5-iIndex1)*CCT_FINGERPRINT_SIZE;
// 		if (iIndex1 == 4)
// 			filter1CCT[vIndex1] = ((filter1CCT[vIndex1] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter1CCT[vIndex1] >> bigDesloc) << bigDesloc);
// 		else
// 			filter1CCT[vIndex1] = ((filter1CCT[vIndex1] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter0CCT[vIndex1] >> bigDesloc) << bigDesloc);
// 		olderCuckoo = aux;

// 		sprintf(str, "%d", olderCuckoo);
// 		index0 = index1;
// 		index0 = HASH_FILTER1;
		
// 		vIndex0 = (uint32_t) index0/5; //vector index
// 		iIndex0 = index0%5; //bucket index inside 32 bits word

// 		if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == olderCuckoo))
// 			return 3;

// 		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 		{
// 			filter0CCT[vIndex0] = filter0CCT[vIndex0]
// 					| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) ;
// 			return 1;
// 		}
// 	}
// 	return 2;
// }