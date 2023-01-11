#ifndef _HWDJ_H
void delay(u16 i);
void int0init();
void time0init();
void irpros(); //红外接收数据处理 ，区分是数据0还是1
void irwork();  //将四个字节的数据转换成10进制数显示
void display();
void motopros();
#endif

