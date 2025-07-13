#include "cc_bn_mul.h"

// note: bn_out must be at least bn_word_len+1
void cc_bn_mul_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        uint64_t t = (uint64_t)A[i] * (uint64_t)d + (uint64_t)carry;
        carry = t >> CC_BN_DIGIT_BITS;
        R[i] = t & CC_BN_DIGIT_MAX;
    }
    R[i] = carry; // store carry in the last digit
}

// R = R + A * d
// R_word_len = bn_word_len + 1, R[bn_word_len] may need to be zero
void cc_bn_mul_word_add(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R)
{
    if (bn_word_len == 0)
    {
        return;
    }

    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        uint64_t t = (uint64_t)A[i] * (uint64_t)d + (uint64_t)carry + (uint64_t)R[i];
        carry = t >> CC_BN_DIGIT_BITS;
        R[i] = t & CC_BN_DIGIT_MAX;
    }
    R[i] += carry; // store carry in the last digit
}

// R = A * B
// R_word_len = bn_word_len * 2
// R can not be alias for A or B
void cc_bn_mul_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_set_zero(R, bn_word_len * 2);

    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_mul_word_add(A, bn_word_len, B[i], R + i);
    }
}

// R = A * B
// R_word_len = A_word_len + B_word_len
// R can not be alias for A or B
void cc_bn_mul(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R)
{
    cc_bn_set_zero(R, A_word_len + B_word_len);

    size_t i;
    for (i = 0; i < B_word_len; i++)
    {
        cc_bn_mul_word_add(A, A_word_len, B[i], R + i);
    }
}

// montgomery multiplication, N must be odd
// R2 = 2^(2*N_bit_len) mod N
void cc_bn_mont_R2(const cc_bn_t *N, size_t N_word_len, cc_bn_t *R2)
{
    size_t i;
    size_t N_bit_len = cc_bn_bit_len(N, N_word_len);

    // t=1
    cc_bn_set_one(R2, N_word_len);

    for (i = 0; i < N_bit_len * 2; i++)
    {
        // t = t << 1
        cc_bn_t carry = cc_bn_lshift_1(R2, N_word_len, R2);
        if ((cc_bn_cmp(R2, N_word_len, N, N_word_len) > 0) || carry)
        {
            cc_bn_sub_small(R2, N_word_len, N, N_word_len, R2);
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
    for (i = CC_BN_DIGIT_BITS; i >= 8; i /= 2)
    {
        x *= (2 - (N[0] * x));
    }

    return ~x + 1;
}

// R = R + A * d
// return carry
cc_bn_t cc_bn_mul_word_add_ret(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        uint64_t t = (uint64_t)A[i] * (uint64_t)d + (uint64_t)carry + (uint64_t)R[i];
        carry = t >> CC_BN_DIGIT_BITS;
        R[i] = t & CC_BN_DIGIT_MAX;
    }
    return carry;
}

// R = mont_mul(A, b) = A * b * r^(-1) mod N
// A, b < N, N must be odd
// R can not be alias for A
void cc_bn_mont_mul_word(const cc_bn_t *A, const cc_bn_t b, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t a0 = A[0];

    // d = 0
    cc_bn_set_zero(R, bn_word_len);

    size_t i, j;

    // q = (d0 + a0 *bi) * Ni
    cc_bn_t q = (R[0] + a0 * b) * Ni;

    // d = d + A * B[i]
    cc_bn_t carry1 = cc_bn_mul_word_add_ret(A, bn_word_len, b, R);
    // d = d + q * N
    cc_bn_t carry2 = cc_bn_mul_word_add_ret(N, bn_word_len, q, R);

    cc_bn_t Rn = carry1 + carry2;
    cc_bn_t carry = Rn < carry1;

    // d = d * r^(-1)
    for (j = 0; j < bn_word_len - 1; j++)
    {
        R[j] = R[j + 1];
    }
    R[bn_word_len - 1] = Rn;

    for (i = 1; i < bn_word_len; i++)
    {
        // q = (d0 + a0 *bi) * Ni
        q = R[0] * Ni;

        // d = d + q * N
        carry2 = cc_bn_mul_word_add_ret(N, bn_word_len, q, R);
        Rn = carry2 + carry;
        carry = Rn < carry2;

        // d = d * r^(-1)
        for (j = 0; j < bn_word_len - 1; j++)
        {
            R[j] = R[j + 1];
        }
        R[bn_word_len - 1] = Rn;
    }

    // if d >= N, d = d - N
    if ((cc_bn_cmp(R, bn_word_len, N, bn_word_len) >= 0) || carry)
    {
        cc_bn_sub_small(R, bn_word_len, N, bn_word_len, R);
    }
}

// R = mont_mul(A, B) = A * B * r^(-1) mod N
// A, B < N, N must be odd
// R can not be alias for A or B
void cc_bn_mont_mul(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t Ni, cc_bn_t *R)
{
    cc_bn_t a0 = A[0];

    // d = 0
    cc_bn_set_zero(R, bn_word_len);

    cc_bn_t carry = 0;

    size_t i, j;
    for (i = 0; i < bn_word_len; i++)
    {
        // q = (d0 + a0 *bi) * Ni
        cc_bn_t q = (R[0] + a0 * B[i]) * Ni;

        // d = d + A * B[i]
        cc_bn_t carry1 = cc_bn_mul_word_add_ret(A, bn_word_len, B[i], R);
        // d = d + q * N
        cc_bn_t carry2 = cc_bn_mul_word_add_ret(N, bn_word_len, q, R);

        cc_bn_t Rn = carry1 + carry2;
        cc_bn_t c1 = Rn < carry1;
        Rn = Rn + carry;
        cc_bn_t c2 = Rn < carry;
        carry = c1 | c2;

        // d = d * r^(-1)
        for (j = 0; j < bn_word_len - 1; j++)
        {
            R[j] = R[j + 1];
        }
        R[bn_word_len - 1] = Rn;
    }

    // if d >= N, d = d - N
    if ((cc_bn_cmp(R, bn_word_len, N, bn_word_len) >= 0) || carry)
    {
        cc_bn_sub_small(R, bn_word_len, N, bn_word_len, R);
    }
}
