#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn_mod.h"
#include "cc_bn_config.h"

int main()
{
    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {5};
        cc_bn_t B[CC_BN_MAX_WORDS] = {3};
        cc_bn_t G1[] = {1};
        cc_bn_t X1[] = {2};
        cc_bn_t Y1[] = {3};

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];
        size_t bn_word_len = 1;
        cc_status_t status = cc_bn_core_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {0xFFA61464, 0x1583164D};
        cc_bn_t B[CC_BN_MAX_WORDS] = {0x010001, 0};
        cc_bn_t G1[] = {1, 0};
        cc_bn_t X1[] = {0x0000f2c5, 0x00000000};
        cc_bn_t Y1[] = {0x812138f3, 0x1466648f};

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];
        size_t bn_word_len = 2;
        cc_status_t status = cc_bn_core_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    printf("all test cases pass\n");
    return 0;
}
