
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>

// 一阶布尔掩码
void sbox_mask(const uint8_t sbox_in[256], uint8_t mask_in, uint8_t mask_out, uint8_t sbox_out[256])
{
    uint8_t sbox_tmp[256];
    int i;
    for (i = 0; i < 256; i++)
    {
        sbox_tmp[i] = sbox_in[i ^ mask_in] ^ mask_out;
    }
    memcpy(sbox_out, sbox_tmp, 256);
}

uint8_t gen_mask()
{
    uint8_t m = rand() & 0xFF;
    return m | 0x81;
}

int main()
{
    uint8_t sbox_in[256] = {0};
    uint8_t mask_in = 0xA5;
    uint8_t mask_out = 0xC9;
    uint8_t sbox_out[256] = {0};

    int i;
    for (i = 0; i < 256; i++)
    {
        sbox_in[i] = i;
    }

    int j;

    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < 256; j++)
        {
            mask_in = gen_mask();
            mask_out = gen_mask();
            sbox_mask(sbox_in, mask_in, mask_out, sbox_out);

            uint8_t a = j;
            uint8_t a_m = a ^ mask_in;
            uint8_t b_m = sbox_out[a_m];
            uint8_t b = b_m ^ mask_out;

            if (a != b)
            {
                printf("a != b\n");
                printf("%d\n", a);
                printf("%d\n", b);
                printf("%d\n", a_m);
                printf("%d\n", b_m);
                printf("%d\n", mask_in);
                printf("%d\n", mask_out);
            }
        }
    }
    printf("test passed\n");
}
