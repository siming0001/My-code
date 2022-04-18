/*-----------------------------------------------
  ���ƣ�LCD1602.h
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�
  ���Ŷ������£�1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>

#ifndef __1602_H__
#define __1602_H__

void DispStr(unsigned char x,unsigned char y,unsigned char *ptr);
void DispNChar(unsigned char x,unsigned char y, unsigned char n,unsigned char *ptr);
void LocateXY(unsigned char x,unsigned char y);
void Disp1Char(unsigned char x,unsigned char y,unsigned char data1);
void LcdReset(void);
void LcdWriteCommand(unsigned char cmd,unsigned char chk);
void LcdWriteData( unsigned char data1 );
void WaitForEnable(void);
void LcdClear(void);

#define LCD_Init          LcdReset
#define LCD_Write_Char    Disp1Char
#define LCD_Write_String  DispStr
#define LCD_Clear         LcdClear
   
#endif
