#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "cc_bn_mod.h"

int main()
{
    {
        const size_t word_len = 4; // 128位大数
        cc_bn_t A[4] = {1, 0, 0, 0};
        cc_bn_t B[4] = {1, 0, 0, 0};
        cc_bn_t N[4] = {0xFFFFFFFF, 0, 0, 0};
        cc_bn_t R[4] = {0, 0, 0, 0};
        cc_bn_t expected[4] = {2, 0, 0, 0};
        cc_bn_mod_add(R, A, B, N, word_len);
        assert(memcmp(R, expected, sizeof(R)) == 0);
    }

    {
        const size_t word_len = 4; // 128位大数
        cc_bn_t A[4] = {0xFFFFFFFE, 0, 0, 0};
        cc_bn_t B[4] = {0xFFFFFFFE, 0, 0, 0};
        cc_bn_t N[4] = {0xFFFFFFFF, 0, 0, 0};
        cc_bn_t R[4] = {0, 0, 0, 0};
        cc_bn_t expected[4] = {0xfffffffd, 0, 0, 0};
        cc_bn_mod_add(R, A, B, N, word_len);
        assert(memcmp(R, expected, sizeof(R)) == 0);
    }

    {
        const size_t word_len = 4; // 128位大数
        cc_bn_t A[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t B[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[4] = {0, 0, 0, 0};
        cc_bn_t expected[4] = {0xfffffffd, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_mod_add(R, A, B, N, word_len);
        assert(memcmp(R, expected, sizeof(R)) == 0);
    }

    {
        const size_t word_len = 4; // 128位大数
        cc_bn_t A[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t B[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[4] = {0, 0, 0, 0};
        cc_bn_t expected[4] = {0xfffffffd, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_mod_add(R, A, B, N, word_len);
        assert(memcmp(R, expected, sizeof(R)) == 0);
    }

    {
        const size_t word_len = 0; // 128位大数
        cc_bn_t A[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t B[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[4] = {0, 1, 2, 3};
        cc_bn_t expected[4] = {0, 1, 2, 3};
        cc_bn_mod_add(R, A, B, N, word_len);
        assert(memcmp(R, expected, sizeof(R)) == 0);
    }

    {
        const size_t word_len = 4; // 128位大数
        cc_bn_t A[4] = {0, 0, 0, 0};
        cc_bn_t B[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[4] = {0, 1, 2, 3};
        cc_bn_t expected[4] = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_mod_add(R, A, B, N, word_len);
        assert(memcmp(R, expected, sizeof(R)) == 0);
    }

    printf("All tests passed!\n");

    return 0;
}
