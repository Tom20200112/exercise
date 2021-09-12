#include <stdio.h>

int main(void)
{
    int n, i;
    int product = 1;
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        product = product * i;
    }

    printf("The product from 1 to %d is %d\n", n, product);

    return 0;
}