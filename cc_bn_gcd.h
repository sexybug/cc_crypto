
#ifndef CC_BN_GCD_H
#define CC_BN_GCD_H

#include "cc_bn.h"

// D = gcd(A, B)
// return D_word_len, D_word_len = A_word_len or B_word_len
// D can alias A B
size_t cc_bn_gcd(cc_bn_t *D, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

#endif // CC_BN_GCD_H