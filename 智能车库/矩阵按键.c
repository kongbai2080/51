/*
���󰴼���ֱ����ֲʹ��ģ�飬����һ�����ͱ�����ַ����������������ֵ
*/


/* ʵ�������������㣬ÿ������λ�����ٿ��԰�λ  */
#include<reg52.h>
typedef unsigned int u16;	//��ϵͳĬ���������ͽ����ض���
typedef unsigned char u8;
u16 key;

void delay1(u16 i)
{
	while(i--);
}


void keyscan(u8 *p,u8 *q)
{
	P1=0xfe;   //���һ��Ϊ0��Ȼ���ж�����һ�а���
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
				case 0x70: *p=3;break;	  //��
			}
		}
		while(P1!=0xfe);

	}


	P1=0xfd;				//��ڶ���Ϊ0���ж�����һ�а���
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
				case 0x70: *p=7;break;	  //����
			}	
		}
		while(P1!=0xfd);
		
	}


	P1=0xfb;		 //�������Ϊ0���ж���һ�а���
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
				case 0x70: *p=11;break;  //��*
			}	
		}
		while(P1!=0xfb);
		
	}


	P1=0xf7;		 //�������Ϊ0���ж���һ�а���
	if(P1!=0xf7)
	{
		delay1(1000);
		if(P1!=0xf7)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: *q=3;break;  //0
				case 0xd0: *q=2;break;  //���rst
				case 0xb0: *q=1;break;  //�Ⱥ�=
				case 0x70: *q=0;break;  //��/
			}	
		}
		while(P1!=0xf7);
	}
}
