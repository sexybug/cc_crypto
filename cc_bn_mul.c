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

// R = R + A * d, R must be at least bn_word_len+1
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

// R = A * B, R must be at least 2*bn_word_len
// R can not be aliased to A or B
void cc_bn_mul_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_set_zero(R, bn_word_len * 2);

    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_mul_word_add(A, bn_word_len, B[i], R + i);
    }
}

// R = A * B, R must be at least A_word_len + B_word_len
// R can not be aliased to A or B
void cc_bn_mul(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R)
{
    cc_bn_set_zero(R, A_word_len + B_word_len);

    size_t i;
    for (i = 0; i < B_word_len; i++)
    {
        cc_bn_mul_word_add(A, A_word_len, B[i], R + i);
    }
}

// R2 = 2^(2*N_bit_len) mod N, R2 is at least N_word_len+1
void cc_bn_mont_R2(const cc_bn_t *N, size_t N_word_len, cc_bn_t *R2)
{
    size_t i;
    size_t N_bit_len = cc_bn_bit_len(N, N_word_len);
    size_t R2_word_len = N_word_len + 1; // R2 must be at least one word longer than N

    // t=1
    cc_bn_set_one(R2, R2_word_len);

    for (i = 0; i < N_bit_len * 2; i++)
    {
        cc_bn_lshift_1(R2, R2_word_len, R2);
        if (cc_bn_cmp(R2, R2_word_len, N, N_word_len) > 0)
        {
            cc_bn_sub_small(R2, R2_word_len, N, N_word_len, R2);
        }
    }
}