#include "1602.h"
#include "delay.h"
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

#define _NOP() _nop_()
sbit RS = P2^0;   //����˿� 
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
	LcdReset();               //LCD1602��ʼ��
	DelayMs(10);	
	sprintf(temp,"1111111111111111");//������ʾ
	DispStr(0,0,(unsigned char *)temp);//��ӡ��ʾ
	sprintf(temp,"1111111111111111");//������ʾ
	DispStr(0,1,(unsigned char *)temp);//��ӡ��ʾ
*/
/***********************************************
�������ƣ�DispStr
��    �ܣ���Һ����ĳ��λ����������ʾһ���ַ���
��    ����x--λ�õ�������
          y--λ�õ�������
          ptr--ָ���ַ������λ�õ�ָ��
����ֵ  ����
***********************************************/
void DispStr(uchar x,uchar y,uchar *ptr) 
{
    uchar *temp;
    uchar i,n = 0;
    
    temp = ptr;
    while(*ptr++ != '\0')   n++;    //�����ַ�����Ч�ַ��ĸ���
    
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
�������ƣ�DispNchar
��    �ܣ���Һ����ĳ��λ����������ʾN���ַ�
��    ����x--λ�õ�������
          y--λ�õ�������
          n--�ַ�����
          ptr--ָ���ַ����λ�õ�ָ��
����ֵ  ����
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
�������ƣ�LocateXY
��    �ܣ���Һ��������ʾ�ַ�λ�õ�������Ϣ
��    ����x--λ�õ�������
          y--λ�õ�������
����ֵ  ����
********************************************/
void LocateXY(uchar x,uchar y) 
{
    uchar temp;

    temp = x&0x0f;
    y &= 0x01;
    if(y)   temp |= 0x40;  //����ڵ�2��
    temp |= 0x80;

    LcdWriteCommand(temp,1);
}

/*******************************************
�������ƣ�Disp1Char
��    �ܣ���ĳ��λ����ʾһ���ַ�
��    ����x--λ�õ�������
          y--λ�õ�������
          data--��ʾ���ַ�����
����ֵ  ����
********************************************/
void Disp1Char(uchar x,uchar y,uchar data1) 
{
    LocateXY( x, y );			
    LcdWriteData( data1 );		
}

/*******************************************
�������ƣ�LcdReset
��    �ܣ���1602Һ��ģ����и�λ����
��    ������
����ֵ  ����
********************************************/
void LcdReset(void) 
{
//    DataDir  = 0xFF;                 //���ݶ˿���Ϊ���״̬ 
    LcdWriteCommand(0x38, 0);	    //�涨�ĸ�λ����
    DelayMs(5);
    LcdWriteCommand(0x38, 0);		
    DelayMs(5);
    LcdWriteCommand(0x38, 0);
    DelayMs(5);

    LcdWriteCommand(0x38, 1);		//��ʾģʽ����
    LcdWriteCommand(0x08, 1);		//��ʾ�ر�
    LcdWriteCommand(0x01, 1);	    //��ʾ����
    LcdWriteCommand(0x06, 1);		//д�ַ�ʱ���岻�ƶ�
    LcdWriteCommand(0x0c, 1);		//��ʾ���������α꣬����˸
}

/*------------------------------------------------
                ��������
------------------------------------------------*/
void LcdClear(void) 
{ 
	LcdWriteCommand(0x01,1); 
	DelayMs(5);
}

/*******************************************
�������ƣ�LcdWriteCommand
��    �ܣ���Һ��ģ��д������
��    ����cmd--���
          chk--�Ƿ���æ�ı�־��1����æ��0������
����ֵ  ����
********************************************/
void LcdWriteCommand(uchar cmd,uchar chk) 
{

    if (chk) WaitForEnable();   // ���æ�ź�?
    
    CLR_RS;	
    CLR_RW; 
    _NOP();

    DataPort = cmd;             //��������д�����ݶ˿� 
    _NOP();					
    
    SET_EN;                     //����ʹ�������ź�
    _NOP();
    _NOP();
    CLR_EN;			
}

/*******************************************
�������ƣ�LcdWriteData
��    �ܣ���Һ����ʾ�ĵ�ǰ��ַд����ʾ����
��    ����data--��ʾ�ַ�����
����ֵ  ����
********************************************/
void LcdWriteData( uchar data1 ) 
{
    WaitForEnable();        //�ȴ�Һ����æ
    SET_RS;
    CLR_RW; 

    SET_EN;
	
	_NOP();
    DataPort = data1;        //����ʾ����д�����ݶ˿�
    _NOP();
                //����ʹ�������ź�
    _NOP(); 
    _NOP(); 
    CLR_EN;		
}

/*******************************************
�������ƣ�WaitForEnable
��    �ܣ��ȴ�1602Һ������ڲ�����
��    ������
����ֵ  ����
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
    while(((DataPIN&0x80)!=0)&&(later<1000))  //���æ��־
    {
      DelayUs2x(2);
      later++;        
    }
    CLR_EN;
//    DataDir|=0xFF;  //��P4���л�Ϊ���״̬
}		

