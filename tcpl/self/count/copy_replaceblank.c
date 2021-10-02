#include <assert.h>
#include <stdio.h>

#define INBLANK 1
#define OUTBLANK 0

int main(void)
{
    int c;
    int state = OUTBLANK;
    while ((c = getchar()) != EOF) {
        if (state == OUTBLANK) {
            putchar(c);
            if (c == ' ') {
                state = INBLANK;
            }
        } else {
            assert(state == INBLANK);
            if (c != ' ') {
                putchar(c);
                state = OUTBLANK;
            }
        }
    }

    return 0;
}