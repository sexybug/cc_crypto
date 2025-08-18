#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_bn_mont.h"
#include "cc_test.h"

int main(void)
{
    {
        cc_bn_word_t N[2] = {0x87654321, 0x87654321};
        size_t N_len = 2;
        cc_bn_word_t RR[2];
        cc_bn_word_t expected[2] = {0x38be92c5, 0x38be92c4};

        cc_bn_mont_RR(RR, N, N_len);
        assert(memcmp(RR, expected, sizeof(expected)) == 0);
    }

    {
        cc_bn_word_t N[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t N_len = 2;
        cc_bn_word_t RR[2];
        cc_bn_word_t expected[2] = {1, 0};

        cc_bn_mont_RR(RR, N, N_len);
        assert(memcmp(RR, expected, sizeof(expected)) == 0);
    }

        {
        cc_bn_word_t N[8] = {0xFFFFFFF1, 0xFFFFFFF1, 0xFFFFFFF1, 0xFFFFFFF1, 0xFFFFFFF1, 0xFFFFFFF1, 0xFFFFFFFF, 0xFFFFFFFF};
        size_t N_len = 7;
        cc_bn_word_t RR[8];
        // cc_bn_word_t expected[2] = {1, 0};

        cc_bn_mont_RR(RR, N, N_len);
        print_bn("RR", RR, N_len);
    }

    printf("uint_test_cc_bn_mont_R2: PASS\n");

    return 0;
}