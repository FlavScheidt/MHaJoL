#include "hashTable.h"

int lookupHashTable(linkedList ** buckets, int key)
{
	char str[10];

	sprintf(str, "%d", key);
	int index = HASH_FUNC;
	
	linkedList * node = buckets[index];

	if (node->C_CUSTKEY != key)
	{
		while (node->next != NULL)
		{
			node = node->next;
			if (node->C_CUSTKEY == key)
				return 1;
		}
	}

	return 0;
}

void generateHashTable(column_orders *c_orders, int tamOrders, linkedList ** buckets)
{
	int index = 0;
	clock_t init, end;
	int control = 0;
	char str[10];

	linkedList * node;

	//Generates hash table
	init = clock();
	for (int i = 0; i<tamOrders; i++)
	{

		if (lookupHashTable(buckets, c_orders[i].O_CUSTKEY) == 0)
		{
			control = 0;

			sprintf(str, "%d", c_orders[i].O_CUSTKEY);
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
	}
	end = clock();

	printf("Hash Table Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
}
