
#include "cc_bn_gcd.h"
#include "cc_bn_config.h"

// D = gcd(A, B), refer to mbedtls
// A_word_len must >= B_word_len
// return D_word_len, D_word_len = A_word_len or B_word_len
// D can alias A B
size_t cc_bn_gcd_unsafe(cc_bn_t *D, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    if (cc_bn_is_zero(A, A_word_len))
    {
        cc_bn_copy(D, B, B_word_len);
        return B_word_len;
    }

    if (cc_bn_is_zero(B, B_word_len))
    {
        cc_bn_copy(D, A, A_word_len);
        return A_word_len;
    }

    cc_bn_t AT[CC_BN_MAX_WORDS];
    cc_bn_t BT[CC_BN_MAX_WORDS];

    size_t A_lsb = cc_bn_lsb(A, A_word_len);
    size_t B_lsb = cc_bn_lsb(B, B_word_len);
    size_t shift = A_lsb <= B_lsb ? A_lsb : B_lsb;

    // make AT BT odd
    cc_bn_rshift(AT, A, A_word_len, A_lsb);
    cc_bn_rshift(BT, B, B_word_len, B_lsb);

    while (!cc_bn_is_zero(AT, B_word_len))
    {
        size_t AT_lsb = cc_bn_lsb(AT, A_word_len);
        size_t BT_lsb = cc_bn_lsb(BT, B_word_len);
        cc_bn_rshift(AT, AT, A_word_len, AT_lsb);
        cc_bn_rshift(BT, BT, B_word_len, BT_lsb);

        if (cc_bn_cmp(AT, A_word_len, BT, B_word_len) >= 0)
        {
            cc_bn_sub_small(AT, AT, A_word_len, BT, B_word_len);
            cc_bn_rshift_1(AT, AT, A_word_len);
        }
        else
        {
            cc_bn_sub_words(BT, BT, AT, B_word_len);
            cc_bn_rshift_1(BT, BT, B_word_len);
        }
    }

    cc_bn_lshift(D, BT, B_word_len, shift);
    return B_word_len;
}

// D = gcd(A, B)
// return D_word_len, D_word_len = A_word_len or B_word_len
// D can alias A B
size_t cc_bn_gcd(cc_bn_t *D, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len)
{
    if (A_word_len >= B_word_len)
    {
        return cc_bn_gcd_unsafe(D, A, A_word_len, B, B_word_len);
    }
    else
    {
        return cc_bn_gcd_unsafe(D, B, B_word_len, A, A_word_len);
    }
}

