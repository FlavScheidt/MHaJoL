#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "join.h"

#define HASH_INDEX0 ((MurmurHash2(str, strlen(str), tamOrders) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX1 ((hash_any(str, strlen(str)) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX2 ((one_at_a_time(str, strlen(str)) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX3 ((hash_jenkins(str) << 10) >> 10) & (nBuckets-1)
#define HASH_INDEX4 ((hash_pearson(str) << 10) >> 10) & (nBuckets-1)

int bloomFilter(customer *t_customer, orders *t_orders, int tamCustomer, int tamOrders, float * t_result);