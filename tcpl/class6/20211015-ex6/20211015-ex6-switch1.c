#include <stdio.h>

int main(void) {
    int a, num = 0;
    scanf("%d", &a);

    switch (a) {
        case 0:
            num += 0;
        case 1:
            num += 1;
        case 2:
            num += 2;
        case 3:
            num += 3;
        case 4:
            num += 4;
        case 5:
            num += 5;
        case 6:
            num += 6;
    }

    printf("num=%d\n", num);

    return 0;
}