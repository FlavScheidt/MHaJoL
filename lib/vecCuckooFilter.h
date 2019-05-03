#include "vecCuckoo.h"

/******************************
	SIZES
******************************/
#define FINGERPRINT_SIZE 8 //1 byte
#define POSITIONS_PER_BUCKET 6 
#define POPCOUNTER_SIZE 2 //2 bytes

#define FILTER_SIZE 131070 // filter_size * 6 = 786420
#define FILTER_TABLE_SIZE 65535 //FILTER_SIZE/2 -> each side of the filter has this size

/****************************
	PARAMETERS
****************************/
#define THRESHOLD 20

/***************************
	DATA TYPES AND STRUCTURES
****************************/
alignas(32) uint64_t filter[FILTER_SIZE];


/***************************
	FUNCTIONS
***************************/
// int vecFilterLookUp(__m256i key);
void cViViDGenerateFilter(column_orders * c_orders);
void cViViDGenerate(column_orders * c_orders);
