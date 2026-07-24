
#ifndef CC_EC_CURVES_H
#define CC_EC_CURVES_H

#include "cc_bn.h"

struct ec_curve_t
{
    size_t bits;
    cc_bn_word_t *P;
    cc_bn_word_t *A;
    cc_bn_word_t *B;
    cc_bn_word_t *N;
    cc_bn_word_t *X;
    cc_bn_word_t *Y;
    cc_bn_word_t h;
};

enum ec_curve_id_t
{
    EC_CURVE_SM2 = 0,
    EC_CURVE_SECP192R1,
    EC_CURVE_SECP224R1,
    EC_CURVE_SECP256R1,
    EC_CURVE_SECP384R1,
    EC_CURVE_SECP521R1
};

struct ec_curve_t *ec_curve_get(enum ec_curve_id_t id);

#endif // CC_EC_CURVES_H