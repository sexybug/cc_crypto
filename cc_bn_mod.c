#include "cc_bn_mod.h"

#define CC_BN_MAX_WORD_LEN 10

int cc_bn_div(const cc_bn_digit_t *dividend, size_t dividend_word_len,
                  const cc_bn_digit_t *divisor, size_t divisor_word_len,
                  cc_bn_digit_t *quotient, size_t *quotient_word_len,
                  cc_bn_digit_t *remainder, size_t *remainder_word_len) {
    // Initialize quotient and remainder
    cc_bn_set_zero(quotient, CC_BN_MAX_WORD_LEN);
    cc_bn_set_zero(remainder, CC_BN_MAX_WORD_LEN);
    
    // Check if divisor is zero
    if (cc_bn_is_zero(divisor, divisor_word_len)) {
        // Handle division by zero case
        return CC_ERR_DIV_BY_ZERO; // or set an error code
    }
    // Perform division
    // Simple long division algorithm (schoolbook method)
    size_t n = dividend_word_len;
    size_t m = divisor_word_len;
    if (n < m) {
        // Dividend < Divisor, quotient = 0, remainder = dividend
        cc_bn_copy(remainder, dividend, n);
        *remainder_word_len = n;
        *quotient_word_len = 1;
        quotient[0] = 0;
        return 0;
    }

    cc_bn_digit_t temp_dividend[CC_BN_MAX_WORD_LEN] = {0};
    cc_bn_copy(temp_dividend, dividend, n);

    size_t q_len = n - m + 1;
    cc_bn_set_zero(quotient, q_len);

    for (size_t i = n - m; i >= 0; --i) {
        // Estimate quotient digit
        cc_bn_digit_t q_digit = 0;
        cc_bn_digit_t high = (cc_bn_digit_t)-1;
        cc_bn_digit_t low = 0;
        while (low <= high) {
            cc_bn_digit_t mid = low + ((high - low) >> 1);
            // Multiply divisor by mid and shift left by i
            cc_bn_digit_t prod[CC_BN_MAX_WORD_LEN] = {0};
            cc_bn_mul_digit_shift(prod, divisor, m, mid, i);
            if (cc_bn_cmp_shift(temp_dividend, prod, n, m + i) >= 0) {
                q_digit = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        // Subtract (divisor * q_digit << i) from temp_dividend
        cc_bn_digit_t prod[CC_BN_MAX_WORD_LEN] = {0};
        cc_bn_mul_digit_shift(prod, divisor, m, q_digit, i);
        cc_bn_sub(temp_dividend, temp_dividend, prod, n);
        quotient[i] = q_digit;
    }

    // Set quotient and remainder lengths
    *quotient_word_len = q_len;
    // Remove leading zeros in quotient
    while (*quotient_word_len > 1 && quotient[*quotient_word_len - 1] == 0)
        (*quotient_word_len)--;

    // Copy remainder
    cc_bn_copy(remainder, temp_dividend, m);
    *remainder_word_len = m;
    // Remove leading zeros in remainder
    while (*remainder_word_len > 1 && remainder[*remainder_word_len - 1] == 0)
        (*remainder_word_len)--;

    return 0;
}