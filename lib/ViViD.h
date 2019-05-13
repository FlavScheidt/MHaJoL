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

//AVX-512 Polychroniou
#ifndef _RAND_H_
#define _RAND_H_
typedef struct rand_state_32 rand32_t;
typedef struct rand_state_64 rand64_t;
rand32_t *rand32_init(uint32_t seed);
rand64_t *rand64_init(uint64_t seed);
uint32_t rand32_next(rand32_t *state);
uint64_t rand64_next(rand64_t *state);
#endif
size_t min(size_t x, size_t y) { return x < y ? x : y; }

typedef struct rand_state_64 {
	uint64_t num[313];
	size_t index;
} rand64_t;

typedef struct rand_state_32 {
	uint32_t num[625];
	size_t index;
} rand32_t;

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

//AVX-512 Ross & Polychroniou
int avx512Join(column_customer * c_customer, column_orders * c_orders);
int avx512Generate(column_orders * c_orders,  uint32_t factor[2])
