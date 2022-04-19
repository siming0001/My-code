#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<stdio.h>
#include <intrins.h>
#include "delay.h"
#include "rc522.h"
#include "1602.h"
#include "beep.h"
#include<string.h>

unsigned char UID[5];	//����
unsigned char Temp[4] ; //�ݴ�����  
unsigned char code A[3]={0x00,0x00,0x00};//����ռλ��ʵ��������

unsigned long time_20ms;   //��ʱ����
char dis0[16];		//Һ����ʾ����
char dis1[16];
unsigned char disFlag=0;	//��ʾ��־
unsigned char manNum=0;
void Init_Timer0(void);		 //��������
void UART_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void uartSendStr(unsigned char *s,unsigned char length);
void uartSendByte(unsigned char dat);
bit Flag=0;//ǩ��ǩ��ģʽѡ��
bit modeFlag=0;//��Ƭ¼��ģʽѡ��
bit beepFlag0=0;
bit beepFlag1=0;
bit selectFlag=0;//�����ѯ��־λ

void main (void)
{     
	unsigned char disPlace=0;	//��ʾλ��
	UART_Init();	  //���ڳ�ʼ��
	Init_Timer0();        //��ʱ��0��ʼ��
	LCD_Init();           //��ʼ��Һ��
	Delay1Ms(200);          //��ʱ�������ȶ�
	INT0_Init();	//�ⲿ�ж�0��ʼ
	INT1_Init();	//�ⲿ�ж�1��ʼ
	LCD_Clear();

	PcdReset();//��λRC522
	PcdAntennaOn();//�������߷��� 

	//��ӭ����
	sprintf(dis0," Hello,welcome");//��ӡ����
	LCD_Write_String(0,0,dis0);//��ʾ�ڶ���
	sprintf(dis1," to the system!");//��ӡ����
	LCD_Write_String(0,1,dis1);//��ʾ�ڶ���
	Delay1Ms(2000);
	while (1)         //��ѭ��
	{
		//���°���K4�������ⲿ�ж�1�����п�Ƭ¼��
		while(modeFlag)
		{
			if(beepFlag1)//�����ⲿ�ж�1��modeFlag==1������������
			{
				Beep_Time1(100);
				beepFlag1=0;
			}
			LCD_Clear();
			sprintf(dis0,"Card number entr");//��ӡ����
			sprintf(dis1,"y");//��ӡ����
			LCD_Write_String(0,0,dis0);//��ʾ��һ��
			LCD_Write_String(0,1,dis1);//��ʾ�ڶ���
			Delay1Ms(200);
			LCD_Clear();
			if(PcdRequest(0x52,Temp)==MI_OK)  //��⵽����
			{
				if(PcdAnticoll(UID)==MI_OK)
					uartSendStr(UID,4);
			}
		}
	
		if(selectFlag)
		{
			//�ı���λ�������ѯ״̬λ
			uartSendByte(0xff);
			uartSendStr(A,3);
			selectFlag=0;
		}
		
		if(PcdRequest(0x52,Temp)==MI_OK)  //��⵽����
		{
				if(PcdAnticoll(UID)==MI_OK)
				{ 
					//���ڷ��Ϳ���
					uartSendStr(UID,4);
					Beep_Time(100);
					Delay1Ms(100);
				}
		}
			//��ʱ���ж϶�ʱ��ʾ
		if(disFlag  == 1)
		{
				LCD_Clear();
				
				if(Flag)
				{
					if(beepFlag0)//�����ⲿ�ж�0������������
					{
						Beep_Time1(100);
						beepFlag0=0;
					}
					sprintf(dis0,"Please sign out");//��ӡ����
				}
				else
				{
					if(beepFlag0)
					{
						Beep_Time1(100);
						beepFlag0=0;
					}
					sprintf(dis0,"Please sign in");
				}
				LCD_Write_String(0,0,dis0);//��ʾ��һ��
				disFlag=0;                   
		}
	}
}


void Init_Timer0(void)
{
	TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
	TH0=(65536-20000)/256;		  //���¸�ֵ 20ms
	TL0=(65536-20000)%256;
	EA=1;            //���жϴ�
	ET0=1;           //��ʱ���жϴ�
	TR0=1;           //��ʱ�����ش�
}

void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-20000)/256;		  //���¸�ֵ 20ms
	TL0=(65536-20000)%256;	
	time_20ms++;
	if(time_20ms%10==0)	   //��ʱ��ʾ
	{disFlag = 1 ;}
}

void INT0_Init(void)
{
	IE0=0;
	EA=1;
	IT0=1;
	EX0=1;
	IP=0x01;
}

void INT1_Init(void)
{
	IE1=0;
	EA=1;
	IT1=1;
	EX1=1;
}

void INT0Handler(void) interrupt 0
{
	Flag=!Flag;                                                                                                            
	beepFlag0=1;
	selectFlag=1;
}

void INT1Handler(void) interrupt 2
{
	modeFlag=!modeFlag;
	beepFlag1=1;
}

void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz
		TL1 = TH1;  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    EA    = 1;                  //�����ж�
    ES    = 1;                  //�򿪴����ж�
}


void uartSendByte(unsigned char dat)
{
	unsigned char time_out;
	time_out=0x00;
	SBUF = dat;			  //�����ݷ���SBUF��
	while((!TI)&&(time_out<100))  //����Ƿ��ͳ�ȥ
	{time_out++;DelayUs2x(10);}	//δ���ͳ�ȥ ���ж�����ʱ
	TI = 0;						//���ti��־
}

void uartSendStr(unsigned char *s,unsigned char length)
{
	unsigned char NUM;
	NUM=0x00;
	if(Flag)
	{
		if(modeFlag)
			uartSendByte(0x02);//��Ƭ¼��ģʽλ
		else
			uartSendByte(0x01);//ǩ��ģʽλ
	}
	else
	{
		if(modeFlag)
			uartSendByte(0x02);//��Ƭ¼��ģʽλ
		else
			uartSendByte(0x00);//ǩ��ģʽλ
	}
	while(NUM<length)	//���ͳ��ȶԱ�
	{
		uartSendByte(*s);  //���͵��ֽ�����
		s++;		  //ָ��++
		NUM++;		  //��һ��++
  }
}

void UART_SER (void) interrupt 4 	//�����жϷ������
{
	if(RI)                        //�ж��ǽ����жϲ���
	{
		RI=0;                      //��־λ����
	}
	
	if(TI)  //����Ƿ��ͱ�־λ������
	TI=0;
} 