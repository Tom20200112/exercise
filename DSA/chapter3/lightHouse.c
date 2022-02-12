#include <assert.h>
#include <stdio.h>

void merge_x(int A[][2], int lo, int mi, int hi) {
    int lb = mi - lo;

    int B[lb][2];

    for (int i = 0; i < lb; B[i][0] = A[lo + i][0], B[i][1] = A[lo + i][1], i++)
        ;

    int lc = hi - mi;

    for (int i = 0, j = 0, k = 0; j < lb;) {
        if ((k < lc) && (A[mi + k][0] < B[j][0])) A[lo + i][0] = A[mi + k][0], A[lo + i][1] = A[mi + k][1], i++, k++;
        if (lc <= k || B[j][0] <= A[mi + k][0]) A[lo + i][0] = B[j][0], A[lo + i][1] = B[j][1], i++, j++;
    }
}

void mergeSort_x(int A[][2], int lo, int hi) {
    if (hi - lo < 2) {
        return;
    }

    int mi = (hi + lo) >> 1;
    mergeSort_x(A, lo, mi);
    mergeSort_x(A, mi, hi);
    merge_x(A, lo, mi, hi);
}

void merge_y(int A[][2], int lo, int mi, int hi) {
    int lb = mi - lo;

    int B[lb];

    for (int i = 0; i < lb; B[i] = A[lo + i][1], i++)
        ;

    int lc = hi - mi;

    for (int i = 0, j = 0, k = 0; j < lb;) {
        if ((k < lc) && (A[mi + k][1] < B[j])) A[lo + i][1] = A[mi + k][1], i++, k++;
        if (lc <= k || B[j] <= A[mi + k][1]) A[lo + i][1] = B[j], i++, j++;
    }
}

void mergeSort_y(int A[][2], int lo, int hi) {
    if (hi - lo < 2) return;

    int mi = (hi + lo) >> 1;

    mergeSort_y(A, lo, mi);
    mergeSort_y(A, mi, hi);
    merge_y(A, lo, mi, hi);
}

int binSearch_y(int lightHouse[][2], int e, int lo, int hi) {
    int mi;

    while (lo < hi) {
        mi = (hi + lo) >> 1;
        if (e < lightHouse[mi][1]) {
            hi = mi;
        } else {
            lo = mi + 1;
        }
    }

    return --lo;
}

long long int invBetween(int lightHouse[][2], int lo, int mi, int hi) {
    long long int inv = 0;

    mergeSort_y(lightHouse, lo, mi);
    mergeSort_y(lightHouse, mi, hi);

    for (int i = mi; i < hi; i++) {
        inv += mi - binSearch_y(lightHouse, lightHouse[i][1], lo, mi) - 1;
    }

    return inv;
}

long long int invInside(int lightHouse[][2], int lo, int hi) {
    if (hi - lo < 2) return 0;

    int mi = (lo + hi) >> 1;

    return invInside(lightHouse, lo, mi) + invInside(lightHouse, mi, hi) + invBetween(lightHouse, lo, mi, hi);
}

int main(void) {
    int n;
    scanf("%d", &n);

    int lightHouse[n][2];

    for (int i = 0; i < n; i++) {
        scanf("%d", &lightHouse[i][0]);
        scanf("%d", &lightHouse[i][1]);
    }

    mergeSort_x(lightHouse, 0, n);

    printf("%lld\n", (long long int)n * ((long long int)n - 1) / 2 - invInside(lightHouse, 0, n));

    return 0;
}