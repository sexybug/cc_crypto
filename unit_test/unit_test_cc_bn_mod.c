#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn_mod.h"
#include "cc_bn_config.h"

// 辅助函数：比较两个大数是否相等
int bn_equals(const cc_bn_t *a, const cc_bn_t *b, size_t word_len)
{
    for (size_t i = 0; i < word_len; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

void test_cc_bn_div()
{
    printf("开始测试 cc_bn_core_div 函数...\n");

    // 测试用例1：除数为零
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {0, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        // A = 10, N = 0
        cc_bn_set_zero(A, 4);
        A[0] = 10;
        cc_bn_set_zero(N, 4);

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_ERR_DIV_BY_ZERO);
        printf("测试1通过：除数为零错误处理\n");
    }

    // 测试用例2：被除数长度小于除数长度
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[2] = {1, 0};
        cc_bn_t expected_R[4] = {2, 0, 0, 0};

        cc_bn_set_zero(A, 2);
        cc_bn_set_zero(N, 4);
        A[0] = 5;
        N[0] = 3;

        int result = cc_bn_core_div(Q, R, A, 2, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试2通过：被除数长度小于除数长度\n");
    }

    // 测试用例3：被除数小于除数 (A < N)
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {0, 0, 0, 0};
        cc_bn_t expected_R[4] = {5, 0, 0, 0};

        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 5;  // A = 5
        N[0] = 10; // N = 10

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试3通过：被除数小于除数\n");
    }

    // 测试用例4：简单整除 (12 ÷ 3 = 4)
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {4, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 12; // A = 12
        N[0] = 3;  // N = 3

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试4通过：简单整除\n");
    }

    // 测试用例5：带余数除法 (13 ÷ 3 = 4 余 1)
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {4, 0, 0, 0};
        cc_bn_t expected_R[4] = {1, 0, 0, 0};

        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 13; // A = 13
        N[0] = 3;  // N = 3

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试5通过：带余数除法\n");
    }

    // 测试用例6：除数为1
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {42, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 42; // A = 42
        N[0] = 1;  // N = 1

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试6通过：除数为1\n");
    }

    // 测试用例7：相等的数相除 (A = N)
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {1, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 15; // A = 15
        N[0] = 15; // N = 15

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试7通过：相等数相除\n");
    }

    // 测试用例8：大数除法测试
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {0x0001ffff, 0x00000000, 0x00000000, 0x00000000};
        cc_bn_t expected_R[4] = {0x0000ffff, 0x00000000, 0x00000000, 0x00000000};
        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 0xFFFFFFFF; // A = 最大32位数
        A[1] = 0x1;        // A = 2^32 + 2^32 - 1
        N[0] = 0x10000;    // N = 65536

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试8通过：大数除法\n");
    }

    // 测试用例9：多字长度测试
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[8] = {0x0004d5e6, 0x11100000, 0x00011111,
                                 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
        cc_bn_t expected_R[4] = {0xc4d5f678, 0x00001eef, 0x00000000, 0x00000000};
        cc_bn_set_zero(A, 8);
        cc_bn_set_zero(N, 4);

        // 构造多字大数
        A[0] = 0x12345678;
        A[1] = 0x9ABCDEF0;
        A[2] = 0x11111111;
        A[3] = 0x22222222;

        N[0] = 0x1000;
        N[1] = 0x2000;

        int result = cc_bn_core_div(Q, R, A, 8, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试9通过：多字长度除法\n");
    }

    // 测试用例10：边界条件 - 最小非零除数
    {
        cc_bn_t A[CC_BN_MAX_WORDS], N[CC_BN_MAX_WORDS];
        cc_bn_t Q[CC_BN_MAX_WORDS], R[CC_BN_MAX_WORDS];
        cc_bn_t expected_Q[4] = {100, 0, 0, 0};
        cc_bn_t expected_R[4] = {0, 0, 0, 0};

        cc_bn_set_zero(A, 4);
        cc_bn_set_zero(N, 4);
        A[0] = 100;
        N[0] = 1;

        int result = cc_bn_core_div(Q, R, A, 4, N, 4);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试10通过：最小非零除数\n");
    }

    // 测试用例11：大数除法测试
    {
        cc_bn_t A[2] = {0x00000001, 0xFFFFFFFF};
        cc_bn_t N[2] = {0xFFFFFFFF};
        cc_bn_t Q[2], R[1];
        cc_bn_t expected_Q[2] = {0x00000000, 1};
        cc_bn_t expected_R[1] = {0x00000001};

        int result = cc_bn_core_div(Q, R, A, 2, N, 1);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试11通过：大数除法\n");
    }

    // 测试用例12：大数除法测试
    {
        cc_bn_t A[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[2] = {0x00000001, 0xFFFFFFFF};
        cc_bn_t Q[2], R[2];
        cc_bn_t expected_Q[2] = {0x00000001, 0};
        cc_bn_t expected_R[2] = {0xfffffffe, 0};

        int result = cc_bn_core_div(Q, R, A, 2, N, 2);
        assert(result == CC_BN_SUCCESS);
        assert(bn_equals(Q, expected_Q, sizeof(expected_Q) / sizeof(expected_Q[0])));
        assert(bn_equals(R, expected_R, sizeof(expected_R) / sizeof(expected_R[0])));
        printf("测试12通过：大数除法\n");
    }

    printf("所有测试用例通过！\n");
}

int main()
{
    test_cc_bn_div();
    return 0;
}
