
#ifndef CC_BN_MUL_H
#define CC_BN_MUL_H

#include "cc_bn.h"

// note: bn_out must be at least bn_word_len+1
void cc_bn_mul_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = R + A * d, R must be at least bn_word_len+1
void cc_bn_mul_word_add(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = A * B, R must be at least 2*bn_word_len
// R can not be aliased to A or B
void cc_bn_mul_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R);

// R = A * B, R must be at least A_word_len + B_word_len
// R can not be aliased to A or B
void cc_bn_mul(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R);

// R2 = 2^(2*N_bit_len) mod N, R2 is at least N_word_len+1
void cc_bn_mont_R2(const cc_bn_t *N, size_t N_word_len, cc_bn_t *R2);

#endif // CC_BN_MUL_H