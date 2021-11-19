#include <stdio.h>

#define MAX 100

void squeeze(char s[], int c) {
    int i, j;

    for (i = j = 0; s[i] != '\0'; i++)
        if (s[i] != c) s[j++] = s[i];
    s[j] = '\0';
}

int main(void) {
    char string[MAX];
    scanf("%s", string);

    int c;
    while ((c = getchar()) == '\n' || c == '\t' || c == ' ') {
        ;
    }

    squeeze(string, c);
    printf("%s\n", string);

    return 0;
}