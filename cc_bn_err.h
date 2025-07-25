
#ifndef CC_BN_ERR_H
#define CC_BN_ERR_H

typedef enum
{
    CC_BN_SUCCESS = 0,
    CC_BN_IS_PRIME = 1,       // 100% certainty that the number is prime
    CC_BN_PROBABLY_PRIME = 2, // miller-rabin prime test passed, but not 100% certain
    CC_BN_IS_COMPOSITE = 3,   // 100% certainty that the number is composite

    CC_BN_ERR_LEN_TOO_SHORT = 0x100, // ERROR: bn length too short
    CC_BN_ERR_LEN_TOO_LONG = 0x101, // ERROR: bn length too long
    CC_BN_ERR_A_LT_B = 0x102,        // ERROR: args A<B
    CC_BN_ERR_DIV_BY_ZERO = 0x103,   // ERROR: division by zero
    CC_BN_ERR_NOT_SQUARE = 0x104,    // ERROR: not a square
    CC_BN_ERR_GEN_RAND = 0x105,      // ERROR: random number generation failed
} cc_bn_status_t;

#define CC_BN_ERR(cc_bn_status) ((cc_bn_status) != CC_BN_SUCCESS)

#endif /* CC_BN_ERR_H */