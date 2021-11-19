#include <stdio.h>

#define NUM 15

int binsearch(int x, int v[], int n) /* 研究一下函数原型 */
{
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x < v[mid])
            high = mid - 1;
        else if (x > v[mid])
            low = mid + 1;
        else /* found match */
            return mid;
    }

    return -1; /* no match */
}

int main(void) {
    int data[] = {1, 2, 4, 10, 29, 33, 35, 48, 54, 56, 61, 62, 63, 70, 79};

    int x, index;
    scanf("%d", &x);
    index = binsearch(x, data, NUM);

    if (index != -1) {
        printf("Index=%d\n", index);

    } else {
        printf("%d is not found\n", x);
    }

    return 0;
}