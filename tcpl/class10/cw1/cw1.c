#include <stdio.h>

#define MAX 1000

char aa[MAX];
int i = 0;

void itoad(int n, char array[]) {
    if (n < 0) {
        array[0] = '-';
        i++;
        n = -n;
    }
    if (n / 10) {
        itoad(n / 10, array);
    }

    array[i++] = (n % 10 + '0');
}

int main(void) {
    int a;
    printf("please input an integer: ");

    scanf("%d", &a);
    itoad(a, aa);
    aa[i] = '\0';

    printf("array=%s\n", aa);

    return 0;
}

/* printd:  print n in decimal */
void printd(int n) {
    if (n < 0) {
        putchar('-');
        n = -n;
    }

    if (n / 10) printd(n / 10);

    putchar(n % 10 + '0');
}
