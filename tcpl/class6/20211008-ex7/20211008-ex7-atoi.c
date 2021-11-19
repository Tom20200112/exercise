#include <stdio.h>
#include <string.h>

#define MAX 1000

int atoi(char s[]);

int main(void) {
    char s[MAX];
    scanf("%s", s);

    printf("The number you have inputed is %d\n", atoi(s));

    return 0;
}

int atoi(char s[]) {
    int n = 0;

    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] - '0' >= 0 && s[i] - '0' <= 9) {
            n = 10 * n + (s[i] - '0');
        }
    }

    return n;
}