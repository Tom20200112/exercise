#include <assert.h>
#include <stdio.h>

int main(void)
{
    int n;
    scanf("%d", &n);
    float sum = 0;
    float fac = 1, i = 1;

    asert(n > 0);
    if (n == 1) {
        sum = 1;
    } else {
        for (; i <= n; ++i) {
            fac *= i;
            sum += 1 / fac;
        }
        sum += 1;
    }

    printf("The sum is %f", sum);

    return 0;
}