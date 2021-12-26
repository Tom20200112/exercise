#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 棋盘使用的是 GBK or UTF-8 编码，每一个中文字符占用 2 or 3 个字节。

#define SIZE 15
#define CHARSIZE 3  // 2 for GBK, 3 for UTF-8

void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

struct connected {
    int color;  // 1 for black, 2 for white

    int x;
    int y;

    int N;
    int S;
    int E;
    int W;
    int NE;
    int NW;
    int SE;
    int SW;
};

struct connected pieces[SIZE * SIZE];

// 棋盘基本模板
char aInitDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1] = {
    "┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨", "┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"};
// 此数组用于显示棋盘
char aDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1];

char play1Pic[] = "●";  //黑棋子;
char play1CurrentPic[] = "▲";

char play2Pic[] = "◎";  //白棋子;
char play2CurrentPic[] = "△";

int x[1];
int y[1];

//此数组用于记录棋盘格局
int aRecordBoard[SIZE][SIZE];

void getposition(int x[], int y[]) {
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

void addConnected(int move, int color /*1 for black, 2 for white*/) {
    int temp = 2 * move + color - 1;

    pieces[temp].color = color;
    pieces[temp].x = x[0];
    pieces[temp].y = y[0];
    pieces[temp].N = 0;
    pieces[temp].S = 0;
    pieces[temp].E = 0;
    pieces[temp].W = 0;
    pieces[temp].SW = 0;
    pieces[temp].SE = 0;
    pieces[temp].NE = 0;
    pieces[temp].NW = 0;

    for (int k = 1; aRecordBoard[SIZE - y[0]][x[0] - 'A' + k] == color && x[0] - 'A' + k < SIZE; k++) {
        pieces[temp].E++;
    }
    for (int k = 1; aRecordBoard[SIZE - y[0]][x[0] - 'A' - k] == color && x[0] - 'A' - k >= 0; k++) {
        pieces[temp].W++;
    }
    for (int k = 1; aRecordBoard[SIZE - y[0] - k][x[0] - 'A'] == color && SIZE - y[0] - k >= 0; k++) {
        pieces[temp].N++;
    }
    for (int k = 1; aRecordBoard[SIZE - y[0] + k][x[0] - 'A'] == color && SIZE - y[0] + k < SIZE; k++) {
        pieces[temp].S++;
    }
    for (int k = 1;
         aRecordBoard[SIZE - y[0] + k][x[0] - 'A' + k] == color && SIZE - y[0] + k < SIZE && x[0] - 'A' + k < SIZE;
         k++) {
        pieces[temp].SE++;
    }
    for (int k = 1;
         aRecordBoard[SIZE - y[0] - k][x[0] - 'A' + k] == color && SIZE - y[0] - k >= 0 && x[0] - 'A' + k < SIZE; k++) {
        pieces[temp].NE++;
    }
    for (int k = 1;
         aRecordBoard[SIZE - y[0] - k][x[0] - 'A' - k] == color && SIZE - y[0] - k >= 0 && x[0] - 'A' - k >= 0; k++) {
        pieces[temp].NW++;
    }
    for (int k = 1;
         aRecordBoard[SIZE - y[0] + k][x[0] - 'A' - k] == color && SIZE - y[0] + k <= SIZE && x[0] - 'A' - k >= 0;
         k++) {
        pieces[temp].SW++;
    }
}

int detectWin(int move, int color) {
    return (pieces[2 * move + color - 1].N + pieces[2 * move + color - 1].S + 1 >= 5) ||
           (pieces[2 * move + color - 1].E + pieces[2 * move + color - 1].W + 1 >= 5) ||
           (pieces[2 * move + color - 1].SW + pieces[2 * move + color - 1].NE + 1 >= 5) ||
           (pieces[2 * move + color - 1].SE + pieces[2 * move + color - 1].NW + 1 >= 5);
}

void thinkPosition(int x[], int y[]) {
    while (aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
        x[0] = rand() % 15 + 'A';
        y[0] = rand() % 15;
    }
}

int main(void) {
    int move = 0;

    int mode, side;

    initRecordBorard();
    recordtoDisplayArray();
    displayBoard();

    printf("Please choose your mode(1 for human vs computer, 2 for human vs human): ");

    scanf("%d", &mode);

    assert(mode == 1 || mode == 2);

    if (mode == 1) {
        printf("Please choose your side(1 for black, 2 for white): ");
        scanf("%d", &side);

        assert(side == 1 || side == 2);

        if (side == 1) {
            while (move >= 0) {
                printf("Black to move: ");
                getposition(x, y);

                while ((!(((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) &&
                          (y[0] >= 1 && y[0] <= SIZE))) ||
                       aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
                    printf("Not a valid position. Retype: ");
                    getposition(x, y);
                }

                addConnected(move, 1);

                aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 3;
                recordtoDisplayArray();
                displayBoard();

                refresh();

                if (detectWin(move, 1)) {
                    printf("Black wins! ");
                    return 0;
                }

                printf("Computer thinking...\n");
                thinkPosition(x, y);

                addConnected(move, 2);

                aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 4;
                recordtoDisplayArray();
                displayBoard();

                refresh();

                if (detectWin(move, 2)) {
                    printf("White wins! ");
                    return 0;
                }

                move++;
            }
        } else {
            while (move >= 0) {
                printf("Computer thinking...\n");
                thinkPosition(x, y);

                addConnected(move, 1);

                aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 3;
                recordtoDisplayArray();
                displayBoard();

                refresh();

                if (detectWin(move, 1)) {
                    printf("Black wins! ");
                    return 0;
                }

                printf("White to move: ");
                getposition(x, y);

                while ((!(((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) &&
                          (y[0] >= 1 && y[0] <= SIZE))) ||
                       aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
                    printf("Not a valid position. Retype: ");
                    getposition(x, y);
                }

                addConnected(move, 2);

                aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 4;
                recordtoDisplayArray();
                displayBoard();

                refresh();

                if (detectWin(move, 2)) {
                    printf("White wins! ");
                    return 0;
                }

                move++;
            }
        }
    }

    else {
        while (move >= 0) {
            printf("Black to move: ");
            getposition(x, y);

            while ((!(((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) && (y[0] >= 1 && y[0] <= SIZE))) ||
                   aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
                printf("Not a valid position. Retype: ");
                getposition(x, y);
            }

            addConnected(move, 1);

            aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 3;
            recordtoDisplayArray();
            displayBoard();

            refresh();

            if (detectWin(move, 1)) {
                printf("Black wins! ");
                return 0;
            }

            printf("White to move: ");
            getposition(x, y);

            while ((!(((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) && (y[0] >= 1 && y[0] <= SIZE))) ||
                   aRecordBoard[SIZE - y[0]][x[0] - 'A'] != 0) {
                printf("Not a valid position. Retype: ");
                getposition(x, y);
            }

            addConnected(move, 2);

            aRecordBoard[SIZE - y[0]][x[0] - 'A'] = 4;
            recordtoDisplayArray();
            displayBoard();

            refresh();

            if (detectWin(move, 2)) {
                printf("White wins! ");
                return 0;
            }

            move++;
        }
    }
    /*
    initRecordBorard();
    aRecordBoard[5][8]=1;
    aRecordBoard[5][9]=2;
    recordtoDisplayArray();
    displayBoard();
    getchar();
    aRecordBoard[3][4]=2;
    recordtoDisplayArray();
    displayBoard();
    getchar();

    aRecordBoard[6][1]=1;
    recordtoDisplayArray();
    displayBoard();
    getchar();

    aRecordBoard[9][4]=2;
    recordtoDisplayArray();
    displayBoard();
    getchar();
    return 0;
    */
}

// 初始化棋盘格局
void initRecordBorard(void) {
    // 通过双重循环，将aRecordBoard清0
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            aRecordBoard[i][j] = 0;
        }
    }
}

// 将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
void recordtoDisplayArray(void) {
    // 第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE * CHARSIZE + 1; j++) {
            aDisplayBoardArray[i][j] = aInitDisplayBoardArray[i][j];
        }
    }

    // 第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
    // 注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (aRecordBoard[i][j] == 1) {
                aDisplayBoardArray[i][CHARSIZE * j] = play1Pic[0];
                aDisplayBoardArray[i][CHARSIZE * j + 1] = play1Pic[1];
                if (CHARSIZE == 3) {
                    aDisplayBoardArray[i][CHARSIZE * j + 2] = play1Pic[2];
                }
            } else if (aRecordBoard[i][j] == 2) {
                aDisplayBoardArray[i][CHARSIZE * j] = play2Pic[0];
                aDisplayBoardArray[i][CHARSIZE * j + 1] = play2Pic[1];
                if (CHARSIZE == 3) {
                    aDisplayBoardArray[i][CHARSIZE * j + 2] = play2Pic[2];
                }

            } else if (aRecordBoard[i][j] == 3) {
                aDisplayBoardArray[i][CHARSIZE * j] = play1CurrentPic[0];
                aDisplayBoardArray[i][CHARSIZE * j + 1] = play1CurrentPic[1];
                if (CHARSIZE == 3) {
                    aDisplayBoardArray[i][CHARSIZE * j + 2] = play1CurrentPic[2];
                }

            } else if (aRecordBoard[i][j] == 4) {
                aDisplayBoardArray[i][CHARSIZE * j] = play2CurrentPic[0];
                aDisplayBoardArray[i][CHARSIZE * j + 1] = play2CurrentPic[1];
                if (CHARSIZE == 3) {
                    aDisplayBoardArray[i][CHARSIZE * j + 2] = play2CurrentPic[2];
                }
            }
        }
    }
}

// 显示棋盘格局
void displayBoard(void) {
    int i;
    // 第一步：清屏
    system("clear");  //清屏

    // 第二步：将aDisplayBoardArray输出到屏幕上
    for (i = 0; i < SIZE; i++) {
        printf("%3d%s\n", SIZE - i, aDisplayBoardArray[i]);
    }

    // 第三步：输出最下面的一行字母A B ....
    printf("   ");
    for (i = 0; i < SIZE; i++) {
        printf("%2c", 'A' + i);
    }
    printf("\n");
}
