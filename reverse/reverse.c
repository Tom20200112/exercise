#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int get_line(char line[], int maxline);
void reverse(char* s);

int main(void)
{
    int len;
    char line[MAXLINE];

    while ((len = get_line(line, MAXLINE)) > 0) {
        reverse(line);
        printf("%s", line);
    }

    return 0;
}

void reverse(char* s)
{
    int len;
    int i;
    char x;

    len = strlen(s);

    for (i = 0; 2 * i < len; i++) {
        x = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = x;
    }
}

int get_line(char s[], int lim)
{
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