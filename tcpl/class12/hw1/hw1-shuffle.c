#include <stdio.h>

#define MAX 100

char input[MAX];

char T[MAX];

int position;

void shuffle(char s[], int m) {
    char *p1, *p2;
    p1 = s;
    p2 = &s[m];

    char *temp;
    temp = T;

    int i;

    for (i = 0; i < m; ++i) {
        temp[i] = p1[i];
    }
    temp[i] = '\0';

    for (i = 0; p2[i] != '\0'; i++) {
        p1[i] = p2[i];
    }

    int j = 0;

    for (; p1[i + j] != '\0'; j++) {
        p1[i + j] = temp[j];
    }
}

int main(void) {
    printf("Please input a string: ");
    scanf("%s", input);

    printf("Please input a number: ");
    scanf("%d", &position);

    shuffle(input, position);

    printf("%s\n", input);

    return 0;
}