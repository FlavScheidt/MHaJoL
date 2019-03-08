#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


#include "load.h"
#include "hash/hash.h"

#define HASH_BUCKETS 4194304
#define HASH_FUNC ((hash_any(str, strlen(str)) << 10) >> 10) & (HASH_BUCKETS-1)

#define HASH_INDEX0 MurmurHash2(str, strlen(str), tamOrders) & (nBuckets-1)
#define HASH_INDEX1 hash_jenkins(str) & (nBuckets-1)
#define HASH_INDEX2 fnv1a(key) & (nBuckets-1)
#define HASH_INDEX3 murmurhash3(str, strlen(str), tamOrders) & (nBuckets-1)
#define HASH_INDEX4 hash_any(str, strlen(str)) & (nBuckets-1)