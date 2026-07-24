
#ifndef CC_EC_FP_H
#define CC_EC_FP_H

#include "cc_bn.h"
#include "cc_ec_curves.h"
#include "cc_bn_config.h"
#include "cc_err.h"

struct cc_ec_fp_curve_t
{
    struct ec_curve_t *curve_params;
    size_t bn_words;
    cc_bn_word_t P_RR[CC_EC_MAX_WORDS]; // montogemory R^2 mod P
    cc_bn_word_t P_Ni;                  // montogemory 1/P mod R
    cc_bn_word_t N_RR[CC_EC_MAX_WORDS]; // montogemory R^2 mod N
    cc_bn_word_t N_Ni;                  // montogemory 1/N mod R
};

void cc_ec_fp_curve_init(enum ec_curve_id_t curve_id, struct cc_ec_fp_curve_t *curve_fp);
cc_status_t cc_ec_fp_point_on_curve(const struct cc_ec_fp_curve_t *curve, const cc_bn_word_t *X, const cc_bn_word_t *Y);

#endif // CC_EC_FP_H