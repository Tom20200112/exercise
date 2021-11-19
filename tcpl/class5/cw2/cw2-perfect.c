#include <stdio.h>

#define MAXFACTOR 1000

int FACTOR[MAXFACTOR];

int IsPerfect(int n) {
    int i;
    int j = 0, sum = 0;

    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i == 1) {
                sum += 1;
            } else {
                sum += (i * i == n ? i : i + n / i);
            }

            if (i * i == n) {
                FACTOR[j] = i;
                j++;
            } else {
                FACTOR[j++] = i;
                FACTOR[j++] = n / i;
            }
        }
    }

    if (sum == n) {
        return 1;
    }

    return 0;
}

int main(void) {
    for (int j = 0; j < MAXFACTOR; ++j) {
        FACTOR[j] = 0;
    }

    int n = 1;

    for (; n <= 1000; n++) {
        if (IsPerfect(n)) {
            printf("%d, ", n);
            printf("factors are: ");
            for (int j = 0; FACTOR[j] != 0; j++) {
                printf("%d, ", FACTOR[j]);
            }

            printf("\n");
        }
    }

    return 0;
}
