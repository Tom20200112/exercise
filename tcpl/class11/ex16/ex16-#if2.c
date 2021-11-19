#include <stdio.h>

int main(void) {
    int a = 10, b = 3;

#if !defined(HDR)
    a += b;
#endif
    a *= b;

    printf("Result=%d\n", a);

    return 0;
}