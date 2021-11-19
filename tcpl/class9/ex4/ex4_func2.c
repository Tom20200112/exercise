#include <stdio.h>

extern int externalvar;

void func2(void) {
    externalvar += 200;

    printf("1 in function2: externalvar=%d\n", externalvar);
}