#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */
typedef  unsigned       char ub1;   /* unsigned 1-byte quantities */

#define hashsize(n) ((ub4)1<<(n))
#define hashmask(n) (hashsize(n)-1)

size_t hash_pearson(const char* s);
size_t hash_jenkins(const char* s);
ub4 one_at_a_time(char *key, ub4 len);