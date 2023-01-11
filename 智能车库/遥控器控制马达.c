/*
ʵ��˵����
	�����Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
	����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע�͡�
ʵ����ߣ�
	1��LCD1602Һ��ģ��-->��Ƭ���ܽ�
	�ο�LCD1602Һ����ʾʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	2���������ģ��-->��Ƭ���ܽ� 
	�ο�����ң��ʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	J11-->P32
	3��ֱ�����ģ��-->��Ƭ���ܽ�
	IN1-->P10���ο�ֱ�����ʵ����ߣ�
	
ʵ������
	LCD1602��ʾң������ֵ��������ʾ���״̬��ON/OFF����ͨ������һ���������Կ�������������ֹͣ
*/


#include<reg52.h>
//#include"lcd.h"
#include <intrins.h>
#include "JZAJ.h"
#include "LCD1602.h"
#include "24c02.h"
sbit HWL=P3^4;
sbit HWR=P3^3;
sbit beep=P3^5;		
sbit PWM=P3^7;
u8 mm=0,ms=1,mmdat=-1;

u16 count1=0,count2=0;	 //��Ϊ��ʱ����8����㣬����Ҫһ�����Ķ�ʱ��count2
u8 dat[]="0123456789";
//u8 d1[8]=" RED IR ";

u8 num[16]="0123456789ABCDEF";
u8 timer1;
//u8 d2[7]="STATE:";	 ��Ϊ����б����ڴ�ռ����ƣ�dataֻ��<=125(Ӧ����)���������޷�����.hex�ļ�
//					��ֻ��ɾ�����ñ��������ͷſռ䣬���߽������ڴ�ռ��Ĭ��smallģʽ����largeģʽ���ռ��󣬵������࣬�ٶȱ�����һ�㲻�Ƽ���															
u8 irtime;	  //����ʱ��
u8 startflag;  //��ʼ���
u8 bitnum;	  //bitλ��
u8 irdata[33]; //�Ĵ�û�и�bit��ʱ��
u8 irreceok;	  //������ɱ�־λ
u8 ircode[4];  //�����յ��ĸ��ֽ����ݱ���
u8 irprosok;	  //�������ݴ����־λ
u8 irdisp[8];  //���ĸ��ֽڵ�16����ͨ��ת�����ʮ���ƴ洢
u8 dat1,dat2;
u8 keynum;
//u8 motoflag;
void delay(u16 i)
{
	while(i--);
}

/*��ʱ����*/
void delay1s(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}
void delayms(u16 i)
{
	u16 x,y;
	for(x=i;x>0;x--) for(y=110;y>0;y--);
}


/*void lcdwrc(u8 c)
{
	lcd1602_write_cmd(c);
}
void lcdwrd(u8 dat)
{
	lcd1602_write_data(dat);	
}
void lcd_init()
{
	lcd1602_init();
}  */
void int0init()
{
	EA=1;
	EX0=1;
	IT0=1;
}
void time0init()
{
	TMOD=0X02;	//���ö�ʱ��0ģʽ2.��ģʽΪ�Զ�װ��ģʽ
	TH0=0X00;
	TL0=0X00;//�趨������ֵ��ÿ��TL0������255ʱ��TH0�����Լ������ݸ�TL0�������¼���
	TR0=1;
	ET0=1;
	EA=1;	
}
void irpros() //����������ݴ��� ������������0����1
{
	u8 i,j,value;
	u8 k=1;	//������ȥ����������k=1��
	for(j=0;j<4;j++)  //ȡ����һ֡�����е��ĸ��ֽ�����
	{
		for(i=0;i<8;i++)	//ȡ����һ���ֽڵ�����		
		{
			value>>=1;
			if(irdata[k]>6)
			{
				value=value|0x80;
			}
			k++;
		}
		ircode[j]=value;
	}
	irprosok=1;
}
void irwork()  //���ĸ��ֽڵ�����ת����10��������ʾ
{
	irdisp[0]=ircode[0]/16;//��ʾ��λ
	irdisp[1]=ircode[0]%16;//��ʾ��λ  
	irdisp[2]=ircode[1]/16;
	irdisp[3]=ircode[1]%16;	 //ͬһ��ң������2���ֽڵ������������ǲ���ı�ģ��ı��ֻ�������뼰����
	irdisp[4]=ircode[2]/16;
	irdisp[5]=ircode[2]%16;
	irdisp[6]=ircode[3]/16;
	irdisp[7]=ircode[3]%16;
	dat1=irdisp[4];
	dat2=irdisp[5];
	keynum++;
}
void display()
{
/*	u8 i;
	lcd1602_write_cmd(0x00+0x80);
	for(i=0;i<8;i++)
	{
		lcdwrd(d1[i]);
	}	 
	for(i=4;i<6;i++)
	{
		lcd1602_write_data(num[irdisp[i]]);	   
	} 			 							*/
/*	lcdwrc(0x40+0x80);
	for(i=0;i<6;i++)
	{
		lcdwrd(d2[i]);
	}
	if(motoflag==1)			//���״̬��ʾ
	{
		lcdwrd('O');
		lcdwrd('N');
		lcdwrd(' ');
	}
	else
	{
		lcdwrd('O');
		lcdwrd('F');
		lcdwrd('F');	
	}  */
}
void motopros()
{
	if((dat1==4)&&(dat2==5)&&(keynum==1))  //���µ�һ�ΰ��������
	{			
		dat1=0;
		dat2=0;
//		motoflag=1;
		timer1 =2;//����ָ���0���λ��
		irtime=0;//�ö�ʱ�����¼���		   
//		delay1s();						   //����ȥ���ӳٺ�������������
	}
			
	if((dat1==4)&&(dat2==5)&&(keynum>=2))	//���µڶ��δ˰���ʱ�ر����
	{
		dat1=0;
		dat2=0;
		keynum=0;
//		motoflag=0;
		timer1 =6;//�����ת90��
		irtime=0;
//		delay1s();						  //����ȥ���ӳٺ�������������
	}				
}
void main()
{
//	moto=0;	
	u8 swi,gwi;
//	u8 mm2=0;
	u16 sw,gw;
//	u16 ii;
//	lcd_init();
	lcd1602_init();
	lcd1602_show_string(4,0,"znck");
	int0init();
	time0init();

	mm=at24c02_read_one_byte(2);
	if((mm<0)||(mm>9)) mm=0;
	while(1)
	{
	keyscan(&mmdat,&ms);
	

	if(ms==1) 
	{	
		lcd1602_show_string(13,1,"ms1");
		lcd1602_show_string(10,0,"       ");
		lcd1602_show_string(0,1,"        ");//����������ģʽ����ģʽ1��Ӱ��
		while(1)
		{
			keyscan(&mmdat,&ms);
			if(ms!=1) break;
			if((HWL!=0)||(HWR!=0))
			{
				
				timer1 =2;//����ָ���0���λ��
				irtime=0;//�ö�ʱ�����¼���
				delay1s();
				while((HWL!=0)||(HWR!=0));
				if((HWL==0)||(HWR==0))
				{
				timer1 =6;//�����ת90��
				irtime=0;
				}
				 
			}
				
		}
     }
	if(ms==2)
	{
		while(1)
		{
		if(ms==1||ms==3) break;
		lcd1602_show_string(13,1,"ms2");
		lcd1602_show_string(0,1,"qsrmm    "); 
		lcd1602_show_string(10,0,"      ");//����������ģʽ����ģʽ2��Ӱ�� 
		mmdat=-1;
		keyscan(&mmdat,&ms);
		if(mm==mmdat) 
		{
			timer1 =2;//����ָ���0���λ��
			irtime=0;//�ö�ʱ�����¼���
			delay1s();
			count1=0;
			while(1)   
			{
				mmdat=-1;
				if (count1 >=14000) 							   		//��Ϊʹ�õ��Ƕ�ʱ����ģʽ1��ÿ���������Ҫ�����װ��ֵ�����Ի�����ϴ�����ʱ���ж�ÿ3�����2��������һ������Ԥ��ʱ�䣬���������ԭ����50000��5�룩������30000��3�룩
				{ 
					count2=0;											//��count1��2��������ٽ���ѭ�����¼�ʱ
					count1=0;
					while(count2<4)							 
					{
						if(count1>=20000)								//���������������ĳ������T0��ʼ����ᵼ���жϼ�����������ֹͣ
						{
						count1=0;
						count2++;
						}
						if(mm==mmdat) break; 
						sw=(20-(count2*5+count1/4000))/10;		//14000ƽ���ֳ�5��		
						swi=dat[sw];
						gw=(20-(count2*5+count1/4000))%10;
						gwi=dat[gw];
						lcd1602_show_string(10,0,"djs"); 
						lcd1602_show_string(13,0,&swi);
						lcd1602_show_string(14,0,&gwi);
						lcd1602_show_string(15,0,"s");
						keyscan(&mmdat,&ms);					 //����mmdat����ԭ����ΪҪ���Ź���
					}

					while(count2>=4)						 
					{
					count2=5;								 //��ʱ���������Զ����㣬����Ҫ�ó���ͣ�ڴ�ѭ���ͱ�����count2����һ���ϴ����
					keyscan(&mmdat,&ms);						 //����mmdat����ԭ����ΪҪ���Ź���
					if(mm==mmdat) break;
					beep=!beep;
					delayms(1);
					}											 
					beep=0;
				}
				keyscan(&mmdat,&ms);
				if(mm==mmdat) break;
 
				if(ms==0) 									//��ģʽ2��ֻ�п����Ų��ܸ�����
				{
					lcd1602_show_string(13,1,"ms2");
					lcd1602_show_string(0,1,"qsrxmm");
					lcd1602_show_string(10,0,"      ");//����������ģʽ����ģʽ0��������ģʽ����Ӱ��
					mmdat=mm;
 				//	mmdat=-1;		   ���ﱾ����Ҫ��ԭmmdat�ģ���Ҫ�����룬����������mmһ����Ȼ�����ѭ����ȡ��������˳�
					while(mmdat==mm) keyscan(&mmdat,&ms);
					mm=mmdat;
			/*		while(1)
					{
					if(mm==dat[ii])
					{
					mm2=dat[ii];
					ii=0;
					break;
					}
					ii++;
					}  */
					at24c02_write_one_byte(2,mm);			 
					lcd1602_show_string(0,1,"ygxmm ");		//˳����ʾ���˳�ѭ��
					lcd1602_show_string(10,0,"      ");//����������ģʽ����ģʽ0��������ģʽ����Ӱ��
					delayms(1000);
					mmdat=-1;
					ms=2;									//����������˳�������ģʽ���ص�ģʽ2
				}
				lcd1602_show_string(0,1,"qsrmm ");
				lcd1602_show_string(10,0,"      ");//����������ģʽ����ģʽ2��Ӱ��
			}
			timer1 =6;//�����ת90��
			irtime=0;
			}
		}
	}
	if(ms==3) 
	{	
		lcd1602_show_string(13,1,"ms3");
		lcd1602_show_string(10,0,"       ");
		lcd1602_show_string(0,1,"        ");//����������ģʽ����ģʽ3��Ӱ��	  

		while(1)
		{
		keyscan(&mmdat,&ms);
		if(ms!=3) break; 
		if(irreceok)
		{
			irreceok=0;
			irpros();
			if(irprosok)
			{
				irwork();
			}
		}
		motopros();
		display();
		}
	}
}
}		
 
void time0() interrupt 1
{
	irtime++;//ÿ����һ�ξ�˵����ʱʱ��Ϊ256us��
	count1++;
	if(irtime<= timer1) //5==0�� 15==90��
	{ 
		PWM = 1; 
	}
	else 
	{ 
		PWM = 0; 
	}
		
}
void init0() interrupt 0
{
	if(startflag)
	{
		if(irtime>32)//��������룬������������ʱ�����һ�μ���ʱ�䣬����Ҫ���ٴ�
		{
			bitnum=0;	
		}
		irdata[bitnum]=irtime;
		irtime=0;
		bitnum++;
		if(bitnum==33)
		{
			bitnum=0;
			irreceok=1;//һ֡�������ݽ�����ɱ�־
		}
	}
	else
	{
		startflag=1;//����ʼ��־λ��1���ȵ��´ν����жϼ��ɽ���if���
		irtime=0;//����ʼ֮ǰ�ļ�����ʱ�����㡣�ȵ��´ν����жϵ�ʱ�����������������ʱ��
	}
}