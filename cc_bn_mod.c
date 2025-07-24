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
        remainder = (remainder << CC_BN_WORD_BITS) | (cc_bn_dword_t)A[i - 1];
        remainder = remainder % (cc_bn_dword_t)d;
    }
    return remainder;
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

// R = A + d mod N. A < N, d < N
// R can alias A, R can not alias N
void cc_bn_mod_add_word(cc_bn_t *R, const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t carry = cc_bn_add_word(R, A, bn_word_len, d);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, R, N, bn_word_len);
    }
}

// R = A + B mod N. A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_add(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t carry = cc_bn_add_words(R, A, B, bn_word_len);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, R, N, bn_word_len);
    }
}

// R = 2A mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_double(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_mod_add(R, A, A, N, bn_word_len);
}

// R = A - d mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_sub_word(cc_bn_t *R, const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t borrow = cc_bn_sub_word(R, A, bn_word_len, d);

    if (borrow == 1)
    {
        cc_bn_add_words(R, R, N, bn_word_len);
    }
}

// R = A - B mod N, A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_sub(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t borrow = cc_bn_sub_words(R, A, B, bn_word_len);

    if (borrow == 1)
    {
        cc_bn_add_words(R, R, N, bn_word_len);
    }
}

// R = -A mod N, A < N
// R can alias A N
void cc_bn_mod_neg(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_sub_words(R, N, A, bn_word_len);
}

// R = A / 2 mod N, A < N
// N must be odd
// R can alias A N
void cc_bn_mod_half(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t carry = 0;

    if (A[0] & 1)
    {
        carry = cc_bn_add_words(R, A, N, bn_word_len);
    }
    else
    {
        cc_bn_copy(R, A, bn_word_len);
    }

    size_t i;
    for (i = 0; i < bn_word_len - 1; i += 1)
    {
        R[i] = (R[i + 1] << (CC_BN_WORD_BITS - 1)) | (R[i] >> 1);
    }
    R[bn_word_len - 1] = (carry << (CC_BN_WORD_BITS - 1)) | (R[bn_word_len - 1] >> 1);
}
