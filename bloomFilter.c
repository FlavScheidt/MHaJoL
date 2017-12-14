#include "bloomFilter.h"

// short int * generateBitVector()
int bloomFilter(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	char bloom[nBuckets];
	char str[10];
	clock_t init, end;

	init = clock();
	for (int b=0; b<nBuckets; b++)
		bloom[b] = 0;
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));


	//Generate bitmask
	init = clock();
	for(int i=0; i<tamOrders; i++)
	{
		sprintf(str, "%ld", c_orders[i].O_CUSTKEY);
		bloom[HASH_INDEX0] = 1;
		//bloom[HASH_INDEX1] = 1;
		bloom[HASH_INDEX2] = 1;
		bloom[HASH_INDEX3] = 1;
		bloom[HASH_INDEX4] = 1;
	}
	end = clock();
	printf("Bloom Filter Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));


	//Join
	init = clock();
	for (int j=0; j<tamCustomer; j++)
	{
		sprintf(str, "%d", c_customer[j].C_CUSTKEY);
		if (bloom[HASH_INDEX0] == 0 || /*bloom[HASH_INDEX1] == 0 ||*/ bloom[HASH_INDEX2] == 0 || bloom[HASH_INDEX3] == 0 || bloom[HASH_INDEX4] == 0)
		{
			t_result[index] = c_customer[j].C_ACCTBAL;
			index++;
		}
	}
	end = clock();
	printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}

int bloomFilterParam(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets, int nHash)
{
	int index = 0;
	char bloom[nBuckets];
	char str[10];
	clock_t init, end;

	init = clock();
	for (int b=0; b<nBuckets; b++)
		bloom[b] = 0;
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));


	//Generate bitmask
	init = clock();
	for(int i=0; i<tamOrders; i++)
	{
		sprintf(str, "%ld", c_orders[i].O_CUSTKEY);
		if (nHash >= 0)
			bloom[HASH_INDEX0] = 1;
		if (nHash >= 1)
			bloom[HASH_INDEX1] = 1;
		if (nHash >= 2)
			bloom[HASH_INDEX2] = 1;
		if (nHash >= 3)
			bloom[HASH_INDEX3] = 1;
		if (nHash == 4)
		bloom[HASH_INDEX4] = 1;
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
			if (bloom[HASH_INDEX0] == 0)
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
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			if (bloom[HASH_INDEX0] == 0 || bloom[HASH_INDEX1] == 0)
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
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			if (bloom[HASH_INDEX0] == 0 || bloom[HASH_INDEX1] == 0 || bloom[HASH_INDEX2] == 0)
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
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			if (bloom[HASH_INDEX0] == 0 || bloom[HASH_INDEX1] == 0 || bloom[HASH_INDEX2] == 0 || bloom[HASH_INDEX3] == 0)
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
			sprintf(str, "%d", c_customer[j].C_CUSTKEY);
			if (bloom[HASH_INDEX0] == 0 || bloom[HASH_INDEX1] == 0 || bloom[HASH_INDEX2] == 0 || bloom[HASH_INDEX3] == 0 || bloom[HASH_INDEX4] == 0)
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


int bloomNested(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	int exists = 0;
	char bloom[nBuckets];
	char str[10];
	clock_t init, end;

	init = clock();
	for (int b=0; b<nBuckets; b++)
		bloom[b] = 0;
	end = clock();

	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Generate bitmask
	init = clock();
	for(int i=0; i<tamOrders; i++)
	{
		sprintf(str, "%ld", c_orders[i].O_CUSTKEY);
		bloom[HASH_INDEX0] = 1;
		bloom[HASH_INDEX1] = 1;
		bloom[HASH_INDEX2] = 1;
		bloom[HASH_INDEX3] = 1;
		bloom[HASH_INDEX4] = 1;
	}
	end = clock();
	printf("Bloom Filter Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Join
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", c_customer[i].C_CUSTKEY);
		if (bloom[HASH_INDEX0] == 0 /*|| bloom[HASH_INDEX1] == 0*/ || bloom[HASH_INDEX2] == 0 || bloom[HASH_INDEX3] == 0 || bloom[HASH_INDEX4] == 0)
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

int bloomHash(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	int control = 0;
	int nResult=0;
	char str[10];
	clock_t init, end;

	linkedList * node;
	linkedList * buckets[HASH_BUCKETS];

	//Initialize buckets
	init = clock();
	for (int n = 0; n<HASH_BUCKETS; n++)
	{
		buckets[n] = (linkedList *) malloc (sizeof(linkedList));
		buckets[n]->C_CUSTKEY = -1;
		buckets[n]->next = NULL;
	}

	char bloom[nBuckets];
	for (int b=0; b<nBuckets; b++)
		bloom[b] = 0;
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Generates hash table
	init = clock();
	for (int i = 0; i<tamOrders; i++)
	{
		control = 0;

		sprintf(str, "%ld", c_orders[i].O_CUSTKEY);
		index = HASH_FUNC;

		bloom[HASH_INDEX0] = 1;
		//bloom[HASH_INDEX1] = 1;
		bloom[HASH_INDEX2] = 1;
		bloom[HASH_INDEX3] = 1;
		bloom[HASH_INDEX4] = 1;

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
		if (bloom[HASH_INDEX0] == 0 /*|| bloom[HASH_INDEX1] == 0 */|| bloom[HASH_INDEX2] == 0 || bloom[HASH_INDEX3] == 0 || bloom[HASH_INDEX4] == 0)
		{
			t_result[index] = c_customer[j].C_ACCTBAL;
			index++;
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

	return nResult;
}
