#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "cc_bn.h"


// 测试辅助函数：比较两个大数是否相等
int bn_equal(const cc_bn_t *a, const cc_bn_t *b, size_t len) {
    return memcmp(a, b, len * sizeof(cc_bn_t)) == 0;
}

void test_cc_bn_mul_words() {
    printf("开始测试 cc_bn_mul_words 函数\n");
    
    // 测试用例1：零乘以任意数
    {
        printf("\n测试用例1：零乘以任意数\n");
        cc_bn_t A[] = {0, 0};
        cc_bn_t B[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_t R[4];
        cc_bn_t expected[] = {0, 0, 0, 0};
        
        cc_bn_mul_words(A, B, 2, R);
        assert(bn_equal(R, expected, 4));
        printf("✓ 通过\n");
    }
    
    // 测试用例2：任意数乘以零
    {
        printf("\n测试用例2：任意数乘以零\n");
        cc_bn_t A[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_t B[] = {0, 0};
        cc_bn_t R[4];
        cc_bn_t expected[] = {0, 0, 0, 0};
        
        cc_bn_mul_words(A, B, 2, R);
        assert(bn_equal(R, expected, 4));
        printf("✓ 通过\n");
    }
    
    // 测试用例3：单字长度乘法
    {
        printf("\n测试用例3：单字长度乘法\n");
        cc_bn_t A[] = {0x12345678};
        cc_bn_t B[] = {0x9ABCDEF0};
        cc_bn_t R[2];
        
        cc_bn_mul_words(A, B, 1, R);
        
        // 手动计算期望结果：0x12345678 * 0x9ABCDEF0
        cc_bn_dword_t expected_full = (cc_bn_dword_t)0x12345678 * 0x9ABCDEF0;
        cc_bn_t expected[] = {
            (cc_bn_t)(expected_full & 0xFFFFFFFF),
            (cc_bn_t)(expected_full >> 32)
        };
        
        assert(bn_equal(R, expected, 2));
        printf("✓ 通过\n");
    }
    
    // 测试用例4：一乘以任意数
    {
        printf("\n测试用例4：一乘以任意数\n");
        cc_bn_t A[] = {1, 0};
        cc_bn_t B[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_t R[4];
        cc_bn_t expected[] = {0x12345678, 0x9ABCDEF0, 0, 0};
        
        cc_bn_mul_words(A, B, 2, R);
        assert(bn_equal(R, expected, 4));
        printf("✓ 通过\n");
    }
    
    // 测试用例5：任意数乘以一
    {
        printf("\n测试用例5：任意数乘以一\n");
        cc_bn_t A[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_t B[] = {1, 0};
        cc_bn_t R[4];
        cc_bn_t expected[] = {0x12345678, 0x9ABCDEF0, 0, 0};
        
        cc_bn_mul_words(A, B, 2, R);
        assert(bn_equal(R, expected, 4));
        printf("✓ 通过\n");
    }
    
    // 测试用例6：最大值乘法（边界情况）
    {
        printf("\n测试用例6：最大值乘法\n");
        cc_bn_t A[] = {0xFFFFFFFF};
        cc_bn_t B[] = {0xFFFFFFFF};
        cc_bn_t R[2];
        cc_bn_t expected[] = {0x00000001, 0xFFFFFFFE};
        
        cc_bn_mul_words(A, B, 1, R);
        assert(bn_equal(R, expected, 2));
        printf("✓ 通过\n");
    }
    
    // 测试用例7：多字长度乘法
    {
        printf("\n测试用例7：多字长度乘法\n");
        cc_bn_t A[] = {0x12345678, 0x9ABCDEF0, 0x11111111};
        cc_bn_t B[] = {0x22222222, 0x33333333, 0x44444444};
        cc_bn_t R[6];
        cc_bn_t expected[] = {0xca5f6bf0, 0x508dfea4,0x69a96090,0xf966a0a8,0x4a424b60,0x048d159e};
        
        cc_bn_mul_words(A, B, 3, R);
        assert(bn_equal(R, expected, 6));
        printf("✓ 通过\n");
    }
    
    // 测试用例8：交换律验证
    {
        printf("\n测试用例8：交换律验证 (A*B = B*A)\n");
        cc_bn_t A[] = {0x12345678, 0x9ABCDEF0};
        cc_bn_t B[] = {0xFEDCBA98, 0x76543210};
        cc_bn_t R1[4], R2[4];
        
        cc_bn_mul_words(A, B, 2, R1);
        cc_bn_mul_words(B, A, 2, R2);
        
        assert(bn_equal(R1, R2, 4));
        printf("✓ 通过\n");
    }
    
    // 测试用例9：长度为1的边界情况
    {
        printf("\n测试用例9：长度为1的各种情况\n");
        cc_bn_t test_values[] = {0, 1, 2, 0x80000000, 0xFFFFFFFF};
        int num_values = sizeof(test_values) / sizeof(test_values[0]);
        
        for (int i = 0; i < num_values; i++) {
            for (int j = 0; j < num_values; j++) {
                cc_bn_t A[] = {test_values[i]};
                cc_bn_t B[] = {test_values[j]};
                cc_bn_t R[2];
                
                cc_bn_mul_words(A, B, 1, R);
                
                // 验证结果等于直接相乘
                cc_bn_dword_t expected_full = (cc_bn_dword_t)test_values[i] * test_values[j];
                cc_bn_t expected[] = {
                    (cc_bn_t)(expected_full & 0xFFFFFFFF),
                    (cc_bn_t)(expected_full >> 32)
                };
                
                assert(bn_equal(R, expected, 2));
            }
        }
        printf("✓ 通过\n");
    }
    
    // 测试用例10：进位处理
    {
        printf("\n测试用例10：进位处理\n");
        cc_bn_t A[] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t B[] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[4];
        cc_bn_t expected[] = {0x00000001, 0x00000000, 0xfffffffe, 0xffffffff};
        
        cc_bn_mul_words(A, B, 2, R);
        assert(bn_equal(R, expected, 4));
        printf("✓ 通过\n");
    }
    
    printf("\n所有测试用例通过！\n");
}

int main() {
    test_cc_bn_mul_words();
    return 0;
}
