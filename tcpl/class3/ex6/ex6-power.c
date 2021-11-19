#include <assert.h>
#include <stdio.h>

int power(int x, int y);

int main(void)
{
    int x, y, i;

    scanf("%d", &x);
    scanf("%d", &y);

    assert(y > 0);

    printf("%d to the %dth power is %d\n", x, y, power(x, y));

    for (i = 0; i < 10; i++) {
        printf("%3d, %6d, %6d\n", i, power(2, i), power(-3, i));
    }

    return 0;
}

int power(int x, int y)
{
    int base = 1;

    for (int i = 0; i < y; i++) {
        base *= x;
    }

    return base;
}
