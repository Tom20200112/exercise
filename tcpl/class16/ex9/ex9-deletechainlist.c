#include <stdio.h>
#include <stdlib.h>

struct lnode {
    int data;
    struct lnode *next;
};

struct lnode *lalloc(void) {
    return (struct lnode *)malloc(sizeof(struct lnode));
}

void printlist(struct lnode *plist) {
    struct lnode *pnode;
    pnode = plist;
    while (pnode != NULL) {
        printf("%3d ", pnode->data);
        pnode = pnode->next;
    }

    printf("\n");
}

struct lnode *getmiddle(struct lnode *plist, int n) {
    struct lnode *p;

    int middle;
    middle = (n + 1) / 2;
    p = plist;

    while (--middle > 0) {
        p = p->next;
    }

    return p;
}

struct lnode *buildlist(int n) {
    struct lnode *plist, *p;
    plist = NULL;

    while (n) {
        p = lalloc();
        p->data = n;
        p->next = plist;
        plist = p;
        n--;
    }

    return plist;
}

int main(int argc, char *argv[]) {
    struct lnode *plist, *pmid, *pre;

    int n;

    if (argc != 2 || (n = atoi(argv[1])) <= 0) {
        printf("Usage: ./ex9-deletechainlist n\n");
    }

    plist = buildlist(n);

    pmid = getmiddle(plist, n);

    pre = plist;

    while (pre != NULL && pre->next != pmid) {
        pre = pre->next;
    }

    if (pre != NULL) {
        pre->next = pmid->next;
        free(pmid);
    }

    printlist(plist);

    return 0;
}