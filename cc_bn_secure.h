#ifndef CC_BN_SECURE_H
#define CC_BN_SECURE_H

#include "cc_bn.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // 安全的常数时间比较，返回0表示相等，1表示不相等
    int cc_secure_memcmp(const uint8_t *a, const uint8_t *b, size_t size);

    int cc_bn_secure_cmp(const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len);

    bool cc_bn_secure_is_zero(const cc_bn_word_t *bn, size_t bn_word_len);

    bool cc_bn_secure_is_one(const cc_bn_word_t *bn, size_t bn_word_len);

    // flag=0: R=A; flag=1: R=B
    void cc_bn_const_time_select(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len, bool flag);

    void cc_bn_const_time_select3(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, size_t bn_word_len, bool selA, bool selB, bool selC);

    void cc_bn_const_time_select4(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, const cc_bn_word_t *D, size_t bn_word_len, bool selA, bool selB, bool selC, bool selD);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CC_BN_SECURE_H