#include <stdio.h>

int main(void)
{
    long int n, i;
    long int product = 1;
    scanf("%ld", &n);

    for (i = 1; i <= n; i += 2) {
        product = product * i;
    }

    printf("The product of odd numbers from 1 to %ld is %ld\n", n, product);

    return 0;
}