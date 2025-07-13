#ifndef CC_BN_MOD_H
#define CC_BN_MOD_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// R = A mod d, d != 0
// Vertical division
cc_bn_t cc_bn_mod_word(const cc_bn_t *A, size_t A_word_len, cc_bn_t d);

// Q = A / N, R = A % N
// A, N are modified in the process, N array must be at least A_word_len
// Q_word_len = A_word_len, R_word_len = N_word_len
cc_bn_status_t cc_bn_div(cc_bn_t *A, size_t A_word_len, cc_bn_t *N, size_t N_word_len,
                         cc_bn_t *Q, cc_bn_t *R);

// compare {carry, A} with B
int cc_bn_cmp_carry(cc_bn_t carry, const cc_bn_t *A, cc_bn_t *B, size_t bn_word_len);

// R = A + d mod N, A < N
void cc_bn_mod_add_word(const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

// R = A + B mod N, A < N, B < N
void cc_bn_mod_add(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

// R = 2A mod N, A < N
void cc_bn_mod_double(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

// R = A - d mod N, A < N
void cc_bn_mod_sub_word(const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

// R = A - B mod N, A < N, B < N
void cc_bn_mod_sub(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

// R = -A mod N, A < N
void cc_bn_mod_neg(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

// R = A / 2 mod N, A < N
void cc_bn_mod_half(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R);

#endif // CC_BN_MOD_H