#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cc_test.h"

uint32_t mbedtls_mpi_core_montmul_init(const uint32_t *N)
{
    uint32_t x = N[0];

    x += ((N[0] + 2) & 4) << 1;

    for (unsigned int i = 32; i >= 8; i /= 2)
    {
        x *= (2 - (N[0] * x));
    }

    return ~x + 1;
}

int main()
{
    // {
    //     uint32_t v = 0x80000000;
    //     v = v >> 1;

    //     uint32_t a = 0xf0000001 % 3;

    //     uint32_t N[2] = {0x87654321, 0x87654321};
    //     uint32_t N_inv = mbedtls_mpi_core_montmul_init(N);
    // }

    // {
    //     uint64_t a = 1 << 31;
    //     uint64_t b = (uint64_t)1 << 32;

    //     printf("%llx\n", a);
    //     printf("%llx\n", b);
    // }
    // {
    //     size_t a = 0xFFFFFFFF;
    //     int b = a;
    //     printf("%d\n", b);
    // }

    {
        char *str = "FFFE0102";
        int8_t dst[300];
        int len = strlen(str)/2;
        HexString2Hex(str, len, dst);
        int i;
        for(i=0;i<len;i++)
        {
            printf("%d,",dst[i]);
        }
    }

    return 0;
}