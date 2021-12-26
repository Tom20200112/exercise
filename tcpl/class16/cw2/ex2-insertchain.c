#include <stdio.h>
#include <stdlib.h>

#define LEN 10

int array[LEN];

struct node {
    int data;
    struct node *next;
};

struct node *lalloc(void) {
    return (struct node *)malloc(sizeof(struct node));
}

struct node *insert(struct node *plist, int data) {
    struct node *p, *q;

    p = plist;

    if (plist->data >= data) {
        p = lalloc();
        p->next = plist;
        p->data = data;

        plist = p;
    } else {
        while (((p->next) != NULL) && (p->next)->data <= data) {
            p = p->next;
        }
        q = lalloc();
        q->data = data;
        if (p->next != NULL) {
            q->next = p->next;
            p->next = q;
        } else {
            q->next = NULL;

            p->next = q;
        }
    }

    return plist;
}

void printlist(struct node *plist) {
    struct node *pnode;

    pnode = plist;

    while (pnode != NULL) {
        printf("%3d ", pnode->data);
        pnode = pnode->next;
    }

    printf("\n");
}

int main(void) {
    printf("Please input 10 integers: ");

    for (int i = 0; i < LEN; ++i) {
        scanf("%d", &array[i]);
    }

    struct node *plist;
    plist = lalloc();
    plist->data = array[0];
    plist->next = NULL;

    for (int i = 1; i < LEN; i++) {
        plist = insert(plist, array[i]);
    }

    printlist(plist);

    return 0;
}