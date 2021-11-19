#include <stdio.h>

int main(void) {
    int org = 0x12345678;
    int a, b;

    a = (char)org;
    b = (short)org;
    printf("1: a=%x, b=%x\n", a, b);

    org = 0x123456ff;

    a = (char)org;
    b = (short)org;
    printf("2: a=%x, b=%x\n", a, b);

    return 0;
}