#include <stdio.h>
#define MAXLEN 1000

void stringcat(char s[], char t[]) {
    int i, j;

    i = j = 0;
    while (s[i] != '\0') /* find end of s */
        i++;
    while ((s[i++] = t[j++]) != '\0') /* copy t */
        ;
}

int get_line(char s[], int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        s[i] = c;
    }

    s[i] = '\0';
    return i;
}

int main(void) {
    char s[MAXLEN];
    char t[MAXLEN];

    printf("Please input a string:\n");
    get_line(s, MAXLEN);

    printf("Please input another string:\n");
    get_line(t, MAXLEN);

    stringcat(s, t);

    printf("Concatenated s=%s\n", s);

    return 0;
}