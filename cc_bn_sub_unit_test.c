#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"
#include "test.h"

// 辅助函数：比较两个大数是否相等
int bn_equal(const cc_bn_digit_t *a, const cc_bn_digit_t *b, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

void test_cc_bn_sub()
{
    cc_bn_digit_t result[10];
    cc_bn_digit_t borrow;

    // 测试1: 基本减法，无借位
    {
        cc_bn_digit_t A[] = {10, 20, 30};
        cc_bn_digit_t B[] = {5, 15, 25};
        cc_bn_digit_t expected[] = {5, 5, 5};

        borrow = cc_bn_sub(A, B, 3, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 3));
        printf("测试1通过: 基本减法，无借位\n");
    }

    // 测试2: 需要借位的减法
    {
        cc_bn_digit_t A[] = {5, 10, 15};
        cc_bn_digit_t B[] = {10, 5, 20};
        cc_bn_digit_t expected[] = {0xFFFFFFFBULL, 4, 0xFFFFFFFBULL};

        borrow = cc_bn_sub(A, B, 3, result);
        assert(borrow == 1);
        assert(bn_equal(result, expected, 3));
        printf("测试2通过: 需要借位的减法\n");
    }

    // 测试3: 相等数相减
    {
        cc_bn_digit_t A[] = {100, 200, 300};
        cc_bn_digit_t B[] = {100, 200, 300};
        cc_bn_digit_t expected[] = {0, 0, 0};

        borrow = cc_bn_sub(A, B, 3, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 3));
        printf("测试3通过: 相等数相减\n");
    }

    // 测试4: 全零减法
    {
        cc_bn_digit_t A[] = {0, 0, 0};
        cc_bn_digit_t B[] = {0, 0, 0};
        cc_bn_digit_t expected[] = {0, 0, 0};

        borrow = cc_bn_sub(A, B, 3, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 3));
        printf("测试4通过: 全零减法\n");
    }

    // 测试5: 最大值减1
    {
        cc_bn_digit_t A[] = {0xFFFFFFFFULL, 0xFFFFFFFFULL};
        cc_bn_digit_t B[] = {1, 0};
        cc_bn_digit_t expected[] = {0xFFFFFFFEULL, 0xFFFFFFFFULL};

        borrow = cc_bn_sub(A, B, 2, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 2));
        printf("测试5通过: 最大值减1\n");
    }

    // 测试6: 0减1（产生借位）
    {
        cc_bn_digit_t A[] = {0, 0};
        cc_bn_digit_t B[] = {1, 0};
        cc_bn_digit_t expected[] = {0xFFFFFFFFULL, 0xFFFFFFFFULL};

        borrow = cc_bn_sub(A, B, 2, result);
        assert(borrow == 1);
        assert(bn_equal(result, expected, 2));
        printf("测试6通过: 0减1（产生借位）\n");
    }

    // 测试7: 单字长度测试
    {
        cc_bn_digit_t A[] = {100};
        cc_bn_digit_t B[] = {50};
        cc_bn_digit_t expected[] = {50};

        borrow = cc_bn_sub(A, B, 1, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 1));
        printf("测试7通过: 单字长度测试\n");
    }

    // 测试8: 连续借位
    {
        cc_bn_digit_t A[] = {0, 0, 1};
        cc_bn_digit_t B[] = {1, 0, 0};
        cc_bn_digit_t expected[] = {0xFFFFFFFFULL, 0xFFFFFFFFULL, 0};

        borrow = cc_bn_sub(A, B, 3, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 3));
        printf("测试8通过: 连续借位\n");
    }

    // 测试9: 边界值测试 - 最大数减最大数
    {
        cc_bn_digit_t A[] = {0xFFFFFFFFULL, 0xFFFFFFFFULL};
        cc_bn_digit_t B[] = {0xFFFFFFFFULL, 0xFFFFFFFFULL};
        cc_bn_digit_t expected[] = {0, 0};

        borrow = cc_bn_sub(A, B, 2, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 2));
        printf("测试9通过: 最大数减最大数\n");
    }

    // 测试10: 复杂借位场景
    {
        cc_bn_digit_t A[] = {0x80000000ULL, 0x40000000ULL};
        cc_bn_digit_t B[] = {0x80000001ULL, 0x20000000ULL};
        cc_bn_digit_t expected[] = {0xFFFFFFFFULL, 0x1FFFFFFFULL};

        borrow = cc_bn_sub(A, B, 2, result);
        assert(borrow == 0);
        assert(bn_equal(result, expected, 2));
        printf("测试10通过: 复杂借位场景\n");
    }

    printf("所有测试用例通过！\n");
}

int main()
{
    test_cc_bn_sub();
    return 0;
}
