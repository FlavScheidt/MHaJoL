#include "hashTable.h"

#define HASH_INDEX0 ((MurmurHash2(str, strlen(str), tamOrders) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX1 ((hash_jenkins(str) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX2 ((fnv1a(str) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX3 ((murmurhash3(str, strlen(str), tamOrders) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX4 ((hash_any(str, strlen(str)) << 10) >> 10) & (nBuckets-1)

void generateBloomFilter(column_orders *c_orders, int tamOrders, int nBuckets, char * bloom);
int bloomFilter(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);
int bloomNested(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);
int bloomFilterParam(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets, int nHash);
int bloomHash(column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);