#include "hash.h"

uint32_t multiplicativeHash(uint32_t v)
{
    return v * UINT32_C(2654435761);
}