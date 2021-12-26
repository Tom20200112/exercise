#include <stdio.h>
#include <string.h>

#define MAX 200

/* reverse:  reverse string s in place */
void reverse(char s[]) /* 研究一下函数原型 */
{
    int c, i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int main(void) {
    char a[MAX + 1];
    char b[MAX + 1];

    int carry = 0;
    int i, j;

    printf("Please input an addend: ");
    scanf("%s", a);

    printf("Please input another addend: ");
    scanf("%s", b);

    reverse(a);
    reverse(b);

    for (i = 0; a[i] != '\0' && b[i] != '\0'; ++i) {
        if (a[i] - '0' + b[i] - '0' + carry > 9) {
            b[i] = a[i] - '0' + b[i] + carry - 10;
            carry = 1;
        } else {
            b[i] = a[i] - '0' + b[i] + carry;
            carry = 0;
        }
    }

    if (a[i] == '\0' && b[i] == '\0') {
        if (carry != 0) {
            b[i] = carry + '0';
            b[i + 1] = '\0';
        }
    } else if (a[i] == '\0' && b[i] != '\0') {
        for (j = i; b[j] != '\0'; j++) {
            if (0 + b[j] - '0' + carry > 9) {
                b[j] = 0 + b[j] + carry - 10;
                carry = 1;
            } else {
                b[j] = 0 + b[j] + carry;
                carry = 0;
            }
        }
        if (carry != 0) {
            b[j] = carry + '0';
            b[j + 1] = '\0';
        }
    } else if (a[i] != '\0' && b[i] == '\0') {
        for (j = i; a[j] != '\0'; j++) {
            if (0 + a[j] - '0' + carry > 9) {
                b[j] = 0 + a[j] + carry - 10;
                carry = 1;
            } else {
                b[j] = 0 + a[j] + carry;
                carry = 0;
            }
        }

        b[j] = '\0';

        if (carry != 0) {
            b[j] = carry + '0';
            b[j + 1] = '\0';
        }
    }

    reverse(b);

    printf("sum = %s\n", b);

    return 0;
}