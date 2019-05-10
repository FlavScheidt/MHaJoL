#include "cuckoo.h"
/***************************************************
	Cuckoo Hash and Cuckoo Filter Implementations
***************************************************/
inline int cuckooInsert(int key, int *cTable1, int *cTable2, int nBuckets, int *try)
{
	char str[10];
	int index1, index2;
	unsigned int olderCuckoo;
	int aux;
	int seed1, seed2;
	HOPS = 0;

	if (try == 0)
	{
		seed1 = 0x5bd1e995;
		seed2 = 0x8bf12985;
	}
	else
	{
		seed1 = 0x08BF6457;
		seed2 = 0xF5309751;
	}

	sprintf(str, "%d", key);
	index1 = CUCKOO_H1;
	index2 = CUCKOO_H2;

	if (cuckooLookUp(key, index1, index2, cTable1, cTable2) > 0)
		return 2;

	olderCuckoo = key;

	if (cTable1[index1] == -1)
	{
		cTable1[index1] = olderCuckoo;
		return 1;
	}

	if (cTable2[index2] == -1)
	{
		cTable2[index2] = olderCuckoo;
		return 1;
	}

	aux = cTable2[index2];
	cTable2[index2] = olderCuckoo;
	olderCuckoo = aux;

	sprintf(str, "%d", olderCuckoo);
	index1 = CUCKOO_H1;

	for (int j=0; j<CUCKOO_MAX_TRY; j++)
	{
		HOPS++;
		//Table 1
		if (cTable1[index1] == -1)
		{
			cTable1[index1] = olderCuckoo;
			return 3;
		}

		aux = cTable1[index1];
		cTable1[index1] = olderCuckoo;
		olderCuckoo = aux;

		HOPS++;

		//Table 2
		sprintf(str, "%d", olderCuckoo);
		index2 = CUCKOO_H2;

		if (cTable2[index2] == -1)
		{
			cTable2[index2] = olderCuckoo;
			return 3;
		}

		aux = cTable2[index2];
		cTable2[index2] = olderCuckoo;
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index1 = CUCKOO_H1;
	}
	return olderCuckoo;
}

inline int cuckooLookUp(int key, int index1, int index2, int * cTable1, int *cTable2)
{
	if (cTable1[index1] == key || cTable2[index2] == key)
		return 1;

	return 0;
}

inline void generateCuckooTable(column_orders c_orders[TAM_ORDERS], int tamOrders, int nBuckets, int * cTable1, int * cTable2, int *try)
{
	clock_t init, end;
	init = clock();

	HOPSGENERAL = 0;
	
	// likwid_markerStartRegion("Initialization");
	for (int b=0; b<nBuckets; b++)
	{
		cTable1[b] = -1;
		cTable2[b] = -1;
	}
	// likwid_markerStopRegion("Initialization");
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	// likwid_markerStartRegion("Generation");
	for (int i=0; i<tamOrders; i++)
	{
		switch (cuckooInsert(c_orders[i].O_CUSTKEY, cTable1, cTable2, nBuckets, try))
		{
			case 1:
				SUC++;
				break;
			case 3:
				HOPSGENERAL = (HOPS+HOPSGENERAL)/2;
				HOPED++;
				break;
			case 2:
				DUP++;
				break;
			default:
				printf("hops %d\n", HOPS);
				printf("	Realocating cuckoo table... it may take a while\n");
				try++;
				generateCuckooTable(c_orders, tamOrders, nBuckets, cTable1, cTable2, try);
				// likwid_markerStopRegion("Generation");
				end = clock();
				return;
		}
	}
	// likwid_markerStopRegion("Generation");
	end = clock();
	printf("Cuckoo Table Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
}


inline int cuckooHash(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	char str[10];
	clock_t init, end;
	OCC = 0;
	DUP = 0;
	SUC = 0;
	HOPED = 0;

	int index = 0;
	int index1, index2;

	int *try = 0;
	int seed1, seed2;

	nBuckets = nBuckets/2;

	int * cTable1 = malloc((nBuckets)*sizeof(int));
	int * cTable2 = malloc((nBuckets)*sizeof(int));

	generateCuckooTable(c_orders, tamOrders, nBuckets, cTable1, cTable2, try);

	for (int j=0; j<nBuckets; j++)
	{
		if (cTable1[j] != -1)
			OCC++;
		
		if (cTable2[j] != -1)
			OCC++;
	}

	init=clock();
	// likwid_markerStartRegion("Core");
	if (try == 0)
	{
		seed1 = 0x5bd1e995;
		seed2 = 0x8bf12985;
	}
	else
	{
		seed1 = 0x08BF6457;
		seed2 = 0xF5309751;
	}

	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", c_customer[i].C_CUSTKEY);
		index1 = CUCKOO_H1;
		index2 = CUCKOO_H2;

		if (cuckooLookUp(c_customer[i].C_CUSTKEY, index1, index2, cTable1, cTable2) == 1)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			index++;
		}
	}
	// likwid_markerStopRegion("Core");
	end=clock();

	free(cTable1);
	free(cTable2);

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	printf("Cuckoo Ocupation: %.f%% \n", (double)(OCC*100)/(nBuckets*2));
	printf("Average number of hops: %.f \n", HOPSGENERAL);
	printf("Succesfull insertions on first try: %d \n", SUC);
	printf("Succesfull insertions with hops: %d \n", HOPED);
	printf("Duplicated keys: %d \n", DUP);

	return index;
}
