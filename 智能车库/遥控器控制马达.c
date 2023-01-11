/*
实验说明：
	根据自己使用的LCD1602是否带有转接板，如果带有转接板的即为4位，需在LCD.H头文件中
	将宏#define LCD1602_4PINS打开，我们这里使用的LCD1602是8位，所以默认将该宏注释。
实验接线：
	1，LCD1602液晶模块-->单片机管脚
	参考LCD1602液晶显示实验接线（开发攻略内在对应的实验章节内实验现象有接线说明）
	2，红外接收模块-->单片机管脚 
	参考红外遥控实验接线（开发攻略内在对应的实验章节内实验现象有接线说明）
	J11-->P32
	3，直流电机模块-->单片机管脚
	IN1-->P10（参考直流电机实验接线）
	
实验现象：
	LCD1602显示遥控器键值，并且显示电机状态（ON/OFF），通过按第一个按键可以控制马达的启动和停止
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

u16 count1=0,count2=0;	 //因为定时器满8秒归零，所以要一个大点的定时器count2
u8 dat[]="0123456789";
//u8 d1[8]=" RED IR ";

u8 num[16]="0123456789ABCDEF";
u8 timer1;
//u8 d2[7]="STATE:";	 因为软件有变量内存空间限制，data只能<=125(应该是)，超过了无法生成.hex文件
//					，只有删掉无用变量才能释放空间，或者将变量内存空间的默认small模式调成large模式（空间变大，但代码变多，速度变慢，一般不推荐）															
u8 irtime;	  //接收时间
u8 startflag;  //开始检测
u8 bitnum;	  //bit位数
u8 irdata[33]; //寄存没有个bit的时间
u8 irreceok;	  //接收完成标志位
u8 ircode[4];  //将接收的四个字节数据保存
u8 irprosok;	  //接收数据处理标志位
u8 irdisp[8];  //将四个字节的16进制通过转换变成十进制存储
u8 dat1,dat2;
u8 keynum;
//u8 motoflag;
void delay(u16 i)
{
	while(i--);
}

/*延时程序*/
void delay1s(void)   //误差 0us
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
	TMOD=0X02;	//设置定时器0模式2.该模式为自动装载模式
	TH0=0X00;
	TL0=0X00;//设定计数初值，每当TL0计数到255时，TH0将把自己的数据给TL0，又重新计数
	TR0=1;
	ET0=1;
	EA=1;	
}
void irpros() //红外接收数据处理 ，区分是数据0还是1
{
	u8 i,j,value;
	u8 k=1;	//引导码去掉，所以令k=1；
	for(j=0;j<4;j++)  //取出了一帧数据中的四个字节数据
	{
		for(i=0;i<8;i++)	//取出了一个字节的数据		
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
void irwork()  //将四个字节的数据转换成10进制数显示
{
	irdisp[0]=ircode[0]/16;//显示高位
	irdisp[1]=ircode[0]%16;//显示低位  
	irdisp[2]=ircode[1]/16;
	irdisp[3]=ircode[1]%16;	 //同一个遥控器此2个字节的引导码数据是不会改变的，改变的只是数据码及反码
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
	if(motoflag==1)			//马达状态显示
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
	if((dat1==4)&&(dat2==5)&&(keynum==1))  //按下第一次按键开马达
	{			
		dat1=0;
		dat2=0;
//		motoflag=1;
		timer1 =2;//舵机恢复到0°的位置
		irtime=0;//让定时器重新计数		   
//		delay1s();						   //这里去掉延迟后舵机更流畅快速
	}
			
	if((dat1==4)&&(dat2==5)&&(keynum>=2))	//按下第二次此按键时关闭马达
	{
		dat1=0;
		dat2=0;
		keynum=0;
//		motoflag=0;
		timer1 =6;//舵机旋转90°
		irtime=0;
//		delay1s();						  //这里去掉延迟后舵机更流畅快速
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
		lcd1602_show_string(0,1,"        ");//消除从其他模式跳到模式1的影响
		while(1)
		{
			keyscan(&mmdat,&ms);
			if(ms!=1) break;
			if((HWL!=0)||(HWR!=0))
			{
				
				timer1 =2;//舵机恢复到0°的位置
				irtime=0;//让定时器重新计数
				delay1s();
				while((HWL!=0)||(HWR!=0));
				if((HWL==0)||(HWR==0))
				{
				timer1 =6;//舵机旋转90°
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
		lcd1602_show_string(10,0,"      ");//消除从其他模式跳到模式2的影响 
		mmdat=-1;
		keyscan(&mmdat,&ms);
		if(mm==mmdat) 
		{
			timer1 =2;//舵机恢复到0°的位置
			irtime=0;//让定时器重新计数
			delay1s();
			count1=0;
			while(1)   
			{
				mmdat=-1;
				if (count1 >=14000) 							   		//因为使用的是定时器的模式1，每次溢出都需要软件重装初值，所以会产生较大误差，定时器中断每3秒会有2秒的误差且一般会大于预定时间，所以这里把原来的50000（5秒）缩短至30000（3秒）
				{ 
					count2=0;											//让count1、2都归零后再进入循环重新计时
					count1=0;
					while(count2<4)							 
					{
						if(count1>=20000)								//若把这个计数归零的程序放在T0初始化里会导致中断计数缓慢甚至停止
						{
						count1=0;
						count2++;
						}
						if(mm==mmdat) break; 
						sw=(20-(count2*5+count1/4000))/10;		//14000平均分成5份		
						swi=dat[sw];
						gw=(20-(count2*5+count1/4000))%10;
						gwi=dat[gw];
						lcd1602_show_string(10,0,"djs"); 
						lcd1602_show_string(13,0,&swi);
						lcd1602_show_string(14,0,&gwi);
						lcd1602_show_string(15,0,"s");
						keyscan(&mmdat,&ms);					 //这里mmdat不还原是因为要把门关上
					}

					while(count2>=4)						 
					{
					count2=5;								 //定时器溢出后会自动归零，所以要让程序停在此循环就必须让count2等于一个较大的数
					keyscan(&mmdat,&ms);						 //这里mmdat不还原是因为要把门关上
					if(mm==mmdat) break;
					beep=!beep;
					delayms(1);
					}											 
					beep=0;
				}
				keyscan(&mmdat,&ms);
				if(mm==mmdat) break;
 
				if(ms==0) 									//在模式2中只有开着门才能改密码
				{
					lcd1602_show_string(13,1,"ms2");
					lcd1602_show_string(0,1,"qsrxmm");
					lcd1602_show_string(10,0,"      ");//消除从其他模式跳到模式0（改密码模式）的影响
					mmdat=mm;
 				//	mmdat=-1;		   这里本来是要复原mmdat的，但要改密码，就先让它和mm一样，然后进入循环获取新密码后退出
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
					lcd1602_show_string(0,1,"ygxmm ");		//顺便提示已退出循环
					lcd1602_show_string(10,0,"      ");//消除从其他模式跳到模式0（改密码模式）的影响
					delayms(1000);
					mmdat=-1;
					ms=2;									//改完密码就退出改密码模式，回到模式2
				}
				lcd1602_show_string(0,1,"qsrmm ");
				lcd1602_show_string(10,0,"      ");//消除从其他模式跳到模式2的影响
			}
			timer1 =6;//舵机旋转90°
			irtime=0;
			}
		}
	}
	if(ms==3) 
	{	
		lcd1602_show_string(13,1,"ms3");
		lcd1602_show_string(10,0,"       ");
		lcd1602_show_string(0,1,"        ");//消除从其他模式跳到模式3的影响	  

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
	irtime++;//每进来一次就说明定时时间为256us；
	count1++;
	if(irtime<= timer1) //5==0° 15==90°
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
		if(irtime>32)//检测引导码，求法是用引导码时间除以一次计数时间，看看要多少次
		{
			bitnum=0;	
		}
		irdata[bitnum]=irtime;
		irtime=0;
		bitnum++;
		if(bitnum==33)
		{
			bitnum=0;
			irreceok=1;//一帧红外数据接收完成标志
		}
	}
	else
	{
		startflag=1;//将开始标志位置1，等到下次进入中断即可进入if语句
		irtime=0;//将开始之前的计数器时间清零。等到下次进入中断的时候才是引导码真正的时间
	}
}