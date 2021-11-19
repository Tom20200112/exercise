#include <stdio.h>
#include <string.h>

#define MAX 1000

char s[1000];

long long power(long long x, long long y) {
    long long base = 1;

    for (; y > 0; y--) {
        // *= is better than `base = base * x'.
        base *= x;
    }

    return base;
}

void reverse(char s[]) /* 研究一下函数原型 */
{
    int c, i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void itoh(long int n, char s[]) /* 研究一下函数原型 */
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
    {
        n = power(2, 31) + n;
        n += power(2, 31);
    } /* make n positive */

    i = 0;

    do {
        /* generate digits in reverse order */
        s[i++] = (n % 16 >= 0 && n % 16 <= 9) ? (n % 16 + '0') : (n % 16 - 10 + 'a'); /* get next digit */
    } while ((n /= 16) > 0);                                                          /* delete it */

    s[i] = '\0';

    reverse(s);
}

int main(void) {
    int x;
    printf("Please type in an integer: ");
    scanf("%d", &x);

    itoh(x, s);

    printf("%s\n", s);

    return 0;
}