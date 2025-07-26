
#ifndef CC_BN_PRIME_H
#define CC_BN_PRIME_H

#include "cc_bn.h"
#include "cc_bn_err.h"
#include "cc_crypto_rng.h"

/*
 * Small divisors test, to quickly eliminate non-primes.
 */
cc_bn_status_t cc_bn_prime_trial_division(const cc_bn_t *X, size_t bn_word_len);

int cc_bn_prime_calc_miller_rabin_iterations(int bits);

// refer to FIPS 186-5 B.3.1
// W > 2
cc_bn_status_t cc_bn_prime_miller_rabin(const cc_bn_t *W, size_t bn_word_len, int iterations, cc_crypto_rng_f rng);

// return CC_BN_ERR_INVALID_ARG if X is invalid
// return CC_BN_IS_PRIME if X is certainly prime
// return CC_BN_PROBABLY_PRIME if X is probably prime
// return CC_BN_IS_COMPOSITE if X is composite
cc_bn_status_t cc_bn_prime_check(const cc_bn_t *X, size_t bn_word_len, cc_crypto_rng_f rng);

cc_bn_status_t cc_bn_gen_prime(cc_bn_t *X, size_t bits, cc_crypto_rng_f rng);

#endif // CC_BN_PRIME_H