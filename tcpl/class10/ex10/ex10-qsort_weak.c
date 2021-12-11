#include <stdio.h>

#define MAX 100

/* swap:  interchange v[i] and v[j] */
void swap(int v[], int i, int j) {
    int temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void qsort(int v[], int left, int right) {
    int i = (left + right) / 2, scanleft = left, scanright = right;

    if (left >= right) {
        return;
    }

    while (scanleft < scanright) {
        for (; v[scanleft] <= v[i] && scanleft <= i; ++scanleft)
            ;

        if (scanleft < i) {
            swap(v, scanleft, i);
            i = scanleft;
        }

        for (; v[scanright] >= v[i] && scanright >= i; --scanright)
            ;

        if (scanright > i) {
            swap(v, scanright, i);
            i = scanright;
        }
    }

    qsort(v, 0, i - 1);
    qsort(v, i + 1, right);
}

int main(void) {
    int n;
    int array[MAX];

    printf("How many numbers to sort: ");

    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        printf("Please input number %d: ", i + 1);
        scanf("%d", &array[i]);
    }

    qsort(array, 0, n - 1);

    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}