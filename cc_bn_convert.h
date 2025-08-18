
#ifndef CC_BN_CONVERT_H
#define CC_BN_CONVERT_H

#include "cc_bn.h"

void cc_bn_from_u8(cc_bn_word_t *bn, size_t bn_word_len, const uint8_t *src, size_t byte_len);

// return the number of bytes used in dst
size_t cc_bn_to_u8(uint8_t *dst, const cc_bn_word_t *bn, size_t bn_word_len);

// bn will be filled with the minimum number of words needed to fit the byte array
// return the number of words used in bn
size_t cc_bn_from_u8_fit(cc_bn_word_t *bn, const uint8_t *src, size_t byte_len);

// dst will be filled with the minimum number of bytes needed to fit the bn
// return the number of bytes used in dst
size_t cc_bn_to_u8_fit(uint8_t *dst, const cc_bn_word_t *bn, size_t bn_word_len);

// hex string can be odd length
void cc_bn_from_hex(cc_bn_word_t *bn, size_t bn_word_len, const char *str);

// note: hex string ends with '\0', you must make sure memory is enough
// return hex string length
size_t cc_bn_to_hex(char *hex, const cc_bn_word_t *bn, size_t bn_word_len);

// bn will be filled with the minimum number of words needed to fit the hex string
// hex string can be odd length
// return the number of words used in bn
size_t cc_bn_from_hex_fit(cc_bn_word_t *bn, const char *str);

// note: hex string ends with '\0', you must make sure memory is enough
// dst will be filled with the minimum number of hex chars needed to fit the bn
// return the number of hex chars in dst
size_t cc_bn_to_hex_fit(char *hex_str, const cc_bn_word_t *bn, size_t bn_word_len);

#endif // CC_BN_CONVERT_H