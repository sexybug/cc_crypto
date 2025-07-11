#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"

// 辅助函数：比较两个数组是否相等
int arrays_equal(const cc_bn_t *arr1, const cc_bn_t *arr2, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (arr1[i] != arr2[i])
            return 0;
    }
    return 1;
}

// 单元测试函数
void test_cc_bn_sub_small()
{
    cc_bn_t result[10];
    cc_bn_t borrow;
    
    printf("开始测试 cc_bn_sub_small 函数...\n");

    // 测试用例1: 基本减法，无借位
    // A = [5, 3], B = [2, 1], 期望结果: X = [3, 2], borrow = 0
    {
        cc_bn_t A[] = {5, 3};
        cc_bn_t B[] = {2, 1};
        cc_bn_t expected[] = {3, 2};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 2, B, 2, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 2));
        printf("测试用例1: 基本减法，无借位 - 通过\n");
    }

    // 测试用例2: 需要借位的减法
    // A = [2, 3], B = [5, 1], 期望结果: X = [ULONG_MAX - 2, 1], borrow = 0
    {
        cc_bn_t A[] = {2, 3};
        cc_bn_t B[] = {5, 1};
        cc_bn_t expected[] = {(cc_bn_t)(-3), 1};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 2, B, 2, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 2));
        printf("测试用例2: 需要借位的减法 - 通过\n");
    }

    // 测试用例3: A长度大于B长度
    // A = [10, 5, 3], B = [3, 2], 期望结果: X = [7, 3, 3], borrow = 0
    {
        cc_bn_t A[] = {10, 5, 3};
        cc_bn_t B[] = {3, 2};
        cc_bn_t expected[] = {7, 3, 3};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 3, B, 2, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 3));
        printf("测试用例3: A长度大于B长度 - 通过\n");
    }

    // 测试用例4: A长度大于B长度且需要借位传播
    // A = [0, 0, 1], B = [1], 期望结果: X = [ULONG_MAX, ULONG_MAX, 0], borrow = 0
    {
        cc_bn_t A[] = {0, 0, 1};
        cc_bn_t B[] = {1};
        cc_bn_t expected[] = {(cc_bn_t)(-1), (cc_bn_t)(-1), 0};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 3, B, 1, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 3));
        printf("测试用例4: A长度大于B长度且需要借位传播 - 通过\n");
    }

    // 测试用例5: 结果产生最终借位（A < B的情况）
    // A = [1, 2], B = [2, 3], 期望结果: borrow = 1
    {
        cc_bn_t A[] = {1, 2};
        cc_bn_t B[] = {2, 3};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 2, B, 2, result);
        assert(borrow == 1);
        printf("测试用例5: 结果产生最终借位 - 通过\n");
    }

    // 测试用例6: 单个元素减法，无借位
    // A = [10], B = [3], 期望结果: X = [7], borrow = 0
    {
        cc_bn_t A[] = {10};
        cc_bn_t B[] = {3};
        cc_bn_t expected[] = {7};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 1, B, 1, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 1));
        printf("测试用例6: 单个元素减法，无借位 - 通过\n");
    }

    // 测试用例7: 单个元素减法，有借位
    // A = [3], B = [10], 期望结果: X = [ULONG_MAX - 6], borrow = 1
    {
        cc_bn_t A[] = {3};
        cc_bn_t B[] = {10};
        cc_bn_t expected[] = {(cc_bn_t)(-7)};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 1, B, 1, result);
        assert(borrow == 1);
        assert(arrays_equal(result, expected, 1));
        printf("测试用例7: 单个元素减法，有借位 - 通过\n");
    }

    // 测试用例8: 边界情况 - 零减法
    // A = [0], B = [0], 期望结果: X = [0], borrow = 0
    {
        cc_bn_t A[] = {0};
        cc_bn_t B[] = {0};
        cc_bn_t expected[] = {0};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 1, B, 1, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 1));
        printf("测试用例8: 零减法 - 通过\n");
    }

    // 测试用例9: 最大值减法
    // A = [ULONG_MAX], B = [1], 期望结果: X = [ULONG_MAX - 1], borrow = 0
    {
        cc_bn_t A[] = {(cc_bn_t)(-1)};
        cc_bn_t B[] = {1};
        cc_bn_t expected[] = {(cc_bn_t)(-2)};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 1, B, 1, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 1));
        printf("测试用例9: 最大值减法 - 通过\n");
    }

    // 测试用例10: 复杂借位传播
    // A = [0, 0, 0, 1], B = [1, 0, 0], 期望结果需要多级借位传播
    {
        cc_bn_t A[] = {0, 0, 0, 1};
        cc_bn_t B[] = {1, 0, 0};
        cc_bn_t expected[] = {(cc_bn_t)(-1), (cc_bn_t)(-1), (cc_bn_t)(-1), 0};
        memset(result, 0, sizeof(result));
        
        borrow = cc_bn_sub_small(A, 4, B, 3, result);
        assert(borrow == 0);
        assert(arrays_equal(result, expected, 4));
        printf("测试用例10: 复杂借位传播 - 通过\n");
    }

    printf("所有测试用例均通过！\n");
}

int main()
{
    test_cc_bn_sub_small();
    return 0;
}
