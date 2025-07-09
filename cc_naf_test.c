#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "cc_bn.h"
#include "cc_naf.h"
#include "test.h"

void print_naf(int *naf, size_t naf_len)
{
    printf("NAF: \n");
    for (size_t i = naf_len - 1; i < naf_len; i--)
    {
        if ((i + 1) % 8 == 0)
        {
            printf("\n");
        }
        printf("naf[%zu] = %d, ", i, naf[i]);
    }
    printf("\n");
}
int main()
{
    cc_bn_digit_t K2[2] = {0x42E576F7, 0x00};

    int naf2[64];
    size_t naf2_len = naf(K2, 2, 2, naf2);
    printf("naf2_len = %d\n", naf2_len);
    print_naf(naf2, naf2_len);
    /*
    naf2_len = 31
NAF:
naf[30] = 1, naf[29] = 0, naf[28] = 0, naf[27] = 0, naf[26] = 1, naf[25] = 0, naf[24] = -1,
naf[23] = 0, naf[22] = 0, naf[21] = -1, naf[20] = 0, naf[19] = 1, naf[18] = 0, naf[17] = -1, naf[16] = 0,
naf[15] = -1, naf[14] = 0, naf[13] = 0, naf[12] = 0, naf[11] = -1, naf[10] = 0, naf[9] = 0, naf[8] = -1,
naf[7] = 0, naf[6] = 0, naf[5] = 0, naf[4] = 0, naf[3] = -1, naf[2] = 0, naf[1] = 0, naf[0] = -1,
    */

    cc_bn_digit_t K3[2] = {0x42E576F7, 0x00};
    int naf3[64];
    size_t naf3_len = naf(K3, 2, 3, naf3);
    printf("naf3_len = %d\n", naf3_len);
    print_naf(naf3, naf3_len);
    /*
    naf3_len = 31
NAF:
naf[30] = 1, naf[29] = 0, naf[28] = 0, naf[27] = 0, naf[26] = 0, naf[25] = 0, naf[24] = 3,
naf[23] = 0, naf[22] = 0, naf[21] = -1, naf[20] = 0, naf[19] = 0, naf[18] = 1, naf[17] = 0, naf[16] = 0,
naf[15] = 3, naf[14] = 0, naf[13] = 0, naf[12] = 0, naf[11] = -1, naf[10] = 0, naf[9] = 0, naf[8] = -1,
naf[7] = 0, naf[6] = 0, naf[5] = 0, naf[4] = 0, naf[3] = -1, naf[2] = 0, naf[1] = 0, naf[0] = -1,
    */

    cc_bn_digit_t K4[2] = {0x42E576F7, 0x00};
    int naf4[64];
    size_t naf4_len = naf(K4, 2, 4, naf4);
    printf("naf4_len = %d\n", naf4_len);
    print_naf(naf4, naf4_len);
    /*
    naf4_len = 31
NAF:
naf[30] = 1, naf[29] = 0, naf[28] = 0, naf[27] = 0, naf[26] = 0, naf[25] = 1, naf[24] = 0,
naf[23] = 0, naf[22] = 0, naf[21] = 7, naf[20] = 0, naf[19] = 0, naf[18] = 0, naf[17] = 0, naf[16] = 5,
naf[15] = 0, naf[14] = 0, naf[13] = 0, naf[12] = 7, naf[11] = 0, naf[10] = 0, naf[9] = 0, naf[8] = 7,
naf[7] = 0, naf[6] = 0, naf[5] = 0, naf[4] = -1, naf[3] = 0, naf[2] = 0, naf[1] = 0, naf[0] = 7,
    */

    cc_bn_digit_t K5[2] = {0x42E576F7, 0x00};
    int naf5[64];
    size_t naf5_len = naf(K5, 2, 5, naf5);
    printf("naf5_len = %d\n", naf5_len);
    print_naf(naf5, naf5_len);
    /*
    naf5_len = 32
NAF:
naf[31] = 1, naf[30] = 0, naf[29] = 0, naf[28] = 0, naf[27] = 0, naf[26] = -15, naf[25] = 0, naf[24] = 0,
naf[23] = 0, naf[22] = 0, naf[21] = -9, naf[20] = 0, naf[19] = 0, naf[18] = 0, naf[17] = 0, naf[16] = 0,
naf[15] = 11, naf[14] = 0, naf[13] = 0, naf[12] = 0, naf[11] = 0, naf[10] = 0, naf[9] = 0, naf[8] = -9,
naf[7] = 0, naf[6] = 0, naf[5] = 0, naf[4] = 0, naf[3] = 0, naf[2] = 0, naf[1] = 0, naf[0] = -9,
    */

    cc_bn_digit_t K6[2] = {0x42E576F7, 0x00};
    int naf6[64];
    size_t naf6_len = naf(K6, 2, 6, naf6);
    printf("naf6_len = %d\n", naf6_len);
    print_naf(naf6, naf6_len);
    /*
    naf6_len = 31
    NAF:
    naf[30] = 1, naf[29] = 0, naf[28] = 0, naf[27] = 0, naf[26] = 0, naf[25] = 0, naf[24] = 0,
    naf[23] = 0, naf[22] = 0, naf[21] = 23, naf[20] = 0, naf[19] = 0, naf[18] = 0, naf[17] = 0, naf[16] = 0,
    naf[15] = 11, naf[14] = 0, naf[13] = 0, naf[12] = 0, naf[11] = 0, naf[10] = 0, naf[9] = 0, naf[8] = -9,
    naf[7] = 0, naf[6] = 0, naf[5] = 0, naf[4] = 0, naf[3] = 0, naf[2] = 0, naf[1] = 0, naf[0] = -9,
    */

    return 0;
}
