// #include "cuckoo.h"

// //Hash functions
// #define CHT_HASH (multiplicativeHash(key) & ((CHT_BMP_SIZE*32)-1))
// #define CHT_OHT_HASH (MurmurHash2(str, strlen(str), 0x08BF6457) & (CHT_OHT_SIZE-1))

// //Bitmap size
// #define CHT_BMP_SIZE 131072

// //OHT size
// #define CHT_OHT_SIZE 32768

// //CHT Threshold collisions
// #define CHT_THRESHOLD 32
// //Strutct
// typedef struct CHT
// {
// 	uint64_t bitmap[CHT_BMP_SIZE];
// 	uint32_t OHT[CHT_OHT_SIZE];
// 	uint32_t *HT;
// 	uint32_t tamHT;
// } CHT;

// //Functions
// void generateCHT(column_orders *c_orders, int tamOrders, CHT * cht);
// int chtInsertBitmap(unsigned int key, int tamOrders, CHT * cht);
// int chtInsertConciseTable(unsigned int key, CHT * cht, int tamOrders);
// int chtLookUp(unsigned int key, CHT *cht);
// int chtJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, int nBuckets);
// uint64_t popCount(uint64_t bits);
// uint32_t * chtCreateConciseTable(uint32_t *ohtOcc, uint32_t *chtOcc, column_orders *c_orders, int tamOrders, CHT *cht);

