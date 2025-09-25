
#include "cc_mem_tools.h"

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