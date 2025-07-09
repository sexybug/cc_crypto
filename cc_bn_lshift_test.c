#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "cc_bn.h"

// 辅助函数：比较两个大数是否相等
int cc_bn_equal(const cc_bn_digit_t *a, const cc_bn_digit_t *b, size_t len)
{
    return memcmp(a, b, len * sizeof(cc_bn_digit_t)) == 0;
}

void test_cc_bn_left_shift()
{
    printf("开始测试 cc_bn_lshift 函数...\n");

    // 测试用例1: 基本左移操作（无跨字边界）
    {
        cc_bn_digit_t input[] = {0x12345678};
        cc_bn_digit_t output[1];
        cc_bn_digit_t expected[] = {0x23456780}; // 左移4位

        cc_bn_lshift(input, 1, 4, output);
        assert(cc_bn_equal(output, expected, 1));
        printf("测试用例1通过: 基本左移4位\n");
    }

    // 测试用例2: 零移位
    { // 9ABCDEF012345678
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[] = {0x12345678, 0x9ABCDEF0};

        cc_bn_lshift(input, 2, 0, output);
        assert(cc_bn_equal(output, expected, 2));
        printf("测试用例2通过: 零移位\n");
    }

    // 测试用例3: 跨字边界左移
    { // 9ABCDEF012345678
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[] = {0x45678000, 0xCDEF0123}; // 左移12位

        cc_bn_lshift(input, 2, 12, output);
        assert(cc_bn_equal(output, expected, 2));
        printf("测试用例3通过: 跨字边界左移12位\n");
    }

    // 测试用例4: 整字移位
    {
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[] = {0x00000000, 0x12345678}; // 左移32位

        cc_bn_lshift(input, 2, 32, output);
        assert(cc_bn_equal(output, expected, 2));
        printf("测试用例4通过: 整字移位32位\n");
    }

    // 测试用例5: 移位长度超过输入长度
    {
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[] = {0x00000000, 0x00000000};

        cc_bn_lshift(input, 2, 64, output);
        assert(cc_bn_equal(output, expected, 2));
        printf("测试用例5通过: 移位长度超过输入长度\n");
    }

    // 测试用例6: 全零输入
    {
        cc_bn_digit_t input[] = {0x00000000, 0x00000000};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[] = {0x00000000, 0x00000000};

        cc_bn_lshift(input, 2, 15, output);
        assert(cc_bn_equal(output, expected, 2));
        printf("测试用例6通过: 全零输入\n");
    }

    // 测试用例7: 最大值移位1位
    {
        cc_bn_digit_t input[] = {0xFFFFFFFF};
        cc_bn_digit_t output[1];
        cc_bn_digit_t expected[] = {0xFFFFFFFE}; // 左移1位

        cc_bn_lshift(input, 1, 1, output);
        assert(cc_bn_equal(output, expected, 1));
        printf("测试用例7通过: 最大值左移1位\n");
    }

    // 测试用例8: 多字数组，整字加部分位移位
    { // 111111119ABCDEF012345678
        cc_bn_digit_t input[] = {0x12345678, 0x9ABCDEF0, 0x11111111};
        cc_bn_digit_t output[3];
        cc_bn_digit_t expected[] = {0x00000000, 0x45678000, 0xCDEF0123}; // 左移44位(32+12)

        cc_bn_lshift(input, 3, 44, output);
        assert(cc_bn_equal(output, expected, 3));
        printf("测试用例8通过: 多字数组整字加部分位移位\n");
    }

    // 测试用例9: 边界情况 - 移位31位
    {
        cc_bn_digit_t input[] = {0x00000001, 0x00000000};
        cc_bn_digit_t output[2];
        cc_bn_digit_t expected[] = {0x80000000, 0x00000000}; // 左移31位

        cc_bn_lshift(input, 2, 31, output);
        assert(cc_bn_equal(output, expected, 2));
        printf("测试用例9通过: 边界情况左移31位\n");
    }

    // 测试用例10: 单字长度，移位等于字位数
    {
        cc_bn_digit_t input[] = {0x12345678};
        cc_bn_digit_t output[1];
        cc_bn_digit_t expected[] = {0x00000000}; // 左移32位

        cc_bn_lshift(input, 1, 32, output);
        assert(cc_bn_equal(output, expected, 1));
        printf("测试用例10通过: 单字移位等于字位数\n");
    }

    // 测试用例11: 大数组测试
    { // 4444444433333333 2222222211111111
        cc_bn_digit_t input[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
        cc_bn_digit_t output[4];
        cc_bn_digit_t expected[] = {0x88888888, 0x11111110, 0x99999999, 0x22222221}; // 左移3位

        cc_bn_lshift(input, 4, 3, output);
        assert(cc_bn_equal(output, expected, 4));
        printf("测试用例11通过: 大数组左移3位\n");
    }

    // 测试用例12: 验证原地操作（输入输出同一数组）
    {
        cc_bn_digit_t data[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_digit_t expected[] = {0x48D159E0, 0x6AF37BC0}; // 左移2位

        cc_bn_lshift(data, 2, 2, data);
        assert(cc_bn_equal(data, expected, 2));
        printf("测试用例12通过: 原地操作\n");
    }

    printf("所有测试用例通过！\n");
}

int main()
{
    test_cc_bn_left_shift();
    return 0;
}
