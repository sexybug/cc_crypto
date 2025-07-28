
#include "cc_bn_prime.h"
#include "cc_bn_mont.h"
#include "cc_bn_mod.h"
#include "cc_bn_config.h"
#include "cc_bn_rand.h"

int cc_bn_prime_calc_miller_rabin_iterations(int bits)
{
    // openssl rounds
    // if (bits > 2048)
    //     return 128;
    // return 64;

    // mbedtls rounds
    return ((bits >= 1450)   ? 4
            : (bits >= 1150) ? 5
            : (bits >= 1000) ? 6
            : (bits >= 850)  ? 7
            : (bits >= 750)  ? 8
            : (bits >= 500)  ? 13
            : (bits >= 250)  ? 28
            : (bits >= 150)  ? 40
                             : 51);
}

// refer to FIPS 186-5 B.3.1
// W > 2
cc_status_t cc_bn_prime_miller_rabin(const cc_bn_t *W, size_t bn_word_len, int iterations, cc_crypto_rng_f rng)
{
    int a, i, j;
    cc_bn_t W1[CC_BN_MAX_WORDS];
    cc_bn_t M[CC_BN_MAX_WORDS];
    cc_bn_t B[CC_BN_MAX_WORDS];
    cc_bn_t Z[CC_BN_MAX_WORDS];
    cc_bn_t RR[CC_BN_MAX_WORDS];

    // if w is even, it is composite
    if (CC_BN_IS_EVEN(W))
    {
        return CC_BN_IS_COMPOSITE;
    }

    /* w1 := w - 1 */
    cc_bn_sub_word(W1, W, bn_word_len, 1);

    /* (Step 1) Calculate largest integer 'a' such that 2^a divides w-1 */
    a = 1;
    while (cc_bn_get_bit(W1, a) == 0)
    {
        a += 1;
    }

    /* (Step 2) m = (w-1) / 2^a */
    cc_bn_rshift(M, W1, bn_word_len, a);

    // calc mont RR
    cc_bn_mont_RR(RR, W, bn_word_len);

    cc_bn_t Ni = cc_bn_mont_Ni(W);

    for (i = 0; i < iterations; i++)
    {
        /* (Step 4.1-4.2) obtain a Random string of bits b where 1 < b < w-1 */
        // generate B in [2, w-2]
        CC_CHK(cc_bn_core_rand_rangeN(B, 2, W1, bn_word_len, rng));

        /* (Step 4.3) z = b^m mod w */
        cc_bn_core_mod_exp_mont(Z, B, M, W, bn_word_len, RR, Ni);

        /* (Step 4.4) if z = 1 or z = w-1 then go to Step 4.7 */
        if (cc_bn_cmp_word(Z, bn_word_len, 1) == 0 || cc_bn_cmp_words(Z, W1, bn_word_len) == 0)
        {
            continue;
        }
        /* (Step 4.5) for j = 1 to a-1 */
        if (a == 1)
        {
            return CC_BN_IS_COMPOSITE;
        }

        int found_w1 = 0;
        for (j = 1; j < a; j++)
        {
            /* (Step 4.5.1) z = z^2 mod w */
            cc_bn_core_mod_square_mont(Z, Z, W, bn_word_len, RR, Ni);

            /* (Step 4.5.2) if z = w-1 then go to Step 4.7 */
            if (cc_bn_cmp_words(Z, W1, bn_word_len) == 0)
            {
                found_w1 = 1; // 标记找到W-1，跳出循环
                break;
            }
            /* (Step 4.5.3) if z = 1 then go to Step 4.6 */
            if (cc_bn_cmp_word(Z, bn_word_len, 1) == 0)
            {
                return CC_BN_IS_COMPOSITE;
            }
        }
        // 内层循环结束未找到W-1 → 合数
        if (found_w1 == 0)
        {
            return CC_BN_IS_COMPOSITE;
        }
    }
    return CC_BN_PROBABLY_PRIME;
}

// return CC_ERR_BN_INVALID_ARG if X is invalid
// return CC_BN_IS_PRIME if X is certainly prime
// return CC_BN_PROBABLY_PRIME if X is probably prime
// return CC_BN_IS_COMPOSITE if X is composite
cc_status_t cc_bn_prime_check(const cc_bn_t *X, size_t bn_word_len, cc_crypto_rng_f rng)
{
    // if X = 0 or 1, it is invalid
    if (cc_bn_cmp_word(X, bn_word_len, 1) <= 0)
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    cc_status_t trial_div_ret = cc_bn_prime_trial_division(X, bn_word_len);
    if (trial_div_ret != CC_BN_PROBABLY_PRIME)
    {
        return trial_div_ret;
    }

    int iterations = cc_bn_prime_calc_miller_rabin_iterations(cc_bn_bit_len(X, bn_word_len));
    return cc_bn_prime_miller_rabin(X, bn_word_len, iterations, rng);
}

// return CC_OK if generate success
// return CC_ERR_BN_GEN_RAND if rng error
cc_status_t cc_bn_gen_prime(cc_bn_t *X, size_t bits, cc_crypto_rng_f rng)
{
    cc_status_t check_ret;
    do
    {
        cc_bn_rand_bits(X, bits, rng);
        cc_bn_set_bit(X, bits - 1, 1); // make X is n bits
        X[0] |= 1;                     // set X odd
        check_ret = cc_bn_prime_check(X, cc_bn_word_len_from_bit_len(bits), rng);
        if (check_ret == CC_ERR_BN_GEN_RAND)
        {
            return check_ret; // rng error
        }
    } while (!((check_ret == CC_BN_IS_PRIME) || (check_ret == CC_BN_PROBABLY_PRIME)));

    return CC_OK;
}