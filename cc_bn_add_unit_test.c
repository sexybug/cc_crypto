#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"
#include "test.h"

// 单元测试函数
void test_cc_bn_add() {
    // 测试用例1: 基本加法，无进位
    {
        cc_bn_digit_t in1[] = {0x1234567890ABCDEF};
        cc_bn_digit_t in2[] = {0x1111111111111111};
        cc_bn_digit_t out[1];
        cc_bn_digit_t expected[] = {0x23456789A1BCDF00};
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 1, out);
        assert(carry == 0);
        assert(out[0] == expected[0]);
    }
    
    // 测试用例2: 单字进位
    {
        cc_bn_digit_t in1[] = {0xFFFFFFFFFFFFFFFF};
        cc_bn_digit_t in2[] = {0x0000000000000001};
        cc_bn_digit_t out[1];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 1, out);
        assert(carry == 1);
        assert(out[0] == 0);
    }
    
    // 测试用例3: 两个最大值相加
    {
        cc_bn_digit_t in1[] = {0xFFFFFFFFFFFFFFFF};
        cc_bn_digit_t in2[] = {0xFFFFFFFFFFFFFFFF};
        cc_bn_digit_t out[1];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 1, out);
        assert(carry == 1);
        assert(out[0] == 0xFFFFFFFFFFFFFFFE);
    }
    
    // 测试用例4: 零值相加
    {
        cc_bn_digit_t in1[] = {0x0000000000000000};
        cc_bn_digit_t in2[] = {0x0000000000000000};
        cc_bn_digit_t out[1];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 1, out);
        assert(carry == 0);
        assert(out[0] == 0);
    }
    
    // 测试用例5: 多字无进位
    {
        cc_bn_digit_t in1[] = {0x1111111111111111, 0x2222222222222222};
        cc_bn_digit_t in2[] = {0x3333333333333333, 0x4444444444444444};
        cc_bn_digit_t out[2];
        cc_bn_digit_t expected[] = {0x4444444444444444, 0x6666666666666666};
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 2, out);
        assert(carry == 0);
        assert(out[0] == expected[0]);
        assert(out[1] == expected[1]);
    }
    
    // 测试用例6: 多字连续进位
    {
        cc_bn_digit_t in1[] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
        cc_bn_digit_t in2[] = {0x0000000000000001, 0x0000000000000000};
        cc_bn_digit_t out[2];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 2, out);
        assert(carry == 1);
        assert(out[0] == 0);
        assert(out[1] == 0);
    }
    
    // 测试用例7: 部分进位
    {
        cc_bn_digit_t in1[] = {0xFFFFFFFFFFFFFFFF, 0x1111111111111111, 0x2222222222222222};
        cc_bn_digit_t in2[] = {0x0000000000000001, 0x2222222222222222, 0x3333333333333333};
        cc_bn_digit_t out[3];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 3, out);
        assert(carry == 0);
        assert(out[0] == 0);
        assert(out[1] == 0x3333333333333334);
        assert(out[2] == 0x5555555555555555);
    }
    
    // 测试用例8: 长度为0的边界测试
    {
        cc_bn_digit_t in1[] = {0};
        cc_bn_digit_t in2[] = {0};
        cc_bn_digit_t out[1];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 0, out);
        assert(carry == 0);
    }
    
    // 测试用例9: 大数组测试
    {
        const size_t len = 10;
        cc_bn_digit_t in1[10];
        cc_bn_digit_t in2[10];
        cc_bn_digit_t out[10];
        
        // 填充测试数据
        for (size_t i = 0; i < len; i++) {
            in1[i] = 0x1111111111111111;
            in2[i] = 0x2222222222222222;
        }
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, len, out);
        assert(carry == 0);
        
        for (size_t i = 0; i < len; i++) {
            assert(out[i] == 0x3333333333333333);
        }
    }
    
    // 测试用例10: 交替最大值测试
    {
        cc_bn_digit_t in1[] = {0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF};
        cc_bn_digit_t in2[] = {0x0000000000000001, 0xFFFFFFFFFFFFFFFF, 0x0000000000000001};
        cc_bn_digit_t out[3];
        
        cc_bn_digit_t carry = cc_bn_add(in1, in2, 3, out);
        assert(carry == 1);
        assert(out[0] == 0);
        assert(out[1] == 0);
        assert(out[2] == 1);
    }
}

int main() {
    test_cc_bn_add();
    printf("所有测试用例通过！\n");
    return 0;
}
