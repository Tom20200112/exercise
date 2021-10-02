#include <stdio.h>

#define MAXLINE 1000

char line[MAXLINE];

int getaline(char s[], int lim);

int main(void) {
    int len, n;

    scanf("%d", &n);

    while ((len = getaline(line, MAXLINE)) > 0) {
        if (len >= n) {
            printf("%s", line);
        }
    }

    return 0;
}

int getaline(char s[], int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
        s[i] = c;
    }

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';

    return i;
}