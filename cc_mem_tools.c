
#include "cc_mem_tools.h"

// Swap bytes in a 32-bit unsigned integer
void cc_swap_u32_array(uint32_t *array, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        array[i] = cc_swap_u32(array[i]);
    }
}

void cc_memset_u32(uint32_t *dest, uint32_t value, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        dest[i] = value;
    }
}

void cc_memcpy_u32(uint32_t *dest, const uint32_t *src, size_t word_size)
{
    size_t i;
    for (i = 0; i < word_size; i++)
    {
        dest[i] = src[i];
    }
}

void cc_memxor(uint8_t *dst, const uint8_t *src1, const uint8_t *src2, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
    {
        dst[i] = src1[i] ^ src2[i];
    }
}

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