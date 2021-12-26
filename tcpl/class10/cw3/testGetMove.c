#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 15

int x[1];
int y[1];

int aRecordBoard[SIZE][SIZE];

// 初始化棋盘格局
void initRecordBorard(void) {
    // 通过双重循环，将aRecordBoard清0
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            aRecordBoard[i][j] = 0;
        }
    }
}

void getposition(int x[], int y[]) {
    int c;
    y[0] = 0;

    if ((x[0] = getchar()) >= 'a' && x[0] <= 'z') {
        x[0] = x[0] - 'a' + 'A';
    }

    if (x[0] == EOF) {
        exit(1);
    }

    while ((c = getchar()) != EOF && c != '\t' && c != '\n' && c != ' ') {
        y[0] *= 10;
        y[0] += c - '0';
    }
}

void refresh(void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (aRecordBoard[i][j] == 3) {
                aRecordBoard[i][j] = 1;
            } else if (aRecordBoard[i][j] == 4) {
                aRecordBoard[i][j] = 2;
            }
        }
    }
}

int main(void) {
    int move = 0;

    initRecordBorard();

    while (move >= 0) {
        printf("Black to move: ");
        getposition(x, y);

        while ((!(((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) && (y[0] >= 1 && y[0] <= SIZE))) ||
               aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
            printf("Not a valid position. Retype: ");
            getposition(x, y);
        }

        aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 3;

        refresh();

        move++;
    }

    return 0;
}