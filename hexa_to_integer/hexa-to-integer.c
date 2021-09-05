#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXDIGIT 100

int htoi(char s[]);

int main(void)
{
    char s[MAXDIGIT];

    int c;
    int i = 0;

    while ((c = getchar()) != EOF) {
        s[i] = c;
        i++;
    }
    s[i] = '\0';

    printf("%d", htoi(s));
    printf("%c", '\n');

    return 0;
}

int htoi(char s[])
{
    int i;
    int n = 0;

    for (i = 2; i < strlen(s); ++i) {
        if ((int)s[i] >= '0' && (int)s[i] <= '9')
            n = 16 * n + (int)s[i] - '0';
        else if ((int)s[i] >= 'a' && (int)s[i] <= 'f')
            n = 16 * n + 10 + (int)s[i] - 'a';
        else if ((int)s[i] >= 'A' && (int)s[i] <= 'F')
            n = 16 * n + 10 + (int)s[i] - 'A';
    }

    return n;
}