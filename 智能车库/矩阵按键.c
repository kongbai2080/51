/*
矩阵按键可直接移植使用模块，输入一个整型变量地址，输出按键所代表的值
*/


/* 实现两个数的运算，每个数的位数至少可以八位  */
#include<reg52.h>
typedef unsigned int u16;	//锟斤拷系统默锟斤拷锟斤拷锟斤拷锟斤拷锟酵斤拷锟斤拷锟截讹拷锟斤拷
typedef unsigned char u8;
u16 key;

void delay1(u16 i)
{
	while(i--);
}


void keyscan(u8 *p,u8 *q)
{
	P1=0xfe;   //令第一行为0，然后判断是哪一列按下
	if(P1!=0xfe)
	{
		delay1(1000);
		if(P1!=0xfe)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: *p=0;break;	  //1
				case 0xd0: *p=1;break;	  //2
				case 0xb0: *p=2;break;	  //3
				case 0x70: *p=3;break;	  //加
			}
		}
		while(P1!=0xfe);

	}


	P1=0xfd;				//令第二行为0，判断是哪一列按下
	if(P1!=0xfd)
	{
		delay1(1000);
		if(P1!=0xfd)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: *p=4;break;	  //4
				case 0xd0: *p=5;break;	  //5
				case 0xb0: *p=6;break;	  //6
				case 0x70: *p=7;break;	  //减—
			}	
		}
		while(P1!=0xfd);
		
	}


	P1=0xfb;		 //令第三行为0，判断哪一列按下
	if(P1!=0xfb)
	{
		delay1(1000);
		if(P1!=0xfb)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: *p=8;break;	  //7
				case 0xd0: *p=9;break;	  //8
				case 0xb0: *p=10;break;  //9
				case 0x70: *p=11;break;  //乘*
			}	
		}
		while(P1!=0xfb);
		
	}


	P1=0xf7;		 //令第四行为0，判断哪一列按下
	if(P1!=0xf7)
	{
		delay1(1000);
		if(P1!=0xf7)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: *q=3;break;  //0
				case 0xd0: *q=2;break;  //清除rst
				case 0xb0: *q=1;break;  //等号=
				case 0x70: *q=0;break;  //除/
			}	
		}
		while(P1!=0xf7);
	}
}
