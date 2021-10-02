#include <stdio.h>

int main(void)
{
    int m, n, gcd, lcm;
    int m1, n1;
    scanf("%d", &m);
    scanf("%d", &n);
    m1 = m;
    n1 = n;

    while (m1 != 0 && n1 != 0 && (m1 % n1) != 0 && (n1 % m1) != 0) {
        m1 = m1 % n1;
        n1 = n1 % m1;
    }

    if (m1 != 0 && n1 != 0) {
        gcd = m1 < n1 ? m1 : n1;
    } else {
        gcd = m1 != 0 ? m1 : n1;
    }

    printf("gcd(%d, %d) is %d\n", m, n, gcd);
    lcm = gcd * (m / gcd) * (n / gcd);
    printf("lcm(%d, %d) is %d\n", m, n, lcm);

    return 0;
}