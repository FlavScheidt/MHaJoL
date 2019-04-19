#include "vecCuckoo.h"

/******************************
	SIZES
******************************/
#define FINGERPRINT_SIZE 8 //1 byte
#define POPCOUNTER_SIZE 24 //6 nibbles

#define FILTER_SIZE 65536 //2^(16)/2
#define FILTER_TABLE_SIZE 32768 //FILTER_SIZE/2

/****************************
	PARAMETERS
****************************/
#define THRESHOLD 2

/***************************
	DATA TYPES AND STRUCTURES
****************************/
alignas(32) uint32_t filter[FILTER_SIZE];

uint32_t tamOrders;
uint32_t tamCustomer;

/***************************
	FUNCTIONS
***************************/
// int vecFilterLookUp(__m256i key);
void cViViDGenerateFilter(column_orders * c_orders);