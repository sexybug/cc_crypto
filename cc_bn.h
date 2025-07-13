#ifndef CC_BN_H
#define CC_BN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// note: cc_bn_t must be uint32_t
typedef uint32_t cc_bn_t;

#define CC_BN_DIGIT_BYTES sizeof(cc_bn_t)
#define CC_BN_DIGIT_BITS (CC_BN_DIGIT_BYTES * 8)
#define CC_BN_DIGIT_MAX ((1ULL << CC_BN_DIGIT_BITS) - 1)
#define CC_BN_DIGIT_MASK ((1ULL << CC_BN_DIGIT_BITS) - 1)

void cc_u8_to_bn(const uint8_t *src, size_t byte_len, size_t bn_word_len, cc_bn_t *bn);

void cc_bn_to_u8(const cc_bn_t *bn, size_t bn_word_len, uint8_t *dst);

// bn will be filled with the minimum number of words needed to fit the byte array
//  return the number of words used in bn
size_t cc_u8_to_bn_fit(const uint8_t *src, size_t byte_len, cc_bn_t *bn);

// dst will be filled with the minimum number of bytes needed to fit the bn
//  return the number of bytes used in dst
size_t cc_bn_to_u8_fit(const cc_bn_t *bn, size_t bn_word_len, uint8_t *dst);

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

void cc_bn_xor(const cc_bn_t *src1, const cc_bn_t *src2, size_t bn_word_len, cc_bn_t *dst);

cc_bn_t cc_bn_get_bit(const cc_bn_t *bn, size_t bit_index);
void cc_bn_set_bit(cc_bn_t *bn, size_t bit_index, cc_bn_t bit);

size_t cc_bn_bit_len(const cc_bn_t *bn, size_t bn_word_len);
size_t cc_bn_byte_len(const cc_bn_t *bn, size_t bn_word_len);
size_t cc_bn_word_len(const cc_bn_t *bn, size_t bn_word_len);

// rigth move 1 bit, bn_out = bn_in / 2
// bn_out can be alias for bn_in
void cc_bn_rshift_1(const cc_bn_t *bn_in, size_t bn_word_len, cc_bn_t *bn_out);

// left move 1 bit, bn_out = bn_in * 2, return higest bit moved out
// bn_out can be alias for bn_in
cc_bn_t cc_bn_lshift_1(const cc_bn_t *bn_in, size_t bn_word_len, cc_bn_t *bn_out);

// bn_out can be alias for bn_in
void cc_bn_lshift(const cc_bn_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_t *bn_out);

// bn_out can be alias for bn_in
void cc_bn_rshift(const cc_bn_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_t *bn_out);

// R = A + d
cc_bn_t cc_bn_add_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = A + B
cc_bn_t cc_bn_add_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R);

// R = A + B
//  A_word_len >= B_word_len
cc_bn_t cc_bn_add_small(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R);

// R = A + B
cc_bn_t cc_bn_add(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R);

// R = A - d
cc_bn_t cc_bn_sub_word(const cc_bn_t *A, size_t bn_word_len, cc_bn_t d, cc_bn_t *R);

// R = A - B
// R can be alias for A or B
cc_bn_t cc_bn_sub_words(const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len, cc_bn_t *R);

// R = A - B
// A_word_len >= B_word_len
cc_bn_t cc_bn_sub_small(const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, cc_bn_t *R);

#endif // CC_BN_H