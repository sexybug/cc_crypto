#include "cc_bn_rand.h"
#include "cc_crypto_rng.h"
#include "cc_bn_prime.h"
#include "cc_rsa_core.h"
#include "cc_test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    {
        cc_rsa_privkey_st privkey;
        size_t bits = 64;
        size_t words = (bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
        size_t PQ_words = ((bits / 2) + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
        cc_bn_t E[] = {65537};
        size_t E_word_len = 1;
        bool crt = true;
        cc_status_t ret = cc_rsa_core_gen_key(&privkey, bits, E, E_word_len, crt, cc_crypto_rng_ex);
        assert(ret == CC_OK);
        print_bn("N", privkey.N, words);
        print_bn("D", privkey.D, words);
        print_bn("E", privkey.E, words);
        print_bn("P", privkey.P, PQ_words);
        print_bn("Q", privkey.Q, PQ_words);
        print_bn("DP", privkey.DP, PQ_words);
        print_bn("DQ", privkey.DQ, PQ_words);
        print_bn("QP", privkey.QP, PQ_words);

        assert(cc_rsa_validate_params(privkey.bits, privkey.N, privkey.E, privkey.D, privkey.P, privkey.Q, cc_crypto_rng_ex) == CC_OK);
        assert(cc_rsa_validate_crt(privkey.bits, privkey.D, privkey.P, privkey.Q, privkey.DP, privkey.DQ, privkey.QP) == CC_OK);
    }

    {
        cc_rsa_privkey_st privkey;
        size_t bits = 64+16;
        size_t words = (bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
        size_t PQ_words = ((bits / 2) + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
        cc_bn_t E[] = {0x7E84DB71,0x011CB1BE};
        size_t E_word_len = 2;
        bool crt = true;
        cc_status_t ret = cc_rsa_core_gen_key(&privkey, bits, E, E_word_len, crt, cc_crypto_rng_ex);
        assert(ret == CC_OK);
        print_bn("N", privkey.N, words);
        print_bn("D", privkey.D, words);
        print_bn("E", privkey.E, words);
        print_bn("P", privkey.P, PQ_words);
        print_bn("Q", privkey.Q, PQ_words);
        print_bn("DP", privkey.DP, PQ_words);
        print_bn("DQ", privkey.DQ, PQ_words);
        print_bn("QP", privkey.QP, PQ_words);

        assert(cc_rsa_validate_params(privkey.bits, privkey.N, privkey.E, privkey.D, privkey.P, privkey.Q, cc_crypto_rng_ex) == CC_OK);
        assert(cc_rsa_validate_crt(privkey.bits, privkey.D, privkey.P, privkey.Q, privkey.DP, privkey.DQ, privkey.QP) == CC_OK);
    }

    printf("all tests pass\n");
    return 0;
}