
#ifndef CC_BN_CONFIG_H
#define CC_BN_CONFIG_H

#include "cc_bn.h"

#define CC_BN_MAX_BITS 8192 // max bits of BN, used for stack allocation
#define CC_BN_MAX_BYTES (CC_BN_MAX_BITS / 8)
#define CC_BN_MAX_WORDS (CC_BN_MAX_BITS / CC_BN_WORD_BITS)

#endif // CC_BN_CONFIG_H
