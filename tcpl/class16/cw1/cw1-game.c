#include <stdio.h>
#include <stdlib.h>

struct player {
    int number;
    struct player *next;
};

void printlist(struct player *plist) {}

struct player *lalloc(void) {
    return (struct player *)malloc(sizeof(struct player));
}

struct player *plist;

struct player *delete_before(struct player *pscan) {
    struct player *p, *q;

    for (p = plist; (p->next)->next != pscan; p = p->next) {
        ;
    }

    q = p->next;

    p->next = (p->next)->next;

    if (plist == q) {
        plist = q->next;
    }

    return q;
}

struct player *buildalist(int n) {
    struct player *plist, *plast, *p;
    plist = lalloc();
    plist->number = n;
    plist->next = plist;
    plast = plist;

    while (--n > 0) {
        p = lalloc();
        p->number = n;
        p->next = plist;
        plist = p;
    }

    plast->next = plist;

    return plist;
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int t = atoi(argv[3]);

    int quitted = 0;
    int count = 1;
    int m;

    struct player *p, *pscan;

    plist = buildalist(n);

    pscan = plist;
    for (; pscan->next != pscan; pscan = pscan->next) {
        if (count % k == 0) {
            ++quitted;
            m = delete_before(pscan)->number;
            if (quitted >= n - t) {
                printf("%d\n", m);
            }
        }
        count++;
    }

    return 0;
}