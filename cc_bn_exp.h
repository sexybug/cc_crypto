
#ifndef CC_BN_EXP_H
#define CC_BN_EXP_H

#include "cc_bn.h"
#include "cc_bn_err.h"

// R = A^E mod N
// R can alias A E N
void cc_bn_mod_exp(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *E, size_t E_word_len,
                   const cc_bn_t *N, size_t N_word_len);

// R = mont_exp(A, E) = A^E mod N, A R is montgomery form
// R cannot alias A E N
void cc_bn_mont_exp(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni);

// r = a^(1/2) mod p,  p is prim and p = 3 (mod 4), A is montgomery form
// (p - r) is also a square root of a
// not every number has a square root, return CC_BN_ERR_NOT_SQUARE if not found
// R can alias A P
cc_bn_status_t cc_bn_mont_sqrt_p3(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni);

// r = a^(-1) mod p,  p is prim, A is montgomery form
// R cannot alias A P
void cc_bn_mont_inv(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni);

#endif // CC_BN_EXP_H