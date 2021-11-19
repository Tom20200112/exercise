#include <stdio.h>

enum escapes { BELL = '\a', BACKSPACE = '\b', TAB = '\t', NEWLINE = '\n', VTAB = '\v', RETURN = '\r' };

int main(void) {
    int escapes[6];

    escapes[0] = BELL;
    escapes[1] = BACKSPACE;
    escapes[2] = TAB;
    escapes[3] = NEWLINE;
    escapes[4] = VTAB;
    escapes[5] = RETURN;

    for (int i = 0; i < 6; ++i) {
        printf("%d\n", escapes[i]);
    }

    return 0;
}