#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 200

char *line1, *line2;

int linecount = 0;

int main(int argc, char *argv[]) {
    FILE *fp1, *fp2;

    char *prog = argv[0];

    if (argc != 3) {
        fprintf(stderr, "error: not comparing two files\n");
        exit(1);
    }

    if ((fp1 = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: can't open %s\n", prog, argv[1]);
        exit(2);
    }

    if ((fp2 = fopen(argv[2], "r")) == NULL) {
        fprintf(stderr, "%s: can't open %s\n", prog, argv[2]);
        exit(2);
    }

    line1 = (char *)malloc(sizeof(char) * MAXLINE);
    line2 = (char *)malloc(sizeof(char) * MAXLINE);

    while ((line1 = fgets(line1, MAXLINE, fp1))) {
        linecount++;

        if (strcmp((line2 = fgets(line2, MAXLINE, fp2)), line1) == 0) {
            continue;
        }

        printf("%d %s:%s%d %s:%s", linecount, argv[1], line1, linecount, argv[2], line2);
        exit(0);
    }

    if ((line2 = fgets(line2, MAXLINE, fp2))) {
        linecount++;
        printf("%d %s:%s%d %s:%s", linecount, argv[1], line1, linecount, argv[2], line2);
        exit(0);
    }

    return 0;
}