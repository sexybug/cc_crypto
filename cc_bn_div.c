
#include "cc_bn_div.h"

// Q = A / N, R = A % N
// A, N are modified in the process, N array must be at least A_word_len
// Q_word_len = A_word_len, R_word_len = N_word_len
cc_bn_status_t cc_bn_div_unsafe(cc_bn_t *Q, cc_bn_t *R, cc_bn_t *A, size_t A_word_len, cc_bn_t *N, size_t N_word_len)
{
    // if N == 0, return ERROR
    if (cc_bn_is_zero(N, N_word_len))
    {
        return CC_BN_ERR_DIV_BY_ZERO;
    }

    // if A < N, Q = 0, R = A
    if (cc_bn_cmp(A, A_word_len, N, N_word_len) < 0)
    {
        cc_bn_copy(R, A, N_word_len);
        cc_bn_set_zero(Q, A_word_len);
        return CC_BN_SUCCESS;
    }

    // A >= N, A_real_word_len >= N_real_word_len
    size_t A_real_word_len = cc_bn_word_len(A, A_word_len);
    size_t N_real_word_len = cc_bn_word_len(N, N_word_len);

    size_t i;
    size_t A_bit_len = cc_bn_bit_len(A, A_real_word_len);
    size_t N_bit_len = cc_bn_bit_len(N, N_real_word_len);

    for (i = N_real_word_len; i < A_real_word_len; i++)
    {
        N[i] = 0;
    }
    cc_bn_set_zero(Q, A_word_len); // Q = 0

    cc_bn_lshift(N, N, A_real_word_len, A_bit_len - N_bit_len); // N = N << (A_bit_len - N_bit_len)

    for (i = 0; i <= A_bit_len - N_bit_len; i++)
    {
        cc_bn_lshift_1(Q, Q, A_real_word_len - N_real_word_len + 1); // Q = Q << 1
        // if (A >= N)
        if (cc_bn_cmp(A, A_real_word_len, N, A_real_word_len) >= 0)
        {
            Q[0] |= 1;                                 // Q = Q | 1
            cc_bn_sub_words(A, A, N, A_real_word_len); // A = A - N
        }
        cc_bn_rshift_1(N, N, A_real_word_len); // N = N >> 1
    }
    // R = A
    cc_bn_from_words(R, N_word_len, A, N_real_word_len);
    return CC_BN_SUCCESS;
}
