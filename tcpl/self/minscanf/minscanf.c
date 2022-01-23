#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

void minscanf(char *fmt, ...) {
    va_list ap;
    char *p, *sval;

    int *ival;
    float *dval;
    int c;

    va_start(ap, fmt);

    for (p = fmt; *p; p++) {
        if (*p != '%') {
            c = getchar();
            if ((char)c == *p) {
                continue;
            } else {
                break;
            }
        }

        switch (*++p) {
            case 'd':
                ival = va_arg(ap, int *);
                scanf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, float *);
                scanf("%f", dval);
                break;
            case 's':
                sval = va_arg(ap, char *);

                scanf("%s", sval);
                break;
        }
    }
}

int main(void) {
    int a, b;

    char s[MAX];

    minscanf("%d %d", &a, &b);

    printf("a=%d, b=%d\n", a, b);

    minscanf("%s", s);

    printf("s=%s\n", s);

    return 0;
}