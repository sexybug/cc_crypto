#ifndef CC_BASIS_CONVERT_H
#define CC_BASIS_CONVERT_H

#include "cc_bn.h"

void cc_basis_convert(const cc_bn_digit_t *src, size_t bn_word_len, const cc_bn_digit_t *maps, cc_bn_digit_t *dst);

#endif // CC_BASIS_CONVERT_H