
#ifndef CC_BN_MUL_H
#define CC_BN_MUL_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// R = A * d
// note: R must be at least A_word_len + 1
// R can alias A
// return R_word_len = A_word_len + 1
size_t cc_bn_mul_word(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, cc_bn_t d);

// R = R + A * d
// R_word_len must >= A_word_len + 1
// R can alias A
cc_bn_status_t cc_bn_mul_word_add(cc_bn_t *R, size_t R_word_len, const cc_bn_t *A, size_t A_word_len, cc_bn_t d);

// R = A * B, vertical multiplication
// R_word_len = bn_word_len * 2
// R cannot alias A or B
void cc_bn_core_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);

// R = A * B, vertical multiplication
// R_word_len = A_word_len + B_word_len
// R cannot alias A or B
void cc_bn_core_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

// R = A * B, vertical multiplication
// R_word_len = bn_word_len * 2
// R can alias A B
void cc_bn_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);

// R = A * B, vertical multiplication
// R_word_len = A_word_len + B_word_len
// R can alias A B
void cc_bn_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

#endif // CC_BN_MUL_H