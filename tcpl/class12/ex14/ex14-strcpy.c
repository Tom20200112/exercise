#include <stdio.h>

#define MAXLEN 100

/* strcpy:  copy t to s; pointer version */
void mystrcpy(char *s, const char *t) {
    while ((*s = *t) != '\0') {
        s++;
        t++;
    }
}

int main(void) {
    char a[MAXLEN];
    char b[] = "I am a string";

    mystrcpy(a, b);
    printf("%s\n", a);

    return 0;
}