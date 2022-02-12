#include <stdio.h>

int letters[26];

int main(void) {
    for (int i = 0; i < 26; ++i) {
        letters[i] = 0;
    }

    int c;

    while ((c = getchar()) != EOF) {
        if (c - 'A' >= 0 && c - 'A' < 26) {
            letters[c - 'A']++;
        } else if (c - 'a' >= 0 && c - 'a' < 26) {
            letters[c - 'a']++;
        }
    }

    for (int i = 0; i < 26; ++i) {
        if (letters[i] > 0) {
            printf("%c: %d\n", 'A' + i, letters[i]);
        }
    }

    return 0;
}