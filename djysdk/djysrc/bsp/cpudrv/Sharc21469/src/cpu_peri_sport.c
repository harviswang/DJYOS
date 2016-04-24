//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   �캣��.
//����ģ��: Sport����ģ��
//����:  �캣��.
//�汾��V1.0.0
//�ļ�����: SPORT�������ļ���SPORTģ��SPI
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-04-24
//   ����:  �캣��.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "string.h"
#include "int.h"
#include "cpu_peri.h"

#include "def21469.h"



bool_t SPORT1_Send_Flag=false;
bool_t SPORT0_Recv_Flag=false;
bool_t SPORT1_Recv_Flag=false;
bool_t SPORT0_Send_Flag=false;

volatile tag_SportReg * const pg_sport_group1_reg =  (volatile tag_SportReg *)cn_sport_group1_baddr;
volatile tag_SportReg * const pg_sport_group2_reg =  (volatile tag_SportReg *)cn_sport_group2_baddr;
volatile tag_SportReg * const pg_sport_group3_reg =  (volatile tag_SportReg *)cn_sport_group3_baddr;
volatile tag_SportReg * const pg_sport_group4_reg =  (volatile tag_SportReg *)cn_sport_group4_baddr;

volatile tag_DMA_SportReg *const  pg_sport0_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport0_baddr;
volatile tag_DMA_SportReg *const  pg_sport1_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport1_baddr;
volatile tag_DMA_SportReg *const  pg_sport2_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport2_baddr;
volatile tag_DMA_SportReg *const  pg_sport3_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport3_baddr;
volatile tag_DMA_SportReg *const  pg_sport4_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport4_baddr;
volatile tag_DMA_SportReg *const  pg_sport5_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport5_baddr;
volatile tag_DMA_SportReg *const  pg_sport6_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport6_baddr;
volatile tag_DMA_SportReg *const  pg_sport7_dma_reg=(volatile tag_DMA_SportReg* )cn_dma_sport7_baddr;

volatile tag_TXRX_SportReg *const pg_sport_group1_txrx_reg=(volatile tag_TXRX_SportReg *)cn_sport_txrx_group1_baddr;
volatile tag_TXRX_SportReg *const pg_sport_group2_txrx_reg=(volatile tag_TXRX_SportReg *)cn_sport_txrx_group2_baddr;
volatile tag_TXRX_SportReg *const pg_sport_group3_txrx_reg=(volatile tag_TXRX_SportReg *)cn_sport_txrx_group3_baddr;
volatile tag_TXRX_SportReg *const pg_sport_group4_txrx_reg=(volatile tag_TXRX_SportReg *)cn_sport_txrx_group4_baddr;





// ----SPORTģ��ģ��SPI��������-------------------------------------------
//���ܣ�ʹ��SPORTĳ���˿�
//������SportNum��SPORT�ڱ�ţ�SPORT0~SPORT7��Ӧ��ŷֱ�Ϊ1~8��
//���أ�true=�ɹ���false=ʧ��
// ------------------------------------------------------------------------


bool_t EnableSport(u32 SportNum)
{
   switch (SportNum)
    {
    case 0:
            *pSPCTL0=SPEN_A;
              break;
            case 1:
            *pSPCTL1=SPEN_A;
        break;
            case 2:
            *pSPCTL2=SPEN_A;
              break;
            case 3:
            *pSPCTL3=SPEN_A;
        break;

            case 4:
            *pSPCTL4=SPEN_A;
              break;
            case 5:
            *pSPCTL5=SPEN_A;
        break;
             case 6:
            *pSPCTL6=SPEN_A;
              break;
            case 7:
            *pSPCTL7=SPEN_A;
        break;

    default :
        return false;

    }

        return true;
}




// ----SPORTģ��ģ��SPI��������-------------------------------------------
//���ܣ�����SPORTĳ���˿�
//������SportNum��SPORT�ڱ�ţ�SPORT0~SPORT7��Ӧ��ŷֱ�Ϊ1~8��
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------


bool_t DisableSport(u32 SportNum)
{
   switch (SportNum)
    {
    case 0:
            *pSPCTL0&=(~SPEN_A);
              break;
            case 1:
            *pSPCTL1&=(~SPEN_A);
        break;
            case 2:
            *pSPCTL2&=(~SPEN_A);
              break;
            case 3:
            *pSPCTL3&=(~SPEN_A);
        break;

            case 4:
            *pSPCTL4&=(~SPEN_A);
              break;
            case 5:
            *pSPCTL5&=(~SPEN_A);
        break;
             case 6:
            *pSPCTL6&=(~SPEN_A);
              break;
            case 7:
            *pSPCTL7&=(~SPEN_A);
        break;

    default :
        return false;

    }

        return true;
}


// ----SPORT Init------------------------------------------
//���ܣ�����SPORT0/SPORT1�ж��߼���ʼ��������SPORTʱ�Ӽ���ʼ��SPORT���ƼĴ���
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//      TransMode��TransMode=0,SPORT����Ϊ���Ͷˣ�TransMode=1,SPORT����Ϊ���ն�
//      Baud,SPORT�������ʣ���40,000000��25,000000��10,000000��5,000000�ĸ���ѡ��
//      DMAMode,SPORT���ݴ���ģʽ��DMAMode=0��ʾ��DMA���ں˴��䣻DMAMode=1��ʾStandard DMA��
//      DMAMode=2��ʾChained-DMA��
//���أ��ɹ�����true��ʧ�ܷ���false
// --------------------------------------------------------------
/*
bool_t SportInit(u32 SportGroupNum,u32 Index,u8 TransMode,u32 Baud,u32 DMAMode)
{
     volatile  tag_SportReg *tpsport;
     u16 temp=0;
     switch(SportGroupNum)
        {
        case 1:
        tpsport=pg_sport_group1_reg ;
        break;
    case 2:
        tpsport=pg_sport_group2_reg ;
        break;
    case 3:
        tpsport=pg_sport_group3_reg ;
        break;
    case 4:
                tpsport=pg_sport_group1_reg ;
        break;
    default :
        return false;
        }
     if(Index==0)
        {
           temp=CN_CFG_PCLK1/(4*Baud)-1;
           tpsport->rDIV_H|=temp;
           //tpsport->rSPCTL_H=0;
           //tpsport->rSPMCTL_H=0;

        if(TransMode==0)
            {

        if(DMAMode==0)
            {
            tpsport->rSPCTL_H=SPTRAN;
            }
        else if(DMAMode==1)
            {
            tpsport->rSPMCTL_H|=ETDINTEN;
            tpsport->rSPCTL_H|=(CKRE |LFS| LAFS | SPTRAN|IFS| SLEN8 | FSR);
            }
        else if(DMAMode==2)
            {
            }
            }
    else if (TransMode==1)
        {
        if(DMAMode==0)
            {
            }
        else if (DMAMode==1)
            {
            tpsport->rSPCTL_H=( SLEN8 | LAFS| FSR | DITFS| LFS );
            }
        else if (DMAMode==2)
            {
            }
        else
            {
            return false;
            }
        }
        }
     else if (Index==1)
        {
         temp=CN_CFG_PCLK1/(4*Baud)-1;
     tpsport->rDIV_L|=temp;
     //tpsport->rSPCTL_L=0;
     //tpsport->rSPMCTL_L=0;

        if(TransMode==0)
            {

        if(DMAMode==0)
            {
            tpsport->rSPCTL_L=SPTRAN;
            }
        else if(DMAMode==1)
            {
            tpsport->rSPMCTL_L|=ETDINTEN;
            tpsport->rSPCTL_L|=(CKRE |LFS| LAFS | SPTRAN  | SLEN8 | FSR);
            }
        else if(DMAMode==2)
            {
            }
            }
    else if (TransMode==1)
        {
        if(DMAMode==0)
            {
            }
        else if (DMAMode==1)
            {
            tpsport->rSPCTL_L=( SLEN8 | LAFS| FSR | DITFS| LFS );
            }
        else if (DMAMode==2)
            {
            }
        else
            {
            return false;
            }
        }
        }
     else
        {
        return false;
        }
        return true;
}
*/


 void SportInit(u32 Baud)
 {
    volatile  tag_SportReg *tpsport;
    u16 temp=0;
    tpsport=pg_sport_group1_reg ;
    temp=CN_CFG_PCLK1/(4*Baud)-1;
    tpsport->rDIV_H|=temp;

    tpsport->rSPCTL_L|=FSR|LAFS|LFS|SLEN8|CKRE;  //SPORT0 ��
    tpsport->rSPCTL_H|=FSR|LAFS|IFS|LFS|SLEN8|ICLK|SPTRAN;  //SPORT1��


    //tpsport->rSPCTL_L|=FSR|LAFS|LFS|CKRE|SLEN8|SPTRAN;
    //tpsport->rSPCTL_H|=FSR|LAFS|IFS|SLEN8|ICLK;

 }




/*-------ʹ��SPORT DMA----------------------------------------------**
//  ���ܣ�ʹ��SPORT DMA��������DMA��ͨ��DMA��������ʱ������Ϊ������ʹ��SPORT��Ȼ������
//        DMA������������ʹ��DMA����������һ��DMA���䡣
//  ������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//        Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//        ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//  ���أ��ɹ�����true��ʧ�ܷ���false��

**--------------------------------------------------------------------*/
bool_t EnableSportDMA(u32 SportGroupNum,u32 Index )
{
    volatile  tag_SportReg *tpsport;
     switch(SportGroupNum)
        {
        case 1:
        tpsport=pg_sport_group1_reg ;
        break;
    case 2:
        tpsport=pg_sport_group2_reg ;
        break;
    case 3:
        tpsport=pg_sport_group3_reg ;
        break;
    case 4:
                tpsport=pg_sport_group1_reg ;
        break;
    default :
        return false;
        }
     if(Index==0)
        {
        tpsport->rSPCTL_H|=SDEN_A;
        }
     else if(Index==1)
        {
        tpsport->rSPCTL_L|=SDEN_A;
        }
        return true;
}


/*--------����SPORT DMA---------------------------------------------**
//  ���ܣ�����DMA��
//  ������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//        Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//        ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
    ���أ��ɹ�����true��ʧ�ܷ���false��

**--------------------------------------------------------------------*/
bool_t DisableSportDMA(u32 SportGroupNum,u32 Index )
{
    volatile  tag_SportReg *tpsport;
     switch(SportGroupNum)
        {
        case 1:
        tpsport=pg_sport_group1_reg ;
        break;
    case 2:
        tpsport=pg_sport_group2_reg ;
        break;
    case 3:
        tpsport=pg_sport_group3_reg ;
        break;
    case 4:
                tpsport=pg_sport_group1_reg ;
        break;
    default :
        return false;
        }
     if(Index==0)
        {
        tpsport->rSPCTL_H|=(~SDEN_A);
        }
     else if(Index==1)
        {
        tpsport->rSPCTL_L|=(~SDEN_A);
        }
        return true;
}



/*-------����SPORT DMA����----------------------------------------------**
    ���ܣ�����SPORT DMA������������������������ֱ�ΪIISP(DMA������ʼ��ַ)��CSP(DMA���䳤��)��
//        IMSP(DMAÿ�δ�����ʵ�ַ�������ȣ�Ĭ�϶���Ϊ1)��
    ������SportNum��SPORT�ڱ�ţ�SPORT0~SPORT7��Ӧ��ŷֱ�Ϊ1~8��
//        *pData��Ҫ���������bufferָ�룻
//        DataLen,Ҫ��������ݳ���(�ֽ���)
    ���أ��ɹ�����true��ʧ�ܷ���false��

**--------------------------------------------------------------------*/
bool_t InitSportDMA(u32 SportNum,u8* pData,u32 DataLen)
{
    volatile tag_DMA_SportReg *tgdmasport;
     switch(SportNum)
        {
        case 1:
        tgdmasport=pg_sport0_dma_reg;
        break;
    case 2:
        tgdmasport=pg_sport1_dma_reg;
        break;
    case 3:
        tgdmasport=pg_sport2_dma_reg;
        break;
    case 4:
                tgdmasport=pg_sport3_dma_reg;
        break;
    case 5:
         tgdmasport=pg_sport4_dma_reg;
         break;
            case 6:
        tgdmasport=pg_sport5_dma_reg;
        break;
    case 7:
                tgdmasport=pg_sport6_dma_reg;
        break;
    case 8:
         tgdmasport=pg_sport7_dma_reg;
         break;

    default :
        return false;
        }
     tgdmasport->rIISPA=(int)pData;
     tgdmasport->rIMSPA=1;
     tgdmasport->rCSPA=DataLen;
     return true;
}



// ----SPORT1ģ�鷢���жϷ�����-------------------------------------------
//���ܣ��÷��ͳɹ���־λ��ͬʱ����SPORT DMA��
//�������жϺš�
//���أ��ɹ�����true��ʧ�ܷ���false��
// ------------------------------------------------------------------------

u32 SPORT1_TX_IRQ(ufast_t ufl_line)
{

    SPORT1_Send_Flag=true;
    //DisableSport(1);
    DisableSportDMA(1, 0);
    return true;
}

// ----SPORT1ģ������жϷ�����-------------------------------------------
//���ܣ��ý��ճɹ���־λ��ͬʱ����SPORT DMA��
//�������жϺš�
//���أ��ɹ�����true��ʧ�ܷ���false��
// ------------------------------------------------------------------------

u32 SPORT1_RX_IRQ(ufast_t ufl_line)
{
    SPORT1_Recv_Flag=true;
    //DisableSport(1);
    DisableSportDMA(1, 0);
    return true;
}

// ----SPORT0ģ������жϷ�����-------------------------------------------
//���ܣ��ý��ճɹ���־λ��ͬʱ����SPORT DMA��
//�������жϺš�
//���أ��ɹ�����true��ʧ�ܷ���false��
// ------------------------------------------------------------------------
u32 SPORT0_RX_IRQ(ufast_t ufl_line)
{
    SPORT0_Recv_Flag=true;
    //DisableSport(0);
    DisableSportDMA(1, 1);
    return true;
}


// ----SPORT1ģ�鷢���жϷ�����-------------------------------------------
//���ܣ��÷��ͳɹ���־λ��ͬʱ����SPORT DMA��
//�������жϺš�
//���أ��ɹ�����true��ʧ�ܷ���false��
// ------------------------------------------------------------------------
u32 SPORT0_TX_IRQ(ufast_t ufl_line)
{
    SPORT0_Send_Flag=true;
    //DisableSport(0);
    DisableSportDMA(1, 1);
    return true;
}


// ----SPORT Init------------------------------------------
//���ܣ�����SPORT0/SPORT1�ж��߼���ʼ��������SPORTʱ�Ӽ���ʼ��SPORT���ƼĴ���
//������SportNum��SPORT�ڱ�ţ�SPORT0~SPORT7��Ӧ��ŷֱ�Ϊ1~8��
//      TransMode��TransMode=0,SPORT����Ϊ���Ͷˣ�TransMode=1,SPORT����Ϊ���ն�
//���أ��ɹ�����true��ʧ�ܷ���false��
// --------------------------------------------------------------
bool_t Sport_Interrupt_Init(u32 SportNum,u32 TransMode)
{
     if(SportNum==0)
        {
        if(TransMode==0)
            {
             Int_IsrConnect(cn_int_line_SP0I,SPORT0_TX_IRQ);

            }
    else if(TransMode==1)
        {
        Int_IsrConnect(cn_int_line_SP0I,SPORT0_RX_IRQ);
        }
    else
        {
        return false;
        }
        }
     else if (SportNum==1)
        {
        if(TransMode==0)
            {
             Int_IsrConnect(cn_int_line_SP1I,SPORT1_TX_IRQ);

            }
    else if(TransMode==1)
        {
        Int_IsrConnect(cn_int_line_SP1I,SPORT1_RX_IRQ);
        }
    else
        {
        return false;
        }
        }
     else
        {
        }
    return true;
}





//------ͨ��DMA����һ������ΪLen buffer----------------------------------
  //  ���ܣ�ͨ��DMA����һ������ΪLen buffer��
  //  ������SportNum��SPORT�ڱ�ţ�SPORT0~SPORT7��Ӧ��ŷֱ�Ϊ1~8��
   //       *pData,��������buffer��ʼ��ַ��
   //       Len,Ҫ�������ݳ��ȡ�
//  ���أ��ɹ�����true��ʧ�ܷ���false��

//--------------------------------------------------------------------
/*
bool_t SPORT_TX_SEM(u32 SportNum,u8 *pData,u32 Len)
{
    InitSportDMA( SportNum, pData,Len);
    SportInit( 1,0,0, Baud_10,1);      //SPORT1 ����10MHz, Standard DMA
    EnableSportDMA(1, 0);               //ʹ��DMA
    //EnableSport(1);                         //ʹ��SPORT
     return true;
}


//---------ͨ��DMA����һ������ΪLen buffer----------------------------
   // ���ܣ�ͨ��DMA����һ������ΪLen buffer��
   // ������SportNum��SPORT�ڱ�ţ�SPORT0~SPORT7��Ӧ��ŷֱ�Ϊ1~8��
    //      *pData,��Ž�������buffer��ʼ��ַ��
     //     Len,Ҫ�������ݳ��ȡ�
//  ���أ��ɹ�����true��ʧ�ܷ���false��
//--------------------------------------------------------------------
bool_t SPORT_RX_SEM(u32 SportNum,u8 *pData,u32 Len)
{
    InitSportDMA( SportNum, pData,Len);
    SportInit( 1,0,0, Baud_10,1);      //SPORT0 �� 10MHz, Standard DMA
    EnableSportDMA(1, 1);               //ʹ��DMA
   // EnableSport(0);                         //ʹ��SPORT
     return true;
}
*/

// ----SPORTģ���д״̬-------------------------------------------
//���ܣ�ȷ��д���ݴ��ھ���״̬
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------
bool_t SPORT_Check_TX_Ready(u32 SportGroupNum,u32 Index)
{
    volatile  tag_SportReg *tpsport;
    int nTimeout=10000;

     switch(SportGroupNum)
        {
        case 1:
        tpsport=pg_sport_group1_reg ;
        break;
    case 2:
        tpsport=pg_sport_group2_reg ;
        break;
    case 3:
        tpsport=pg_sport_group3_reg ;
        break;
    case 4:
                tpsport=pg_sport_group1_reg ;
        break;
    default :
        return false;
        }
     if(Index==0)   //SPORT��Ŵ���Ǹ�SPORT
        {
        while((DA_S_Full&tpsport->rSPCTL_H))
            {
            if(nTimeout--<0)
                {
                return false;
                }
            }
    return true;
        }
     else if(Index==1)
        {
        while((DA_S_Full&tpsport->rSPCTL_L))
            {
            if(nTimeout--<0)
                {
                return false;
                }
            }
    return true;
        }
     else
        {
        return false;
        }
}

// ----SPORTģ���д״̬-------------------------------------------
//���ܣ�ȷ�϶����ݴ��ھ���״̬
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------
bool_t SPORT_Check_RX_Ready(u32 SportGroupNum,u32 Index)
{
   volatile  tag_SportReg *tpsport;
    int nTimeout=10000;

     switch(SportGroupNum)
        {
        case 1:
        tpsport=pg_sport_group1_reg ;
        break;
    case 2:
        tpsport=pg_sport_group2_reg ;
        break;
    case 3:
        tpsport=pg_sport_group3_reg ;
        break;
    case 4:
                tpsport=pg_sport_group1_reg ;
        break;
    default :
        return false;
        }
     if(Index==0)   //SPORT��Ŵ���Ǹ�SPORT
        {
        while((DA_S_Empty&tpsport->rSPCTL_H))
            {
            if(nTimeout--<0)
                {
                return false;
                }
            }
    return true;
        }
     else if(Index==1)
        {
        while((DA_S_Empty&tpsport->rSPCTL_L))
            {
            if(nTimeout--<0)
                {
                return false;
                }
            }
    return true;
        }
     else
        {
        return false;
        }
}


// ----SPORTģ�鷢��һ��Byte����-------------------------------------------
//���ܣ�SPORT����һ��Byte����
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------
UINT32 SPORT_txByte(u32 SportGroupNum,u32 Index,u8 Data)
{
   UINT32 nStatus=0;
   volatile tag_TXRX_SportReg *tp_txrx_reg;

   if(SPORT_Check_TX_Ready(SportGroupNum,Index))
    {
    switch(SportGroupNum)
        {
        case 1:
        tp_txrx_reg=pg_sport_group1_txrx_reg;
        break;
        case 2:
        tp_txrx_reg=pg_sport_group2_txrx_reg;
        break;
        case 3:
        tp_txrx_reg=pg_sport_group3_txrx_reg;
        break;
        case 4:
        tp_txrx_reg=pg_sport_group4_txrx_reg;
        break;
        default:
        return false;
        }
    if(Index==0)
        {
        tp_txrx_reg->rTXSPA_H=Data;
        nStatus=0x1;
        }
    else if(Index==1)
        {
        tp_txrx_reg->rTXSPA_L=Data;
        nStatus=0x1;
        }
    else
        {
        return false;
        }
    }
   return nStatus;
}



// ----SPORT����һ��Byte����-------------------------------------------
//���ܣ�SPORT����һ��Byte��
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      *pData����Ž��յ����ݵĵ�ַ
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------
UINT32 SPORT_rxByte(u32 SportGroupNum,u32 Index,u8 *pData)
{
   UINT32 nStatus=0;
   volatile tag_TXRX_SportReg *tp_txrx_reg;

   if(SPORT_Check_TX_Ready(SportGroupNum,Index))
    {
    switch(SportGroupNum)
        {
        case 1:
        tp_txrx_reg=pg_sport_group1_txrx_reg;
        break;
        case 2:
        tp_txrx_reg=pg_sport_group2_txrx_reg;
        break;
        case 3:
        tp_txrx_reg=pg_sport_group3_txrx_reg;
        break;
        case 4:
        tp_txrx_reg=pg_sport_group4_txrx_reg;
        break;
        default:
        return false;
        }
    if(Index==0)
        {
        *pData=tp_txrx_reg->rRXSPA_H;
        nStatus=0x1;
        }
    else if(Index==1)
        {
        *pData=tp_txrx_reg->rRXSPA_L;
        nStatus=0x1;
        }
    else
        {
        return false;
        }
    }
   return nStatus;
}


// ----SPORT����һ��Buffer����-------------------------------------------
//���ܣ�SPORT���ͳ���Ϊlen��Buffer����
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//      *send_buf��Ҫ���͵�buffer�׵�ַ��
//      len��Ҫ���͵�buffer����(�ֽ���)
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------
UINT32 SPORT_txArray(u32 SportGroupNum,u32 Index,u8 *send_buf,u32 len)
{
  volatile tag_TXRX_SportReg *tp_txrx_reg;

     u32 i=0;
    for(i=0;i<len;i++)
        {
        SPORT_txByte( SportGroupNum,  Index, *(send_buf+i));
        }
    return true;
}

// ----SPORT����һ��Buffer����-------------------------------------------
//���ܣ�SPORT���ճ���Ϊlen��Buffer����
//������SporGrouptNum,SPORT���ţ�SPORT0~SPORT7 8��SPORT���λ���Ϊ4 groups
//      Index,һ��group���������SPORT������group1����SPORT0��SPORT1��Index��������1��group
//      ��SPORT�ڣ�Index=0��ʾSPORT��Ŵ����һ����Index=1��ʾSPORT���С����һ����
//      *recv_buf����Ž������ݵ��׵�ַ��
//      len��Ҫ���͵�buffer����(�ֽ���)
//���أ��ɹ�����true��ʧ�ܷ���false
// ------------------------------------------------------------------------
UINT32 SPORT_rxArray(u32 SportGroupNum,u32 Index,u8 *recv_buf,u32 len)
{
     u32 i=0;
    for(i=0;i<len;i++)
        {
        SPORT_rxByte(SportGroupNum, Index, (recv_buf+i));
        }
    return true;
}

