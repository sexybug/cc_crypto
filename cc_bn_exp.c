
#include "cc_bn_exp.h"
#include "cc_bn_mul.h"
#include "cc_bn_mod.h"
#include "cc_bn_div.h"
#include "cc_bn_config.h"

// R = A mod N, R length = N length
// R can alias A N
cc_bn_status_t cc_bn_mod(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t A_tmp[CC_BN_MAX_WORDS];
    cc_bn_t N_tmp[CC_BN_MAX_WORDS];
    cc_bn_t Q_tmp[CC_BN_MAX_WORDS];
    cc_bn_t R_tmp[CC_BN_MAX_WORDS];

    cc_bn_copy(A_tmp, A, A_word_len);
    cc_bn_copy(N_tmp, N, N_word_len);
    cc_bn_status_t div_status = cc_bn_div_unsafe(Q_tmp, R_tmp, A_tmp, A_word_len, N_tmp, N_word_len);
    if (CC_BN_ERR(div_status))
    {
        return div_status;
    }
    cc_bn_copy(R, R_tmp, N_word_len);
}

// TODO: optimize
// R = A^2
// R can alias A N
void cc_bn_mod_exp_square(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_mul_words(T, A, A, N_word_len);
    cc_bn_copy(R, T, N_word_len * 2);
}

// R = A * B
// R can alias A B
void cc_bn_mod_exp_mul(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_mul_words(T, A, B, N_word_len);
    cc_bn_copy(R, T, N_word_len * 2);
}

// R = A^E mod N
// R can alias A E N
void cc_bn_mod_exp(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *E, size_t E_word_len,
                   const cc_bn_t *N, size_t N_word_len)
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
        cc_bn_mod(A_tmp, A, A_word_len, N, N_word_len);
    }
    else
    {
        cc_bn_copy(A_tmp, A, N_word_len);
    }
    cc_bn_copy(T, A_tmp, N_word_len);

    for (i = E_bit_len - 2; i >= 0; i -= 1)
    {
        // T = T^2 mod N
        cc_bn_mod_exp_square(T, T, N, N_word_len);
        cc_bn_mod(T, T, N_word_len * 2, N, N_word_len);
        if (cc_bn_get_bit(E, i))
        {
            // T = T * A mod N
            cc_bn_mod_exp_mul(T, T, A_tmp, N, N_word_len);
            cc_bn_mod(T, T, N_word_len * 2, N, N_word_len);
        }
    }
    cc_bn_copy(R, T, N_word_len);
}

// R = A^2 mod N, A R is montgomery form
// R can alias A N
void cc_bn_exp_mont_square(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(T, A, A, N, bn_word_len, Ni);
    cc_bn_copy(R, T, bn_word_len);
}

// R = A * B mod N, A B R is montgomery form
// R can alias A B N
void cc_bn_exp_mont_mul(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_mont_mul(T, A, B, N, bn_word_len, Ni);
    cc_bn_copy(R, T, bn_word_len);
}

// R = mont_exp(A, E) = A^E mod N, A R is montgomery form
// R cannot alias A E N
void cc_bn_mont_exp(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
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
        cc_bn_exp_mont_square(R, R, N, bn_word_len, Ni);
        if (cc_bn_get_bit(E, i))
        {
            // T = T * A
            cc_bn_exp_mont_mul(R, R, A, N, bn_word_len, Ni);
        }
    }
}

// r = a^(1/2) mod p,  p is prim and p = 3 (mod 4), A is montgomery form
// (p - r) is also a square root of a
// not every number has a square root, return CC_BN_ERR_NOT_SQUARE if not found
// R can alias A P
cc_bn_status_t cc_bn_mont_sqrt_p3(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni)
{
    // r = a^((p + 1)/4) when p = 3 (mod 4)

    cc_bn_t t[CC_BN_MAX_WORDS];
    cc_bn_t r[CC_BN_MAX_WORDS];

    // t = (p + 1)/4
    cc_bn_add_word(t, P, bn_word_len, 1);
    cc_bn_rshift(t, t, bn_word_len, 2);

    // r = a^((p + 1)/4)
    cc_bn_mont_exp(r, A, t, P, bn_word_len, Ni);

    // check r^2 = a
    cc_bn_exp_mont_square(t, r, P, bn_word_len, Ni);
    if (cc_bn_cmp_words(t, A, bn_word_len) != 0)
    {
        return CC_BN_ERR_NOT_SQUARE;
    }

    cc_bn_copy(R, r, bn_word_len);
    return CC_BN_SUCCESS;
}

// r = a^(-1) mod p,  p is prim, A is montgomery form
// R cannot alias A P
void cc_bn_mont_inv(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni)
{
    // r = a^(p - 2) when p is prim

    // e = p - 2
    cc_bn_t E[CC_BN_MAX_WORDS];
    cc_bn_sub_word(E, P, bn_word_len, 2);

    // r = a^(p - 2)
    cc_bn_mont_exp(R, A, E, P, bn_word_len, Ni);
}