
#include "cc_bn_secure.h"

// 安全的常数时间比较，返回0表示相等，1表示不相等
int cc_secure_memcmp(const uint8_t *a, const uint8_t *b, size_t size)
{
    volatile uint8_t result = 0;
    size_t i;
    for (i = 0; i < size; i++)
    {
        result |= a[i] ^ b[i];
    }
    return result != 0;
}

/**
 * @brief 安全比较两个大数是否相等
 *
 * @param A 第一个大数的指针，指向cc_bn_word_t类型的数组
 * @param B 第二个大数的指针，指向cc_bn_word_t类型的数组
 * @param bn_word_len 要比较的大数字数，以cc_bn_word_t为单位
 * @return int 返回0表示两个大数相等，非0表示不相等
 *
 * @note 该函数使用恒定时间比较算法，避免时序攻击
 * @warning 输入参数不能为NULL
 */
int cc_bn_secure_cmp(const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len)
{
    volatile cc_bn_word_t result = 0;
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        result |= A[i] ^ B[i];
    }
    return result != 0;
}

/**
 * @brief 检查大数是否为零（安全版本）
 *
 * 该函数以时序安全的方式检查一个大数是否为零。通过使用volatile关键字和按位或操作，
 * 确保无论输入为何值，函数执行时间都保持恒定，防止时序攻击。
 *
 * @param bn 指向要检查的大数的指针
 * @param bn_word_len 大数的字长（以cc_bn_word_t为单位）
 * @return bool 如果大数为零返回true，否则返回false
 */
bool cc_bn_secure_is_zero(const cc_bn_word_t *bn, size_t bn_word_len)
{
    volatile cc_bn_word_t result = 0;
    size_t i;
    for (i = 0; i < bn_word_len; i++)
    {
        result |= bn[i];
    }
    return result == 0;
}

bool cc_bn_secure_is_one(const cc_bn_word_t *bn, size_t bn_word_len)
{
    volatile cc_bn_word_t result = 0;
    result |= bn[0] ^ 1;
    size_t i;
    for (i = 1; i < bn_word_len; i++)
    {
        result |= bn[i];
    }
    return result == 0;
}
