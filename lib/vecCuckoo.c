#include "vecCuckoo.h"

inline int vecCuckooInsert(uint32_t key)
{
	uint32_t index;
	uint16_t hps = 0;

	//Variables for the AVX funcs
	int * base_addr;
	int vIndex;
	int scale;

	//8 positions!!!!
	if (vecCuckooLookUp(key) > 0)
		return 0;

	sprintf(str, "%d", key);
	index = H1;

	do
	{
		//Load the 8 positions
		base_addr = &cuckoo[(index*8)-1];
		vIndex = ;
		scale = 1;

		//Find a empty one

		//Insert

		//No empty position? Then HOP
		hps++;
	} while (hps < VCUCKOO_MAX_TRY);

inline void vecCuckooGenerate(column_orders * c_orders)
{
	clock_t init, end;

	init = clock();
	likwid_markerStartRegion("Initialization");
	for (int i=0; i<VEC_SIZE; i++)
		cuckoo[i] = 0;
	likwid_markerStopRegion("Initialization");
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	likwid_markerStartRegion("Generation");
	for (int i=0; i<tamOrders; i++)
	{
		switch (vecCuckooInsert(c_orders[i].O_CUSTKEY))
		{
			case 0:
				printf("ERROR: Something went wrong while inserting the key %u on the Table\n", c_orders[i].O_CUSTKEY);
				break;
			case 1:
				OCC++;
				break;
			case 2:
				DUP++;
				break;
		}	
	}
	likwid_markerStopRegion("Generation");
	end = clock();
	printf("Vectorized Cuckoo Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	printf("%d keys, %d duplicated\n", OCC, DUP);
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

	init=clock();
	likwid_markerStartRegion("Core");
	for (int i=0; i<tamCustomer; i++)
	{
		if (vecCuckooLookUp(c_customer[i].C_CUSTKEY) == 0)
			index++;
	}
	likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}