#include <stdio.h>

#define MAXLINE 1000 /* maximum input line length */

int index2(char line[], int max);
int strindex(const char source[], const char searchfor[]);

const char Pattern[] = "ould"; /* pattern to search for */

int main(void) {
    char line[MAXLINE];
    int found = 0;

    while (index2(line, MAXLINE) > 0) {
        if (strindex(line, Pattern) >= 0) {
            printf("%s", line);
            found++;
        }
    }

    return found;
}