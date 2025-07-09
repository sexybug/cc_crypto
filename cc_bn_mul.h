
#ifndef CC_BN_MUL_H
#define CC_BN_MUL_H

#include "cc_bn.h"

// note: cc_bn_digit_t must be uint32_t
// note: bn_out must be at least bn_word_len+1
void cc_bn_mul_uint(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t d, cc_bn_digit_t *bn_out);

#endif // CC_BN_MUL_H