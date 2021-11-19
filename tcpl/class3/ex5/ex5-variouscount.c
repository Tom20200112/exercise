#include <stdio.h>

#define MAX_NUMBER 10

int main(void)
{
    int digit[MAX_NUMBER];
    int nblank = 0;
    int nother = 0;
    int c;

    for (int i = 0; i < MAX_NUMBER; i++) {
        digit[i] = 0;
    }

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            ++nblank;
        } else if (c - '0' >= 0 && c - '0' <= 9) {
            ++digit[c - '0'];
        } else {
            ++nother;
        }
    }

    printf("\ndigits=");
    for (int i = 0; i < MAX_NUMBER; i++) {
        printf("%d ", digit[i]);
    }

    printf("\nblanks=%d", nblank);
    printf("\nothers=%d", nother);

    return 0;
}