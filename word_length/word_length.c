#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INWORD 1
#define OUTWORD 0

const int Max_length = 10;

int main(void)
{
    int length[Max_length];
    memset(length, 0, sizeof(int) * Max_length);

    int c;
    int state = OUTWORD;
    int nchar = 0;
    while ((c = getchar()) != EOF) {
        if ((state == OUTWORD) && !isspace(c)) {
            state = INWORD;
            nchar++;
        } else if ((state == INWORD) && isspace(c)) {
            state = OUTWORD;
            if (nchar < Max_length) {
                length[nchar]++;
            } else {
                length[0]++;
            }

            nchar = 0;
        } else if (state == INWORD && !isspace(c)) {
            nchar++;
        }
    }

    printf("wordlengths =");
    for (int i = 0; i < Max_length; i++) {
        printf(" %d", length[i]);
    }

    printf("\n");
    printf("words of more than 9 characters: ");

    for (int i = 1; i <= length[0]; i++) {
        printf("#");
    }

    printf("\n");

    for (int i = 1; i < Max_length; i++) {
        printf("words of %d character(s): ", i);
        for (int j = 1; j <= length[i]; j++) {
            printf("#");
        }
        printf("\n");
    }

    return 0;
}