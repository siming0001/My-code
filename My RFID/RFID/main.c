#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include <intrins.h>
#include "delay.h"
#include "rc522.h"
#include "1602.h"
#include "beep.h"
#include<string.h>

unsigned char UID[5];	//卡号
unsigned char Temp[4] ; //暂存数组  
unsigned char code A[3]={0x00,0x00,0x00};//用来占位，实际无意义

unsigned long time_20ms;   //定时计数
char dis0[16];		//液晶显示数组
char dis1[16];
unsigned char disFlag=0;	//显示标志
unsigned char manNum=0;
void Init_Timer0(void);		 //函数声明
void UART_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void uartSendStr(unsigned char *s,unsigned char length);
void uartSendByte(unsigned char dat);
bit Flag=0;//签到签退模式选择
bit modeFlag=0;//卡片录入模式选择
bit beepFlag0=0;
bit beepFlag1=0;
bit selectFlag=0;//考情查询标志位

void main (void)
{     
	unsigned char disPlace=0;	//显示位置
	UART_Init();	  //串口初始化
	Init_Timer0();        //定时器0初始化
	LCD_Init();           //初始化液晶
	Delay1Ms(200);          //延时有助于稳定
	INT0_Init();	//外部中断0初始
	INT1_Init();	//外部中断1初始
	LCD_Clear();

	PcdReset();//复位RC522
	PcdAntennaOn();//开启天线发射 

	//欢迎界面
	sprintf(dis0," Hello,welcome");//打印数据
	LCD_Write_String(0,0,dis0);//显示第二行
	sprintf(dis1," to the system!");//打印数据
	LCD_Write_String(0,1,dis1);//显示第二行
	Delay1Ms(2000);
	while (1)         //主循环
	{
		//按下按键K4，触发外部中断1，进行卡片录入
		while(modeFlag)
		{
			if(beepFlag1)//触发外部中断1且modeFlag==1，蜂鸣器鸣叫
			{
				Beep_Time1(100);
				beepFlag1=0;
			}
			LCD_Clear();
			sprintf(dis0,"Card number entr");//打印数据
			sprintf(dis1,"y");//打印数据
			LCD_Write_String(0,0,dis0);//显示第一行
			LCD_Write_String(0,1,dis1);//显示第二行
			Delay1Ms(200);
			LCD_Clear();
			if(PcdRequest(0x52,Temp)==MI_OK)  //检测到卡号
			{
				if(PcdAnticoll(UID)==MI_OK)
					uartSendStr(UID,4);
			}
		}
	
		if(selectFlag)
		{
			//改变上位机考情查询状态位
			uartSendByte(0xff);
			uartSendStr(A,3);
			selectFlag=0;
		}
		
		if(PcdRequest(0x52,Temp)==MI_OK)  //检测到卡号
		{
				if(PcdAnticoll(UID)==MI_OK)
				{ 
					//串口发送卡号
					uartSendStr(UID,4);
					Beep_Time(100);
					Delay1Ms(100);
				}
		}
			//定时器中断定时显示
		if(disFlag  == 1)
		{
				LCD_Clear();
				
				if(Flag)
				{
					if(beepFlag0)//触发外部中断0，蜂鸣器鸣叫
					{
						Beep_Time1(100);
						beepFlag0=0;
					}
					sprintf(dis0,"Please sign out");//打印数据
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
				LCD_Write_String(0,0,dis0);//显示第一行
				disFlag=0;                   
		}
	}
}


void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
	TH0=(65536-20000)/256;		  //重新赋值 20ms
	TL0=(65536-20000)%256;
	EA=1;            //总中断打开
	ET0=1;           //定时器中断打开
	TR0=1;           //定时器开关打开
}

void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-20000)/256;		  //重新赋值 20ms
	TL0=(65536-20000)%256;	
	time_20ms++;
	if(time_20ms%10==0)	   //定时显示
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
    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz
		TL1 = TH1;  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    ES    = 1;                  //打开串口中断
}


void uartSendByte(unsigned char dat)
{
	unsigned char time_out;
	time_out=0x00;
	SBUF = dat;			  //将数据放入SBUF中
	while((!TI)&&(time_out<100))  //检测是否发送出去
	{time_out++;DelayUs2x(10);}	//未发送出去 进行短暂延时
	TI = 0;						//清除ti标志
}

void uartSendStr(unsigned char *s,unsigned char length)
{
	unsigned char NUM;
	NUM=0x00;
	if(Flag)
	{
		if(modeFlag)
			uartSendByte(0x02);//卡片录入模式位
		else
			uartSendByte(0x01);//签退模式位
	}
	else
	{
		if(modeFlag)
			uartSendByte(0x02);//卡片录入模式位
		else
			uartSendByte(0x00);//签到模式位
	}
	while(NUM<length)	//发送长度对比
	{
		uartSendByte(*s);  //发送单字节数据
		s++;		  //指针++
		NUM++;		  //下一个++
  }
}

void UART_SER (void) interrupt 4 	//串行中断服务程序
{
	if(RI)                        //判断是接收中断产生
	{
		RI=0;                      //标志位清零
	}
	
	if(TI)  //如果是发送标志位，清零
	TI=0;
} 