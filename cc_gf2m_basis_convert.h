#ifndef CC_GF2M_BASIS_CONVERT_H
#define CC_GF2M_BASIS_CONVERT_H

#include "cc_bn.h"

// dst cannot alias src
void cc_basis_convert(cc_bn_t *dst, const cc_bn_t *src, size_t bn_word_len, const cc_bn_t *maps);

#endif // CC_GF2M_BASIS_CONVERT_H