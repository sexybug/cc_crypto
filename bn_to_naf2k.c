
#include "cc_bn.h"
#include <stdio.h>
#include <string.h>
#include "test.h"

void print_naf2k(int *naf2k, int len)
{
    printf("NAF2k: \n");
    for (int i = len - 1; i >= 0; i--)
    {
        if ((i + 1) % 8 == 0)
        {
            printf("\n");
        }
        printf("naf2k[%d] = %d, ", i, naf2k[i]);
    }
    printf("\n");
}
void print_binary(const uint8_t *data, int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            printf("%d", (data[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}
void compute_naf2k(int *naf2k, const uint32_t K[8])
{
    int bit1, bit2, bit0;
    int b1 = 0, b2 = 0, e;
    int initial = 0; // initial PointC = PointA value,Previous defined

    int i;
    int sNum = cc_bn_bit_len(K, 8) - 1; // 253
    for (i = sNum + 1; i >= 0; i -= 1)
    {
        // deal the first time
        if (i == sNum + 1)
        {
            bit1 = cc_bn_get_bit(K, i - 1); // the higest bit			//bit(254)=1
            bit2 = cc_bn_get_bit(K, i - 2); // the second_highest bit	//bit(253)=1
            b1 = (bit1 + bit2) / 2;         // 1
            e = b1;                         // 1
            b2 = b1;                        // 1

            bit0 = bit1; // 1
            bit1 = bit2; // 1

            // initial PointC = PointA value,Previous defined
            if (e == 1)
            {
                initial = 1; // 1
                naf2k[i] = 1;
            }
            // step out cycle
            continue;
        }
        // deal the second backward time
        if (i == 1)
        {
            // if initial = 0,NO operation,else PointC = 2*PointC
            if (initial == 1)
            {
                printf("Q = 2Q\n");
                naf2k[i] = 0;
            }

            b1 = (b2 + bit1) / 2;
            e = -2 * b2 + bit0 + b1;
            b2 = b1;

            bit0 = bit1;
            if (e == 1)
            {
                // if no initial,initial PointC = PointA value,Previous defined
                if (initial != 1)
                {
                    initial = 1;
                }
                else
                {
                    printf("Q = Q + P\n");
                    naf2k[i] = 1;
                }
            }
            if (e == -1)
            {
                printf("Q = Q - P\n");
                naf2k[i] = -1;
            }
            // step out cycle
            continue;
        }
        if (i == 0)
        {
            // if initial = 0,NO operation,else PointC = 2*PointC
            if (initial == 1)
            {
                printf("Q = 2Q\n");
                naf2k[i] = 0;
            }

            e = -2 * b2 + bit0;
            if (e == 1)
            {
                // if no initial,initial PointC = PointA value,Previous defined
                if (initial != 1)
                {
                    initial = 1;
                }
                else
                {
                    printf("Q = Q + P\n");
                    naf2k[i] = 1;
                }
            }
            if (e == -1)
            {
                printf("Q = Q - P\n");
                naf2k[i] = -1;
            }
            // finish cycle
            break;
        }

        // if initial = 0,NO operation,else PointC = 2*PointC
        if (initial == 1)
        {
            printf("Q = 2Q\n");
            naf2k[i] = 0;
        }

        bit2 = cc_bn_get_bit(K, i - 2); // bit(251)=1,bit(250)=0,bit(249)=0,bit(248)=1,
        b1 = (b2 + bit1 + bit2) / 2;    // b1 = [(b2+ki_1+ki_2)/2}]	//1,1,0, 0,
        e = -2 * b2 + bit0 + b1;        // e = -2*b2 + ki + b1			//0,0,-1,0,
        b2 = b1;                        // b2 = b1;									//1,1,0, 0,

        bit0 = bit1; // 1,1,0, 0,
        bit1 = bit2; // 1,0,0, 1,

        if (e == 1)
        {
            // if no initial,initial PointC = PointA value,Previous defined
            if (initial != 1)
            {
                initial = 1;
                naf2k[i] = 1;
            }
            else
            {
                printf("Q = Q + P\n");
                naf2k[i] = 1;
            }
        }
        if (e == -1)
        {
            printf("Q = Q - P\n");
            naf2k[i] = -1;
        }
    }

    print_naf2k(naf2k, 257);
}





int main(int argc, char *argv[])
{
    // char key_str[] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";
    // char key_str[] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    // char key_str[] = "8000000000000000000000000000000000000000000000000000000000000000";
    // char key_str[] = "8000000000000000000000000000000000000000000000000000000000000003";
    // char key_str[] = "12AEEF5512D9E0BF357C07F6BEF68DD6CDFD114436B8FB9D9FB46863C02A0083";
    char key_str[] = "83002AC06368B49F9DFBB8364411FDCDD68DF6BEF6077C35BFE0D91255EFAE12";
    uint8_t key[32];
    uint32_t K[8];
    HexString2Hex(key_str, 32, key);
    print_binary(key, 32);

    cc_u8_to_bn(key, 32, 8, K);

    int naf2k[257] = {0};
    compute_naf2k(naf2k, K);

    for(int i=256; i >= 0; i--)
    {
        printf("%d,",naf2k[i]);
    }
}

