
#include "cc_ec_fp.h"
#include "cc_bn_mont.h"
#include "cc_bn_mod.h"

void cc_ec_fp_curve_init(enum ec_curve_id_t curve_id, struct cc_ec_fp_curve_t *curve_fp)
{
    struct ec_curve_t *curve_params = ec_curve_get(curve_id);

    curve_fp->curve_params = curve_params;
    curve_fp->bn_words = cc_bn_word_len_from_bit_len(curve_params->bits);

    cc_bn_mont_RR(curve_fp->P_RR, curve_params->P, curve_fp->bn_words);
    curve_fp->P_Ni = cc_bn_mont_Ni(curve_params->P);

    cc_bn_mont_RR(curve_fp->N_RR, curve_params->N, curve_fp->bn_words);
    curve_fp->N_Ni = cc_bn_mont_Ni(curve_params->N);
}

cc_status_t cc_ec_fp_point_on_curve(const struct cc_ec_fp_curve_t *curve, const cc_bn_word_t *X, const cc_bn_word_t *Y)
{

    cc_bn_word_t Y2[CC_EC_MAX_WORDS];
    cc_bn_word_t rhs[CC_EC_MAX_WORDS];

    // Y2 = Y^2
    CC_CHK(cc_bn_core_mod_square_mont(Y2, Y, curve->curve_params->P, curve->bn_words, curve->P_RR, curve->P_Ni));

    // rhs = (X^2 + a)*X + b
    CC_CHK(cc_bn_core_mod_square_mont(rhs, X, curve->curve_params->P, curve->bn_words, curve->P_RR, curve->P_Ni));
    cc_bn_mod_add(rhs, rhs, curve->curve_params->A, curve->curve_params->P, curve->bn_words);
    CC_CHK(cc_bn_core_mod_mul_mont(rhs, rhs, X, curve->curve_params->P, curve->bn_words, curve->P_RR, curve->P_Ni));
    cc_bn_mod_add(rhs, rhs, curve->curve_params->B, curve->curve_params->P, curve->bn_words);

    if (cc_bn_cmp_words(Y2, rhs, curve->bn_words) != 0)
    {
        return CC_ERR_EC_INVALID_POINT;
    }
    return CC_SUCCESS;
}

cc_status_t cc_ec_fp_check_point(const struct cc_ec_fp_curve_t *curve, const cc_bn_word_t *X, const cc_bn_word_t *Y)
{
    size_t word_len = curve->bn_words;
    if (cc_bn_is_zero(X, word_len) || cc_bn_cmp_words(X, curve->curve_params->P, word_len) >= 0)
    {
        return CC_ERR_EC_INVALID_POINT;
    }
    if (cc_bn_is_zero(Y, word_len) || cc_bn_cmp_words(Y, curve->curve_params->P, word_len) >= 0)
    {
        return CC_ERR_EC_INVALID_POINT;
    }
    cc_status_t on_curve = cc_ec_fp_point_on_curve(curve, X, Y);
    if (on_curve != CC_SUCCESS)
    {
        return CC_ERR_EC_INVALID_POINT;
    }
    if (curve->curve_params->h != 1)
    {
        // TODO:n*P=0
    }
    return CC_SUCCESS;
}