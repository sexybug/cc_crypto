#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cc_bn_mod.h"


int compare_bn(cc_bn_t *a, cc_bn_t *b, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

// 单元测试函数
void test_cc_bn_div()
{
    printf("开始测试 cc_bn_core_div 函数\n");

    // 测试用例1: 除零错误
    printf("\n测试用例1: 除零错误\n");
    {
        cc_bn_t A[4] = {1, 0, 0, 0};
        cc_bn_t N[4] = {0, 0, 0, 0};
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_ERR_BN_DIV_BY_ZERO);
        printf("✓ 除零错误检测正确\n");
    }

    // 测试用例2: A < N的情况
    printf("\n测试用例2: A < N (被除数小于除数)\n");
    {
        cc_bn_t A[4] = {5, 0, 0, 0};  // A = 5
        cc_bn_t N[4] = {10, 0, 0, 0}; // N = 10
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};
        cc_bn_t expected_Q[4] = {0, 0, 0, 0};
        cc_bn_t expected_R[4] = {5, 0, 0, 0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        assert(compare_bn(Q, expected_Q, 4));
        assert(compare_bn(R, expected_R, 4));
        printf("✓ A < N 情况处理正确\n");
    }

    // 测试用例3: A = N的情况
    printf("\n测试用例3: A = N (被除数等于除数)\n");
    {
        cc_bn_t A[4] = {15, 0, 0, 0}; // A = 15
        cc_bn_t N[4] = {15, 0, 0, 0}; // N = 15
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};
        cc_bn_t expected_Q[4] = {1, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        assert(compare_bn(Q, expected_Q, 4));
        assert(compare_bn(R, expected_R, 4));
        printf("✓ A = N 情况处理正确\n");
    }

    // 测试用例4: 简单整除情况
    printf("\n测试用例4: 简单整除 (无余数)\n");
    {
        cc_bn_t A[4] = {20, 0, 0, 0}; // A = 20
        cc_bn_t N[4] = {4, 0, 0, 0};  // N = 4
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};
        cc_bn_t expected_Q[4] = {5, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        assert(compare_bn(Q, expected_Q, 4));
        assert(compare_bn(R, expected_R, 4));
        printf("✓ 简单整除处理正确\n");
    }

    // 测试用例5: 有余数的除法
    printf("\n测试用例5: 有余数的除法\n");
    {
        cc_bn_t A[4] = {23, 0, 0, 0}; // A = 23
        cc_bn_t N[4] = {7, 0, 0, 0};  // N = 7
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};
        cc_bn_t expected_Q[4] = {3, 0, 0, 0};
        cc_bn_t expected_R[4] = {2, 0, 0, 0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        assert(compare_bn(Q, expected_Q, 4));
        assert(compare_bn(R, expected_R, 4));
        printf("✓ 有余数除法处理正确\n");
    }

    // 测试用例6: 大数除法
    printf("\n测试用例6: 大数除法\n");
    {
        cc_bn_t A[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0, 0}; // 大数
        cc_bn_t N[4] = {0x10000, 0, 0, 0};             // 较小除数
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        printf("✓ 大数除法执行成功\n");
    }

    // 测试用例7: 除数为1
    printf("\n测试用例7: 除数为1\n");
    {
        cc_bn_t A[4] = {12345, 0, 0, 0}; // A = 12345
        cc_bn_t N[4] = {1, 0, 0, 0};     // N = 1
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};
        cc_bn_t expected_Q[4] = {12345, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        assert(compare_bn(Q, expected_Q, 4));
        assert(compare_bn(R, expected_R, 4));
        printf("✓ 除数为1处理正确\n");
    }

    // 测试用例8: 被除数为0
    printf("\n测试用例8: 被除数为0\n");
    {
        cc_bn_t A[4] = {0, 0, 0, 0}; // A = 0
        cc_bn_t N[4] = {5, 0, 0, 0}; // N = 5
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};
        cc_bn_t expected_Q[4] = {0, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        assert(compare_bn(Q, expected_Q, 4));
        assert(compare_bn(R, expected_R, 4));
        printf("✓ 被除数为0处理正确\n");
    }

    // 测试用例9: 多字长度测试
    printf("\n测试用例9: 不同字长度组合\n");
    {
        cc_bn_t A[8] = {100, 0, 0, 0, 0, 0, 0, 0};
        cc_bn_t N[4] = {10, 0, 0, 0};
        cc_bn_t Q[8] = {0};
        cc_bn_t R[4] = {0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 8, N, 4);
        assert(result == CC_OK);
        printf("✓ 不同字长度处理正确\n");
    }

    // 测试用例10: 边界值测试
    printf("\n测试用例10: 最大值除法\n");
    {
        cc_bn_t A[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[4] = {0xFFFFFFFF, 0, 0, 0};
        cc_bn_t Q[4] = {0};
        cc_bn_t R[4] = {0};

        cc_status_t result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_OK);
        printf("✓ 最大值除法执行成功\n");
    }

    printf("\n所有测试用例通过！✓\n");
}

int main()
{
    test_cc_bn_div();
    return 0;
}
