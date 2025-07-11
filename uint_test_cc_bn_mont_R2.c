#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_bn.h"

int main(void)
{
    {
        cc_bn_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_t R2[3];
        cc_bn_t expected[2] = {0x38be92c5, 0x38be92c4};

        cc_bn_mont_R2(N, N_len, R2);
        assert(memcmp(R2, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_t N[2] = {0x87654321, 0x12345678};
        size_t N_len = 2;
        cc_bn_t R2[3];
        cc_bn_t expected[2] = {0xffbc0000, 0x443ffff};

        cc_bn_mont_R2(N, N_len, R2);
        assert(memcmp(R2, expected, sizeof(expected)) == 0);
    }

    printf("uint_test_cc_bn_mont_R2: PASS\n");

    return 0;
}