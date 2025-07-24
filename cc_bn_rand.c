
#include "cc_bn_rand.h"

/**
 * @brief generate a random number R in [1, N-1]
 * @note R cannot alias N
 */
cc_bn_status_t cc_bn_rand_range(cc_bn_t *R, const cc_bn_t *N, size_t bn_word_len, cc_crypto_rng_f rng)
{
    int count = 1000; // at most try 1000 times
    size_t bits = cc_bn_bit_len(N, bn_word_len);
    size_t bn_real_word_len = (bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;

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
            rng(R, bn_real_word_len * CC_BN_WORD_BYTES);

            // random bit length <= N bit length
            if ((bits % CC_BN_WORD_BITS) != 0)
            {
                R[bn_real_word_len - 1] &= (((cc_bn_dword_t)1 << (bits % CC_BN_WORD_BITS)) - 1);
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
