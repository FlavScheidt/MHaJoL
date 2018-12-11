#include "vecCuckoo.h"

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
	printf("CHT Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	printf("%d keys, %d duplicated\n", OCC, DUP);
}