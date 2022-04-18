#include "1602.h"
#include "delay.h"
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

#define _NOP() _nop_()
sbit RS = P2^0;   //定义端口 
sbit RW = P2^1;
sbit EN = P2^2;

#define DataPort    P0				
#define DataPIN     P0

#define CLR_RS (RS=0) 
#define SET_RS (RS=1)	
#define CLR_RW (RW=0)	
#define SET_RW (RW=1)
#define CLR_EN (EN=0)
#define SET_EN (EN=1)	

/*
	LcdReset();               //LCD1602初始化
	DelayMs(10);	
	sprintf(temp,"1111111111111111");//更新显示
	DispStr(0,0,(unsigned char *)temp);//打印显示
	sprintf(temp,"1111111111111111");//更新显示
	DispStr(0,1,(unsigned char *)temp);//打印显示
*/
/***********************************************
函数名称：DispStr
功    能：让液晶从某个位置起连续显示一个字符串
参    数：x--位置的列坐标
          y--位置的行坐标
          ptr--指向字符串存放位置的指针
返回值  ：无
***********************************************/
void DispStr(uchar x,uchar y,uchar *ptr) 
{
    uchar *temp;
    uchar i,n = 0;
    
    temp = ptr;
    while(*ptr++ != '\0')   n++;    //计算字符串有效字符的个数
    
    for (i=0;i<n;i++)
    {
        Disp1Char(x++,y,temp[i]);
        if (x == 0x10)
        {
            break;
        }
    }
}

/*******************************************
函数名称：DispNchar
功    能：让液晶从某个位置起连续显示N个字符
参    数：x--位置的列坐标
          y--位置的行坐标
          n--字符个数
          ptr--指向字符存放位置的指针
返回值  ：无
*******************************************
void DispNChar(uchar x,uchar y, uchar n,uchar *ptr) 
{
    uchar i;
    
    for (i=0;i<n;i++)
    {
        Disp1Char(x++,y,ptr[i]);
        if (x == 0x10)
        {
           x = 0; 
            y ^= 1;
        }
    }
}
*/
/*******************************************
函数名称：LocateXY
功    能：向液晶输入显示字符位置的坐标信息
参    数：x--位置的列坐标
          y--位置的行坐标
返回值  ：无
********************************************/
void LocateXY(uchar x,uchar y) 
{
    uchar temp;

    temp = x&0x0f;
    y &= 0x01;
    if(y)   temp |= 0x40;  //如果在第2行
    temp |= 0x80;

    LcdWriteCommand(temp,1);
}

/*******************************************
函数名称：Disp1Char
功    能：在某个位置显示一个字符
参    数：x--位置的列坐标
          y--位置的行坐标
          data--显示的字符数据
返回值  ：无
********************************************/
void Disp1Char(uchar x,uchar y,uchar data1) 
{
    LocateXY( x, y );			
    LcdWriteData( data1 );		
}

/*******************************************
函数名称：LcdReset
功    能：对1602液晶模块进行复位操作
参    数：无
返回值  ：无
********************************************/
void LcdReset(void) 
{
//    DataDir  = 0xFF;                 //数据端口设为输出状态 
    LcdWriteCommand(0x38, 0);	    //规定的复位操作
    DelayMs(5);
    LcdWriteCommand(0x38, 0);		
    DelayMs(5);
    LcdWriteCommand(0x38, 0);
    DelayMs(5);

    LcdWriteCommand(0x38, 1);		//显示模式设置
    LcdWriteCommand(0x08, 1);		//显示关闭
    LcdWriteCommand(0x01, 1);	    //显示清屏
    LcdWriteCommand(0x06, 1);		//写字符时整体不移动
    LcdWriteCommand(0x0c, 1);		//显示开，不开游标，不闪烁
}

/*------------------------------------------------
                清屏函数
------------------------------------------------*/
void LcdClear(void) 
{ 
	LcdWriteCommand(0x01,1); 
	DelayMs(5);
}

/*******************************************
函数名称：LcdWriteCommand
功    能：向液晶模块写入命令
参    数：cmd--命令，
          chk--是否判忙的标志，1：判忙，0：不判
返回值  ：无
********************************************/
void LcdWriteCommand(uchar cmd,uchar chk) 
{

    if (chk) WaitForEnable();   // 检测忙信号?
    
    CLR_RS;	
    CLR_RW; 
    _NOP();

    DataPort = cmd;             //将命令字写入数据端口 
    _NOP();					
    
    SET_EN;                     //产生使能脉冲信号
    _NOP();
    _NOP();
    CLR_EN;			
}

/*******************************************
函数名称：LcdWriteData
功    能：向液晶显示的当前地址写入显示数据
参    数：data--显示字符数据
返回值  ：无
********************************************/
void LcdWriteData( uchar data1 ) 
{
    WaitForEnable();        //等待液晶不忙
    SET_RS;
    CLR_RW; 

    SET_EN;
	
	_NOP();
    DataPort = data1;        //将显示数据写入数据端口
    _NOP();
                //产生使能脉冲信号
    _NOP(); 
    _NOP(); 
    CLR_EN;		
}

/*******************************************
函数名称：WaitForEnable
功    能：等待1602液晶完成内部操作
参    数：无
返回值  ：无
********************************************/
void WaitForEnable(void) 
{
  	unsigned int later=0;
	DataPort=0xff;
    CLR_RS;
    SET_RW;
    _NOP();
    SET_EN; 
    _NOP();
    _NOP();
//    while((DataPIN&Busy)!=0);    
    while(((DataPIN&0x80)!=0)&&(later<1000))  //检测忙标志
    {
      DelayUs2x(2);
      later++;        
    }
    CLR_EN;
//    DataDir|=0xFF;  //将P4口切换为输出状态
}		

