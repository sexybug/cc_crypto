
#ifndef CC_BN_CONVERT_H
#define CC_BN_CONVERT_H

#include "cc_bn.h"

void cc_u8_to_bn(const uint8_t *src, size_t byte_len, cc_bn_t *bn, size_t bn_word_len);

// return the number of bytes used in dst
size_t cc_bn_to_u8(const cc_bn_t *bn, size_t bn_word_len, uint8_t *dst);

// bn will be filled with the minimum number of words needed to fit the byte array
// return the number of words used in bn
size_t cc_u8_to_bn_fit(const uint8_t *src, size_t byte_len, cc_bn_t *bn);

// dst will be filled with the minimum number of bytes needed to fit the bn
// return the number of bytes used in dst
size_t cc_bn_to_u8_fit(const cc_bn_t *bn, size_t bn_word_len, uint8_t *dst);

// hex string can be odd length
void hex_to_bn(const char *str, cc_bn_t *bn, size_t bn_word_len);

// note: hex string ends with '\0', you must make sure memory is enough
// return hex string length
size_t bn_to_hex(const cc_bn_t *bn, size_t bn_word_len, char *hex);

// bn will be filled with the minimum number of words needed to fit the hex string
// hex string can be odd length
// return the number of words used in bn
size_t hex_to_bn_fit(const char *str, cc_bn_t *bn);

// note: hex string ends with '\0', you must make sure memory is enough
// dst will be filled with the minimum number of hex chars needed to fit the bn
// return the number of hex chars in dst
size_t bn_to_hex_fit(const cc_bn_t *bn, size_t bn_word_len, char *hex_str);

#endif // CC_BN_CONVERT_H