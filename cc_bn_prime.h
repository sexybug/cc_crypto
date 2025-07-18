
#ifndef CC_BN_PRIME_H
#define CC_BN_PRIME_H

#include "cc_bn.h"
#include "cc_bn_err.h"
#include "cc_crypto_rng.h"

int calc_miller_rabin_iterations(int bits);

// generate random R in [1, N-1]
cc_bn_status_t cc_bn_rand_range(cc_bn_t *N, size_t bn_word_len, cc_crypto_rng_f rng, cc_bn_t *R);

// refer to FIPS 186-5 B.3.1
// W > 2
cc_bn_status_t cc_bn_miller_rabin(const cc_bn_t *W, size_t bn_word_len, int iterations, cc_crypto_rng_f rng);

#endif // CC_BN_PRIME_H