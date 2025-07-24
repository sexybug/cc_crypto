#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "cc_bn_convert.h"

// 测试用例1：正常情况 - 字节数组完全填充大数
void test_normal_case_exact_fit() {
    printf("测试1：正常情况 - 字节数组完全填充大数\n");
    
    uint8_t src[] = {0x12, 0x34, 0x56, 0x78, 0xAB, 0xCD, 0xEF, 0x01};
    size_t byte_len = 8;
    size_t bn_word_len = 2;
    cc_bn_t bn[2];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0xABCDEF01);
    assert(bn[1] == 0x12345678);
    printf("通过\n");
}

// 测试用例2：需要零填充的情况
void test_zero_padding() {
    printf("测试2：需要零填充的情况\n");
    
    uint8_t src[] = {0x12, 0x34};
    size_t byte_len = 2;
    size_t bn_word_len = 2;
    cc_bn_t bn[2];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x00001234);
    assert(bn[1] == 0x00000000);
    printf("通过\n");
}

// 测试用例3：字节长度不是word长度的整数倍
void test_non_aligned_bytes() {
    printf("测试3：字节长度不是word长度的整数倍\n");
    
    uint8_t src[] = {0x12, 0x34, 0x56};
    size_t byte_len = 3;
    size_t bn_word_len = 1;
    cc_bn_t bn[1];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x00123456);
    printf("通过\n");
}

// 测试用例4：单字节输入
void test_single_byte() {
    printf("测试4：单字节输入\n");
    
    uint8_t src[] = {0xFF};
    size_t byte_len = 1;
    size_t bn_word_len = 1;
    cc_bn_t bn[1];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x000000FF);
    printf("通过\n");
}

// 测试用例5：空输入（零字节）
void test_zero_bytes() {
    printf("测试5：空输入（零字节）\n");
    
    uint8_t *src = NULL;
    size_t byte_len = 0;
    size_t bn_word_len = 2;
    cc_bn_t bn[2] = {0xFFFFFFFF, 0xFFFFFFFF}; // 预填充非零值
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x00000000);
    assert(bn[1] == 0x00000000);
    printf("通过\n");
}

// 测试用例6：字节长度超过bn容量的边界情况
void test_byte_len_exceeds_capacity() {
    printf("测试6：字节长度超过bn容量的边界情况\n");
    
    uint8_t src[] = {0x12, 0x34, 0x56, 0x78, 0xAB, 0xCD, 0xEF, 0x01, 0x23};
    size_t byte_len = 9; // 超过2个word的容量(8字节)
    size_t bn_word_len = 2;
    cc_bn_t bn[2] = {0xFFFFFFFF, 0xFFFFFFFF}; // 预填充值
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    // 函数应该直接返回，不修改bn数组
    assert(bn[0] == 0xFFFFFFFF);
    assert(bn[1] == 0xFFFFFFFF);
    printf("通过\n");
}

// 测试用例7：字节长度等于bn容量的边界情况
void test_byte_len_equals_capacity() {
    printf("测试7：字节长度等于bn容量的边界情况\n");
    
    uint8_t src[] = {0x12, 0x34, 0x56, 0x78, 0xAB, 0xCD, 0xEF, 0x01};
    size_t byte_len = 8; // 等于2个word的容量
    size_t bn_word_len = 2;
    cc_bn_t bn[2];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0xABCDEF01);
    assert(bn[1] == 0x12345678);
    printf("通过\n");
}

// 测试用例8：大数组测试
void test_large_array() {
    printf("测试8：大数组测试\n");
    
    uint8_t src[12] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC};
    size_t byte_len = 12;
    size_t bn_word_len = 3;
    cc_bn_t bn[3];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x99AABBCC);
    assert(bn[1] == 0x55667788);
    assert(bn[2] == 0x11223344);
    printf("通过\n");
}

// 测试用例9：bn_word_len为1的边界情况
void test_single_word_bn() {
    printf("测试9：bn_word_len为1的边界情况\n");
    
    uint8_t src[] = {0xAB, 0xCD};
    size_t byte_len = 2;
    size_t bn_word_len = 1;
    cc_bn_t bn[1];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x0000ABCD);
    printf("通过\n");
}

// 测试用例10：全零字节数组
void test_all_zero_bytes() {
    printf("测试10：全零字节数组\n");
    
    uint8_t src[] = {0x00, 0x00, 0x00, 0x00};
    size_t byte_len = 4;
    size_t bn_word_len = 2;
    cc_bn_t bn[2];
    
    cc_u8_to_bn(src, byte_len, bn, bn_word_len);
    
    assert(bn[0] == 0x00000000);
    assert(bn[1] == 0x00000000);
    printf("通过\n");
}

int main() {
    printf("开始cc_u8_to_bn函数单元测试\n\n");
    
    test_normal_case_exact_fit();
    test_zero_padding();
    test_non_aligned_bytes();
    test_single_byte();
    test_zero_bytes();
    test_byte_len_exceeds_capacity();
    test_byte_len_equals_capacity();
    test_large_array();
    test_single_word_bn();
    test_all_zero_bytes();
    
    printf("\n所有测试用例通过！\n");
    return 0;
}
