
#ifndef CC_RSA_CORE_H
#define CC_RSA_CORE_H

#include "cc_bn.h"
#include "cc_bn_config.h"
#include "cc_crypto_rng.h"
#include "cc_err.h"

#define CC_RSA_PQ_MAX_WORDS ((CC_BN_MAX_WORDS + 1) / 2)

typedef struct
{
    size_t bits;
    cc_bn_word_t N[CC_BN_MAX_WORDS];
    cc_bn_word_t E[CC_BN_MAX_WORDS];
} cc_rsa_pubkey_st;

typedef struct
{
    size_t bits;
    cc_bn_word_t N[CC_BN_MAX_WORDS];
    cc_bn_word_t D[CC_BN_MAX_WORDS];

    cc_bn_word_t P[CC_RSA_PQ_MAX_WORDS];
    cc_bn_word_t Q[CC_RSA_PQ_MAX_WORDS];
    cc_bn_word_t DP[CC_RSA_PQ_MAX_WORDS];
    cc_bn_word_t DQ[CC_RSA_PQ_MAX_WORDS];
    cc_bn_word_t QP[CC_RSA_PQ_MAX_WORDS];
} cc_rsa_privkey_st;

void cc_rsa_set_pubkey(cc_rsa_pubkey_st *pubkey, const cc_bn_word_t *N, const cc_bn_word_t *E, size_t bits);

void cc_rsa_set_privkey(cc_rsa_privkey_st *privkey, const cc_bn_word_t *N, const cc_bn_word_t *D, size_t bits);

void cc_rsa_set_privkey_crt(cc_rsa_privkey_st *privkey, const cc_bn_word_t *N, const cc_bn_word_t *D, const cc_bn_word_t *P, const cc_bn_word_t *Q, const cc_bn_word_t *DP, const cc_bn_word_t *DQ, const cc_bn_word_t *QP, size_t bits);

// E>=3, bits(E)<=bits, E is odd
cc_status_t cc_rsa_core_gen_key(cc_rsa_pubkey_st *pubkey, cc_rsa_privkey_st *privkey,
                                size_t bits, const cc_bn_word_t *E, size_t E_word_len, bool crt,
                                cc_crypto_rng_f rng);

// if validate pass, return CC_OK
// if validate fail, return CC_ERR_RSA_VALIDATE_KEY
cc_status_t cc_rsa_validate_params(size_t bits, const cc_bn_word_t *N, const cc_bn_word_t *E, const cc_bn_word_t *D, const cc_bn_word_t *P, const cc_bn_word_t *Q, cc_crypto_rng_f rng);

cc_status_t cc_rsa_validate_crt(size_t bits, const cc_bn_word_t *D, const cc_bn_word_t *P, const cc_bn_word_t *Q, const cc_bn_word_t *DP, const cc_bn_word_t *DQ, const cc_bn_word_t *QP);

// C = M^E mod N
// M < N
// C can alias M
cc_status_t cc_rsa_core_public_op(cc_rsa_pubkey_st *pubkey, const cc_bn_word_t *M, cc_bn_word_t *C);

// M = C^D mod N
// C < N
// M can alias C
cc_status_t cc_rsa_core_private_op(cc_rsa_privkey_st *privkey, const cc_bn_word_t *C, cc_bn_word_t *M);

// M = C^D mod N
// C < N
// M can alias C
cc_status_t cc_rsa_core_private_op_crt(cc_rsa_privkey_st *privkey, const cc_bn_word_t *C, cc_bn_word_t *M);

#endif // CC_RSA_CORE_H