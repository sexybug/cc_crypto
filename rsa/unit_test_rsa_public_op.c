#include "cc_bn_rand.h"
#include "cc_crypto_rng.h"
#include "cc_bn_prime.h"
#include "cc_bn_convert.h"
#include "cc_rsa_core.h"
#include "cc_test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    {
        cc_rsa_pubkey_st pubkey;
        cc_bn_word_t N[CC_BN_MAX_WORDS];
        cc_bn_word_t E[CC_BN_MAX_WORDS];
        cc_bn_word_t M[CC_BN_MAX_WORDS];
        cc_bn_word_t C[CC_BN_MAX_WORDS];
        cc_bn_word_t C_expect[CC_BN_MAX_WORDS];

        size_t bits = 128;
        size_t N_words = cc_bn_word_len_from_bit_len(bits);

        cc_bn_from_hex(N, N_words, "C1725F0E7D61419EA4E7E4B78B79D443");
        cc_bn_from_hex(E, N_words, "010001");
        cc_bn_from_hex(M, N_words, "5563E803DC628EF4F369BCB9C90BB041");
        cc_bn_from_hex(C_expect, N_words, "5258AAA637C639CF5B05C7D6BA9FE1A6");

        cc_rsa_set_pubkey(&pubkey, N, E, bits);

        cc_status_t ret = cc_rsa_core_public_op(&pubkey, M, C);
        assert(ret == CC_OK);
        print_bn("C", C, N_words);
        assert(cc_bn_cmp_words(C, C_expect, N_words) == 0);
    }

    {
        cc_rsa_pubkey_st pubkey;
        cc_bn_word_t N[CC_BN_MAX_WORDS];
        cc_bn_word_t E[CC_BN_MAX_WORDS];
        cc_bn_word_t M[CC_BN_MAX_WORDS];
        cc_bn_word_t C[CC_BN_MAX_WORDS];
        cc_bn_word_t C_expect[CC_BN_MAX_WORDS];

        size_t bits = 80;
        size_t N_words = cc_bn_word_len_from_bit_len(bits);

        cc_bn_from_hex(N, N_words, "BC1A28744920C1528515");
        cc_bn_from_hex(E, N_words, "010001");
        cc_bn_from_hex(M, N_words, "94CD0C9B8FB164765381");
        cc_bn_from_hex(C_expect, N_words, "8F0E946E24A018EC2998");

        cc_rsa_set_pubkey(&pubkey, N, E, bits);

        cc_status_t ret = cc_rsa_core_public_op(&pubkey, M, C);
        assert(ret == CC_OK);
        print_bn("C", C, N_words);
        assert(cc_bn_cmp_words(C, C_expect, N_words) == 0);
    }

    printf("all tests pass\n");
    return 0;
}