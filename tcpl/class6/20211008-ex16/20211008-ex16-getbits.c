#include <stdio.h>

unsigned getbits(unsigned x, int p, int n) { return (x >> (p + 1 - n)) & ~(~0 << n); }

int main(void) {
    int x, p, n, b;
    scanf("%x %d %d", &x, &p, &n);

    b = getbits(x, p, n);
    printf("b=%x\n", b);

    return 0;
}