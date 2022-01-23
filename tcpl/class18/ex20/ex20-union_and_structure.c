#include <stdio.h>

#define NSYM 3

enum UTYPE { INT, FLOAT, STRING };

struct info {
    char *name;
    int flags;
    enum UTYPE utype;

    union {
        int ival;
        float fval;
        char *sval;
    } u;

} symtab[NSYM];

void buildsymtab() {
    symtab[0].name = "laowang";
    symtab[0].flags = 1;
    symtab[0].utype = INT;
    symtab[0].u.ival = 1;

    symtab[1].name = "laozhang";
    symtab[1].flags = 2;
    symtab[1].utype = FLOAT;
    symtab[1].u.fval = 2.0;

    symtab[2].name = "laoli";
    symtab[2].flags = 3;
    symtab[2].utype = STRING;
    symtab[2].u.sval = "Hello laowang";
}

int main(void) {
    int i;
    printf("sizeof(struct info)=%ld\n", sizeof(struct info));
    printf("sizeof(symtab)=%ld\n)", sizeof(symtab));

    buildsymtab();

    for (i = 0; i < NSYM; i++) {
        printf("%15s ", symtab[i].name);
        printf("%3d ", symtab[i].flags);

        switch (symtab[i].utype) {
            case INT: {
                printf("%15d \n", symtab[i].u.ival);
                break;
            }

            case FLOAT: {
                printf("%15.1f \n", symtab[i].u.fval);
                break;
            }

            case STRING: {
                printf("%15s \n", symtab[i].u.sval);
                break;
            }
        }
    }

    return 0;
}