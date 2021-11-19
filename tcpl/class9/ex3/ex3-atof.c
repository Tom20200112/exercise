#include <ctype.h>
#include <stdio.h>

#define MAX 1000

/* atof:  convert string s to double */
double atof(const char s[]) {
    double val, power;
    int i, sign;

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

    return sign * val / power;
}

int get_line(char s[], int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        s[i] = c;
    }

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';
    return i;
}

int main(void) {
    char s[MAX];
    double d;

    printf("Please input a float point number: ");

    get_line(s, MAX);
    d = atof(s);

    printf("s=%s", s);
    printf("d=%lf\n", d);

    return 0;
}