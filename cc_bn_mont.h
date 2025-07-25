
#ifndef CC_BN_MONT_H
#define CC_BN_MONT_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// pre-compute montgomery multiplication RR
// N must be odd
// R = 2^(CC_BN_WORD_BITS*n), RR = 2^(CC_BN_WORD_BITS*n*2) mod N, n is N word length
// R canot alias N
void cc_bn_mont_RR(cc_bn_t *RR, const cc_bn_t *N, size_t N_word_len);

// montgomery multiplication, N must be odd
// Ni = -(N^-1) mod 2^r, r is bit length of every bn digit
cc_bn_t cc_bn_mont_Ni(const cc_bn_t *N);

// D = mont_mul(A, b) = A * b * R^(-1) mod N
// A, b < N, N must be odd
// D cannot alias A
void cc_bn_core_mont_mul_word(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// D = mont_mul(A, B) = A * B * R^(-1) mod N
// A, b < N, N must be odd
// D cannot alias A or B
void cc_bn_core_mont_mul(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// D = mont_mul(A, b) = A * b * R^(-1) mod N
// A, b < N, N must be odd
// D can alias A
void cc_bn_mont_mul_word(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// D = mont_mul(A, B) = A * B * R^(-1) mod N
// A, B < N, N must be odd
// D can alias A B
void cc_bn_mont_mul(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// D = mont_mul(A, A) = A^2 * R^(-1) mod N
// A < N, N must be odd
// D can alias A
void cc_bn_mont_square(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// R = mont_exp(A, E) = A^E mod N, A R is montgomery form
// R cannot alias A E N
void cc_bn_core_mont_exp(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// R = mont_exp(A, E) = A^E mod N, A R is montgomery form
// R can alias A E N
void cc_bn_mont_exp(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// R = A^(-1) mod P,  P is prim, A is montgomery form
// R can alias A P
void cc_bn_mont_inv(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni);

// r = A^(1/2) mod P,  P is prim and P = 3 (mod 4), A is montgomery form
// (P - R) is also a square root of A
// not every number has a square root, return CC_BN_ERR_NOT_SQUARE if not found
// R can alias A P
cc_bn_status_t cc_bn_mont_sqrt_p3(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni);

// R = A*B mod N, A,B,R isn't montgomery form, A B < N
// R can alias A B N
void cc_bn_core_mod_mul_mont(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *RR, cc_bn_t Ni);

// R = A^2 mod N, A R isn't montgomery form, A < N
// R can alias A N
void cc_bn_core_mod_square_mont(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *RR, cc_bn_t Ni);

// R = A^E mod N, A R isn't montgomery form, A < N
// R can alias A E N
void cc_bn_core_mod_exp_mont(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *RR, cc_bn_t Ni);

#endif // CC_BN_MONT_H