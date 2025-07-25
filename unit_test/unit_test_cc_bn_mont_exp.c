#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_bn_mont.h"

int main(void)
{
    cc_bn_t one[3] = {1, 0, 0};
    // TODO: fix bugs
    {
        cc_bn_t A[1] = {0x7};
        cc_bn_t B[1] = {1};
        cc_bn_t N[1] = {0x9};
        size_t N_len = 1;
        cc_bn_t RR[1];
        cc_bn_t expected[1] = {0x7};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_RR(RR, N, N_len);

        cc_bn_core_mont_mul(montA, A, RR, N, N_len, Ni);
        cc_bn_mont_exp(montC, montA, B, N, N_len, Ni);
        cc_bn_core_mont_mul(C, montC, one, N, N_len, Ni);
        // cc_bn_core_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0x77777777, 0x77777777};
        cc_bn_t B[2] = {0x55555555, 0x55555555};
        cc_bn_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_t RR[2];
        cc_bn_t expected[2] = {0x11f4d553, 0x11f4d553};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_RR(RR, N, N_len);

        cc_bn_core_mont_mul(montA, A, RR, N, N_len, Ni);
        cc_bn_mont_exp(montC, montA, B, N, N_len, Ni);
        // cc_bn_core_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_core_mont_mul_word(C, montC, 1, N, N_len, Ni);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0x77777777, 0x77777777};
        cc_bn_t B[2] = {0x55555555, 0x55555555};
        cc_bn_t N[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t N_len = 2;
        cc_bn_t RR[2];
        cc_bn_t expected[2] = {0x77777777, 0x77777777};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_RR(RR, N, N_len);

        cc_bn_core_mont_mul(montA, A, RR, N, N_len, Ni);
        cc_bn_mont_exp(montC, montA, B, N, N_len, Ni);
        // cc_bn_core_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_core_mont_mul_word(C, montC, 1, N, N_len, Ni);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0xFFFFFFFF, 0xFFFFFFF1};
        cc_bn_t B[2] = {0xFFFFFFFF, 0xFFFFFFF2};
        cc_bn_t N[2] = {0xFFFFFFFF, 0xFFFFFFFE};
        size_t N_len = 2;
        cc_bn_t RR[2];
        cc_bn_t expected[2] = {0x820b8580, 0x53ab4c59};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_RR(RR, N, N_len);

        cc_bn_core_mont_mul(montA, A, RR, N, N_len, Ni);
        cc_bn_mont_exp(montC, montA, B, N, N_len, Ni);
        // cc_bn_core_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_core_mont_mul_word(C, montC, 1, N, N_len, Ni);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
        cc_bn_t B[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
        cc_bn_t N[3] = {0xFFFFFFFF, 0xFFFFFFF1, 0xFFFFFFFF};
        size_t N_len = 3;
        cc_bn_t RR[3];
        cc_bn_t expected[3] = {0xac3eaab5, 0x6b35f104, 0x0f8a3103};
        cc_bn_t montA[3], montB[3], montC[3], C[3];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_RR(RR, N, N_len);

        cc_bn_core_mont_mul(montA, A, RR, N, N_len, Ni);
        cc_bn_mont_exp(montC, montA, B, N, N_len, Ni);
        // cc_bn_core_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_core_mont_mul_word(C, montC, 1, N, N_len, Ni);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    printf("uint_test_cc_bn_mont_exp: PASS\n");

    return 0;
}