#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"

// 辅助函数：比较两个大数数组是否相等
int cc_bn_compare(const cc_bn_digit_t *a, const cc_bn_digit_t *b, size_t len)
{
    return memcmp(a, b, len * sizeof(cc_bn_digit_t));
}

// 单元测试函数
void test_cc_bn_rshift()
{
    printf("开始测试 cc_bn_rshift 函数...\n\n");

    // 测试用例1: 移位长度大于等于数组长度，结果应为零
    {
        printf("测试用例1: 移位长度大于数组长度\n");
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[2] = {0, 0};

        cc_bn_rshift(input, 2, 64, output); // 移位64位，等于2个字长
        assert(cc_bn_compare(output, expected, 2) == 0);
        printf("通过: 移位64位结果为零\n");

        cc_bn_rshift(input, 2, 100, output); // 移位100位，大于2个字长
        assert(cc_bn_compare(output, expected, 2) == 0);
        printf("通过: 移位100位结果为零\n\n");
    }

    // 测试用例2: 移位0位，结果应与输入相同
    {
        printf("测试用例2: 移位0位\n");
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98};
        cc_bn_digit_t output[3];

        cc_bn_rshift(input, 3, 0, output);
        assert(cc_bn_compare(output, input, 3) == 0);
        printf("通过: 移位0位结果与输入相同\n\n");
    }

    // 测试用例3: 按字边界移位（移位位数是CC_BN_DIGIT_BITS的倍数）
    {
        printf("测试用例3: 按字边界移位\n");
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x11223344};
        cc_bn_digit_t output[4];
        cc_bn_digit_t expected[] = {0x9ABCDEF0, 0xFEDCBA98, 0x11223344, 0};

        cc_bn_rshift(input, 4, 32, output); // 右移32位（1个字）
        assert(cc_bn_compare(output, expected, 4) == 0);
        printf("通过: 右移32位\n");

        cc_bn_digit_t expected2[] = {0xFEDCBA98, 0x11223344, 0, 0};
        cc_bn_rshift(input, 4, 64, output); // 右移64位（2个字）
        assert(cc_bn_compare(output, expected2, 4) == 0);
        printf("通过: 右移64位\n\n");
    }

    // 测试用例4: 非字边界移位（需要位操作）
    { // 9ABCDEF012345678
        printf("测试用例4: 非字边界移位\n");
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t output[2];

        // 右移4位
        cc_bn_rshift(input, 2, 4, output);
        // 0x12345678 >> 4 = 0x01234567，但还需要从高位借位
        // 0x9ABCDEF0的低4位是0，所以第一个字变成0x01234567
        // 0x9ABCDEF0 >> 4 = 0x09ABCDEF
        cc_bn_digit_t expected1[] = {0x01234567, 0x09ABCDEF};
        assert(cc_bn_compare(output, expected1, 2) == 0);
        printf("通过: 右移4位\n");

        // 右移1位
        cc_bn_rshift(input, 2, 1, output);
        cc_bn_digit_t expected2[] = {0x091A2B3C, 0x4D5E6F78};
        assert(cc_bn_compare(output, expected2, 2) == 0);
        printf("通过: 右移1位\n\n");
    }

    // 测试用例5: 单个字的移位
    {
        printf("测试用例5: 单个字的移位\n");
        cc_bn_digit_t input[] = {0x80000000};
        cc_bn_digit_t output[1];

        cc_bn_rshift(input, 1, 1, output);
        cc_bn_digit_t expected[] = {0x40000000};
        assert(cc_bn_compare(output, expected, 1) == 0);
        printf("通过: 单字右移1位\n");

        cc_bn_rshift(input, 1, 31, output);
        cc_bn_digit_t expected2[] = {0x00000001};
        assert(cc_bn_compare(output, expected2, 1) == 0);
        printf("通过: 单字右移31位\n\n");
    }

    // 测试用例6: 边界条件 - 移位31位（CC_BN_DIGIT_BITS - 1）
    {
        printf("测试用例6: 移位31位\n");
        cc_bn_digit_t input[] = {0xFFFFFFFF, 0x80000000};
        cc_bn_digit_t output[2];

        cc_bn_rshift(input, 2, 31, output);
        // 0xFFFFFFFF >> 31 = 0x1，加上0x80000000的最低位0 << 1 = 0x1
        // 0x80000000 >> 31 = 0x1
        cc_bn_digit_t expected[] = {0x00000001, 0x00000001};
        assert(cc_bn_compare(output, expected, 2) == 0);
        printf("通过: 右移31位\n\n");
    }

    // 测试用例7: 全零输入
    {
        printf("测试用例7: 全零输入\n");
        cc_bn_digit_t input[] = {0, 0, 0};
        cc_bn_digit_t output[3];
        cc_bn_digit_t expected[] = {0, 0, 0};

        cc_bn_rshift(input, 3, 15, output);
        assert(cc_bn_compare(output, expected, 3) == 0);
        printf("通过: 全零输入右移15位\n\n");
    }

    // 测试用例8: 全1输入
    {
        printf("测试用例8: 全1输入\n");
        cc_bn_digit_t input[] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_digit_t output[2];

        cc_bn_rshift(input, 2, 8, output);
        // 每个字右移8位，需要从高位借8位
        cc_bn_digit_t expected[] = {0xFFFFFFFF, 0x00FFFFFF};
        assert(cc_bn_compare(output, expected, 2) == 0);
        printf("通过: 全1输入右移8位\n\n");
    }

    // 测试用例9: 复杂的非对齐移位
    { // FEDCBA98 9ABCDEF0 12345678
        printf("测试用例9: 复杂的非对齐移位\n");
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98};
        cc_bn_digit_t output[3];

        cc_bn_rshift(input, 3, 36, output); // 32 + 4位
        // 相当于先右移32位，再右移4位
        cc_bn_digit_t expected[] = {0x89ABCDEF, 0x0FEDCBA9, 0};
        assert(cc_bn_compare(output, expected, 3) == 0);
        printf("通过: 右移36位\n\n");
    }

    printf("所有测试用例通过！cc_bn_rshift函数工作正常。\n");
}

int main()
{
    test_cc_bn_rshift();
    return 0;
}
