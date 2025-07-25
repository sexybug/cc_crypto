#ifndef CC_BN_H
#define CC_BN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t cc_bn_t;
typedef uint64_t cc_bn_dword_t; // double word

#define CC_BN_WORD_BYTES sizeof(cc_bn_t)
#define CC_BN_WORD_BITS (CC_BN_WORD_BYTES * 8)
#define CC_BN_WORD_MAX (((cc_bn_dword_t)1 << CC_BN_WORD_BITS) - 1)

// set bn=0
void cc_bn_set_zero(cc_bn_t *bn, size_t bn_word_len);

// set bn=1
void cc_bn_set_one(cc_bn_t *bn, size_t bn_word_len);

// bn = {word, 0, 0, ...}
void cc_bn_from_word(cc_bn_t *bn, size_t bn_word_len, cc_bn_t word);

void cc_bn_from_words(cc_bn_t *bn, size_t bn_word_len, const cc_bn_t *words, size_t words_len);

bool cc_bn_is_zero(const cc_bn_t *bn, size_t bn_word_len);

bool cc_bn_is_one(const cc_bn_t *bn, size_t bn_word_len);

// compare A and {B, 0, 0, ...}, return 1 if A > {B, 0, 0, ...}
int cc_bn_cmp_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t B);
int cc_bn_cmp_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);
int cc_bn_cmp(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

void cc_bn_copy(cc_bn_t *dst, const cc_bn_t *src, size_t bn_word_len);

// swap A and B
void cc_bn_swap(cc_bn_t *A, cc_bn_t *B, size_t bn_word_len);

// bn = bit(n-1)bit(n-2)...bit(0), bit(n-1) is the most significant bit
cc_bn_t cc_bn_get_bit(const cc_bn_t *bn, size_t bit_index);
void cc_bn_set_bit(cc_bn_t *bn, size_t bit_index, cc_bn_t bit);

// return least significant bit index
// bn != 0
// example: 0x01 -> 0, 0x02 -> 1, 0x04 -> 2
size_t cc_bn_lsb(const cc_bn_t *bn, size_t bn_word_len);

size_t cc_bn_bit_len(const cc_bn_t *bn, size_t bn_word_len);
size_t cc_bn_byte_len(const cc_bn_t *bn, size_t bn_word_len);
size_t cc_bn_word_len(const cc_bn_t *bn, size_t bn_word_len);

// rigth move 1 bit, R = A / 2
// R can alias A
void cc_bn_rshift_1(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len);

// left move 1 bit, R = A * 2, return higest bit moved out
// R can alias A
cc_bn_t cc_bn_lshift_1(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len);

// R = A << shift_bit_len
// R can alias A
void cc_bn_lshift(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, size_t shift_bit_len);

// R = A >> shift_bit_len
// R can alias A
void cc_bn_rshift(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, size_t shift_bit_len);

// R = A + d
// R can alias A
cc_bn_t cc_bn_add_word(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d);

// R = A + B
// R can alias A B
cc_bn_t cc_bn_add_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);

// R = A + B
// A_word_len must >= B_word_len
// R can alias A B
cc_bn_t cc_bn_add_small(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

// R = A + B
// R can alias A B
cc_bn_t cc_bn_add(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

// R = A - d
// R can alias A
cc_bn_t cc_bn_sub_word(cc_bn_t *R, const cc_bn_t *A, size_t bn_word_len, cc_bn_t d);

// R = A - B
// R can alias A B
// if A < B, return borrow = 1, and R = 2^n - (B - A),
// else return borrow = 0, and R = A - B
cc_bn_t cc_bn_sub_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len);

// R = A - B
// A_word_len must >= B_word_len
// R_word_len = A_word_len
// R can alias A B
cc_bn_t cc_bn_sub_small(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len);

#endif // CC_BN_H