#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 棋盘使用的是 GBK or UTF-8 编码，每一个中文字符占用 2 or 3 个字节。

// Macros
#define SIZE 15
#define CHARSIZE 3  // 2 for GBK, 3 for UTF-8
#define MAX_LINE 128
#define LOG_FILE "./gobang.log"
#define CHOICE 5

// Types
typedef struct Connected {
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
} Connected;

// Prototypes
void logMove(int currentSide, int x, int y);
void getPosition(int *x, int *y);
void refresh(void);
void addConnected(int color, int x, int y);
int detectWin(int color);
int isValid(int x, int y);
void thinkPosition(int *x, int *y, int side);
void removeConnected(int index);
bool takeBackMove(int currentSide);
bool human2HumanNextMove(int currentSide);
bool human2ComputerNextMove(int side, int currentSide);

void initRecordBoard(void);
void recordToDisplayArray(void);
void displayBoard(void);

// Variables
int Move = 0;

char *Color[2] = {"Black", "White"};
// 注意，枚举值从 1 开始，数组 Color 下标从 0 开始
enum Side { BLACK = 1, WHITE = 2 };

enum Mode { HUMAN2COMPUTER = 1, HUMAN2HUMAN = 2 };

Connected Pieces[SIZE * SIZE];

struct singleInfo {
    int linkNum;  //连珠数量
    int oppNum;   //两端对手棋子数量
};

struct singleScore {
    struct singleInfo info[4];  //记录四个方向连珠信息

    int score;  //当前点评分
};

struct point {
    int i;
    int j;
};

struct point highestFive[CHOICE] = {{7, 7}, {7, 6}, {7, 8}, {6, 7}, {8, 7}};

struct singleScore myBoardScore[SIZE][SIZE][2];

// 棋盘基本模板
// clang-format off
char InitDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1] = {
    "┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
    "┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"};
// clang-format on
// 此数组用于显示棋盘
char DisplayBoardArray[SIZE][SIZE * CHARSIZE + 1];

char Play1Pic[] = "●";  // 黑棋子;
char Play1CurrentPic[] = "▲";

char Play2Pic[] = "◎";  // 白棋子;
char Play2CurrentPic[] = "△";

// 此数组用于记录棋盘格局
int RecordBoard[SIZE][SIZE];

void logMove(int currentSide, int x, int y) {
    static FILE *fp = NULL;
    if (fp == NULL) {
        fp = fopen(LOG_FILE, "a");
        assert(fp != NULL);
        char *banner = "New game...\n";
        size_t sz = strlen(banner) * sizeof(char);
        size_t rc = fwrite(banner, sz, 1, fp);
        assert(rc == 1);
        fflush(fp);
    }

    if (fp != NULL) {
        char buf[MAX_LINE + 1];
        snprintf(buf, MAX_LINE, "side: %s, move: %c%d\n", Color[currentSide - 1], x, y);
        size_t sz = strlen(buf) * sizeof(char);
        size_t rc = fwrite(buf, sz, 1, fp);
        assert(rc == 1);
        fflush(fp);
    }
}

void getPosition(int *x, int *y) {
    int c;

    *x = *y = 0;

    // Eat the \n and other spaces
    while (isspace(c = getchar())) {
    }

    if (c == EOF) {
        printf("Got EOF, exit.\n");
        exit(1);
    }

    // printf("%d: c=%d\n", __LINE__, c);
    *x = c;
    if (c >= 'a' && c <= 'z') {
        *x = c - 'a' + 'A';
    }

    while (!isspace(c = getchar())) {
        if (c == EOF) {
            printf("Got EOF, exit.\n");
            exit(1);
        } else if (c >= '0' && c <= '9') {
            *y *= 10;
            *y += c - '0';
        }
    }

    // printf("%d: x=%d y=%d\n", __LINE__, *x, *y);
}

void refresh(void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (RecordBoard[i][j] == 3) {
                RecordBoard[i][j] = 1;
            } else if (RecordBoard[i][j] == 4) {
                RecordBoard[i][j] = 2;
            }
        }
    }
}

// color: 1 for black, 2 for white
void addConnected(int color, int x, int y) {
    int temp = 2 * Move + color - 1;

    Pieces[temp].color = color;
    Pieces[temp].x = x;
    Pieces[temp].y = y;
    Pieces[temp].N = 0;
    Pieces[temp].S = 0;
    Pieces[temp].E = 0;
    Pieces[temp].W = 0;
    Pieces[temp].SW = 0;
    Pieces[temp].SE = 0;
    Pieces[temp].NE = 0;
    Pieces[temp].NW = 0;

    for (int k = 1; RecordBoard[SIZE - y][x - 'A' + k] == color && x - 'A' + k < SIZE; k++) {
        Pieces[temp].E++;
    }
    for (int k = 1; RecordBoard[SIZE - y][x - 'A' - k] == color && x - 'A' - k >= 0; k++) {
        Pieces[temp].W++;
    }
    for (int k = 1; RecordBoard[SIZE - y - k][x - 'A'] == color && SIZE - y - k >= 0; k++) {
        Pieces[temp].N++;
    }
    for (int k = 1; RecordBoard[SIZE - y + k][x - 'A'] == color && SIZE - y + k < SIZE; k++) {
        Pieces[temp].S++;
    }
    for (int k = 1; RecordBoard[SIZE - y + k][x - 'A' + k] == color && SIZE - y + k < SIZE && x - 'A' + k < SIZE; k++) {
        Pieces[temp].SE++;
    }
    for (int k = 1; RecordBoard[SIZE - y - k][x - 'A' + k] == color && SIZE - y - k >= 0 && x - 'A' + k < SIZE; k++) {
        Pieces[temp].NE++;
    }
    for (int k = 1; RecordBoard[SIZE - y - k][x - 'A' - k] == color && SIZE - y - k >= 0 && x - 'A' - k >= 0; k++) {
        Pieces[temp].NW++;
    }
    for (int k = 1; RecordBoard[SIZE - y + k][x - 'A' - k] == color && SIZE - y + k <= SIZE && x - 'A' - k >= 0; k++) {
        Pieces[temp].SW++;
    }
}

int detectWin(int color) {
    return (Pieces[2 * Move + color - 1].N + Pieces[2 * Move + color - 1].S + 1 >= 5) ||
           (Pieces[2 * Move + color - 1].E + Pieces[2 * Move + color - 1].W + 1 >= 5) ||
           (Pieces[2 * Move + color - 1].SW + Pieces[2 * Move + color - 1].NE + 1 >= 5) ||
           (Pieces[2 * Move + color - 1].SE + Pieces[2 * Move + color - 1].NW + 1 >= 5);
}

int isValid(int x, int y) {
    return (x == 'R') || (x >= 'A' && x < 'A' + SIZE && y >= 1 && y <= SIZE && RecordBoard[SIZE - y][x - 'A'] == 0);
}

void evalueScore(int i, int j, int side) {
    for (int m = 0; m < 4; ++m) {
        myBoardScore[i][j][side - 1].info[m].linkNum = 0;
        myBoardScore[i][j][side - 1].info[m].oppNum = 0;
    }

    myBoardScore[i][j][side - 1].score = 0;

    if (RecordBoard[i][j] != 0) {
        myBoardScore[i][j][side - 1].score = -1;
    } else {
        int m = 1;
        for (; j + m < SIZE && (RecordBoard[i][j + m] == side || RecordBoard[i][j + m] == side + 2); m++) {
            myBoardScore[i][j][side - 1].info[0].linkNum++;
        }

        if (j + m < SIZE && RecordBoard[i][j + m] != 0) {
            myBoardScore[i][j][side - 1].info[0].oppNum++;
        }

        for (m = 1; j - m >= 0 && (RecordBoard[i][j - m] == side || RecordBoard[i][j - m] == side + 2); m++) {
            myBoardScore[i][j][side - 1].info[0].linkNum++;
        }

        if (j - m >= 0 && RecordBoard[i][j - m] != 0) {
            myBoardScore[i][j][side - 1].info[0].oppNum++;
        }

        for (m = 1; i + m < SIZE && (RecordBoard[i + m][j] == side || RecordBoard[i + m][j] == side + 2); m++) {
            myBoardScore[i][j][side - 1].info[1].linkNum++;
        }

        if (i + m < SIZE && RecordBoard[i + m][j] != 0) {
            myBoardScore[i][j][side - 1].info[1].oppNum++;
        }

        for (m = 1; i - m >= 0 && (RecordBoard[i - m][j] == side || RecordBoard[i - m][j] == side + 2); m++) {
            myBoardScore[i][j][side - 1].info[1].linkNum++;
        }

        if (i - m >= 0 && RecordBoard[i - m][j] != 0) {
            myBoardScore[i][j][side - 1].info[1].oppNum++;
        }

        for (m = 1; i + m < SIZE && j + m < SIZE &&
                    (RecordBoard[i + m][j + m] == side || RecordBoard[i + m][j + m] == side + 2);
             m++) {
            myBoardScore[i][j][side - 1].info[2].linkNum++;
        }

        if (i + m < SIZE && j + m < SIZE && RecordBoard[i + m][j + m] != 0) {
            myBoardScore[i][j][side - 1].info[2].oppNum++;
        }

        for (m = 1;
             i - m >= 0 && j - m >= 0 && (RecordBoard[i - m][j - m] == side || RecordBoard[i - m][j - m] == side + 2);
             m++) {
            myBoardScore[i][j][side - 1].info[2].linkNum++;
        }

        if (i - m >= 0 && j - m >= 0 && RecordBoard[i - m][j - m] != 0) {
            myBoardScore[i][j][side - 1].info[2].oppNum++;
        }

        for (m = 1;
             i + m < SIZE && j - m >= 0 && (RecordBoard[i + m][j - m] == side || RecordBoard[i + m][j - m] == side + 2);
             m++) {
            myBoardScore[i][j][side - 1].info[3].linkNum++;
        }

        if (i + m < SIZE && j - m >= 0 && RecordBoard[i + m][j - m] != 0) {
            myBoardScore[i][j][side - 1].info[3].oppNum++;
        }

        for (m = 1;
             i - m >= 0 && j + m < SIZE && (RecordBoard[i - m][j + m] == side || RecordBoard[i - m][j + m] == side + 2);
             m++) {
            myBoardScore[i][j][side - 1].info[3].linkNum++;
        }

        if (i - m >= 0 && j + m < SIZE && RecordBoard[i - m][j + m] != 0) {
            myBoardScore[i][j][side - 1].info[3].oppNum++;
        }

        for (int n = 0; n < 4; n++) {
            if (myBoardScore[i][j][side - 1].info[n].linkNum >= 4) {
                myBoardScore[i][j][side - 1].score += 200;
            } else {
                myBoardScore[i][j][side - 1].score += (myBoardScore[i][j][side - 1].info[n].linkNum) *
                                                      (4 - 1 * (myBoardScore[i][j][side - 1].info[n].oppNum));
            }
        }
    }
}

void thinkPosition(int *x, int *y, int side) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            evalueScore(i, j, side);
            evalueScore(i, j, 3 - side);
        }
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < CHOICE; k++) {
                if (2 * myBoardScore[i][j][side - 1].score + 3 * myBoardScore[i][j][2 - side].score >
                    2 * myBoardScore[highestFive[k].i][highestFive[k].j][side - 1].score +
                        3 * myBoardScore[highestFive[k].i][highestFive[k].j][2 - side].score) {
                    highestFive[k].i = i;
                    highestFive[k].j = j;
                }
            }
        }
    }

    int random;

    do {
        random = rand() % CHOICE;
        *x = highestFive[random].j + 'A';
        *y = SIZE - highestFive[random].i;
    } while (myBoardScore[highestFive[random].i][highestFive[random].j][side - 1].score < 0);
    /*do {
        *x = rand() % 15 + 'A';
        *y = rand() % 15;
    } while (RecordBoard[SIZE - *y][*x - 'A'] != 0);*/
}

void removeConnected(int index) {
    RecordBoard[SIZE - Pieces[index].y][Pieces[index].x - 'A'] = 0;
    recordToDisplayArray();
    displayBoard();
}

bool takeBackMove(int currentSide) {
    if (Move <= 0) {
        printf("No moves to take back!\n");
        return false;
    }

    removeConnected(2 * Move + currentSide - 2);
    removeConnected(2 * Move + currentSide - 3);

    Move--;

    printf("Please redo %s's last move: \n", Color[currentSide - 1]);
    return true;
}

// return: if is a take back?
bool human2HumanNextMove(int currentSide) {
    int x, y;

    printf("%s to move: ", Color[currentSide - 1]);
    getPosition(&x, &y);

    while (!isValid(x, y)) {
        printf("Not a valid position. Retype: ");
        getPosition(&x, &y);
    }

    // Log the move
    logMove(currentSide, x, y);

    if (x == 'R') {  // take back
        takeBackMove(currentSide);
        return true;
    } else {
        addConnected(currentSide, x, y);

        RecordBoard[SIZE - y][x - 'A'] = currentSide + 2;
        recordToDisplayArray();
        displayBoard();

        refresh();
    }

    if (detectWin(currentSide)) {
        printf("%s wins!\n", Color[currentSide - 1]);
        exit(1);
    }

    return false;
}

// return value: if is a take back?
bool human2ComputerNextMove(int side, int currentSide) {
    int x, y;

    if (side == currentSide) {
        printf("%s to move: ", Color[side - 1]);
        getPosition(&x, &y);

        while (!isValid(x, y)) {
            printf("Not a valid position. Retype: ");
            getPosition(&x, &y);
        }

        // Log the move
        logMove(currentSide, x, y);

        if (x == 'R') {  // take back
            takeBackMove(currentSide);
            return true;
        } else {
            addConnected(side, x, y);

            RecordBoard[SIZE - y][x - 'A'] = side + 2;
            recordToDisplayArray();
            displayBoard();

            refresh();
        }

        if (detectWin(side)) {
            printf("%s wins!\n", Color[side - 1]);
            exit(1);
        }
    } else {
        printf("Computer thinking...\n");
        thinkPosition(&x, &y, currentSide);

        // Log the move
        logMove(currentSide, x, y);

        addConnected(currentSide, x, y);

        RecordBoard[SIZE - y][x - 'A'] = currentSide + 2;
        recordToDisplayArray();
        displayBoard();

        printf("Computer's move: ");

        putchar(x);
        printf("%d\n", y);

        refresh();

        if (detectWin(currentSide)) {
            printf("%s wins!\n", Color[currentSide - 1]);
            exit(1);
        }
    }

    return false;
}

int main(void) {
    int mode, side;

    initRecordBoard();
    recordToDisplayArray();
    displayBoard();

    while (true) {
        printf("Please choose your mode(1 for human vs computer, 2 for human vs human): ");
        scanf("%d", &mode);

        if (mode == EOF) {
            printf("Got EOF, exit\n");
            exit(1);
        }

        if (mode == HUMAN2COMPUTER || mode == HUMAN2HUMAN) {
            break;
        }
    }

    if (mode == HUMAN2COMPUTER) {
        while (true) {
            printf("Please choose your side(1 for black, 2 for white): ");
            scanf("%d", &side);

            if (side == EOF) {
                printf("Got EOF, exit\n");
                exit(1);
            }

            if (side == BLACK || side == WHITE) {
                break;
            }
        }

        while (Move >= 0) {
            while (human2ComputerNextMove(side, BLACK))
                ;
            while (human2ComputerNextMove(side, WHITE))
                ;

            Move++;
        }
    } else {
        while (Move >= 0) {
            while (human2HumanNextMove(BLACK))
                ;
            while (human2HumanNextMove(WHITE))
                ;

            Move++;
        }
    }
}

// 初始化棋盘格局
void initRecordBoard(void) {
    // 通过双重循环，将 RecordBoard 清 0
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            RecordBoard[i][j] = 0;
        }
    }
}

// 将 RecordBoard 中记录的棋子位置，转化到 DisplayBoardArray 中
void recordToDisplayArray(void) {
    // 第一步：将 InitDisplayBoardArray 中记录的空棋盘，复制到 DisplayBoardArray 中
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE * CHARSIZE + 1; j++) {
            DisplayBoardArray[i][j] = InitDisplayBoardArray[i][j];
        }
    }

    // 第二步：扫描 RecordBoard，当遇到非 0 的元素，将 ● 或者 ◎ 复制到 DisplayBoardArray 的相应位置上
    // 注意：DisplayBoardArray 所记录的字符是中文字符，每个字符占 2 或 3 个字节。● 和 ◎ 也是中文字符，
    // 每个也占 2 或 3 个字节。
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (RecordBoard[i][j] == 1) {
                DisplayBoardArray[i][CHARSIZE * j] = Play1Pic[0];
                DisplayBoardArray[i][CHARSIZE * j + 1] = Play1Pic[1];
                if (CHARSIZE == 3) {
                    DisplayBoardArray[i][CHARSIZE * j + 2] = Play1Pic[2];
                }
            } else if (RecordBoard[i][j] == 2) {
                DisplayBoardArray[i][CHARSIZE * j] = Play2Pic[0];
                DisplayBoardArray[i][CHARSIZE * j + 1] = Play2Pic[1];
                if (CHARSIZE == 3) {
                    DisplayBoardArray[i][CHARSIZE * j + 2] = Play2Pic[2];
                }
            } else if (RecordBoard[i][j] == 3) {
                DisplayBoardArray[i][CHARSIZE * j] = Play1CurrentPic[0];
                DisplayBoardArray[i][CHARSIZE * j + 1] = Play1CurrentPic[1];
                if (CHARSIZE == 3) {
                    DisplayBoardArray[i][CHARSIZE * j + 2] = Play1CurrentPic[2];
                }
            } else if (RecordBoard[i][j] == 4) {
                DisplayBoardArray[i][CHARSIZE * j] = Play2CurrentPic[0];
                DisplayBoardArray[i][CHARSIZE * j + 1] = Play2CurrentPic[1];
                if (CHARSIZE == 3) {
                    DisplayBoardArray[i][CHARSIZE * j + 2] = Play2CurrentPic[2];
                }
            }
        }
    }
}

// 显示棋盘格局
void displayBoard(void) {
    // 第一步：清屏
    system("clear");  // 清屏

    printf("Work of 张治成\n");

    // 第二步：将 DisplayBoardArray 输出到屏幕上
    for (int i = 0; i < SIZE; i++) {
        printf("%3d%s\n", SIZE - i, DisplayBoardArray[i]);
    }

    // 第三步：输出最下面的一行字母 A, B, ...
    printf("   ");
    for (int i = 0; i < SIZE; i++) {
        printf("%2c", 'A' + i);
    }

    printf("\n");
}