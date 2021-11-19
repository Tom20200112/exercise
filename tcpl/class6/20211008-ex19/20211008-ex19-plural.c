#include <stdio.h>
#define N 5

int main(void) {
    int i;
    for (i = 0; i < N; i++) {
        printf("You have %d item%s.\n", i, i == 1 ? "" : "s");
    }

    return 0;
}