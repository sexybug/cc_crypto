/**
 * @file cc_bn_scm.h
 * @author your name (you@domain.com)
 * @brief cc_bn side channel mitigation basic operations
 * @version 0.1
 * @date 2025-11-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef CC_BN_SCM_H
#define CC_BN_SCM_H

#include "cc_bn.h"

// flag=0: R=A; flag=1: R=B
void cc_bn_const_time_select(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, size_t bn_word_len, bool flag);

void cc_bn_const_time_select3(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, size_t bn_word_len, bool selA, bool selB, bool selC);

void cc_bn_const_time_select4(cc_bn_word_t *R, const cc_bn_word_t *A, const cc_bn_word_t *B, const cc_bn_word_t *C, const cc_bn_word_t *D, size_t bn_word_len, bool selA, bool selB, bool selC, bool selD);

#endif // CC_BN_SCM_H