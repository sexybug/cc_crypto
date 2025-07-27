
#ifndef CC_ERR_H
#define CC_ERR_H

typedef enum
{
    CC_OK = 0,

    CC_BN_IS_PRIME = 1,       // 100% certainty that the number is prime
    CC_BN_PROBABLY_PRIME = 2, // miller-rabin prime test passed, but not 100% certain
    CC_BN_IS_COMPOSITE = 3,   // 100% certainty that the number is composite

    CC_ERR_BN_LEN_TOO_SHORT = 0x100, // ERROR: bn length too short
    CC_ERR_BN_LEN_TOO_LONG = 0x101,  // ERROR: bn length too long
    CC_ERR_BN_A_LT_B = 0x102,        // ERROR: args A<B
    CC_ERR_BN_DIV_BY_ZERO = 0x103,   // ERROR: division by zero
    CC_ERR_BN_NOT_SQUARE = 0x104,    // ERROR: not a square
    CC_ERR_BN_NO_INVERSE = 0x105,    // ERROR: no inverse
    CC_ERR_BN_GEN_RAND = 0x106,      // ERROR: random number generation failed
    CC_ERR_BN_INVALID_ARG = 0x107,   // ERROR: invalid argument

    CC_ERR_RSA_GEN_KEY = 0x200, // ERROR: RSA key generation failed
    CC_ERR_RSA_VALIDATE_KEY = 0x201, // ERROR: RSA key validation failed
} cc_status_t;

#define CC_ERR(cc_bn_status) ((cc_bn_status) != CC_OK)
#define CC_CHK(f)              \
    {                          \
        cc_status_t ret = (f); \
        if (CC_ERR(ret))       \
            return ret;        \
    }

#endif /* CC_ERR_H */