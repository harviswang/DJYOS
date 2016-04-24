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
// �ļ���     ��cpu_peri_spi.c
// ģ������: SPIģ��Ӳ������������SPI BUSģ��Ҫ��д������
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 08/10.2014
// =============================================================================

#include "config-prj.h"
#include "os.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "string.h"
#include "int_hard.h"

// =============================================================================
#define min(x, y)    (((x) < (y)) ? (x) : (y))

#define ESPI_MAX_CS_NUM     4

#define ESPI_EV_RNE                 (1 << 9)
#define ESPI_EV_RXCNT_OFF           (0x3F << 24)
#define ESPI_EV_RXCNT(x)            ((x & ESPI_EV_RXCNT_OFF)>> 24)
#define ESPI_EV_TNF                 (1 << 8)
#define ESPI_EV_TXE                 (1 << 15)

#define ESPI_MODE_EN                (1 << 31)   /* Enable interface */
#define ESPI_MODE_TXTHR(x)          ((x) << 8)  /* Tx FIFO threshold */
#define ESPI_MODE_RXTHR(x)          ((x) << 0)  /* Rx FIFO threshold */

#define ESPI_COM_CS(x)              ((x) << 30)
#define ESPI_COM_RXSKIP(x)          ((x) << 15)
#define ESPI_COM_TRANLEN(x)         ((x) << 0)

#define ESPI_CSMODE_CI_INACTIVEHIGH (1 << 31)
#define ESPI_CSMODE_CP_BEGIN_EDGCLK (1 << 30)
#define ESPI_CSMODE_REV_MSB_FIRST   (1 << 29)
#define ESPI_CSMODE_DIV16           (1 << 28)
#define ESPI_CSMODE_PM(x)           ((x) << 24)
#define ESPI_CSMODE_POL_ASSERTED_LOW    (1 << 20)
#define ESPI_CSMODE_LEN(x)          ((x) << 16)
#define ESPI_CSMODE_CSBEF(x)        ((x) << 12)
#define ESPI_CSMODE_CSAFT(x)        ((x) << 8)
#define ESPI_CSMODE_CSCG(x)         ((x) << 3)

#define ESPI_CSMODE_INIT_VAL (ESPI_CSMODE_POL_ASSERTED_LOW | \
        ESPI_CSMODE_CSBEF(0) | ESPI_CSMODE_CSAFT(0) | \
        ESPI_CSMODE_CSCG(1))

#define ESPI_SPIM_TXE_MASK     (1<<15)
#define ESPI_SPIM_RXT_MASK     (1<<13)

/*һ֡�������������ٸ�����*/
#define ESPI_MAX_DATA_TRANSFER_LEN 0xFFF0
static u8 spi_cs = 0;/*CS�ľ�̬������������Դ�ļ�����*/

// =============================================================================
//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};

#define CN_SPI_BASE        cn_spi_baddr
#define CN_SPI_BUF_LEN      128
static struct tagSPI_CB s_SPI_CB;
static u8 s_SPI_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset;





void __SPI_TxRxIntEnable(tagSpiReg *Reg)
{
    Reg->mask |=  (ESPI_SPIM_TXE_MASK);
}

void __SPI_TxRxIntDisable(tagSpiReg *Reg)
{
    Reg->mask &= ~(ESPI_SPIM_TXE_MASK);
}




//-----����ʱ���ź�---------------------------------------------------------
//���ܣ�����ʱ���źţ����������clk����д��Ĵ����е�ֵ
//������freq_hz��ʱ��Ƶ��
//      div16������ֵ���������üĴ���
//���أ�pm,���üĴ�����ֵ
//----------------------------------------------------------------------
u8 __spi_clk_cal(u32 freq_hz, u32 *div16)
{
    u32 spibrg;
    u8 pm;

    /* Set eSPI BRG clock source */
    spibrg = cfg_core_ccb_clk / 2;
    *div16 = 0;
    if ((spibrg / freq_hz) > 32)
        {
        *div16 = ESPI_CSMODE_DIV16;
        pm = spibrg / (freq_hz * 16 * 2);
        if (pm > 16)
            {
            pm = 16;
//          djy_printf("Requested speed is too low: %d Hz, %ld Hz "
//              "is used.\r\n", max_hz, spibrg / (32 * 16));
        }
    }
        else
        pm = spibrg / (freq_hz * 2);
    if (pm)
        pm--;

    return pm;
}

//-----Ƭѡ����---------------------------------------------------------
//���ܣ�Ƭѡʹ�ܣ�����ʼ���Ĵ���cs��Ӧ�ļĴ���
//������tpSpi����������spi���ƽṹ�ĵ�ַ
//      data_len,���͵����ݳ���
//���أ���
//----------------------------------------------------------------------
void __spi_cs_activate(volatile tagSpiReg * tpSpi,u32 data_len)
{
    unsigned int com = 0;

    com &= ~(ESPI_COM_CS(0x3) | ESPI_COM_TRANLEN(0xFFFF));
    com |= ESPI_COM_CS(spi_cs);
    com |= ESPI_COM_TRANLEN(data_len - 1);
    tpSpi->com = com;
}

//-----Ƭѡ�ر�---------------------------------------------------------
//���ܣ�Ƭѡʧ�ܣ�����ʼ���Ĵ���cs��Ӧ�ļĴ���
//������tpSpi����������spi���ƽṹ�ĵ�ַ
//���أ���
//----------------------------------------------------------------------
void __spi_cs_deactivate(volatile tagSpiReg * tpSpi)
{
    /* clear the RXCNT and TXCNT */
    tpSpi->mode &= ~ESPI_MODE_EN;   /*�������δ������*/
    tpSpi->mode |= ESPI_MODE_EN;    /*����ʹ��SPI*/
}
// =============================================================================
// ���ܣ�SPIʱ�����ú�����ʱ����ԴΪ50M����SCR��CPSR��Ƶ�õ�ʱ�ӣ�ʱ�Ӽ��㹫ʽΪ��
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// ������spi_dev���豸���
//      spiclk�������õ�ʱ���ٶȣ���λΪHz
// ���أ�true=�ɹ���false=ʧ��
// ˵�����������̶�SCR = 1;�������ֲᣬCPSDVSR����Ϊ2-254��ż������ˣ�Ƶ�ʷ�ΧΪ
//       12.5M ~ 100kHz
// =============================================================================
void __SPI_SetClk(volatile tagSpiReg *tpSPI,u32 spisck)
{
//  u32 temp;
//  if(tpSPI == NULL)
//      return;
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
void __SPI_Config(volatile tagSpiReg *tpSpi,u8 cs,tagSpiConfig *tagpInConfig)
{
    u8 pm;
    u32 mode,div16;

    mode = tagpInConfig->Mode;
    pm = __spi_clk_cal(tagpInConfig->Freq,&div16);

    /* Enable eSPI interface */
    tpSpi->mode = ESPI_MODE_RXTHR(3) | ESPI_MODE_TXTHR(4) | ESPI_MODE_EN;

    tpSpi->event  = 0xffffffff; /* Clear all eSPI events */
    tpSpi->mask  = 0x00000000; /* Mask  all eSPI interrupts */

    /* Init CS0,1,2,3 mode interface */
    if(cs < 4)
    {
        tpSpi->csmode[cs] = ESPI_CSMODE_INIT_VAL;

        tpSpi->csmode[cs] = tpSpi->csmode[cs] &
            ~(ESPI_CSMODE_PM(0xF) | ESPI_CSMODE_DIV16
            | ESPI_CSMODE_CI_INACTIVEHIGH | ESPI_CSMODE_CP_BEGIN_EDGCLK
            | ESPI_CSMODE_REV_MSB_FIRST | ESPI_CSMODE_LEN(0xF));

        /* Set eSPI BRG clock source */
        tpSpi->csmode[cs] = tpSpi->csmode[cs] | ESPI_CSMODE_PM(pm) | div16;

        /* Set eSPI mode */
        if (mode & SPI_CPHA)
            tpSpi->csmode[cs] = tpSpi->csmode[cs] | ESPI_CSMODE_CP_BEGIN_EDGCLK;
        if (mode & SPI_CPOL)
            tpSpi->csmode[cs] = tpSpi->csmode[cs] | ESPI_CSMODE_CI_INACTIVEHIGH;

        /* Character bit order: msb/lsb first */
        if(tagpInConfig->ShiftDir == SPI_SHIFT_MSB)
            tpSpi->csmode[cs] = tpSpi->csmode[cs] | ESPI_CSMODE_REV_MSB_FIRST;
        else
            tpSpi->csmode[cs] = tpSpi->csmode[cs] & (~ESPI_CSMODE_REV_MSB_FIRST);

        /* Character length in bits, between 0x3~0xf, i.e. 4bits~16bits */
        tpSpi->csmode[cs] = tpSpi->csmode[cs] | ESPI_CSMODE_LEN(7);
    }
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __SPI_HardDefaultSet(u32 BaseAddr)
{
    u8 i;
    tagSpiConfig    pg_spi_Config = {SPI_MODE_3,SPI_SHIFT_MSB,8,600000};
    for(i = 0; i < 4; i++)
    {
        __SPI_Config((volatile tagSpiReg *)BaseAddr,i,&pg_spi_Config);
    }
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __SPI_IntConfig(void)
{
    u8 IntLine = cn_int_line_espi;
    //�ж��ߵĳ�ʼ��
    u32 SPI_ISR(ufast_t IntLine);
    Int_IsrConnect(IntLine,SPI_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_SetLineTrigerType(IntLine,EN_INT_TRIGER_HIGHLEVEL);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SPI��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
//  unsigned int com = 0;

//  com &= ~(ESPI_COM_CS(0x3) | ESPI_COM_TRANLEN(0xFFFF));
//  com |= ESPI_COM_CS(cs);
//  com |= ESPI_COM_TRANLEN(data_len - 1);
//  Reg->com = com;
    spi_cs = cs;
    return true;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    /* clear the RXCNT and TXCNT */
//  Reg->mode &= ~ESPI_MODE_EN; /*�������δ������*/
//  Reg->mode |= ESPI_MODE_EN;  /*����ʹ��SPI*/
    return true;
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __SPI_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if(Reg == NULL)
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        break;
    case CN_SPI_CS_CONFIG:
        __SPI_Config(Reg,data1,(tagSpiConfig *)data2);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        // Ӳ����������Զ�����CS������
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// ���ܣ�SPI�ײ��������ͽ��պ�������������ɵĹ������£�
//       1.����ṹ��̬�����������ж���ʹ�ã�
//       2.������صļĴ�����ʹ�䴦�ڷ��ͽ�������״̬;
//       3.�����жϣ����жϷ���������������շ���
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       sendlen,�������ݳ��ȣ��ֽ�
//       recvlen,�������ݳ��ȣ��ֽ�
//       recvoff,����������ϡ�����ӽ��յ��ĵڼ����ֽڿ�ʼ�������ݣ��ֽ�
//       block_opt,����ѡ�trueʱ��Ϊ��������������ʽ��ײ������ͷ��ź���������CS
//       spi_semp,�ײ������Ҫ�ͷŵ��ź������Ƿ��ͷ���block_opt����
// ���أ�true,�޴���;false,ʧ��
// =============================================================================
static bool_t __SPI_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_IntParamSet *Param=NULL;
    u32 datalen;
    if((u32)Reg == CN_SPI_BASE)
        Param = &IntParamset;
    else
        return false;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    datalen = sendlen+recvlen;
    __SPI_TxRxIntDisable(Reg);
    __spi_cs_activate(Reg,datalen); //���ô�������������ʹ��CS

    Reg->event = 0xffffffff;        //��������¼���־
//    while(!(Reg->event & ESPI_EV_TXE));   //�ȴ�����FIFOΪ��
//    while(ESPI_MODE_TXTHR(Reg->event))
//      Reg->tx;
//    while()

    __SPI_TxRxIntEnable(Reg);

    return true;
}
// =============================================================================
// ���ܣ�SPI�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.�����շ�һ�壬��˷��Ϳ��ж���Ҳ�յ������ݣ�
//       2.SPI_PortRead()�ж���������ʱ��˵�����͵������Ѿ���ɣ��ɹط����ж�
//       3.��param->RecvDataLen����0������Ҫ�������ݣ���ӽ��յ���������offset�ֽ�
//         ��ʼ�洢���ݣ�������SPI_PortRead()
//       4.
// ������spi_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
u32 SPI_ISR(ufast_t IntLine)
{
    struct tagSPI_CB *SCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u8 ch[32],num,i,j,*tempdst,*tempdin,store_num;
    u32 event,temp;
    static u8 s_TxNum = 0;

    Reg = (tagSpiReg *)CN_SPI_BASE;
    SCB = &s_SPI_CB;
    param = &IntParamset;

    //P1020��FIFOΪ32�ֽ���ȣ�����FIFO�Ĵ���Ϊ32bit���
    event = 0;
    while(ESPI_EV_RXCNT(event) != s_TxNum)//���ͣ������Ƿ����
    {
        event = Reg->event;
    }
    event = Reg->event;
    if((event & ESPI_EV_RNE) && (param->RecvDataLen))       //��������
    {
        num = ESPI_EV_RXCNT(event);
        if(param->RecvOffset >= num)                //���յ���Ч���ݣ�����洢
        {
            param->RecvOffset -= num;
            while(Reg->event & ESPI_EV_RNE)
                temp = Reg->rx;
            Reg->event |= ESPI_EV_RNE;
        }
        else                                        //��ȥ��Ч���ݣ��洢��Ч����
        {
            tempdst = (u8*)&ch;
            Reg->event |= ESPI_EV_RNE;
            while(param->RecvOffset >4)
            {
                temp = Reg->rx;
                param->RecvOffset -= 4;
                num -= 4;
            }

            num = param->RecvDataLen > num ? num:param->RecvDataLen;
            i = num/4 + (num%4>0 ?1:0);
            for(j=0; j < i;j++)             //�����յ������ݴ洢
            {
                temp = Reg->rx;
                tempdin = (u8*)&temp;
                if(j == 0)                  //�׸���Ч�ֽ�
                {
                    if( num > (4 - param->RecvOffset))
                        store_num = 4 - param->RecvOffset;
                    else
                        store_num = num;
                    param->RecvOffset = 0;
                }
                else if(j == i-1)
                {
                    store_num = num - 4*j;
                }
                else
                {
                    store_num = 4;
                }

                param->RecvDataLen -= store_num;
                while(store_num--)
                    *tempdst++ = *tempdin++;
            }
            SPI_PortWrite(SCB,ch,num);
        }
    }

    if(event & ESPI_EV_TXE)         //���Ϳ��ж�
    {
        Reg->event = ESPI_EV_TXE;
        num = SPI_PortRead(SCB,ch,32);
        if(num > 0)
        {
            i = num/4 + (num%4 > 0 ?1:0);
            for(j = 0; j < i; j ++)
            {
                temp = *(u32*)&ch[j*4];
                Reg->tx = temp;
            }
            param->SendDataLen -= num;
        }
        else if(param->RecvDataLen > 0) //���������ݣ�����Ҫ�ṩ����ʱ��
        {
            temp = 0xaabbcc55;      //����Ч���ݣ��ṩʱ��
            Reg->tx = temp;
            num = param->RecvDataLen >4 ? 4:param->RecvDataLen;
        }
        else if(param->RecvDataLen + param->SendDataLen == 0)
        {
            __SPI_TxRxIntDisable(Reg);
            __spi_cs_deactivate(Reg);
        }
    }
    s_TxNum = num;
    return 0;
}


// =============================================================================
// ���ܣ�SPI�ײ������ĳ�ʼ�����������SPI���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�SPI_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��SPI�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����SPIBusAdd��SPIBusAdd_r�������߽�㣻
// ��������
// ���أ���ʼ���ɹ�����1��ʧ�ܷ���0
// =============================================================================
ptu32_t SPI_ModuleInit(ptu32_t para)
{
    struct tagSPI_Param SPI_Config;
    struct tagSPI_CB *SCB;

    SCB = &s_SPI_CB;
    SPI_Config.BusName          = "SPI0";
    SPI_Config.SPIBuf           = (u8*)&s_SPI_Buf;
    SPI_Config.SPIBufLen        = CN_SPI_BUF_LEN;
    SPI_Config.SpecificFlag     = CN_SPI_BASE;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    __SPI_HardDefaultSet(SPI_Config.SpecificFlag);
    __SPI_IntConfig();

    if(NULL == SPI_BusAdd_s(&SPI_Config,SCB))
        return 0;
    return 1;
}
