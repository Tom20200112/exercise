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
    int min = 1000;
    int second_min = 1000;
    int len;

    char S[MAXLINE];
    char secondS[MAXLINE];
    char line[MAXLINE];

    while ((len = getaline(line, MAXLINE)) > 0) {
        if (len < min) {
            min = len;

            copy(S, line);
        } else if (len < second_min) {
            second_min = len;

            copy(secondS, line);
        }
    }

    printf("The shortest line is: %s\nlength=%d\n", S, min);
    printf("The second shortest line is: %s\nlength=%d\n", secondS, second_min);

    return 0;
}