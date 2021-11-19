#include <stdio.h>

int main(void)
{
    int c;
    int nc = 0;

    while ((c = getchar()) != EOF) {
        ++nc;
    }

    printf("Input text countains %d characters", nc);

    return 0;
}