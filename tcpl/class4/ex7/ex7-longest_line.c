#include <stdio.h>

#define MAXLINE 1000

int getaline(char line[], int maxline) {
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n' && i < maxline - 1) {
        line[i] = c;
        ++i;
    }

    if (c == '\n') {
        line[i++] = '\n';
        line[i] = '\0';
    }

    return i;
}

void copy(char to[], char from[]) {
    int j = 0;

    for (; from[j] != '\0'; j++) {
        to[j] = from[j];
    }

    to[j] = '\0';
}

int main(void) {
    char line[MAXLINE], maxline[MAXLINE];
    int maxlen = 0;
    int len;

    while ((len = getaline(line, MAXLINE)) > 0) {
        if (len > maxlen) {
            maxlen = len;

            copy(maxline, line);
        }
    }

    if (maxlen > 0) {
        printf("The longest line is: %s\nlength=%d\n", maxline, maxlen);
    }

    return 0;
}