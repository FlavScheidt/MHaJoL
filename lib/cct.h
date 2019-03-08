#include "cuckoo.h"

#define CCT_FINGERPRINT_SIZE 8 //1 byte
#define CCT_POPCOUNTER_SIZE 24 //6 nibbles

#define CCT_FILTER_SIZE 65536 //2^(16)/2
#define CCT_OHT_SIZE 2048

#define CCT_THRESHOLD 2
#define CCT_OHT_THRESHOLD 2

#define HASH_FINGERPRINT fnv1a8(key)
#define HASH_FILTER0 (fnv1a(key) & ((CCT_FILTER_SIZE*5)-1))
#define HASH_FILTER1 (index0 ^ (hash_jenkins(str) & ((CCT_FILTER_SIZE*5)-1)))
#define HASH_OHT_CCT0 (MurmurHash2(str, strlen(str), 0x08BF6457) & (CCT_OHT_SIZE-1))
#define HASH_OHT_CCT1 (murmurhash3(str, strlen(str), 0x0D50064F7) & (CCT_OHT_SIZE-1))


uint64_t filter0CCT[CCT_FILTER_SIZE];
uint64_t filter1CCT[CCT_FILTER_SIZE];
uint32_t OHT0CCT[CCT_OHT_SIZE];
uint32_t OHT1CCT[CCT_OHT_SIZE];
uint32_t *cuckooTable0;
uint32_t *cuckooTable1;
uint32_t ctSize0;
uint32_t ctSize1;

uint16_t SUCCFILTER;
uint16_t REALOCFILTER;

int cctJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders);
int cctInsertFilter(unsigned int key, int tamOrders);
uint64_t popCount(uint64_t bits);
void generateCCT(column_orders * c_orders, int tamOrders);
int cctInsertConciseTable(uint32_t key, int tamOrders);
int cctLookUp(uint32_t key);