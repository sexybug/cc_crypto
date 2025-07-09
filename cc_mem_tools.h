
#ifndef CC_MEM_TOOLS_H
#define CC_MEM_TOOLS_H

#include <stdint.h>
#include <stddef.h>

#define __align4 __attribute__((aligned(4)))

#define CC_BYTE0(x) ((uint8_t)((x) & 0xFF))
#define CC_BYTE1(x) ((uint8_t)(((x) >> 8) & 0xFF))
#define CC_BYTE2(x) ((uint8_t)(((x) >> 16) & 0xFF))
#define CC_BYTE3(x) ((uint8_t)(((x) >> 24) & 0xFF))
#define CC_BYTE4(x) ((uint8_t)(((x) >> 32) & 0xFF))
#define CC_BYTE5(x) ((uint8_t)(((x) >> 40) & 0xFF))
#define CC_BYTE6(x) ((uint8_t)(((x) >> 48) & 0xFF))
#define CC_BYTE7(x) ((uint8_t)(((x) >> 56) & 0xFF))

static inline uint16_t cc_swap_u16(uint16_t x)
{
    return (x >> 8) | (x << 8);
}
static inline uint32_t cc_swap_u32(uint32_t x)
{
    return (x >> 24) | ((x >> 8) & 0xFF00) | ((x << 8) & 0xFF0000) | (x << 24);
}

static inline uint64_t cc_swap_u64(uint64_t x)
{
    return (x & 0x00000000000000FFULL) << 56 |
           (x & 0x000000000000FF00ULL) << 40 |
           (x & 0x0000000000FF0000ULL) << 24 |
           (x & 0x00000000FF000000ULL) << 8 |
           (x & 0x000000FF00000000ULL) >> 8 |
           (x & 0x0000FF0000000000ULL) >> 24 |
           (x & 0x00FF000000000000ULL) >> 40 |
           (x & 0xFF00000000000000ULL) >> 56;
}

#define CC_R8(addr) (*(volatile uint8_t *)(addr))
#define CC_R16(addr) (*(volatile uint16_t *)(addr))
#define CC_R32(addr) (*(volatile uint32_t *)(addr))
#define CC_R64(addr) (*(volatile uint64_t *)(addr))

#define CC_W8(addr, value) (*(volatile uint8_t *)(addr)) = (value)
#define CC_W16(addr, value) (*(volatile uint16_t *)(addr)) = (value)
#define CC_W32(addr, value) (*(volatile uint32_t *)(addr)) = (value)
#define CC_W64(addr, value) (*(volatile uint64_t *)(addr)) = (value)

#define CC_R16_SWAP(addr) cc_swap_u16(CC_R16(addr))
#define CC_R32_SWAP(addr) cc_swap_u32(CC_R32(addr))
#define CC_R64_SWAP(addr) cc_swap_u64(CC_R64(addr))
#define CC_W16_SWAP(addr, value) CC_W16(addr, cc_swap_u16(value))
#define CC_W32_SWAP(addr, value) CC_W32(addr, cc_swap_u32(value))
#define CC_W64_SWAP(addr, value) CC_W64(addr, cc_swap_u64(value))

void cc_swap_u32_array(uint32_t *array, size_t word_size);

void cc_memset_u32(uint32_t *dest, uint32_t value, size_t word_size);

void cc_memcpy_u32(uint32_t *dest, const uint32_t *src, size_t word_size);

void cc_memxor(uint8_t *dst, const uint8_t *src1, const uint8_t *src2, size_t len);

void cc_memxor_u32(uint32_t *dst, const uint32_t *src1, const uint32_t *src2, size_t word_size);

int cc_memcmp_u32(const uint32_t *src1, const uint32_t *src2, size_t word_size);

#endif // CC_MEM_TOOLS_H