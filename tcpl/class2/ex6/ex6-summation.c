#include <stdio.h>

int main(void)
{
    float sign = 1, i = 1;
    int j, n;
    float sum = 0;

    scanf("%d", &n);

    for (j = 1; j <= n; j++) {
        sum = sum + (sign / i);
        sign = -sign;
        i++;
    }
    printf("The result is %f\n", sum);

    return 0;
}