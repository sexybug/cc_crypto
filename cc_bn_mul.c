#include "cc_bn_mul.h"
#include <assert.h>

// R = A * d
// note: R must be at least bn_word_len + 1
// R can alias A
void cc_bn_mul_word(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_dword_t t = (cc_bn_dword_t)A[i] * (cc_bn_dword_t)d + (cc_bn_dword_t)carry;
        carry = t >> CC_BN_WORD_BITS;
        R[i] = t & CC_BN_WORD_MAX;
    }
    R[i] = carry; // store carry in the last digit
}

// R = R + A * d
// R_word_len must >= bn_word_len + 1
// R can alias A
cc_bn_status_t cc_bn_mul_word_add(cc_bn_t *R, size_t R_word_len, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d)
{
    if (bn_word_len == 0)
    {
        return CC_BN_SUCCESS;
    }
    if (R_word_len < bn_word_len + 1)
    {
        return CC_BN_ERR_LEN_TOO_SHORT;
    }

    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_dword_t t = (cc_bn_dword_t)A[i] * (cc_bn_dword_t)d + (cc_bn_dword_t)carry + (cc_bn_dword_t)R[i];
        carry = t >> CC_BN_WORD_BITS;
        R[i] = t & CC_BN_WORD_MAX;
    }
    while (carry && i < R_word_len)
    {
        R[i] += carry;
        carry = R[i] < carry;
    }
    return CC_BN_SUCCESS;
}

// R = A * B, vertical multiplication
// R_word_len = bn_word_len * 2
// R cannot alias A or B
void cc_bn_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    cc_bn_set_zero(R, bn_word_len * 2);

    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_mul_word_add(R + i, bn_word_len + 1, A, bn_word_len, B[i]);
    }
}

// R = A * B, vertical multiplication
// R_word_len = A_word_len + B_word_len
// R cannot alias A or B
void cc_bn_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    cc_bn_set_zero(R, A_word_len + B_word_len);

    size_t i;
    for (i = 0; i < B_word_len; i++)
    {
        cc_bn_mul_word_add(R + i, A_word_len + 1, A, A_word_len, B[i]);
    }
}

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
void cc_bn_mont_mul_word(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
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
void cc_bn_mont_mul(cc_bn_t *D, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni)
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

        assert(D[0] == 0);

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
