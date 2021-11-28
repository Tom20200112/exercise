#include <stdio.h>

#define NUM 10

int children[NUM];

int even(int s[NUM]) {
    for (int j = 1; j < NUM; j++) {
        if (s[j] != s[0]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int step = 0;

    printf("Please divide candies to 10 children: ");
    for (int i = 0; i < NUM; i++) {
        scanf("%d", &children[i]);
    }

    while (!even(children)) {
        int temp[NUM];
        for (int i = 0; i < NUM; i++) {
            if (children[i] % 2 == 1) {
                children[i]++;
            }
        }

        for (int i = 0; i < NUM; i++) {
            children[i] /= 2;
        }

        for (int i = 0; i < NUM; ++i) {
            temp[i] = children[i];
        }

        for (int i = 1; i < NUM; i++) {
            children[i] += temp[i - 1];
        }

        children[0] += temp[NUM - 1];
        step += 1;

        printf("Candies at step %d: ", step);
        for (int i = 0; i < NUM; ++i) {
            printf("%8d", children[i]);
        }
        printf("\n");
    }

    printf("Candies finally: ");
    for (int i = 0; i < NUM; ++i) {
        printf("%4d", children[i]);
    }
    printf("\n");

    return 0;
}