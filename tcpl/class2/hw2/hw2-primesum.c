#include <stdio.h>

int main(void)
{
    float sum = 0;
    float i;
    int isprime(int i);

    for (i = 0; i < 1000; i++) {
        if (isprime((int)i)) {
            sum = sum + (1 / i);
        }
    }
    printf("The sum of reciprocals of prime numbers from 1 to 1000 is %f\n", sum);

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