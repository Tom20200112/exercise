#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int n;
    int offset = 0;
    int c;
    int i;

    scanf("%d", &n);
    assert(n >= 0);

    while ((c = getchar()) != EOF) {
        if (c == '\t') {
            for (i = 0; i < (n - (offset % n)); i++) {
                printf("%s", " ");
            }
            offset += (n - (offset % n) - 1);
        } else {
            putchar(c);
        }
        offset++;
        if (c == '\n') {
            offset = 0;
        }
    }

    return 0;
}