#include <stdio.h>

void move(char x, char y) { printf("%c->%c\n", x, y); }

void hanoi(int n, char a, char b, char c) {
    if (n == 1) {
        move(a, c);
    } else {
        hanoi(n - 1, a, c, b);
        move(a, c);
        hanoi(n - 1, b, a, c);
    }
}

int main(void) {
    int m;
    printf("Input the number of disks: ");
    scanf("%d", &m);

    printf("The steps to move %d disks from A to C:\n", m);

    hanoi(m, 'A', 'B', 'C');

    return 0;
}