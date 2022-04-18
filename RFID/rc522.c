#include "rc522.h"
#include "delay.h"

//******************************************************************/
//��    �ܣ���RC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//��    �أ�������ֵ
//******************************************************************/
unsigned char ReadRawRC(unsigned char Address)
{
  unsigned char i, ucAddr             ;
  unsigned char ucResult=0            ;
  NSS522_0                            ;
  SCK522_0                            ;
  ucAddr = ((Address<<1)&0x7E)|0x80   ;
  for(i=8;i>0;i--)
  {
    if((ucAddr&0x80)==0x80)
      SI522_1                         ;
    else
      SI522_0                         ;
    SCK522_1                          ;
    ucAddr <<= 1                      ;
    SCK522_0                          ;
  }
  for(i=8;i>0;i--)
  {
    SCK522_1                          ;
    ucResult <<=1                     ;
    ucResult |= SO522                 ;
    SCK522_0                          ;
  }
  SCK522_0                            ;
  NSS522_1                            ;
  return ucResult                     ;
}

//******************************************************************/
//��    �ܣ�дRC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//          value Ҫд���ֵ
//******************************************************************/

void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char  ucAddr        ;

    NSS522_0                     ;
    SCK522_0                     ;
    ucAddr = ((Address<<1)&0x7E) ;
    {
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;
      
      if(ucAddr&0x80)
        SI522_1                  ;
      else
        SI522_0                  ; 
      SCK522_1                   ;
      ucAddr <<= 1               ;
      SCK522_0                   ;                  
    }
    {
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;
      
      if(value&0x80)
        SI522_1                  ;
      else
        SI522_0                  ;
      SCK522_1                   ;
      value <<= 1                ;
      SCK522_0                   ;      
    }
    SCK522_0;
    NSS522_1;
}

//******************************************************************/
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
//******************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x0            ;
  tmp = ReadRawRC(reg)| mask;
  WriteRawRC(reg,tmp | mask);  // set bit mask
}

//******************************************************************/
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
//******************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x0              ;
  tmp = ReadRawRC(reg)&(~mask);
  WriteRawRC(reg, tmp)        ;  // clear bit mask
} 

//******************************************************************/
//��    �ܣ���λRC522
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdReset()
{
  RF_POWER_ON                          ;
  RST522_1                             ;
  DelayUs2x(60)                             ;
  RST522_0                             ;
  DelayUs2x(60)                             ;
  RST522_1                             ;
  DelayUs2x(60)                             ;
  WriteRawRC(CommandReg,PCD_RESETPHASE);
  DelayUs2x(60)                             ;
  WriteRawRC(ModeReg,0x3D)             ;
  WriteRawRC(TReloadRegL,30)           ;
  WriteRawRC(TReloadRegH,0)            ;
  WriteRawRC(TModeReg,0x8D)            ;
  WriteRawRC(TPrescalerReg,0x3E)       ;   
//  WriteRawRC(TxASKReg,0x40)            ; // FOR DEBUG AND TEST
  return MI_OK                         ; 
}

//******************************************************************/
//�������߷���  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
//******************************************************************/
void PcdAntennaOn()
{
  unsigned char i;
  WriteRawRC(TxASKReg,0x40)       ;

	DelayMs(1);
  
  i = ReadRawRC(TxControlReg)     ;
  if(!(i&0x03))
    SetBitMask(TxControlReg, 0x03);
  i=ReadRawRC(TxASKReg)       ;
}

//******************************************************************/
//�ر����߷���
//******************************************************************/
//void PcdAntennaOff()
//{
//  ClearBitMask(TxControlReg, 0x03);
//}

//******************************************************************/
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pInData[IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOutData[OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
//******************************************************************/
char PcdComMF522(unsigned char Command  ,unsigned char *pInData , 
                 unsigned char InLenByte,unsigned char *pOutData, 
                 unsigned int  *pOutLenBit                       )
{
  char status = MI_ERR                          ;
  unsigned char irqEn   = 0x00                  ;
  unsigned char waitFor = 0x00                  ;
  unsigned char lastBits                        ;
  unsigned char n                               ;
  unsigned int  i                               ;
  switch (Command)
  {
    case PCD_AUTHENT:
      irqEn   = 0x12                            ;
      waitFor = 0x10                            ;
      break                                     ;
    case PCD_TRANSCEIVE:
      irqEn   = 0x77                            ;
      waitFor = 0x30                            ;
      break                                     ;
    default:
      break                                     ;
  }
  WriteRawRC(ComIEnReg,irqEn|0x80)              ; //
  ClearBitMask(ComIrqReg,0x80)                  ;
  WriteRawRC(CommandReg,PCD_IDLE)               ;
  SetBitMask(FIFOLevelReg,0x80)                 ; // ���FIFO 
  for(i=0; i<InLenByte; i++)
    WriteRawRC(FIFODataReg,pInData[i])          ; // ����д��FIFO 
  WriteRawRC(CommandReg, Command)               ; // ����д������Ĵ���
  if(Command == PCD_TRANSCEIVE)
    SetBitMask(BitFramingReg,0x80)              ; // ��ʼ����     
  i = 6000                                      ; //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
  do 
  {
    n = ReadRawRC(ComIrqReg)                    ;
    i--                                         ;
  }
  while((i!=0)&&!(n&0x01)&&!(n&waitFor))        ;
  ClearBitMask(BitFramingReg,0x80)              ;
  if(i!=0)
  {
    if(!(ReadRawRC(ErrorReg)&0x1B))
    {
      status = MI_OK                            ;
      if (n&irqEn&0x01)
        status = MI_NOTAGERR                    ;
      if(Command==PCD_TRANSCEIVE)
      {
        n = ReadRawRC(FIFOLevelReg)             ;
        lastBits = ReadRawRC(ControlReg)&0x07   ;
        if(lastBits)
          *pOutLenBit = (n-1)*8 + lastBits      ;
        else
          *pOutLenBit = n*8                     ;
        if(n==0)
          n = 1                                 ;
        if(n>MAXRLEN)
          n = MAXRLEN                           ;
        for (i=0; i<n; i++)
          pOutData[i] = ReadRawRC(FIFODataReg)  ; 
      }
    }
    else
      status = MI_ERR                           ;        
  }
  SetBitMask(ControlReg,0x80)                   ;// stop timer now
  WriteRawRC(CommandReg,PCD_IDLE)               ; 
  return status;
}

//******************************************************************/
//��    �ܣ�Ѱ��                                                    /
//����˵��: req_code[IN]:Ѱ����ʽ                                   /
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�           /
//                0x26 = Ѱδ��������״̬�Ŀ�                       /
//          pTagType[OUT]����Ƭ���ʹ���                             /
//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire                           /
//��    ��: �ɹ�����MI_OK                                           /
//******************************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
  char status                                        ;  
  unsigned int  unLen                                ;
  unsigned char ucComMF522Buf[MAXRLEN]               ; 

  ClearBitMask(Status2Reg,0x08)                      ;
  WriteRawRC(BitFramingReg,0x07)                     ;
  SetBitMask(TxControlReg,0x03)                      ;
 
  ucComMF522Buf[0] = req_code                        ;

  status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,
                       1,ucComMF522Buf,&unLen       );
  if ((status == MI_OK) && (unLen == 0x10))
  {    
    *pTagType     = ucComMF522Buf[0]                 ;
    *(pTagType+1) = ucComMF522Buf[1]                 ;
  }
  else
    status = MI_ERR                                  ;
  return status                                      ;
}

//******************************************************************/
//��    �ܣ�����ײ                                                  /
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�                             /
//��    ��: �ɹ�����MI_OK                                           /
//******************************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

//******************************************************************/
//��MF522����CRC16����
//******************************************************************/
//void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
//{
//    unsigned char i,n;
//    ClearBitMask(DivIrqReg,0x04);
//    WriteRawRC(CommandReg,PCD_IDLE);
//    SetBitMask(FIFOLevelReg,0x80);
//    for (i=0; i<len; i++)
//    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
//    WriteRawRC(CommandReg, PCD_CALCCRC);
//    i = 0xFF;
//    do 
//    {
//        n = ReadRawRC(DivIrqReg);
//        i--;
//    }
//    while ((i!=0) && !(n&0x04));
//    pOutData[0] = ReadRawRC(CRCResultRegL);
//    pOutData[1] = ReadRawRC(CRCResultRegM);
//}

