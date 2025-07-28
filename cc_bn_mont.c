
#include "cc_bn_mont.h"
#include "cc_bn_config.h"
#include "cc_bn_mod.h"

// pre-compute montgomery multiplication RR
// N must be odd
// R = 2^(CC_BN_WORD_BITS*n), RR = 2^(CC_BN_WORD_BITS*n*2) mod N, n is N word length
// R canot alias N
void cc_bn_mont_RR(cc_bn_t *RR, const cc_bn_t *N, size_t N_word_len)
{
    size_t i;
    size_t N_bit_len = cc_bn_word_len(N, N_word_len) * CC_BN_WORD_BITS;

    // t=1
    cc_bn_set_one(RR, N_word_len);

    for (i = 0; i < N_bit_len * 2; i++)
    {
        // t = t << 1
        cc_bn_t carry = cc_bn_lshift_1(RR, RR, N_word_len);
        if ((cc_bn_cmp(RR, N_word_len, N, N_word_len) > 0) || carry)
        {
            cc_bn_sub_small(RR, RR, N_word_len, N, N_word_len);
        }
    }
}

// montgomery multiplication, N must be odd
// Ni = -(N^-1) mod 2^r, r is bit length of every bn digit
cc_bn_t cc_bn_mont_Ni(const cc_bn_t *N)
{
    cc_bn_t x = N[0];
    x += ((N[0] + 2) & 4) << 1;

    unsigned int i;
    for (i = CC_BN_WORD_BITS; i >= 8; i /= 2)
    {
        x *= (2 - (N[0] * x));
    }

    return ~x + 1;
}

// R = R + A * d
// R length = A length
// return carry
// R can alias A
cc_bn_t cc_bn_mul_word_add_ret(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_dword_t t = (cc_bn_dword_t)A[i] * (cc_bn_dword_t)d + (cc_bn_dword_t)carry + (cc_bn_dword_t)R[i];
        carry = t >> CC_BN_WORD_BITS;
        R[i] = t & CC_BN_WORD_MAX;
    }
    return carry;
}

// D = mont_mul(A, b) = A * b * R^(-1) mod N
// A, b < N, N must be odd
// D cannot alias A
void cc_bn_core_mont_mul_word(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t a0 = A[0];

    // d = 0
    cc_bn_set_zero(D, bn_word_len);

    size_t i, j;

    // q = (d0 + a0 *bi) * Ni
    cc_bn_t q = (D[0] + a0 * b) * Ni;

    // d = d + A * B[i]
    cc_bn_t carry1 = cc_bn_mul_word_add_ret(D, A, bn_word_len, b);
    // d = d + q * N
    cc_bn_t carry2 = cc_bn_mul_word_add_ret(D, N, bn_word_len, q);

    cc_bn_t Rn = carry1 + carry2;
    cc_bn_t carry = Rn < carry1;

    // d = d * r^(-1)
    for (j = 0; j < bn_word_len - 1; j++)
    {
        D[j] = D[j + 1];
    }
    D[bn_word_len - 1] = Rn;

    for (i = 1; i < bn_word_len; i++)
    {
        // q = (d0 + a0 *bi) * Ni
        q = D[0] * Ni;

        // d = d + q * N
        carry2 = cc_bn_mul_word_add_ret(D, N, bn_word_len, q);
        Rn = carry2 + carry;
        carry = Rn < carry2;

        // d = d * r^(-1)
        for (j = 0; j < bn_word_len - 1; j++)
        {
            D[j] = D[j + 1];
        }
        D[bn_word_len - 1] = Rn;
    }

    // if d >= N, d = d - N
    if ((cc_bn_cmp(D, bn_word_len, N, bn_word_len) >= 0) || carry)
    {
        cc_bn_sub_small(D, D, bn_word_len, N, bn_word_len);
    }
}

// D = mont_mul(A, B) = A * B * R^(-1) mod N
// A, b < N, N must be odd
// D cannot alias A or B
void cc_bn_core_mont_mul(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t a0 = A[0];

    // d = 0
    cc_bn_set_zero(D, bn_word_len);

    cc_bn_t carry = 0;

    size_t i, j;
    for (i = 0; i < bn_word_len; i++)
    {
        // q = (d0 + a0 *bi) * Ni mod k
        cc_bn_t q = (D[0] + a0 * B[i]) * Ni;

        // d = d + A * B[i]
        cc_bn_t carry1 = cc_bn_mul_word_add_ret(D, A, bn_word_len, B[i]);
        // d = d + q * N
        cc_bn_t carry2 = cc_bn_mul_word_add_ret(D, N, bn_word_len, q);

        cc_bn_t Rn = carry1 + carry2;
        cc_bn_t c1 = Rn < carry1;
        Rn = Rn + carry;
        cc_bn_t c2 = Rn < carry;
        carry = c1 | c2;

        // d = d * r^(-1)
        for (j = 0; j < bn_word_len - 1; j++)
        {
            D[j] = D[j + 1];
        }
        D[bn_word_len - 1] = Rn;
    }

    // if d >= N, d = d - N
    if ((cc_bn_cmp(D, bn_word_len, N, bn_word_len) >= 0) || carry)
    {
        cc_bn_sub_small(D, D, bn_word_len, N, bn_word_len);
    }
}

// D = mont_mul(A, b) = A * b * R^(-1) mod N
// A, b < N, N must be odd
// D can alias A
void cc_bn_mont_mul_word(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_core_mont_mul_word(T, A, b, N, bn_word_len, Ni);
    cc_bn_copy(D, T, bn_word_len);
}

// D = mont_mul(A, B) = A * B * R^(-1) mod N
// A, B < N, N must be odd
// D can alias A B
void cc_bn_mont_mul(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_core_mont_mul(T, A, B, N, bn_word_len, Ni);
    cc_bn_copy(D, T, bn_word_len);
}

// D = mont_mul(A, A) = A^2 * R^(-1) mod N
// A < N, N must be odd
// D can alias A
void cc_bn_mont_square(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_core_mont_mul(T, A, A, N, bn_word_len, Ni);
    cc_bn_copy(D, T, bn_word_len);
}

// R = mont_exp(A, E) = A^E mod N, A R is montgomery form
// R cannot alias A E N
void cc_bn_core_mont_exp(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, size_t E_word_len, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    int i;
    size_t E_bit_len = cc_bn_bit_len(E, E_word_len);
    if (E_bit_len == 0)
    {
        cc_bn_set_one(R, bn_word_len);
        return;
    }

    // R = A
    cc_bn_copy(R, A, bn_word_len);

    for (i = E_bit_len - 2; i >= 0; i -= 1)
    {
        // R = R^2
        cc_bn_mont_square(R, R, N, bn_word_len, Ni);
        if (cc_bn_get_bit(E, i))
        {
            // R = R * A
            cc_bn_mont_mul(R, R, A, N, bn_word_len, Ni);
        }
    }
}

// R = mont_exp(A, E) = A^E mod N, A R is montgomery form
// R can alias A E N
void cc_bn_mont_exp(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, size_t E_word_len, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_core_mont_exp(T, A, E, E_word_len, N, bn_word_len, Ni);
    cc_bn_copy(R, T, bn_word_len);
}

// R = A^(-1) mod P,  P is prim, A is montgomery form
// R can alias A P
void cc_bn_mont_inv(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni)
{
    // R = A^(P - 2) when P is prim

    // E = P - 2
    cc_bn_t E[CC_BN_MAX_WORDS];
    cc_bn_sub_word(E, P, bn_word_len, 2);

    // R = A^(P - 2)
    cc_bn_mont_exp(R, A, E, bn_word_len, P, bn_word_len, Ni);
}

// r = A^(1/2) mod P,  P is prim and P = 3 (mod 4), A is montgomery form
// (P - R) is also a square root of A
// not every number has a square root, return CC_ERR_BN_NOT_SQUARE if not found
// R can alias A P
cc_status_t cc_bn_mont_sqrt_p3(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *P, size_t bn_word_len, cc_bn_t Ni)
{
    // R = A^((P + 1)/4) when P = 3 (mod 4)

    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_t R1[CC_BN_MAX_WORDS];

    // T = (P + 1)/4
    cc_bn_add_word(T, P, bn_word_len, 1);
    cc_bn_rshift(T, T, bn_word_len, 2);

    // R = A^((P + 1)/4)
    cc_bn_mont_exp(R1, A, T, bn_word_len, P, bn_word_len, Ni);

    // T = R^2
    cc_bn_mont_square(T, R1, P, bn_word_len, Ni);
    // check R^2 = A
    if (cc_bn_cmp_words(T, A, bn_word_len) != 0)
    {
        return CC_ERR_BN_NOT_SQUARE;
    }

    cc_bn_copy(R, R1, bn_word_len);
    return CC_OK;
}

// R = A*B mod N, using montgomery, A,B,R isn't montgomery form, A B < N
// R can alias A B N
void cc_bn_core_mod_mul_mont(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *RR, cc_bn_t Ni)
{
    cc_bn_t montA[CC_BN_MAX_WORDS];
    cc_bn_t montB[CC_BN_MAX_WORDS];
    cc_bn_t montR[CC_BN_MAX_WORDS];

    cc_bn_core_mont_mul(montA, A, RR, N, bn_word_len, Ni);
    cc_bn_core_mont_mul(montB, B, RR, N, bn_word_len, Ni);
    cc_bn_core_mont_mul(montR, montA, montB, N, bn_word_len, Ni);
    cc_bn_core_mont_mul_word(R, montR, 1, N, bn_word_len, Ni);
}

// R = A^2 mod N, using montgomery, A R isn't montgomery form, A < N
// R can alias A N
void cc_bn_core_mod_square_mont(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *RR, cc_bn_t Ni)
{
    cc_bn_t montA[CC_BN_MAX_WORDS];
    cc_bn_t montR[CC_BN_MAX_WORDS];

    cc_bn_core_mont_mul(montA, A, RR, N, bn_word_len, Ni);
    cc_bn_core_mont_mul(montR, montA, montA, N, bn_word_len, Ni);
    cc_bn_core_mont_mul_word(R, montR, 1, N, bn_word_len, Ni);
}

// R = A^E mod N, using montgomery, A R isn't montgomery form, A < N
// R can alias A E N
void cc_bn_core_mod_exp_mont(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *E, size_t E_word_len, const cc_bn_t *N, size_t bn_word_len, const cc_bn_t *RR, cc_bn_t Ni)
{
    cc_bn_t montA[CC_BN_MAX_WORDS];
    cc_bn_t montR[CC_BN_MAX_WORDS];

    // montA = mont(A, RR)
    cc_bn_core_mont_mul(montA, A, RR, N, bn_word_len, Ni);
    // montR = montA ^ E mod N
    cc_bn_core_mont_exp(montR, montA, E, E_word_len, N, bn_word_len, Ni);
    // R = mont(montR, 1)
    cc_bn_core_mont_mul_word(R, montR, 1, N, bn_word_len, Ni);
}

// R = A^E mod N, using montgomery, A R isn't montgomery form, A < N
// R can alias A E N
void cc_bn_mod_exp_mont(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *E, size_t E_word_len, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t RR[CC_BN_MAX_WORDS];
    cc_bn_t TA[CC_BN_MAX_WORDS];

    cc_bn_mont_RR(RR, N, N_word_len);
    cc_bn_t Ni = cc_bn_mont_Ni(N);

    if (cc_bn_cmp(A, A_word_len, N, N_word_len) >= 0)
    {
        cc_bn_mod(TA, A, A_word_len, N, N_word_len);
    }
    else
    {
        cc_bn_from_words(TA, N_word_len, A, A_word_len);
    }
    cc_bn_core_mod_exp_mont(R, TA, E, E_word_len, N, N_word_len, RR, Ni);
}