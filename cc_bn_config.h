
#ifndef CC_BN_CONFIG_H
#define CC_BN_CONFIG_H

#include "cc_bn.h"

// max bits of input BN, used for stack allocation
// make sure it is a multiple of CC_BN_WORD_BITS*2, otherwise will overflow in multiply
#define CC_BN_MAX_BITS 8192
#define CC_BN_MAX_BYTES (CC_BN_MAX_BITS / 8)
#define CC_BN_MAX_WORDS (CC_BN_MAX_BITS / CC_BN_WORD_BITS)

#define CC_EC_MAX_BITS 521
#define CC_EC_MAX_BYTES ((CC_EC_MAX_BITS + 7) / 8)
#define CC_EC_MAX_WORDS ((CC_EC_MAX_BITS + CC_BN_WORD_BITS - 1) / CC_BN_WORD_BITS)

#endif // CC_BN_CONFIG_H
