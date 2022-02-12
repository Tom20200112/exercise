#include <stdio.h>

long long int power_2(int n) {
    long long int pow = 1;
    long long int p_pow = 1;

    for (; 0 < n; n >>= 1) {
        if (n & 1) {
            pow <<= p_pow;
        }

        p_pow <<= 1;
    }

    return pow;
}

int main(void) {
    int n;

    scanf("%d", &n);

    printf("%lld\n", power_2(n));
}