#include "cc_bn.h"

// set bn=0
void cc_bn_set_zero(cc_bn_t *bn, size_t bn_word_len)
{
    int i;
    for (i = 0; i < bn_word_len; i++)
    {
        bn[i] = 0;
    }
}

// set bn=1
void cc_bn_set_one(cc_bn_t *bn, size_t bn_word_len)
{
    int i;
    bn[0] = 1;
    for (i = 1; i < bn_word_len; i++)
    {
        bn[i] = 0;
    }
}

// bn = {word, 0, 0, ...}
void cc_bn_from_word(cc_bn_t *bn, size_t bn_word_len, cc_bn_t word)
{
    size_t i;
    bn[0] = word;
    for (i = 1; i < bn_word_len; i++)
    {
        bn[i] = 0;
    }
}

void cc_bn_from_words(cc_bn_t *bn, size_t bn_word_len, const cc_bn_t *words, size_t words_len)
{
    size_t i;
    if (bn_word_len > words_len)
    {
        for (i = 0; i < words_len; i++)
        {
            bn[i] = words[i];
        }
        for (; i < bn_word_len; i++)
        {
            bn[i] = 0;
        }
    }
    else
    {
        for (i = 0; i < bn_word_len; i++)
        {
            bn[i] = words[i];
        }
    }
}

bool cc_bn_is_zero(const cc_bn_t *bn, size_t bn_word_len)
{
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        if (bn[i] != 0)
        {
            return false;
        }
    }
    return true;
}

bool cc_bn_is_one(const cc_bn_t *bn, size_t bn_word_len)
{
    if (bn_word_len == 0 || bn[0] != 1)
    {
        return false;
    }

    size_t i;
    for (i = 1; i < bn_word_len; i++)
    {
        if (bn[i] != 0)
        {
            return false;
        }
    }
    return true;
}

// compare A and {b, 0, 0, ...}, return 1 if A > {b, 0, 0, ...}
int cc_bn_cmp_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t b)
{
    int i;
    for (i = bn_word_len - 1; i >= 1; i -= 1)
    {
        if (A[i] != 0)
        {
            return 1;
        }
    }
    if (A[0] > b)
    {
        return 1;
    }
    else if (A[0] < b)
    {
        return -1;
    }
    return 0;
}

int cc_bn_cmp_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    int i;
    for (i = bn_word_len - 1; i >= 0; i -= 1)
    {
        if (A[i] > B[i])
        {
            return 1;
        }
        else if (A[i] < B[i])
        {
            return -1;
        }
    }
    return 0;
}

int cc_bn_cmp(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    size_t A_real_word_len = cc_bn_word_len(A, A_word_len);
    size_t B_real_word_len = cc_bn_word_len(B, B_word_len);
    if (A_real_word_len > B_real_word_len)
    {
        return 1; // A is greater than B
    }
    else if (A_real_word_len < B_real_word_len)
    {
        return -1; // A is less than B
    }

    // If they have the same length, compare word by word
    return cc_bn_cmp_words(A, B, A_real_word_len);
}

void cc_bn_copy(cc_bn_t *dst, const cc_bn_t *src, size_t bn_word_len)
{
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        dst[i] = src[i];
    }
}

// swap A and B
void cc_bn_swap(cc_bn_t *A, cc_bn_t *B, size_t bn_word_len)
{
    size_t i;
    cc_bn_t tmp;
    for (i = 0; i < bn_word_len; i++)
    {
        tmp = A[i];
        A[i] = B[i];
        B[i] = tmp;
    }
}

// bn = bit(n-1)bit(n-2)...bit(0), bit(n-1) is the most significant bit
cc_bn_t cc_bn_get_bit(const cc_bn_t *bn, size_t bit_index)
{
    int digit_index = bit_index / CC_BN_WORD_BITS;
    int bit_index_in_digit = bit_index % CC_BN_WORD_BITS;
    return (bn[digit_index] >> bit_index_in_digit) & 0x01;
}

void cc_bn_set_bit(cc_bn_t *bn, size_t bit_index, cc_bn_t bit)
{
    int digit_index = bit_index / CC_BN_WORD_BITS;
    int bit_index_in_digit = bit_index % CC_BN_WORD_BITS;
    bn[digit_index] = (bn[digit_index] & ~(1 << bit_index_in_digit)) | (bit << bit_index_in_digit);
}

// return least significant bit index
// bn != 0
// example: 0x01 -> 0, 0x02 -> 1, 0x04 -> 2
size_t cc_bn_lsb(const cc_bn_t *bn, size_t bn_word_len)
{
    int i, j = 0;
    cc_bn_t tmp = 1;
    for (i = 0; i < bn_word_len; i++)
    {
        if (bn[i] != 0)
        {
            j = 0;
            while ((bn[i] & tmp) == 0)
            {
                tmp <<= 1;
                j++;
            }
            break;
        }
    }
    return i * CC_BN_WORD_BITS + j;
}

size_t cc_bn_bit_len(const cc_bn_t *bn, size_t bn_word_len)
{
    int i, j;
    cc_bn_t tmp;
    for (i = bn_word_len - 1; i >= 0; i--)
    {
        if (bn[i] != 0)
        {
            j = 0;
            tmp = bn[i];
            while (tmp != 0)
            {
                tmp >>= 1;
                j++;
            }
            return i * CC_BN_WORD_BITS + j;
        }
    }
    return 0; // If all digits are zero, return 0 bits
}

size_t cc_bn_byte_len(const cc_bn_t *bn, size_t bn_word_len)
{
    int i, j;
    cc_bn_t tmp;
    for (i = bn_word_len - 1; i >= 0; i--)
    {
        if (bn[i] != 0)
        {
            j = 0;
            tmp = bn[i];
            while (tmp != 0)
            {
                tmp >>= 8;
                j++;
            }
            return i * CC_BN_WORD_BYTES + j;
        }
    }
    return 0; // If all digits are zero, return 0 bytes
}

size_t cc_bn_word_len(const cc_bn_t *bn, size_t bn_word_len)
{
    int i;
    for (i = bn_word_len - 1; i >= 0; i--)
    {
        if (bn[i] != 0)
        {
            return i + 1;
        }
    }
    return 0; // If all digits are zero, return 0 words
}

// rigth move 1 bit, R = A / 2
// R can alias A
void cc_bn_rshift_1(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len)
{
    size_t i;
    for (i = 0; i < bn_word_len - 1; i += 1)
    {
        R[i] = (A[i + 1] << (CC_BN_WORD_BITS - 1)) | (A[i] >> 1);
    }
    R[bn_word_len - 1] = A[bn_word_len - 1] >> 1;
}

// left move 1 bit, R = A * 2, return higest bit moved out
// R can alias A
cc_bn_t cc_bn_lshift_1(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len)
{
    int i;
    cc_bn_t carry = A[bn_word_len - 1] >> (CC_BN_WORD_BITS - 1);
    for (i = bn_word_len - 1; i > 0; i -= 1)
    {
        R[i] = (A[i] << 1) | (A[i - 1] >> (CC_BN_WORD_BITS - 1));
    }
    R[0] = A[0] << 1;

    return carry;
}

// R = A << shift_bit_len
// R can alias A
void cc_bn_lshift(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, size_t shift_bit_len)
{
    if (R == A && shift_bit_len == 0)
    {
        return;
    }

    int i;
    int shift_word_len = shift_bit_len / CC_BN_WORD_BITS;
    int shift_bit_len_in_word = shift_bit_len % CC_BN_WORD_BITS;
    int remaining_bit_len = CC_BN_WORD_BITS - shift_bit_len_in_word;

    if (shift_word_len >= bn_word_len)
    {
        // If shift length is greater than the number of words, result is zero
        cc_bn_set_zero(R, bn_word_len);
        return;
    }

    if (shift_bit_len_in_word > 0)
    {
        for (i = bn_word_len - 1; i > shift_word_len; i--)
        {
            R[i] = (A[i - shift_word_len] << shift_bit_len_in_word) |
                   (A[i - shift_word_len - 1] >> remaining_bit_len);
        }
        R[shift_word_len] = A[0] << shift_bit_len_in_word;
        for (i = shift_word_len - 1; i >= 0; i--)
        {
            R[i] = 0;
        }
    }
    else
    {
        for (i = bn_word_len - 1; i >= shift_word_len; i--)
        {
            R[i] = A[i - shift_word_len];
        }
        for (i = shift_word_len - 1; i >= 0; i--)
        {
            R[i] = 0; // Clear the lower words
        }
    }
}

// R = A >> shift_bit_len
// R can alias A
void cc_bn_rshift(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, size_t shift_bit_len)
{
    if (R == A && shift_bit_len == 0)
    {
        return;
    }

    int i;
    int shift_word_len = shift_bit_len / CC_BN_WORD_BITS;
    int shift_bit_len_in_word = shift_bit_len % CC_BN_WORD_BITS;
    int remaining_bit_len = CC_BN_WORD_BITS - shift_bit_len_in_word;

    if (shift_word_len >= bn_word_len)
    {
        // If shift length is greater than the number of words, result is zero
        cc_bn_set_zero(R, bn_word_len);
        return;
    }

    if (shift_bit_len_in_word > 0)
    {
        for (i = 0; i < bn_word_len - shift_word_len - 1; i++)
        {
            R[i] = (A[i + shift_word_len] >> shift_bit_len_in_word) |
                   (A[i + shift_word_len + 1] << remaining_bit_len);
        }
        R[bn_word_len - shift_word_len - 1] = A[bn_word_len - 1] >> shift_bit_len_in_word;
        for (i = bn_word_len - shift_word_len; i < bn_word_len; i++)
        {
            R[i] = 0; // Clear the upper words
        }
    }
    else
    {
        for (i = 0; i < bn_word_len - shift_word_len; i++)
        {
            R[i] = A[i + shift_word_len];
        }
        for (i = bn_word_len - shift_word_len; i < bn_word_len; i++)
        {
            R[i] = 0; // Clear the upper words
        }
    }
}

// R = A + d
// R can alias A
cc_bn_t cc_bn_add_word(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d)
{
    size_t i;
    cc_bn_t carry = d;
    for (i = 0; i < bn_word_len; i++)
    {
        R[i] = A[i] + carry;
        carry = R[i] < carry;
    }
    return carry; // return carry
}

// R = A + B
// R can alias A B
cc_bn_t cc_bn_add_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_t t = A[i] + carry;
        carry = (t < A[i]);
        t += B[i];
        carry += (t < B[i]);
        R[i] = t;
    }
    return carry; // return carry
}

// R = A + B
// A_word_len must >= B_word_len
// R can alias A B
cc_bn_t cc_bn_add_small(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    size_t i;
    cc_bn_t carry = 0;
    for (i = 0; i < B_word_len; i++)
    {
        cc_bn_t t = A[i] + carry;
        carry = (t < A[i]);
        t += B[i];
        carry += (t < B[i]);
        R[i] = t;
    }
    for (i = B_word_len; i < A_word_len; i++)
    {
        R[i] = A[i] + carry;
        carry = (R[i] < carry);
    }
    return carry; // return carry
}

// R = A + B
// R_word_len = max(A_word_len, B_word_len)
// R can alias A B
cc_bn_t cc_bn_add(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    if (A_word_len < B_word_len)
    {
        return cc_bn_add_small(R, B, B_word_len, A, A_word_len);
    }
    else
    {
        return cc_bn_add_small(R, A, A_word_len, B, B_word_len);
    }
}

// R = A - d
// R can alias A
cc_bn_t cc_bn_sub_word(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d)
{
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_t s = A[i];
        cc_bn_t t = s - d;
        d = (t > s);
        R[i] = t;
    }
    return d; // return borrow
}

// R = A - B
// R can alias A B
// if A < B, return borrow = 1, and R = 2^n - (B - A),
// else return borrow = 0, and R = A - B
cc_bn_t cc_bn_sub_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    size_t i;
    cc_bn_t borrow = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_t z = (A[i] < borrow);
        cc_bn_t t = A[i] - borrow;
        borrow = (t < B[i]) + z;
        R[i] = t - B[i];
    }
    return borrow; // return borrow
}

// R = A - B
// A_word_len must >= B_word_len
// R_word_len = A_word_len
// R can alias A B
cc_bn_t cc_bn_sub_small(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    size_t i;
    cc_bn_t borrow = 0;
    for (i = 0; i < B_word_len; i++)
    {
        cc_bn_t z = (A[i] < borrow);
        cc_bn_t t = A[i] - borrow;
        borrow = (t < B[i]) + z;
        R[i] = t - B[i];
    }
    for (i = B_word_len; i < A_word_len; i++)
    {
        cc_bn_t z = (A[i] < borrow);
        R[i] = A[i] - borrow;
        borrow = z;
    }
    return borrow; // return borrow
}
