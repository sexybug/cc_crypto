
#include "cc_bn_exp.h"
#include "cc_bn_mul.h"
#include "cc_bn_mod.h"
#include "cc_bn_config.h"

cc_bn_status_t cc_bn_mod(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *N, size_t N_word_len, cc_bn_t *R)
{
    cc_bn_t A_tmp[CC_BN_MAX_WORDS];
    cc_bn_t N_tmp[CC_BN_MAX_WORDS];
    cc_bn_t Q_tmp[CC_BN_MAX_WORDS];
    cc_bn_t R_tmp[CC_BN_MAX_WORDS];

    cc_bn_copy(A_tmp, A, A_word_len);
    cc_bn_copy(N_tmp, N, N_word_len);
    cc_bn_status_t div_status = cc_bn_div(A_tmp, A_word_len, N_tmp, N_word_len, Q_tmp, R_tmp);
    if (CC_BN_ERR(div_status))
    {
        return div_status;
    }
    cc_bn_copy(R, R_tmp, N_word_len);
}

void cc_bn_mod_exp_square(const cc_bn_t *A, const cc_bn_t *N, size_t N_word_len, cc_bn_t *R)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_mul_words(A, A, N_word_len, T);
    cc_bn_copy(R, T, N_word_len * 2);
}
void cc_bn_mod_exp_mul(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t N_word_len, cc_bn_t *R)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_mul_words(A, B, N_word_len, T);
    cc_bn_copy(R, T, N_word_len * 2);
}

// R = A^E mod N
// R can be alias for A or E or N
void cc_bn_mod_exp(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *E, size_t E_word_len,
                   const cc_bn_t *N, size_t N_word_len, cc_bn_t *R)
{
    int i;
    cc_bn_t A_tmp[CC_BN_MAX_WORDS];
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    size_t E_bit_len = cc_bn_bit_len(E, E_word_len);
    if (E_bit_len == 0)
    {
        cc_bn_set_one(R, N_word_len);
        return;
    }

    if (cc_bn_cmp(A, A_word_len, N, N_word_len) >= 0)
    {
        cc_bn_mod(A, A_word_len, N, N_word_len, A_tmp);
    }
    else
    {
        cc_bn_copy(A_tmp, A, N_word_len);
    }
    cc_bn_copy(T, A_tmp, N_word_len);

    for (i = E_bit_len - 2; i >= 0; i -= 1)
    {
        // T = T^2 mod N
        cc_bn_mod_exp_square(T, N, N_word_len, T);
        cc_bn_mod(T, N_word_len * 2, N, N_word_len, T);
        if (cc_bn_get_bit(E, i))
        {
            // T = T * A mod N
            cc_bn_mod_exp_mul(T, A_tmp, N, N_word_len, T);
            cc_bn_mod(T, N_word_len * 2, N, N_word_len, T);
        }
    }
    cc_bn_copy(R, T, N_word_len);
}

void cc_bn_exp_mont_square(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(A, A, N, bn_word_len, Ni, T);
    cc_bn_copy(R, T, bn_word_len);
}

void cc_bn_exp_mont_mul(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(A, B, N, bn_word_len, Ni, T);
    cc_bn_copy(R, T, bn_word_len);
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
        // T = T^2
        cc_bn_exp_mont_square(R, N, bn_word_len, Ni, R);
        if (cc_bn_get_bit(E, i))
        {
            // T = T * A
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