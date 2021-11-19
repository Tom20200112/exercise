#include <stdio.h>

#include <ctype.h>

#define BUFSIZE 100
#define SIZE 10

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

int getch(void) /* get a (possibly pushed-back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

/* getint:  get next integer from input into *pn */
int getint(int *pn) {
    int c, sign;

    while (isspace(c = getch())) /* skip white space */
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c); /* it is not a number */
        return 0;
    }

    /* 注意：+, -之后没有数字也是合法的表示 */
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-') c = getch();

    /* 没有考虑乘法溢出的问题 */
    for (*pn = 0; isdigit(c); c = getch()) *pn = 10 * *pn + (c - '0');

    /* 也没有考虑int值域的最大值和最小值不对称问题 */
    *pn *= sign;
    if (c != EOF) ungetch(c);

    return c;
}

int array[SIZE];

int main(void) {
    int n;
    for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++) {
        ;
    }
    for (n = 0; n < SIZE; n++) {
        printf("array[%d]=%d\n", n, array[n]);
    }

    return 0;
}