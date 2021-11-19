#include <stdio.h>

int replacePart(int x, int y, int p, int len) {
    int temp1, temp2, temp3;

    temp1 = ((y & (~(~0 << len))) << (p - len + 1));
    temp2 = (((~0 << (p + 1)) | (~(~0 << (p - len + 1)))));
    temp3 = (x & temp2);

    return (temp3 | temp1);
}

int main(void) {
    int x, y, p, len;
    printf("Please input x: ");
    scanf("%d", &x);

    printf("Please input y: ");
    scanf("%d", &y);

    printf("Please input p: ");
    scanf("%d", &p);

    printf("Please input len: ");
    scanf("%d", &len);

    printf("Replaced x is: %d\n", replacePart(x, y, p, len));

    return 0;
}