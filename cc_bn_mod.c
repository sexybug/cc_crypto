#include "cc_bn_mod.h"
#include "cc_bn_mul.h"
#include "cc_bn_config.h"

// R = A mod d, d != 0
// Vertical division
cc_bn_word_t cc_bn_mod_word(const cc_bn_word_t *A, size_t A_word_len, cc_bn_word_t d)
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
int cc_bn_cmp_carry(cc_bn_word_t carry, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len)
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
void cc_bn_mod_add_word(cc_bn_word_t *R, const cc_bn_word_t *A, cc_bn_word_t d, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_word_t carry = cc_bn_add_word(R, A, bn_word_len, d);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, R, N, bn_word_len);
    }
}

// R = A + B mod N. A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_add(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_word_t carry = cc_bn_add_words(R, A, B, bn_word_len);

    if (((carry == 0) && cc_bn_cmp_words(R, N, bn_word_len) >= 0) || (carry == 1))
    {
        cc_bn_sub_words(R, R, N, bn_word_len);
    }
}

// R = 2A mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_double(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_mod_add(R, A, A, N, bn_word_len);
}

// R = A - d mod N, A < N
// R can alias A, R can not alias N
void cc_bn_mod_sub_word(cc_bn_word_t *R, const cc_bn_word_t *A, cc_bn_word_t d, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_word_t borrow = cc_bn_sub_word(R, A, bn_word_len, d);

    if (borrow == 1)
    {
        cc_bn_add_words(R, R, N, bn_word_len);
    }
}

// R = A - B mod N, A < N, B < N
// R can alias A B, R can not alias N
void cc_bn_mod_sub(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_word_t borrow = cc_bn_sub_words(R, A, B, bn_word_len);

    if (borrow == 1)
    {
        cc_bn_add_words(R, R, N, bn_word_len);
    }
}

// R = -A mod N, A < N
// R can alias A N
void cc_bn_mod_neg(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_sub_words(R, N, A, bn_word_len);
}

// R = A / 2 mod N, A < N
// N must be odd
// R can alias A N
void cc_bn_mod_half(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *N, size_t bn_word_len)
{
    // if A is odd, R = (A + N) / 2
    if (CC_BN_IS_ODD(A))
    {
        cc_bn_word_t carry = cc_bn_add_words(R, A, N, bn_word_len);

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
cc_status_t cc_bn_core_div(cc_bn_word_t *Q, cc_bn_word_t *R, cc_bn_word_t *A, size_t A_word_len, cc_bn_word_t *N, size_t N_word_len)
{
    // if N == 0, return ERROR
    if (cc_bn_is_zero(N, N_word_len))
    {
        return CC_ERR_BN_DIV_BY_ZERO;
    }

    // if A < N, Q = 0, R = A
    if (cc_bn_cmp(A, A_word_len, N, N_word_len) < 0)
    {
        cc_bn_copy(R, A, N_word_len);
        cc_bn_set_zero(Q, A_word_len);
        return CC_OK;
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
    return CC_OK;
}

// R = A mod N, R length = N length
// R can alias A N
cc_status_t cc_bn_mod(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *N, size_t N_word_len)
{
    if (A_word_len > CC_BN_MAX_WORDS * 2 || N_word_len > CC_BN_MAX_WORDS)
    {
        return CC_ERR_BN_LEN_TOO_LONG;
    }

    cc_bn_word_t A_tmp[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t N_tmp[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t Q_tmp[CC_BN_MAX_WORDS * 2];

    cc_bn_copy(A_tmp, A, A_word_len);
    cc_bn_copy(N_tmp, N, N_word_len);
    CC_CHK(cc_bn_core_div(Q_tmp, R, A_tmp, A_word_len, N_tmp, N_word_len));
    return CC_OK;
}

// R = A * B mod N
// R_word_len = bn_word_len
// R can alias A B N
void cc_bn_mod_mul_words(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *N, size_t bn_word_len)
{
    cc_bn_word_t P[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t Q[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t N_tmp[CC_BN_MAX_WORDS * 2];

    cc_bn_copy(N_tmp, N, bn_word_len);

    // P = A * B
    cc_bn_core_mul_words(P, A, B, bn_word_len);

    // Q = P / N, R = P % N
    cc_bn_core_div(Q, R, P, bn_word_len * 2, N_tmp, bn_word_len);
}

// R = A * B mod N
// R_word_len = N_word_len
// R can alias A B N
void cc_bn_mod_mul(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len, const cc_bn_word_t *N, size_t N_word_len)
{
    cc_bn_word_t P[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t Q[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t N_tmp[CC_BN_MAX_WORDS * 2];

    cc_bn_copy(N_tmp, N, N_word_len);

    // P = A * B
    cc_bn_core_mul(P, A, A_word_len, B, B_word_len);

    // Q = P / N, R = P % N
    cc_bn_core_div(Q, R, P, A_word_len + B_word_len, N_tmp, N_word_len);
}

// R = A^E mod N
// R can alias A, cannot alias E N
void cc_bn_core_mod_exp(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *E, size_t E_word_len, const cc_bn_word_t *N, size_t N_word_len)
{
    int i;
    cc_bn_word_t A_tmp[CC_BN_MAX_WORDS];
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
void cc_bn_mod_exp(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *E, size_t E_word_len, const cc_bn_word_t *N, size_t N_word_len)
{
    cc_bn_word_t T[CC_BN_MAX_WORDS];
    cc_bn_core_mod_exp(T, A, A_word_len, E, E_word_len, N, N_word_len);
    cc_bn_copy(R, T, N_word_len);
}

// D = gcd(A, B), refer to mbedtls
// A_word_len must >= B_word_len
// return D_word_len, D_word_len = min(A_word_len, B_word_len) if A,B != 0
// D can alias A B
size_t cc_bn_gcd_unsafe(cc_bn_word_t *D, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len)
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

    cc_bn_word_t AT[CC_BN_MAX_WORDS];
    cc_bn_word_t BT[CC_BN_MAX_WORDS];

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
// return D_word_len, D_word_len = min(A_word_len, B_word_len) if A,B != 0
// D can alias A B
size_t cc_bn_gcd(cc_bn_word_t *D, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *B, size_t B_word_len)
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

// assume: a > b and a,b not all even
// g = x*a - y*b
// X Y word_len at least bn_word_len + 1
// G X Y cannot alias A B
cc_status_t cc_bn_core_binary_exgcd_unsafe(cc_bn_word_t *G, cc_bn_word_t *X, cc_bn_word_t *Y, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len)
{
    // if A <= B, return error
    if (cc_bn_cmp_words(A, B, bn_word_len) <= 0)
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    // if A is even and B is even, return error
    if (CC_BN_IS_EVEN(A) && CC_BN_IS_EVEN(B))
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    cc_bn_word_t TA[CC_BN_MAX_WORDS + 1];
    cc_bn_word_t TB[CC_BN_MAX_WORDS + 1];
    cc_bn_word_t TG[CC_BN_MAX_WORDS];

    // X=1
    cc_bn_set_one(X, bn_word_len + 1);
    // Y=0
    cc_bn_set_zero(Y, bn_word_len + 1);
    // G=A
    cc_bn_copy(G, A, bn_word_len);
    // TB=B
    cc_bn_from_words(TB, bn_word_len + 1, B, bn_word_len);
    // TA=A-1
    cc_bn_from_words(TA, bn_word_len + 1, A, bn_word_len);
    cc_bn_sub_word(TA, TA, bn_word_len + 1, 1);
    // TG=B
    cc_bn_copy(TG, B, bn_word_len);

    do
    {
        do
        {
            if (CC_BN_IS_EVEN(G))
            {
                if (CC_BN_IS_ODD(X) || CC_BN_IS_ODD(Y))
                {
                    // X = X + B
                    cc_bn_add_small(X, X, bn_word_len + 1, B, bn_word_len);
                    // Y = Y + A
                    cc_bn_add_small(Y, Y, bn_word_len + 1, A, bn_word_len);
                }
                // X = X >> 1
                cc_bn_rshift_1(X, X, bn_word_len + 1);
                // Y = Y >> 1
                cc_bn_rshift_1(Y, Y, bn_word_len + 1);
                // G = G >> 1
                cc_bn_rshift_1(G, G, bn_word_len);
            }
            if (CC_BN_IS_EVEN(TG) || cc_bn_cmp_words(G, TG, bn_word_len) < 0)
            {
                cc_bn_swap(X, TB, bn_word_len + 1);
                cc_bn_swap(Y, TA, bn_word_len + 1);
                cc_bn_swap(G, TG, bn_word_len);
            }
        } while (CC_BN_IS_EVEN(G));

        while (cc_bn_cmp_words(X, TB, bn_word_len + 1) < 0 || cc_bn_cmp_words(Y, TA, bn_word_len + 1) < 0)
        {
            // X = X + B
            cc_bn_add_small(X, X, bn_word_len + 1, B, bn_word_len);
            // Y = Y + A
            cc_bn_add_small(Y, Y, bn_word_len + 1, A, bn_word_len);
        }

        // X = X - TB
        cc_bn_sub_words(X, X, TB, bn_word_len + 1);
        // Y = Y - TA
        cc_bn_sub_words(Y, Y, TA, bn_word_len + 1);
        // G = G - TG
        cc_bn_sub_words(G, G, TG, bn_word_len);

    } while (!cc_bn_is_zero(TG, bn_word_len));

    while (cc_bn_cmp(X, bn_word_len + 1, B, bn_word_len) >= 0 && cc_bn_cmp(Y, bn_word_len + 1, A, bn_word_len) >= 0)
    {
        // X = X - B
        cc_bn_sub_small(X, X, bn_word_len + 1, B, bn_word_len);
        // Y = Y - A
        cc_bn_sub_small(Y, Y, bn_word_len + 1, A, bn_word_len);
    }

    return CC_OK;
}

// assume: a > b
// a b can all even
// g = x*a - y*b
// X Y word_len at least bn_word_len + 1
// G X Y cannot alias A B
cc_status_t cc_bn_binary_exgcd_unsafe(cc_bn_word_t *G, cc_bn_word_t *X, cc_bn_word_t *Y, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len)
{
    cc_bn_word_t TA[CC_BN_MAX_WORDS];
    cc_bn_word_t TB[CC_BN_MAX_WORDS];

    size_t A_lsb = cc_bn_lsb(A, bn_word_len);
    size_t B_lsb = cc_bn_lsb(B, bn_word_len);

    size_t k = A_lsb < B_lsb ? A_lsb : B_lsb;

    // TA = A>>k
    cc_bn_rshift(TA, A, bn_word_len, k);
    // TB = B>>k
    cc_bn_rshift(TB, B, bn_word_len, k);

    CC_CHK(cc_bn_core_binary_exgcd_unsafe(G, X, Y, TA, TB, bn_word_len));

    cc_bn_lshift(X, X, bn_word_len, k);
    cc_bn_lshift(Y, Y, bn_word_len, k);
    cc_bn_lshift(G, G, bn_word_len, k);

    return CC_OK;
}

// R = A^-1 mod N, R_word_len = N_word_len
// R can alias A N
cc_status_t cc_bn_exgcd_mod_inv(cc_bn_word_t *R, const cc_bn_word_t *A, size_t A_word_len, const cc_bn_word_t *N, size_t N_word_len)
{
    cc_bn_word_t G[CC_BN_MAX_WORDS];
    cc_bn_word_t X[CC_BN_MAX_WORDS + 1];
    cc_bn_word_t Y[CC_BN_MAX_WORDS + 1];
    cc_bn_word_t T[CC_BN_MAX_WORDS];

    // if N<=1 or A=0, return error
    if (cc_bn_cmp_word(N, N_word_len, 1) <= 0 || cc_bn_cmp_word(A, A_word_len, 0) == 0)
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    // if A>=N, T = A mod N
    if (cc_bn_cmp(A, A_word_len, N, N_word_len) >= 0)
    {
        CC_CHK(cc_bn_mod(T, A, A_word_len, N, N_word_len));
    }
    else
    {
        // if A < N, T = A
        cc_bn_from_words(T, N_word_len, A, A_word_len);
    }

    CC_CHK(cc_bn_binary_exgcd_unsafe(G, X, Y, N, T, N_word_len));

    // G = gcd(A, N)
    // if G != 1, A and N are not coprime, return no inverse
    if (!cc_bn_is_one(G, N_word_len))
    {
        return CC_ERR_BN_NO_INVERSE;
    }

    // now Y = -(A^-1) mod N, -Y is (A^-1) mod N
    cc_bn_mod_neg(R, Y, N, N_word_len);

    return CC_OK;
}