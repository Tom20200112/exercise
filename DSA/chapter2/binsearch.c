#include <stdio.h>
#include <stdlib.h>

int *A;

int binsearch(int *A, int e, int lo, int hi) {
    while (1 < hi - lo) {
        int mi = (hi + lo) >> 1;

        if (e < A[mi]) {
            hi = mi;
        } else {
            lo = mi;
        }
    }

    if (e == A[lo]) {
        return lo;
    } else if (e < A[lo]) {
        return lo - 1;
    }
    return lo;
}

void printArray(int *A, int size) {
    for (int i = 0; i < size - 1; i++) {
        printf("%4d", A[i]);
    }

    printf("%4d\n", A[size - 1]);
}

int main(void) {
    int n, e;
    printf("The size of array: ");
    scanf("%d", &n);

    A = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        scanf("%d", &A[i]);
    }

    printf("The number to search: ");
    scanf("%d", &e);

    printArray(A, n);
    printf("%d\n", binsearch(A, e, 0, n));

    return 0;
}