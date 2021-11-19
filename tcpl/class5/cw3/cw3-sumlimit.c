#include <stdio.h>

int main(void) {
    float s = 0;
    int sign = 1;
    float order = 1;

    while ((4 / order) >= 1e-6) {
        s += sign * 4 / order;
        sign *= -1;
        order += 2;
    }

    printf("%f\n", s);

    return 0;
}