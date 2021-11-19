#include <stdio.h>

int main(void)
{
    int c;
    int nl = 0;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            nl++;
        }
    }

    printf("Input text has %d line(s)\n", nl);

    return 0;
}