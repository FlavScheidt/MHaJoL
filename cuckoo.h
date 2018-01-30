#include "bloomFilter.h"

//Cuckoo Filter Setup
#define CUCKOO_FILTER_SIZE 65536
#define CUCKOO_FINGERPRINT fnv1a(str) & (nBuckets-1)
#define CUCKOO_HASH (murmurhash3(str, strlen(str), 0x8bf12985) & (nBuckets-1))

//Cuckoo Table size
#define CUCKOO_SIZE 262144

//Hash functions
#define CUCKOO_H1 (MurmurHash2(str, strlen(str), 0x5bd1e995) & (nBuckets-1))
#define CUCKOO_H2 (murmurhash3(str, strlen(str), 0x8bf12985) & (nBuckets-1))
// #define CUCKOO_H3 hash_jenkins(str) & (nBuckets-1)
// #define CUCKOO_H4 fnv1a(str) & (nBuckets-1)

//Maximum number of tries before reallocating the entire table
#define CUCKOO_MAX_TRY 20

//Functions
int cuckooHash(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);
void generateCuckooTable(column_orders *c_orders, int tamOrders, int nBuckets, int * cTable1, int * cTable2);
int cuckooLookUp(unsigned int key, int index1, int index2, int * cTable1, int * cTable2);
int cuckooInsert(unsigned int key, int *cTable1, int *cTable2, int tamOrders, int nBuckets);

int cuckooFilterJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);
int cuckooFilterInsert(int key, uint64_t * nest, int nBuckets);
int cuckooFilterLookUp(int key, uint64_t fingerPrint, int index1, uint64_t * nest);
void cuckooFilterGeneration(column_orders *c_orders, int tamOrders, uint64_t * nest);
