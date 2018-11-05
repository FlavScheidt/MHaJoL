#include "cct.h"

#define CPCT_FINGERPRINT_SIZE 8 //1 byte
#define CPCT_POPCOUNTER_SIZE 24 //6 nibbles

#define CPCT_FILTER_SIZE 65536 //2^(16)/2
#define CPCT_OHT_SIZE 2048

#define CPCT_THRESHOLD 2
#define CPCT_OHT_THRESHOLD 2

// #define HASH_FINGERPRINT fnv1a8(key)
// #define HASH_FILTER0 (fnv1a(key) & ((CPCT_FILTER_SIZE*5)-1))
// #define HASH_FILTER1 (index0 ^ (hash_jenkins(str) & ((CPCT_FILTER_SIZE*5)-1)))
#define HASH_OHT_CPCT0 (MurmurHash2(str, strlen(str), 0x08BF6457) & (CPCT_OHT_SIZE-1))
#define HASH_OHT_CPCT1 (murmurhash3(str, strlen(str), 0x0D50064F7) & (CPCT_OHT_SIZE-1))


uint64_t filter0[CPCT_FILTER_SIZE];
uint64_t filter1[CPCT_FILTER_SIZE];
uint32_t OHT0[CPCT_OHT_SIZE];
uint32_t OHT1[CPCT_OHT_SIZE];
uint32_t *cuckooTable0;
uint32_t *cuckooTable1;
uint32_t ctSize0;
uint32_t ctSize1;

typedef struct OHT
{
	uint32_t * OHT0;
	uint32_t * OHT1;
}OHT, *ptrOHT;

typedef struct Filter
{
	uint64_t * filter0;
	uint64_t * filter1;
}Filter, *ptrFilter;

typedef struct Customer 
{
	column_customer * c_customer;
	int tamCustomer;
}Customer, *ptrCustomer;

typedef struct Orders 
{
	column_orders * c_orders;
	int tamOrders;
	int init;
}Orders, *ptrOrders;

uint16_t SUCCFILTER;
uint16_t REALOCFILTER;
uint32_t cpctOccFingerprint;
uint32_t cpctOHTFingerprint;

pthread_mutex_t mutexFilter;

int cpctJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders);
int cpctInsertFilter(unsigned int key, int tamOrders);
// uint64_t popCount(uint64_t bits);
void generateCPCT(column_orders * c_orders, int tamOrders);
int cpctInsertConciseTable(uint32_t key, int tamOrders);
int cpctLookUp(uint32_t key);

void * initializeOHT(void * OHT);
void * initializeFilter(void * Filter);
void * insertFilterRoutine(void * orders);

void freeLock(uint64_t * bucket);
int getLock(uint64_t * bucket);