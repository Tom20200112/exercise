#include <stdio.h>

/* shellsort:  sort v[0]...v[n-1] into increasing order */
void shellsort(int v[], int n) {
    int gap, i, j, temp;

    for (gap = n / 2; gap > 0; gap /= 2)
        for (i = gap; i < n; i++)
            for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap) {
                temp = v[j];
                v[j] = v[j + gap];
                v[j + gap] = temp;
            }
}

int main(void) {
    int n;
    printf("How many numbers to sort?\n ");
    scanf("%d", &n);

    int value[n];

    printf("Please input the list of numbers to sort.\n");

    for (int i = 0; i < n; i++) {
        scanf("%d", &value[i]);
    }

    shellsort(value, n);

    for (int i = 0; i < n; ++i) {
        printf("%d ", value[i]);
    }

    printf("\n");

    return 0;
}