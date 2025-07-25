#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "cc_bn.h"
#include "cc_bn_mod.h"

// 测试辅助函数：比较大数是否相等
int compare_bn(const cc_bn_t *a, const cc_bn_t *b, size_t len)
{
    return memcmp(a, b, len * sizeof(cc_bn_t)) == 0;
}

void test_cc_bn_mod_add_word()
{
    printf("开始测试 cc_bn_mod_add_word 函数...\n");

    // 测试用例1：基本加法，无溢出，结果小于模数
    {
        cc_bn_t A[4] = {10, 0, 0, 0};
        cc_bn_t d = 5;
        cc_bn_t N[4] = {20, 0, 0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {15, 0, 0, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例1通过：基本加法\n");
    }

    // 测试用例2：加法结果等于模数，需要减去模数
    {
        cc_bn_t A[4] = {15, 0, 0, 0};
        cc_bn_t d = 5;
        cc_bn_t N[4] = {20, 0, 0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {0, 0, 0, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例2通过：结果等于模数\n");
    }

    // 测试用例3：加法结果大于模数，需要减去模数
    {
        cc_bn_t A[4] = {18, 0, 0, 0};
        cc_bn_t d = 7;
        cc_bn_t N[4] = {20, 0, 0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {5, 0, 0, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例3通过：结果大于模数\n");
    }

    // 测试用例4：产生进位的情况
    {
        cc_bn_t A[4] = {0xFFFFFFFF, 0, 0, 0};
        cc_bn_t d = 1;
        cc_bn_t N[4] = {0x80000000, 1, 0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {0x00000000, 0x00000001, 0, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例4通过：产生进位\n");
    }

    // 测试用例5：多字长度测试
    {
        cc_bn_t A[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0, 0};
        cc_bn_t d = 1;
        cc_bn_t N[4] = {0x12345678, 0x87654321, 1, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {0x00000000, 0x00000000, 0x00000001, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例5通过：多字长度\n");
    }

    // 测试用例6：d为0的情况
    {
        cc_bn_t A[4] = {15, 0, 0, 0};
        cc_bn_t d = 0;
        cc_bn_t N[4] = {20, 0, 0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {15, 0, 0, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例6通过：d为0\n");
    }

    // 测试用例7：A为0的情况
    {
        cc_bn_t A[4] = {0, 0, 0, 0};
        cc_bn_t d = 15;
        cc_bn_t N[4] = {20, 0, 0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {15, 0, 0, 0};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例7通过：A为0\n");
    }

    // 测试用例8：最大值边界测试
    {
        cc_bn_t A[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t d = 0xFFFFFFFF;
        cc_bn_t N[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[4];
        cc_bn_t expected[4] = {0xffffffff, 0x00000000, 0x00000000, 0x00000000};

        cc_bn_mod_add_word(R, A, d, N, 4);
        assert(compare_bn(R, expected, 4));
        printf("测试用例8通过：最大值边界\n");
    }

    // 测试用例9：单字长度测试
    {
        cc_bn_t A[1] = {10};
        cc_bn_t d = 5;
        cc_bn_t N[1] = {12};
        cc_bn_t R[1];
        cc_bn_t expected[1] = {3};

        cc_bn_mod_add_word(R, A, d, N, 1);
        assert(compare_bn(R, expected, 1));
        printf("测试用例9通过：单字长度\n");
    }

    // 测试用例10：进位导致需要模约简的情况
    {
        cc_bn_t A[2] = {0xFFFFFFFE, 0};
        cc_bn_t d = 0xFF;
        cc_bn_t N[2] = {5, 1};
        cc_bn_t R[2];
        cc_bn_t expected[2] = {0x000000f8, 0};

        cc_bn_mod_add_word(R, A, d, N, 2);
        assert(compare_bn(R, expected, 2));
        printf("测试用例10通过：进位后模约简\n");
    }

    printf("所有测试用例通过！\n");
}

int main()
{
    test_cc_bn_mod_add_word();
    return 0;
}
