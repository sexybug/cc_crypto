#include "cc_bn_rand.h"
#include "cc_crypto_rng.h"
#include "cc_bn_prime.h"
#include "cc_test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand32(time(NULL));
    {
        cc_bn_word_t R[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t bits = 32;
        size_t len = cc_bn_word_len_from_bit_len(bits);
        cc_status_t ret = cc_bn_gen_prime(R, bits, cc_crypto_rng_ex);
        print_bn("R", R, len);
        assert(ret == CC_OK);
    }
    {
        cc_bn_word_t R[2] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t bits = 48;
        size_t len = cc_bn_word_len_from_bit_len(bits);
        cc_status_t ret = cc_bn_gen_prime(R, bits, cc_crypto_rng_ex);
        print_bn("R", R, len);
        assert(ret == CC_OK);
    }

    {
        cc_bn_word_t R[256] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t bits = 99;
        size_t len = cc_bn_word_len_from_bit_len(bits);
        cc_status_t ret = cc_bn_gen_prime(R, bits, cc_crypto_rng_ex);
        print_bn("R", R, len);
        assert(ret == CC_OK);
    }

    {
        cc_bn_word_t R[256] = {0xFFFFFFFF, 0xFFFFFFFF};
        size_t bits = 2048;
        size_t len = cc_bn_word_len_from_bit_len(bits);
        cc_status_t ret = cc_bn_gen_prime(R, bits, cc_crypto_rng_ex);
        print_bn("R", R, len);
        assert(ret == CC_OK);
    }

    printf("all test pass\n");
    return 0;
}