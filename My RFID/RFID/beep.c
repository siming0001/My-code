#include <REGX52.H>
#include "delay.h"
#include "intrins.h"
#include "beep.h"

sbit Beep=P2^5;
unsigned int i;

void Beep_Delay500us(void)		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 227;
	while (--i);
}

void Beep_Time(unsigned int ms)	//1000Hz
{
	for(i=0;i<ms*2;i++)
	{
		Beep=!Beep;
		Beep_Delay500us();
	}
}

void Beep_Time1(unsigned int ms)	//500Hz
{
	for(i=0;i<ms*2;i++)
	{
		Beep=!Beep;
		Delay1Ms(1);
	}
}