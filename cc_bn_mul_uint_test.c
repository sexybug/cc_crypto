
#include "cc_bn.h"
#include "cc_bn_mul.h"
#include "test.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

// 辅助函数：比较两个大整数数组
int compare_bn(const cc_bn_digit_t *a, const cc_bn_digit_t *b, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (a[i] != b[i])
        {
            return 0; // 不相等
        }
    }
    return 1; // 相等
}

void test_cc_bn_mul_uint()
{
    printf("开始测试 cc_bn_mul_uint 函数...\n");

    // 测试用例1：基本乘法，无进位
    {
        cc_bn_digit_t bn_in[] = {0x12345678};
        cc_bn_digit_t bn_out[2] = {0};
        cc_bn_digit_t d = 2;
        cc_bn_digit_t expected[] = {0x2468acf0, 0};

        cc_bn_mul_uint(bn_in, 1, d, bn_out);
        assert(compare_bn(bn_out, expected, 2));
        printf("测试用例1通过：基本乘法无进位\n");
    }

    // 测试用例2：乘法产生进位
    {
        cc_bn_digit_t bn_in[] = {0x80000000};
        cc_bn_digit_t bn_out[2] = {0};
        cc_bn_digit_t d = 3;
        cc_bn_digit_t expected[] = {0x80000000, 1};

        cc_bn_mul_uint(bn_in, 1, d, bn_out);
        assert(compare_bn(bn_out, expected, 2));
        printf("测试用例2通过：乘法产生进位\n");
    }

    // 测试用例3：乘以0
    {
        cc_bn_digit_t bn_in[] = {0x12345678, 0xabcdef01};
        cc_bn_digit_t bn_out[3] = {0};
        cc_bn_digit_t d = 0;
        cc_bn_digit_t expected[] = {0, 0, 0};

        cc_bn_mul_uint(bn_in, 2, d, bn_out);
        assert(compare_bn(bn_out, expected, 3));
        printf("测试用例3通过：乘以0\n");
    }

    // 测试用例4：乘以1
    {
        cc_bn_digit_t bn_in[] = {0x12345678, 0xabcdef01};
        cc_bn_digit_t bn_out[3] = {0};
        cc_bn_digit_t d = 1;
        cc_bn_digit_t expected[] = {0x12345678, 0xabcdef01, 0};

        cc_bn_mul_uint(bn_in, 2, d, bn_out);
        assert(compare_bn(bn_out, expected, 3));
        printf("测试用例4通过：乘以1\n");
    }

    // 测试用例5：乘以最大值
    {
        cc_bn_digit_t bn_in[] = {CC_BN_DIGIT_MAX};
        cc_bn_digit_t bn_out[2] = {0};
        cc_bn_digit_t d = CC_BN_DIGIT_MAX;
        cc_bn_digit_t expected[] = {1, CC_BN_DIGIT_MAX - 1};

        cc_bn_mul_uint(bn_in, 1, d, bn_out);
        assert(compare_bn(bn_out, expected, 2));
        printf("测试用例5通过：乘以最大值\n");
    }

    // 测试用例6：多位数乘法，连续进位
    {
        cc_bn_digit_t bn_in[] = {CC_BN_DIGIT_MAX, CC_BN_DIGIT_MAX, CC_BN_DIGIT_MAX};
        cc_bn_digit_t bn_out[4] = {0};
        cc_bn_digit_t d = 2;
        cc_bn_digit_t expected[] = {CC_BN_DIGIT_MAX - 1, CC_BN_DIGIT_MAX, CC_BN_DIGIT_MAX, 1};

        cc_bn_mul_uint(bn_in, 3, d, bn_out);
        assert(compare_bn(bn_out, expected, 4));
        printf("测试用例6通过：多位数乘法连续进位\n");
    }

    // 测试用例7：空数组（长度为0）
    {
        cc_bn_digit_t bn_in[1];
        cc_bn_digit_t bn_out[1] = {0xFF}; // 初始化为非零值
        cc_bn_digit_t d = 5;

        cc_bn_mul_uint(bn_in, 0, d, bn_out);
        assert(bn_out[0] == 0); // 进位应该为0
        printf("测试用例7通过：空数组处理\n");
    }

    // 测试用例8：单个数字的边界值
    {
        cc_bn_digit_t bn_in[] = {1};
        cc_bn_digit_t bn_out[2] = {0};
        cc_bn_digit_t d = CC_BN_DIGIT_MAX;
        cc_bn_digit_t expected[] = {CC_BN_DIGIT_MAX, 0};

        cc_bn_mul_uint(bn_in, 1, d, bn_out);
        assert(compare_bn(bn_out, expected, 2));
        printf("测试用例8通过：单个数字边界值\n");
    }

    // 测试用例9：大数组测试
    {
        const size_t len = 10;
        cc_bn_digit_t bn_in[10];
        cc_bn_digit_t bn_out[10 + 1] = {0};
        cc_bn_digit_t d = 0x12345678;

        // 初始化输入数组
        for (size_t i = 0; i < len; i++)
        {
            bn_in[i] = i + 1;
        }

        cc_bn_mul_uint(bn_in, len, d, bn_out);

        // 验证：手动计算第一个元素
        uint64_t expected_first = (uint64_t)1 * d;
        assert(bn_out[0] == (expected_first & CC_BN_DIGIT_MAX));
        printf("测试用例9通过：大数组测试\n");
    }

    // 测试用例10：溢出边界测试
    {
        cc_bn_digit_t bn_in[] = {0x80000000, 0x80000000};
        cc_bn_digit_t bn_out[3] = {0};
        cc_bn_digit_t d = 0x80000000;

        cc_bn_mul_uint(bn_in, 2, d, bn_out);

        // 验证结果不全为零（确保计算发生）
        int all_zero = 1;
        for (int i = 0; i < 3; i++)
        {
            if (bn_out[i] != 0)
            {
                all_zero = 0;
                break;
            }
        }
        assert(!all_zero);
        printf("测试用例10通过：溢出边界测试\n");
    }

    printf("所有测试用例通过！\n");
}

int main()
{
    test_cc_bn_mul_uint();
    return 0;
}
