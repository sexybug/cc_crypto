#include <stdio.h>
#include <assert.h>
#include "cc_bn.h"

int main() {
    // 测试用例1: 没有进位
    cc_bn_digit_t bn_in1[] = {5, 10, 15};
    cc_bn_digit_t bn_out1[3];
    cc_bn_digit_t carry1 = cc_bn_add_uint(bn_in1, 3, 1, bn_out1);
    assert(carry1 == 0);
    assert(bn_out1[0] == 6);
    assert(bn_out1[1] == 10);
    assert(bn_out1[2] == 15);

    // 测试用例2: 有进位
    cc_bn_digit_t bn_in2[] = {4294967295, 4294967295};
    cc_bn_digit_t bn_out2[2];
    cc_bn_digit_t carry2 = cc_bn_add_uint(bn_in2, 2, 1, bn_out2);
    assert(carry2 == 1);
    assert(bn_out2[0] == 0);
    assert(bn_out2[1] == 0);

    // 测试用例4: 输入数组长度为1
    cc_bn_digit_t bn_in4[] = {4294967295};
    cc_bn_digit_t bn_out4[1];
    cc_bn_digit_t carry4 = cc_bn_add_uint(bn_in4, 1, 1, bn_out4);
    assert(carry4 == 1);
    assert(bn_out4[0] == 0);

    // 测试用例5: 输入数组长度大于1，且进位发生在中间
    cc_bn_digit_t bn_in5[] = {4294967295, 0, 4294967295};
    cc_bn_digit_t bn_out5[3];
    cc_bn_digit_t carry5 = cc_bn_add_uint(bn_in5, 3, 1, bn_out5);
    assert(carry5 == 0);
    assert(bn_out5[0] == 0);
    assert(bn_out5[1] == 1);
    assert(bn_out5[2] == 4294967295);

    printf("所有测试用例通过!\n");
    return 0;
}
