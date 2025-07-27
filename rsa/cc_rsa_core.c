
#include "cc_rsa_core.h"
#include "cc_bn_prime.h"
#include "cc_bn_mod.h"
#include "cc_bn_mul.h"
#include "cc_bn_mont.h"
#include "cc_bn_gcd.h"
#include "cc_crypto_rng.h"

// at most try 1000 times to generate key
#define COUNT_MAX 1000

// E>3, bits(E)<=bits, E is odd
cc_status_t cc_rsa_core_gen_key(cc_rsa_privkey_st *privkey,
                                size_t bits, const cc_bn_t *E, size_t E_word_len, bool crt,
                                cc_crypto_rng_f rng)
{
    int count = 0;
    cc_bn_t H[CC_BN_MAX_WORDS];
    cc_bn_t G[CC_BN_MAX_WORDS];
    cc_bn_t *L = privkey->N;
    size_t N_words = (bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;

    if ((bits % 2 != 0) || (bits < 8) || (bits > CC_BN_MAX_BITS))
    {
        return CC_ERR_BN_INVALID_ARG;
    }
    // E>3, bits(E)<=bits, E is odd
    if ((cc_bn_cmp_word(E, E_word_len, 3) < 0) || (cc_bn_bit_len(E, E_word_len) > bits) || CC_BN_IS_EVEN(E))
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    size_t PQ_bits = bits >> 1;
    // note: N_words <= PQ_words*2
    size_t PQ_words = (PQ_bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
    cc_bn_from_words(privkey->E, N_words, E, E_word_len);

    /*
     * find primes P and Q with P > Q so that:
     * 1.  |P-Q| > 2^( nbits / 2 - 100 )
     * 2.  GCD( E, (P-1)*(Q-1) ) == 1
     * 3.  E^-1 mod LCM(P-1, Q-1) > 2^( nbits / 2 )
     */
    do
    {
        count++;
        if (count > COUNT_MAX)
        {
            return CC_ERR_RSA_GEN_KEY;
        }

        CC_CHK(cc_bn_gen_prime(privkey->P, PQ_bits, rng));
        CC_CHK(cc_bn_gen_prime(privkey->Q, PQ_bits, rng));

        // privkey->P[0] = 0xA37C734D;
        // privkey->Q[0] = 0x86BDCB8D;

        // make P > Q
        if (cc_bn_cmp_words(privkey->P, privkey->Q, PQ_words) < 0)
        {
            cc_bn_swap(privkey->P, privkey->Q, PQ_words);
        }

        /* make sure the difference between p and q is not too small (FIPS 186-4 §B.3.3 step 5.4) */
        cc_bn_sub_words(H, privkey->P, privkey->Q, PQ_words);
        if (cc_bn_bit_len(H, PQ_words) <= ((bits >= 200) ? (PQ_bits - 99) : 0))
        {
            continue;
        }

        /* Temporarily replace P,Q by P-1, Q-1 */
        cc_bn_sub_word(privkey->P, privkey->P, PQ_words, 1);
        cc_bn_sub_word(privkey->Q, privkey->Q, PQ_words, 1);

        // H = (P-1)*(Q-1)
        cc_bn_core_mul_words(H, privkey->P, privkey->Q, PQ_words);

        /* check GCD(E, (P-1)*(Q-1)) == 1 (FIPS 186-4 §B.3.1 criterion 2(a)) */
        cc_bn_gcd(G, privkey->E, N_words, H, N_words);
        if (!cc_bn_is_one(G, N_words))
        {
            continue;
        }

        /* compute smallest possible D = E^-1 mod LCM(P-1, Q-1) (FIPS 186-4 §B.3.1 criterion 3(b)) */
        // G = GCD(P-1, Q-1)
        cc_bn_gcd(G, privkey->P, PQ_words, privkey->Q, PQ_words);
        // L = LCM(P-1, Q-1) = (P-1)*(Q-1) / GCD(P-1, Q-1) = H/G
        CC_CHK(cc_bn_core_div(L, H, H, N_words, G, PQ_words));
        // D = E^-1 mod L
        CC_CHK(cc_bn_exgcd_mod_inv(privkey->D, privkey->E, N_words, L, N_words));

        if (cc_bn_bit_len(privkey->D, N_words) <= PQ_bits)
        {
            continue;
        }

        if (crt)
        {
            // DP = D mod (P-1)
            CC_CHK(cc_bn_mod(privkey->DP, privkey->D, N_words, privkey->P, PQ_words));
            // DQ = D mod (Q-1)
            CC_CHK(cc_bn_mod(privkey->DQ, privkey->D, N_words, privkey->Q, PQ_words));
        }
        break;
    } while (1);

    /* Restore P,Q */
    cc_bn_add_word(privkey->P, privkey->P, PQ_words, 1);
    cc_bn_add_word(privkey->Q, privkey->Q, PQ_words, 1);

    if (crt)
    {
        // QP = Q^-1 mod P
        CC_CHK(cc_bn_exgcd_mod_inv(privkey->QP, privkey->Q, PQ_words, privkey->P, PQ_words));
    }

    /* compute N = P*Q */
    cc_bn_core_mul_words(privkey->N, privkey->P, privkey->Q, PQ_words);
    privkey->bits = bits;

    return CC_OK;
}

// if validate pass, return CC_OK
// if validate fail, return CC_ERR_RSA_VALIDATE_KEY
cc_status_t cc_rsa_validate_params(size_t bits, const cc_bn_t *N, const cc_bn_t *E, const cc_bn_t *D, const cc_bn_t *P, const cc_bn_t *Q, cc_crypto_rng_f rng)
{
    cc_bn_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_t TP[CC_BN_MAX_WORDS / 2];

    size_t N_words = (bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
    size_t PQ_words = ((bits >> 1) + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;

    // check P is prime
    if (P != NULL)
    {
        cc_status_t P_check = cc_bn_prime_check(P, PQ_words, rng);
        if (P_check != CC_BN_IS_PRIME && P_check != CC_BN_PROBABLY_PRIME)
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    // check Q is prime
    if (Q != NULL)
    {
        cc_status_t Q_check = cc_bn_prime_check(Q, PQ_words, rng);
        if (Q_check != CC_BN_IS_PRIME && Q_check != CC_BN_PROBABLY_PRIME)
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    // check 1 < N = P * Q
    if (P != NULL && Q != NULL && N != NULL)
    {
        if (cc_bn_cmp_word(N, N_words, 1) <= 0)
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
        cc_bn_mul_words(T, P, Q, PQ_words);
        if (cc_bn_cmp_words(N, T, N_words) != 0)
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    if (N != NULL && D != NULL && E != NULL)
    {
        // check 1 < D, E < N
        if (cc_bn_cmp_word(D, N_words, 1) <= 0 || cc_bn_cmp_words(D, N, N_words) >= 0 || cc_bn_cmp_word(E, N_words, 1) <= 0 || cc_bn_cmp_words(E, N, N_words) >= 0)
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    // Check that D, E are inverse modulo P-1 and Q-1
    if (P != NULL && Q != NULL && D != NULL && E != NULL)
    {
        if (cc_bn_cmp_word(P, PQ_words, 1) <= 0 || cc_bn_cmp_word(Q, PQ_words, 1) <= 0)
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }

        /* Compute D*E-1 mod P-1 = 0 */
        // T = D*E
        cc_bn_mul_words(T, D, E, N_words);
        cc_bn_sub_word(T, T, N_words * 2, 1);

        cc_bn_sub_word(TP, P, PQ_words, 1);
        CC_CHK(cc_bn_mod(TP, T, N_words * 2, TP, PQ_words));
        if (!cc_bn_is_zero(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }

        /* Compute D*E-1 mod Q-1 = 0 */
        cc_bn_sub_word(TP, Q, PQ_words, 1);
        CC_CHK(cc_bn_mod(TP, T, N_words * 2, TP, PQ_words));
        if (!cc_bn_is_zero(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    return CC_OK;
}

cc_status_t cc_rsa_validate_crt(size_t bits, const cc_bn_t *D, const cc_bn_t *P, const cc_bn_t *Q, const cc_bn_t *DP, const cc_bn_t *DQ, const cc_bn_t *QP)
{
    cc_bn_t T[CC_BN_MAX_WORDS];
    cc_bn_t TP[CC_BN_MAX_WORDS / 2];

    size_t N_words = (bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
    size_t PQ_words = ((bits >> 1) + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;

    /* Check that D - DP == 0 mod P - 1 */
    if (P != NULL && D != NULL && DP != NULL)
    {
        // T = D - DP
        cc_bn_sub_small(T, D, N_words, DP, PQ_words);
        // TP = P - 1
        cc_bn_sub_word(TP, P, PQ_words, 1);
        // TP = T mod (P-1)
        CC_CHK(cc_bn_mod(TP, T, N_words, TP, PQ_words));
        if (!cc_bn_is_zero(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    /* Check that D - DQ == 0 mod Q - 1 */
    if (Q != NULL && D != NULL && DQ != NULL)
    {
        // T = D - DQ
        cc_bn_sub_small(T, D, N_words, DQ, PQ_words);
        // TP = Q - 1
        cc_bn_sub_word(TP, Q, PQ_words, 1);
        // TP = T mod (Q-1)
        CC_CHK(cc_bn_mod(TP, T, N_words, TP, PQ_words));
        if (!cc_bn_is_zero(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    /* Check that QP * Q == 1 mod P */
    if (P != NULL && Q != NULL && QP != NULL)
    {
        // T = QP * Q
        cc_bn_mul_words(T, QP, Q, PQ_words);
        // TP = T mod P
        CC_CHK(cc_bn_mod(TP, T, PQ_words * 2, P, PQ_words));
        if (!cc_bn_is_one(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    return CC_OK;
}