#include <stdio.h>
#define MAXLEN 10

void insertsort(int s[], int len)
{
    int i, j, temp;

    for (i = 0; s[i] != '\0' && i < len; i++) {
        for (j = i; j > 0; j--) {
            if (s[j] < s[j - 1]) {
                temp = s[j];
                s[j] = s[j - 1];
                s[j - 1] = temp;
            }
        }
    }
}

int main(void)
{
    int data1[MAXLEN] = { 10, 7, 29, 14, 33, 55, 22, 4, 56, 234 };
    int data2[MAXLEN] = { 1, 2, 489, 0, 294, 3, 43, 5, 7, 2 };
    int data3[MAXLEN] = { 46, 128, 19, 38, 1, 4, 29, 31, 239, 6 };
    int i;
    insertsort(data1, MAXLEN);
    insertsort(data2, MAXLEN);
    insertsort(data3, MAXLEN);

    for (i = 0; i < MAXLEN; i++) {
        printf("%d, ", data1[i]);
    }
    printf("\n");

    for (i = 0; i < MAXLEN; i++) {
        printf("%d, ", data2[i]);
    }
    printf("\n");

    for (i = 0; i < MAXLEN; i++) {
        printf("%d, ", data3[i]);
    }
    printf("\n");

    return 0;
}