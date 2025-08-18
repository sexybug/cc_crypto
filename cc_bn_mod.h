#ifndef CC_BN_MOD_H
#define CC_BN_MOD_H

#include "cc_bn.h"
#include "cc_err.h"

// R = A mod d, d != 0
// Vertical division
cc_bn_word_t cc_bn_mod_word(const cc_bn_word_t *A, size_t A_word_len, cc_bn_word_t d);

// compare {carry, A} with B
int cc_bn_cmp_carry(cc_bn_word_t carry, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

// R = A + d mod N. A < N, d < N
// R can alias A, R can not alias N
void cc_bn_mod_add_word(cc_bn_word_t *R, const cc_bn_word_t *A, cc_bn_word_t d, const cc_bn_word_t *N, size_t bn_word_len);

// R = A + B mod N. A < N, B < N
// R can alias A, R can not alias N
void cc_bn_mod_add(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *N, size_t bn_word_len);

// R = 2A mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_double(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *N, size_t bn_word_len);

// R = A - d mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_sub_word(cc_bn_word_t *R, const cc_bn_word_t *A, cc_bn_word_t d, const cc_bn_word_t *N, size_t bn_word_len);

// R = A - B mod N, A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_sub(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *N, size_t bn_word_len);

// R = -A mod N, A < N
// R can alias A N
void cc_bn_mod_neg(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *N, size_t bn_word_len);

// R = A / 2 mod N, A < N
// N must be odd
// R can alias A N
void cc_bn_mod_half(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *N, size_t bn_word_len);

// Q = A / N, R = A % N
// A, N are modified in the process
// N array must be at least A_word_len
// Q_word_len = A_word_len, R_word_len = N_word_len
// Q cannot alias A N
// R can alias A N
cc_status_t cc_bn_core_div(cc_bn_word_t *Q, cc_bn_word_t *R, cc_bn_word_t *A, size_t A_word_len, cc_bn_word_t *N, size_t N_word_len);

// R = A mod N, R length = N length
// R can alias A N
cc_status_t cc_bn_mod(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *N, size_t N_word_len);

// R = A * B mod N
// R_word_len = bn_word_len
// R can alias A B N
void cc_bn_mod_mul_words(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *N, size_t bn_word_len);

// R = A * B mod N
// R_word_len = N_word_len
// R can alias A B N
void cc_bn_mod_mul(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len, const cc_bn_word_t *N, size_t N_word_len);

// R = A^E mod N
// R can alias A, cannot alias E N
void cc_bn_core_mod_exp(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *E, size_t E_word_len, const cc_bn_word_t *N, size_t N_word_len);

// R = A^E mod N
// R can alias A E N
void cc_bn_mod_exp(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *E, size_t E_word_len, const cc_bn_word_t *N, size_t N_word_len);

// D = gcd(A, B)
// return D_word_len, D_word_len = min(A_word_len, B_word_len) if A,B != 0
// D can alias A B
size_t cc_bn_gcd(cc_bn_word_t *D, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len);

// assume: a > b and a,b not all even
// g = x*a - y*b
// X Y word_len at least bn_word_len + 1
// G X Y cannot alias A B
cc_status_t cc_bn_core_binary_exgcd_unsafe(cc_bn_word_t *G, cc_bn_word_t *X, cc_bn_word_t *Y, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

// assume: a > b
// a b can all even
// g = x*a - y*b
// X Y word_len at least bn_word_len + 1
// G X Y cannot alias A B
cc_status_t cc_bn_binary_exgcd_unsafe(cc_bn_word_t *G, cc_bn_word_t *X, cc_bn_word_t *Y, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

// R = A^-1 mod N, R_word_len = N_word_len
// R can alias A N
cc_status_t cc_bn_exgcd_mod_inv(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *N, size_t N_word_len);

#endif // CC_BN_MOD_H