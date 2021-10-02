#include <stdio.h>

#define MAXLENGTH 1000

int stringlen(char s[]) {
    int i = 0;

    while (s[i] != '\0') {
        ++i;
    }

    return i;
}

int main() {
    char mystring[MAXLENGTH];
    scanf("%s", mystring);

    printf("The length of my string is %d\n", stringlen(mystring));

    return 0;
}