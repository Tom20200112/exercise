#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MXLEN 100

/* trim:  remove trailing blanks, tabs, newlines */
int trim(char s[]) {
    int n;

    assert(s != NULL);

    for (n = strlen(s) - 1; n >= 0; n--) {
        if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n') break;
    }

    s[n + 1] = '\0';
    return n;
}

int main(void) {
    char s[] = "I love programming!             ";
    trim(s);

    printf("s=%s\n", s);

    return 0;
}