#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"

// 辅助函数：比较两个数组
int arrays_equal(const cc_bn_t *a, const cc_bn_t *b, size_t len)
{
    return memcmp(a, b, len * sizeof(cc_bn_t)) == 0;
}

// 测试用例1：基本加法，无进位
void test_basic_addition_no_carry()
{
    printf("测试1: 基本加法，无进位\n");
    cc_bn_t A[] = {0x10000000, 0x20000000, 0x30000000};
    cc_bn_t B[] = {0x01000000, 0x02000000};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x11000000, 0x22000000, 0x30000000};

    cc_bn_t carry = cc_bn_add(X, A, 3, B, 2);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 测试用例2：基本加法，有进位
void test_basic_addition_with_carry()
{
    printf("测试2: 基本加法，有进位\n");
    cc_bn_t A[] = {0xFFFFFFFF, 0x10000000, 0x20000000};
    cc_bn_t B[] = {0x00000001, 0x00000000};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x00000000, 0x10000001, 0x20000000};

    cc_bn_t carry = cc_bn_add(X, A, 3, B, 2);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 测试用例3：A和B长度相同
void test_equal_length()
{
    printf("测试3: A和B长度相同\n");
    cc_bn_t A[] = {0x12345678, 0x87654321, 0xABCDEF01};
    cc_bn_t B[] = {0x11111111, 0x22222222, 0x33333333};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x23456789, 0xA9876543, 0xDF012234};

    cc_bn_t carry = cc_bn_add(X, A, 3, B, 3);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 测试用例4：B长度为0（空数组）
void test_empty_B()
{
    printf("测试4: B长度为0\n");
    cc_bn_t A[] = {0x12345678, 0x87654321, 0xABCDEF01};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x12345678, 0x87654321, 0xABCDEF01};

    cc_bn_t carry = cc_bn_add(X, A, 3, NULL, 0);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 测试用例5：单位数加法
void test_single_digit()
{
    printf("测试5: 单位数加法\n");
    cc_bn_t A[] = {0x12345678};
    cc_bn_t B[] = {0x11111111};
    cc_bn_t X[1];
    cc_bn_t expected[] = {0x23456789};

    cc_bn_t carry = cc_bn_add(X, A, 1, B, 1);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 1));
    printf("✓ 通过\n\n");
}

// 测试用例6：单位数加法有进位
void test_single_digit_with_carry()
{
    printf("测试6: 单位数加法有进位\n");
    cc_bn_t A[] = {0xFFFFFFFF};
    cc_bn_t B[] = {0x00000001};
    cc_bn_t X[1];
    cc_bn_t expected[] = {0x00000000};

    cc_bn_t carry = cc_bn_add(X, A, 1, B, 1);

    assert(carry == 1);
    assert(arrays_equal(X, expected, 1));
    printf("✓ 通过\n\n");
}

// 测试用例7：最大值相加
void test_maximum_values()
{
    printf("测试7: 最大值相加\n");
    cc_bn_t A[] = {0xFFFFFFFF, 0xFFFFFFFF};
    cc_bn_t B[] = {0xFFFFFFFF, 0xFFFFFFFF};
    cc_bn_t X[2];
    cc_bn_t expected[] = {0xFFFFFFFE, 0xFFFFFFFF};

    cc_bn_t carry = cc_bn_add(X, A, 2, B, 2);

    assert(carry == 1);
    assert(arrays_equal(X, expected, 2));
    printf("✓ 通过\n\n");
}

// 测试用例8：零值相加
void test_zero_addition()
{
    printf("测试8: 零值相加\n");
    cc_bn_t A[] = {0x00000000, 0x00000000, 0x00000000};
    cc_bn_t B[] = {0x00000000, 0x00000000};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x00000000, 0x00000000, 0x00000000};

    cc_bn_t carry = cc_bn_add(X, A, 3, B, 2);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 测试用例9：连续进位
void test_cascading_carry()
{
    printf("测试9: 连续进位\n");
    cc_bn_t A[] = {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};
    cc_bn_t B[] = {0x00000001, 0x00000000};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x00000000, 0x00000000, 0x00000001};

    cc_bn_t carry = cc_bn_add(X, A, 3, B, 2);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 测试用例10：边界条件测试
void test_boundary_conditions()
{
    printf("测试10: 边界条件测试\n");
    cc_bn_t A[] = {0xFFFFF000};
    cc_bn_t B[] = {0x00001000};
    cc_bn_t X[1];
    cc_bn_t expected[] = {0x00000000};

    cc_bn_t carry = cc_bn_add(X, A, 1, B, 1);

    assert(carry == 1);
    assert(arrays_equal(X, expected, 1));
    printf("✓ 通过\n\n");
}

// 测试用例11：大数组测试
void test_large_arrays()
{
    printf("测试11: 大数组测试\n");
    cc_bn_t A[10] = {0x11111111, 0x11111111, 0x11111111, 0x11111111, 0x11111111,
                     0x11111111, 0x11111111, 0x11111111, 0x11111111, 0x11111111};
    cc_bn_t B[5] = {0x11111111, 0x11111111, 0x11111111, 0x11111111, 0x11111111};
    cc_bn_t X[10];
    cc_bn_t expected[10] = {0x22222222, 0x22222222, 0x22222222, 0x22222222, 0x22222222,
                            0x11111111, 0x11111111, 0x11111111, 0x11111111, 0x11111111};

    cc_bn_t carry = cc_bn_add(X, A, 10, B, 5);

    assert(carry == 0);
    assert(arrays_equal(X, expected, 10));
    printf("✓ 通过\n\n");
}

// 测试用例12：最终进位测试
void test_final_carry()
{
    printf("测试12: 最终进位测试\n");
    cc_bn_t A[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    cc_bn_t B[] = {0x00000001, 0x00000000, 0x00000000};
    cc_bn_t X[3];
    cc_bn_t expected[] = {0x00000000, 0x00000000, 0x00000000};

    cc_bn_t carry = cc_bn_add(X, A, 3, B, 3);

    assert(carry == 1);
    assert(arrays_equal(X, expected, 3));
    printf("✓ 通过\n\n");
}

// 主测试函数
int main()
{
    printf("开始32位大数加法函数单元测试\n");
    printf("================================\n\n");

    test_basic_addition_no_carry();
    test_basic_addition_with_carry();
    test_equal_length();
    test_empty_B();
    test_single_digit();
    test_single_digit_with_carry();
    test_maximum_values();
    test_zero_addition();
    test_cascading_carry();
    test_boundary_conditions();
    test_large_arrays();
    test_final_carry();

    printf("================================\n");
    printf("所有测试用例通过！✓\n");

    return 0;
}
