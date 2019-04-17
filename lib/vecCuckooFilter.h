#include "vecCuckoo.h"

#define FINGERPRINT_SIZE 8 //1 byte
#define POPCOUNTER_SIZE 24 //6 nibbles

#define FILTER_SIZE 65536 //2^(16)/2