#include "cuckoo.h"

/******************************
	SIZES
******************************/
#define TAB_SIZE 262144
#define REAL_TAB_SIZE 524288

/****************************
	PARAMETERS
****************************/
#define VCUCKOO_MAX_TRY 100

/***************************
	DATA TYPES AND STRUCTURES
****************************/
alignas(32) uint32_t cuckoo[REAL_TAB_SIZE];

uint32_t tamOrders;
uint32_t tamCustomer;

/***************************
	FUNCTIONS
***************************/
//AVX-2
int avx2Join(column_customer * c_customer, column_orders * c_orders);
int avx2LookUp(__m256i key);
void avx2Generate(column_orders * c_orders);

//AVX-512 - 256 vector
int vivid256Join(column_customer * c_customer, column_orders * c_orders);
int vivid256LookUp(__m256i key);
void vivid256Generate(column_orders * c_orders);

//AVX-512 - 512 vector
int vivid512Join(column_customer * c_customer, column_orders * c_orders);
int vivid512LookUp(__m512i key);
void vivid512Generate(column_orders * c_orders);

