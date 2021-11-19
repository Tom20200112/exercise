#include <stdio.h>

/* swap:  interchange v[i] and v[j] */
void swap(int v[], int i, int j) {
    int temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* qsort:  sort v[left]...v[right] into increasing order */
void qsort(int v[], int left, int right) {
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    swap(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
    {
        if (v[i] < v[left]) swap(v, ++last, i);
    }
    swap(v, left, last); /* restore partition  elem */

    qsort(v, left, last - 1);
    qsort(v, last + 1, right);
}

int main(void) {
    int n;
    printf("Please input the size of array: ");
    scanf("%d", &n);

    int data[n];

    for (int i = 0; i < n; ++i) {
        scanf("%d", &data[i]);
    }

    qsort(data, 0, n - 1);

    for (int i = 0; i < n; ++i) {
        printf("%d ", data[i]);
    }

    printf("\n");

    return 0;
}