#include <stdio.h>

int main(void) {
    int a, num = 0;
    scanf("%d", &a);

    switch (a) {
        case 0:
            num += 0;
            break;
        case 1:
            num += 1;
            break;
        case 2:
            num += 2;
            break;
        case 3:
            num += 3;
            break;
        case 4:
            num += 4;
            break;
        case 5:
            num += 5;
            break;
        case 6:
            num += 6;
            break;
    }

    printf("num=%d\n", num);

    return 0;
}