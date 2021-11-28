#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2
void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);


//����ʹ�õ���GBK���룬ÿһ�������ַ�ռ��2���ֽڡ�

//���̻���ģ�� 
char aInitDisplayBoardArray[SIZE][SIZE*CHARSIZE+1] = 
{
		"���өөөөөөөөөөөөө�",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"���۩۩۩۩۩۩۩۩۩۩۩۩۩�"
};
//������������ʾ���� 
char aDisplayBoardArray[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="��";//������;
char play1CurrentPic[]="��"; 

char play2Pic[]="��";//������;
char play2CurrentPic[]="��";

int x[1];
int y[1];

//���������ڼ�¼���̸�� 
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

int detect_whitewin() {
	



	for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE;j++) {
			if (aRecordBoard[i][j]==2) {
					int east=0;
					int west=0;
	int south=0;
	int north=0;
	int NW=0;
	int NE=0;
	int SW=0;
	int SE=0;

				for (int k=1;aRecordBoard[i][j+k]==2 && j+k<15;k++) {
					east++;
				}
				for (int k=1;aRecordBoard[i][j-k]==2 && j-k>=0;k++) {
					west++;
				}
				for (int k=1;aRecordBoard[i-k][j]==2 && i-k>=0;k++) {
					north++;
				}
				for (int k=1;aRecordBoard[i+k][j]==2 && i+k<15;k++) {
					south++;
				}
				for (int k=1;aRecordBoard[i+k][j+k]==2 && i+k<15 && j+k<15;k++) {
					SE++;
				}
				for (int k=1;aRecordBoard[i-k][j+k]==2 && i-k>=0 && j+k<15;k++) {
					NE++;
				}
				for (int k=1;aRecordBoard[i-k][j-k]==2 && i-k>=0 && j-k>=0;k++) {
					NW++;
				}
				for (int k=1;aRecordBoard[i+k][j-k]==2 && i+k<=15 && j-k>=0;k++) {
					SW++;
				}
				if (east+west+1>=5 || north+south+1>=5 || NW+SE+1>=5 || SW+NE+1>=5) {
					return 1;
				}
			}
		}
	}

	return 0;

}

int detect_blackwin() {




	for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE;j++) {
			if (aRecordBoard[i][j]==1) {
					int east=0;
	int west=0;
	int south=0;
	int north=0;
	int NW=0;
	int NE=0;
	int SW=0;
	int SE=0;


				for (int k=1;aRecordBoard[i][j+k]==1 && j+k<15;k++) {
					east++;
				}
				for (int k=1;aRecordBoard[i][j-k]==1 && j-k>=0;k++) {
					west++;
				}
				for (int k=1;aRecordBoard[i-k][j]==1 && i-k>=0;k++) {
					north++;
				}
				for (int k=1;aRecordBoard[i+k][j]==1 && i+k<15;k++) {
					south++;
				}
				for (int k=1;aRecordBoard[i+k][j+k]==1 && i+k<15 && j+k<15;k++) {
					SE++;
				}
				for (int k=1;aRecordBoard[i-k][j+k]==1 && i-k>=0 && j+k<15;k++) {
					NE++;
				}
				for (int k=1;aRecordBoard[i-k][j-k]==1 && i-k>=0 && j-k>=0;k++) {
					NW++;
				}
				for (int k=1;aRecordBoard[i+k][j-k]==1 && i+k<=15 && j-k>=0;k++) {
					SW++;
				}
				if (east+west+1>=5 || north+south+1>=5 || NW+SE+1>=5 || SW+NE+1>=5) {
					return 1;
				}
			}
		}
	}

	return 0;

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
		if (detect_blackwin()) {
			printf("Black wins! ");
			return 0;
		}
		
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
		if (detect_whitewin()) {
			printf("White wins! ");
			return 0;
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

//��ʼ�����̸�� 
void initRecordBorard(void){
//ͨ��˫��ѭ������aRecordBoard��0
	for (int i=0;i<SIZE;++i) {
		for (int j=0;j<SIZE;++j) {
			aRecordBoard[i][j]=0;
		}
	}
}

//��aRecordBoard�м�¼������λ�ã�ת����aDisplayBoardArray��
void recordtoDisplayArray(void){
//��һ������aInitDisplayBoardArray�м�¼�Ŀ����̣����Ƶ�aDisplayBoardArray��
	for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE*CHARSIZE+1;j++){
			aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
		}
	}
//�ڶ�����ɨ��aRecordBoard����������0��Ԫ�أ�������ߡ��Ƶ�aDisplayBoardArray����Ӧλ����
//ע�⣺aDisplayBoardArray����¼���ַ��������ַ���ÿ���ַ�ռ2���ֽڡ���͡�Ҳ�������ַ���ÿ��Ҳռ2���ֽڡ�
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


//��ʾ���̸�� 
void displayBoard(void){
	int i;
	//��һ��������
	system("clear");   //����  
	//�ڶ�������aDisplayBoardArray�������Ļ��
	for (i=0;i<SIZE;i++) {
		printf("%3d%s\n",SIZE-i,aDisplayBoardArray[i]);
	}
	//������������������һ����ĸA B .... 
	printf("   ");
	for (i=0;i<SIZE;i++) {
		printf("%2c",'A'+i);
	}
	printf("\n");
} 

