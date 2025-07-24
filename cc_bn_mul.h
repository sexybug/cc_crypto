
#ifndef CC_BN_MUL_H
#define CC_BN_MUL_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// R = A * d
// note: R must be at least bn_word_len + 1
// R can alias A
void cc_bn_mul_word(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d);

// R = R + A * d
// R_word_len must >= bn_word_len + 1
// R can alias A
cc_bn_status_t cc_bn_mul_word_add(cc_bn_t *R, size_t R_word_len, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d);

// R = A * B, vertical multiplication
// R_word_len = bn_word_len * 2
// R cannot alias A or B
void cc_bn_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);

// R = A * B, vertical multiplication
// R_word_len = A_word_len + B_word_len
// R cannot alias A or B
void cc_bn_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

// pre-compute montgomery multiplication RR
// N must be odd
// R = 2^(CC_BN_WORD_BITS*n), RR = 2^(CC_BN_WORD_BITS*n*2) mod N, n is N word length
// R canot alias N
void cc_bn_mont_RR(cc_bn_t *RR, const cc_bn_t *N, size_t N_word_len);

// montgomery multiplication, N must be odd
// Ni = -(N^-1) mod 2^r, r is bit length of every bn digit
cc_bn_t cc_bn_mont_Ni(const cc_bn_t *N);

// R = R + A * d
// R length = A length
// return carry
// R can alias A
cc_bn_t cc_bn_mul_word_add_ret(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d);

// D = mont_mul(A, b) = A * b * R^(-1) mod N
// A, b < N, N must be odd
// D cannot alias A
void cc_bn_mont_mul_word(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// D = mont_mul(A, B) = A * B * R^(-1) mod N
// A, b < N, N must be odd
// D cannot alias A or B
void cc_bn_mont_mul(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

#endif // CC_BN_MUL_H