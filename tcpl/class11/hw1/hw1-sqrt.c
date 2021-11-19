#include <stdio.h>

double sqrt(double a) {
    double y = 1;
    double yi = a;

    while (y - yi >= 1e-5 || y - yi <= -1e-5) {
        yi = y;
        y = 0.5 * (y + a / y);
    }

    return y;
}

int main(void) {
    double a, x;
    printf("Type in a number: ");
    scanf("%lf", &a);

    x = sqrt(a);

    printf("Square root of a is %lf\n", x);

    return 0;
}