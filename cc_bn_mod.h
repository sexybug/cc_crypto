#ifndef CC_BN_MOD_H
#define CC_BN_MOD_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// R = A mod d, d != 0
// Vertical division
cc_bn_t cc_bn_mod_word(const cc_bn_t *A, size_t A_word_len, cc_bn_t d);

// compare {carry, A} with B
int cc_bn_cmp_carry(cc_bn_t carry, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);

// R = A + d mod N. A < N, d < N
// R can alias A, R can not alias N
void cc_bn_mod_add_word(cc_bn_t *R, const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len);

// R = A + B mod N. A < N, B < N
// R can alias A, R can not alias N
void cc_bn_mod_add(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len);

// R = 2A mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_double(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len);

// R = A - d mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_sub_word(cc_bn_t *R, const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len);

// R = A - B mod N, A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_sub(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len);

// R = -A mod N, A < N
// R can alias A N
void cc_bn_mod_neg(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len);

// R = A / 2 mod N, A < N
// N must be odd
// R can alias A N
void cc_bn_mod_half(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len);

#endif // CC_BN_MOD_H