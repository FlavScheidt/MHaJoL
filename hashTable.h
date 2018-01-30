#include "hashEssentials.h"

#define HASH_BUCKETS 4194304
#define HASH_FUNC ((hash_any(str, strlen(str)) << 10) >> 10) & (HASH_BUCKETS-1)

//Structs
typedef struct lList
{
	long int C_CUSTKEY;
	struct lList *next;
}linkedList;


void generateHashTable(column_orders *c_orders, int tamOrders, linkedList ** buckets);
int lookupHashTable(linkedList ** buckets, int key);