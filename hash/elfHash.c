#include "hash.h"

unsigned int ElfHash (const char *s)
{
    unsigned int   h = 0, high;
    while ( *s )
    {
        h = ( h << 4 ) + *s++;
        if ( (high = h & 0xF0000000) )
            h ^= high >> 24;
        h &= ~high;
    }
    return h;
}