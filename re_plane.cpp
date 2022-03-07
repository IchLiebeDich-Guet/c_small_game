#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>

void gotoxy(int x,int y)
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void HideCursor(void)
{
	CONSOLE_CURSOR_INFO cursor_info = {1,0};//�ڶ���ֵΪ0��ʾ���ع��
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

int show(int *high,int *length,int *x_position,int *y_position,int *x_bullet,int *y_bullet,int *onfire,int *y_enemy,int *x_enemy,int *isfill,int *score)
{
	gotoxy(0,0);
	for(int i=0;i<*high;i++)
	{
		for(int j=0;j<*length;j++)
		{
			if((i==*y_position)&&(j==*x_position))
				printf("*");     //��ʾ�ɻ�
			else if((i==*y_bullet)&&(j==*x_bullet)&&(*onfire==1))
				printf("|");    //��ʾ�ӵ�
			else if((i==*y_enemy)&&(j==*x_enemy))
				printf("+");    //��ʾ�л�
			else
				printf(" ");   //��ʾ�յ�
		}
		if((*y_bullet==*y_enemy)&&(*x_bullet==*x_enemy))   //���ел�
		{
			*isfill=1;
			*score+=1;
		}
		printf("\n");
	}
	printf("��ǰ�÷֣�%d",*score);
	if(*score==300)
	{
		gotoxy(0,0);
		printf("��ϲͨ��");
		system("pause");
		return 1;
	}
	return 0;
}
void updata_without_input(int *x_bullet,int *y_bullet,int *onfire,int *y_enemy,int *x_enemy,int* isfill)
{
	static int full_velocity=0;
	srand(time(0));
	if(*y_bullet>-1) 
		*y_bullet-=1;//���ӵ�û�ɳ���Ļ��ʱ�������ƶ�
	else 
		*onfire=0;//�ӵ�״̬
	if(*isfill==0)
	{
		full_velocity+=1;
		if(full_velocity%20==0)//ÿ20�ξ��ƶ�һ�Σ���ֹ�ٶ�̫��
		*y_enemy+=1;
		if(*y_enemy>=20){
			*x_enemy=rand()%10;//��������֮�����õл�
			*y_enemy=0;
			*isfill=0;
		}
	}
	else{
		*x_enemy=rand()%10;//�л������к����õл�
		*y_enemy=0;
		*isfill=0;
	}
	
}
void updata_with_input(int *x_position,int *y_position,int *isfire,int *onfire ,int *x_bullet,int *y_bullet)
{
	if(kbhit())
	{
		switch(getch())
		{
			case 'a' : *x_position-=1;
				break;
			case 'd' : *x_position+=1;
				break;
			case 'w' : *y_position-=1;
				break;
			case 's' : *y_position+=1;
				break;
			default  : *isfire=1;
		}
	}
	if(*isfire==1) //�������
	{
		*x_bullet=*x_position;
		*y_bullet=*y_position-1;//�ڷɻ��Ϸ������ӵ�
		*onfire=1;//�ӵ�״̬
	}
	*isfire=0;
}
int main()
{
	int score=0;
	int isfire=0,onfire=0;   //isfire�����Ƿ񿪻�onfire�����ӵ��Ƿ��ڷ�
	int length=20,high=20;   //�����С
	int y_bullet=0,x_bullet=0;  //��ʼ���ӵ�λ��
	int y_position=length/2,x_position=high/2;//��ʼ���ɻ�λ��
	int x_enemy=high/2,y_enemy=0,isfill=0;//�л���λ�ú�����״̬

	HideCursor();

	while(1)
	{
		if(show( &high , &length , &x_position , &y_position , &x_bullet , &y_bullet , &onfire,&y_enemy,&x_enemy,&isfill,&score)/*�������캯��*/)	goto exit;

		  updata_with_input(&x_position,&y_position,&isfire,&onfire,&x_bullet,&y_bullet);   //�й��û�����ĸ���

		  updata_without_input(&x_bullet,&y_bullet,&onfire,&y_enemy,&x_enemy,&isfill);  //�޹��û�����ĸ���
	}
	exit:
	return 0;
}