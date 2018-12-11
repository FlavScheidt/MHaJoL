#include "cpct.h"

/******************************
	SIZES
******************************/
#define VEC_SIZE 256
#define	TAB_SIZE 131072
#define BKT_NUM  8 //Depends on the VEC_SIZE

/*****************************
	HASH FUNCTIONS FUNCTIONS
*****************************/
#define H1 (fnv1a(key) & ((TAB_SIZE/BKT_NUM)-1))
#define H2 (murmurhash3(str, strlen(str), seed2) & ((TAB_SIZE/BKT_NUM)-1))

/*****************************
	AVX_FUNCTIONS
******************************/
#define LOAD _mm(VEC_SIZE)_i32gather_epi32(*base_addr, vindex, scale)
#define COMPARE _mm(VEC_SIZE)_cmpeq_epi32(bucket, comparableKeys)
#define SET _mm(VEC_SIZE)_set1_epi32(key)

/****************************
	PARAMETERS
****************************/
#define CUCKOO_MAX_TRY 200

/***************************
	DATA TYPES AND STRUCTURES
****************************/
uint32_t cuckoo[VEC_SIZE];

uint32_t tamOrders;
uint32_t tamCustomer;

uint32_t HOPS;			//hops for a given key
double   HOPSGENERAL;	//general hops in average
uint32_t OCC;			//occupation	
uint32_t DUP;			//duplicated keys
uint32_t SUC; 			//successfull insertion on the first try
uint32_t HOPED;			//Succesfull insertion with hops

int vecCuckooJoin(column_customer * c_customer, column_orders * c_orders);
int vecCuckooLookUp(uint32_t key);
void vecCuckooGenerate(column_orders * c_orders);
int vecCuckooInsert(uint32_t key);