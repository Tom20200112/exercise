#include <stdio.h>
#include <string.h>

void reverse(char *t) {
    int c, i, j;

    for (i = 0, j = strlen(t) - 1; i < j; i++, j--) {
        c = t[i];
        t[i] = t[j];
        t[j] = c;
    }
}

int main(void) {
    char s[] = "Hello everyone!";
    reverse(&s[4]);

    printf("%s", s);
    printf("\n");

    return 0;
}