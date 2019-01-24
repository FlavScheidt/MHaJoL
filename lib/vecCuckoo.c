#include "vecCuckoo.h"

inline __m256i _mm256_mod_ps2(const __m256i& a, const __m256i& aDiv){
    __m256 c = _mm256_div_ps(a,aDiv);
    __m256i i = _mm256_cvttps_epi32(c);
    __m256 cTrunc = _mm256_cvtepi32_ps(i);
    __m256 base = _mm256_mul_ps(cTrunc, aDiv);
    __m256 r = _mm256_sub_ps(a, base);
    return r;
}

inline void vecCuckooGenerate(column_orders * c_orders)
{
	//Vectors and masks
	__m256i loadMask; //Mask used to load new keys
	__m256i keysVector; //Vector of keys
	__m256i newKeysVector; //Auxiliary vector used to load new keys
	__m256i permutationMask; //Mask to shuffle new and old keys
	__m256i hashedVector; //Hashed keys
	__m256i table1Mask; //Keys to be inserted on table1
	__m256i table2Mask; //Keys to be inserted on table2
	__m256i duplicateMask; //Mask to set duplicated keys

	//Auxiliary Masks
	__m256i mask_1 = _mm256_set1_epi32(1);
	__m256i mask_0 = _mm256_set1_epi32(0);

	__m128i permMask;

	__m256i tableSizeVector = _mm256_set1_epi32(TAB_SIZE-1);

	//Initiate vectors and masks
	loadMask, keysVector, newKeysVector, permutationMask, hashedVector, table1Mask, table2Mask = _mm256_cmpeq_epi32(mask_1, mask_1);

	//Performance counter variables
	clock_t init, end;

	//Other variables
	size_t tuples = 0;
	size_t shiftIndex;

	//Start Occupation Mask
	do 
	{
		//Load the new items using the loadMask
		newKeysVector = _mm256_maskload_epi32(&c_orders[i]->O_CUSTKEY, loadMask);

		//Remove old items
		keysVector = _mm256_andnot_si256(loadMask, keysVector);
		table2Mask = _mm256_andnot_si256(loadMask, keysVector); //Every new key is inserted on table1, so they are not listed here

		//Primary vector of keys receives new keys and table1 vector is set
		keysVector = _mm256_or_si256(keysVector, newKeysVector);
		table1Mask = _mm256_or_si256(table1Mask, mask_1);

		//Hash the keys
		hashedVector = _mm256_and_si256(keysVector, table1Mask);
		hashedVector = _mm256_fnv1a_epi32(hashedVector);
		hashedVector = _mm256_and_si256(hashedVector, tableSizeVector);

		newKeysVector = _mm256_and_si256(keysVector, table2Mask);
		newKeysVector = _mm256_murmur3_epi32(newKeysVector, 0x0D50064F7);
		newKeysVector = _mm256_and_si256(newKeysVector, tableSizeVector);
		newKeysVector = _mm256_add_epi32(newKeysVector, tableSizeVector);

		hashedVector = _mm256_or_si256(hashedVector, newKeysVector);

		//Load the cuckoo table values and check for duplicated values
		newKeysVector = _mm256_i32gather_epi32(cuckoo, hashedVector, 4);
		duplicateMask = _mm256_cmpeq_epi32(keysVector, newKeysVector);

		//Remove duplicates
		duplicateMask = _mm256_andnot_si256(duplicateMask, mask_1);

		//Check for zeros and generate mask removing duplicates
		loadMask = _mm256_cmpeq_epi32(newKeysVector, mask_0);
		loadMask = _mm256_and_si256(loadMask, duplicateMask);
		loadMask = _mm256_andnot_si256(loadMask, mask_1);

		table1Mask = _mm256_and_si256(table1Mask, loadMask);
		table1Mask = _mm256_and_si256(table1Mask, duplicateMask);
		table1Mask = _mm256_andnot_si256(table1Mask, mask_1);

		table2Mask = _mm256_and_si256(table1Mask, loadMask);
		table2Mask = _mm256_and_si256(table2Mask, duplicateMask);
		table2Mask = _mm256_andnot_si256(table2Mask, mask_1);

		//IMPLEMENT ROUTINE TO INSERT ON BOTH TABLES USING THREADS
		
		

		//Invert values on the tables mask's
		table1Mask = _mm256_andnot_si256(table1Mask, mask_1);
		table2Mask = _mm256_andnot_si256(table2Mask, mask_1);

		//Move new keys to keys vector
		keysVector = _mm256_or_si256(keysVector, newKeysVector);

		//Shuffle vector
		shiftIndex = _mm256_movemask_ps(_mm256_castsi256_ps(inv));
		permMask = _mm_loadl_epi64((__m128i*) &perm[m ^ 255]);
		permutationMask = mm256_cvtepi8_epi32(permMask);
		loadMask = _mm256_permutevar8x32_epi32(loadMask, permutationMask);
		keysVector = _mm256_permutevar8x32_epi32(keysVector, permutationMask);
		table1Mask = _mm256_permutevar8x32_epi32(table1Mask, permutationMask);
		table2Mask = _mm256_permutevar8x32_epi32(table2Mask, permutationMask);
	} while (tuples <= tamOrders-8)
	
}

int vecCuckooJoin(column_customer * c_customer, column_orders * c_orders)
{
	clock_t init, end;
	uint32_t index=0;

	HOPS = 0;			//hops for a given key
	HOPSGENERAL = 0;	//general hops in average
	OCC = 0;			//occupation	
	DUP = 0;			//duplicated keys
	SUC = 0; 			//successfull insertion on the first try
	HOPED = 0;			//Succesfull insertion with hops

	vecCuckooGenerate(c_orders, tamOrders);

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

    return index; }
