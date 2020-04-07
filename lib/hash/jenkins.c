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

inline __m256i _mm256_jenkins_epi32(__m256i data)
{
   __m256i hash = _mm256_setzero_si256();
   __m256i shift;

   //Extract the byte
   shift = _mm256_slli_epi32(data, 24);
   shift = _mm256_srli_epi32(shift, 24);

   //Hash
   hash  = _mm256_xor_si256(hash, shift);
   shift = _mm256_slli_epi32(hash, 10);
   hash  = _mm256_add_epi32(shift, hash);
   shift = _mm256_srli_epi32(hash, 6);
   hash  = _mm256_xor_si256(shift, hash);

   //Extract the byte
   shift = _mm256_slli_epi32(data, 16);
   shift = _mm256_srli_epi32(shift, 24);

   //Hash
   hash  = _mm256_xor_si256(hash, shift);
   shift = _mm256_slli_epi32(hash, 10);
   hash  = _mm256_add_epi32(shift, hash);
   shift = _mm256_srli_epi32(hash, 6);
   hash  = _mm256_xor_si256(shift, hash);

   //Extract the byte
   shift = _mm256_slli_epi32(data, 8);
   shift = _mm256_srli_epi32(shift, 24);

   //Hash
   hash  = _mm256_xor_si256(hash, shift);
   shift = _mm256_slli_epi32(hash, 10);
   hash  = _mm256_add_epi32(shift, hash);
   shift = _mm256_srli_epi32(hash, 6);
   hash  = _mm256_xor_si256(shift, hash);

   //Extract the byte
   shift = _mm256_srli_epi32(data, 24);

   //Hash
   hash  = _mm256_xor_si256(hash, shift);
   shift = _mm256_slli_epi32(hash, 10);
   hash  = _mm256_add_epi32(shift, hash);
   shift = _mm256_srli_epi32(hash, 6);
   hash  = _mm256_xor_si256(shift, hash);

   //Mix
   shift = _mm256_slli_epi32(hash, 3);
   hash  = _mm256_add_epi32(hash, shift);

   shift = _mm256_srli_epi32(hash, 11);
   hash  = _mm256_xor_si256(hash, shift);

   shift = _mm256_slli_epi32(hash, 15);
   hash  = _mm256_add_epi32(hash, shift);
   
   return hash;
}