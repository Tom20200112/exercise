#include <stdio.h>

#define MAXLEN 10

void swap(int a, int b) {
    int temp;
    temp = a;
    a = b;
    b = temp;
}

int main(void) {
    int a = 1, b = 2;
    printf("before swap a=%d b=%d\n", a, b);

    swap(a, b);
    printf("after swap a=%d b=%d\n", a, b);

    return 0;
}