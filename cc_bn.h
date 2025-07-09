#ifndef CC_BN_H
#define CC_BN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t cc_bn_digit_t;

#define CC_BN_DIGIT_BYTES sizeof(cc_bn_digit_t)
#define CC_BN_DIGIT_BITS (CC_BN_DIGIT_BYTES * 8)
#define CC_BN_DIGIT_MAX ((1ULL << CC_BN_DIGIT_BITS) - 1)

void cc_u8_to_bn(const uint8_t *src, size_t byte_len, size_t bn_word_len, cc_bn_digit_t *bn);

void cc_bn_to_u8(const cc_bn_digit_t *bn, size_t bn_word_len, uint8_t *dst);

// bn will be filled with the minimum number of words needed to fit the byte array
//  return the number of words used in bn
size_t cc_u8_to_bn_fit(const uint8_t *src, size_t byte_len, cc_bn_digit_t *bn);

// dst will be filled with the minimum number of bytes needed to fit the bn
//  return the number of bytes used in dst
size_t cc_bn_to_u8_fit(const cc_bn_digit_t *bn, size_t bn_word_len, uint8_t *dst);

// set bn=0
void cc_bn_set_zero(cc_bn_digit_t *bn, size_t bn_word_len);

// set bn=1
void cc_bn_set_one(cc_bn_digit_t *bn, size_t bn_word_len);

bool cc_bn_is_zero(const cc_bn_digit_t *bn, size_t bn_word_len);

int cc_bn_cmp(const cc_bn_digit_t *bn1, const cc_bn_digit_t *bn2, size_t bn_word_len);

void cc_bn_copy(cc_bn_digit_t *dst, const cc_bn_digit_t *src, size_t bn_word_len);

void cc_bn_xor(const cc_bn_digit_t *src1, const cc_bn_digit_t *src2, size_t bn_word_len, cc_bn_digit_t *dst);

cc_bn_digit_t cc_bn_get_bit(const cc_bn_digit_t *bn, size_t bit_index);

size_t cc_bn_bit_len(const cc_bn_digit_t *bn, size_t bn_word_len);
size_t cc_bn_byte_len(const cc_bn_digit_t *bn, size_t bn_word_len);
size_t cc_bn_word_len(const cc_bn_digit_t *bn, size_t bn_word_len);

// rigth move 1 bit, bn_out = bn_in / 2
void cc_bn_rshift_1(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t *bn_out);
// left move 1 bit, bn_out = bn_in * 2
void cc_bn_lshift_1(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t *bn_out);

void cc_bn_lshift(const cc_bn_digit_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_digit_t *bn_out);

void cc_bn_rshift(const cc_bn_digit_t *bn_in, size_t bn_word_len, size_t shift_bit_len, cc_bn_digit_t *bn_out);

cc_bn_digit_t cc_bn_add_uint(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t d, cc_bn_digit_t *bn_out);

cc_bn_digit_t cc_bn_add(const cc_bn_digit_t *A, const cc_bn_digit_t *B, size_t bn_word_len, cc_bn_digit_t *X);

cc_bn_digit_t cc_bn_sub_uint(const cc_bn_digit_t *bn_in, size_t bn_word_len, cc_bn_digit_t d, cc_bn_digit_t *bn_out);

cc_bn_digit_t cc_bn_sub(const cc_bn_digit_t *A, const cc_bn_digit_t *B, size_t bn_word_len, cc_bn_digit_t *X);

#endif // CC_BN_H