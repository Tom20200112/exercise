#include <stdio.h>

int main() /*This is the main function that is called right after starting the program and 
do the program's main work. */
{
    int max(int x, int y); //declaration of function max

    int a, b, c;

    scanf("%d, %d", &a, &b); /*User inputs the values of variables a and b*/
    c = max(a, b);

    printf("max=%d\n", c); //Print the max value

    return 0;
}
/*definition of function max*/
int max(int x, int y)
{
    int z;
    if (x > y)
        z = x;
    else
        z = y;

    return z;
}