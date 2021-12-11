#include <stdio.h>
#include <string.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);

void writelines(char *lineptr[], int nlines);

void qsort(char *lineptr[], int left, int right);

int main(void) {
    int nlines;

    if ((nlines = readlines(lineptr, MAXLINES)) > 0) {
        qsort(lineptr, 0, nlines - 1);

        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("Error: input too big to sort\n");
        return 1;
    }

    return 0;
}