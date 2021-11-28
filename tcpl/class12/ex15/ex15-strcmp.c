#include <stdio.h>

/* strcmp:  return <0 if s<t, 0 if s==t, >0 if s>t */
int mystrcmp(char *s, char *t) {
    for (; *s == *t; s++, t++)
        if (*s == '\0') return 0;
    return *s - *t;
}

int main(void) {
    printf("%d\n", mystrcmp("hello kitty", "hello kitty"));
    printf("%d\n", mystrcmp("hello kitty1", "hello kitty2"));
    printf("%d\n", mystrcmp("hello kitty", "Hello kitty"));

    return 0;
}