#include "cc_naf.h"

// return naf_ki = k % w
static inline int8_t cc_naf_mod(cc_bn_digit_t k, size_t w)
{
    int8_t ki = k & (((cc_bn_digit_t)1 << w) - 1);
    if (ki > ((cc_bn_digit_t)1 << (w - 1)))
    {
        ki = (ki - ((cc_bn_digit_t)1 << w));
    }
    return ki;
}

// return naf length
// note: K is changed
size_t naf(cc_bn_digit_t *K, size_t K_word_len, size_t w, int8_t *naf)
{
    size_t i = 0;

    int8_t ki;
    while (cc_bn_is_zero(K, K_word_len) == false)
    {
        if (K[0] & 1)
        {
            ki = cc_naf_mod(K[0], w);
            naf[i] = ki;
            if (ki > 0)
            {
                cc_bn_sub_uint(K, K_word_len, ki, K);
            }
            else
            {
                cc_bn_add_uint(K, K_word_len, -ki, K);
            }
        }
        else
        {
            naf[i] = 0;
        }
        cc_bn_rshift_1(K, K_word_len, K);
        i++;
    }
    return i;
}