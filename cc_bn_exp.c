
#include "cc_bn_exp.h"
#include "cc_bn_mul.h"
#include "cc_bn_config.h"

void cc_bn_exp_mont_square(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t t[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(A, A, N, bn_word_len, Ni, t);
    cc_bn_copy(R, t, bn_word_len);
}

void cc_bn_exp_mont_mul(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t t[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(A, B, N, bn_word_len, Ni, t);
    cc_bn_copy(R, t, bn_word_len);
}

// R = mont_exp(A, E) = A^E mod N, A is montgomery form
// R can not be alias for A or B
void cc_bn_mont_exp(const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    int i;
    size_t E_bit_len = cc_bn_bit_len(E, bn_word_len);
    if (E_bit_len == 0)
    {
        cc_bn_set_one(R, bn_word_len);
        return;
    }

    cc_bn_copy(R, A, bn_word_len);

    for (i = E_bit_len - 2; i >= 0; i -= 1)
    {
        // t = t^2
        cc_bn_exp_mont_square(R, N, bn_word_len, Ni, R);
        if (cc_bn_get_bit(E, i))
        {
            // t = t * A
            cc_bn_exp_mont_mul(R, A, N, bn_word_len, Ni, R);
        }
    }
}

// r = a^(1/2) mod p,  p is prim and p = 3 (mod 4), A is montgomery form
// (p - r) is also a square root of a
// not every number has a square root, return CC_BN_ERR_NOT_SQUARE if not found
cc_bn_status_t cc_bn_mont_sqrt_p3(const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    // r = a^((p + 1)/4) when p = 3 (mod 4)

    cc_bn_t t[CC_BN_MAX_WORDS];
    cc_bn_t r[CC_BN_MAX_WORDS];

    // t = (p + 1)/4
    cc_bn_add_word(P, bn_word_len, 1, t);
    cc_bn_rshift(t, bn_word_len, 2, t);

    // r = a^((p + 1)/4)
    cc_bn_mont_exp(A, t, P, bn_word_len, Ni, r);

    // check r^2 = a
    cc_bn_exp_mont_square(r, P, bn_word_len, Ni, t);
    if (cc_bn_cmp_words(t, A, bn_word_len) != 0)
    {
        return CC_BN_ERR_NOT_SQUARE;
    }

    cc_bn_copy(R, r, bn_word_len);
    return CC_BN_SUCCESS;
}

// r = a^(-1) mod p,  p is prim, A is montgomery form
// R can not be alias for A
void cc_bn_mont_inv(const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    // r = a^(p - 2) when p is prim

    // e = p - 2
    cc_bn_t E[CC_BN_MAX_WORDS];
    cc_bn_sub_word(P, bn_word_len, 2, E);

    // r = a^(p - 2)
    cc_bn_mont_exp(A, E, P, bn_word_len, Ni, R);
}