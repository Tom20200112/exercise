// 1-18
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int get_line(char line[], int maxline);

int main(void)
{
    char line[MAXLINE];
    int len;
    int j;

    while ((len = get_line(line, MAXLINE)) > 0) {
        for (j = len - 1; isspace(line[j]) && j >= 0; j--)
            ;
        line[j + 1] = '\n';
        line[j + 2] = '\0';
        if (j >= 0) {
            printf("%s", line);
        }
    }

    return 0;
}

/*
 * get_line: read a line into s, return length
 */
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
