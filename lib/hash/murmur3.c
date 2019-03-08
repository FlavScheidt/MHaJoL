#include "hash.h"
/**
 * `murmurhash3.h' - murmurhash
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */


inline uint32_t murmurhash3 (const char *key, uint32_t len, uint32_t seed) {
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;
  uint32_t r1 = 15;
  uint32_t r2 = 13;
  uint32_t m = 5;
  uint32_t n = 0xe6546b64;
  uint32_t h = 0;
  uint32_t k = 0;
  uint8_t *d = (uint8_t *) key; // 32 bit extract from `key'
  const uint32_t *chunks = NULL;
  const uint8_t *tail = NULL; // tail - last 8 bytes
  int i = 0;
  int l = len / 4; // chunk length

  h = seed;

  chunks = (const uint32_t *) (d + l * 4); // body
  tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'

  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i) {
    // next 4 byte chunk of `key'
    k = chunks[i];

    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }

  k = 0;

  // remainder
  switch (len & 3) { // `len % 4'
    case 3: k ^= (tail[2] << 16);
    case 2: k ^= (tail[1] << 8);

    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
      h ^= k;
  }

  h ^= len;

  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);

  return h;
}

inline __m256i _mm256_murmur3_epi32(__m256i keys, const uint32_t seed)
{

  __m256i c1 = _mm256_set1_epi32(0xcc9e2d51);
  __m256i c2 = _mm256_set1_epi32(0x1b873593);
  const int r1 = 15;
  const int r2 = 13;
  const __m256i m = _mm256_set1_epi32(5);
  const __m256i n = _mm256_set1_epi32(0xe6546b64);
  __m256i hash = _mm256_set1_epi32(seed);
  __m256i k1 = _mm256_setzero_si256();
  __m256i k2 = _mm256_setzero_si256();

  //Multiply
  keys = _mm256_mullo_epi32(keys, c1);

  //Rotate left
  k1 = _mm256_slli_epi32(keys, r1);
  k2 = _mm256_srli_epi32(keys, (32-r1));
  keys = _mm256_or_si256(k1, k2);

  //Multiply
  keys = _mm256_mullo_epi32(keys, c2);

  //XOR
  hash = _mm256_xor_si256(hash, keys);

  //Rotate left
  k1 = _mm256_slli_epi32(hash, r2);
  k2 = _mm256_srli_epi32(hash, (32-r2));
  hash = _mm256_or_si256(k1, k2);

  //Multiply Add
  hash = _mm256_mullo_epi32(hash, m);
  hash = _mm256_add_epi32(hash, n);

  //FINAL AVALANCHE!

  c1 = _mm256_set1_epi32(0x85ebca6b);
  c2 = _mm256_set1_epi32(0xc2b2ae35);

  //Shift 16 and xor
  k1 = _mm256_srli_epi32(hash, 16);
  hash = _mm256_xor_si256(hash, k1);

  hash = _mm256_mullo_epi32(hash, c1);

  k1 = _mm256_srli_epi32(hash, 13);
  hash = _mm256_xor_si256(hash, k1);

  hash = _mm256_mullo_epi32(hash, c2);

  k1 = _mm256_srli_epi32(hash, 16);
  hash = _mm256_xor_si256(hash, k1);

  return hash;

}
