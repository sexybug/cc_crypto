#include "cc_bn_mul.h"

// note: cc_bn_digit_t must be uint32_t
// note: bn_out must be at least bn_word_len+1
void cc_bn_mul_uint(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t d, cc_bn_digit_t *bn_out)
{
    size_t i;
    cc_bn_digit_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        uint64_t t = (uint64_t)bn_in[i] * (uint64_t)d + (uint64_t)carry;
        carry = t >> CC_BN_DIGIT_BITS;
        bn_out[i] = t & CC_BN_DIGIT_MAX;
    }
    bn_out[i] = carry; // store carry in the last digit
}