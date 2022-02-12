#include <stdio.h>

void merge(int A[], int lo, int mi, int hi) {
    int *pA = A + lo;
    int lb = mi - lo;

    int B[lb];

    for (int i = 0; i < lb; B[i] = pA[i], i++)
        ;

    int lc = hi - mi;

    int *pC = A + mi;

    for (int i = 0, j = 0, k = 0; j < lb;) {
        if ((k < lc) && (pC[k] < B[j])) pA[i++] = pC[k++];
        if (lc <= k || B[j] <= pC[k]) pA[i++] = B[j++];
    }
}

void mergeSort(int A[], int lo, int hi) {
    if (hi - lo < 2) return;

    int mi = (hi + lo) >> 1;
    mergeSort(A, lo, mi);
    mergeSort(A, mi, hi);
    merge(A, lo, mi, hi);
}

int binSearch(int A[], int e, int lo, int hi) {
    int mi;

    while (lo < hi) {
        mi = (lo + hi) >> 1;
        if (e < A[mi]) {
            hi = mi;
        } else {
            lo = mi + 1;
        }
    }

    return --lo;
}

int main(void) {
    int n, m, a, b, r1, r2;

    scanf("%d", &n);
    scanf("%d", &m);

    int S[n];

    int i;

    for (i = 0; i < n; i++) {
        scanf("%d", &S[i]);
    }

    mergeSort(S, 0, n);

    for (i = 0; i < m; i++) {
        scanf("%d", &a);
        scanf("%d", &b);

        r1 = binSearch(S, a, 0, n);
        r2 = binSearch(S, b, 0, n);

        if (r1 == -1) {
            if (r2 == -1) {
                printf("%d\n", 0);
            } else {
                printf("%d\n", r2 + 1);
            }
        } else if (S[r1] == a) {
            printf("%d\n", r2 - r1 + 1);
        } else {
            printf("%d\n", r2 - r1);
        }
    }

    return 0;
}