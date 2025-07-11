#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    uint32_t v=0x80000000;
    v = v>>1;

    uint32_t a = 0xf0000001%3;
    return 0;
}