#include "cuckoo.h"

/******************************
	SIZES
******************************/
#define VEC_SIZE 256
#define TAB_SIZE 262144
#define REAL_TAB_SIZE 524288
#define VEC_OHT 128
#define VEC_OHT_TOTAL 256

/*****************************
	HASH FUNCTIONS FUNCTIONS
*****************************/ 
//NEED TO IMPLEMENT THE VECTORIZED VERSIONS OF THIS FUNCTIONS
//THE SECOND ONE MUST BE ADD BY 65535 ON THE END
#define H1 (_mm256_fnv1a_epi32(key) & (TAB_SIZE-1)
#define H2 (_mm256_murmur3_epi32(key, seed) & (TAB_SIZE-1))

#define OHT_HASH (fnv1a(olderCuckoo) & (VEC_OHT-1))
#define OHT_HASH1 (murmurhash3((const char *)&olderCuckoo, 4, 0x0D50064F7) & (VEC_OHT-1))
#define OHT_CIRCULAR (VEC_OHT-1)

/****************************
	PARAMETERS
****************************/
#define VCUCKOO_MAX_TRY 300
#define MAX_THREADS 2
#define OHT_THRESHOLD 8

/***************************
	DATA TYPES AND STRUCTURES
****************************/
alignas(32) uint32_t cuckoo[REAL_TAB_SIZE];
alignas(32) uint32_t vecOHT0[VEC_OHT];
alignas(32) uint32_t vecOHT1[VEC_OHT];

uint32_t tamOrders;
uint32_t tamCustomer;

uint32_t HOPS;			//hops for a given key
double   HOPSGENERAL;	//general hops in average
uint32_t OCC;			//occupation	
uint32_t DUP;			//duplicated keys
uint32_t SUC; 			//successfull insertion on the first try
uint32_t HOPED;		//Succesfull insertion with hops
uint32_t OHTOCC;    

/***************************
	FUNCTIONS
***************************/
int vecCuckooJoin(column_customer * c_customer, column_orders * c_orders);
int vecCuckooLookUp(__m256i key);
void vecCuckooGenerate(column_orders * c_orders);
int vecCuckooLookUpOHT(__m256i key, __m256i hash1);
int _mm256_insertOHT_epi32(__m256i keysVector, __m256i ohtMask);
__m256i _mm256_lookUpOHT(__m256i key);
