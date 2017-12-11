#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "load.h"
#include "hash_any.h"
#include "MurmurHash2.h"
#include "oat_hash.h"

//Defines
#define HASH_BUCKETS 131072
#define HASH_FUNC ((hash_any(str, strlen(str)) << 10) >> 10) & (nBuckets-1)

//Structs
typedef struct lList
{
	long int C_CUSTKEY;
	struct lList *next;
}linkedList;

int nestedLoopJoin (customer *t_customer, orders *t_orders, int tamCustomer, int tamOrders, float * t_result);
int hashJoin (customer *t_customer, orders *t_orders, int tamCustomer, int tamOrders, float * t_result);
