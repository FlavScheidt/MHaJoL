#include "bloomFilter.h"

//Cuckoo Filter size
#define CUCKOO_SIZE 262144

//Hash functions
#define CUCKOO_HASH hash_pearson(str) & (nBuckets-1)
#define CUCKOO_H1 MurmurHash2(str, strlen(str), tamOrders) & (nBuckets-1)
#define CUCKOO_H2 murmurhash3(str, strlen(str), tamOrders) & (nBuckets-1)
// #define CUCKOO_H3 hash_jenkins(str) & (nBuckets-1)
// #define CUCKOO_H4 fnv1a(str) & (nBuckets-1)

//Maximum number of tries before reallocating the entire table
#define CUCKOO_MAX_TRY 20

//Functions
int cuckooHash(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);
void generateCuckooTable(column_orders *c_orders, int tamOrders, int nBuckets, int * cTable1, int * cTable2);
int cuckooLookUp(unsigned int key, int index1, int index2, int * cTable1, int * cTable2);
int cuckooInsert(unsigned int key, int *cTable1, int *cTable2, int tamOrders, int nBuckets);