
#include "cc_bn_prime.h"
#include "cc_bn_mul.h"
#include "cc_bn_mod.h"
#include "cc_bn_exp.h"
#include "cc_crypto_rng.h"
#include "cc_bn_config.h"

int calc_miller_rabin_iterations(int bits)
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

// generate random R in [1, N-1]
cc_bn_status_t cc_bn_rand_range(cc_bn_t *N, size_t bn_word_len, cc_crypto_rng_f rng, cc_bn_t *R)
{
    int count = 1000; // at most try 1000 times
    size_t bits = cc_bn_bit_len(N, bn_word_len);
    size_t bn_real_word_len = (bits + CC_BN_DIGIT_BITS - 1) / CC_BN_DIGIT_BITS;

    size_t i;
    for (i = bn_real_word_len; i < bn_word_len; i++)
    {
        R[i] = 0;
    }

    do
    {
        // generate R >= 1
        do
        {
            count -= 1;
            if (count < 0)
            {
                return CC_BN_ERR_GEN_RAND;
            }
            rng(R, bn_real_word_len * CC_BN_DIGIT_BYTES);

            // random bit length <= N bit length
            if ((bits % CC_BN_DIGIT_BITS) != 0)
            {
                R[bn_real_word_len - 1] &= (((cc_bn_dword_t)1 << (bits % CC_BN_DIGIT_BITS)) - 1);
            }

        } while (cc_bn_is_zero(R, bn_word_len));

        // make R in [1, N]
        int bit_index = bits - 2;
        while ((cc_bn_cmp_words(R, N, bn_word_len) >= 0) && (bit_index >= 0))
        {
            // R bit = 0
            cc_bn_t N_bit = cc_bn_get_bit(N, bit_index);
            if (N_bit == 0)
            {
                cc_bn_set_bit(R, bit_index, 0);
            }
            bit_index -= 1;
        }
        // if bit_index < 0, now R = N,  re-generate R
    } while (cc_bn_cmp_words(R, N, bn_word_len) == 0);

    return CC_BN_SUCCESS;
}

// R = A^E mod N, R can not be aliased with A, E, N
void cc_bn_mr_exp(const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *R2, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(A, R2, N, bn_word_len, Ni, T);
    cc_bn_mont_exp(T, E, N, bn_word_len, Ni, R);
    cc_bn_mont_mul_word(R, 1, N, bn_word_len, Ni, T);
    cc_bn_copy(R, T, bn_word_len);
}

// R = A^2 mod N, R can not be aliased with A, N
void cc_bn_mr_square(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *R2, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(A, R2, N, bn_word_len, Ni, T);
    cc_bn_mont_mul(T, T, N, bn_word_len, Ni, R);
    cc_bn_mont_mul_word(R, 1, N, bn_word_len, Ni, T);
    cc_bn_copy(R, T, bn_word_len);
}

// refer to FIPS 186-5 B.3.1
// W > 2
cc_bn_status_t cc_bn_miller_rabin(const cc_bn_t *W, size_t bn_word_len, int iterations, cc_crypto_rng_f rng)
{
    int a, i, j;
    cc_bn_t W1[CC_BN_MAX_WORDS];
    cc_bn_t M[CC_BN_MAX_WORDS];
    cc_bn_t B[CC_BN_MAX_WORDS];
    cc_bn_t Z[CC_BN_MAX_WORDS];
    cc_bn_t R2[CC_BN_MAX_WORDS];

    // if w is even, it is composite
    if ((W[0] & 1) == 0)
    {
        return CC_BN_IS_COMPOSITE;
    }

    /* w1 := w - 1 */
    cc_bn_sub_word(W, bn_word_len, 1, W1);

    /* (Step 1) Calculate largest integer 'a' such that 2^a divides w-1 */
    a = 1;
    while (cc_bn_get_bit(W1, a) == 0)
    {
        a += 1;
    }

    /* (Step 2) m = (w-1) / 2^a */
    cc_bn_rshift(W1, bn_word_len, a, M);

    // calc mont R2
    cc_bn_mont_R2(W, bn_word_len, R2);

    cc_bn_t Ni = cc_bn_mont_Ni(W);

    for (i = 0; i < iterations; i++)
    {
        /* (Step 4.1-4.2) obtain a Random string of bits b where 1 < b < w-1 */
        do
        {
            // generate B in [1, W-2]
            cc_bn_status_t rand_status = cc_bn_rand_range(W1, bn_word_len, rng, B);
            if (CC_BN_ERR(rand_status))
            {
                return rand_status;
            }
            // if B = 1, re-generate B
        } while (cc_bn_cmp_word(B, bn_word_len, 1) <= 0);

        /* (Step 4.3) z = b^m mod w */
        cc_bn_mr_exp(B, M, W, bn_word_len, R2, Ni, Z);

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
            cc_bn_mr_square(Z, W, bn_word_len, R2, Ni, Z);

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
