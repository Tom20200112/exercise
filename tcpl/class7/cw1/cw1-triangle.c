#include <stdio.h>

int basic_factorial(int n) {
    __uint128_t fact = 1;

    for (; n > 0; n--) {
        fact *= n;
    }

    return fact;
}

int combine(int i, int j) { return basic_factorial(i) / basic_factorial(j) / basic_factorial(i - j); }

int main(void) {
    int triangle[10][10];

    triangle[0][0] = 1;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j <= i; j++) {
            triangle[i][j] = combine(i, j);
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j <= i; ++j) {
            printf("%d ", triangle[i][j]);
        }

        printf("\n");
    }

    return 0;
}