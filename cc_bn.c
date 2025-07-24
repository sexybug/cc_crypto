#include "cc_bn.h"

static cc_bn_t cc_u8_to_bn_word(const uint8_t *src, size_t byte_len)
{
    size_t i;
    cc_bn_t word = 0;
    for (i = 0; (i < byte_len) && (i < CC_BN_DIGIT_BYTES); i++)
    {
        word = (word << 8) | src[i];
    }
    return word;
}

void cc_u8_to_bn(const uint8_t *src, size_t byte_len, size_t bn_word_len, cc_bn_t *bn)
{
    int i;
    int bn_index = bn_word_len - 1;

    // 检查是否有足够空间存储数据
    if (byte_len > bn_word_len * CC_BN_DIGIT_BYTES)
    {
        return;
    }

    // 前方需要补多少字节的0
    int zero_pad_len = bn_word_len * CC_BN_DIGIT_BYTES - byte_len;

    for (i = 0; i < zero_pad_len / CC_BN_DIGIT_BYTES; i++)
    {
        bn[bn_index] = 0;
        bn_index -= 1;
    }
    // 最高的word需要从src中读取多少字节
    int left_u8_len = byte_len % CC_BN_DIGIT_BYTES;
    if (left_u8_len != 0)
    {
        bn[bn_index] = cc_u8_to_bn_word(src, left_u8_len);
        bn_index -= 1;
    }

    int src_index = left_u8_len;
    while (bn_index >= 0)
    {
        bn[bn_index] = cc_u8_to_bn_word(src + src_index, CC_BN_DIGIT_BYTES);
        bn_index -= 1;
        src_index += CC_BN_DIGIT_BYTES;
    }
}

void cc_bn_to_u8(const cc_bn_t *bn, size_t bn_word_len, uint8_t *dst)
{
    int i;
    int byte_len = bn_word_len * CC_BN_DIGIT_BYTES;

    for (i = 0; i < byte_len; i++)
    {
        dst[i] = (bn[bn_word_len - 1 - i / CC_BN_DIGIT_BYTES] >> (CC_BN_DIGIT_BITS - 8 - (i % CC_BN_DIGIT_BYTES) * 8)) & 0xFF;
    }
}

// bn will be filled with the minimum number of words needed to fit the byte array
//  return the number of words used in bn
size_t cc_u8_to_bn_fit(const uint8_t *src, size_t byte_len, cc_bn_t *bn)
{
    size_t bn_word_len = (byte_len + CC_BN_DIGIT_BYTES - 1) / CC_BN_DIGIT_BYTES;
    cc_u8_to_bn(src, byte_len, bn_word_len, bn);
    return bn_word_len;
}

// dst will be filled with the minimum number of bytes needed to fit the bn
//  return the number of bytes used in dst
size_t cc_bn_to_u8_fit(const cc_bn_t *bn, size_t bn_word_len, uint8_t *dst)
{
    int i, j;
    size_t n = 0;
    uint8_t tmp;

    for (i = bn_word_len - 1; i >= 0; i--)
    {
        if ((bn[i] != 0) || (n > 0)) // Skip leading zeros
        {
            for (j = CC_BN_DIGIT_BITS - 8; j >= 0; j -= 8)
            {
                tmp = (bn[i] >> j) & 0xFF;
                if (tmp != 0 || n > 0) // Skip leading zeros
                {
                    dst[n++] = (uint8_t)tmp;
                }
            }
        }
    }

    if (n == 0) // If all digits are zero, return at least one byte
    {
        dst[n++] = 0;
    }
    return n; // Return the number of bytes written
}

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

// compare A and {B, 0, 0, ...}, return 1 if A > {B, 0, 0, ...}
int cc_bn_cmp_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t B)
{
    int i;
    for (i = bn_word_len - 1; i >= 1; i -= 1)
    {
        if (A[i] != 0)
        {
            return 1;
        }
    }
    if (A[0] > B)
    {
        return 1;
    }
    else if (A[0] < B)
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

void cc_bn_xor(const cc_bn_t *src1, const cc_bn_t *src2, size_t bn_word_len, cc_bn_t *dst)
{
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        dst[i] = src1[i] ^ src2[i];
    }
}

cc_bn_t cc_bn_get_bit(const cc_bn_t *bn, size_t bit_index)
{
    int digit_index = bit_index / CC_BN_DIGIT_BITS;
    int bit_index_in_digit = bit_index % CC_BN_DIGIT_BITS;
    return (bn[digit_index] >> bit_index_in_digit) & 0x01;
}

void cc_bn_set_bit(cc_bn_t *bn, size_t bit_index, cc_bn_t bit)
{
    int digit_index = bit_index / CC_BN_DIGIT_BITS;
    int bit_index_in_digit = bit_index % CC_BN_DIGIT_BITS;
    bn[digit_index] = (bn[digit_index] & ~(1 << bit_index_in_digit)) | (bit << bit_index_in_digit);
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
            return i * CC_BN_DIGIT_BITS + j;
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
            return i * CC_BN_DIGIT_BYTES + j;
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

// rigth move 1 bit, bn_out = bn_in / 2
// bn_out can be alias for bn_in
void cc_bn_rshift_1(const cc_bn_t *bn_in, size_t bn_word_len, cc_bn_t *bn_out)
{
    size_t i;
    for (i = 0; i < bn_word_len - 1; i += 1)
    {
        bn_out[i] = (bn_in[i + 1] << (CC_BN_DIGIT_BITS - 1)) | (bn_in[i] >> 1);
    }
    bn_out[bn_word_len - 1] = bn_in[bn_word_len - 1] >> 1;
}

// left move 1 bit, bn_out = bn_in * 2, return higest bit moved out
// bn_out can be alias for bn_in
cc_bn_t cc_bn_lshift_1(const cc_bn_t *bn_in, size_t bn_word_len, cc_bn_t *bn_out)
{
    int i;
    cc_bn_t carry = bn_in[bn_word_len - 1] >> (CC_BN_DIGIT_BITS - 1);
    for (i = bn_word_len - 1; i > 0; i -= 1)
    {
        bn_out[i] = (bn_in[i] << 1) | (bn_in[i - 1] >> (CC_BN_DIGIT_BITS - 1));
    }
    bn_out[0] = bn_in[0] << 1;

    return carry;
}

// bn_out can be alias for bn_in
void cc_bn_lshift(const cc_bn_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_t *bn_out)
{
    int i;
    int shift_word_len = shift_bit_len / CC_BN_DIGIT_BITS;
    int shift_bit_len_in_word = shift_bit_len % CC_BN_DIGIT_BITS;
    int remaining_bit_len = CC_BN_DIGIT_BITS - shift_bit_len_in_word;

    if (shift_word_len >= bn_word_len)
    {
        // If shift length is greater than the number of words, result is zero
        cc_bn_set_zero(bn_out, bn_word_len);
        return;
    }

    if (shift_bit_len_in_word > 0)
    {
        for (i = bn_word_len - 1; i > shift_word_len; i--)
        {
            bn_out[i] = (bn_in[i - shift_word_len] << shift_bit_len_in_word) |
                        (bn_in[i - shift_word_len - 1] >> remaining_bit_len);
        }
        bn_out[shift_word_len] = bn_in[0] << shift_bit_len_in_word;
        for (i = shift_word_len - 1; i >= 0; i--)
        {
            bn_out[i] = 0;
        }
    }
    else
    {
        for (i = bn_word_len - 1; i >= shift_word_len; i--)
        {
            bn_out[i] = bn_in[i - shift_word_len];
        }
        for (i = shift_word_len - 1; i >= 0; i--)
        {
            bn_out[i] = 0; // Clear the lower words
        }
    }
}

// bn_out can be alias for bn_in
void cc_bn_rshift(const cc_bn_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_t *bn_out)
{
    int i;
    int shift_word_len = shift_bit_len / CC_BN_DIGIT_BITS;
    int shift_bit_len_in_word = shift_bit_len % CC_BN_DIGIT_BITS;
    int remaining_bit_len = CC_BN_DIGIT_BITS - shift_bit_len_in_word;

    if (shift_word_len >= bn_word_len)
    {
        // If shift length is greater than the number of words, result is zero
        cc_bn_set_zero(bn_out, bn_word_len);
        return;
    }

    if (shift_bit_len_in_word > 0)
    {
        for (i = 0; i < bn_word_len - shift_word_len - 1; i++)
        {
            bn_out[i] = (bn_in[i + shift_word_len] >> shift_bit_len_in_word) |
                        (bn_in[i + shift_word_len + 1] << remaining_bit_len);
        }
        bn_out[bn_word_len - shift_word_len - 1] = bn_in[bn_word_len - 1] >> shift_bit_len_in_word;
        for (i = bn_word_len - shift_word_len; i < bn_word_len; i++)
        {
            bn_out[i] = 0; // Clear the upper words
        }
    }
    else
    {
        for (i = 0; i < bn_word_len - shift_word_len; i++)
        {
            bn_out[i] = bn_in[i + shift_word_len];
        }
        for (i = bn_word_len - shift_word_len; i < bn_word_len; i++)
        {
            bn_out[i] = 0; // Clear the upper words
        }
    }
}

// R = A + d
// R can be alias for A
cc_bn_t cc_bn_add_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R)
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
cc_bn_t cc_bn_add_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R)
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
//  A_word_len >= B_word_len
cc_bn_t cc_bn_add_small(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R)
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
cc_bn_t cc_bn_add(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R)
{
    if (A_word_len < B_word_len)
    {
        return cc_bn_add_small(B, B_word_len, A, A_word_len, R);
    }
    else
    {
        return cc_bn_add_small(A, A_word_len, B, B_word_len, R);
    }
}

// R = A - d
cc_bn_t cc_bn_sub_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R)
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
// R can be alias for A or B
cc_bn_t cc_bn_sub_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R)
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
// A_word_len >= B_word_len
cc_bn_t cc_bn_sub_small(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R)
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
