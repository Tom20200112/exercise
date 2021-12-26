#include <stdio.h>

struct {
    char name[10];
    int gender;
    int age;
} laowang = {"laowang", 1, 28};

int main(void) {
    printf("size of laowang=%ld\n", sizeof(laowang));
    printf("Address of laowang.name=  %p\n", laowang.name);
    printf("Address of laowang.gender=%p\n", &laowang.gender);
    printf("Address of laowang.age   =%p\n", &laowang.age);

    return 0;
}