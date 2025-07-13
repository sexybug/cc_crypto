
#ifndef CC_BN_MUL_H
#define CC_BN_MUL_H

#include "cc_bn.h"

// note: bn_out must be at least bn_word_len+1
void cc_bn_mul_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = R + A * d
// R_word_len = bn_word_len + 1, R[bn_word_len] may need to be zero
void cc_bn_mul_word_add(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = A * B, R must be at least 2*bn_word_len
// R can not be alias for A or B
void cc_bn_mul_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R);

// R = A * B, R must be at least A_word_len + B_word_len
// R can not be alias for A or B
void cc_bn_mul(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R);

// montgomery multiplication, N must be odd
// R2 = 2^(2*N_bit_len) mod N
void cc_bn_mont_R2(const cc_bn_t *N, size_t N_word_len, cc_bn_t *R2);

// montgomery multiplication, N must be odd
// Ni = -(N^-1) mod 2^r, r is bit length of every bn digit
cc_bn_t cc_bn_mont_Ni(const cc_bn_t *N);

// R = R + A * d
// return carry
cc_bn_t cc_bn_mul_word_add_ret(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = mont_mul(A, b) = A * b * r^(-1) mod N
// A, b < N, N must be odd
// R can not be alias for A
void cc_bn_mont_mul_word(const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R);

// R = mont_mul(A, B) = A * B * r^(-1) mod N
// A, B < N, N must be odd
// R can not be alias for A or B
void cc_bn_mont_mul(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R);

#endif // CC_BN_MUL_H