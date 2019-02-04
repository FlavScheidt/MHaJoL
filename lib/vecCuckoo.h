#include "cpct.h"

/******************************
	SIZES
******************************/
#define VEC_SIZE 256
#define TAB_SIZE 65536
#define REAL_TAB_SIZE 131072
#define VEC_OHT 1024

/*****************************
	HASH FUNCTIONS FUNCTIONS
*****************************/
//NEED TO IMPLEMENT THE VECTORIZED VERSIONS OF THIS FUNCTIONS
//THE SECOND ONE MUST BE ADD BY 65535 ON THE END
#define H1 (_mm256_fnv1a_epi32(key) & (TAB_SIZE-1)
#define H2 (_mm256_murmur3_epi32(key, seed) & (TAB_SIZE-1))

/****************************
	PARAMETERS
****************************/
#define VCUCKOO_MAX_TRY 4
#define MAX_THREADS 2

/***************************
	DATA TYPES AND STRUCTURES
****************************/
alignas(32) uint32_t cuckoo[REAL_TAB_SIZE];
alignas(32) uint32_t OHT[VEC_OHT];

uint32_t tamOrders;
uint32_t tamCustomer;

uint32_t HOPS;			//hops for a given key
double   HOPSGENERAL;	//general hops in average
uint32_t OCC;			//occupation	
uint32_t DUP;			//duplicated keys
uint32_t SUC; 			//successfull insertion on the first try
uint32_t HOPED;			//Succesfull insertion with hops

typedef struct keysArray
{
     __m256i keys;
     __m256i hashed;
     __m256i mask;
}keysArray;

typedef keysArray * ptrKeysArray;

typedef struct keysArrayOHT
{
     __m256i keys;
     __m256i mask;
}keysArrayOHT;

typedef keysArrayOHT *ptrKeysArrayOHT;

pthread_mutex_t lockCuckoo;

/***************************
	FUNCTIONS
***************************/
int vecCuckooJoin(column_customer * c_customer, column_orders * c_orders);
int vecCuckooLookUp(__m256i key);
void vecCuckooGenerate(column_orders * c_orders);
void * vecCuckooInsert(void * keys);
void * vecCuckooInsertOHT(void * keys);
int vecCuckooLookUpOHT(__m256i key, __m256i hash1);
