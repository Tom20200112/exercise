#include <stdio.h>

#define INWORD 1
#define OUTWORD 0

int main(void) {
    int state = OUTWORD;
    int c;

    char letter[26];
    for (int i = 0; i < 26; i++) {
        letter[i] = 0;
    }

    while ((c = getchar()) != EOF) {
        if (state == OUTWORD) {
            if (c != ' ' && c != '\t' && c != '\n') {
                state = INWORD;
                if (c - 'a' >= 0 && c - 'a' <= 25) {
                    c = 'A' + c - 'a';
                    letter[c - 'A']++;
                }
            }
        } else {
            if (c - 'A' >= 0 && c - 'A' <= 25) {
                c = 'a' + c - 'A';
            } else if (c == ' ' || c == '\t' || c == '\n') {
                state = OUTWORD;
            }
        }

        putchar(c);
    }

    for (int j = 0; j < 26; j++) {
        printf("%d, ", letter[j]);
    }

    printf("\n");
    return 0;
}