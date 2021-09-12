#include <stdio.h>

int main(void)
{
    int isprime(int i);
    int i = 2;

    for (; i < 1000; i++) {
        if (isprime(i)) {
            printf("%d \t", i);
        }
    }
    printf("\n");

    return 0;
}

int isprime(int i)
{
    int j;
    for (j = 1; j < i; j++) {
        if (i % j == 0 && j != 1) {
            return 0;
        }
    }

    if (i >= 2) {
        return 1;
    } else {
        return 0;
    }
}