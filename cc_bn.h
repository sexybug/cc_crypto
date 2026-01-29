#ifndef CC_BN_H
#define CC_BN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t cc_bn_word_t;
typedef uint64_t cc_bn_dword_t; // double word

#define CC_BN_WORD_BYTES sizeof(cc_bn_word_t)
#define CC_BN_WORD_BITS (CC_BN_WORD_BYTES * 8)
#define CC_BN_WORD_MAX (((cc_bn_dword_t)1 << CC_BN_WORD_BITS) - 1)

#define CC_BN_IS_ODD(bn) (((bn)[0]) & 1)
#define CC_BN_IS_EVEN(bn) (!CC_BN_IS_ODD(bn))

static inline size_t cc_bn_word_len_from_bit_len(size_t bit_len)
{
    return (bit_len + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
}

// set bn=0
void cc_bn_set_zero(cc_bn_word_t *bn, size_t bn_word_len);

// set bn=1
void cc_bn_set_one(cc_bn_word_t *bn, size_t bn_word_len);

// bn = {word, 0, 0, ...}
void cc_bn_from_word(cc_bn_word_t *bn, size_t bn_word_len, cc_bn_word_t word);

void cc_bn_from_words(cc_bn_word_t *bn, size_t bn_word_len, const cc_bn_word_t *words, size_t words_len);

bool cc_bn_is_zero(const cc_bn_word_t *bn, size_t bn_word_len);

bool cc_bn_is_one(const cc_bn_word_t *bn, size_t bn_word_len);

// compare A and {B, 0, 0, ...}, return 1 if A > {B, 0, 0, ...}
int cc_bn_cmp_word(const cc_bn_word_t *A, size_t bn_word_len, cc_bn_word_t B);
int cc_bn_cmp_words(const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);
int cc_bn_cmp(const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len);

void cc_bn_copy(cc_bn_word_t *dst, const cc_bn_word_t *src, size_t bn_word_len);

// swap A and B
void cc_bn_swap(cc_bn_word_t *A, cc_bn_word_t *B, size_t bn_word_len);

// bn = bit(n-1)bit(n-2)...bit(0), bit(n-1) is the most significant bit
cc_bn_word_t cc_bn_get_bit(const cc_bn_word_t *bn, size_t bit_index);
void cc_bn_set_bit(cc_bn_word_t *bn, size_t bit_index, cc_bn_word_t bit);

/**
 * @brief get value in window
 *
 * get the value in the specified bit window, starting from start_bit_index with size window_size.
 *
 * @param bn pointer to the big number stored as an array of words
 * @param start_bit_index starting bit index of the window, 0 is the least significant bit
 * @param window_size size of the window (in bits)
 * @return cc_bn_word_t value in the window
 *
 * @note if the requested window exceeds the actual bit length of the big number, the excess part will be considered as 0.
 */
cc_bn_word_t cc_bn_get_window(const cc_bn_word_t *bn, size_t bn_word_len, size_t start_bit_index, size_t window_size);

// return least significant bit index
// bn != 0
// example: 0x01 -> 0, 0x02 -> 1, 0x04 -> 2
size_t cc_bn_lsb(const cc_bn_word_t *bn, size_t bn_word_len);

size_t cc_bn_bit_len(const cc_bn_word_t *bn, size_t bn_word_len);
size_t cc_bn_byte_len(const cc_bn_word_t *bn, size_t bn_word_len);
size_t cc_bn_word_len(const cc_bn_word_t *bn, size_t bn_word_len);

// rigth move 1 bit, R = A / 2
// R can alias A
void cc_bn_rshift_1(cc_bn_word_t *R, const cc_bn_word_t *A, size_t bn_word_len);

// left move 1 bit, R = A * 2, return higest bit moved out
// R can alias A
cc_bn_word_t cc_bn_lshift_1(cc_bn_word_t *R, const cc_bn_word_t *A, size_t bn_word_len);

// R = A << shift_bit_len
// R can alias A
void cc_bn_lshift(cc_bn_word_t *R, const cc_bn_word_t *A, size_t bn_word_len, size_t shift_bit_len);

// R = A >> shift_bit_len
// shift_bit_len can be any number
// R can alias A
void cc_bn_rshift(cc_bn_word_t *R, const cc_bn_word_t *A, size_t bn_word_len, size_t shift_bit_len);

// R = A + d
// R can alias A
cc_bn_word_t cc_bn_add_word(cc_bn_word_t *R, const cc_bn_word_t *A, size_t bn_word_len, cc_bn_word_t d);

// R = A + B
// R can alias A B
cc_bn_word_t cc_bn_add_words(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

// R = A + B
// A_word_len must >= B_word_len
// R can alias A B
cc_bn_word_t cc_bn_add_small(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len);

// R = A + B
// R_word_len = max(A_word_len, B_word_len)
// R can alias A B
cc_bn_word_t cc_bn_add(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len);

// R = A - d
// R can alias A
// return borrow
cc_bn_word_t cc_bn_sub_word(cc_bn_word_t *R, const cc_bn_word_t *A, size_t bn_word_len, cc_bn_word_t d);

// R = A - B
// R can alias A B
// if A < B, return borrow = 1, and R = 2^n - (B - A),
// else return borrow = 0, and R = A - B
cc_bn_word_t cc_bn_sub_words(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

// R = A - B
// A_word_len must >= B_word_len
// R_word_len = A_word_len
// R can alias A B
cc_bn_word_t cc_bn_sub_small(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len);

#endif // CC_BN_H