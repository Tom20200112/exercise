#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> /* for  atof() */

#define BUFSIZE 100
#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define MAXVAL 100 /* maximum depth of val stack */

int sp = 0;        /* next free stack position */
int val[MAXVAL];   /* value stack */
char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

extern int getop(char[]);
extern void push(int);
extern int pop(void);
extern void clearbuf(char buf[]) { bufp = 0; }
extern void clearval(int val[]) { sp = 0; }

/* reverse Polish calculator */
int main(void) {
    int type;
    int op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push((int)atof(s));
                break;

            case '+':
                push(pop() + pop());
                break;

            case '*':
                push(pop() * pop());
                break;

            case '-':
                op2 = pop();
                push(pop() - op2);
                break;

            case '/':
                op2 = pop();
                if (op2 != 0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;

            case '%':
                op2 = pop();
                if (op2 != 0)
                    push(pop() % op2);
                else
                    printf("error: zero divisor\n");
                break;

            case '&':
                push(pop() & pop());
                break;

            case '|':
                push(pop() | pop());
                break;

            case '<':
                op2 = pop();
                push(pop() << op2);
                break;

            case '>':
                op2 = pop();
                push(pop() >> op2);
                break;

            case '\n':
                printf("\t%d\n", pop());
                clearbuf(buf);
                clearval(val);
                break;

            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }

    return 0;
}

/* push:  push f onto value stack */
void push(int f) {
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %d\n", f);
}

/* pop:  pop and return top value from stack */
int pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

extern int getch(void);
extern void ungetch(int);

/* getop:  get next character or numeric operand */
int getop(char s[]) {
    int i, c, d;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';

    if (!isdigit(c) && c != '.' && c != '<' && c != '>') return c; /* not a number */

    if ((c == '<') | (c == '>')) {
        d = getchar();
        d = getchar();
        if (d != EOF) {
            ungetch(d);
        }
        return c;
    }

    i = 0;
    if (isdigit(c)) /* collect integer part */
        while (isdigit(s[++i] = c = getch()))
            ;

    if (c == '.') /* collect fraction part */
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';

    if (c != EOF) ungetch(c);

    return NUMBER;
}

int getch(void) /* get a (possibly pushed-back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}