#include "cc_crypto_rng.h"
#include <stdlib.h>
#include <time.h>

// generate random bytes
static uint32_t seed32 = 0xEE79F069;
void srand32(uint32_t seed)
{
    seed32 = seed;
}
uint32_t rand32()
{
    seed32 ^= seed32 << 13;
    seed32 ^= seed32 >> 17;
    seed32 ^= seed32 << 5;

    return seed32;
}
int cc_crypto_rng_ex(void *random, size_t len)
{
    uint32_t *random_words = (uint32_t *)random;
    size_t left_words = len / 4;
    size_t right_bytes = len % 4;
    size_t i;
    for (i = 0; i < left_words; i++)
    {
        random_words[i] = rand32();
    }

    if (right_bytes > 0)
    {
        uint8_t *random_bytes = (uint8_t *)((uint8_t *)random + left_words * 4);
        uint32_t t = rand32();
        for (i = 0; i < right_bytes; i++)
        {
            random_bytes[i] = t & 0xFF;
            t >>= 8;
        }
    }

    return 0;
}