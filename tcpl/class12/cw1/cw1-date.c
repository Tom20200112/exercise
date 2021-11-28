#include <stdio.h>

int date[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                   {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int is_leap(int year) { return ((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0 ? 1 : 0; }

int totaldayAD(int year, int month, int day) {
    int totalday = 0;

    for (int i = 1; i < year; i++) {
        if (is_leap(i)) {
            totalday += 366;
        } else {
            totalday += 365;
        }
    }

    for (int i = 0; i < month; i++) {
        totalday += date[is_leap(year)][i];
    }

    totalday += day;

    return totalday;
}

int main(void) {
    int year1, year2, month1, month2, day1, day2;

    printf("Please input date1: ");
    scanf("%d", &year1);
    scanf("%d", &month1);
    scanf("%d", &day1);

    printf("Please input date2: ");
    scanf("%d", &year2);
    scanf("%d", &month2);
    scanf("%d", &day2);

    printf("There %s %d day%s between date1 and date2\n",
           totaldayAD(year2, month2, day2) - totaldayAD(year1, month1, day1) == 1 ? "is" : "are",
           totaldayAD(year2, month2, day2) - totaldayAD(year1, month1, day1),
           totaldayAD(year2, month2, day2) - totaldayAD(year1, month1, day1) == 1 ? "" : "s");

    return 0;
}