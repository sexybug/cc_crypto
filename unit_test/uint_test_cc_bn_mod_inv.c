#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn_mod.h"
#include "cc_bn_config.h"

int main()
{
    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t N[CC_BN_MAX_WORDS] = {1};
        cc_bn_t R[CC_BN_MAX_WORDS];
        size_t A_word_len = 1;
        size_t N_word_len = 1;
        cc_bn_status_t status;

        // 测试1: N <= 1 的情况
        status = cc_bn_mod_inv(R, A, A_word_len, N, N_word_len);
        assert(status == CC_BN_ERR_INVALID_ARG);
    }
    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {0x07};
        cc_bn_t N[CC_BN_MAX_WORDS] = {0xFFFFFFFF};
        cc_bn_t expect[] = {0x49249249};
        cc_bn_t R[CC_BN_MAX_WORDS];
        size_t A_word_len = 1;
        size_t N_word_len = 1;
        cc_bn_status_t status;

        // 测试1: N <= 1 的情况
        status = cc_bn_mod_inv(R, A, A_word_len, N, N_word_len);
        assert(status == CC_BN_OK);
        assert(memcmp(R, expect, N_word_len * CC_BN_WORD_BYTES) == 0);
    }
    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[CC_BN_MAX_WORDS] = {0xFFFFFFF1, 0xFFFFFFFF};
        cc_bn_t expect[] = {0x318BC2A4, 0xE7306867};
        cc_bn_t R[CC_BN_MAX_WORDS];
        size_t A_word_len = 3;
        size_t N_word_len = 2;
        cc_bn_status_t status;

        // 测试1: A>N 的情况
        status = cc_bn_mod_inv(R, A, A_word_len, N, N_word_len);
        assert(status == CC_BN_OK);
        assert(memcmp(R, expect, N_word_len * CC_BN_WORD_BYTES) == 0);
    }
    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[CC_BN_MAX_WORDS] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t expect[] = {0x318BC2A4, 0xE7306867};
        cc_bn_t R[CC_BN_MAX_WORDS];
        size_t A_word_len = 3;
        size_t N_word_len = 2;
        cc_bn_status_t status;

        // 测试1: A和N不互素 的情况
        status = cc_bn_mod_inv(R, A, A_word_len, N, N_word_len);
        assert(status == CC_BN_ERR_NO_INVERSE);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {0xFFFFFFFF};
        cc_bn_t N[CC_BN_MAX_WORDS] = {0xFFFFFFF1, 0xFFFFFFFF};
        cc_bn_t expect[] = {0xA492491B, 0xA4924924};
        cc_bn_t R[CC_BN_MAX_WORDS];
        size_t A_word_len = 1;
        size_t N_word_len = 2;
        cc_bn_status_t status;

        // 测试1: A<N 的情况
        status = cc_bn_mod_inv(R, A, A_word_len, N, N_word_len);
        assert(status == CC_BN_OK);
        assert(memcmp(R, expect, N_word_len * CC_BN_WORD_BYTES) == 0);
    }

    printf("all test cases pass\n");
    return 0;
}
