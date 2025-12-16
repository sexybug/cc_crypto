#ifndef CC_BN_SECURE_H
#define CC_BN_SECURE_H

#include "cc_bn.h"

#ifdef __cplusplus
extern "C"
{
    // 安全的常数时间比较，返回0表示相等，1表示不相等
    int cc_secure_memcmp(const uint8_t *a, const uint8_t *b, size_t size);

    int cc_bn_secure_cmp(const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

    bool cc_bn_secure_is_zero(const cc_bn_word_t *bn, size_t bn_word_len);

    bool cc_bn_secure_is_one(const cc_bn_word_t *bn, size_t bn_word_len);
}
#endif // __cplusplus

#endif // CC_BN_SECURE_H