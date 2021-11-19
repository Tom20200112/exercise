#include <stdio.h>

int isLegalANSI(char s[]);

int main(void) {
    char s[100];

    scanf("%s", s);

    if (isLegalANSI(s)) {
        printf("This is a legal variable name\n");
    } else {
        printf("This is not a legal variable name\n");
    }

    return 0;
}

int isLegalANSI(char s[]) {
    int i = 0;

    if (s[i] - '0' >= 0 && s[i] - '0' <= 9) {
        return 0;
    }

    for (; s[i] != '\0'; i++) {
        if (!((s[i] - '0' <= 9 && s[i] - '0' >= 0) || (s[i] - 'a' >= 0 && s[i] - 'a' <= 25) ||
              (s[i] - 'A' >= 0 && s[i] - 'A' <= 25) || s[i] == '_')) {
            return 0;
        }
    }

    if (i >= 31) {
        return 0;
    }

    return 1;
}
