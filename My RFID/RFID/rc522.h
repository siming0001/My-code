#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include <intrins.h>


sbit RF_SS=P1^0;						// p1.0 射频卡从机选择(SS)---SDA
sbit RF_SCLK=P1^1;						// p1.1 射频卡数据时钟输出(SCLK)
sbit RF_DATA_OUT=P1^2;					// p1.2 射频卡数据输出(MOSI)
sbit RF_DATA_IN=P1^3;					// p1.3 射频模块输入(MISO) 
sbit RF_LPCTL=P1^4;						// P1.4 射频卡休眠控制 ------RST   

//sbit RF_SS=P3^7;						// p1.0 射频卡从机选择(SS)---SDA
//sbit RF_SCLK=P3^6;						// p1.1 射频卡数据时钟输出(SCLK)
//sbit RF_DATA_OUT=P3^5;					// p1.2 射频卡数据输出(MOSI)
//sbit RF_DATA_IN=P3^4;					// p1.3 射频模块输入(MISO) 
//sbit RF_LPCTL=P3^3;						// P1.4 射频卡休眠控制 ------RST 

//------------------------------- RC522操作宏 ------------------------------//
#define    RF_POWER_ON             _nop_()                                // 射频模块上电
#define    RF_POWER_OFF            _nop_()                                // 射频模块下电

#define    NSS522_1          (RF_SS=1)      	//P4OUT |= RF_SS                        // 从机选择无效       
#define    NSS522_0          (RF_SS=0)     		//P4OUT &=~RF_SS                        // 从机选择有效
#define    SCK522_1          (RF_SCLK=1)    	//P4OUT |= RF_SCLK                      // 数据时钟置高
#define    SCK522_0          (RF_SCLK=0)    	//P4OUT &=~RF_SCLK                      // 数据时钟置低
#define    SI522_1           (RF_DATA_OUT=1)    //P4OUT |= RF_DATA_OUT                  // 数据置高        
#define    SI522_0           (RF_DATA_OUT=0)    //P4OUT &=~RF_DATA_OUT                  // 数据置低        
#define    SO522             RF_DATA_IN 		//((P4IN & RF_DATA_IN)>>4)              // 数据输入
#define    RST522_1          (RF_LPCTL=1)   	//P4OUT |= RF_LPCTL
#define    RST522_0          (RF_LPCTL=0)  		//P4OUT &=~RF_LPCTL

#define MAXRLEN        18

//*******************RC522 FIFO长度定义***************************/
//#define DEF_FIFO_LENGTH             64           //FIFO size=64byte

//***********************RC522命令字*****************************/
#define PCD_IDLE                  0x00           //取消当前命令
#define PCD_AUTHENT               0x0E           //验证密钥
//#define PCD_RECEIVE               0x08           //接收数据
//#define PCD_TRANSMIT              0x04           //发送数据
#define PCD_TRANSCEIVE            0x0C           //发送并接收数据
#define PCD_RESETPHASE            0x0F           //复位
#define PCD_CALCCRC               0x03           //CRC计算

//*******************Mifare_One卡片命令字****************************/
#define PICC_ANTICOLL1            0x93           //防冲撞

//************************MF522寄存器定义**************************/
// PAGE 0    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02     
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06      
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
// PAGE 1     
#define     ModeReg               0x11
#define     TxControlReg          0x14
#define     TxASKReg              0x15
// PAGE 2    
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D

//*******************RC522通讯返回错误代码*********************/
#define MI_ERR                      0xFE 
#define MI_OK                          0 
#define MI_NOTAGERR                 0xFF 

//***********************函数定义***************************/
void Rc522IoInit(void);
char PcdReset(void)                                                          ;
void PcdAntennaOn(void)                                                      ;
//void PcdAntennaOff(void)                                                     ;
char PcdRequest(unsigned char req_code,unsigned char *pTagType)              ;   
char PcdAnticoll(unsigned char *pSnr)                                        ;                               
char PcdComMF522(unsigned char Command, unsigned char *pInData, 
                 unsigned char InLenByte,unsigned char *pOutData, 
                 unsigned int  *pOutLenBit                       )           ;
void WriteRawRC(unsigned char Address,unsigned char value)                   ;
unsigned char ReadRawRC(unsigned char Address)                               ; 
void SetBitMask(unsigned char reg,unsigned char mask)                        ; 
void ClearBitMask(unsigned char reg,unsigned char mask)                      ;

//void CalulateCRC(unsigned char *pIndata,unsigned char len,
//                 unsigned char *pOutData                  )                  ;
