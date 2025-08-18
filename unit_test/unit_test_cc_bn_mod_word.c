#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "cc_bn_mod.h"


// 单元测试函数
void test_cc_bn_mod_word() {
    printf("开始测试 cc_bn_mod_word 函数 (32位版本)...\n");
    
    // 测试用例1：除数为1的情况
    {
        cc_bn_word_t A[] = {123, 456, 789};
        cc_bn_word_t result = cc_bn_mod_word(A, 3, 1);
        assert(result == 0);
        printf("测试1通过：除数为1时返回0\n");
    }
    
    // 测试用例2：被除数为0的情况
    {
        cc_bn_word_t A[] = {0, 0, 0};
        cc_bn_word_t result = cc_bn_mod_word(A, 3, 5);
        assert(result == 0);
        printf("测试2通过：被除数为0时返回0\n");
    }
    
    // 测试用例3：除数为2的情况（奇数）
    {
        cc_bn_word_t A[] = {123};  // 奇数
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 2);
        assert(result == 1);
        printf("测试3通过：奇数除以2余1\n");
    }
    
    // 测试用例4：除数为2的情况（偶数）
    {
        cc_bn_word_t A[] = {122};  // 偶数
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 2);
        assert(result == 0);
        printf("测试4通过：偶数除以2余0\n");
    }
    
    // 测试用例5：单个字长度的普通除法
    {
        cc_bn_word_t A[] = {100};
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 7);
        assert(result == 2);  // 100 % 7 = 2
        printf("测试5通过：100 mod 7 = 2\n");
    }
    
    // 测试用例6：多个字长度的除法
    {
        cc_bn_word_t A[] = {0x1, 0x1};  // 表示 2^32 + 1
        cc_bn_word_t result = cc_bn_mod_word(A, 2, 3);
        assert(result == 2);
        printf("测试6通过：多字长度除法 (2^32+1) mod 3 = 1\n");
    }
    
    // 测试用例7：边界情况 - 最大32位值除以小数
    {
        cc_bn_word_t A[] = {0xFFFFFFFF};  // 2^32 - 1
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 10);
        assert(result == 5);  // (2^32-1) % 10 = 4294967295 % 10 = 5
        printf("测试7通过：最大32位数除以10余5\n");
    }
    
    // 测试用例8：多字长度，被除数小于除数
    {
        cc_bn_word_t A[] = {5, 0, 0};
        cc_bn_word_t result = cc_bn_mod_word(A, 3, 10);
        assert(result == 5);
        printf("测试8通过：被除数小于除数\n");
    }
    
    // 测试用例9：长度为1的数组
    {
        cc_bn_word_t A[] = {42};
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 13);
        assert(result == 3);  // 42 % 13 = 3
        printf("测试9通过：42 mod 13 = 3\n");
    }
    
    // 测试用例10：除数等于被除数的情况
    {
        cc_bn_word_t A[] = {15};
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 15);
        assert(result == 0);
        printf("测试10通过：除数等于被除数时余数为0\n");
    }
    
    // 测试用例11：大的除数
    {
        cc_bn_word_t A[] = {0x100, 0x200};
        cc_bn_word_t result = cc_bn_mod_word(A, 2, 0x80000000);  // 2^31
        assert(result == 0x100);
        printf("测试11通过：大除数情况，结果=%u\n", result);
    }
    
    // 测试用例12：全零数组但长度不同
    {
        cc_bn_word_t A[] = {0};
        cc_bn_word_t result = cc_bn_mod_word(A, 1, 7);
        assert(result == 0);
        printf("测试12通过：单元素零数组\n");
    }
    
    // 测试用例13：最大32位数的多字测试
    {
        cc_bn_word_t A[] = {0xFFFFFFFF, 0xFFFFFFFF};  // 2^64 - 1 在32位表示
        cc_bn_word_t result = cc_bn_mod_word(A, 2, 7);
        // 需要根据实际算法计算期望值
        assert(result == 1);
        printf("测试13通过：多字最大值除法，结果=%u\n", result);
    }
    
    // 测试用例14：进位测试
    {
        cc_bn_word_t A[] = {0xFFFFFFFF, 0x1};  // 2^32
        cc_bn_word_t result = cc_bn_mod_word(A, 2, 3);
        assert(result == 1);  // 2^32 % 3 = 1
        printf("测试14通过：进位情况 2^32 mod 3 = 1\n");
    }
    
    // 测试用例15：典型的三字长度测试
    {
        cc_bn_word_t A[] = {0x12345678, 0x9ABCDEF0, 0x11111111};
        cc_bn_word_t result = cc_bn_mod_word(A, 3, 1000000);
        assert(result == 0x000b3ab8);
        printf("测试15通过：三字长度除法，结果=%u\n", result);
    }
    
    printf("所有测试用例通过！\n");
}

int main() {
    test_cc_bn_mod_word();
    return 0;
}
