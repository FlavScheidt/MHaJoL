#include "cct.h"

#define CPCT_FINGERPRINT_SIZE 8 //1 byte
#define CPCT_POPCOUNTER_SIZE 24 //6 nibbles

#define CPCT_FILTER_SIZE 65536 //2^(16)/2
#define CPCT_OHT_SIZE 1024

#define CPCT_THRESHOLD 2
#define CPCT_OHT_THRESHOLD 2

#define NUM_THREADS 2

#define HASH_FINGERPRINT_CPCT fnv1a8(key)
#define HASH_FILTER0_CPCT (fnv1a(key) & ((CPCT_FILTER_SIZE)-1))
#define HASH_FILTER1_CPCT (index0 ^ (hash_jenkins(str) & ((CPCT_FILTER_SIZE)-1)))
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

typedef struct Orders 
{
	column_orders * c_orders;
	int tamOrders;
	int init;
	int cpctHT;
	int cpctOHT;
	int dupHT;
}Orders, *ptrOrders;

typedef struct BucketArray
{
	uint32_t olderKey;
	int status; //return status
}BucketArray, *ptrBucketArray;

uint16_t SUCCFILTER;
uint16_t REALOCFILTER;
uint32_t cpctOccFingerprint;
uint32_t cpctOHTFingerprint;
uint32_t dup;

uint32_t tamOrders;
uint32_t tamCustomer;

pthread_mutex_t mutexOHT0;
pthread_mutex_t mutexOHT1;

pthread_mutex_t mutexArray0;
pthread_mutex_t mutexArray1;

int cpctJoin(column_customer * c_customer, column_orders * c_orders);
int cpctInsertFilter(unsigned int key);
// uint64_t popCount(uint64_t bits);
void generateCPCT(column_orders * c_orders);
int cpctInsertConciseTable(unsigned int key);
int cpctLookUp(unsigned int key);

void * insertArrayRoutine(void * orders);
void * cpctInsertOHT(void * bucketArray);

void freeLock(uint64_t * bucket);
int getLock(uint64_t * bucket);