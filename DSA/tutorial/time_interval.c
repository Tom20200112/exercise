#include <stdio.h>

int main(void) {
    int hh1, mm1, ss1, hh2, mm2, ss2;

    scanf("%d:%d:%d %d:%d:%d", &hh1, &mm1, &ss1, &hh2, &mm2, &ss2);

    int interval = (hh2 - hh1) * 3600 + (mm2 - mm1) * 60 + ss2 - ss1;

    if (interval < 0) {
        interval += 86400;
    }

    printf("%d\n", interval);
}