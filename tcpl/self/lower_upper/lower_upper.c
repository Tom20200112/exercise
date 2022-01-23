#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int c;

    if (argc != 2 || (strcmp(argv[argc - 1], "u2l") != 0 && strcmp(argv[argc - 1], "l2lu") != 0)) {
        printf("Error: u2l for upper to lower, l2u for lower to upper\n");
        return -1;
    } else if (strcmp(argv[argc - 1], "u2l") == 0) {
        while ((c = getchar()) != EOF) {
            putchar(tolower(c));
        }

        return 0;
    } else {
        while ((c = getchar()) != EOF) {
            putchar(toupper(c));
        }

        return 1;
    }
}