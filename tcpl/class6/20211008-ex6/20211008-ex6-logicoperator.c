#include <stdio.h>

int main(void) {
    int i = 0, j = 0;
    if (++i == 1 || ++j > 0) {
        printf("i=%d, j=%d\n", i, j);
    }

    i = j = 0;
    if (++i != 1 || ++j > 0) {
        printf("i=%d, j=%d\n", i, j);
    }

    return 0;
}