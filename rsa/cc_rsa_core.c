
#include "cc_rsa_core.h"
#include "cc_bn_prime.h"
#include "cc_bn_mod.h"
#include "cc_bn_mul.h"
#include "cc_bn_mont.h"
#include "cc_crypto_rng.h"

#include "cc_test.h"

// at most try 1000 times to generate key
#define COUNT_MAX 1000

void cc_rsa_set_pubkey(cc_rsa_pubkey_st *pubkey, const cc_bn_word_t *N, const cc_bn_word_t *E, size_t bits)
{
    size_t N_words = cc_bn_word_len_from_bit_len(bits);

    pubkey->bits = bits;
    if (N != NULL)
    {
        cc_bn_copy(pubkey->N, N, N_words);
    }
    if (E != NULL)
    {
        cc_bn_copy(pubkey->E, E, N_words);
    }
}

void cc_rsa_set_privkey(cc_rsa_privkey_st *privkey, const cc_bn_word_t *N, const cc_bn_word_t *D, size_t bits)
{
    size_t N_words = cc_bn_word_len_from_bit_len(bits);

    privkey->bits = bits;
    if (N != NULL)
    {
        cc_bn_copy(privkey->N, N, N_words);
    }
    if (D != NULL)
    {
        cc_bn_copy(privkey->D, D, N_words);
    }
}

void cc_rsa_set_privkey_crt(cc_rsa_privkey_st *privkey, const cc_bn_word_t *N, const cc_bn_word_t *D, const cc_bn_word_t *P, const cc_bn_word_t *Q, const cc_bn_word_t *DP, const cc_bn_word_t *DQ, const cc_bn_word_t *QP, size_t bits)
{
    size_t N_words = cc_bn_word_len_from_bit_len(bits);
    size_t PQ_words = cc_bn_word_len_from_bit_len(bits >> 1);

    privkey->bits = bits;
    if (N != NULL)
    {
        cc_bn_copy(privkey->N, N, N_words);
    }
    if (D != NULL)
    {
        cc_bn_copy(privkey->D, D, N_words);
    }
    if (P != NULL)
    {
        cc_bn_copy(privkey->P, P, PQ_words);
    }
    if (Q != NULL)
    {
        cc_bn_copy(privkey->Q, Q, PQ_words);
    }
    if (DP != NULL)
    {
        cc_bn_copy(privkey->DP, DP, PQ_words);
    }
    if (DQ != NULL)
    {
        cc_bn_copy(privkey->DQ, DQ, PQ_words);
    }
    if (QP != NULL)
    {
        cc_bn_copy(privkey->QP, QP, PQ_words);
    }
}

// 3<= E<= N-1, bits(E)<=bits, E is odd
cc_status_t cc_rsa_core_gen_key(cc_rsa_pubkey_st *pubkey, cc_rsa_privkey_st *privkey,
                                size_t bits, const cc_bn_word_t *E, size_t E_word_len, bool crt,
                                cc_crypto_rng_f rng)
{
    int count = 0;
    cc_bn_word_t H[CC_RSA_PQ_MAX_WORDS * 2];
    cc_bn_word_t *G = pubkey->N;
    cc_bn_word_t *L = privkey->N;
    size_t N_words = cc_bn_word_len_from_bit_len(bits);

    if ((bits % 2 != 0) || (bits < 8) || (bits > CC_BN_MAX_BITS))
    {
        return CC_ERR_BN_INVALID_ARG;
    }
    // E>=3, bits(E)<=bits, E is odd
    if ((cc_bn_cmp_word(E, E_word_len, 3) < 0) || (cc_bn_bit_len(E, E_word_len) > bits) || CC_BN_IS_EVEN(E))
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    size_t PQ_bits = bits >> 1;
    // note: N_words <= PQ_words*2
    size_t PQ_words = (PQ_bits + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS;
    cc_bn_from_words(pubkey->E, N_words, E, E_word_len);

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
        cc_bn_gcd(G, pubkey->E, N_words, H, N_words);
        if (!cc_bn_is_one(G, N_words))
        {
            continue;
        }

        /* compute smallest possible D = E^-1 mod LCM(P-1, Q-1) (FIPS 186-5 A.1.1) */
        // G = GCD(P-1, Q-1)
        cc_bn_gcd(G, privkey->P, PQ_words, privkey->Q, PQ_words);
        // L = LCM(P-1, Q-1) = (P-1)*(Q-1) / GCD(P-1, Q-1) = H/G
        CC_CHK(cc_bn_core_div(L, H, H, N_words, G, PQ_words));
        // D = E^-1 mod LCM(P-1, Q-1)
        CC_CHK(cc_bn_exgcd_mod_inv(privkey->D, pubkey->E, N_words, L, N_words));

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

    cc_bn_copy(pubkey->N, privkey->N, N_words);
    pubkey->bits = bits;

    return CC_OK;
}

// if validate pass, return CC_OK
// if validate fail, return CC_ERR_RSA_VALIDATE_KEY
cc_status_t cc_rsa_validate_params(size_t bits, const cc_bn_word_t *N, const cc_bn_word_t *E, const cc_bn_word_t *D, const cc_bn_word_t *P, const cc_bn_word_t *Q, cc_crypto_rng_f rng)
{
    cc_bn_word_t T[CC_BN_MAX_WORDS * 2];
    cc_bn_word_t TP[CC_RSA_PQ_MAX_WORDS];

    size_t N_words = cc_bn_word_len_from_bit_len(bits);
    size_t PQ_words = cc_bn_word_len_from_bit_len(bits >> 1);

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

        /* Compute D*E mod P-1 = 1 */
        // T = D*E
        cc_bn_mul_words(T, D, E, N_words);

        cc_bn_sub_word(TP, P, PQ_words, 1);
        CC_CHK(cc_bn_mod(TP, T, N_words * 2, TP, PQ_words));
        if (!cc_bn_is_one(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }

        /* Compute D*E mod Q-1 = 1 */
        cc_bn_sub_word(TP, Q, PQ_words, 1);
        CC_CHK(cc_bn_mod(TP, T, N_words * 2, TP, PQ_words));
        if (!cc_bn_is_one(TP, PQ_words))
        {
            return CC_ERR_RSA_VALIDATE_KEY;
        }
    }

    return CC_OK;
}

cc_status_t cc_rsa_validate_crt(size_t bits, const cc_bn_word_t *D, const cc_bn_word_t *P, const cc_bn_word_t *Q, const cc_bn_word_t *DP, const cc_bn_word_t *DQ, const cc_bn_word_t *QP)
{
    cc_bn_word_t T[CC_RSA_PQ_MAX_WORDS * 2];
    cc_bn_word_t TP[CC_RSA_PQ_MAX_WORDS];

    size_t N_words = cc_bn_word_len_from_bit_len(bits);
    size_t PQ_words = cc_bn_word_len_from_bit_len(bits >> 1);

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

// C = M^E mod N
// M < N
// C can alias M
cc_status_t cc_rsa_core_public_op(cc_rsa_pubkey_st *pubkey, const cc_bn_word_t *M, cc_bn_word_t *C)
{
    size_t N_words = cc_bn_word_len_from_bit_len(pubkey->bits);

    // if M >= N, return error
    if (cc_bn_cmp_words(M, pubkey->N, N_words) >= 0)
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    // C = M^E mod N
    cc_bn_mod_exp_mont(C, M, N_words, pubkey->E, N_words, pubkey->N, N_words);

    return CC_OK;
}

// M = C^D mod N
// C < N
// M can alias C
cc_status_t cc_rsa_core_private_op(cc_rsa_privkey_st *privkey, const cc_bn_word_t *C, cc_bn_word_t *M)
{
    size_t N_words = cc_bn_word_len_from_bit_len(privkey->bits);

    // if C >= N, return error
    if (cc_bn_cmp_words(M, privkey->N, N_words) >= 0)
    {
        return CC_ERR_BN_INVALID_ARG;
    }

    // M = C^D mod N
    cc_bn_mod_exp_mont(M, C, N_words, privkey->D, N_words, privkey->N, N_words);

    return CC_OK;
}

// M = C^D mod N
// C < N
// M can alias C
cc_status_t cc_rsa_core_private_op_crt(cc_rsa_privkey_st *privkey, const cc_bn_word_t *C, cc_bn_word_t *M)
{
    size_t N_words = cc_bn_word_len_from_bit_len(privkey->bits);
    size_t PQ_words = cc_bn_word_len_from_bit_len(privkey->bits >> 1);

    cc_bn_word_t M1[CC_RSA_PQ_MAX_WORDS];
    cc_bn_word_t M2[CC_RSA_PQ_MAX_WORDS];
    cc_bn_word_t MT[CC_RSA_PQ_MAX_WORDS * 2];

    // M1 = C^DP mod P
    cc_bn_mod_exp_mont(M1, C, N_words, privkey->DP, PQ_words, privkey->P, PQ_words);
    // M2 = C^DQ mod Q
    cc_bn_mod_exp_mont(M2, C, N_words, privkey->DQ, PQ_words, privkey->Q, PQ_words);
    // H = (M1 - M2) mod P
    cc_bn_mod_sub(M1, M1, M2, privkey->P, PQ_words);
    // H = (M1 - M2) * QP mod P
    cc_bn_mod_mul_words(M1, M1, privkey->QP, privkey->P, PQ_words);
    // MT = Q * H
    cc_bn_mul(MT, privkey->Q, PQ_words, M1, PQ_words);
    // M = M2 + MT
    cc_bn_add(M, M2, PQ_words, MT, N_words);

    return CC_OK;
}