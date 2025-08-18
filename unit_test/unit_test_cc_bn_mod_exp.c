#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_bn_mod.h"

int main(void)
{
    {
        cc_bn_word_t A[2] = {0x77777777, 0x77777777};
        cc_bn_word_t E[1] = {3};
        cc_bn_word_t N[1] = {0x87654321};
        size_t N_len = 1;
        cc_bn_word_t expected[1] = {0x3ACF49D1};
        cc_bn_word_t C[1];

        cc_bn_mod_exp(C, A, 2, E, 1, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[1] = {0x7};
        cc_bn_word_t E[1] = {1};
        cc_bn_word_t N[1] = {0x9};
        size_t N_len = 1;
        cc_bn_word_t R2[1];
        cc_bn_word_t expected[1] = {0x7};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 1, E, 1, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[2] = {0, 0};
        cc_bn_word_t E[2] = {0x55555555, 0x55555555};
        cc_bn_word_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[2] = {0, 0};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 2, E, 2, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[2] = {0x77777777, 0x77777777};
        cc_bn_word_t E[2] = {0x55555555, 0x55555555};
        cc_bn_word_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[2] = {0x11f4d553, 0x11f4d553};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 2, E, 2, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[3] = {0x77777777, 0x77777777, 0x77777777};
        cc_bn_word_t E[2] = {0x55555555, 0x55555555};
        cc_bn_word_t N[3] = {0x87654321, 0x87654321, 0x87654321};
        size_t N_len = 3;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[3] = {0x67DD38F1, 0x67DD38F1, 0x67DD38F1};
        cc_bn_word_t C[3];

        cc_bn_mod_exp(C, A, 3, E, 2, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[2] = {0x77777777, 0x77777777};
        cc_bn_word_t E[1] = {0x55555555};
        cc_bn_word_t N[1] = {0x87654321};
        size_t N_len = 1;
        cc_bn_word_t expected[1] = {0x731906E6};
        cc_bn_word_t C[1];

        cc_bn_mod_exp(C, A, 2, E, 1, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[3] = {0x77777777, 0x77777777, 0x77777777};
        cc_bn_word_t E[2] = {0x55555555, 0x55555555};
        cc_bn_word_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[2] = {0xB07CB2C5, 0x52152CB4};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 3, E, 2, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[3] = {0x77777777, 0x77777777, 0x77777777};
        cc_bn_word_t E[3] = {0x55555555, 0x55555555, 0x55555555};
        cc_bn_word_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[2] = {0x77B47146, 0x36F431FB};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 3, E, 3, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[2] = {0x77777777, 0x77777777};
        cc_bn_word_t E[2] = {0x55555555, 0x55555555};
        cc_bn_word_t N[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t N_len = 2;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[2] = {0x77777777, 0x77777777};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 2, E, 2, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[2] = {0xFFFFFFFF, 0xFFFFFFF1};
        cc_bn_word_t E[2] = {0xFFFFFFFF, 0xFFFFFFF2};
        cc_bn_word_t N[2] = {0xFFFFFFFF, 0xFFFFFFFE};
        size_t N_len = 2;
        cc_bn_word_t R2[2];
        cc_bn_word_t expected[2] = {0x820b8580, 0x53ab4c59};
        cc_bn_word_t C[2];

        cc_bn_mod_exp(C, A, 2, E, 2, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t A[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
        cc_bn_word_t E[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
        cc_bn_word_t N[3] = {0xFFFFFFFF, 0xFFFFFFF1, 0xFFFFFFFF};
        size_t N_len = 3;
        cc_bn_word_t R2[3];
        cc_bn_word_t expected[3] = {0xac3eaab5, 0x6b35f104, 0x0f8a3103};
        cc_bn_word_t C[3];

        cc_bn_mod_exp(C, A, 3, E, 3, N, N_len);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    printf("uint_test_cc_bn_mont_exp: PASS\n");

    return 0;
}