#include <assert.h>
#include <stdio.h>

/* strlen:  return length of string s */
int mystrlen(const char *s) {
    const char *p = s;

    assert(s != NULL);
    while (*p != '\0') p++;

    return p - s;
}

int main(void) {
    char c[] = "Hello friends!";
    int len;
    len = mystrlen(c);
    printf("len=%d\n", len);

    return 0;
}