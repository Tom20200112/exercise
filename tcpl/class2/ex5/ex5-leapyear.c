#include <stdio.h>

int main(void)
{
    int year;
    int isleap(int year);

    for (year = 2000; year <= 2100; year++) {
        if (isleap(year)) {
            printf("%d is leap year\n", year);
        } else {
            printf("%d is not leap year\n", year);
        }
    }

    return 0;
}

int isleap(int year)
{
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return 1;
            } else {
                return 0;
            }

        } else {
            return 1;
        }
    } else {
        return 0;
    }
}