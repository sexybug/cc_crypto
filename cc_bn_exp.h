
#ifndef CC_BN_EXP_H
#define CC_BN_EXP_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// R = mont_exp(A, E) = A^E mod N, A is montgomery form
// R can not be alias for A or B
void cc_bn_mont_exp(const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R);

// r = a^(1/2) mod p,  p is prim and p = 3 (mod 4), A is montgomery form
// not every number has a square root, return CC_BN_ERR_NOT_SQUARE if not found
cc_bn_status_t cc_bn_mont_sqrt_p3(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R);

// r = a^(-1) mod p,  p is prim, A is montgomery form
// R can not be alias for A
void cc_bn_mont_inv(const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R);

#endif // CC_BN_EXP_H