#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<stdio.h>
#include <intrins.h>


sbit RF_SS=P1^0;						// p1.0 ��Ƶ���ӻ�ѡ��(SS)---SDA
sbit RF_SCLK=P1^1;						// p1.1 ��Ƶ������ʱ�����(SCLK)
sbit RF_DATA_OUT=P1^2;					// p1.2 ��Ƶ���������(MOSI)
sbit RF_DATA_IN=P1^3;					// p1.3 ��Ƶģ������(MISO) 
sbit RF_LPCTL=P1^4;						// P1.4 ��Ƶ�����߿��� ------RST   

//sbit RF_SS=P3^7;						// p1.0 ��Ƶ���ӻ�ѡ��(SS)---SDA
//sbit RF_SCLK=P3^6;						// p1.1 ��Ƶ������ʱ�����(SCLK)
//sbit RF_DATA_OUT=P3^5;					// p1.2 ��Ƶ���������(MOSI)
//sbit RF_DATA_IN=P3^4;					// p1.3 ��Ƶģ������(MISO) 
//sbit RF_LPCTL=P3^3;						// P1.4 ��Ƶ�����߿��� ------RST 

//------------------------------- RC522������ ------------------------------//
#define    RF_POWER_ON             _nop_()                                // ��Ƶģ���ϵ�
#define    RF_POWER_OFF            _nop_()                                // ��Ƶģ���µ�

#define    NSS522_1          (RF_SS=1)      	//P4OUT |= RF_SS                        // �ӻ�ѡ����Ч       
#define    NSS522_0          (RF_SS=0)     		//P4OUT &=~RF_SS                        // �ӻ�ѡ����Ч
#define    SCK522_1          (RF_SCLK=1)    	//P4OUT |= RF_SCLK                      // ����ʱ���ø�
#define    SCK522_0          (RF_SCLK=0)    	//P4OUT &=~RF_SCLK                      // ����ʱ���õ�
#define    SI522_1           (RF_DATA_OUT=1)    //P4OUT |= RF_DATA_OUT                  // �����ø�        
#define    SI522_0           (RF_DATA_OUT=0)    //P4OUT &=~RF_DATA_OUT                  // �����õ�        
#define    SO522             RF_DATA_IN 		//((P4IN & RF_DATA_IN)>>4)              // ��������
#define    RST522_1          (RF_LPCTL=1)   	//P4OUT |= RF_LPCTL
#define    RST522_0          (RF_LPCTL=0)  		//P4OUT &=~RF_LPCTL

#define MAXRLEN        18

//*******************RC522 FIFO���ȶ���***************************/
//#define DEF_FIFO_LENGTH             64           //FIFO size=64byte

//***********************RC522������*****************************/
#define PCD_IDLE                  0x00           //ȡ����ǰ����
#define PCD_AUTHENT               0x0E           //��֤��Կ
//#define PCD_RECEIVE               0x08           //��������
//#define PCD_TRANSMIT              0x04           //��������
#define PCD_TRANSCEIVE            0x0C           //���Ͳ���������
#define PCD_RESETPHASE            0x0F           //��λ
#define PCD_CALCCRC               0x03           //CRC����

//*******************Mifare_One��Ƭ������****************************/
#define PICC_ANTICOLL1            0x93           //����ײ

//************************MF522�Ĵ�������**************************/
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

//*******************RC522ͨѶ���ش������*********************/
#define MI_ERR                      0xFE 
#define MI_OK                          0 
#define MI_NOTAGERR                 0xFF 

//***********************��������***************************/
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
