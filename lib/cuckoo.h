#include "hashEssentials.h"

//Cuckoo Table size
#define CUCKOO_SIZE 524288

//Hash functions
#define CUCKOO_H1 (murmurhash2(str, strlen(str), seed1) & (nBuckets-1))
#define CUCKOO_H2 (murmurhash3(str, strlen(str), seed2) & (nBuckets-1))
// #define CUCKOO_H3 hash_jenkins(str) & (nBuckets-1)
// #define CUCKOO_H4 fnv1a(str) & (nBuckets-1)

//Maximum number of tries before reallocating the entire table
#define CUCKOO_MAX_TRY 100

//Definition
typedef uint64_t bucket;

//Stats
uint32_t HOPS;			//hops for a given key
double   HOPSGENERAL;	//general hops in average
uint32_t OCC;			//occupation	
uint32_t DUP;			//duplicated keys
uint32_t SUC; 			//successfull insertion on the first try
uint32_t HOPED;			//Succesfull insertion with hops

//Functions
int cuckooHash(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);
void generateCuckooTable(column_orders *c_orders, int tamOrders, int nBuckets, int * cTable1, int * cTable2, int * try);
int cuckooLookUp(int key, int index1, int index2, int * cTable1, int * cTable2);
int cuckooInsert(int key, int *cTable1, int *cTable2, int nBuckets, int * try);
