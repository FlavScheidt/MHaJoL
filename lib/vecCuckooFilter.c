#include "vecCuckooFilter.h"

const uint64_t permTable[256] = {0x0706050403020100ull,
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

inline void cViViDGenerateFilter(column_orders * c_orders)
{

	/*******************************************
		Masks
	*******************************************/
	__mmask8 loadMask;
	__mmask8 storeMask;
	__mmask8 remotionMask;
	__mmask8 fingerprintMask; //~(loadMask)

	/*******************************************
		Vectors
	*******************************************/
	//Read only (write on load and on fingerprint generation)
	__m256i keysVector; //Vector of keys
	__m256i fingerprintVector; //Vector of fingerprints

	//Buckets and positions
	__m512i position512Vector; //Bucket
	__m512i bucket512Vector;
	__m256i bucketVector; //Bucket
	__m256i positionVector; //Position

	__m256i hopsVector;

	//Retrieved values
	__m512i valuesVector;
	__m256i fpValuesVector;

	/*******************************************
		Auxiliary masks and vectors
	*******************************************/
	//Auxiliary integers
	__m256i oneVector = _mm256_set1_epi32(1);
	__m256i zeroVector = _mm256_set1_epi32(0);

	__m256 temporaryPSVector;
	__m256 temporaryPSVector2;
	__m256 temporaryPSVector3;

	__m256i integer256Vector;
	__m512i integer512Vector;

	__m256i tableSizeVector = _mm256_set1_epi32((FILTER_SIZE/POSITIONS_PER_BUCKET)-1);
	// __m256i thresholdVector = _mm256_set1_epi32(VCUCKOO_MAX_TRY);

	__m256i temporaryVector; //Auxiliary vector used to load new keys
	__m512i temporary512Vector1;
	__m512i temporary512Vector2;
	__m128i permMask;

	__m256i permutationMask;

	/*******************************************
		Initiate vectors and masks
	*******************************************/
	loadMask 		= _cvtu32_mask8(255);
	fingerprintMask = _cvtu32_mask8(0);

	keysVector = _mm256_cmpeq_epi32(zeroVector, oneVector);
	fingerprintVector = _mm256_cmpeq_epi32(zeroVector, oneVector);

	/*******************************************
		Other Variables
	*******************************************/
	clock_t init, end;

//	int key[tamOrders];
	unsigned int shiftIndex;

	size_t tuples = 0;
	size_t index;
	int threshold = 0;

//	key[0] = 0;
//	for (unsigned int i=0; i<tamOrders;i++)
//	{
//		printf("%d %d\n", i, c_orders[i].O_CUSTKEY);
//		key[i] = c_orders[i].O_CUSTKEY;
//	}
	init = clock();
	likwid_markerStartRegion("Generation");

	while (tuples <= tamOrders-8)
	{
		threshold++;

		/******************************************
			PHASE 1 - THE LOAD
			Load the new items using the loadMask
		******************************************/
		keysVector = _mm256_mask_load_epi32 (keysVector, loadMask, &c_orders[tuples]);

		//Number of keys loaded to set the new tuples value
		index 	= _cvtmask8_u32(loadMask);
		tuples += _mm_popcnt_u64(index);

		/*******************************************
			PHASE 2 - THE HASH AND FINGERPRINT
		*******************************************/
		//Fingerprint
		temporaryVector 	= _mm256_fnv1a_epi32(keysVector); //Calculate
		integer256Vector 	= _mm256_set1_epi32(255);
		fingerprintVector 	= _mm256_mask_and_epi32 (fingerprintVector, loadMask, temporaryVector, integer256Vector); //Truncate to get the last 8-bits

		//Buckets
		temporaryVector = _mm256_murmur3_epi32(keysVector, 0x0D50064F7); //hash new keys
		bucketVector 	= _mm256_mask_and_epi32(bucketVector, loadMask, temporaryVector, tableSizeVector);

		temporaryVector = _mm256_murmur3_epi32(fingerprintVector, 0x0D50064F7); //hash old fingerprints
		temporaryVector = _mm256_and_si256(temporaryVector, tableSizeVector);
		bucketVector	= _mm256_mask_xor_epi32(bucketVector, fingerprintMask, temporaryVector, bucketVector); 

		//Positions (fingerprint%5 = fingerprint-((fingerprint/5)*5))
		integer256Vector 	= _mm256_set1_epi32(POSITIONS_PER_BUCKET-1);
		temporaryPSVector 	= _mm256_cvtepi32_ps(fingerprintVector);
		temporaryPSVector2	= _mm256_cvtepi32_ps(integer256Vector);
		temporaryPSVector3	= _mm256_div_ps(temporaryPSVector, temporaryPSVector2);
		temporaryPSVector3  = _mm256_round_ps(temporaryPSVector3, _MM_FROUND_TO_NEG_INF);

		temporaryVector 	= _mm256_cvtps_epi32(temporaryPSVector3); //fp/5
		temporaryVector 	= _mm256_mullo_epi32(temporaryVector, integer256Vector); // *5
		positionVector 		= _mm256_mask_sub_epi32(positionVector, loadMask, fingerprintVector, temporaryVector); //-fp

		/*******************************************
			PHASE 3 - THE RETRIEVAL
			Load the cuckoo filter values and check for zeros and duplicated values
		*******************************************/
		//Extract buckets
		bucket512Vector			= _mm512_cvtepi32_epi64(bucketVector);
		valuesVector 			= _mm512_i64gather_epi64(bucket512Vector, filter, 8);

		//Extract fingerprints accordly to the positions
		position512Vector		= _mm512_cvtepi32_epi64(positionVector);

		integer512Vector 		= _mm512_set1_epi64(2);
		temporary512Vector1 	= _mm512_add_epi64(position512Vector, integer512Vector);
		integer512Vector 		= _mm512_set1_epi64(8);
		temporary512Vector1 	= _mm512_mullo_epi64(integer512Vector, temporary512Vector1);

		//Shift and cast
		temporary512Vector1 	= _mm512_srlv_epi64(valuesVector, temporary512Vector1);
		fpValuesVector 			= _mm512_cvtepi64_epi32(temporary512Vector1);

		/*******************************************
			PHASE 4 - THE DUPLICATES AND THE ZEROS
			Check if the values are already there
		*******************************************/
		//Duplicates
		remotionMask = _mm256_movepi32_mask(_mm256_cmpeq_epi32(fpValuesVector, fingerprintVector));

		//Zeros
		loadMask 	= _mm256_movepi32_mask(_mm256_cmpeq_epi32(fpValuesVector, zeroVector));

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

		//Adds 1 to the hops counter
		//Set to 1 the counters of the new keys
		hopsVector = _mm256_mask_add_epi32(oneVector, loadMask, hopsVector, oneVector);

		/*******************************************
			PHASE 4 - THE POP COUNTER UPDATES
		*******************************************/
		integer512Vector 	= _mm512_set1_epi64(1);
		valuesVector 		= _mm512_add_epi64(valuesVector, integer512Vector);

		/*******************************************
			PHASE 7 - THE STORE
			Almost everyone goes to the cuckoo table... except the ones that reached the threshold, those must be stored on the OHT
		*******************************************/
		//Creates a mask to zero the buckets to be inserted
		//(POSITIONS_PER_BUCKET-1) - position
		integer512Vector		= _mm512_set1_epi64(POSITIONS_PER_BUCKET-1);
		temporary512Vector2		= _mm512_sub_epi64(integer512Vector, position512Vector);
		integer512Vector		= _mm512_set1_epi64(FINGERPRINT_SIZE);
		temporary512Vector2		= _mm512_mullo_epi64(integer512Vector, temporary512Vector2);

		// 1X64 << (POSITIONS_PER_BUCKET-1) - position
		temporary512Vector1 	= _mm512_set1_epi64(0XFFFFFFFFFFFFFFFF);
		temporary512Vector1 	= _mm512_sllv_epi64(temporary512Vector1, temporary512Vector2);

		// >> POSITIONS_PER_BUCKET
		temporary512Vector2		= _mm512_set1_epi64(POSITIONS_PER_BUCKET+1);
		temporary512Vector2		= _mm512_mullo_epi64(temporary512Vector2, integer512Vector);
		temporary512Vector1		= _mm512_srlv_epi64(temporary512Vector1, temporary512Vector2);

		// << position+2
		temporary512Vector2		= _mm512_set1_epi64(2);
		temporary512Vector2		= _mm512_add_epi64(temporary512Vector2, position512Vector);
		temporary512Vector2		= _mm512_mullo_epi64(temporary512Vector2, integer512Vector);
		temporary512Vector1		= _mm512_sllv_epi64(temporary512Vector1, temporary512Vector2);

		//Invert
		temporary512Vector2 	= _mm512_set1_epi64(0XFFFFFFFFFFFFFFFF);
		temporary512Vector1		= _mm512_andnot_si512(temporary512Vector1, temporary512Vector2);

		//Unset the bits of the bucket to be inserted
		valuesVector = _mm512_and_si512(valuesVector, temporary512Vector1);

		//Move fingerprints to the right position
		//(position+2)*8
		integer512Vector 		= _mm512_set1_epi64(POPCOUNTER_SIZE);
		temporary512Vector1 	= _mm512_add_epi64(position512Vector, integer512Vector);
		integer512Vector 		= _mm512_set1_epi64(FINGERPRINT_SIZE);
		temporary512Vector1 	= _mm512_mullo_epi64(integer512Vector, temporary512Vector1);

		//fingerprint << (position+2)*8
		temporary512Vector2		= _mm512_cvtepi32_epi64(fingerprintVector);
		temporary512Vector2		= _mm512_sllv_epi64(temporary512Vector2, temporary512Vector1);

		//Insert on the position
		valuesVector = _mm512_or_si512(temporary512Vector2, valuesVector);

		//Usar scatter para store
		_mm512_mask_i64scatter_epi64 (filter, storeMask, position512Vector, valuesVector, 8);

		/*******************************************
			PHASE 8 - THE SHUFFLE
		*******************************************/
		//Calculates shift index
		shiftIndex 			= _cvtmask8_u32(loadMask);
		permMask 			= _mm_loadl_epi64((__m128i*) &permTable[shiftIndex ^ 255]);
		permutationMask 	= _mm256_cvtepi8_epi32(permMask);

		//Shuffle keys and fingerprints
		fingerprintVector	= _mm256_permutevar8x32_epi32(fpValuesVector, permutationMask);
		keysVector 			= _mm256_permutevar8x32_epi32(keysVector, permutationMask);

		//Shuffle Masks
		temporaryVector 	= _mm256_maskz_and_epi32(loadMask, oneVector, oneVector);
		temporaryVector 	= _mm256_permutevar8x32_epi32(temporaryVector, permutationMask);
		loadMask 			= _mm256_movepi32_mask(temporaryVector);
		fingerprintMask		= _knot_mask8(loadMask);
	}
}
