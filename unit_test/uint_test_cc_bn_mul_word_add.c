#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "cc_bn.h"

// 辅助函数：比较两个大数数组
int compare_arrays(const cc_bn_t *a, const cc_bn_t *b, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

// 测试用例1：基本功能测试
void test_basic_functionality()
{
    cc_bn_t A[] = {1, 2, 3};
    cc_bn_t R[4] = {0, 0, 0, 0};
    cc_bn_t expected[4] = {2, 4, 6, 0};

    cc_bn_mul_word_add(A, 3, 2, R);
    assert(compare_arrays(R, expected, 4));
}

// 测试用例2：零乘法测试
void test_multiply_by_zero()
{
    cc_bn_t A[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    cc_bn_t R[4] = {1, 2, 3, 0};
    cc_bn_t expected[] = {1, 2, 3, 0};

    cc_bn_mul_word_add(A, 3, 0, R);
    assert(compare_arrays(R, expected, 4));
}

// 测试用例3：乘以1测试
void test_multiply_by_one()
{
    cc_bn_t A[] = {0x12345678, 0x9ABCDEF0, 0x11111111};
    cc_bn_t R[4] = {0x11111111, 0x22222222, 0x33333333, 0};
    cc_bn_t expected[] = {0x23456789, 0xBCDF0112, 0x44444444, 0};

    cc_bn_mul_word_add(A, 3, 1, R);
    assert(compare_arrays(R, expected, 4));
}

// 测试用例4：进位测试
void test_carry_propagation()
{
    cc_bn_t A[] = {0xFFFFFFFF, 0xFFFFFFFF};
    cc_bn_t R[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
    cc_bn_t expected[] = {0x00000000, 0xFFFFFFFF, 0xFFFFFFFF};

    cc_bn_mul_word_add(A, 2, 0xFFFFFFFF, R);
    assert(compare_arrays(R, expected, 3));
}

// 测试用例5：最大值测试
void test_maximum_values()
{
    cc_bn_t A[] = {0xFFFFFFFF};
    cc_bn_t R[2] = {0xFFFFFFFF, 0};
    cc_bn_t expected[] = {0, 0xFFFFFFFF};

    cc_bn_mul_word_add(A, 1, 0xFFFFFFFF, R);
    assert(compare_arrays(R, expected, 2));
}

// 测试用例6：零数组测试
void test_zero_array()
{
    cc_bn_t A[] = {0, 0, 0};
    cc_bn_t R[] = {5, 10, 15, 0};
    cc_bn_t expected[] = {5, 10, 15, 0};

    cc_bn_mul_word_add(A, 3, 100, R);
    assert(compare_arrays(R, expected, 4));
}

// 测试用例7：单个元素测试
void test_single_element()
{
    cc_bn_t A[] = {0x12345678};
    cc_bn_t R[2] = {0x11111111, 0};
    cc_bn_t expected[2] = {0x47ae1479, 0};

    cc_bn_mul_word_add(A, 1, 3, R);
    assert(compare_arrays(R, expected, 2));
}

// 测试用例8：长度为0的测试
void test_zero_length()
{
    cc_bn_t A[] = {1, 2, 3};
    cc_bn_t R[4] = {1, 2, 3, 0};
    cc_bn_t expected[4] = {1, 2, 3, 0};

    cc_bn_mul_word_add(A, 0, 5, R);
    assert(compare_arrays(R, expected, 3));
}

// 测试用例9：大进位链测试
void test_large_carry_chain()
{
    cc_bn_t A[] = {0x80000000, 0x80000000, 0x80000000};
    cc_bn_t R[] = {0x80000000, 0x80000000, 0x80000000};
    cc_bn_t expected[] = {0x80000000, 0x80000001, 0x80000001};

    cc_bn_mul_word_add(A, 3, 2, R);
    assert(compare_arrays(R, expected, 3));
}

// 测试用例10：边界值测试
void test_boundary_values()
{
    cc_bn_t A[] = {1, 0xFFFFFFFE, 1};
    cc_bn_t R[4] = {0xFFFFFFFF, 1, 0xFFFFFFFF, 0};
    cc_bn_t expected[4] = {0xfffffffe, 0x00000004, 0xfffffffb, 2};

    cc_bn_mul_word_add(A, 3, 0xFFFFFFFF, R);
    assert(compare_arrays(R, expected, 3));
}

// 主测试函数
int main()
{
    test_basic_functionality();
    test_multiply_by_zero();
    test_multiply_by_one();
    test_carry_propagation();
    test_maximum_values();
    test_zero_array();
    test_single_element();
    test_zero_length();
    test_large_carry_chain();
    test_boundary_values();

    printf("所有测试用例通过！\n");
    return 0;
}
