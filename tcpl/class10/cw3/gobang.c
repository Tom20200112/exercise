// 1. 支持 3 个级别：
//    1 - 最弱，随机乱走
//    2 - 偏弱，评分取前 N，再随机选一个
//    3 - 最强，在 2 的基础上，加成五、活四、冲四等
// 2. 命令行选择：难度级别、检查禁手、打印 Debug 信息
//    ./gobang level1 debug
// 3. 会把棋谱输出到 ./gobang.log 文件
//
// 术语：禁手(Forbidden)，活四(Open Fours)
#include <assert.h>
#include <ctype.h>
#include <math.h>
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

// Types
typedef struct Connected {
    int color;  // 1 for black, 2 for white
    int x, y;
    int N, S, E, W;
    int NE, NW, SE, SW;
} Connected;

typedef struct Sorter {
    int i, j;
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
    int i, j;
} Point;

// Prototypes
bool forbidden(int x, int y, int side);
void initRecordBoard(void);
void recordToDisplayArray(void);
void displayBoard(void);

int max(int x, int y) { return x > y ? x : y; }

int min(int x, int y) { return x < y ? x : y; }

// Variables
bool Debug = false;
bool Forbidden = false;
int Level = 1;
int Move = 0;

bool Chengwu[2] = {false, false};
bool Huosi[2] = {false, false};
bool Chongsi[2] = {false, false};

Point ChengwuPoint[2] = {{-1, -1}, {-1, -1}};
Point HuosiPoint[2][2] = {{{-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}}};
Point ChongsiPoint[2][2] = {{{-1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}}};
Point ImagineWulian[5] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};

char *Color[2] = {"Black", "White"};
// 注意，枚举值从 1 开始，数组 Color 下标从 0 开始
enum Side { BLACK = 1, WHITE = 2 };

enum Mode { HUMAN2COMPUTER = 1, HUMAN2HUMAN = 2 };

Connected Pieces[SIZE * SIZE];

SingleScore MyBoardScore[SIZE][SIZE][2];
SingleScore ImagineMyBoardScore[SIZE][SIZE][2];

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
int ImagineRecordBoard[SIZE][SIZE];

// 记录棋谱到日志文件 LOG_FILE
void logMove(int currentSide, int x, int y) {
    static FILE *fp = NULL;
    // 第一次调用 lovMove() 时需要先打开文件
    if (fp == NULL) {
        fp = fopen(LOG_FILE, "a");  // 追加模式打开文件，总是在文件尾部追加日志
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

// 获得用户的走子位置
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

    for (int k = 1; x - 'A' + k < SIZE && RecordBoard[SIZE - y][x - 'A' + k] == color; k++) {
        Pieces[temp].E++;
    }
    for (int k = 1; x - 'A' - k >= 0 && RecordBoard[SIZE - y][x - 'A' - k] == color; k++) {
        Pieces[temp].W++;
    }
    for (int k = 1; SIZE - y - k >= 0 && RecordBoard[SIZE - y - k][x - 'A'] == color; k++) {
        Pieces[temp].N++;
    }
    for (int k = 1; SIZE - y + k < SIZE && RecordBoard[SIZE - y + k][x - 'A'] == color; k++) {
        Pieces[temp].S++;
    }
    for (int k = 1; SIZE - y + k < SIZE && x - 'A' + k < SIZE && RecordBoard[SIZE - y + k][x - 'A' + k] == color; k++) {
        Pieces[temp].SE++;
    }
    for (int k = 1; SIZE - y - k >= 0 && x - 'A' + k < SIZE && RecordBoard[SIZE - y - k][x - 'A' + k] == color; k++) {
        Pieces[temp].NE++;
    }
    for (int k = 1; SIZE - y - k >= 0 && x - 'A' - k >= 0 && RecordBoard[SIZE - y - k][x - 'A' - k] == color; k++) {
        Pieces[temp].NW++;
    }
    for (int k = 1; SIZE - y + k < SIZE && x - 'A' - k >= 0 && RecordBoard[SIZE - y + k][x - 'A' - k] == color; k++) {
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

void evaluateScore(int i, int j, int side) {
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

    if (Debug) {
        printf("Optional points: \n");
        for (int i = 0; i < count; ++i) {
            putchar(sorter[i].j + 'A');
            printf("%d\n", SIZE - sorter[i].i);
        }
    }

    int random = rand() % count;
    *x = sorter[random].j + 'A';
    *y = SIZE - sorter[random].i;
}

void initialChengwu(int side) {
    Chengwu[side - 1] = false;

    ChengwuPoint[side - 1].i = -1;
    ChengwuPoint[side - 1].j = -1;
}

void initialHuosi(int side) {
    Huosi[side - 1] = false;

    HuosiPoint[side - 1][0].i = -1;
    HuosiPoint[side - 1][0].j = -1;

    HuosiPoint[side - 1][1].i = -1;
    HuosiPoint[side - 1][1].j = -1;
}

void initialChongsi(int side) {
    Chongsi[side - 1] = false;

    for (int j = 0; j < 2; j++) {
        ChongsiPoint[side - 1][j].i = -1;
        ChongsiPoint[side - 1][j].j = -1;
    }
}

void checkChengwu(int side) {
    initialChengwu(side);

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

    initialHuosi(side);

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

    initialChongsi(side);

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
    switch (Level) {
        case 1:
            do {
                *x = random() % SIZE;
                *y = random() % SIZE;
            } while (!isValid(*x + 'A', *y));
            *x += 'A';
            break;
        case 2:
        case 3:
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    evaluateScore(i, j, side);
                    evaluateScore(i, j, 3 - side);
                }
            }

            if (Level == 2) {
                chooseMove(x, y, side);
                return;
            }

            checkChengwu(side);
            checkChengwu(3 - side);

            checkHuosi(side);
            checkHuosi(3 - side);

            checkChongsi(side);
            checkChongsi(3 - side);

            if (Debug) {
                printf("ifChengwu: %d\n", Chengwu[side - 1]);
                printf("Chengwu:");
                printf("(%d, %d)\n", ChengwuPoint[side - 1].i, ChengwuPoint[side - 1].j);
                printf("oppChengwu:(%d, %d)\n", ChengwuPoint[2 - side].i, ChengwuPoint[2 - side].j);

                printf("ifHuosi: %d\n", Huosi[side - 1]);
                printf("Huosi:");
                printf("(%d, %d)\n", HuosiPoint[side - 1][0].i, HuosiPoint[side - 1][0].j);
                printf("oppHuosi:(%d, %d)\n", HuosiPoint[2 - side][0].i, HuosiPoint[2 - side][0].j);

                printf("ifChongsi: %d\n", Chongsi[side - 1]);
                printf("Chongsi:");
                printf("(%d, %d)\n", ChongsiPoint[side - 1][0].i, ChongsiPoint[side - 1][0].j);
                printf("oppChongsi:(%d, %d)\n", ChongsiPoint[2 - side][0].i, ChongsiPoint[2 - side][0].j);
            }

            if (Chengwu[side - 1]) {
                *x = ChengwuPoint[side - 1].j + 'A';
                *y = SIZE - ChengwuPoint[side - 1].i;
            } else if (Chengwu[2 - side]) {
                *x = ChengwuPoint[2 - side].j + 'A';
                *y = SIZE - ChengwuPoint[2 - side].i;
            } else if (Huosi[side - 1]) {
                int random;
                do {
                    random = rand() % 2;
                    *x = HuosiPoint[side - 1][random].j + 'A';
                    *y = SIZE - HuosiPoint[side - 1][random].i;
                } while (HuosiPoint[side - 1][random].j < 0);
            } else if (Huosi[2 - side]) {
                int random;
                do {
                    random = rand() % 2;
                    *x = HuosiPoint[2 - side][random].j + 'A';
                    *y = SIZE - HuosiPoint[2 - side][random].i;
                } while (HuosiPoint[2 - side][random].j < 0);
            } else if (Chongsi[side - 1]) {
                int random;
                do {
                    random = rand() % 2;
                    *x = ChongsiPoint[side - 1][random].j + 'A';
                    *y = SIZE - ChongsiPoint[side - 1][random].i;
                } while (ChongsiPoint[side - 1][random].j < 0);
            } else if (Chongsi[2 - side]) {
                int random;
                do {
                    random = rand() % 2;
                    *x = ChongsiPoint[2 - side][random].j + 'A';
                    *y = SIZE - ChongsiPoint[2 - side][random].i;
                } while (ChongsiPoint[2 - side][random].j < 0);
            } else {
                chooseMove(x, y, side);
            }
            break;
        default:
            assert(false);
            break;
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
bool humanNextMove(int side) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            evaluateScore(i, j, side);
            evaluateScore(i, j, 3 - side);
        }
    }

    int x, y;

    printf("%s to move: ", Color[side - 1]);
    do {
        getPosition(&x, &y);
        if (!isValid(x, y)) {
            printf("Not a valid position. Retype: ");
        } else if (forbidden(x, y, side)) {
            printf("Aha! This move is forbidden. Retype: ");
        } else {
            break;
        }
    } while (true);

    // Log the move
    logMove(side, x, y);

    if (x == 'R') {  // take back
        takeBackMove(side);
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

    return false;
}

// return: if is a take back?
bool human2HumanNextMove(int side) { return humanNextMove(side); }

// 判断side方在(x, y)落子后是否形成五连
bool wulian(int x, int y, int side) {
    for (int k = 0; k < 4; k++) {
        if (MyBoardScore[SIZE - y][x - 'A'][side - 1].info[k].linkNum == 4) {
            return true;
        }
    }

    return false;
}

// 判断side方在(x, y)落子后是否形成长连
bool changlian(int x, int y, int side) {
    for (int k = 0; k < 4; k++) {
        if (MyBoardScore[SIZE - y][x - 'A'][side - 1].info[k].linkNum > 4) {
            return true;
        }
    }

    return false;
}

// 判断 side 方在(x, y)落子后是否形成三三
bool sansan(int x, int y, int side) {
    int sansanCount = 0;

    for (int k = 0; k < 4; k++) {
        if (MyBoardScore[SIZE - y][x - 'A'][side - 1].info[k].linkNum == 2 &&
            MyBoardScore[SIZE - y][x - 'A'][side - 1].info[k].oppNum == 0) {
            sansanCount++;
        }
    }

    return sansanCount > 1;
}

void copyBoard(int a[SIZE][SIZE], int b[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            b[i][j] = a[i][j];
        }
    }
}

void imagineEvaluateScore(int i, int j, int side) {
    for (int m = 0; m < 4; ++m) {
        ImagineMyBoardScore[i][j][side - 1].info[m].linkNum = 0;
        ImagineMyBoardScore[i][j][side - 1].info[m].oppNum = 0;
    }

    ImagineMyBoardScore[i][j][side - 1].score = 0;

    if (ImagineRecordBoard[i][j] != 0) {
        ImagineMyBoardScore[i][j][side - 1].score = -1;
    } else {
        int m = 1;
        for (; j + m < SIZE && (ImagineRecordBoard[i][j + m] == side || ImagineRecordBoard[i][j + m] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[0].linkNum++;
        }

        if ((j + m < SIZE && ImagineRecordBoard[i][j + m] != 0) || j + m >= SIZE) {
            ImagineMyBoardScore[i][j][side - 1].info[0].oppNum++;
        }

        for (m = 1; j - m >= 0 && (ImagineRecordBoard[i][j - m] == side || ImagineRecordBoard[i][j - m] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[0].linkNum++;
        }

        if ((j - m >= 0 && ImagineRecordBoard[i][j - m] != 0) || j - m < 0) {
            ImagineMyBoardScore[i][j][side - 1].info[0].oppNum++;
        }

        for (m = 1; i + m < SIZE && (ImagineRecordBoard[i + m][j] == side || ImagineRecordBoard[i + m][j] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[1].linkNum++;
        }

        if ((i + m < SIZE && ImagineRecordBoard[i + m][j] != 0) || i + m >= SIZE) {
            ImagineMyBoardScore[i][j][side - 1].info[1].oppNum++;
        }

        for (m = 1; i - m >= 0 && (ImagineRecordBoard[i - m][j] == side || ImagineRecordBoard[i - m][j] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[1].linkNum++;
        }

        if ((i - m >= 0 && ImagineRecordBoard[i - m][j] != 0) || i - m < 0) {
            ImagineMyBoardScore[i][j][side - 1].info[1].oppNum++;
        }

        for (m = 1; i + m < SIZE && j + m < SIZE &&
                    (ImagineRecordBoard[i + m][j + m] == side || ImagineRecordBoard[i + m][j + m] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[2].linkNum++;
        }

        if ((i + m < SIZE && j + m < SIZE && ImagineRecordBoard[i + m][j + m] != 0) || i + m >= SIZE || j + m >= SIZE) {
            ImagineMyBoardScore[i][j][side - 1].info[2].oppNum++;
        }

        for (m = 1; i - m >= 0 && j - m >= 0 &&
                    (ImagineRecordBoard[i - m][j - m] == side || ImagineRecordBoard[i - m][j - m] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[2].linkNum++;
        }

        if ((i - m >= 0 && j - m >= 0 && ImagineRecordBoard[i - m][j - m] != 0) || i - m < 0 || j - m < 0) {
            ImagineMyBoardScore[i][j][side - 1].info[2].oppNum++;
        }

        for (m = 1; i + m < SIZE && j - m >= 0 &&
                    (ImagineRecordBoard[i + m][j - m] == side || ImagineRecordBoard[i + m][j - m] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[3].linkNum++;
        }

        if ((i + m < SIZE && j - m >= 0 && ImagineRecordBoard[i + m][j - m] != 0) || i + m >= SIZE || j - m < 0) {
            ImagineMyBoardScore[i][j][side - 1].info[3].oppNum++;
        }

        for (m = 1; i - m >= 0 && j + m < SIZE &&
                    (ImagineRecordBoard[i - m][j + m] == side || ImagineRecordBoard[i - m][j + m] == side + 2);
             m++) {
            ImagineMyBoardScore[i][j][side - 1].info[3].linkNum++;
        }

        if ((i - m >= 0 && j + m < SIZE && ImagineRecordBoard[i - m][j + m] != 0) || i - m < 0 || j + m >= SIZE) {
            ImagineMyBoardScore[i][j][side - 1].info[3].oppNum++;
        }
    }
}

bool imagineWulian(int i, int j, int side) {
    for (int k = 0; k < 4; k++) {
        if (ImagineMyBoardScore[i][j][side - 1].info[k].linkNum == 4) {
            return true;
        }
    }

    return false;
}

bool coincideWulian(Point point1, Point point2, int side) {
    if (point1.i == point2.i) {
        for (int i = 1; i < 5; i++) {
            if (ImagineRecordBoard[point1.i][min(point1.j, point2.j) + i] != side) {
                return false;
            }
        }
    } else if (point1.j == point2.j) {
        for (int i = 1; i < 5; i++) {
            if (ImagineRecordBoard[min(point1.i, point2.i) + i][point1.j] != side) {
                return false;
            }
        }

    } else if (point2.j - point1.j == point2.i - point1.i && point2.j - point1.j == 5) {
        for (int i = 1; i < 5; i++) {
            if (ImagineRecordBoard[point1.i + i][point1.j + i] != side) {
                return false;
            }
        }
    } else if (point2.j - point1.j == point1.i - point2.i && point2.j - point1.j == 5) {
        for (int i = 1; i < 5; i++) {
            if (ImagineRecordBoard[point1.i - i][point1.j + i] != side) {
                return false;
            }
        }
    } else if (point1.j - point2.j == point1.i - point2.i && point1.j - point2.j == 5) {
        for (int i = 1; i < 5; i++) {
            if (ImagineRecordBoard[point1.i - i][point1.j - i] != side) {
                return false;
            }
        }
    } else if (point1.j - point2.j == point2.i - point1.i && point1.j - point2.j == 5) {
        for (int i = 1; i < 5; i++) {
            if (ImagineRecordBoard[point1.i + i][point1.j - i] != side) {
                return false;
            }
        }
    }

    return true;
}

// 判断side方在(x, y)落子后是否形成四四
bool sisi(int x, int y, int side) {
    int sisiCount = 0;

    copyBoard(RecordBoard, ImagineRecordBoard);

    ImagineRecordBoard[SIZE - y][x - 'A'] = side;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            imagineEvaluateScore(i, j, side);
            if (imagineWulian(i, j, side) && sisiCount < 5) {
                ImagineWulian[sisiCount].i = i;
                ImagineWulian[sisiCount].j = j;
                sisiCount++;
            }
        }
    }

    return sisiCount > 2 || (sisiCount == 2 && !coincideWulian(ImagineWulian[0], ImagineWulian[1], side));
}

// 禁手只对黑方管用
bool forbidden(int x, int y, int side) {
    // 禁手功能没有打开
    if (!Forbidden) {
        return false;
    }

    return side == BLACK && !wulian(x, y, side) && (changlian(x, y, side) || sansan(x, y, side) || sisi(x, y, side));
}

// return value: if is a take back?
bool human2ComputerNextMove(int side, int currentSide) {
    if (side == currentSide) {
        return humanNextMove(side);
    }

    printf("Computer thinking...\n");
    int x, y;
    thinkPosition(&x, &y, currentSide);

    // Log the move
    logMove(currentSide, x, y);

    addConnected(currentSide, x, y);

    RecordBoard[SIZE - y][x - 'A'] = currentSide + 2;
    recordToDisplayArray();
    displayBoard();

    printf("Computer's move: %c%d\n", x, y);

    refresh();

    if (detectWin(currentSide)) {
        printf("%s wins!\n", Color[currentSide - 1]);
        exit(1);
    }

    return false;
}

// Clear stdin when scanf() failed to read wanted result.
void clearStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

void getOptions(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *option = argv[i];
        if (strcmp(option, "debug") == 0) {
            Debug = true;
        } else if (strcmp(option, "level1") == 0) {
            Level = 1;
        } else if (strcmp(option, "level2") == 0) {
            Level = 2;
        } else if (strcmp(option, "level3") == 0) {
            Level = 3;
        } else if (strcmp(option, "forbidden") == 0) {
            Forbidden = true;
        } else {
            printf("Invalid option: %s, ignore it\n", option);
        }
    }

    printf("Use options: level=%d, forbidden=%s, debug=%s\n", Level, Forbidden ? "true" : "false",
           Debug ? "true" : "false");
    assert(Level >= 1 && Level <= 3);
}

int main(int argc, char *argv[]) {
    int mode, side;

    initRecordBoard();
    recordToDisplayArray();
    displayBoard();

    // 处理命令行参数
    getOptions(argc, argv);

    while (true) {
        printf("Please choose your mode(1 for human vs computer, 2 for human vs human): ");
        if (scanf("%d", &mode) == EOF) {
            printf("Got EOF, exit\n");
            exit(1);
        }

        if (mode == HUMAN2COMPUTER || mode == HUMAN2HUMAN) {
            break;
        }

        clearStdin();
    }

    if (mode == HUMAN2COMPUTER) {
        while (true) {
            printf("Please choose your side(1 for black, 2 for white): ");
            if (scanf("%d", &side) == EOF) {
                printf("Got EOF, exit\n");
                exit(1);
            }

            if (side == BLACK || side == WHITE) {
                break;
            }

            clearStdin();
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
    if (!Debug) {
        system("clear");  // 清屏
    }

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
