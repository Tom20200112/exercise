#include <stdio.h>

int main(void) {
    int m, n;
    scanf("%d", &m);
    scanf("%d", &n);
    if (m < 0 || n < 0) {
        printf("m and n must >= 0.\n");
        return 1;
    }

    int m1 = m, n1 = n;

    while (m1 != 0 && n1 != 0 && (m1 % n1) != 0 && (n1 % m1) != 0) {
        m1 = m1 % n1;
        n1 = n1 % m1;
    }

    int gcd, lcm;
    if (m1 != 0 && n1 != 0) {
        // printf("if\n");
        gcd = m1 < n1 ? m1 : n1;
    } else {
        // printf("else\n");
        gcd = m1 != 0 ? m1 : n1;
    }

    printf("gcd(%d, %d) is %d\n", m, n, gcd);

    lcm = gcd * (m / gcd) * (n / gcd);
    printf("lcm(%d, %d) is %d\n", m, n, lcm);

    return 0;
}