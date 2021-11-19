#include <stdio.h>
#include <string.h>

#define MAX 1000

/* reverse:  reverse string s in place */
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
void itoa(int n, char s[]) /* 研究一下函数原型 */
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */

    i = 0;
    do {
        /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0);   /* delete it */

    if (sign < 0) s[i++] = '-';
    s[i] = '\0';

    reverse(s);
}

char s[MAX];

int main(void) {
    for (int i = 0; i < MAX; ++i) {
        s[i] = 0;
    }

    printf("Please input an integer: ");

    int n;
    scanf("%d", &n);

    itoa(n, s);

    printf("%s\n", s);

    return 0;
}