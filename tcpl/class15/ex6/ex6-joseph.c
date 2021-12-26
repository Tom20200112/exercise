#include <stdio.h>

#define LEN 1000

void left(int n, int k) {
    struct player {
        int num;
        int status;
    } cycle[LEN];

    int i, count, remain;

    for (i = 0; i < n; i++) {
        cycle[i].num = i + 1;
        cycle[i].status = 1;
    }

    i = count = 0;
    remain = n;

    while (remain) {
        /* code */
        if (cycle[i].status == 1) {
            if (++count % k == 0) {
                printf("%3d", cycle[i].num);
                cycle[i].status = 0;
                remain--;
            }
        }
        i = (i + 1) % n;
    }

    printf("\n");
    return;
}

#include <ctype.h>

/* atoi:  convert s to integer; version 2 */
int atoi(char s[]) /* 研究一下函数原型 */
{
    int i, n, sign;

    for (i = 0; isspace(s[i]); i++) /* skip white space */
        ;
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-') /* skip sign */
        i++;
    for (n = 0; isdigit(s[i]); i++) n = 10 * n + (s[i] - '0');
    return sign * n;
}

int main(int argc, char *argv[]) {
    int n, k, leftno;

    if (argc != 3 || (n = atoi(argv[1])) <= 0 || (k = atoi(argv[2])) <= 0) {
        printf("Usage: ./game n k\n");
        return 1;
    }
    if (n > LEN) {
        printf("n is too large, it should be less than %d", LEN);
    }

    left(n, k);

    return 0;
}