#ifndef MURMUR_H
#define MURMUR_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define MURMUR_M32 (uint32_t)0x5bd1e995
#define MURMUR_R32 (int)24

#define MURMUR_DEFAULT_SEED 0x9747b28c

size_t murmur_hash_32(const void *key, int length, const uint32_t seed)
{
    /* Initialize the hash to a 'random' value */

    uint32_t h = seed ^ length;

    /* Mix 4 bytes at a time into the hash */

    const unsigned char *data = (const unsigned char *)key;

    while (length >= 4)
    {
        uint32_t k = *(uint32_t *)data;

        k *= MURMUR_M32;
        k ^= k >> MURMUR_R32;
        k *= MURMUR_M32;

        h *= MURMUR_M32;
        h ^= k;

        data += 4;
        length -= 4;
    }

    /* Handle the last few bytes of the input array  */

    switch (length)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= MURMUR_M32;
    };

    /* Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.  */

    h ^= h >> 13;
    h *= MURMUR_M32;
    h ^= h >> 15;

    return h;
}

#endif // MURMUR_H