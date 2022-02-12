#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000

int size, firstMoreThan2, numEqual;

void delete (char *beads, int lo, int hi) {
    for (int i = hi; i <= size; i++) {
        beads[i - (hi - lo)] = beads[i];
    }
}

void insert(char *beads, int rank, char *c) {
    for (int i = size; i > rank; i--) {
        beads[i] = beads[i - 1];
    }

    beads[rank] = *c;
    beads[++size] = '\0';
}

void check(char *beads) {
    for (int i = 0, j = 0; i < size; i++) {
        for (j = i; j < size && beads[j] == beads[i]; j++)
            ;
        if (j - i >= 3) {
            firstMoreThan2 = i;
            numEqual = j - i;
            return;
        }
    }

    firstMoreThan2 = -1;
    return;
}

void printBeads(char *beads) {
    if (beads[0] == '\0') {
        printf("-\n");
    } else {
        printf("%s\n", beads);
    }
}

void eliminate(char *beads) {
    check(beads);
    while (firstMoreThan2 >= 0) {
        delete (beads, firstMoreThan2, firstMoreThan2 + numEqual);
        check(beads);
    }
}

int main(void) {
    int n, rank;
    char *beads, *c;
    c = malloc(sizeof(char));

    beads = malloc(MAX * sizeof(char));

    for (size = 0; (beads[size++] = getchar()) != '\n';) {
    }
    beads[--size] = '\0';

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &rank);
        scanf("%s", c);

        insert(beads, rank, c);

        eliminate(beads);
        printBeads(beads);
    }

    return 0;
}