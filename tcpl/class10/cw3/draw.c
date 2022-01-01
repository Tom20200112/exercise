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

typedef struct Sorter {
    int i;
    int j;
    int score;
} Sorter;

typedef struct SingleInfo {
    int linkNum;  // 连珠数量
    int oppNum;   // 两端对手棋子数量
} SingleInfo;

typedef struct SingleScore {
    SingleInfo info[4];  // 记录四个方向连珠信息
    int score;           // 当前点评分
} SingleScore;

typedef struct Point {
    int i;
    int j;
} Point;

// Prototypes
void logMove(int currentSide, int x, int y);
void getPosition(int *x, int *y);
void refresh(void);
void addConnected(int color, int x, int y);
int detectWin(int color);
int isValid(int x, int y);
int cmpSorter(const void *a, const void *b);
void chooseMove(int *x, int *y, int side);
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

bool Chengwu[2] = {false, false};

bool Huosi[2] = {false, false};

bool Chongsi[2] = {false, false};

Point ChengwuPoint[2] = {{-1, -1}, {-1, -1}};

Point HuosiPoint[2][2] = {{{-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}}};

Point ChongsiPoint[2][2] = {{{-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}}};

char *Color[2] = {"Black", "White"};
// 注意，枚举值从 1 开始，数组 Color 下标从 0 开始
enum Side { BLACK = 1, WHITE = 2 };

enum Mode { HUMAN2COMPUTER = 1, HUMAN2HUMAN = 2 };

Connected Pieces[SIZE * SIZE];

SingleScore MyBoardScore[SIZE][SIZE][2];

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
        ;
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
        MyBoardScore[i][j][side - 1].info[m].linkNum = 0;
        MyBoardScore[i][j][side - 1].info[m].oppNum = 0;
    }

    MyBoardScore[i][j][side - 1].score = 0;

    if (RecordBoard[i][j] != 0) {
        MyBoardScore[i][j][side - 1].score = -1;
    } else {
        int m = 1;
        for (; j + m < SIZE && (RecordBoard[i][j + m] == side || RecordBoard[i][j + m] == side + 2); m++) {
            MyBoardScore[i][j][side - 1].info[0].linkNum++;
        }

        if ((j + m < SIZE && RecordBoard[i][j + m] != 0) || j + m >= SIZE) {
            MyBoardScore[i][j][side - 1].info[0].oppNum++;
        }

        for (m = 1; j - m >= 0 && (RecordBoard[i][j - m] == side || RecordBoard[i][j - m] == side + 2); m++) {
            MyBoardScore[i][j][side - 1].info[0].linkNum++;
        }

        if ((j - m >= 0 && RecordBoard[i][j - m] != 0) || j - m < 0) {
            MyBoardScore[i][j][side - 1].info[0].oppNum++;
        }

        for (m = 1; i + m < SIZE && (RecordBoard[i + m][j] == side || RecordBoard[i + m][j] == side + 2); m++) {
            MyBoardScore[i][j][side - 1].info[1].linkNum++;
        }

        if ((i + m < SIZE && RecordBoard[i + m][j] != 0) || i + m >= SIZE) {
            MyBoardScore[i][j][side - 1].info[1].oppNum++;
        }

        for (m = 1; i - m >= 0 && (RecordBoard[i - m][j] == side || RecordBoard[i - m][j] == side + 2); m++) {
            MyBoardScore[i][j][side - 1].info[1].linkNum++;
        }

        if ((i - m >= 0 && RecordBoard[i - m][j] != 0) || i - m < 0) {
            MyBoardScore[i][j][side - 1].info[1].oppNum++;
        }

        for (m = 1; i + m < SIZE && j + m < SIZE &&
                    (RecordBoard[i + m][j + m] == side || RecordBoard[i + m][j + m] == side + 2);
             m++) {
            MyBoardScore[i][j][side - 1].info[2].linkNum++;
        }

        if ((i + m < SIZE && j + m < SIZE && RecordBoard[i + m][j + m] != 0) || i + m >= SIZE || j + m >= SIZE) {
            MyBoardScore[i][j][side - 1].info[2].oppNum++;
        }

        for (m = 1;
             i - m >= 0 && j - m >= 0 && (RecordBoard[i - m][j - m] == side || RecordBoard[i - m][j - m] == side + 2);
             m++) {
            MyBoardScore[i][j][side - 1].info[2].linkNum++;
        }

        if ((i - m >= 0 && j - m >= 0 && RecordBoard[i - m][j - m] != 0) || i - m < 0 || j - m < 0) {
            MyBoardScore[i][j][side - 1].info[2].oppNum++;
        }

        for (m = 1;
             i + m < SIZE && j - m >= 0 && (RecordBoard[i + m][j - m] == side || RecordBoard[i + m][j - m] == side + 2);
             m++) {
            MyBoardScore[i][j][side - 1].info[3].linkNum++;
        }

        if ((i + m < SIZE && j - m >= 0 && RecordBoard[i + m][j - m] != 0) || i + m >= SIZE || j - m < 0) {
            MyBoardScore[i][j][side - 1].info[3].oppNum++;
        }

        for (m = 1;
             i - m >= 0 && j + m < SIZE && (RecordBoard[i - m][j + m] == side || RecordBoard[i - m][j + m] == side + 2);
             m++) {
            MyBoardScore[i][j][side - 1].info[3].linkNum++;
        }

        if ((i - m >= 0 && j + m < SIZE && RecordBoard[i - m][j + m] != 0) || i - m < 0 || j + m >= SIZE) {
            MyBoardScore[i][j][side - 1].info[3].oppNum++;
        }

        for (int n = 0; n < 4; n++) {
            if (MyBoardScore[i][j][side - 1].info[n].linkNum >= 4) {
                MyBoardScore[i][j][side - 1].score += 200;
            } else {
                MyBoardScore[i][j][side - 1].score += (MyBoardScore[i][j][side - 1].info[n].linkNum) *
                                                      (4 - 2 * (MyBoardScore[i][j][side - 1].info[n].oppNum));
            }
        }
    }
}

// Sort descending
int cmpSorter(const void *a, const void *b) {
    int va = ((Sorter *)a)->score;
    int vb = ((Sorter *)b)->score;
    if (va < vb) {
        return 1;
    } else if (va > vb) {
        return -1;
    } else {
        return 0;
    }
}

void chooseMove(int *x, int *y, int side) {
    Sorter sorter[SIZE * SIZE];
    int pos = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MyBoardScore[i][j][side - 1].score >= 0) {
                sorter[pos].i = i;
                sorter[pos].j = j;
                sorter[pos].score = 2 * MyBoardScore[i][j][side - 1].score + 3 * MyBoardScore[i][j][2 - side].score;
                pos++;
            }
        }
    }

    // At least one valid point is found
    if (pos <= 0) {
        printf("No valid point found!\n");
        exit(1);
    }

    qsort(sorter, pos, sizeof(Sorter), cmpSorter);

    // Find the top N elements with the same scores
    int count = 1;
    for (int i = 1; i < pos; i++) {
        if (sorter[i].score == sorter[0].score) {
            count++;
        } else {
            break;
        }
    }

    assert(count >= 1);

    printf("Optional points: \n");

    for (int i = 0; i < count; ++i) {
        putchar(sorter[i].j + 'A');
        printf("%d\n", SIZE - sorter[i].i);
    }

    int random = rand() % count;

    printf("%d\n", random);

    *x = sorter[random].j + 'A';
    *y = SIZE - sorter[random].i;
}

void initialChengwu() {
    Chengwu[0] = false;
    Chengwu[1] = false;

    for (int i = 0; i < 2; i++) {
        ChengwuPoint[i].i = -1;
        ChengwuPoint[i].j = -1;
    }
}

void initialHuosi() {
    Huosi[0] = false;
    Huosi[1] = false;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            HuosiPoint[i][j].i = -1;
            HuosiPoint[i][j].j = -1;
        }
    }
}

void initialChongsi() {
    Chongsi[0] = false;
    Chongsi[1] = false;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ChongsiPoint[i][j].i = -1;
            ChongsiPoint[i][j].j = -1;
        }
    }
}

void checkChengwu(int side) {
    initialChengwu();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < 4; k++) {
                if (MyBoardScore[i][j][side - 1].info[k].linkNum == 4) {
                    ChengwuPoint[side - 1].i = i;
                    ChengwuPoint[side - 1].j = j;

                    Chengwu[side - 1] = true;
                    return;
                }
            }
        }
    }
}

void checkHuosi(int side) {
    int l = 0;

    initialHuosi();

    for (int i = 1; i < SIZE - 1; i++) {
        for (int j = 1; j < SIZE - 1; j++) {
            for (int k = 0; k < 4; k++) {
                if (MyBoardScore[i][j][side - 1].info[k].linkNum == 3 &&
                    MyBoardScore[i][j][side - 1].info[k].oppNum == 0) {
                    if (l < 2) {
                        HuosiPoint[side - 1][l].i = i;
                        HuosiPoint[side - 1][l].j = j;

                        Huosi[side - 1] = true;
                        l++;
                    } else {
                        return;
                    }
                }
            }
        }
    }
}

void checkChongsi(int side) {
    int l = 0;

    initialChongsi();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < 4; k++) {
                if (MyBoardScore[i][j][side - 1].info[k].linkNum == 3 &&
                    MyBoardScore[i][j][side - 1].info[k].oppNum == 1) {
                    if (l < 2) {
                        ChongsiPoint[side - 1][l].i = i;
                        ChongsiPoint[side - 1][l].j = j;

                        Chongsi[side - 1] = true;
                        l++;
                    } else {
                        return;
                    }
                }
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

    // Print scores
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", MyBoardScore[i][j][side - 1].score);
        }
        printf("\n");
    }

    checkChengwu(side);
    checkChengwu(3 - side);

    checkHuosi(side);
    checkHuosi(3 - side);

    checkChongsi(side);
    checkChongsi(3 - side);

    printf("ifChengwu: %d\n", Chengwu[side - 1]);

    printf("ifHuosi: %d\n", Huosi[side - 1]);

    printf("ifChongsi: %d\n", Chongsi[side - 1]);

    if (Chengwu[side - 1]) {
        *x = ChengwuPoint[side - 1].j + 'A';
        *y = SIZE - ChengwuPoint[side - 1].i;

        printf("Chengwu:\n");
        putchar(*x);
        printf("%d\n", *y);
    } else if (Chengwu[2 - side]) {
        *x = ChengwuPoint[2 - side].j + 'A';
        *y = SIZE - ChengwuPoint[2 - side].i;

        printf("oppChengwu:\n");
        putchar(*x);
        printf("%d\n", *y);
    } else if (Huosi[side - 1]) {
        int random;

        do {
            random = rand() % 2;
            *x = HuosiPoint[side - 1][random].j + 'A';
            *y = SIZE - HuosiPoint[side - 1][random].i;
        } while (HuosiPoint[side - 1][random].j < 0);

        printf("Huosi:\n");
        putchar(*x);
        printf("%d\n", *y);
    } else if (Huosi[2 - side]) {
        int random;

        do {
            random = rand() % 2;
            *x = HuosiPoint[2 - side][random].j + 'A';
            *y = SIZE - HuosiPoint[2 - side][random].i;
        } while (HuosiPoint[2 - side][random].j < 0);

        printf("oppHuosi:\n");
        putchar(*x);
        printf("%d\n", *y);
    } else if (Chongsi[side - 1]) {
        int random;

        do {
            random = rand() % 2;
            *x = ChongsiPoint[side - 1][random].j + 'A';
            *y = SIZE - ChongsiPoint[side - 1][random].i;
        } while (ChongsiPoint[side - 1][random].j < 0);

        printf("Chongsi:\n");
        putchar(*x);
        printf("%d\n", *y);
    } else if (Chongsi[2 - side]) {
        int random;

        do {
            random = rand() % 2;
            *x = ChongsiPoint[2 - side][random].j + 'A';
            *y = SIZE - ChongsiPoint[2 - side][random].i;
        } while (ChongsiPoint[2 - side][random].j < 0);

        printf("oppChongsi:\n");
        putchar(*x);
        printf("%d\n", *y);
    } else {
        chooseMove(x, y, side);
    }
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
    // 注释掉下面这个 clear 可以方便调试
    // system("clear");  // 清屏

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