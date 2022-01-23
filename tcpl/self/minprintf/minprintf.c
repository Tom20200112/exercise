#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...) {
    va_list ap; /* points to each unnamed arg in turn */
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt); /* make ap point to 1st unnamed arg */
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        switch (*++p) {
            case 'd':
                ival = va_arg(ap, int);
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++) putchar(*sval);
                break;
            default:
                putchar(*p);
                break;
        }
    }

    va_end(ap); /* clean up when done */
}

int main(void) {
    float ftest;
    int itest;
    char *stest;

    stest = (char *)malloc(sizeof(char));

    printf("ftest: ");
    scanf("%f", &ftest);

    printf("itest: ");
    scanf("%d", &itest);

    printf("stest: ");
    scanf("\n%s", stest);

    int c = getchar();
    putchar(c);

    minprintf("float is:%f  integer is:%d  string is:%s\n", ftest, itest, stest);
}
