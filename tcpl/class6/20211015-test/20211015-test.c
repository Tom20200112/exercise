#include <stdio.h>

int main(void) {
    int a, b, c;
    a = 023 << 3 ^ 6 | 0x14;
    b = 0x27 & ~6 + 17 >> 2;
    c = 0x8265 & (~0 << 6);

    printf("%x, %x, %x\n", a, b, c);

    return 0;
}