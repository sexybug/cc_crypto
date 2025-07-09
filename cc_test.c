#include "cc_test.h"
#include <stdio.h>
#include <string.h>

void dump_mem(const void *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (i % 8 == 0 && i != 0)
        {
            // printf(" ");
        }
        if (i % 16 == 0 && i != 0)
            printf("\n");
        printf("%02X", *((uint8_t *)ptr + i));
        // printf("%02x ", *((uint8_t *)ptr + i));
    }
    printf("\n");
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
    if (value >= 0 && value <= 9)
    {
        return '0' + value;
    }
    else if (value >= 10 && value <= 15)
    {
        return 'A' + (value - 10);
    }
    return '?'; // Error case
}

void HexString2Hex(const char *str, int len, uint8_t *out)
{
    int i;
    for (i = 0; i < len; i++)
    {
        out[i] = (hex_char_to_u8(str[i * 2]) << 4) | hex_char_to_u8(str[i * 2 + 1]);
    }
}

// return byte length of out
size_t hex_to_u8(const char *str, uint8_t *out)
{
    if (str == NULL || out == NULL)
    {
        return 0; // Handle null pointers
    }

    if (strlen(str) % 2 != 0)
    {
        return 0; // Handle odd-length strings
    }

    // Calculate the length of the output array
    size_t len = strlen(str) / 2;
    size_t i;
    for (i = 0; i < len; i++)
    {
        out[i] = (hex_char_to_u8(str[i * 2]) << 4) | hex_char_to_u8(str[i * 2 + 1]);
    }
    return len; // Return the number of bytes written
}

// return hex string length
size_t u8_to_hex(const uint8_t *src, size_t src_len, char *hex)
{
    size_t i;
    for (i = 0; i < src_len; i++)
    {
        hex[i * 2] = u8_to_hex_char(src[i] >> 4);
        hex[i * 2 + 1] = u8_to_hex_char(src[i] & 0x0F);
    }
    hex[src_len * 2] = '\0';
    return src_len * 2;
}

/**
 * Prints the binary representation of the given byte array.
 *
 * @param ptr pointer to the byte array to be printed
 * @param len length of the byte array
 *
 * @return void
 *
 * @throws None
 */
void print_binary(const uint8_t *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            printf("%d", (ptr[i] >> j) & 0x01);
        }
        printf(" ");
    }
    printf("\n");
}

void print_u8(const char *title, const uint8_t *ptr, size_t len)
{
    size_t i;
    printf("uint8_t %s[%d] = {\n", title, len);
    for (i = 0; i < len; i++)
    {
        if (i % 16 == 0)
        {
            printf("    ");
        }
        printf("0x%02x, ", ptr[i]);
        if ((i + 1) % 16 == 0 || ((i + 1) == len && (i + 1) % 16 != 0))
        {
            printf("\n");
        }
    }
    printf("};\n\n");
}

void print_u32(const char *title, const uint32_t *ptr, size_t len)
{
    size_t i;
    printf("uint32_t %s[%d] = {\n", title, len);
    for (i = 0; i < len; i++)
    {
        if (i % 8 == 0)
        {
            printf("    ");
        }
        printf("0x%08x, ", ptr[i]);
        if ((i + 1) % 8 == 0 || ((i + 1) == len && (i + 1) % 8 != 0))
        {
            printf("\n");
        }
    }
    printf("};\n\n");
}

void print_u64(const char *title, const uint64_t *ptr, size_t len)
{
    size_t i;
    printf("uint64_t %s[%d] = {\n", title, len);
    for (i = 0; i < len; i++)
    {
        if (i % 4 == 0)
        {
            printf("    ");
        }
        printf("0x%016lx, ", ptr[i]);
        if ((i + 1) % 4 == 0 || ((i + 1) == len && (i + 1) % 4 != 0))
        {
            printf("\n");
        }
    }
    printf("};\n\n");
}

void print_bn(const char *title, const cc_bn_digit_t *bn, size_t len)
{
    int i;
    printf("%s (%d bytes) = {", title, len * CC_BN_DIGIT_BYTES);

    for (i = len - 1; i >= 0; i--)
    {
        printf("%08x", bn[i]);
    }
    printf("}\n");
}

cc_bn_digit_t hex_to_bn_word(const char *str, size_t char_size)
{
    size_t i;
    cc_bn_digit_t bn = 0;
    for (i = 0; (i < char_size) && (i < CC_BN_DIGIT_BYTES * 2); i++)
    {
        bn = (bn << 4) | hex_char_to_u8(str[i]);
    }
    return bn;
}
void hex_to_bn(const char *str, cc_bn_digit_t *bn, size_t bn_word_len)
{
    int i;
    int str_len = strlen(str);
    int byte_len = (str_len + 1) / 2;
    int bn_index = bn_word_len - 1;

    if (byte_len > bn_word_len * CC_BN_DIGIT_BYTES)
    {
        // Handle error: input string is too long
        return;
    }

    int zero_pad_len = bn_word_len * CC_BN_DIGIT_BYTES - byte_len;
    for (i = 0; i < zero_pad_len / CC_BN_DIGIT_BYTES; i++)
    {
        bn[bn_index] = 0;
        bn_index -= 1;
    }

    int left_str_len = str_len % (CC_BN_DIGIT_BYTES * 2);
    if (left_str_len != 0)
    {
        bn[bn_index] = hex_to_bn_word(str, left_str_len);
        bn_index -= 1;
    }

    int str_index = left_str_len;
    while (bn_index >= 0)
    {
        bn[bn_index] = hex_to_bn_word(str + str_index, CC_BN_DIGIT_BYTES * 2);
        bn_index -= 1;
        str_index += CC_BN_DIGIT_BYTES * 2;
    }
}

// return hex string length
size_t bn_to_hex(const cc_bn_digit_t *bn, size_t bn_word_len, char *hex)
{
    size_t i,j;
    size_t hex_len = bn_word_len * CC_BN_DIGIT_BYTES * 2;

    cc_bn_digit_t tmp;
    for (i = 0; i < bn_word_len; i++)
    {
        tmp = bn[bn_word_len - 1 - i]; // 从高位开始处理
        for (j = 0; j < CC_BN_DIGIT_BYTES * 2; j++)
        {
            hex[i * CC_BN_DIGIT_BYTES * 2 + j] = u8_to_hex_char((tmp >> (CC_BN_DIGIT_BITS - 4 - j * 4)) & 0x0F);
        }

    }
    hex[hex_len] = '\0';
    return hex_len;
}