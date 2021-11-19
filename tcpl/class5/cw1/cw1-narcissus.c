#include <stdio.h>

int IsNarcissus(int n) {
    int j;
    int s = 0;
    int m = n;

    for (j = 0; j < 3; j++) {
        s = s + (m % 10) * (m % 10) * (m % 10);
        m = m / 10;
    }

    if (s == n) {
        return 1;
    }

    return 0;
}

int main(void) {
    int n;

    for (n = 100; n < 1000; n++) {
        if (IsNarcissus(n)) {
            printf("%d, ", n);
        }
    }

    return 0;
}
