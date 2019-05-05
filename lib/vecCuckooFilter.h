#include "vecCuckoo.h"

/******************************
	SIZES
******************************/
#define FINGERPRINT_SIZE 8 //1 byte
#define POSITIONS_PER_BUCKET 6 
#define POPCOUNTER_SIZE 2 //2 bytes

#define FILTER_SIZE 131070 // filter_size * 6 = 786420

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
void cViViDGenerateFilter(int orders[TAM_ORDERS]);
void cViViDGenerate(int orders[TAM_ORDERS]);
