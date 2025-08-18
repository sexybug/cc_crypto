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
        cc_rsa_privkey_st privkey;
        cc_bn_word_t N[CC_BN_MAX_WORDS];
        cc_bn_word_t D[CC_BN_MAX_WORDS];
        cc_bn_word_t M[CC_BN_MAX_WORDS];
        cc_bn_word_t C[CC_BN_MAX_WORDS];
        cc_bn_word_t M_expect[CC_BN_MAX_WORDS];

        size_t bits = 80;
        size_t N_words = cc_bn_word_len_from_bit_len(bits);

        cc_bn_from_hex(N, N_words, "BC1A28744920C1528515");
        cc_bn_from_hex(D, N_words, "94CD0C9B8FB164765381");
        cc_bn_from_hex(C, N_words, "8F0E946E24A018EC2998");
        cc_bn_from_hex(M_expect, N_words, "94CD0C9B8FB164765381");

        cc_rsa_set_privkey(&privkey, N, D, bits);

        cc_status_t ret = cc_rsa_core_private_op(&privkey, C, M);
        assert(ret == CC_OK);
        print_bn("M", M, N_words);
        assert(cc_bn_cmp_words(M, M_expect, N_words) == 0);
    }

    {
        cc_rsa_privkey_st privkey;
        cc_bn_word_t N[CC_BN_MAX_WORDS];
        cc_bn_word_t D[CC_BN_MAX_WORDS];
        cc_bn_word_t M[CC_BN_MAX_WORDS];
        cc_bn_word_t C[CC_BN_MAX_WORDS];
        cc_bn_word_t M_expect[CC_BN_MAX_WORDS];

        size_t bits = 160;
        size_t N_words = cc_bn_word_len_from_bit_len(bits);

        cc_bn_from_hex(N, N_words, "D63CE7CA25E0EA5A3FD5EDDA39B8B61DAE069451");
        cc_bn_from_hex(D, N_words, "2BF027D5B4B911375B39E3A4487103102FF0A8A5");
        cc_bn_from_hex(C, N_words, "BDF4B027BFCAB0C4B7B6CF693DC46ED51C331F45");
        cc_bn_from_hex(M_expect, N_words, "2BF027D5B4B911375B39E3A4487103102FF0A8A5");

        cc_rsa_set_privkey(&privkey, N, D, bits);

        cc_status_t ret = cc_rsa_core_private_op(&privkey, C, M);
        assert(ret == CC_OK);
        print_bn("M", M, N_words);
        assert(cc_bn_cmp_words(M, M_expect, N_words) == 0);
    }

    printf("all tests pass\n");
    return 0;
}