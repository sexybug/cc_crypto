#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn.h"


// 测试用例
void test_cc_bn_sub_uint()
{
    cc_bn_digit_t bn_in[4];
    cc_bn_digit_t bn_out[4];
    cc_bn_digit_t borrow;
    
    printf("开始测试 cc_bn_sub_uint 函数...\n");
    
    // 测试用例1: 基本减法，无借位
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 10;
    bn_in[1] = 20;
    bn_in[2] = 30;
    bn_in[3] = 40;
    borrow = cc_bn_sub_uint(bn_in, 4, 5, bn_out);
    assert(bn_out[0] == 5);
    assert(bn_out[1] == 20);
    assert(bn_out[2] == 30);
    assert(bn_out[3] == 40);
    assert(borrow == 0);
    printf("测试用例1通过: 基本减法，无借位\n");
    
    // 测试用例2: 需要借位的情况
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 3;
    bn_in[1] = 0;
    bn_in[2] = 0;
    bn_in[3] = 0;
    borrow = cc_bn_sub_uint(bn_in, 4, 5, bn_out);
    assert(bn_out[0] == (cc_bn_digit_t)(3 - 5)); // 下溢出
    assert(bn_out[1] == (cc_bn_digit_t)(-1)); // 借位传播
    assert(bn_out[2] == (cc_bn_digit_t)(-1)); // 借位传播
    assert(bn_out[3] == (cc_bn_digit_t)(-1)); // 借位传播
    assert(borrow == 1);
    printf("测试用例2通过: 需要借位的情况\n");
    
    // 测试用例3: 减数为0
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 100;
    bn_in[1] = 200;
    bn_in[2] = 300;
    borrow = cc_bn_sub_uint(bn_in, 3, 0, bn_out);
    assert(bn_out[0] == 100);
    assert(bn_out[1] == 200);
    assert(bn_out[2] == 300);
    assert(borrow == 0);
    printf("测试用例3通过: 减数为0\n");
    
    // 测试用例4: 被减数为0
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    borrow = cc_bn_sub_uint(bn_in, 3, 5, bn_out);
    assert(bn_out[0] == (cc_bn_digit_t)(0 - 5)); // 下溢出
    assert(bn_out[1] == (cc_bn_digit_t)(-1)); // 借位传播
    assert(bn_out[2] == (cc_bn_digit_t)(-1)); // 借位传播
    assert(borrow == 1);
    printf("测试用例4通过: 被减数为0\n");
    
    // 测试用例5: 单字长度测试
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 15;
    borrow = cc_bn_sub_uint(bn_in, 1, 10, bn_out);
    assert(bn_out[0] == 5);
    assert(borrow == 0);
    printf("测试用例5通过: 单字长度，无借位\n");
    
    // 测试用例6: 单字长度需要借位
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 5;
    borrow = cc_bn_sub_uint(bn_in, 1, 10, bn_out);
    assert(bn_out[0] == (cc_bn_digit_t)(5 - 10)); // 下溢出
    assert(borrow == 1);
    printf("测试用例6通过: 单字长度，需要借位\n");
    
    // 测试用例7: 最大值测试
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = (cc_bn_digit_t)(-1); // 最大值
    borrow = cc_bn_sub_uint(bn_in, 1, 1, bn_out);
    assert(bn_out[0] == (cc_bn_digit_t)(-2));
    assert(borrow == 0);
    printf("测试用例7通过: 最大值减1\n");
    
    // 测试用例8: 连续借位传播
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 0;
    bn_in[1] = 0;
    bn_in[2] = 1;
    borrow = cc_bn_sub_uint(bn_in, 3, 1, bn_out);
    assert(bn_out[0] == (cc_bn_digit_t)(-1)); // 借位
    assert(bn_out[1] == (cc_bn_digit_t)(-1)); // 借位传播
    assert(bn_out[2] == 0); // 1-1=0
    assert(borrow == 0);
    printf("测试用例8通过: 连续借位传播\n");
    
    // 测试用例9: 长度为0的边界情况
    borrow = cc_bn_sub_uint(bn_in, 0, 5, bn_out);
    assert(borrow == 5); // 返回原始减数
    printf("测试用例9通过: 长度为0的边界情况\n");
    
    // 测试用例10: 相等数值相减
    memset(bn_in, 0, sizeof(bn_in));
    memset(bn_out, 0, sizeof(bn_out));
    bn_in[0] = 100;
    borrow = cc_bn_sub_uint(bn_in, 1, 100, bn_out);
    assert(bn_out[0] == 0);
    assert(borrow == 0);
    printf("测试用例10通过: 相等数值相减\n");
    
    printf("所有测试用例通过！\n");
}

int main()
{
    test_cc_bn_sub_uint();
    return 0;
}
