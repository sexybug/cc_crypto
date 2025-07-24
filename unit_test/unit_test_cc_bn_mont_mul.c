#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_bn_mul.h"


int main(void)
{
    {
        cc_bn_t A[2] = {7};
        cc_bn_t B[2] = {1};
        cc_bn_t N[2] = {9};
        size_t N_len = 1;
        cc_bn_t R2[2];
        cc_bn_t expected[1] = {7};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0x77777777};
        cc_bn_t B[2] = {1};
        cc_bn_t N[2] = {0x87654321};
        size_t N_len = 1;
        cc_bn_t R2[2];
        cc_bn_t expected[1] = {0x77777777};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0x77777777};
        cc_bn_t B[2] = {1};
        cc_bn_t N[2] = {0xFFFFFFFF};
        size_t N_len = 1;
        cc_bn_t R2[2];
        cc_bn_t expected[1] = {0x77777777};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0x77777777, 0x77777777};
        cc_bn_t B[2] = {0x55555555, 0x55555555};
        cc_bn_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_t R2[2];
        cc_bn_t expected[2] = {0x79beba92, 0x79beba92};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0x77777777, 0x77777777};
        cc_bn_t B[2] = {0x55555555, 0x55555555};
        cc_bn_t N[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t N_len = 2;
        cc_bn_t R2[2];
        cc_bn_t expected[2] = {0x55555555, 0x55555555};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t A[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t B[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t N[2] = {0xFFFFFFFF, 0xFFFFFFF1};
        size_t N_len = 2;
        cc_bn_t R2[2];
        cc_bn_t expected[2] = {0x000000c4, 0x00000ab8};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        // A,B must < N
        cc_bn_t A[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
        cc_bn_t B[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0};
        cc_bn_t N[3] = {0xFFFFFFFF, 0xFFFFFFF1, 0xFFFFFFFF};
        size_t N_len = 3;
        cc_bn_t R2[3];
        cc_bn_t expected[3] = {0x00000001, 0x00000001, 0x0000000c};
        cc_bn_t montA[3], montB[3], montC[3], C[3];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    {
        // A,B must < N
        cc_bn_t A[3] = {0xFFFFFFFF, 0, 0};
        cc_bn_t B[3] = {0xFFFFFFFF, 0, 0};
        cc_bn_t N[3] = {0x00000001, 0x00000001, 0};
        size_t N_len = 3;
        cc_bn_t R2[3];
        cc_bn_t expected[3] = {0x04, 0x00000000, 0x00000000};
        cc_bn_t montA[3], montB[3], montC[3], C[3];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_mul(B, R2, N, N_len, Ni, montB);
        cc_bn_mont_mul(montA, montB, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    printf("uint_test_cc_bn_mont_mul: PASS\n");

    return 0;
}