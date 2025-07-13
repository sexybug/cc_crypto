#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_bn_mul.h"
#include "cc_bn_exp.h"

int main(void)
{
    cc_bn_t one[3] = {1, 0, 0};
    {
        cc_bn_t A[2] = {0xFFFFFFF8, 0x0808FFFF};
        cc_bn_t N[2] = {0x7C16BAD7, 0xCDD1ADCB};
        size_t N_len = 2;
        cc_bn_t R2[2];
        cc_bn_t expected[2] = {0xc87dfd59, 0x35fc3cd2};
        cc_bn_t montA[2], montB[2], montC[2], C[2];

        cc_bn_t Ni = cc_bn_mont_Ni(N);
        cc_bn_mont_R2(N, N_len, R2);

        cc_bn_mont_mul(A, R2, N, N_len, Ni, montA);
        cc_bn_mont_sqrt_p3(montA, N, N_len, Ni, montC);
        // cc_bn_mont_mul(montC, one, N, N_len, Ni, C);
        cc_bn_mont_mul_word(montC, 1, N, N_len, Ni, C);

        assert(memcmp(C, expected, sizeof(expected)) == 0);
    }

    printf("uint_test_cc_bn_mont_sqrt_p3: PASS\n");

    return 0;
}