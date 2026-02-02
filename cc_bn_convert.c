
#include "cc_bn_convert.h"
#include <string.h>

static cc_bn_word_t cc_bn_word_from_u8(const uint8_t *src, size_t byte_len)
{
    size_t i;
    cc_bn_word_t word = 0;
    for (i = 0; (i < byte_len) && (i < CC_BN_WORD_BYTES); i++)
    {
        word = (word << 8) | src[i];
    }
    return word;
}

void cc_bn_from_u8(cc_bn_word_t *bn, size_t bn_word_len, const uint8_t *src, size_t byte_len)
{
    int i;
    int bn_index = bn_word_len - 1;

    // check if there is enough space to store the data
    if (byte_len > bn_word_len * CC_BN_WORD_BYTES)
    {
        return;
    }

    // how many leading zero bytes to pad
    int zero_pad_len = bn_word_len * CC_BN_WORD_BYTES - byte_len;

    for (i = 0; i < zero_pad_len / CC_BN_WORD_BYTES; i++)
    {
        bn[bn_index] = 0;
        bn_index -= 1;
    }

    // the highest word needs to read how many bytes from src
    int left_u8_len = byte_len % CC_BN_WORD_BYTES;
    if (left_u8_len != 0)
    {
        bn[bn_index] = cc_bn_word_from_u8(src, left_u8_len);
        bn_index -= 1;
    }

    int src_index = left_u8_len;
    while (bn_index >= 0)
    {
        bn[bn_index] = cc_bn_word_from_u8(src + src_index, CC_BN_WORD_BYTES);
        bn_index -= 1;
        src_index += CC_BN_WORD_BYTES;
    }
}

// return the number of bytes used in dst
size_t cc_bn_to_u8(uint8_t *dst, const cc_bn_word_t *bn, size_t bn_word_len)
{
    int i, j;
    int byte_len = bn_word_len * CC_BN_WORD_BYTES;

    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t word = bn[bn_word_len - 1 - i];
        for (j = 0; j < CC_BN_WORD_BYTES; j++)
        {
            dst[i * CC_BN_WORD_BYTES + j] = (word >> (CC_BN_WORD_BITS - 8 - j * 8)) & 0xFF;
        }
    }
    return byte_len;
}

// bn will be filled with the minimum number of words needed to fit the byte array
// return the number of words used in bn
size_t cc_bn_from_u8_fit(cc_bn_word_t *bn, const uint8_t *src, size_t byte_len)
{
    size_t bn_word_len = (byte_len + CC_BN_WORD_BYTES - 1) / CC_BN_WORD_BYTES;
    cc_bn_from_u8(bn, bn_word_len, src, byte_len);
    return bn_word_len;
}

// dst will be filled with the minimum number of bytes needed to fit the bn
// return the number of bytes used in dst
size_t cc_bn_to_u8_fit(uint8_t *dst, const cc_bn_word_t *bn, size_t bn_word_len)
{
    int i, j;
    size_t n = 0;
    uint8_t tmp;

    for (i = bn_word_len - 1; i >= 0; i--)
    {
        if ((bn[i] != 0) || (n > 0)) // Skip leading zeros
        {
            for (j = CC_BN_WORD_BITS - 8; j >= 0; j -= 8)
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

static inline uint8_t hex_char_to_u8(char c)
{
    if (c >= '0' && c <= '9')
    {
        return (c - '0');
    }
    else if (c >= 'a' && c <= 'f')
    {
        return 10 + (c - 'a');
    }
    else if (c >= 'A' && c <= 'F')
    {
        return 10 + (c - 'A');
    }
    else
    {
        return -1;
    }
}
static inline char u8_to_hex_char(uint8_t value)
{
    if (value <= 9)
    {
        return '0' + value;
    }
    else if (value <= 15)
    {
        return 'A' + (value - 10);
    }
    return '?'; // Error case
}

cc_bn_word_t cc_bn_word_from_hex(const char *str, size_t char_size)
{
    size_t i;
    cc_bn_word_t bn = 0;
    for (i = 0; (i < char_size) && (i < CC_BN_WORD_BYTES * 2); i++)
    {
        bn = (bn << 4) | hex_char_to_u8(str[i]);
    }
    return bn;
}

// hex string can be odd length
void cc_bn_from_hex(cc_bn_word_t *bn, size_t bn_word_len, const char *str)
{
    int i;
    size_t str_len = strlen(str);
    int byte_len = (str_len + 1) / 2;
    int bn_index = bn_word_len - 1;

    if (byte_len > bn_word_len * CC_BN_WORD_BYTES)
    {
        // Handle error: input string is too long
        return;
    }

    int zero_pad_len = bn_word_len * CC_BN_WORD_BYTES - byte_len;
    for (i = 0; i < zero_pad_len / CC_BN_WORD_BYTES; i++)
    {
        bn[bn_index] = 0;
        bn_index -= 1;
    }

    int left_str_len = str_len % (CC_BN_WORD_BYTES * 2);
    if (left_str_len != 0)
    {
        bn[bn_index] = cc_bn_word_from_hex(str, left_str_len);
        bn_index -= 1;
    }

    int str_index = left_str_len;
    while (bn_index >= 0)
    {
        bn[bn_index] = cc_bn_word_from_hex(str + str_index, CC_BN_WORD_BYTES * 2);
        bn_index -= 1;
        str_index += CC_BN_WORD_BYTES * 2;
    }
}

// note: hex string ends with '\0', you must make sure memory is enough
// return hex string length
size_t cc_bn_to_hex(char *hex, const cc_bn_word_t *bn, size_t bn_word_len)
{
    size_t i, j;
    size_t hex_len = bn_word_len * CC_BN_WORD_BYTES * 2;

    cc_bn_word_t tmp;
    for (i = 0; i < bn_word_len; i++)
    {
        tmp = bn[bn_word_len - 1 - i]; // from significant word
        for (j = 0; j < CC_BN_WORD_BYTES * 2; j++)
        {
            hex[i * CC_BN_WORD_BYTES * 2 + j] = u8_to_hex_char((tmp >> (CC_BN_WORD_BITS - 4 - j * 4)) & 0x0F);
        }
    }
    hex[hex_len] = '\0';
    return hex_len;
}

// bn will be filled with the minimum number of words needed to fit the hex string
// hex string can be odd length
// return the number of words used in bn
size_t cc_bn_from_hex_fit(cc_bn_word_t *bn, const char *str)
{
    size_t str_len = strlen(str);
    size_t bn_word_len = (str_len + 1) / (CC_BN_WORD_BYTES * 2);
    cc_bn_from_hex(bn, bn_word_len, str);
    return bn_word_len;
}

// note: hex string ends with '\0', you must make sure memory is enough
// dst will be filled with the minimum number of hex chars needed to fit the bn
// return the number of hex chars in dst
size_t cc_bn_to_hex_fit(char *hex_str, const cc_bn_word_t *bn, size_t bn_word_len)
{
    int i, j;
    size_t n = 0;
    uint8_t tmp;

    for (i = bn_word_len - 1; i >= 0; i--)
    {
        if ((bn[i] != 0) || (n > 0)) // Skip leading zeros
        {
            for (j = CC_BN_WORD_BITS - 4; j >= 0; j -= 4)
            {
                tmp = (bn[i] >> j) & 0x0F;
                if (tmp != 0 || n > 0) // Skip leading zeros
                {
                    hex_str[n++] = u8_to_hex_char(tmp);
                }
            }
        }
    }
    if (n == 0) // If all digits are zero, return "00"
    {
        hex_str[n++] = '0';
        hex_str[n++] = '0';
    }
    hex_str[n] = '\0';
    return n;
}