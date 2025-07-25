#include "cc_bn_mod.h"
#include "cc_bn_mul.h"
#include "cc_bn_config.h"

// R = A mod d, d != 0
// Vertical division
cc_bn_t cc_bn_mod_word(const cc_bn_t *A, size_t A_word_len, cc_bn_t d)
{
    size_t i;

    if (d == 1 || cc_bn_is_zero(A, A_word_len))
    {
        return 0;
    }

    if (d == 2)
    {
        return A[0] & 1;
    }

    cc_bn_dword_t remainder = A[A_word_len - 1] % d;
    for (i = A_word_len - 1; i > 0; i -= 1)
    {
        remainder = (remainder << CC_BN_WORD_BITS) | (cc_bn_dword_t)A[i - 1];
        remainder = remainder % (cc_bn_dword_t)d;
    }
    return remainder;
}

// compare {carry, A} with B
int cc_bn_cmp_carry(cc_bn_t carry, const cc_bn_t *A, const cc_bn_t *B, size_t bn_word_len)
{
    if (carry == 0)
    {
        return cc_bn_cmp_words(A, B, bn_word_len);
    }
    else
    {
        return 1;
    }
}

// R = A + d mod N. A < N, d < N
// R can alias A, R can not alias N
void cc_bn_mod_add_word(cc_bn_t *R, const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t carry = cc_bn_add_word(R, A, bn_word_len, d);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, R, N, bn_word_len);
    }
}

// R = A + B mod N. A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_add(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t carry = cc_bn_add_words(R, A, B, bn_word_len);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, R, N, bn_word_len);
    }
}

// R = 2A mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_double(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_mod_add(R, A, A, N, bn_word_len);
}

// R = A - d mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_sub_word(cc_bn_t *R, const cc_bn_t *A, cc_bn_t d, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t borrow = cc_bn_sub_word(R, A, bn_word_len, d);

    if (borrow == 1)
    {
        cc_bn_add_words(R, R, N, bn_word_len);
    }
}

// R = A - B mod N, A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_sub(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t borrow = cc_bn_sub_words(R, A, B, bn_word_len);

    if (borrow == 1)
    {
        cc_bn_add_words(R, R, N, bn_word_len);
    }
}

// R = -A mod N, A < N
// R can alias A N
void cc_bn_mod_neg(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_sub_words(R, N, A, bn_word_len);
}

// R = A / 2 mod N, A < N
// N must be odd
// R can alias A N
void cc_bn_mod_half(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *N, size_t bn_word_len)
{
    // if A is odd, R = (A + N) / 2
    if (A[0] & 1)
    {
        cc_bn_t carry = cc_bn_add_words(R, A, N, bn_word_len);

        size_t i;
        for (i = 0; i < bn_word_len - 1; i += 1)
        {
            R[i] = (R[i + 1] << (CC_BN_WORD_BITS - 1)) | (R[i] >> 1);
        }
        R[bn_word_len - 1] = (carry << (CC_BN_WORD_BITS - 1)) | (R[bn_word_len - 1] >> 1);
    }
    else
    {
        // R = A / 2
        size_t i;
        for (i = 0; i < bn_word_len - 1; i += 1)
        {
            R[i] = (R[i + 1] << (CC_BN_WORD_BITS - 1)) | (R[i] >> 1);
        }
        R[bn_word_len - 1] = (R[bn_word_len - 1] >> 1);
    }
}

// Q = A / N, R = A % N
// A, N are modified in the process
// N array must be at least A_word_len
// Q_word_len = A_word_len, R_word_len = N_word_len
// Q cannot alias A N
// R can alias A N
cc_bn_status_t cc_bn_core_div(cc_bn_t *Q, cc_bn_t *R, cc_bn_t *A, size_t A_word_len, cc_bn_t *N, size_t N_word_len)
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
        return CC_BN_OK;
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
    return CC_BN_OK;
}

// R = A mod N, R length = N length
// R can alias A N
cc_bn_status_t cc_bn_mod(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *N, size_t N_word_len)
{
    if (A_word_len > CC_BN_MAX_WORDS || N_word_len > CC_BN_MAX_WORDS)
    {
        return CC_BN_ERR_LEN_TOO_LONG;
    }

    cc_bn_t A_tmp[CC_BN_MAX_WORDS];
    cc_bn_t N_tmp[CC_BN_MAX_WORDS];
    cc_bn_t Q_tmp[CC_BN_MAX_WORDS];

    cc_bn_copy(A_tmp, A, A_word_len);
    cc_bn_copy(N_tmp, N, N_word_len);
    cc_bn_status_t div_status = cc_bn_core_div(Q_tmp, R, A_tmp, A_word_len, N_tmp, N_word_len);
    if (CC_BN_ERR(div_status))
    {
        return div_status;
    }
    return CC_BN_OK;
}

// R = A * B mod N
// R_word_len = bn_word_len
// R can alias A B N
void cc_bn_mod_mul_words(cc_bn_t *R, const cc_bn_t *A, const cc_bn_t *B, const cc_bn_t *N, size_t bn_word_len)
{
    cc_bn_t P[CC_BN_MAX_WORDS * 2];
    cc_bn_t Q[CC_BN_MAX_WORDS * 2];
    cc_bn_t N_tmp[CC_BN_MAX_WORDS * 2];

    cc_bn_copy(N_tmp, N, bn_word_len);

    // P = A * B
    cc_bn_core_mul_words(P, A, B, bn_word_len);

    // Q = P / N, R = P % N
    cc_bn_core_div(Q, R, P, bn_word_len * 2, N_tmp, bn_word_len);
}

// R = A * B mod N
// R_word_len = N_word_len
// R can alias A B N
void cc_bn_mod_mul(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *B, size_t B_word_len, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t P[CC_BN_MAX_WORDS * 2];
    cc_bn_t Q[CC_BN_MAX_WORDS * 2];
    cc_bn_t N_tmp[CC_BN_MAX_WORDS * 2];

    cc_bn_copy(N_tmp, N, N_word_len);

    // P = A * B
    cc_bn_core_mul(P, A, A_word_len, B, B_word_len);

    // Q = P / N, R = P % N
    cc_bn_core_div(Q, R, P, A_word_len + B_word_len, N_tmp, N_word_len);
}

// R = A^E mod N
// R can alias A, cannot alias E N
void cc_bn_core_mod_exp(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *E, size_t E_word_len, const cc_bn_t *N, size_t N_word_len)
{
    int i;
    cc_bn_t A_tmp[CC_BN_MAX_WORDS];
    size_t E_bit_len = cc_bn_bit_len(E, E_word_len);

    // if E=0, R=1
    if (E_bit_len == 0)
    {
        cc_bn_set_one(R, N_word_len);
        return;
    }
    // if A >= N, A' = A mod N
    if (cc_bn_cmp(A, A_word_len, N, N_word_len) >= 0)
    {
        cc_bn_mod(A_tmp, A, A_word_len, N, N_word_len);
    }
    else
    {
        // if A < N, A' = A
        cc_bn_copy(A_tmp, A, N_word_len);
    }
    cc_bn_copy(R, A_tmp, N_word_len);

    for (i = E_bit_len - 2; i >= 0; i -= 1)
    {
        // T = T^2 mod N
        cc_bn_mod_mul_words(R, R, R, N, N_word_len);
        if (cc_bn_get_bit(E, i))
        {
            // T = T * A mod N
            cc_bn_mod_mul_words(R, R, A_tmp, N, N_word_len);
        }
    }
}

// R = A^E mod N
// R can alias A E N
void cc_bn_mod_exp(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *E, size_t E_word_len, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_core_mod_exp(T, A, A_word_len, E, E_word_len, N, N_word_len);
    cc_bn_copy(R, T, N_word_len);
}

// R = A^-1 mod N, R_word_len = N_word_len
// R can alias A N
cc_bn_status_t cc_bn_mod_inv(cc_bn_t *R, const cc_bn_t *A, size_t A_word_len, const cc_bn_t *N, size_t N_word_len)
{
    cc_bn_t U[CC_BN_MAX_WORDS];
    cc_bn_t V[CC_BN_MAX_WORDS];
    cc_bn_t X1[CC_BN_MAX_WORDS];
    cc_bn_t X2[CC_BN_MAX_WORDS];

    // if N<=1 or A=0, return error
    if (cc_bn_cmp_word(N, N_word_len, 1) <= 0 || cc_bn_cmp_word(A, A_word_len, 0) == 0)
    {
        return CC_BN_ERR_INVALID_ARG;
    }

    // if A>=N, U = A mod N
    if (cc_bn_cmp(A, A_word_len, N, N_word_len) >= 0)
    {
        CC_BN_CHK(cc_bn_mod(U, A, A_word_len, N, N_word_len));
    }
    else
    {
        // if A < N, U = A
        cc_bn_copy(U, A, N_word_len);
    }
    // V = N
    cc_bn_copy(V, N, N_word_len);
    // X1 = 1
    cc_bn_set_one(X1, N_word_len);
    // X2 = 0
    cc_bn_set_zero(X2, N_word_len);

    while (!cc_bn_is_zero(U, N_word_len))
    {
        // while U is even
        while ((U[0] & 1) == 0)
        {
            // U = U >> 1
            cc_bn_rshift_1(U, U, N_word_len);
            // X1 = X1 / 2 mod N
            cc_bn_mod_half(X1, X1, N, N_word_len);
        }

        // while V is even
        while ((V[0] & 1) == 0)
        {
            // V = V >> 1
            cc_bn_rshift_1(V, V, N_word_len);
            // X2 = X2 / 2 mod N
            cc_bn_mod_half(X2, X2, N, N_word_len);
        }

        // if U >= V
        if (cc_bn_cmp_words(U, V, N_word_len) >= 0)
        {
            // U = U - V
            cc_bn_sub_words(U, U, V, N_word_len);
            // X1 = X1 - X2 mod N
            cc_bn_mod_sub(X1, X1, X2, N, N_word_len);
        }
        else
        {
            // V = V - U
            cc_bn_sub_words(V, V, U, N_word_len);
            // X2 = X2 - X1 mod N
            cc_bn_mod_sub(X2, X2, X1, N, N_word_len);
        }
    }

    // now V = gcd(A, N)
    // if V != 1, A and N are not coprime
    if (!cc_bn_is_one(V, N_word_len))
    {
        return CC_BN_ERR_NO_INVERSE;
    }

    // R = X2
    cc_bn_copy(R, X2, N_word_len);

    return CC_BN_OK;
}