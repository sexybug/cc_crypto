
#include "cc_mem_tools.h"

void cc_u16_to_u8(uint16_t n, uint8_t out[2])
{
    out[0] = (n >> 8) & 0xFF;
    out[1] = n & 0xFF;
}

void cc_u32_to_u8(uint32_t n, uint8_t out[4])
{
    out[0] = (n >> 24) & 0xFF;
    out[1] = (n >> 16) & 0xFF;
    out[2] = (n >> 8) & 0xFF;
    out[3] = n & 0xFF;
}

void cc_u64_to_u8(uint64_t n, uint8_t out[8])
{
    out[0] = (n >> 56) & 0xFF;
    out[1] = (n >> 48) & 0xFF;
    out[2] = (n >> 40) & 0xFF;
    out[3] = (n >> 32) & 0xFF;
    out[4] = (n >> 24) & 0xFF;
    out[5] = (n >> 16) & 0xFF;
    out[6] = (n >> 8) & 0xFF;
    out[7] = n & 0xFF;
}

uint16_t cc_u8_to_u16(const uint8_t in[2])
{
    return (in[0] << 8) | in[1];
}

uint32_t cc_u8_to_u32(const uint8_t in[4])
{
    return (in[0] << 24) | (in[1] << 16) | (in[2] << 8) | in[3];
}

uint64_t cc_u8_to_u64(const uint8_t in[8])
{
    return (uint64_t)in[0] << 56 | (uint64_t)in[1] << 48 | (uint64_t)in[2] << 40 | (uint64_t)in[3] << 32 |
           (uint64_t)in[4] << 24 | (uint64_t)in[5] << 16 | (uint64_t)in[6] << 8 | in[7];
}

void cc_memset_u32(uint32_t *dst, uint32_t value, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        dst[i] = value;
    }
}

// dst cannot alias src
void cc_memcpy_u32(uint32_t *dst, const uint32_t *src, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        dst[i] = src[i];
    }
}

// dst can alias src1 src2
void cc_memxor(uint8_t *dst, const uint8_t *src1, const uint8_t *src2, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
    {
        dst[i] = src1[i] ^ src2[i];
    }
}

// dst can alias src1 src2
void cc_memxor_u32(uint32_t *dst, const uint32_t *src1, const uint32_t *src2, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        dst[i] = src1[i] ^ src2[i];
    }
}

int cc_memcmp_u32(const uint32_t *src1, const uint32_t *src2, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        if (src1[i] != src2[i])
        {
            return (src1[i] < src2[i]) ? -1 : 1;
        }
    }
    return 0;
}

// reverse bytes in an array of 8-bit unsigned integers
void cc_reverse_u8_array(uint8_t *array, size_t len)
{
    size_t i;
    for (i = 0; i < len / 2; i++)
    {
        uint8_t temp = array[i];
        array[i] = array[len - i - 1];
        array[len - i - 1] = temp;
    }
}

// reverse words in an array of 16-bit unsigned integers
void cc_reverse_u16_array(uint16_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size / 2; i++)
    {
        uint16_t temp = array[i];
        array[i] = array[word_size - i - 1];
        array[word_size - i - 1] = temp;
    }
}

// reverse words in an array of 32-bit unsigned integers
void cc_reverse_u32_array(uint32_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size / 2; i++)
    {
        uint32_t temp = array[i];
        array[i] = array[word_size - i - 1];
        array[word_size - i - 1] = temp;
    }
}

// reverse words in an array of 64-bit unsigned integers
void cc_reverse_u64_array(uint64_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size / 2; i++)
    {
        uint64_t temp = array[i];
        array[i] = array[word_size - i - 1];
        array[word_size - i - 1] = temp;
    }
}

// Swap bytes in a 16-bit unsigned integer
void cc_swap_u16_array(uint16_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        array[i] = cc_swap_u16(array[i]);
    }
}

// Swap bytes in a 32-bit unsigned integer
void cc_swap_u32_array(uint32_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        array[i] = cc_swap_u32(array[i]);
    }
}

// Swap bytes in a 64-bit unsigned integer
void cc_swap_u64_array(uint64_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        array[i] = cc_swap_u64(array[i]);
    }
}