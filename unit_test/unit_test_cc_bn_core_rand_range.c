#include "cc_bn_rand.h"
#include "cc_crypto_rng.h"
#include "cc_test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    {
        cc_bn_t A[2] = {0xFFFFFFFF, 1};
        cc_bn_t B[2] = {2, 2};
        cc_bn_t R[2];
        size_t len = 2;
        cc_bn_status_t ret = cc_bn_core_rand_range(R, A, B, len, cc_crypto_rng_ex);
        print_u32("R", R, len);
        assert(ret == CC_BN_OK);
        assert(cc_bn_cmp_words(R, A, len) >= 0 && cc_bn_cmp_words(R, B, len) <= 0);
    }
    {
        cc_bn_t A[2] = {7, 0};
        cc_bn_t B[2] = {19, 0};
        cc_bn_t R[2];
        size_t len = 1;
        cc_bn_status_t ret = cc_bn_core_rand_range(R, A, B, len, cc_crypto_rng_ex);
        print_u32("R", R, len);
        assert(ret == CC_BN_OK);
        assert(cc_bn_cmp_words(R, A, len) >= 0 && cc_bn_cmp_words(R, B, len) <= 0);
    }
    {
        cc_bn_t A[2] = {7, 0};
        cc_bn_t B[2] = {19, 19};
        cc_bn_t R[2];
        size_t len = 2;
        cc_bn_status_t ret = cc_bn_core_rand_range(R, A, B, len, cc_crypto_rng_ex);
        print_u32("R", R, len);
        assert(ret == CC_BN_OK);
        assert(cc_bn_cmp_words(R, A, len) >= 0 && cc_bn_cmp_words(R, B, len) <= 0);
    }
    {
        cc_bn_t A[3] = {0, 0, 0xFFFFFFFF};
        cc_bn_t B[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        cc_bn_t R[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        size_t len = 3;
        cc_bn_status_t ret = cc_bn_core_rand_range(R, A, B, len, cc_crypto_rng_ex);
        print_u32("R", R, len);
        assert(ret == CC_BN_OK);
        assert(cc_bn_cmp_words(R, A, len) >= 0 && cc_bn_cmp_words(R, B, len) <= 0);
    }

    printf("all test pass\n");
    return 0;
}