#include "cc_bn_mod.h"

// R = A mod d, d != 0
// Vertical division
cc_bn_t cc_bn_mod_word(const cc_bn_t *A, size_t A_word_len, cc_bn_t d)
{
    size_t i;

    if (d == 1 || cc_bn_is_zero(A, A_word_len))
    {
        return 0;
    }

    if (d == 2)
    {
        return A[0] & 1;
    }

    cc_bn_dword_t remainder = A[A_word_len - 1] % d;
    for (i = A_word_len - 1; i > 0; i -= 1)
    {
        remainder = (remainder << CC_BN_DIGIT_BITS) | (cc_bn_dword_t)A[i - 1];
        remainder = remainder % (cc_bn_dword_t)d;
    }
    return remainder & CC_BN_DIGIT_MASK;
}

// Q = A / N, R = A % N
// A, N are modified in the process, N array must be at least A_word_len
// Q_word_len = A_word_len, R_word_len = N_word_len
cc_bn_status_t cc_bn_div(cc_bn_t *A, size_t A_word_len, cc_bn_t *N, size_t N_word_len,
                         cc_bn_t *Q, cc_bn_t *R)
{
    // if N == 0, return ERROR
    if (cc_bn_is_zero(N, N_word_len))
    {
        return CC_BN_ERR_DIV_BY_ZERO;
    }

    // if A < N, Q = 0, R = A
    if (cc_bn_cmp(A, A_word_len, N, N_word_len) < 0)
    {
        cc_bn_copy(R, A, N_word_len);
        cc_bn_set_zero(Q, A_word_len);
        return CC_BN_SUCCESS;
    }

    // A >= N, A_real_word_len >= N_real_word_len
    size_t A_real_word_len = cc_bn_word_len(A, A_word_len);
    size_t N_real_word_len = cc_bn_word_len(N, N_word_len);

    size_t i;
    size_t A_bit_len = cc_bn_bit_len(A, A_real_word_len);
    size_t N_bit_len = cc_bn_bit_len(N, N_real_word_len);

    for (i = N_real_word_len; i < A_real_word_len; i++)
    {
        N[i] = 0;
    }
    cc_bn_set_zero(Q, A_word_len); // Q = 0

    cc_bn_lshift(N, A_real_word_len, A_bit_len - N_bit_len, N); // N = N << (A_bit_len - N_bit_len)

    for (i = 0; i <= A_bit_len - N_bit_len; i++)
    {
        cc_bn_lshift_1(Q, A_real_word_len - N_real_word_len + 1, Q); // Q = Q << 1
        // if (A >= N)
        if (cc_bn_cmp(A, A_real_word_len, N, A_real_word_len) >= 0)
        {
            Q[0] |= 1;                                 // Q = Q | 1
            cc_bn_sub_words(A, N, A_real_word_len, A); // A = A - N
        }
        cc_bn_rshift_1(N, A_real_word_len, N); // N = N >> 1
    }
    // R = A
    cc_bn_from_words(R, N_word_len, A, N_real_word_len);
    return CC_BN_SUCCESS;
}

// compare {carry, A} with B
int cc_bn_cmp_carry(cc_bn_t carry, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    if (carry == 0)
    {
        return cc_bn_cmp_words(A, B, bn_word_len);
    }
    else
    {
        return 1;
    }
}

// R = A + d mod N, A < N
void cc_bn_mod_add_word(const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_t carry = cc_bn_add_word(A, bn_word_len, d, R);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, N, bn_word_len, R);
    }
}

// R = A + B mod N, A < N, B < N
void cc_bn_mod_add(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_t carry = cc_bn_add_words(A, B, bn_word_len, R);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, N, bn_word_len, R);
    }
}

// R = 2A mod N, A < N
void cc_bn_mod_double(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_mod_add(A, A, N, bn_word_len, R);
}

// R = A - d mod N, A < N
void cc_bn_mod_sub_word(const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_t borrow = cc_bn_sub_word(A, bn_word_len, d, R);

    if (borrow == 1)
    {
        cc_bn_add_words(R, N, bn_word_len, R);
    }
}

// R = A - B mod N, A < N, B < N
void cc_bn_mod_sub(const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_t borrow = cc_bn_sub_words(A, B, bn_word_len, R);

    if (borrow == 1)
    {
        cc_bn_add_words(R, N, bn_word_len, R);
    }
}

// R = -A mod N, A < N
void cc_bn_mod_neg(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_sub_words(N, A, bn_word_len, R);
}

// R = A / 2 mod N, A < N
void cc_bn_mod_half(const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len, cc_bn_t *R)
{
    cc_bn_t carry = 0;

    if (A[0] & 1)
    {
        carry = cc_bn_add_words(A, N, bn_word_len, R);
    }
    else
    {
        cc_bn_copy(R, A, bn_word_len);
    }

    size_t i;
    for (i = 0; i < bn_word_len - 1; i += 1)
    {
        R[i] = (R[i + 1] << (CC_BN_DIGIT_BITS - 1)) | (R[i] >> 1);
    }
    R[bn_word_len - 1] = (carry << (CC_BN_DIGIT_BITS - 1)) | (R[bn_word_len - 1] >> 1);
}
