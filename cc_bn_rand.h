
#ifndef CC_BN_RAND_H
#define CC_BN_RAND_H

#include "cc_bn.h"
#include "cc_bn_err.h"
#include "cc_crypto_rng.h"

// generate a random number R in [0, 2^bits-1]
void cc_bn_rand_bits(cc_bn_t *R, size_t bits, cc_crypto_rng_f rng);

// generate a random number R in [1, 2^bits-1]
cc_bn_status_t cc_bn_rand_bits_gh0(cc_bn_t *R, size_t bits, cc_crypto_rng_f rng);

// generate a random number R in [A, B]
// you must make sure A < B
// R cannot alias A, can alias B
cc_bn_status_t cc_bn_core_rand_range(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_crypto_rng_f rng);

/**
 * @brief generate a random number R in [a, N-1]
 * @note R can alias N
 */
cc_bn_status_t cc_bn_core_rand_rangeN(cc_bn_t *R, cc_bn_t a, const cc_bn_t *N, size_t bn_word_len, cc_crypto_rng_f rng);

#endif // CC_BN_RAND_H