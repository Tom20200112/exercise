#include <stdio.h>

int main(void) {
    long long int a, b;
    char op;

    scanf("%lld %c %lld", &a, &op, &b);

    switch (op) {
        case '+':
            printf("%lld\n", a + b);
            break;
        case '-':
            printf("%lld\n", a - b);
            break;
        case '*':
            printf("%lld\n", a * b);
            break;

        default:
            break;
    }

    return 0;
}