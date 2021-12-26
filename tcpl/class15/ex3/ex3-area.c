#include <stdio.h>

struct point {
    double x;
    double y;
};

struct rect {
    struct point p1;
    struct point p2;
};

int main(void) {
    struct rect screen = {{1, 3}, {4, 5}};

    printf("point1:(%.lf,%.lf) point2:(%.lf,%.lf)\n", screen.p1.x, screen.p1.y, screen.p2.x, screen.p2.y);
    printf("square=:%.lf\n", (screen.p2.x - screen.p1.x) * (screen.p2.y - screen.p1.y));

    return 0;
}
