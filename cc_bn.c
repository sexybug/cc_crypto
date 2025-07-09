#include "cc_bn.h"

void cc_u8_to_bn(const uint8_t *src, size_t byte_len, size_t bn_word_len, cc_bn_digit_t *bn)
{
    int i, j;

    // 检查是否有足够空间存储数据
    if (byte_len > bn_word_len * CC_BN_DIGIT_BYTES)
    {
        return;
    }

    // 前方需要补多少字节的0
    int zero_pad_len = bn_word_len * CC_BN_DIGIT_BYTES - byte_len;

    for (i = 0; i < zero_pad_len / CC_BN_DIGIT_BYTES; i++)
    {
        bn[bn_word_len - 1 - i] = 0;
    }

    int src_index = 0;
    if (zero_pad_len % CC_BN_DIGIT_BYTES != 0)
    {
        src_index = CC_BN_DIGIT_BYTES - zero_pad_len % CC_BN_DIGIT_BYTES;

        cc_bn_digit_t tmp = 0;
        for (i = 0; i < src_index; i++)
        {
            tmp = (tmp << 8) | src[i];
        }
        bn[bn_word_len - 1 - zero_pad_len / CC_BN_DIGIT_BYTES] = tmp;
    }

    for (i = (zero_pad_len + 3) / CC_BN_DIGIT_BYTES; i < bn_word_len; i++)
    {
        cc_bn_digit_t tmp = 0;
        for (j = 0; j < CC_BN_DIGIT_BYTES; j++)
        {
            tmp = (tmp << 8) | src[src_index];
            src_index++;
        }
        bn[bn_word_len - 1 - i] = tmp;
    }
}

void cc_bn_to_u8(const cc_bn_digit_t *bn, size_t bn_word_len, uint8_t *dst)
{
    int i;
    int byte_len = bn_word_len * CC_BN_DIGIT_BYTES;

    for (i = 0; i < byte_len; i++)
    {
        dst[i] = (bn[bn_word_len - 1 - i / CC_BN_DIGIT_BYTES] >> (CC_BN_DIGIT_BITS - 8 - (i % CC_BN_DIGIT_BYTES) * 8)) & 0xFF;
    }
}

//bn will be filled with the minimum number of words needed to fit the byte array
// return the number of words used in bn
size_t cc_u8_to_bn_fit(const uint8_t *src, size_t byte_len, cc_bn_digit_t *bn)
{
    size_t bn_word_len = (byte_len + CC_BN_DIGIT_BYTES - 1) / CC_BN_DIGIT_BYTES;
    cc_u8_to_bn(src, byte_len, bn_word_len, bn);
    return bn_word_len;
}

//dst will be filled with the minimum number of bytes needed to fit the bn
// return the number of bytes used in dst
size_t cc_bn_to_u8_fit(const cc_bn_digit_t *bn, size_t bn_word_len, uint8_t *dst)
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
void cc_bn_set_zero(cc_bn_digit_t *bn, size_t bn_word_len)
{
    int i;
    for (i = 0; i < bn_word_len; i++)
    {
        bn[i] = 0;
    }
}
// set bn=1
void cc_bn_set_one(cc_bn_digit_t *bn, size_t bn_word_len)
{
    int i;
    bn[0] = 1;
    for (i = 1; i < bn_word_len; i++)
    {
        bn[i] = 0;
    }
}

bool cc_bn_is_zero(const cc_bn_digit_t *bn, size_t bn_word_len)
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

int cc_bn_cmp(const cc_bn_digit_t *bn1, const cc_bn_digit_t *bn2, size_t bn_word_len)
{
    int i;
    for (i = bn_word_len - 1; i >= 0; i -= 1)
    {
        if (bn1[i] > bn2[i])
        {
            return 1;
        }
        else if (bn1[i] < bn2[i])
        {
            return -1;
        }
    }
    return 0;
}

void cc_bn_copy(cc_bn_digit_t *dst, const cc_bn_digit_t *src, size_t bn_word_len)
{
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        dst[i] = src[i];
    }
}

void cc_bn_xor(const cc_bn_digit_t *src1, const cc_bn_digit_t *src2, size_t bn_word_len, cc_bn_digit_t *dst)
{
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        dst[i] = src1[i] ^ src2[i];
    }
}

cc_bn_digit_t cc_bn_get_bit(const cc_bn_digit_t *bn, size_t bit_index)
{
    int digit_index = bit_index / CC_BN_DIGIT_BITS;
    int bit_index_in_digit = bit_index % CC_BN_DIGIT_BITS;
    return (bn[digit_index] >> bit_index_in_digit) & 0x01;
}
size_t cc_bn_bit_len(const cc_bn_digit_t *bn, size_t bn_word_len)
{
    int i, j;
    cc_bn_digit_t tmp;
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

size_t cc_bn_byte_len(const cc_bn_digit_t *bn, size_t bn_word_len)
{
    int i, j;
    cc_bn_digit_t tmp;
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

size_t cc_bn_word_len(const cc_bn_digit_t *bn, size_t bn_word_len)
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
void cc_bn_rshift_1(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t *bn_out)
{
    size_t i;
    for (i = 0; i < bn_word_len - 1; i += 1)
    {
        bn_out[i] = (bn_in[i + 1] << (CC_BN_DIGIT_BITS - 1)) | (bn_in[i] >> 1);
    }
    bn_out[bn_word_len - 1] = bn_in[bn_word_len - 1] >> 1;
}

// left move 1 bit, bn_out = bn_in * 2
void cc_bn_lshift_1(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t *bn_out)
{
    int i;
    for (i = bn_word_len - 1; i > 0; i -= 1)
    {
        bn_out[i] = (bn_in[i] << 1) | (bn_in[i - 1] >> (CC_BN_DIGIT_BITS - 1));
    }
    bn_out[0] = bn_in[0] << 1;
}

void cc_bn_lshift(const cc_bn_digit_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_digit_t *bn_out)
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

void cc_bn_rshift(const cc_bn_digit_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_digit_t *bn_out)
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

cc_bn_digit_t cc_bn_add_uint(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t d, cc_bn_digit_t *bn_out)
{
    size_t i;
    cc_bn_digit_t s, t;
    for (i = 0; i < bn_word_len; i++)
    {
        s = bn_in[i];
        t = s + d;
        d = (t < s);
        bn_out[i] = t;
    }
    return d; // return carry
}
cc_bn_digit_t cc_bn_add(const cc_bn_digit_t *A, const cc_bn_digit_t *B, size_t bn_word_len, cc_bn_digit_t *X)
{
    size_t i;
    cc_bn_digit_t carry = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_digit_t t = carry + A[i];
        carry = (t < A[i]);
        t += B[i];
        carry += (t < B[i]);
        X[i] = t;
    }
    return carry; // return carry
}

cc_bn_digit_t cc_bn_sub_uint(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t d, cc_bn_digit_t *bn_out)
{
    size_t i;
    cc_bn_digit_t s, t;
    for (i = 0; i < bn_word_len; i++)
    {
        s = bn_in[i];
        t = s - d;
        d = (t > s);
        bn_out[i] = t;
    }
    return d; // return borrow
}

cc_bn_digit_t cc_bn_sub(const cc_bn_digit_t *A, const cc_bn_digit_t *B, size_t bn_word_len, cc_bn_digit_t *X)
{
    size_t i;
    cc_bn_digit_t borrow = 0;
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_digit_t z = (A[i] < borrow);
        cc_bn_digit_t t = A[i] - borrow;
        borrow = (t < B[i]) + z;
        X[i] = t - B[i];
    }
    return borrow; // return borrow
}
