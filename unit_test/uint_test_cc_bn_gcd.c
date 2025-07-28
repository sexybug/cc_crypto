#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cc_bn_mod.h"

int main(int argc, char **argv)
{
    {
        cc_bn_t A[3] = {0};
        cc_bn_t B[3] = {18};
        cc_bn_t G[] = {18};
        cc_bn_t D[3];

        size_t A_word_len = 1;
        size_t B_word_len = 1;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
        printf("A=0 test passed.\n");
    }
    {
        cc_bn_t A[3] = {48};
        cc_bn_t B[3] = {0};
        cc_bn_t G[] = {48};
        cc_bn_t D[3];

        size_t A_word_len = 1;
        size_t B_word_len = 1;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
    }
    {
        cc_bn_t A[3] = {48};
        cc_bn_t B[3] = {18};
        cc_bn_t G[] = {6};
        cc_bn_t D[3];

        size_t A_word_len = 1;
        size_t B_word_len = 1;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
        printf("B=0 test passed.\n");
    }
    {
        cc_bn_t A[3] = {48};
        cc_bn_t B[3] = {18};
        cc_bn_t G[] = {6};
        cc_bn_t D[3];

        size_t A_word_len = 1;
        size_t B_word_len = 1;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
        printf("bn_word_len=1 test passed.\n");
    }
    {
        cc_bn_t A[3] = {0x48, 0x48};
        cc_bn_t B[3] = {0x18, 0x18};
        cc_bn_t G[] = {0x18, 0x18};
        cc_bn_t D[3];

        size_t A_word_len = 2;
        size_t B_word_len = 2;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
        printf("bn_word_len=2 test passed.\n");
    }
    {
        cc_bn_t A[3] = {0x48, 0x48};
        cc_bn_t B[3] = {0x10000018};
        cc_bn_t G[] = {0x8};
        cc_bn_t D[3];

        size_t A_word_len = 2;
        size_t B_word_len = 1;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
        printf("A len > B len test passed.\n");
    }
    {
        cc_bn_t A[3] = {0x48, 0x48};
        cc_bn_t B[3] = {0x00000018, 0x00000018, 0x00000018};
        cc_bn_t G[] = {0x48, 0};
        cc_bn_t D[3];

        size_t A_word_len = 2;
        size_t B_word_len = 3;

        size_t D_word_len = cc_bn_gcd(D, A, A_word_len, B, B_word_len);

        assert(D_word_len == sizeof(G) / sizeof(G[0]));
        assert(cc_bn_cmp_words(D, G, sizeof(G) / sizeof(G[0])) == 0);
        printf("B len > A len test passed.\n");
    }

    printf("All tests passed.\n");
    return 0;
}
