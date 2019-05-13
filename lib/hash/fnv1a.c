#include "hash.h"

inline uint32_t fnv1aByte(unsigned char oneByte, uint32_t hash)
{
	return (oneByte ^ hash) * PRIME;
}
// hash a short (two bytes)
inline uint32_t fnv1aShort(unsigned short twoBytes, uint32_t hash)
{
	const unsigned char* ptr = (const unsigned char*) &twoBytes;
	hash = fnv1aByte(*ptr++, hash);

	return fnv1aByte(*ptr, hash);
}
/// hash a 32 bit integer (four bytes)
inline uint32_t fnv1a(unsigned int fourBytes)
{
	uint32_t hash = SEED;

	const unsigned char* ptr = (const unsigned char*) &fourBytes;
	hash = fnv1aShort(*ptr++, hash);
	hash = fnv1aShort(*ptr++, hash);
	hash = fnv1aShort(*ptr++, hash);

	return fnv1aShort(*ptr, hash);
}
/******************************************************
	SIMD version
*******************************************************/
inline __m256i _mm256_fnv1a_epi32(__m256i data)
{
	__m256i hash = _mm256_set1_epi32(2166136261);
	__m256i prime = _mm256_set1_epi32(16777619);
	__m256i shift;

	//Extract the byte
	shift = _mm256_slli_epi32(data, 24);
	shift = _mm256_srli_epi32(shift, 24);

	//Hash
	hash = _mm256_xor_si256(hash, shift);
	hash = _mm256_mullo_epi32(hash, prime);

	//Extract the byte
	shift = _mm256_slli_epi32(data, 16);
	shift = _mm256_srli_epi32(shift, 24);

	//Hash
	hash = _mm256_xor_si256(hash, shift);
	hash = _mm256_mullo_epi32(hash, prime);

	//Extract the byte
	shift = _mm256_slli_epi32(data, 8);
	shift = _mm256_srli_epi32(shift, 24);

	//Hash
	hash = _mm256_xor_si256(hash, shift);
	hash = _mm256_mullo_epi32(hash, prime);

	//Extract the byte
	shift = _mm256_srli_epi32(data, 24);

	//Hash
	hash = _mm256_xor_si256(hash, shift);
	hash = _mm256_mullo_epi32(hash, prime);

	return hash;
}

/******************************************************
	SIMD version 512
*******************************************************/
inline __m512i _mm256_fnv1a_epi32(__m256i data)
{
	__m512i hash = _mm512_set1_epi32(2166136261);
	__m512i prime = _mm512_set1_epi32(16777619);
	__m512i shift;

	//Extract the byte
	shift = _mm512_slli_epi32(data, 24);
	shift = _mm512_srli_epi32(shift, 24);

	//Hash
	hash = _mm512_xor_si512(hash, shift);
	hash = _mm512_mullo_epi32(hash, prime);

	//Extract the byte
	shift = _mm512_slli_epi32(data, 16);
	shift = _mm512_srli_epi32(shift, 24);

	//Hash
	hash = _mm512_xor_si512(hash, shift);
	hash = _mm512_mullo_epi32(hash, prime);

	//Extract the byte
	shift = _mm512_slli_epi32(data, 8);
	shift = _mm512_srli_epi32(shift, 24);

	//Hash
	hash = _mm512_xor_si512(hash, shift);
	hash = _mm512_mullo_epi32(hash, prime);

	//Extract the byte
	shift = _mm512_srli_epi32(data, 24);

	//Hash
	hash = _mm512_xor_si512(hash, shift);
	hash = _mm512_mullo_epi32(hash, prime);

	return hash;
}

/******************************************************
	8 bits return
******************************************************/
inline uint8_t fnv1aByte8(unsigned char oneByte, uint8_t hash)
{
	return (oneByte ^ hash) * PRIME8;
}
// hash a short (two bytes)
inline uint8_t fnv1aShort8(unsigned short twoBytes, uint8_t hash)
{
	const unsigned char* ptr = (const unsigned char*) &twoBytes;
	hash = fnv1aByte8(*ptr, hash);

	return fnv1aByte8(*ptr++, hash);
}
/// hash a 32 bit integer (four bytes)
inline uint8_t fnv1a8(unsigned int fourBytes)
{
	uint8_t hash = SEED8;

	const unsigned char* ptr = (const unsigned char*) &fourBytes;

	hash = fnv1aShort8(*ptr, hash);

	return fnv1aShort8(*ptr++, hash);
}

/******************************************************
	16 bits return
******************************************************/
inline uint16_t fnv1aByte16(unsigned char oneByte, uint16_t hash)
{
	return (oneByte ^ hash) * PRIME16;
}
// hash a short (two bytes)
inline uint16_t fnv1aShort16(unsigned short twoBytes, uint16_t hash)
{
	const unsigned char* ptr = (const unsigned char*) &twoBytes;
	hash = fnv1aByte16(*ptr, hash);

	return fnv1aByte16(*ptr++, hash);
}
/// hash a 32 bit integer (four bytes)
inline uint16_t fnv1a16(unsigned int fourBytes)
{
	uint16_t hash = SEED8;

	const unsigned char* ptr = (const unsigned char*) &fourBytes;

	hash = fnv1aShort16(*ptr, hash);

	return fnv1aShort16(*ptr++, hash);
}