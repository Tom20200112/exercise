#include <stdio.h>
#include <string.h>

/* swap:  interchange v[i] and v[j] */
void swap(char *v[], int i, int j) {
    char *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* qsort:  sort v[left]...v[right] into increasing order */
void shellsort(char *v[], int n) {
    int gap, i, j;

    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; ++i) {
            for (j = i - gap; j >= 0 && strcmp(v[j], v[j + gap]) > 0; j -= gap) {
                swap(v, j, j + gap);
            }
        }
    }
}