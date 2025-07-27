#include "cc_bn_mul.h"
#include "cc_bn_config.h"

// R = A * d
// note: R must be at least A_word_len + 1
// R can alias A
// return R_word_len = A_word_len + 1
size_t cc_bn_mul_word(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, cc_bn_t d)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < A_word_len; i++)
    {
        cc_bn_dword_t t = (cc_bn_dword_t)A[i] * (cc_bn_dword_t)d + (cc_bn_dword_t)carry;
        carry = t >> CC_BN_WORD_BITS;
        R[i] = t & CC_BN_WORD_MAX;
    }
    R[i] = carry; // store carry in the last digit
    return A_word_len + 1;
}

// R = R + A * d
// R_word_len must >= A_word_len + 1
// R can alias A
cc_status_t cc_bn_mul_word_add(cc_bn_t *R, size_t R_word_len, const cc_bn_t *A, size_t A_word_len, cc_bn_t d)
{
    if (A_word_len == 0)
    {
        return CC_OK;
    }
    if (R_word_len < A_word_len + 1)
    {
        return CC_ERR_BN_LEN_TOO_SHORT;
    }

    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < A_word_len; i++)
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
    return CC_OK;
}

// R = A * B, vertical multiplication
// R_word_len = bn_word_len * 2
// R cannot alias A or B
void cc_bn_core_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
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
void cc_bn_core_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    cc_bn_set_zero(R, A_word_len + B_word_len);

    size_t i;
    for (i = 0; i < B_word_len; i++)
    {
        cc_bn_mul_word_add(R + i, A_word_len + 1, A, A_word_len, B[i]);
    }
}

// R = A * B, vertical multiplication
// R_word_len = bn_word_len * 2
// R can alias A B
void cc_bn_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_core_mul_words(T, A, B, bn_word_len);
    cc_bn_copy(R, T, bn_word_len * 2);
}

// R = A * B, vertical multiplication
// R_word_len = A_word_len + B_word_len
// R can alias A B
void cc_bn_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_core_mul(T, A, A_word_len, B, B_word_len);
    cc_bn_copy(R, T, A_word_len + B_word_len);
}
