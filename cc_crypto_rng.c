#include "cc_crypto_rng.h"
#include <stdlib.h>
#include <time.h>

// generate random bytes
int cc_crypto_rng_ex(void *random, size_t len)
{
    uint8_t *random_bytes = (uint8_t *)random;
    size_t i;
    srand(time(NULL));
    for (i = 0; i < len; i++)
    {
        random_bytes[i] = rand() & 0xFF;
        // random_bytes[i] = 0x01;
        // random_bytes[i] = 0xFF;
    }
    return 0;
}