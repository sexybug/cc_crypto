#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "cc_test.h"

// 单元测试
void test_bn_to_hex() {
    printf("开始测试 bn_to_hex 函数...\n");
    
    // 测试用例1：单个字为0
    {
        cc_bn_word_t bn1[] = {0x0};
        char hex1[16];
        size_t len1 = bn_to_hex(bn1, 1, hex1);
        assert(len1 == 8);
        assert(strcmp(hex1, "00000000") == 0);
        printf("测试1通过：单个0值\n");
    }
    
    // 测试用例2：单个字最大值
    {
        cc_bn_word_t bn2[] = {0xFFFFFFFF};
        char hex2[16];
        size_t len2 = bn_to_hex(bn2, 1, hex2);
        assert(len2 == 8);
        assert(strcmp(hex2, "FFFFFFFF") == 0);
        printf("测试2通过：单个最大值\n");
    }
    
    // 测试用例3：单个字特定值
    {
        cc_bn_word_t bn3[] = {0x12345678};
        char hex3[16];
        size_t len3 = bn_to_hex(bn3, 1, hex3);
        assert(len3 == 8);
        assert(strcmp(hex3, "12345678") == 0);
        printf("测试3通过：单个特定值\n");
    }
    
    // 测试用例4：多个字（两个字）
    {
        cc_bn_word_t bn4[] = {0x11111111, 0x22222222};
        char hex4[32];
        size_t len4 = bn_to_hex(bn4, 2, hex4);
        assert(len4 == 16);
        assert(strcmp(hex4, "2222222211111111") == 0);
        printf("测试4通过：两个字（注意字节序）\n");
    }
    
    // 测试用例5：多个字包含0
    {
        cc_bn_word_t bn5[] = {0x0, 0xAAAAAAAA};
        char hex5[32];
        size_t len5 = bn_to_hex(bn5, 2, hex5);
        assert(len5 == 16);
        assert(strcmp(hex5, "AAAAAAAA00000000") == 0);
        printf("测试5通过：多个字包含0\n");
    }
    
    // 测试用例6：三个字
    {
        cc_bn_word_t bn6[] = {0x00000001, 0x00000002, 0x00000003};
        char hex6[32];
        size_t len6 = bn_to_hex(bn6, 3, hex6);
        assert(len6 == 24);
        assert(strcmp(hex6, "000000030000000200000001") == 0);
        printf("测试6通过：三个字\n");
    }
    
    // 测试用例7：边界值测试
    {
        cc_bn_word_t bn7[] = {0x80000000};
        char hex7[16];
        size_t len7 = bn_to_hex(bn7, 1, hex7);
        assert(len7 == 8);
        assert(strcmp(hex7, "80000000") == 0);
        printf("测试7通过：边界值（最高位为1）\n");
    }
    
    // 测试用例8：所有位都是1和0的交替模式
    {
        cc_bn_word_t bn8[] = {0x55555555};
        char hex8[16];
        size_t len8 = bn_to_hex(bn8, 1, hex8);
        assert(len8 == 8);
        assert(strcmp(hex8, "55555555") == 0);
        printf("测试8通过：交替位模式\n");
    }
    
    // 测试用例9：四个字的复杂组合
    {
        cc_bn_word_t bn9[] = {0x76543210, 0x89ABCDEF, 0xCAFEBABE, 0x12345678};
        char hex9[64];
        size_t len9 = bn_to_hex(bn9, 4, hex9);
        assert(len9 == 32);
        assert(strcmp(hex9, "12345678CAFEBABE89ABCDEF76543210") == 0);
        printf("测试9通过：四个字复杂组合\n");
    }
    
    // 测试用例10：空输入边界情况
    {
        char hex10[16];
        size_t len10 = bn_to_hex(NULL, 0, hex10);
        assert(len10 == 0);
        assert(hex10[0] == '\0');
        printf("测试10通过：空输入\n");
    }
    
    // 测试用例11：包含小写字母的十六进制值
    {
        cc_bn_word_t bn11[] = {0xABCDEF01};
        char hex11[16];
        size_t len11 = bn_to_hex(bn11, 1, hex11);
        assert(len11 == 8);
        assert(strcmp(hex11, "ABCDEF01") == 0);
        printf("测试11通过：包含字母的十六进制值\n");
    }
    
    // 测试用例12：测试最小非零值
    {
        cc_bn_word_t bn12[] = {0x00000001};
        char hex12[16];
        size_t len12 = bn_to_hex(bn12, 1, hex12);
        assert(len12 == 8);
        assert(strcmp(hex12, "00000001") == 0);
        printf("测试12通过：最小非零值\n");
    }
    
    printf("所有测试通过！\n");
}

// 辅助函数：打印测试结果
void print_test_result(const char* test_name, const char* expected, const char* actual, size_t expected_len, size_t actual_len) {
    printf("测试: %s\n", test_name);
    printf("期望长度: %zu, 实际长度: %zu\n", expected_len, actual_len);
    printf("期望结果: %s\n", expected);
    printf("实际结果: %s\n", actual);
    printf("结果: %s\n\n", (strcmp(expected, actual) == 0 && expected_len == actual_len) ? "通过" : "失败");
}

int main() {
    test_bn_to_hex();
    return 0;
}
