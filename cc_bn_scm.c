
#include "cc_bn_scm.h"

// flag=0: R=A; flag=1: R=B
void cc_bn_const_time_select(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len, bool flag)
{
    // flag: 0 or 1
    size_t i;
    cc_bn_word_t mask = -flag; // all bits are flag
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t tmp = (A[i] & ~mask) | (B[i] & mask);
        R[i] = tmp;
    }
}

void cc_bn_const_time_select3(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, size_t bn_word_len, bool selA, bool selB, bool selC)
{
    // selA, selB, selC: 0 or 1
    size_t i;
    cc_bn_word_t maskA = -selA; // all bits are selA
    cc_bn_word_t maskB = -selB; // all bits are selB
    cc_bn_word_t maskC = -selC; // all bits are selC
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t tmp = (A[i] & maskA) | (B[i] & maskB) | (C[i] & maskC);
        R[i] = tmp;
    }
}

void cc_bn_const_time_select4(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, const cc_bn_word_t *D, size_t bn_word_len, bool selA, bool selB, bool selC, bool selD)
{
    // selA, selB, selC, selD: 0 or 1
    size_t i;
    cc_bn_word_t maskA = -selA; // all bits are selA
    cc_bn_word_t maskB = -selB; // all bits are selB
    cc_bn_word_t maskC = -selC; // all bits are selC
    cc_bn_word_t maskD = -selD; // all bits are selD
    for (i = 0; i < bn_word_len; i++)
    {
        cc_bn_word_t tmp = (A[i] & maskA) | (B[i] & maskB) | (C[i] & maskC) | (D[i] & maskD);
        R[i] = tmp;
    }
}