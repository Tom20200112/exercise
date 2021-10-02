#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INSPACE 1
#define OUTSPACE 0

int main(void)
{
    int n;
    int offset = 0;
    int c;
    int i;
    int state = INSPACE;
    int nspace = 0;

    scanf("%d", &n);
    assert(n >= 0);

    while ((c = getchar()) != EOF) {
        if (state == OUTSPACE && c != ' ') {
            putchar(c);
            if (c == '\t') {
                offset += n - (offset % n) - 1;
            }
            offset += 1;
            if (c == '\n') {
                offset = 0;
            }
        } else if (state == INSPACE && c == ' ') {
            offset += 1;
            nspace += 1;
            if (offset % n == 0) {
                printf("%c", '\t');
                nspace = 0;
            }
        } else if (state == INSPACE && c != ' ') {
            state = OUTSPACE;
            for (i = 0; i < nspace; i++) {
                printf("%c", ' ');
            }
            putchar(c);
            nspace = 0;
            offset += 1;
            if (c == '\n') {
                offset = 0;
            }
        } else if (state == OUTSPACE && c == ' ') {
            state = INSPACE;
            nspace += 1;
            offset += 1;
            if (c == '\n') {
                offset = 0;
            }
        }
    }

    return 0;
}