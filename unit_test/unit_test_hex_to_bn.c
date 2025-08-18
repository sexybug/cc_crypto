#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "cc_bn_convert.h"

// 测试用例
void test_hex_to_bn()
{
    cc_bn_word_t bn[4];

    // 测试1: 正常转换 - 8位十六进制字符串
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "12345678");
    assert(bn[0] == 0x12345678);
    assert(bn[1] == 0);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试1通过: 8位十六进制字符串转换\n");

    // 测试2: 奇数长度字符串
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "1234567");
    assert(bn[0] == 0x01234567);
    assert(bn[1] == 0);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试2通过: 奇数长度字符串转换\n");

    // 测试3: 完整填充所有字节
    memset(bn, 0, sizeof(bn));
    cc_bn_from_hex(bn, 4, "0123456789ABCDEF");
    assert(bn[0] == 0x89ABCDEF);
    assert(bn[1] == 0x01234567);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试3通过: 完整填充所有字节\n");

    // 测试4: 超长字符串（超过bn容量）
    memset(bn, 0x55, sizeof(bn));
    cc_bn_from_hex(bn, 4, "123456789ABCDEF0123456789ABCDEF0123");
    // 函数应该直接返回，不修改bn
    assert(bn[0] == 0x55555555);
    assert(bn[1] == 0x55555555);
    assert(bn[2] == 0x55555555);
    assert(bn[3] == 0x55555555);
    printf("测试4通过: 超长字符串处理\n");

    // 测试5: 空字符串
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "");
    assert(bn[0] == 0);
    assert(bn[1] == 0);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试5通过: 空字符串处理\n");

    // 测试6: 单字符
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "A");
    assert(bn[0] == 0x0A);
    assert(bn[1] == 0);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试6通过: 单字符处理\n");

    // 测试7: 小写字母
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "abcdef12");
    assert(bn[0] == 0xabcdef12);
    assert(bn[1] == 0);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试7通过: 小写字母处理\n");

    // 测试8: 较小的bn_word_len
    cc_bn_word_t small_bn[1];
    memset(small_bn, 0xFF, sizeof(small_bn));
    cc_bn_from_hex(small_bn, 1, "1234");
    assert(small_bn[0] == 0x1234);
    printf("测试8通过: 较小的bn_word_len\n");

    // 测试9: 跨字节边界的奇数长度
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "123456789");
    assert(bn[0] == 0x23456789);
    assert(bn[1] == 0x1);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试9通过: 跨字节边界的奇数长度\n");

    // 测试10: 全零字符串
    memset(bn, 0xFF, sizeof(bn));
    cc_bn_from_hex(bn, 4, "00000000");
    assert(bn[0] == 0);
    assert(bn[1] == 0);
    assert(bn[2] == 0);
    assert(bn[3] == 0);
    printf("测试10通过: 全零字符串\n");
}

int main()
{
    test_hex_to_bn();
    printf("所有测试通过！\n");
    return 0;
}
