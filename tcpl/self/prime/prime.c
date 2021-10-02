#include <stdio.h>

int main(void)
{
    int number[1001];
    int i, j, k;

    number[0] = 0;
    number[1] = 0;

    for (i = 2; i < 1001; i++) {
        number[i] = 1;
    }

    for (i = 0; i < 1001; i++) {
        if (number[i] == 1) {
            for (j = i + i; j < 1001; j += i) {
                if (number[j] == 1) {
                    number[j] = 0;
                }
            }
        }
    }

    for (k = 0; k < 1001; k++) {
        if (number[k] == 1) {
            printf("%d, ", k);
        }
    }

	printf("\n");
    return 0;
}
