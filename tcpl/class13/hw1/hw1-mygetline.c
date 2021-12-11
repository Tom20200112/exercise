#include <stdio.h>

int mygetline(char line[], int maxline) {
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n' && i < maxline - 1) {
        line[i] = c;
        ++i;
    }

    if (c == '\n') {
        line[i++] = '\n';
    }
    line[i] = '\0';

    return i;
}
