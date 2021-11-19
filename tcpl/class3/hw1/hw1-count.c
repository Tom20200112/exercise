#include <stdio.h>

#define INWORD 1
#define OUTWORD 0

int main(void)
{
    int c;
    int nwupper = 0, nwlower = 0, nwother = 0;
    int state = OUTWORD;

    while ((c = getchar()) != EOF) {
        if (state == OUTWORD) {
            if (c != '\n' && c != ' ' && c != '\t') {
                state = INWORD;
                if (c - 'A' >= 0 && c - 'A' <= 25) {
                    nwupper++;
                } else if (c - 'a' >= 0 && c - 'a' <= 25) {
                    nwlower++;
                } else {
                    nwother++;
                }
            }
        } else {
            if (c == '\n' || c == ' ' || c == '\t') {
                state = OUTWORD;
            }
        }
    }

    printf("There are %d words starting with uppercase letter\n", nwupper);
    printf("There are %d words starting with lowercase letter\n", nwlower);
    printf("There are %d words starting with other letters\n", nwother);

    return 0;
}