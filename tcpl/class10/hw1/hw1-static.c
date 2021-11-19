#include <stdio.h>
int c = 0;

int foo(int x);

int main(void) {
    int result = 6;

    result = foo(result);
    printf("c=%d, result=%d\n", c, result);

    result = foo(result);
    printf("c=%d, result=%d\n", c, result);
}

int foo(int x) {
    int a;
    static int b = 3;

    a = 2 * x;
    c += (++a, b++);

    return a + b + c;
}