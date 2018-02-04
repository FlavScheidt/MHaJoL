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
	hash = fnv1aByte(*ptr, hash);

	return fnv1aByte(*ptr++, hash);
}
/// hash a 32 bit integer (four bytes)
inline uint8_t fnv1a8(unsigned int fourBytes)
{
	uint8_t hash = SEED8;

	const unsigned char* ptr = (const unsigned char*) &fourBytes;

	hash = fnv1aShort(*ptr, hash);

	return fnv1aShort(*ptr++, hash);
}