
#include "cc_bn_rand.h"
#include "cc_bn_config.h"

// at most try 100 times to generate a random number that meets the condition
#define COUNT_MAX 100

// generate a random number R in [0, 2^bits-1]
void cc_bn_rand_bits(cc_bn_word_t *R, size_t bits, cc_crypto_rng_f rng)
{
    size_t bn_word_len = cc_bn_word_len_from_bit_len(bits);
    rng(R, bn_word_len * CC_BN_WORD_BYTES);
    if ((bits % CC_BN_WORD_BITS) != 0)
    {
        R[bn_word_len - 1] &= (((cc_bn_dword_t)1 << (bits % CC_BN_WORD_BITS)) - 1);
    }
}

// generate a random number R in [1, 2^bits-1]
cc_status_t cc_bn_rand_bits_gh0(cc_bn_word_t *R, size_t bits, cc_crypto_rng_f rng)
{
    int count = 0;
    do
    {
        count += 1;
        if (count > COUNT_MAX)
        {
            return CC_ERR_BN_GEN_RAND;
        }
        cc_bn_rand_bits(R, bits, rng);
    } while (cc_bn_is_zero(R, cc_bn_word_len_from_bit_len(bits)));

    return CC_OK;
}

// generate a random number R in [A, B]
// you must make sure A < B
// R cannot alias A, can alias B
cc_status_t cc_bn_core_rand_range(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len, cc_crypto_rng_f rng)
{
    cc_bn_word_t RANGE[CC_BN_MAX_WORDS];
    int count = 0;

    cc_bn_sub_words(RANGE, B, A, bn_word_len);
    size_t RANGE_bits = cc_bn_bit_len(RANGE, bn_word_len);
    size_t RANGE_Words = (RANGE_bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
    do
    {
        count += 1;
        if (count > COUNT_MAX)
        {
            return CC_ERR_BN_GEN_RAND;
        }
        cc_bn_rand_bits(R, RANGE_bits, rng);
    } while (cc_bn_cmp_words(R, RANGE, RANGE_Words) > 0);

    cc_bn_add(R, A, bn_word_len, R, RANGE_Words);
    return CC_OK;
}

/**
 * @brief generate a random number R in [a, N-1]
 * @note R can alias N
 */
cc_status_t cc_bn_core_rand_rangeN(cc_bn_word_t *R, cc_bn_word_t a, const cc_bn_word_t *N, size_t bn_word_len, cc_crypto_rng_f rng)
{
    cc_bn_word_t RANGE[CC_BN_MAX_WORDS];
    int count = 0;

    if (a < CC_BN_WORD_MAX)
    {
        cc_bn_sub_word(RANGE, N, bn_word_len, a + 1);
    }
    else
    {
        cc_bn_sub_word(RANGE, N, bn_word_len, 1);
        cc_bn_sub_word(RANGE, N, bn_word_len, a);
    }

    size_t RANGE_bits = cc_bn_bit_len(RANGE, bn_word_len);
    size_t RANGE_Words = (RANGE_bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
    size_t i;
    for (i = RANGE_Words; i < bn_word_len; i++)
    {
        R[i] = 0;
    }
    do
    {
        count += 1;
        if (count > COUNT_MAX)
        {
            return CC_ERR_BN_GEN_RAND;
        }
        cc_bn_rand_bits(R, RANGE_bits, rng);
    } while (cc_bn_cmp_words(R, RANGE, RANGE_Words) > 0);

    cc_bn_add_word(R, R, bn_word_len, a);
    return CC_OK;
}
