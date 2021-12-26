#include <stdio.h>
#include <stdlib.h>

struct lnode {
    int data;
    struct lnode *next;
};

void printlist(struct lnode *plist) {
    struct lnode *pnode;
    pnode = plist;
    while (pnode != NULL) {
        printf("%3d ", pnode->data);
        pnode = pnode->next;
    }

    printf("\n");
}

struct lnode *lalloc(void) {
    return (struct lnode *)malloc(sizeof(struct lnode));
}

int main(int argc, char *argv[]) {
    struct lnode *plist, *p;

    int n;

    if (argc != 2 || (n = atoi(argv[1])) <= 0) {
        printf("Usage: ./ex8-chainlist n\n");
    }

    plist = NULL;

    while (n) {
        p = lalloc();
        p->data = n;
        p->next = plist;
        plist = p;
        n--;
    }

    printlist(plist);

    return 0;
}