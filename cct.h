#include "cuckoo.h"

#define CCT_FINGERPRINT_SIZE 8 //1 byte
#define CCT_POPCOUNTER_SIZE 24 //6 nibbles

#define CCT_FILTER_SIZE 65536 //2^(17)/2
#define CCT_OHT_SIZE 16384

#define CCT_THRESHOLD 16

#define HASH_FINGERPRINT fnv1a8(key)
#define HASH_FILTER0 (murmurhash3(str, strlen(str), 0x08BF6457) & ((CCT_FILTER_SIZE*5)-1))
#define HASH_FILTER1 (index0 ^ (murmurhash3(str, strlen(str), 0x08BF6457))) & ((CCT_FILTER_SIZE*5)-1)
#define HASH_OHT_CCT (MurmurHash2(str, strlen(str), 0x08BF6457) & (CCT_OHT_SIZE-1))


uint64_t filter0[CCT_FILTER_SIZE];
uint64_t filter1[CCT_FILTER_SIZE];
uint32_t OHT[CCT_OHT_SIZE];
uint32_t *cuckooTable0;
uint32_t *cuckooTable1;
uint32_t ctSize0;
uint32_t ctSize1;

int cctJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders);
int cctInsertFilter(unsigned int key, int tamOrders);
uint64_t popCount(uint64_t bits);
void generateCCT(column_orders * c_orders, int tamOrders);
int cctInsertConciseTable(uint32_t key, int tamOrders);