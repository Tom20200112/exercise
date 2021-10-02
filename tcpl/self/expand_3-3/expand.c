#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define MAXSTRING 1000

int expand(char s1[], char s2[])
{
    int i, j, k;
    for (i, j = 0; i < strlen(s1); i++) {
        if (s1[i] == "-" && i >= 1 && s1[i + 1] > s1[i - 1]) {
            for (k = 1; k <= (s1[i + 1] - s1[i - 1]); k++) {
                j++;
                s2[j] = s1[i - 1] + k;
            }

        } else if (s1[i] != "-") {
            s2[j] = s1[i];
            i++;
            j++;
        }
    }
}