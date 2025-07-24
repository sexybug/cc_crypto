#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"

// 辅助函数：比较两个大数是否相等
int bn_equal(const cc_bn_t *a, const cc_bn_t *b, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}


// 测试用例1：基本加法，无进位
void test_basic_addition_no_carry() {
    printf("测试1：基本加法，无进位\n");
    cc_bn_t input[] = {0x10000000, 0x20000000, 0x30000000};
    cc_bn_t output[3];
    cc_bn_t expected[] = {0x10000005, 0x20000000, 0x30000000};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 3, 0x5);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 3));
    printf("通过\n\n");
}

// 测试用例2：单个字进位
void test_single_word_carry() {
    printf("测试2：单个字进位\n");
    cc_bn_t input[] = {0xFFFFFFFF, 0x10000000, 0x20000000};
    cc_bn_t output[3];
    cc_bn_t expected[] = {0x00000000, 0x10000001, 0x20000000};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 3, 0x1);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 3));
    printf("通过\n\n");
}

// 测试用例3：多个字连续进位
void test_multiple_word_carry() {
    printf("测试3：多个字连续进位\n");
    cc_bn_t input[] = {0xFFFFFFFF, 0xFFFFFFFF, 0x10000000};
    cc_bn_t output[3];
    cc_bn_t expected[] = {0x00000000, 0x00000000, 0x10000001};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 3, 0x1);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 3));
    printf("通过\n\n");
}

// 测试用例4：最终产生进位
void test_final_carry() {
    printf("测试4：最终产生进位\n");
    cc_bn_t input[] = {0xFFFFFFFF, 0xFFFFFFFF};
    cc_bn_t output[2];
    cc_bn_t expected[] = {0x00000000, 0x00000000};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 2, 0x1);
    
    assert(carry == 1);
    assert(bn_equal(output, expected, 2));
    printf("通过\n\n");
}

// 测试用例5：加0
void test_add_zero() {
    printf("测试5：加0\n");
    cc_bn_t input[] = {0x12345678, 0x87654321};
    cc_bn_t output[2];
    cc_bn_t expected[] = {0x12345678, 0x87654321};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 2, 0x0);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 2));
    printf("通过\n\n");
}

// 测试用例6：单字长度
void test_single_word_length() {
    printf("测试6：单字长度\n");
    cc_bn_t input[] = {0x80000000};
    cc_bn_t output[1];
    cc_bn_t expected[] = {0x80000001};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 1, 0x1);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 1));
    printf("通过\n\n");
}

// 测试用例7：单字长度产生进位
void test_single_word_carry_out() {
    printf("测试7：单字长度产生进位\n");
    cc_bn_t input[] = {0xFFFFFFFF};
    cc_bn_t output[1];
    cc_bn_t expected[] = {0x00000000};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 1, 0x1);
    
    assert(carry == 1);
    assert(bn_equal(output, expected, 1));
    printf("通过\n\n");
}

// 测试用例8：加最大值
void test_add_max_digit() {
    printf("测试8：加最大值\n");
    cc_bn_t input[] = {0x1, 0x0};
    cc_bn_t output[2];
    cc_bn_t expected[] = {0x0, 0x1};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 2, 0xFFFFFFFF);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 2));
    printf("通过\n\n");
}

// 测试用例9：零长度（边界情况）
void test_zero_length() {
    printf("测试9：零长度\n");
    cc_bn_t *input = NULL;
    cc_bn_t *output = NULL;
    
    cc_bn_t carry = cc_bn_add_word(output, input, 0, 0x5);
    
    assert(carry == 0x5);
    printf("通过\n\n");
}

// 测试用例10：就地操作（输入输出相同）
void test_in_place_operation() {
    printf("测试10：就地操作\n");
    cc_bn_t data[] = {0x10000000, 0x20000000, 0x30000000};
    cc_bn_t expected[] = {0x1000000A, 0x20000000, 0x30000000};
    
    cc_bn_t carry = cc_bn_add_word(data, data, 3, 0xA);
    
    assert(carry == 0);
    assert(bn_equal(data, expected, 3));
    printf("通过\n\n");
}

// 测试用例11：边界值测试
void test_boundary_values() {
    printf("测试11：边界值测试\n");
    cc_bn_t input[] = {0x7FFFFFFF, 0x7FFFFFFF};
    cc_bn_t output[2];
    cc_bn_t expected[] = {0x80000000, 0x7FFFFFFF};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 2, 0x1);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 2));
    printf("通过\n\n");
}

// 测试用例12：连续进位传播测试
void test_carry_propagation() {
    printf("测试12：连续进位传播测试\n");
    cc_bn_t input[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x12345678};
    cc_bn_t output[4];
    cc_bn_t expected[] = {0x00000001, 0x00000000, 0x00000000, 0x12345679};
    
    cc_bn_t carry = cc_bn_add_word(output, input, 4, 0x2);
    
    assert(carry == 0);
    assert(bn_equal(output, expected, 4));
    printf("通过\n\n");
}

int main() {
    printf("开始32位大数加法单元测试\n\n");
    
    test_basic_addition_no_carry();
    test_single_word_carry();
    test_multiple_word_carry();
    test_final_carry();
    test_add_zero();
    test_single_word_length();
    test_single_word_carry_out();
    test_add_max_digit();
    test_zero_length();
    test_in_place_operation();
    test_boundary_values();
    test_carry_propagation();
    
    printf("所有测试通过！\n");
    return 0;
}
