#include <stdio.h>

int bitcount(unsigned x) {
    int b;

    for (b = 0; x != 0; x >>= 1)
        if (x & 01) b++;
    return b;
}

int main(void) {
    int a = 0x56674, b;
    b = bitcount(a);
    printf("b=%d\n", b);
    return 0;
}