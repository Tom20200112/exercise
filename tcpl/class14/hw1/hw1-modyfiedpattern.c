#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

/* strindex:  return index of t in s, -1 if none */
static int word_in_string(const char *string, const char *word) {
    int i = 0;
    char *p;
    p = word;

    /*for (i = 0; s[i] != '\0'; i++) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0') {
            return i;
        }
    }*/

    while (*string != '\0') {
        while (*p == *string) {
            p++;
        }

        if (*p == '\0' && p != word) {
            i++;
        }

        p = word;

        string++;
    }

    return i;
}

int mygetline(char line[], int maxline) {
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n' && i < maxline - 1) {
        line[i] = c;
        ++i;
    }

    if (c == '\n') {
        line[i++] = '\n';
    }
    line[i] = '\0';

    return i;
}

/* find: print lines that match pattern from 1st arg */
int main(int argc, char *argv[]) {
    char line[MAXLINE];
    long lineno = 0;
    int c, except = 0, number = 0, found = 0, wordcount = 1;

    while (--argc > 0 && (*++argv)[0] == '-') {
        while (c = *++argv[0]) {
            switch (c) {
                case 'x':
                    except = 1;
                    break;
                case 'n':
                    number = 1;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    wordcount = c - '0';
                    break;
                default:
                    printf("find: illegal option %c\n", c);
                    argc = 0;
                    found = -1;
                    break;
            }
        }
    }

    if (argc != 1)
        printf("Usage: find -x -n pattern\n");
    else {
        while (mygetline(line, MAXLINE) > 0) {
            lineno++;
            if ((word_in_string(line, *argv) - wordcount >= 0) != except) {
                if (number) printf("%ld:", lineno);
                printf("%s", line);
                found++;
            }
        }
    }

    return found;
}