#include <stdio.h>

int main(void) {
    int intvar = 2147483647;
    printf("1 intvar=%d\n", intvar);

    intvar = 2147483647 * 3 / 3;
    printf("2 intvar=%d\n", intvar);

    return 0;
}