#include <stdio.h>
#include <string.h>

#define MAX 1000

char s[MAX];
char t[MAX];

/* getline:  read a line into s, return length  */
void get_line(char s[], int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) s[i] = c;

    s[i] = '\0';
}

/* strindex:  return index of t in s, -1 if none */
static int strindex(const char s[], const char t[]) {
    int i, j, k;

    for (i = 0; s[i] != '\0'; i++) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0') return i;
    }

    return -1;
}

void stringfilter(char s[], char t[]) {
    int l;

    while (strindex(s, t) != -1) {
        for (l = strindex(s, t); l <= strlen(s) - strlen(t); l++) {
            s[l] = s[l + strlen(t)];
        }
        s[l] = '\0';
    }
}

int main(void) {
    printf("Please input a string: \n");
    get_line(s, MAX);

    printf("Please input another string: \n");
    get_line(t, MAX);

    stringfilter(s, t);

    printf("Filtered s is: %s\n", s);

    return 0;
}