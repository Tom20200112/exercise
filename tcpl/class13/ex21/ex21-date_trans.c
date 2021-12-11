#include <stdio.h>

int is_leap_year(int year) { return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0 ? 1 : 0; }

static char daytab[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                             {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int day_of_year(int year, int month, int day) {
    int i, leap;

    leap = is_leap_year(year);
    for (i = 0; i < month; i++) {
        day += daytab[leap][i];
    }

    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday) {
    int i, leap;
    leap = is_leap_year(year);

    for (i = 1; yearday > daytab[leap][i]; i++) {
        yearday -= daytab[leap][i];
    }

    *pmonth = i;
    *pday = yearday;
}

int main(void) {
    int mode;
    int year, month, day;
    int yearday;

    printf("Please choose: \n");

    printf("(1) year-month-day to yearday: press1\n");
    printf("(2) yearday to year-month-day: press2\n");
    scanf("%d", &mode);

    if (mode == 1) {
        printf("Input year: ");
        scanf("%d", &year);

        printf("Input month: ");
        scanf("%d", &month);

        printf("Input day: ");
        scanf("%d", &day);

        yearday = day_of_year(year, month, day);
        printf("yearday=%d\n", yearday);

    } else if (mode == 2) {
        printf("Input year: ");
        scanf("%d", &year);

        printf("Input yearday: ");
        scanf("%d", &yearday);

        month_day(year, yearday, &month, &day);

        printf("year=%d month=%d day=%d\n", year, month, day);
    } else {
        printf("Wrong mode!\n");
    }

    return 0;
}