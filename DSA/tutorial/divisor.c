#include <stdio.h>

int main(void) {
    int a, b;

    scanf("%d %d", &a, &b);

    while (a != 0 && b != 0 && (a % b) != 0 && (b % a) != 0) {
        a = a % b;
        b = b % a;
    }

    if (a != 0 && b != 0) {
        printf("%d", a < b ? a : b);
    } else {
        printf("%d", a != 0 ? a : b);
    }

    return 0;
}