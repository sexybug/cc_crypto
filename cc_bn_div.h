
#ifndef CC_BN_DIV_H
#define CC_BN_DIV_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// Q = A / N, R = A % N
// A, N are modified in the process, N array must be at least A_word_len
// Q_word_len = A_word_len, R_word_len = N_word_len
cc_bn_status_t cc_bn_div_unsafe(cc_bn_t *Q, cc_bn_t *R, cc_bn_t *A, size_t A_word_len, cc_bn_t *N, size_t N_word_len);

#endif // CC_BN_DIV_H