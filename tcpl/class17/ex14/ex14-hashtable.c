#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nlist {          /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *name;         /* defined name */
    char *defn;         /* replacement text */
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

struct nlist *lookup(const char *s);

/* hash:  form hash value for string s */
unsigned hash(const char *s) {
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++) hashval = *s + 31 * hashval;

    return hashval % HASHSIZE;
}

char *mystrdup(char *s) /* make a duplicate of s */
{
    char *p;

    /* 最好先检查参数 s 的取值是不是为 NULL。 */
    p = (char *)malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL) strcpy(p, s);
    return p;
}

/* install:  put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct nlist *)malloc(sizeof(*np));
        if (np == NULL || (np->name = mystrdup(name)) == NULL) return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else                      /* already there */
        free((void *)np->defn); /* free previous defn */

    if ((np->defn = mystrdup(defn)) == NULL) return NULL;

    return np;
}

/* lookup:  look for s in hashtab */
struct nlist *lookup(const char *s) {
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) return np; /* found */
    }

    return NULL; /* not found */
}

void buildlist() {
    struct name_and_defn {
        char *name;
        char *defn;
    };

    struct name_and_defn ndpairs[] = {{"IN", "1"},   {"OUT", "2"}, {"MAX", "10000"},  {"ZHANGSAN", "1"},
                                      {"LISI", "2"}, {"MIN", "3"}, {"NUMBER", "1024"}};

    int i;

    for (i = 0; i < sizeof(ndpairs) / sizeof(struct name_and_defn); i++) {
        install(ndpairs[i].name, ndpairs[i].defn);
    }
}

int main(void) {
    struct nlist *plist;

    char keyword[100];
    buildlist();

    printf("Please input keyword: ");
    scanf("%s", (char *)keyword);

    plist = (struct nlist *)lookup(keyword);

    if (plist) {
        printf("%s>>%s\n", plist->name, plist->defn);
    } else {
        printf("not found\n");
    }

    return 0;
}