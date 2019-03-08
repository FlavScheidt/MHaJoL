#include "hash.h"

inline size_t hash_jenkins(const char* s)
{
   size_t hash = 0;

   while (*s != '\0')
   {
      hash += *s++;
      hash += (hash << 10);
      hash ^= (hash >> 6);
   }

   hash += (hash << 3);
   hash ^= (hash >> 11);
   hash += (hash << 15);

   return hash;
}