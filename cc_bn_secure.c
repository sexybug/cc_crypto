
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

// flag=0: R=A; flag=1: R=B
void cc_bn_const_time_select(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len, bool flag)
{
    // flag: 0 or 1
    size_t i;
    cc_bn_word_t mask = -flag; // all bits are flag
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t tmp = (A[i] & ~mask) | (B[i] & mask);
        R[i] = tmp;
    }
}

void cc_bn_const_time_select3(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, size_t bn_word_len, bool selA, bool selB, bool selC)
{
    // selA, selB, selC: 0 or 1
    size_t i;
    cc_bn_word_t maskA = -selA; // all bits are selA
    cc_bn_word_t maskB = -selB; // all bits are selB
    cc_bn_word_t maskC = -selC; // all bits are selC
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t tmp = (A[i] & maskA) | (B[i] & maskB) | (C[i] & maskC);
        R[i] = tmp;
    }
}

void cc_bn_const_time_select4(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, const cc_bn_word_t *D, size_t bn_word_len, bool selA, bool selB, bool selC, bool selD)
{
    // selA, selB, selC, selD: 0 or 1
    size_t i;
    cc_bn_word_t maskA = -selA; // all bits are selA
    cc_bn_word_t maskB = -selB; // all bits are selB
    cc_bn_word_t maskC = -selC; // all bits are selC
    cc_bn_word_t maskD = -selD; // all bits are selD
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t tmp = (A[i] & maskA) | (B[i] & maskB) | (C[i] & maskC) | (D[i] & maskD);
        R[i] = tmp;
    }
}