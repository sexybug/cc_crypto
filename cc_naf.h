
#ifndef CC_NAF_H
#define CC_NAF_H

#include "cc_bn.h"

// return naf length
// note: K is changed
size_t naf(cc_bn_digit_t *K, size_t K_word_len, size_t w, int8_t *naf);

#endif // CC_NAF_H