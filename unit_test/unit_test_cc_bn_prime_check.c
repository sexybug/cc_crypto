#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cc_crypto_rng.h"
#include "cc_bn_prime.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));
    {
        cc_bn_t N[] = {0xF};
        size_t N_len = sizeof(N) / sizeof(N[0]);

        cc_status_t res = cc_bn_prime_check(N, N_len, cc_crypto_rng_ex);

        assert(res == CC_BN_IS_COMPOSITE);
    }

    {
        cc_bn_t N[] = {151};
        size_t N_len = sizeof(N) / sizeof(N[0]);

        cc_status_t res = cc_bn_prime_check(N, N_len, cc_crypto_rng_ex);

        assert(res == CC_BN_IS_PRIME);
    }

    {
        cc_bn_t N[] = {0x7FFFFFFF};
        size_t N_len = sizeof(N) / sizeof(N[0]);

        cc_status_t res = cc_bn_prime_check(N, N_len, cc_crypto_rng_ex);

        assert(res == CC_BN_PROBABLY_PRIME);
    }

    {
        cc_bn_t N[2] = {0x7C16BAD7, 0xCDD1ADCB};
        size_t N_len = sizeof(N) / sizeof(N[0]);

        cc_status_t res = cc_bn_prime_check(N, N_len, cc_crypto_rng_ex);

        assert(res == CC_BN_PROBABLY_PRIME);
    }

    {
        cc_bn_t N[] = {0x13F19AA1, 0x9D82ACEA, 0x5A65CF14, 0xCDCB013B, 0xE8E78DBF, 0x88690DFA, 0x30ACF433, 0x37715F17, 0x45ED9520, 0x2623AF8E, 0xE8CE2897, 0xF3F1F057, 0x18CE3A80, 0x9A494832, 0x26F97633, 0xF3232706, 0x2710678D, 0xBCE1CDDB, 0x2C1300B9, 0xC37C99DD, 0x435B6D90, 0xA74C2B76, 0x63F39740, 0xFFC68199, 0x18681191, 0x58AEBE20, 0x915775B7, 0xC25321BD, 0xE0D25987, 0x664E6B99, 0x3FE882F8, 0xEE06D253};
        size_t N_len = sizeof(N) / sizeof(N[0]);

        cc_status_t res = cc_bn_prime_check(N, N_len, cc_crypto_rng_ex);

        assert(res == CC_BN_PROBABLY_PRIME);
    }

    {
        cc_bn_t N[] = {0x19321309, 0x21085789, 0x2B24031F, 0xC7540D47, 0x65B0C5D3, 0x8F903DA7, 0x8069C16E, 0x2F88CC37, 0x6D2601E9, 0x9FBC8C45, 0x524CC7A1, 0xA22EE23A, 0x71317D3F, 0x9F104209, 0x6DAF57DC, 0x1B6AA692, 0x21C7A304, 0xCE5CA067, 0x2952CD21, 0xE00362BC, 0xA73C1BDE, 0x665296B2, 0x1374638B, 0xE6C2045E, 0x437B60BB, 0x9602004B, 0xE2E8FC42, 0xF4DA4519, 0x15A4B872, 0x15B862CC, 0x2E6DE367, 0x723B78AC, 0xF7F8FD45, 0x770408A6, 0xB90F830F, 0xE336F380, 0x66DCE24F, 0x1C84B283, 0x387B6615, 0x937A6A28, 0x32927A05, 0x61CA0F29, 0x5BC0A2E1, 0x63E67A86, 0x6AF2B4AD, 0xC94654FA, 0xA1FFC226, 0xCE77F96B, 0x20992060, 0x10177C2F, 0x1EDDB68C, 0x85D88F53, 0x3196E601, 0xB4386C5F, 0xB6A46D4D, 0x2E0F1CF1, 0x70A5A85B, 0x7E337C51, 0x70FB75E7, 0x56C15934, 0x585D3D0D, 0x573E0197, 0xB6217E58, 0xCCF73179};
        size_t N_len = sizeof(N) / sizeof(N[0]);

        cc_status_t res = cc_bn_prime_check(N, N_len, cc_crypto_rng_ex);

        assert(res == CC_BN_IS_COMPOSITE);
    }

    printf("uint_test_cc_bn_prime_check: PASS\n");

    return 0;
}