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
	CONSOLE_CURSOR_INFO cursor_info = {1,0};//第二个值为0表示隐藏光标
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
				printf("*");     //显示飞机
			else if((i==*y_bullet)&&(j==*x_bullet)&&(*onfire==1))
				printf("|");    //显示子弹
			else if((i==*y_enemy)&&(j==*x_enemy))
				printf("+");    //显示敌机
			else
				printf(" ");   //显示空地
		}
		if((*y_bullet==*y_enemy)&&(*x_bullet==*x_enemy))   //击中敌机
		{
			*isfill=1;
			*score+=1;
		}
		printf("\n");
	}
	printf("当前得分：%d",*score);
	if(*score==300)
	{
		gotoxy(0,0);
		printf("恭喜通关");
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
		*y_bullet-=1;//在子弹没飞出屏幕的时候，向上移动
	else 
		*onfire=0;//子弹状态
	if(*isfill==0)
	{
		full_velocity+=1;
		if(full_velocity%20==0)//每20次就移动一次，防止速度太快
		*y_enemy+=1;
		if(*y_enemy>=20){
			*x_enemy=rand()%10;//超出界面之后重置敌机
			*y_enemy=0;
			*isfill=0;
		}
	}
	else{
		*x_enemy=rand()%10;//敌机被击中后重置敌机
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
	if(*isfire==1) //如果开火
	{
		*x_bullet=*x_position;
		*y_bullet=*y_position-1;//在飞机上方生成子弹
		*onfire=1;//子弹状态
	}
	*isfire=0;
}
int main()
{
	int score=0;
	int isfire=0,onfire=0;   //isfire控制是否开火，onfire控制子弹是否在飞
	int length=20,high=20;   //画面大小
	int y_bullet=0,x_bullet=0;  //初始化子弹位置
	int y_position=length/2,x_position=high/2;//初始化飞机位置
	int x_enemy=high/2,y_enemy=0,isfill=0;//敌机的位置和命中状态

	HideCursor();

	while(1)
	{
		if(show( &high , &length , &x_position , &y_position , &x_bullet , &y_bullet , &onfire,&y_enemy,&x_enemy,&isfill,&score)/*画面制造函数*/)	goto exit;

		  updata_with_input(&x_position,&y_position,&isfire,&onfire,&x_bullet,&y_bullet);   //有关用户输入的更新

		  updata_without_input(&x_bullet,&y_bullet,&onfire,&y_enemy,&x_enemy,&isfill);  //无关用户输入的更新
	}
	exit:
	return 0;
}