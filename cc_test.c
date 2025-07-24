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
    printf("uint8_t %s[%zu] = {\n", title, len);
    for (i = 0; i < len; i++)
    {
        if (i % 16 == 0)
        {
            printf("    ");
        }
        printf("0x%02x, ", ptr[i]);
        if ((i + 1) % 16 == 0 || (i + 1) == len)
        {
            printf("\n");
        }
    }
    printf("};\n\n");
}

void print_u32(const char *title, const uint32_t *ptr, size_t len)
{
    size_t i;
    printf("uint32_t %s[%zu] = {\n", title, len);
    for (i = 0; i < len; i++)
    {
        if (i % 8 == 0)
        {
            printf("    ");
        }
        printf("0x%08x, ", ptr[i]);
        if ((i + 1) % 8 == 0 || (i + 1) == len)
        {
            printf("\n");
        }
    }
    printf("};\n\n");
}

void print_u64(const char *title, const uint64_t *ptr, size_t len)
{
    size_t i;
    printf("uint64_t %s[%zu] = {\n", title, len);
    for (i = 0; i < len; i++)
    {
        if (i % 4 == 0)
        {
            printf("    ");
        }
        printf("0x%016llx, ", ptr[i]);
        if ((i + 1) % 4 == 0 || (i + 1) == len)
        {
            printf("\n");
        }
    }
    printf("};\n\n");
}

void print_bn(const char *title, const cc_bn_t *bn, size_t len)
{
    int i;
    printf("%s (%zu bytes) = {", title, len * CC_BN_WORD_BYTES);

    for (i = len - 1; i >= 0; i--)
    {
        printf("%08x", bn[i]);
    }
    printf("}\n");
}
