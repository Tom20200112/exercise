#include <stdio.h>

double beer(double n) {
    double c = 1;
    for (; n > 0; n--) {
        c = (c + n) * (n + 4) / (n + 3);
    }

    return c;
}

int main(void) {
    int n;
    printf("How many days are you drinking beer: ");

    scanf("%d", &n);

    printf("You bought a bottle of %lf litres beer\n", 0.25 * beer(n));

    printf("%-8s%-16s%-16s%-16s%-8s%-8s\n", "day", "left", "1/(n+4)", "1/(n+4)*left", "CUPs", "cup*0.25");

    double L = 0.25 * beer(n);

    for (int i = 1; i <= n; i++) {
        printf("day%-2d:  %-16lf1/%-14d%-16lf%-8d%-8lf\n", i, L, i + 4, L / (i + 4), i, ((double)i) * 0.25);
        L = L * (i + 3) / (i + 4) - i * 0.25;
    }

    printf("day%-2d:  %-16f\n", n + 1, 0.25);

    return 0;
}