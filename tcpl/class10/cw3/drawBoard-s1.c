#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);


//棋盘使用的是GBK编码，每一个中文字符占用2个字节。

//棋盘基本模板 
char aInitDisplayBoardArray[SIZE][SIZE*CHARSIZE+1] = 
{
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
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组用于显示棋盘 
char aDisplayBoardArray[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="●";//黑棋子;
char play1CurrentPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurrentPic[]="△";

int x[1];
int y[1];

//此数组用于记录棋盘格局 
int aRecordBoard[SIZE][SIZE];

void getposition(int x[], int y[]) {
	int c;
	y[0]=0;

	if ((x[0]=getchar())>='a'&&x[0]<='z') {
		x[0]=x[0]-'a'+'A';
	}

	while ((c=getchar())!=EOF &&c!='\t'&&c!='\n'&&c!=' ') {
		y[0]*=10;
		y[0]+=c-'0';
	}
}

void refresh(void) {
	for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE;j++) {
			if (aRecordBoard[i][j]==3){
				aRecordBoard[i][j]=1;
			} else if (aRecordBoard[i][j]==4) {
				aRecordBoard[i][j]=2;
			}
		}
	}
}



int main()

{
	int move = 0;
	initRecordBorard();
	recordtoDisplayArray();
    displayBoard();


	while (move >=0) {
		printf("Black to move: ");
		getposition(x,y);
		while ((!(((x[0]>='a'&&x[0]<='z')|(x[0]>='A'&&x[0]<='Z'))&&(y[0]>=1&&y[0]<=15)))|aRecordBoard[SIZE-y[0]][x[0]-'A']!=0) {
			printf("Not a valid position. Retype: ");
			getposition(x,y);
		}
		aRecordBoard[SIZE-y[0]][x[0]-'A']=3;
		recordtoDisplayArray();
		displayBoard();
		refresh();
		
		printf("White to move: ");
		getposition(x,y);
		while ((!(((x[0]>='a'&&x[0]<='z')|(x[0]>='A'&&x[0]<='Z'))&&(y[0]>=1&&y[0]<=15)))|aRecordBoard[SIZE-y[0]][x[0]-'A']!=0) {
			printf("Not a valid position. Retype: ");
			getposition(x,y);
		}
		aRecordBoard[SIZE-y[0]][x[0]-'A']=4;
		recordtoDisplayArray();
		displayBoard();
		move++;

		refresh();
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

//初始化棋盘格局 
void initRecordBorard(void){
//通过双重循环，将aRecordBoard清0
	for (int i=0;i<SIZE;++i) {
		for (int j=0;j<SIZE;++j) {
			aRecordBoard[i][j]=0;
		}
	}
}

//将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
void recordtoDisplayArray(void){
//第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
	for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE*CHARSIZE+1;j++){
			aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
		}
	}
//第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
    for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE; j++) {
			if (aRecordBoard[i][j]==1) {
				aDisplayBoardArray[i][CHARSIZE*j]=play1Pic[0];
				aDisplayBoardArray[i][CHARSIZE*j+1]=play1Pic[1];
				if (CHARSIZE==3) {
					aDisplayBoardArray[i][CHARSIZE*j+2]=play1Pic[2];
				}
			} else if (aRecordBoard[i][j]==2) {
				aDisplayBoardArray[i][CHARSIZE*j]=play2Pic[0];
				aDisplayBoardArray[i][CHARSIZE*j+1]=play2Pic[1];
				if (CHARSIZE==3) {
					aDisplayBoardArray[i][CHARSIZE*j+2]=play2Pic[2];
				}
			
			} else if (aRecordBoard[i][j]==3) {
				aDisplayBoardArray[i][CHARSIZE*j]=play1CurrentPic[0];
				aDisplayBoardArray[i][CHARSIZE*j+1]=play1CurrentPic[1];
				if (CHARSIZE==3) {
					aDisplayBoardArray[i][CHARSIZE*j+2]=play1CurrentPic[2];
				}
			
			} else if (aRecordBoard[i][j]==4) {
				aDisplayBoardArray[i][CHARSIZE*j]=play2CurrentPic[0];
				aDisplayBoardArray[i][CHARSIZE*j+1]=play2CurrentPic[1];
				if (CHARSIZE==3) {
					aDisplayBoardArray[i][CHARSIZE*j+2]=play2CurrentPic[2];
				}
			
			}

		} 
	}
}


//显示棋盘格局 
void displayBoard(void){
	int i;
	//第一步：清屏
	system("clear");   //清屏  
	//第二步：将aDisplayBoardArray输出到屏幕上
	for (i=0;i<SIZE;i++) {
		printf("%3d%s\n",SIZE-i,aDisplayBoardArray[i]);
	}
	//第三步：输出最下面的一行字母A B .... 
	printf("   ");
	for (i=0;i<SIZE;i++) {
		printf("%2c",'A'+i);
	}
	printf("\n");
} 

