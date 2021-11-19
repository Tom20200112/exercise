#include <assert.h>
#include <stdio.h>

#define INWORD 1
#define OUTWORD 0

int main(void)
{
    int c;
    int nw = 0;
    int state = OUTWORD;

    while ((c = getchar()) != EOF) {
        if (state == OUTWORD) {
            if (c != '\n' && c != ' ' && c != '\t') {
                state = INWORD;
                nw++;
            }
        } else {
            if (c == '\n' || c == ' ' || c == '\t') {
                state = OUTWORD;
            }
        }
    }

    printf("There are %d words\n", nw);
    return 0;
}