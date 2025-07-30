
#ifndef CC_CRYPTO_RNG_H
#define CC_CRYPTO_RNG_H

#include <stdint.h>
#include <stddef.h>

typedef int (*cc_crypto_rng_f)(void *random, size_t byte_len);

void srand32(uint32_t seed);
uint32_t rand32();

int cc_crypto_rng_ex(void *random, size_t len);

#endif // CC_CRYPTO_RNG_H