#include "bloomFilter.h"

// short int * generateBitVector()
int bloomFilter(customer *t_customer, orders *t_orders, int tamCustomer, int tamOrders, float * t_result)
{
	int index = 0;
	int nBuckets = 4194304;
	char bloom[nBuckets];
	char str[10];

	for (int b=0; b<nBuckets; b++)
		bloom[b] = 0;

	//Generate bitmask
	for(int i=0; i<tamOrders; i++)
	{
		sprintf(str, "%ld", t_orders[i].O_CUSTKEY);
		bloom[HASH_INDEX0] = 1;
		bloom[HASH_INDEX1] = 1;
		bloom[HASH_INDEX2] = 1;
		bloom[HASH_INDEX3] = 1;
		bloom[HASH_INDEX4] = 1;
	}

	//Join
	for (int j=0; j<tamCustomer; j++)
	{
		sprintf(str, "%d", t_customer[j].C_CUSTKEY);
		if (bloom[HASH_INDEX0] == 0 || bloom[HASH_INDEX1] == 0 || bloom[HASH_INDEX2] == 0 || bloom[HASH_INDEX3] == 0 || bloom[HASH_INDEX4] == 0)
		{
			t_result[index] = t_customer[j].C_ACCTBAL;
			index++;
		}
	}
	return index;
}