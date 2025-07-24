
#ifndef CC_BN_RAND_H
#define CC_BN_RAND_H

#include "cc_bn.h"
#include "cc_bn_err.h"
#include "cc_crypto_rng.h"

/**
 * @brief generate a random number R in [1, N-1]
 * @note R cannot alias N
 */
cc_bn_status_t cc_bn_rand_range(cc_bn_t *R, const cc_bn_t *N, size_t bn_word_len, cc_crypto_rng_f rng);

#endif // CC_BN_RAND_H