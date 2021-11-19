#include <ctype.h>
#include <stdio.h>

#define MAX 1000

char s[MAX];

double power1(double x, double y) {
    double base;
    for (base = 1.0; y > 0.0; y--) {
        base *= x;
    }

    return base;
}

/* atof:  convert string s to double */
double atof(char s[]) {
    double val, power;
    int i, sign, powsign;
    double pow;

    for (i = 0; isspace(s[i]); i++) /* skip white space */
        ;

    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-') i++;

    for (val = 0.0; isdigit(s[i]); i++) val = 10.0 * val + (s[i] - '0');

    if (s[i] == '.') i++;

    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10;
    }

    if (s[i] == 'e' || s[i] == 'E') {
        powsign = (s[++i] == '-') ? -1 : 1;
        if (s[i] == '+' || s[i] == '-') i++;
        for (pow = 0.0; isdigit(s[i]); i++) {
            pow = 10.0 * pow + s[i] - '0';
        }
        pow = powsign * pow;
    }

    return sign * val * (power1(10, pow)) / power;
}

int main(void) {
    printf("Please input a number: ");

    scanf("%s", s);

    printf("You have inputed %f\n", atof(s));

    return 0;
}