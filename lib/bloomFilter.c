#include "bloomFilter.h"

inline void generateBloomFilter(column_orders *c_orders, int tamOrders, int nBuckets, uint64_t * bloom)
{
	char str[10];
	clock_t init, end;
	unsigned int key;
	init = clock();
	uint64_t index0, index1, index2, index3, index4;
	uint64_t desloc0, desloc1, desloc2, desloc3, desloc4;
	uint64_t one = 1;

	for (int b=0; b<(nBuckets/64); b++)
		bloom[b] = 0;
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));


	//Generate bitmask
	init = clock();
	for(int i=0; i<tamOrders; i++)
	{
		sprintf(str, "%d", c_orders[i].O_CUSTKEY);
		key = c_orders[i].O_CUSTKEY;

		index0 = HASH_INDEX0;
		index1 = HASH_INDEX1;
		index2 = HASH_INDEX2;
		index3 = HASH_INDEX3;
		index4 = HASH_INDEX4;

		desloc0 = index0/64;
		desloc1 = index1/64;
		desloc2 = index2/64;
		desloc3 = index3/64;
		desloc4 = index4/64;

		bloom[desloc0] = (one << (index0-(64*desloc0))) | bloom[desloc0];
		bloom[desloc1] = (one << (index1-(64*desloc1))) | bloom[desloc1];
		bloom[desloc2] = (one << (index2-(64*desloc2))) | bloom[desloc2];
		bloom[desloc3] = (one << (index3-(64*desloc3))) | bloom[desloc3];
		bloom[desloc4] = (one << (index4-(64*desloc4))) | bloom[desloc4];
	}
	end = clock();
	printf("Bloom Filter Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
}

// short int * generateBitVector()
inline int bloomFilter(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	//char bloom[nBuckets];
	uint64_t * bloom;
	bloom = malloc(nBuckets/8);
	char str[10];
	clock_t init, end;
	unsigned int key;
	uint64_t index0, index1, index2, index3, index4;
	uint64_t desloc0, desloc1, desloc2, desloc3, desloc4;

	generateBloomFilter(c_orders, tamOrders, nBuckets, bloom);

	//Join
	init = clock();
	for (int j=0; j<tamCustomer; j++)
	{
		key = c_customer[j].C_CUSTKEY;
		sprintf(str, "%d", c_customer[j].C_CUSTKEY);

		index0 = HASH_INDEX0;
		index1 = HASH_INDEX1;
		index2 = HASH_INDEX2;
		index3 = HASH_INDEX3;
		index4 = HASH_INDEX4;

		desloc0 = index0/64;
		desloc1 = index1/64;
		desloc2 = index2/64;
		desloc3 = index3/64;
		desloc4 = index4/64;

		if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
			|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0 
			|| ((bloom[desloc2] << (63-(index2 - (64*desloc2)))) >> 63) == 0 
			|| ((bloom[desloc3] << (63-(index3 - (64*desloc3)))) >> 63) == 0
			|| ((bloom[desloc4] << (63-(index4 - (64*desloc4)))) >> 63) == 0)
		{
			t_result[index] = c_customer[j].C_ACCTBAL;
			index++;
		}
	}
	end = clock();
	printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}

inline int bloomFilterParam(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets, int nHash)
{
	int index = 0;
	uint64_t * bloom;
	bloom = malloc(nBuckets/8);
	char str[10];
	clock_t init, end;
	unsigned int key;
	uint64_t index0, index1, index2, index3, index4;
	uint64_t desloc0, desloc1, desloc2, desloc3, desloc4;
	uint64_t one = 1;

	init = clock();
	for (int b=0; b<(nBuckets/64); b++)
		bloom[b] = 0;
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));


	//Generate bitmask
	init = clock();
	for(int i=0; i<tamOrders; i++)
	{
		key = c_orders[i].O_CUSTKEY;
		sprintf(str, "%d", c_orders[i].O_CUSTKEY);

		if (nHash >= 0)
		{
			index0 = HASH_INDEX0;
			desloc0 = index0/64;
			bloom[desloc0] = (one << (index0-(64*desloc0))) | bloom[desloc0];
		}
		if (nHash >= 1)
		{
			index1 = HASH_INDEX1;
			desloc1 = index1/64;
			bloom[desloc1] = (one << (index1-(64*desloc1))) | bloom[desloc1];
		}
		if (nHash >= 2)
		{
			index2 = HASH_INDEX2;
			desloc2 = index2/64;
			bloom[desloc2] = (one << (index2-(64*desloc2))) | bloom[desloc2];
		}
		if (nHash >= 3)
		{
			index3 = HASH_INDEX3;
			desloc3 = index3/64;
			bloom[desloc3] = (one << (index3-(64*desloc3))) | bloom[desloc3];
		}
		if (nHash == 4)
		{
			index4 = HASH_INDEX4;
			desloc4 = index4/64;
			bloom[desloc4] = (one << (index4-(64*desloc4))) | bloom[desloc4];
		}
	}
	end = clock();
	printf("Bloom Filter Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Join
	if (nHash == 0)
	{
		init = clock();
		for (int j=0; j<tamCustomer; j++)
		{
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			key = c_orders[j].O_CUSTKEY;
			index0 = HASH_INDEX0;

			desloc0 = index0/64;

			if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0)
			{
				t_result[index] = c_customer[j].C_ACCTBAL;
				index++;
			}
		}
		end = clock();
		printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}
	if (nHash == 1)
	{
		init = clock();
		for (int j=0; j<tamCustomer; j++)
		{
			key = c_orders[j].O_CUSTKEY;
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			index0 = HASH_INDEX0;
			index1 = HASH_INDEX1;

			desloc0 = index0/64;
			desloc1 = index1/64;

			if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
				|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0)
			{
				t_result[index] = c_customer[j].C_ACCTBAL;
				index++;
			}
		}
		end = clock();
		printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}
	if (nHash == 2)
	{
		init = clock();
		for (int j=0; j<tamCustomer; j++)
		{
			key = c_orders[j].O_CUSTKEY;
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			index0 = HASH_INDEX0;
			index1 = HASH_INDEX1;
			index2 = HASH_INDEX2;

			desloc0 = index0/64;
			desloc1 = index1/64;
			desloc2 = index2/64;

			if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
				|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0 
				|| ((bloom[desloc2] << (63-(index2 - (64*desloc2)))) >> 63) == 0)
			{
				t_result[index] = c_customer[j].C_ACCTBAL;
				index++;
			}
		}
		end = clock();
		printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}
	if (nHash == 3)
	{
		init = clock();
		for (int j=0; j<tamCustomer; j++)
		{
			key = c_orders[j].O_CUSTKEY;
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			index0 = HASH_INDEX0;
			index1 = HASH_INDEX1;
			index2 = HASH_INDEX2;
			index3 = HASH_INDEX3;

			desloc0 = index0/64;
			desloc1 = index1/64;
			desloc2 = index2/64;
			desloc3 = index3/64;

			if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
				|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0 
				|| ((bloom[desloc2] << (63-(index2 - (64*desloc2)))) >> 63) == 0 
				|| ((bloom[desloc3] << (63-(index3 - (64*desloc3)))) >> 63) == 0)
			{
				t_result[index] = c_customer[j].C_ACCTBAL;
				index++;
			}
		}
		end = clock();
		printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}
	else if (nHash == 4)
	{
		init = clock();
		for (int j=0; j<tamCustomer; j++)
		{
			key = c_orders[j].O_CUSTKEY;
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			index0 = HASH_INDEX0;
			index1 = HASH_INDEX1;
			index2 = HASH_INDEX2;
			index3 = HASH_INDEX3;
			index4 = HASH_INDEX4;

			desloc0 = index0/64;
			desloc1 = index1/64;
			desloc2 = index2/64;
			desloc3 = index3/64;
			desloc4 = index4/64;

			if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
				|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0 
				|| ((bloom[desloc2] << (63-(index2 - (64*desloc2)))) >> 63) == 0 
				|| ((bloom[desloc3] << (63-(index3 - (64*desloc3)))) >> 63) == 0
				|| ((bloom[desloc4] << (63-(index4 - (64*desloc4)))) >> 63) == 0)
			{
				t_result[index] = c_customer[j].C_ACCTBAL;
				index++;
			}
		}
		end = clock();
		printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}

	return index;
}


inline int bloomNested(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	int exists = 0;
	uint64_t * bloom;
	bloom = malloc(nBuckets/8);
	char str[10];
	clock_t init, end;
	unsigned int key;
	uint64_t index0, index1, index2, index3, index4;
	uint64_t desloc0, desloc1, desloc2, desloc3, desloc4;

	generateBloomFilter(c_orders, tamOrders, nBuckets, bloom);

	//Join
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		key = c_orders[i].O_CUSTKEY;
		sprintf(str, "%d", c_customer[i].C_CUSTKEY);

		index0 = HASH_INDEX0;
		index1 = HASH_INDEX1;
		index2 = HASH_INDEX2;
		index3 = HASH_INDEX3;
		index4 = HASH_INDEX4;

		desloc0 = index0/64;
		desloc1 = index1/64;
		desloc2 = index2/64;
		desloc3 = index3/64;
		desloc4 = index4/64;

		if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
			|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0 
			|| ((bloom[desloc2] << (63-(index2 - (64*desloc2)))) >> 63) == 0 
			|| ((bloom[desloc3] << (63-(index3 - (64*desloc3)))) >> 63) == 0
			|| ((bloom[desloc4] << (63-(index4 - (64*desloc4)))) >> 63) == 0)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			index++;
		}
		else
		{

			for (int j=0; j<tamOrders && exists == 0; j++)
			{
				if (c_customer[i].C_CUSTKEY == c_orders[j].O_CUSTKEY)
					exists = 1;
			}
			if (!exists)
			{
				t_result[index] = c_customer[i].C_ACCTBAL;
				index++;
			}
			else
				exists = 0;
		}
	}
	end = clock();
	printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}

inline int bloomHash(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	int control = 0;
	int nResult=0;
	char str[10];
	clock_t init, end;
	unsigned int key;
	uint64_t index0, index1, index2, index3, index4;
	uint64_t desloc0, desloc1, desloc2, desloc3, desloc4;
	uint64_t one = 1;
	uint64_t * bloom;
	bloom = malloc(nBuckets/8);

	linkedList * node;

	linkedList ** buckets;
	buckets = malloc(nBuckets*sizeof(linkedList));

	//Initialize buckets
	init = clock();
	for (int n = 0; n<HASH_BUCKETS; n++)
	{
		buckets[n] = (linkedList *) malloc (sizeof(linkedList));
		buckets[n]->C_CUSTKEY = -1;
		buckets[n]->next = NULL;
	}

	for (int b=0; b<(nBuckets/64); b++)
		bloom[b] = 0;
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Generates hash table
	init = clock();
	for (int i = 0; i<tamOrders; i++)
	{
		control = 0;
		sprintf(str, "%d", c_orders[i].O_CUSTKEY);
		key = c_orders[i].O_CUSTKEY;

		index0 = HASH_INDEX0;
		index1 = HASH_INDEX1;
		index2 = HASH_INDEX2;
		index3 = HASH_INDEX3;
		index4 = HASH_INDEX4;

		desloc0 = index0/64;
		desloc1 = index1/64;
		desloc2 = index2/64;
		desloc3 = index3/64;
		desloc4 = index4/64;

		bloom[desloc0] = (one << (index0-(64*desloc0))) | bloom[desloc0];
		bloom[desloc1] = (one << (index1-(64*desloc1))) | bloom[desloc1];
		bloom[desloc2] = (one << (index2-(64*desloc2))) | bloom[desloc2];
		bloom[desloc3] = (one << (index3-(64*desloc3))) | bloom[desloc3];
		bloom[desloc4] = (one << (index4-(64*desloc4))) | bloom[desloc4];

		index = HASH_FUNC;

		node = buckets[index];
		if (node->C_CUSTKEY == -1)
		{
			node->C_CUSTKEY = c_orders[i].O_CUSTKEY;
			node->next = NULL;
		}
		else
		{
			while (node->next != NULL)
			{
				node = node->next;

				if (node->C_CUSTKEY == c_orders[i].O_CUSTKEY)
					control = 1;
			}

			if (control == 0 && node != NULL)
			{
				node->next = (linkedList *) malloc(sizeof(linkedList));
				node->next->C_CUSTKEY = c_orders[i].O_CUSTKEY;
				node->next->next = NULL;
			}
		}
	}
	end = clock();
	printf("Bloom Filter and Hash Table Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Join
	init = clock();
	for (int j=0; j<tamCustomer; j++)
	{
		sprintf(str, "%d", c_customer[j].C_CUSTKEY);
		key = c_orders[j].O_CUSTKEY;
		index0 = HASH_INDEX0;
		index1 = HASH_INDEX1;
		index2 = HASH_INDEX2;
		index3 = HASH_INDEX3;
		index4 = HASH_INDEX4;

		desloc0 = index0/64;
		desloc1 = index1/64;
		desloc2 = index2/64;
		desloc3 = index3/64;
		desloc4 = index4/64;

		if (((bloom[desloc0] << (63-(index0 - (64*desloc0)))) >> 63) == 0 
			|| ((bloom[desloc1] << (63-(index1 - (64*desloc1)))) >> 63) == 0 
			|| ((bloom[desloc2] << (63-(index2 - (64*desloc2)))) >> 63) == 0 
			|| ((bloom[desloc3] << (63-(index3 - (64*desloc3)))) >> 63) == 0
			|| ((bloom[desloc4] << (63-(index4 - (64*desloc4)))) >> 63) == 0)
		{
			t_result[nResult] = c_customer[j].C_ACCTBAL;
			nResult++;
		}
		else
		{
			index = HASH_FUNC;
		
			control = 0;
			node = buckets[index];

			if (node->C_CUSTKEY != c_customer[j].C_CUSTKEY)
			{
				while (node->next != NULL)
				{
					node = node->next;
					if (node->C_CUSTKEY == c_customer[j].C_CUSTKEY)
						control = 1;
				}

				if (control == 0)
				{
					t_result[nResult] = c_customer[j].C_ACCTBAL;
					nResult++;
				}
			}
		}
	}
	end = clock();
	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// int ocupation = 0;

	// for (int i=0; i<nBuckets; i++)
	// {
	// 	if (bloom[i] == 1)
	// 		ocupation++;
	// }

	// printf("Bloom Filter Ocupation: %d%% \n", (ocupation*100)/HASH_BUCKETS);

	return nResult;
}
