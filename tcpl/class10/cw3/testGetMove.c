#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 15

int x[1];
int y[1];

int aRecordBoard[SIZE][SIZE];

// 初始化棋盘格局
void initRecordBoard(void) {
    // 通过双重循环，将aRecordBoard清0
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            aRecordBoard[i][j] = 0;
        }
    }
}

void getPosition(int x[], int y[]) {
    int c;
    y[0] = 0;

    // Eat the \n and other spaces
    while (isspace(c = getchar())) {
    }

    if (c == EOF) {
        printf("Got EOF, exit.\n");
        exit(1);
    }

    // printf("%d: c=%d\n", __LINE__, c);
    if (c >= 'a' && c <= 'z') {
        x[0] = c - 'a' + 'A';
    } else {
        // Invalid, let caller check it
    }

    while (!isspace(c = getchar())) {
        if (c == EOF) {
            printf("Got EOF, exit.\n");
            exit(1);
        } else if (c >= '0' && c <= '9') {
            y[0] *= 10;
            y[0] += c - '0';
        }
    }

    // printf("%d: x=%d y=%d\n", __LINE__, x[0], y[0]);
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

    initRecordBoard();

    while (move >= 0) {
        printf("Black to move: ");
        getPosition(x, y);

        while ((!(((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) && (y[0] >= 1 && y[0] <= SIZE))) ||
               aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
            printf("Not a valid position. Retype: ");
            getPosition(x, y);
        }

        aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 3;

        refresh();

        move++;
    }

    return 0;
}