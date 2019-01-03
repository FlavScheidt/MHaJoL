#include "vecCuckoo.h"

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

	//Auxiliary Masks
	__m256i mask_1 = _mm256_set1_epi32(1);
	__m256i mask_0 = _mm256_set1_epi32(0);

	//Initiate vectors and masks
	loadMask, keysVector, newKeysVector, permutationMask, hashedVector, table1Mask, table2Mask = _mm256_cmpeq_epi32(mask_1, mask_1);

	//Performance counter variables
	clock_t init, end;

	//Other variables
	size_t tuples = 0;

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
		hashedVector = H1; //First hash using mask with function1
		hashedVector = H2; //Second hash using mask with function2

		//Load the cuckoo table values - Check for zeros
		newKeysVector = _mm256_i32gather_epi32(cuckoo, hashedVector, 4);





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
