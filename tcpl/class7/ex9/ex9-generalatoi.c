#include <ctype.h>
#include <stdio.h>

#define MAX 1000

/* atoi:  convert s to integer; version 2 */
int atoi(char s[]) /* 研究一下函数原型 */
{
    int i, n, sign;

    for (i = 0; isspace(s[i]); i++) /* skip white space */
        ;
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-') /* skip sign */
        i++;
    for (n = 0; isdigit(s[i]); i++) n = 10 * n + (s[i] - '0');
    return sign * n;
}

char s[MAX];

int main(void) {
    printf("Type in a string: ");
    scanf("%s", s);

    printf("Inputed integer value is: %d\n", atoi(s));

    return 0;
}