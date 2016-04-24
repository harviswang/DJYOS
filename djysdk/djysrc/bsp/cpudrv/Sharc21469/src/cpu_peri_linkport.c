//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: linkport��������(������������)
//����:  �캣��.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2014-4-25
//   ����:  �캣��.
//   �°汾�ţ�1.0.0
//   �޸�˵��:
//------------------------------------------------------



#include <string.h>
#include "stdint.h"
#include "cpu_peri.h"

#include "def21469.h"
#include "Cdef21469.h"

volatile tag_LinkPortReg * const pg_linkport0_reg =  (volatile tag_LinkPortReg *)cn_linkport0_baddr;
volatile tag_LinkPortReg * const pg_linkport1_reg =  (volatile tag_LinkPortReg *)cn_linkport1_baddr;
volatile tag_DMA_LinkPortReg * const pg_dma_linkport0_reg =  (volatile tag_DMA_LinkPortReg *)cn_dma_linkport0_baddr;
volatile tag_DMA_LinkPortReg * const pg_dma_linkport1_reg =  (volatile tag_DMA_LinkPortReg *)cn_dma_linkport1_baddr;

#define  DMA_OFFSET 0x80000

#define Rate_2       1
#define Rate_2_5   2
#define Rate_3       3
#define Rate_4        4

bool_t LP0SendFlag=false,LP1SendFlag=false;
bool_t LP0RecvFlag=false,LP1RecvFlag=false;

#define Chained_DMA_Max_Num   10
static  u32 DSP_FPGA_BUF_LEN[Chained_DMA_Max_Num];
static  u32 DSP_FPGA_BUF_ADDR[Chained_DMA_Max_Num];
static  u32 Chained_Send_Index=0x0;

struct t_LinkPortChainedDMA_TCB LinkPort_ChainedDMA_TCB[Chained_DMA_Max_Num]; //10��TCB��
struct t_LinkPortChainedDMA_TCB LinkPort_ChainedDMA_Send_TCB[Chained_DMA_Max_Num];

/*-------InitLinkPort��ʼ��------------------------------------------------**
//  ���ܣ�����LinkPort���ݴ���ģʽ��
//  ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
//        TransMode,LinkPort����ģʽ��TransMode=0��LinkPort��Ϊ���ͣ�TransMode=1,LinkPort��Ϊ���ա�
//        DMAMode��DMAMode,LinkPort���ݴ���ģʽ��DMAMode=0��ʾ��DMA���ں˴��䣻DMAMode=1��ʾStandard DMA��
//        DMAMode=2��ʾChained-DMA��
//  ���أ��ɹ�������true�����򷵻�false��
**--------------------------------------------------------------------*/
bool_t  LinkPort_Init(u32 LinkPortNum,u8 TransMode,u8 DMAMode)
{
      volatile tag_LinkPortReg *tplinkport;
    u32 i=0;
     if(LinkPortNum== 0)
    {
        tplinkport = pg_linkport0_reg;
    }
    else if (LinkPortNum==1)
    {
         tplinkport = pg_linkport1_reg;
    }
    else
    {
        return false;
    }

   if(TransMode==0)
    {
        tplinkport->rLCTL=0;
                if(DMAMode==0)
                    {
                    tplinkport->rLCTL=LTRAN|EXTTXFR_DONE_MSK;     //���ͣ�Coreģʽ����ʹ��DMA
                    }
                else if(DMAMode==1)
                    {
                    tplinkport->rLCTL=LTRAN | LDEN | EXTTXFR_DONE_MSK ; //LinkPort������  DMAʹ��
                    }
       else if(DMAMode==2)
        {
        tplinkport->rLCTL=LTRAN | LDEN | LCHEN|EXTTXFR_DONE_MSK ;
        }
        else
            {
            return false;
            }

    }
      else if(TransMode==1)
        {
         tplinkport->rLCTL=0;
                    if(DMAMode==0)
                    {
                        }
                else if(DMAMode==1)
                    {
                    tplinkport->rLCTL= LDEN | DMACH_IRPT_MSK ; //LinkPort������  DMAʹ��
                    }
       else if(DMAMode==2)
        {
        tplinkport->rLCTL= LDEN | LCHEN|DMACH_IRPT_MSK;
        }
        else
            {
            return false;
            }
        }

      else
        {
            return false;
        }
      return true;
}


/*-------ʹ��LinkPort--------------------------------------------------**
//  ���ܣ�ʹ��LinkPort
//  ������Linkport��ţ�0/1/2�ֱ��ʾLinkport0,Linkport1,Linkport0&Linkport1
//  ���أ��ɹ�������true�����򷵻�false
**--------------------------------------------------------------------*/
bool_t  LinkPort_Enable(u32 LinkPortNum)
{
    u32 temp1;
   u32 temp2;
   temp1=*pLSTAT0_SHADOW;
   temp2=*pLSTAT1_SHADOW;
    if (LinkPortNum==0)
    {
        *pLCTL0 |= LEN ;
    }
    else if (LinkPortNum==1)
    {
        *pLCTL1 |= LEN ;
    }
    else if (LinkPortNum==2)
    {
        *pLCTL0 |= LEN ;
        *pLCTL1 |= LEN ;
    }
    else
    {
         return false;
    }
    return true;

}


/*------����LinkPort---------------------------------------------**
//  ���ܣ�����LinkPort
//  ������Linkport��ţ�0/1/2�ֱ��ʾLinkport0,Linkport1,Linkport0&Linkport1��
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
bool_t  LinkPort_Disable(u32 LinkPortNum)
{
    if (LinkPortNum==0)
    {
        *pLCTL0 &= (~LEN) ;
    }
    else if (LinkPortNum==1)
    {
        *pLCTL1 &= (~LEN) ;
    }
    else if (LinkPortNum==2)
    {
        *pLCTL0 &= (~LEN) ;
        *pLCTL1 &= (~LEN) ;
    }
    else   //��������
    {
                return false;
    }
    return true;


}

/*--------ʹ��LinkPortDMA��������DMA-----------------------------------------------**
//  ���ܣ�ʹ��DMA��������DMA��ͨ��DMA��������ʱ������Ϊ������ʹ��LinkPort��Ȼ������
//        DMA������������ʹ��DMA����������һ��DMA���䡣
//  ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
bool_t  LinkPort_EnableDMA(u32 LinkPortNum)
{


    if (LinkPortNum==0)
    {
        *pLCTL0 |= LDEN ;
    }
    else if (LinkPortNum==1)
    {
        *pLCTL1 |= LDEN ;
    }
    else if (LinkPortNum==2)
    {
        *pLCTL0 |= LDEN ;
        *pLCTL1 |= LDEN ;
    }
    else//��������
    {
         return false;
    }
    return true;
}

/*-------����LinkPortDMA----------------------------------------**
//  ���ܣ�����LinkPortDMA
//  ������Linkport��ţ�0/1/2�ֱ��ʾLinkport0,Linkport1,Linkport0&Linkport1��
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
bool_t LinkPort_DisableDMA(u32 LinkPortNum)
{
    if (LinkPortNum==0)
    {
        *pLCTL0 &= (~LDEN) ;
    }
    else if (LinkPortNum==1)
    {
        *pLCTL1 &= (~LDEN) ;
    }
    else if (LinkPortNum==2)
    {
        *pLCTL0 &= (~LDEN) ;
        *pLCTL1 &= (~LDEN) ;
    }
    else //��������
    {
           return false;
    }
    return true;
}

/*------LinkPort DMA�����������-------------------------------------------**
//  ���ܣ�����LinkPort DMA������������������������ֱ�ΪIILB(DMA������ʼ��ַ)��CLB(DMA���䳤��)��
//        IMLB(DMAÿ�δ�����ʵ�ַ�������ȣ�Ĭ�϶���Ϊ1)��
//  ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
//        *pData��������buffer����ʼ��ַ��
//        DataLen�����䳤�ȣ����ֽ�����
//  ���أ��ɹ�������true�����򷵻�false��
**--------------------------------------------------------------------*/
bool_t LinkPort_InitDMA(u32 LinkPortNum,u32* pData,u32 DataLen)
{
    if (LinkPortNum==0)
    {
        *pIILB0=((int)pData)-DMA_OFFSET; //ȡ��ַ��19λ
        *pCLB0=DataLen;
        *pIMLB0=1;
    }
    else if (LinkPortNum==1)
    {
        *pIILB1=((int)pData)-DMA_OFFSET; //ȡ��ַ��19λ
        *pCLB1=DataLen;
        *pIMLB1=1;
    }
    else //��������
    {
           return false;
    }
    return true;

}



/*-------LinkPort����һ��Byte����---------------------------------------**
//  ���ܣ�LinkPort����һ��Byte���ݣ�������DMA��ʽ������ͨ���ں˷�ʽ���Ȳ�ѯTransmit buffer״̬��
//        ��δ��������Ҫ���͵�����д��Transmit regsiter(TXLB)��
//  ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
//          Val,Ҫ���͵����ݡ�
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
u32  LinkPort_SendData(u32 LinkPortNum,u32 Val)
{
    u32 Tmp=0;

    if (LinkPortNum==0)
    {
        if ( ((*pLSTAT0_SHADOW)&0x60)!=0x60 )  //���ͻ���δ��
        {
            *pTXLB0=Val;
            Tmp=1;
        }
        else //���ͻ�������
        {
             Tmp=0;
        }
    }
    else if (LinkPortNum==1)
    {
        if ( ((*pLSTAT1_SHADOW)&0x60)!=0x60 ) //���ͻ���δ��
        {
            *pTXLB1=Val;
            Tmp=1;
        }
        else //���ͻ�������
        {
           Tmp=0;
        }//������
    }
    else
    {
    Tmp=0;
    }

    return (Tmp);
}


/*-------LinkPort����һ��Byte����--------------------------------------**
//  ���ܣ�LinkPort����һ��Byte���ݣ�������DMA��ʽ������ͨ���ں˷�ʽ���Ȳ�ѯreceive buffer״̬��
//        ���ǿգ�����receiver register�ж�ȡһ�����ݡ�
//  ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
//  ���أ����������ݡ�

**--------------------------------------------------------------------*/
u32 LinkPort_RecvData(u32 LinkPortNum)
{
    u32 Val=0;

    if (LinkPortNum==0)
    {
        if ( ((*pLSTAT0_SHADOW)&0x60)!=0x0 )  //�������ǿ�
        {
            Val=*pRXLB0;
        }
        else
        {
        return false;
        } //����ȡ
    }
    else if (LinkPortNum==1)
    {
        if ( ((*pLSTAT1_SHADOW)&0x60)!=0x0 )  //�������ǿ�
        {
            Val=*pRXLB1;
        }
        else
        {
        return false;
        } //����ȡ
    }
    else
        {
        return false;
        } //����ȡ

    return (Val);
}


/*-----LinkPort0�����жϷ�����-----------------------------------------**
//  ���ܣ�LinkPort0�жϷ�������
//  ������linkport0_int_line���жϺš�
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
u32 LinkPort0_RecvISR(ufast_t linkport0_int_line)
{
    volatile tag_LinkPortReg *tplinkport;
    int lstat = 0;

    tplinkport = pg_linkport0_reg;
    lstat = tplinkport->rLSTAT;      //��LSTAT�Ĵ��������жϱ�־λ

    if( (lstat & DMACH_IRPT ) != 0)
    {
        LP0RecvFlag = true;


        LinkPort_DisableDMA(0);


    }
    return 1;
}



/*-------LinkPort0�����жϷ�����------------------------------------------**
//  ���ܣ�LinkPort0���жϷ�����
//  ������linkport0_int_line���жϺš�
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
u32 LinkPort0_SendISR(ufast_t linkport0_int_line)
{
    volatile tag_LinkPortReg *tplinkport;
    int lstat = 0;

    tplinkport = pg_linkport0_reg;
    lstat = tplinkport->rLSTAT;      //��LSTAT�Ĵ��������жϱ�־λ

    if( (lstat & EXTTXFR_DONE ) != 0)
    {
        //tplinkport->rLCTL = 0;
        LP0SendFlag = true;
        //LinkPort_Disable(0);
        LinkPort_DisableDMA(0);
    }
    return 1;

}



/*-----LinkPort1�����жϷ�����-----------------------------------------**
//  ���ܣ�LinkPort1�жϷ�������
//  ������linkport1_int_line���жϺš�
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
u32  LinkPort1_RecvISR(ufast_t linkport1_int_line)
{
    volatile tag_LinkPortReg *tplinkport;
    int lstat = 0;

    tplinkport = pg_linkport1_reg;
    lstat = tplinkport->rLSTAT;

    if( (lstat & DMACH_IRPT ) != 0)
    {
        LP1RecvFlag = true;
        //LinkPort_Disable(1);
        LinkPort_DisableDMA(1);
    }
       return 1;
}



/*-------LinkPort1�����жϷ�����------------------------------------------**
//  ���ܣ�LinkPort1���жϷ�����
//  ������linkport1_int_line���жϺš�
//  ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
u32 LinkPort1_SendISR(ufast_t linkport1_int_line)
{
     volatile tag_LinkPortReg *tplinkport;
    int lstat = 0;

    tplinkport = pg_linkport1_reg;
    lstat = tplinkport->rLSTAT;

    if( (lstat & EXTTXFR_DONE ) != 0)
    {
        //tplinkport->rLCTL = 0;
        LP1SendFlag = true;
        //LinkPort_Disable(1);
        LinkPort_DisableDMA(1);
    }
    return 1;
}



//----��ʼ��linkportģ���ж�----------------------------------------------------------
//���ܣ���ʼ��linkportģ��
//������linkportģ��ѡ��
//���أ��ɹ�������true�����򷵻�false��
//-----------------------------------------------------------------------------

bool_t LinkPort_InterruptInit(u32 LinkPortNum,u32 TransMode)
{
    //volatile tag_DMA_LinkPortReg *tpdmalinkport;

     if(LinkPortNum == 0)
    {
        //tpdmalinkport = pg_dma_linkport0_reg;
      if(TransMode==0)
         {
            Int_IsrConnect(cn_int_line_LP0I,LinkPort0_SendISR);
         }
        else if(TransMode==1)
        {
                Int_IsrConnect(cn_int_line_LP0I,LinkPort0_RecvISR);
        }
        else
            return false;
    }
    else if (LinkPortNum == 1)
    {
         //tpdmalinkport = pg_dma_linkport1_reg;
       //�ж��߳�ʼ��
    if(TransMode==0)
        {
             Int_IsrConnect(cn_int_line_LP1I,LinkPort1_SendISR);
        }

        else if(TransMode==1)
            {
             Int_IsrConnect(cn_int_line_LP1I,LinkPort1_RecvISR);
            }

        else
            return false;

    }
    else
    {
        return false;
    }

    return true;

}


/*-------Chained-DMA������ʼ��ַ------------------------------------------------**
//  ���ܣ�����Chained-DMA������ʼ��ַ����DMA-Chain��һ��TCB��IILB�ĵ�ַ���ݸ�CPLB�Ĵ�����
          DSP�����Զ���DMA-Chained����TCB�������ص�DMA-Chained�Ĵ����С�
//  ���룺InitTCB_Addr,DMA-Chained TCBģ����ʼ��ַ��Ҳ����һ��TCB��IILB�Ĵ�����ַ
//  �������

**--------------------------------------------------------------------*/
void LinkPort_InitChainedDMA(int InitTCB_Addr)
{
    *pCPLB0=InitTCB_Addr;
}

/*------����LinkPort DMA Chained TCB--------------------------------------**
    ���ܣ�����DMA��TCB������ÿһ��TCB���ĸ�������ɣ��ֱ���IILB(��DMA�ڵ㴫����ʼ��ַ)��
          CLB(DMA���䳤��)��IMLB(DMAÿ�δ�����ʵ�ַ�������ȣ�Ĭ�϶���Ϊ1)��CPLB(��һ��TCB�����ַ)��
          ǰһ��TCB��CPLBֵ�뵱ǰTCB��IILBֵ��ͬ�����һ��TCB��CPLB����0��
    ������DMA�����ȣ����ڵ�����
    ���أ��ɹ�������true�����򷵻�false��
**--------------------------------------------------------------------*/

bool_t  LinkPort_SetLinkPortChainedTCB(int Chained_Num)
{
      u32 i=0;
    if(Chained_Num<=Chained_DMA_Max_Num)
    {
         for(i=0;i<Chained_Num-1;i++)
        {
        LinkPort_ChainedDMA_TCB[i].CPLB=(int)(&LinkPort_ChainedDMA_TCB[i+1].IILB)&0x7FFFF;
    LinkPort_ChainedDMA_TCB[i].CLB=DSP_FPGA_BUF_LEN[i];
    LinkPort_ChainedDMA_TCB[i].IMLB=1;
    LinkPort_ChainedDMA_TCB[i].IILB=DSP_FPGA_BUF_ADDR[i];
        }

    LinkPort_ChainedDMA_TCB[Chained_Num-1].CPLB=0;
    LinkPort_ChainedDMA_TCB[Chained_Num-1].CLB=DSP_FPGA_BUF_LEN[i];
    LinkPort_ChainedDMA_TCB[Chained_Num-1].IMLB=1;
    LinkPort_ChainedDMA_TCB[Chained_Num-1].IILB=DSP_FPGA_BUF_ADDR[i];

    LinkPort_InitChainedDMA((int)(&LinkPort_ChainedDMA_TCB[0].IILB)&0x7FFFF );
     }
      else
        {
        return false;
        }

  return true;
}



/*-------���DMA���ڵ�---------------------------------------**
//  ���ܣ����һ��DMA���ڵ㣬ע�⵱ǰDMA����δ���䣬�˺�������DSP���β�ѯ5����������
          ��������������Ҫ���䣬������һ�α����������һ��DMA���ڵ㣬�����л�����
          �����ϣ����������DMA��TCB���á�
    ������addr,Ҫ��������buffer����ʼ��ַ;
          Len,��������ݳ��ȡ�
    ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/
bool_t SetDMAChainedTCB_Send(u32 addr,u32 Len)
{
           if(Chained_Send_Index<10)
            {
            LinkPort_ChainedDMA_Send_TCB[Chained_Send_Index].CPLB=(int)(&LinkPort_ChainedDMA_Send_TCB[Chained_Send_Index+1].IILB)&0x7FFFF;
    LinkPort_ChainedDMA_Send_TCB[Chained_Send_Index].CLB=Len;
    LinkPort_ChainedDMA_Send_TCB[Chained_Send_Index].IMLB=1;
    LinkPort_ChainedDMA_Send_TCB[Chained_Send_Index].IILB=addr;
    Chained_Send_Index++;
            }
     else
        {
        return false;
        }
     return true;
}

/*------����DMA����-----------------------------------**
    ���ܣ�����������DSP��FPGA�������ݣ���DMA��������ɣ�����DMA���͡�
    ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
    ���أ���

**--------------------------------------------------------------------*/
void LinkPort_EnableDMA_Send(u32 LinkPortNum)
{
      //LinkPort_Enable(LinkPortNum);
      LinkPort_ChainedDMA_Send_TCB[Chained_Send_Index-1].CPLB=0;
      LinkPort_InitChainedDMA((int)(&LinkPort_ChainedDMA_Send_TCB[0].IILB)&0x7FFFF );
      LinkPort_EnableDMA(LinkPortNum);

}

/*-------����LinkPort DMA����------------------------------------------**
    ���ܣ�����DMA����
    ������LinkPortNum,LinkPort��ţ�LinkPortNum=0��ָʹ��LinkPort0��LinkPortNum=1ָʹ��LinkPort1��
    ���أ��ɹ�������true�����򷵻�false��

**--------------------------------------------------------------------*/

void EnableLinkPortDMA_Recv(u32 LinkPortNum)
{
      LinkPort_EnableDMA(LinkPortNum);

}



//----�������ݳس���-------------------------------------------------------------------
//���ܣ�����ĳ��ָ���������򳤶�
//���������������ţ��������򳤶�
//���أ��ɹ�����true,���򷵻�false
//----------------------------------------------------------------------------------

bool_t LinkPort_SetRecvDataPoolLen(u8 Data_Pool_Num, u32 Len)
{

    switch(Data_Pool_Num)
    {
        case 1:
                  DSP_FPGA_BUF_LEN[0]=Len;
                      break;
        case 2:
                 DSP_FPGA_BUF_LEN[1]=Len;
               break;
                case 3:
                 DSP_FPGA_BUF_LEN[2]=Len;
               break;
                case 4:
                 DSP_FPGA_BUF_LEN[3]=Len;
               break;
                case 5:
                 DSP_FPGA_BUF_LEN[4]=Len;
               break;

                        case 6:
                 DSP_FPGA_BUF_LEN[5]=Len;
               break;
                case 7:
                 DSP_FPGA_BUF_LEN[6]=Len;
               break;
                case 8:
                 DSP_FPGA_BUF_LEN[7]=Len;
               break;
                case 9:
                 DSP_FPGA_BUF_LEN[8]=Len;
               break;

                        case 10:
                 DSP_FPGA_BUF_LEN[9]=Len;
               break;

                 default:
             return false;


    }
    return true;
}

//----�������ݳ���ʼ��ַ-------------------------------------------------------------------
//���ܣ�����ĳ������������ʼ��ַ��
//���������������ţ�����������ʼ��ַ��
//���أ��ɹ�����true,���򷵻�false��
//----------------------------------------------------------------------------------

bool_t LinkPort_SetRecvDataPoolAddr(u8 Data_Pool_Num,u32 addr)
{
        switch(Data_Pool_Num)
    {
        case 1:
                  DSP_FPGA_BUF_ADDR[0]=addr;
                      break;
        case 2:
                 DSP_FPGA_BUF_ADDR[1]=addr;
               break;
                case 3:
                 DSP_FPGA_BUF_ADDR[2]=addr;
               break;
                case 4:
                 DSP_FPGA_BUF_ADDR[3]=addr;
               break;
                case 5:
                 DSP_FPGA_BUF_ADDR[4]=addr;
               break;

                case 6:
                  DSP_FPGA_BUF_ADDR[5]=addr;
                      break;
        case 7:
                 DSP_FPGA_BUF_ADDR[6]=addr;
               break;
                case 8:
                 DSP_FPGA_BUF_ADDR[7]=addr;
               break;
                case 9:
                 DSP_FPGA_BUF_ADDR[8]=addr;
               break;
                case 10:
                 DSP_FPGA_BUF_ADDR[9]=addr;
               break;

                 default:
             return false;

    }
    return true;
}


//----LinkPortĬ�ϳ�ʼ������-------------------------------------------------------------------
//���ܣ��˺���Ϊ���Խ׶�ʹ�á�
//��������
//���أ���
//----------------------------------------------------------------------------------


void  LinkPort_DefaultIntInit(void)
{
       LinkPort_InterruptInit(0, 1);      //  LinkPort0   Recv
       LinkPort_InterruptInit(1, 0);      //  LinkPort1   Send
       LinkPort_Init(0, 1, 2) ;            //LinkPort0 Recv  Chained-DMA
       LinkPort_Init(1, 0, 1);        //LinkPort1 Send DMA
}













