#include "cuckoo.h"

//Hash functions
#define CHT_HASH (MurmurHash2(str, strlen(str), 0x5Fd1F995) & (CHT_BMP_SIZE-1))
#define CHT_OHT_HASH (MurmurHash2(str, strlen(str), 0x08BF6457) & (CHT_OHT_SIZE-1))

//Bitmap size
#define CHT_BMP_SIZE 2048

//OHT size
#define CHT_OHT_SIZE 2048

//CHT Threshold collisions
#define CHT_THRESHOLD 4

//Strutct
typedef struct CHT
{
	uint64_t bitmap[CHT_BMP_SIZE];
	bucket OHT[CHT_OHT_SIZE];
	bucket * HT;
	uint32_t tamHT;
} CHT;

//Functions
void generateCHT(column_orders *c_orders, int tamOrders, CHT * cht);
int chtInsertBitmap(unsigned int key, int tamOrders, CHT * cht);
int chtInsertConciseTable(unsigned int key, CHT * cht, int tamOrders);
int chtLookUp(unsigned int key, CHT *cht);
int chtJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets);

