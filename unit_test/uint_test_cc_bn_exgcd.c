#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cc_bn_mod.h"
#include "cc_bn_config.h"
#include "cc_bn_convert.h"

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
        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS] = {48};
        cc_bn_t B[CC_BN_MAX_WORDS] = {18};
        cc_bn_t G1[] = {6};
        cc_bn_t X1[] = {0x10};
        cc_bn_t Y1[] = {0x2A};

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];
        size_t bn_word_len = 1;
        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
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
        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t B[CC_BN_MAX_WORDS];
        cc_bn_t G1[CC_BN_MAX_WORDS];
        cc_bn_t X1[CC_BN_MAX_WORDS];
        cc_bn_t Y1[CC_BN_MAX_WORDS];
        size_t bn_word_len = 2;

        cc_bn_from_hex(A, bn_word_len, "DE339A6B49B27108");
        cc_bn_from_hex(B, bn_word_len, "5D502F1D7EE6C3F4");
        cc_bn_from_hex(G1, bn_word_len, "000000000000000c");
        cc_bn_from_hex(X1, bn_word_len, "378fb399cb266960");
        cc_bn_from_hex(Y1, bn_word_len, "844e1be350b39dc4");

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];

        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t B[CC_BN_MAX_WORDS];
        cc_bn_t G1[CC_BN_MAX_WORDS];
        cc_bn_t X1[CC_BN_MAX_WORDS];
        cc_bn_t Y1[CC_BN_MAX_WORDS];
        size_t bn_word_len = 2;

        cc_bn_from_hex(A, bn_word_len, "FFFFFFFFFFFFFFFF");
        cc_bn_from_hex(B, bn_word_len, "FFFFFFF7");
        cc_bn_from_hex(G1, bn_word_len, "0000000000000001");
        cc_bn_from_hex(X1, bn_word_len, "b6666660");
        cc_bn_from_hex(Y1, bn_word_len, "b666666669999999");

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];

        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t B[CC_BN_MAX_WORDS];
        cc_bn_t G1[CC_BN_MAX_WORDS];
        cc_bn_t X1[CC_BN_MAX_WORDS];
        cc_bn_t Y1[CC_BN_MAX_WORDS];
        size_t bn_word_len = 2;

        cc_bn_from_hex(A, bn_word_len, "FFFFFFFFFFFFFFFF");
        cc_bn_from_hex(B, bn_word_len, "7");
        cc_bn_from_hex(G1, bn_word_len, "0000000000000001");
        cc_bn_from_hex(X1, bn_word_len, "1");
        cc_bn_from_hex(Y1, bn_word_len, "2492492492492492");

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];

        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t B[CC_BN_MAX_WORDS];
        cc_bn_t G1[CC_BN_MAX_WORDS];
        cc_bn_t X1[CC_BN_MAX_WORDS];
        cc_bn_t Y1[CC_BN_MAX_WORDS];
        size_t bn_word_len = 2;

        cc_bn_from_hex(A, bn_word_len, "FFFFFFFFFFFFFFFF");
        cc_bn_from_hex(B, bn_word_len, "FFFFFFFFFFFFFFF7");
        cc_bn_from_hex(G1, bn_word_len, "0000000000000001");
        cc_bn_from_hex(X1, bn_word_len, "1fffffffffffffff");
        cc_bn_from_hex(Y1, bn_word_len, "2000000000000000");

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];

        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t B[CC_BN_MAX_WORDS];
        cc_bn_t G1[CC_BN_MAX_WORDS];
        cc_bn_t X1[CC_BN_MAX_WORDS];
        cc_bn_t Y1[CC_BN_MAX_WORDS];
        size_t bn_word_len = 2;

        cc_bn_from_hex(A, bn_word_len, "FFFFFFFFFFFFFFF4");
        cc_bn_from_hex(B, bn_word_len, "8");
        cc_bn_from_hex(G1, bn_word_len, "4");
        cc_bn_from_hex(X1, bn_word_len, "4");
        cc_bn_from_hex(Y1, bn_word_len, "7ffffffffffffff8");

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];

        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    {
        cc_bn_t A[CC_BN_MAX_WORDS];
        cc_bn_t B[CC_BN_MAX_WORDS];
        cc_bn_t G1[CC_BN_MAX_WORDS];
        cc_bn_t X1[CC_BN_MAX_WORDS];
        cc_bn_t Y1[CC_BN_MAX_WORDS];
        size_t bn_word_len = 2;

        cc_bn_from_hex(A, bn_word_len, "FFFFFFFFFFFFFFF0");
        cc_bn_from_hex(B, bn_word_len, "F7");
        cc_bn_from_hex(G1, bn_word_len, "d");
        cc_bn_from_hex(X1, bn_word_len, "de");
        cc_bn_from_hex(Y1, bn_word_len, "e616cd36ee616cc5");

        cc_bn_t G[CC_BN_MAX_WORDS], X[CC_BN_MAX_WORDS + 1], Y[CC_BN_MAX_WORDS + 1];

        cc_status_t status = cc_bn_binary_exgcd_unsafe(G, X, Y, A, B, bn_word_len);
        assert(status == CC_OK);
        assert(memcmp(G, G1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(X, X1, bn_word_len * CC_BN_WORD_BYTES) == 0);
        assert(memcmp(Y, Y1, bn_word_len * CC_BN_WORD_BYTES) == 0);
    }

    printf("all test cases pass\n");
    return 0;
}
