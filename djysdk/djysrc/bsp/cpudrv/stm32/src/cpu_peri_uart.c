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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_uart.c
// ģ������: DJYOS����ģ��ĵײ��������֣���Ҫʵ�ּĴ�������Ĳ��������жϵ�
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 14/08.2014
// =============================================================================
#include "config-prj.h"
#include "stdint.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include "uart.h"
#include "int.h"
#include "djyos.h"

extern const char *gc_pCfgStddevName;  //��׼�ն�����

// =============================================================================
#define CN_UART1_BASE 0x40013800
#define CN_UART2_BASE 0x40004400
#define CN_UART3_BASE 0x40004800
#define CN_UART4_BASE 0x40004c00
#define CN_UART5_BASE 0x40005000
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE};
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

#define UART1_SendBufLen            32
#define UART1_RecvBufLen            32
#define UART1_DmaBufLen             32
static u8  UART1_DmaSendBuf[UART1_DmaBufLen];
static u8  UART1_DmaRecvBuf[UART1_DmaBufLen];
static bool_t UART1_FIRST_DMA_SEND;
static bool_t UART1_DMA_SENDING = false;
static bool_t s_UART1_DmaUsed = false;
uint32_t UART1_DMA1CH4_ISR(ufast_t IntLine);
uint32_t UART1_DMA1CH5_ISR(ufast_t IntLine);

#define UART2_SendBufLen            32
#define UART2_RecvBufLen            32
#define UART2_DmaBufLen             32
static u8  UART2_DmaSendBuf[UART2_DmaBufLen];
static u8  UART2_DmaRecvBuf[UART2_DmaBufLen];
static bool_t UART2_FIRST_DMA_SEND;
static bool_t UART2_DMA_SENDING = false;
static bool_t s_UART2_DmaUsed = false;
uint32_t UART2_DMA1CH7_ISR(ufast_t IntLine);
uint32_t UART2_DMA1CH6_ISR(ufast_t IntLine);

#define UART3_SendBufLen            32
#define UART3_RecvBufLen            32
#define UART3_DmaBufLen             32
static u8  UART3_DmaSendBuf[UART3_DmaBufLen];
static u8  UART3_DmaRecvBuf[UART3_DmaBufLen];
static bool_t UART3_FIRST_DMA_SEND;
static bool_t UART3_DMA_SENDING = false;
static bool_t s_UART3_DmaUsed = false;

uint32_t UART3_DMA1CH2_ISR(ufast_t IntLine);
uint32_t UART3_DMA1CH3_ISR(ufast_t IntLine);
#define UART4_SendBufLen            32
#define UART4_RecvBufLen            32

#define UART5_SendBufLen            32
#define UART5_RecvBufLen            32

static struct tagUartCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;

// =============================================================================
static ptu32_t UART_ISR(ufast_t IniLine);

// =============================================================================
// ����: ��ֹuart�ķ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~5
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch5);
        else
            bb_uart1_cr1_rxneie = 1;
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch6);
        else
            bb_uart2_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch3);
        else
            bb_uart3_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA2_Ch3);
        else
            bb_uart4_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch5);
        else
            bb_uart1_cr1_rxneie = 0;
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch6);
        else
            bb_uart2_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch3);
        else
            bb_uart3_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA2_Ch3);
        else
            bb_uart4_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch4);
        else
            bb_uart1_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch7);
        else
            bb_uart2_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch2);
        else
            bb_uart3_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA2_Ch4_5);
        else
            bb_uart4_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch4);
        else
            bb_uart1_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch7);
        else
            bb_uart2_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch2);
        else
            bb_uart3_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA2_Ch4_5);
        else
            bb_uart4_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->SR & (1<<7)));
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 port,u32 baud)
{
    u32 temp1,temp2;
    if((port == CN_UART1) || (port == CN_UART5))
    {
        temp1 = CN_CFG_PCLK2/(16*baud);
        temp2 = CN_CFG_PCLK2%(16*baud);
        temp2 = temp2/baud;
    }
    else
    {
        temp1 = CN_CFG_PCLK1/(16*baud);
        temp2 = CN_CFG_PCLK1%(16*baud);
        temp2 = temp2/baud;
    }
    Reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
}

// =============================================================================
// ����: ���ö�ӦUART��IO�ڣ�����ʱ�Ӻ�IO����
// ����: SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_GpioConfig(u8 SerialNo)
{
    //��ʼ��IO�˿�λuart����
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1 GPA0
    //Signal :   RXD1 TXD1  xx   xx   xx   xx   xx   xx   xx   xx   xx
    switch(SerialNo)
    {
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 1;     //uart1ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_A);
        GPIO_CfgPinFunc(CN_GPIO_A,9,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_A,10,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 1;     //uart2ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_A);
        GPIO_CfgPinFunc(CN_GPIO_A,2,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_A,3,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 1;     //uart3ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_B);
        GPIO_CfgPinFunc(CN_GPIO_B,10,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_B,11,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 1;     //uart4ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_C);
        GPIO_CfgPinFunc(CN_GPIO_C,10,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_C,11,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 1;    //uart5ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_C);
        GPIO_PowerOn(CN_GPIO_D);
        GPIO_CfgPinFunc(CN_GPIO_C,12,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_D,2,CN_GPIO_MODE_IN_FLOATING);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        port,���ں�
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,ptu32_t data)
{
    struct tagCOMParam *COM;
    if((data == 0) || (Reg == NULL))
        return;
    COM = (struct tagCOMParam *)data;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_8:    Reg->CR1 &= ~(1<<12);break;
    case CN_UART_DATABITS_9:    Reg->CR1 |= (1<<12);;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    Reg->CR1 |= (1<<10);                // enable parity
    case CN_UART_PARITY_ODD:    Reg->CR1 |= (1<<9);break;
    case CN_UART_PARITY_EVEN:   Reg->CR1 &=~(1<<9);break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    Reg->CR2 &= ~(3<<12);
    case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
    case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (1<<12);break;
    case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
    default:break;
    }
}

// =============================================================================
// ����: ʹ���ڿ�ʼ������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Enable(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 1;    //uart1ʱ��ʹ��
        bb_uart1_cr1_ue = 1;
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 1;    //uart2ʱ��ʹ��
        bb_uart2_cr1_ue = 1;
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 1;    //uart3ʱ��ʹ��
        bb_uart3_cr1_ue = 1;
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 1;    //uart4ʱ��ʹ��
        bb_uart4_cr1_ue = 1;
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 1;    //uart5ʱ��ʹ��
        bb_uart5_cr1_ue = 1;
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Disable(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 0;    //uart1ʱ��ʹ��
        bb_uart1_cr1_ue = 0;
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 0;    //uart2ʱ��ʹ��
        bb_uart2_cr1_ue = 0;
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 0;    //uart3ʱ��ʹ��
        bb_uart3_cr1_ue = 0;
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 0;    //uart4ʱ��ʹ��
        bb_uart4_cr1_ue = 0;
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 0;    //uart5ʱ��ʹ��
        bb_uart5_cr1_ue = 0;
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_USART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_USART2;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_USART3;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART4;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART5;
    else
        return;

    uint32_t UART_ISR(ufast_t uart_int_line);
    Int_SetClearType(IntLine,CN_INT_CLEAR_PRE);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART5)
        return;
    __UART_GpioConfig(SerialNo);
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
   //��ʼ��uartӲ���������ݽṹ
    tg_UART_Reg[SerialNo]->CR1 = 0x20ac;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    tg_UART_Reg[SerialNo]->CR3 = 0x0000;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
}

// =============================================================================
// ����: ��ѯ�ȴ����ڷ�����ϣ���ʱ����
// ����: sending��dma���ͷ�ʽ�������Ƿ��Ѿ��������
// ����: ��
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    u32 timeout = 1000;
    while((sending == true)&& (timeout > 0))//��ʱ
    {
        timeout--;
        Djy_DelayUs(1);
    }
    if(timeout == 0)
        return true;
    else
        return false;
}
// =============================================================================
// ����: ��������DMA���ͣ�ʹ��DMA�������ڷ���ǰ���ж��ϴ�DMA�Ƿ��Ѿ�������ɣ�ͨ��
//       �鿴��־UARTx_DMA_SENDING���ж�UARTx�Ƿ��Ѿ��������
// ����: PORT,���ں�.
// ����: ���͵ĸ���
// =============================================================================
u32 __UART_DMA_SendStart(u32 port)
{
    u32 num,addr;

    switch(port)
    {
    case CN_UART1:
        if(true == __uart_dma_timeout(UART1_DMA_SENDING))
            break;
        num = UART_PortRead(pUartCB[CN_UART1],UART1_DmaSendBuf,UART1_DmaBufLen,0);
        if(UART1_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART1_DmaSendBuf[0];
            UART1_FIRST_DMA_SEND = false;
            addr = (u32)UART1_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART1_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch4_ccr_en = 0;    //��ͣdma����
            pg_dma1_channel4_reg->CMAR = addr;
            pg_dma1_channel4_reg->CNDTR = num;
            bb_dma1_ch4_ccr_en = 1;    //����dma����
            UART1_DMA_SENDING = true;
        }
        break;
    case 2:
        if(true == __uart_dma_timeout(UART2_DMA_SENDING))
            break;
        num = UART_PortRead(pUartCB[CN_UART2],UART2_DmaSendBuf,UART2_DmaBufLen,0);
        if(UART2_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART2_DmaSendBuf[0];
            UART2_FIRST_DMA_SEND = false;
            addr = (u32)UART2_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART2_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch7_ccr_en = 0;    //��ͣdma����
            pg_dma1_channel7_reg->CMAR = addr;
            pg_dma1_channel7_reg->CNDTR = num;
            bb_dma1_ch7_ccr_en = 1;    //����dma����
            UART2_DMA_SENDING = true;
        }
        break;
    case 3:
        if(true == __uart_dma_timeout(UART3_DMA_SENDING))
            break;
        num = UART_PortRead(pUartCB[CN_UART3],(u8*)UART3_DmaSendBuf,UART3_DmaBufLen,0);
        if(UART3_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART3_DmaSendBuf[0];
            UART3_FIRST_DMA_SEND = false;
            addr = (u32)UART3_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART3_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch2_ccr_en = 0;    //��ͣdma����
            pg_dma1_channel2_reg->CMAR = addr;
            pg_dma1_channel2_reg->CNDTR = num;
            bb_dma1_ch2_ccr_en = 1;    //����dma����
            UART3_DMA_SENDING = true;
        }
        break;
    default:
        break;
    }
    return 0;
}
// =============================================================================
// ����: ֱ�ӷ��ͺ�����ʹ����ѯ��ʽ��
// ����: Reg,�Ĵ�����ַָ��.
//       send_buf,�������ݻ�����
//       len,���ݳ��ȣ��ֽ�
//       timeout,��ʱʱ�䣬us
// ����: �����ֽ���
// =============================================================================
static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
{
    u32  result = 0;

    if(Reg == NULL)
        return result;

    Reg->CR1 &= ~(1<<7);                            //disable send INT
    for(result=0; result < len; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
        {
            timeout--;
            Djy_DelayUs(1);
        }
        if(timeout == 0)
            break;
        Reg->DR = send_buf[result];
    }
    Reg->CR1 |= (1<<7);                             //enable send INT
    return result;
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg,u32 timeout)
{
    u8 port,dmaused;

    switch((u32)Reg)
    {
    case CN_UART1_BASE:
        dmaused = s_UART1_DmaUsed;
        port = CN_UART1;    break;
    case CN_UART2_BASE:
        dmaused = s_UART2_DmaUsed;
        port = CN_UART2;    break;
    case CN_UART3_BASE:
        dmaused = s_UART3_DmaUsed;
        port = CN_UART3;    break;
    case CN_UART4_BASE:
        dmaused = CN_DMA_UNUSED;
        port = CN_UART4;    break;
    case CN_UART5_BASE:
        dmaused = CN_DMA_UNUSED;
        port = CN_UART5;    break;
    default:return 0;
    }

    __UART_TxIntDisable(dmaused,port);
    if(dmaused)
    {
        __UART_DMA_SendStart(port);
    }
    else
    {
        //��DMA��ʽ��ֻҪʹ�ܷ��Ϳ��жϣ��򴥷��ж�
    }

    __UART_TxIntEnable(dmaused,port);
    return 1;
}

// =============================================================================
// ����: ����uartʹ��dma�շ�������stm32�����ڵ��շ�dmaͨ�����üĴ����������³�
//       ʼ������dma�Ĵ�������Ϊ:
//       1����ʹ��dma�ڼ�����������豸ʹ��dma���Ӷ����������á�
//       2����ʹuartʹ��dma�ڼ䣬Ҳ�������ģ����ϵ����⵼�������豸ʹ��dma.
// ����: port,�˿ںţ������ںţ���1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch4_ccr_en = 0;    //ֹͣdmaͨ��
        bb_dma1_ch5_ccr_en = 0;    //ֹͣdmaͨ��
        bb_uart1_cr1_rxneie = 0;
        bb_uart1_cr1_txeie = 0;
        bb_uart1_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x000ff000;        //�������ж�״̬
        pg_dma1_channel4_reg->CCR = 0x00000092; //����dma
        pg_dma1_channel4_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART1]->DR);

        pg_dma1_channel5_reg->CCR = 0x00000086; //����dma
        pg_dma1_channel5_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART1]->DR);
        pg_dma1_channel5_reg->CNDTR = 32;
        pg_dma1_channel5_reg->CMAR = (u32)UART1_DmaRecvBuf;

        Int_SetClearType(CN_INT_LINE_DMA1_Ch4,CN_INT_CLEAR_PRE);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch4,UART1_DMA1CH4_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch5,CN_INT_CLEAR_PRE);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch5,UART1_DMA1CH5_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch4);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch5);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch4);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch5);

        tg_UART_Reg[CN_UART1]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
        s_UART1_DmaUsed = CN_DMA_USED;
        bb_dma1_ch5_ccr_en = 1;    //����dmaͨ��
        UART1_FIRST_DMA_SEND = true;
        break;
    case CN_UART2:
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch7_ccr_en = 0;    //ֹͣdmaͨ��
        bb_dma1_ch6_ccr_en = 0;    //ֹͣdmaͨ��
        bb_uart2_cr1_rxneie = 0;
        bb_uart2_cr1_txeie = 0;
        bb_uart2_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x0ff00000;        //�������ж�״̬
        pg_dma1_channel7_reg->CCR = 0x00000092; //����dma
        pg_dma1_channel7_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART2]->DR);

        pg_dma1_channel6_reg->CCR = 0x00000086; //����dma
        pg_dma1_channel6_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART2]->DR);
        pg_dma1_channel6_reg->CNDTR = 32;
        pg_dma1_channel6_reg->CMAR = (u32)UART2_DmaRecvBuf;

        Int_SetClearType(CN_INT_LINE_DMA1_Ch6,CN_INT_CLEAR_PRE);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch6,UART2_DMA1CH6_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch7,CN_INT_CLEAR_PRE);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch7,UART2_DMA1CH7_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch6);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch7);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch6);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch7);

        tg_UART_Reg[CN_UART2]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
        s_UART2_DmaUsed = CN_DMA_USED;
        bb_dma1_ch6_ccr_en = 1;    //����dmaͨ��
        UART2_FIRST_DMA_SEND = true;
        break;
    case CN_UART3:
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch2_ccr_en = 0;    //ֹͣdmaͨ��
        bb_dma1_ch3_ccr_en = 0;    //ֹͣdmaͨ��
        bb_uart3_cr1_rxneie = 0;
        bb_uart3_cr1_txeie = 0;
        bb_uart3_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x000ff000;        //�������ж�״̬
        pg_dma1_channel2_reg->CCR = 0x00000092; //����dma
        pg_dma1_channel2_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART3]->DR);

        pg_dma1_channel3_reg->CCR = 0x00000086; //����dma
        pg_dma1_channel3_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART3]->DR);
        pg_dma1_channel3_reg->CNDTR = 32;
        pg_dma1_channel3_reg->CMAR = (u32)UART3_DmaRecvBuf;

        Int_SetClearType(CN_INT_LINE_DMA1_Ch2,CN_INT_CLEAR_PRE);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch2,UART3_DMA1CH2_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch3,CN_INT_CLEAR_PRE);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch3,UART3_DMA1CH3_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch2);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch3);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch2);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch3);

        tg_UART_Reg[CN_UART3]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
        s_UART3_DmaUsed = CN_DMA_USED;
        bb_dma1_ch3_ccr_en = 1;    //����dmaͨ��
        UART3_FIRST_DMA_SEND = true;
        break;
    default:
        break;
    }

}

//=============================================================================
// ����: ���ò�ʹ��DMA��UART��ʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ������ںţ�Ϊ1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        tg_UART_Reg[CN_UART1]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
        Int_RestoreAsynLine(CN_INT_LINE_USART1);
        if(s_UART1_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch4);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch5);
            s_UART1_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart1_cr1_rxneie = 1;
        bb_uart1_cr1_txeie = 1;
        bb_uart1_cr1_idleie = 0;
        break;
    case 2:
        tg_UART_Reg[CN_UART2]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
        Int_RestoreAsynLine(CN_INT_LINE_USART2);
        if(s_UART2_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch6);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch7);
            s_UART2_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart2_cr1_rxneie = 1;
        bb_uart2_cr1_txeie = 1;
        bb_uart2_cr1_idleie = 0;
        break;
    case 3:
        tg_UART_Reg[CN_UART3]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
        Int_RestoreAsynLine(CN_INT_LINE_USART3);
        if(s_UART3_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch2);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch3);
            s_UART3_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart3_cr1_rxneie = 1;
        bb_uart3_cr1_txeie = 1;
        bb_uart3_cr1_idleie = 0;
        break;
    default:
        break;
    }
}
// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, u32 data1,u32 data2)
{
    ptu32_t result = 0;
    u32 port;
    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
    case CN_UART1_BASE: port = CN_UART1;break;
    case CN_UART2_BASE: port = CN_UART2;break;
    case CN_UART3_BASE: port = CN_UART3;break;
    case CN_UART4_BASE: port = CN_UART4;break;
    case CN_UART5_BASE: port = CN_UART5;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_UART_START:
            __UART_Enable(port);
            break;
        case CN_UART_STOP:
            __UART_Disable(port);
            break;
        case CN_UART_SET_BAUD:  //����Baud
             __UART_BaudSet(Reg,port, data1);
            break;
        case CN_UART_EN_RTS:
            Reg->CR3 |= 0x100;
            break;
        case CN_UART_DIS_RTS:
            Reg->CR3 &= ~0x100;
            break;
        case CN_UART_EN_CTS:
            Reg->CR3 |= 0x200;
            break;
        case CN_UART_DIS_CTS:
            Reg->CR3 &= ~0x200;
            break;
        case CN_UART_DMA_USED:
            __UART_SetDmaUsed(port);
            break;
        case CN_UART_DMA_UNUSED:
            __UART_SetDmaUnUsed(port);
            break;
        case CN_UART_COM_SET:
            __UART_ComConfig(Reg,port,data1);
            break;
        default: break;
    }
    return result;
}

// =============================================================================
// ����: UART1��DMA�жϣ���uart1���ó�dma��ʽʱ��dma1 ch4�����ͣ�ch5�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART1_DMA1CH4_ISR(ufast_t tagIntLine)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x0000f000;
    num = UART_PortRead(pUartCB[CN_UART1],UART1_DmaSendBuf,UART1_DmaBufLen,0);
    if(num == 0)
        UART1_DMA_SENDING = false;
    else
    {
        bb_dma1_ch4_ccr_en = 0;    //��ͣdma����
        pg_dma1_channel4_reg->CMAR = (u32)UART1_DmaSendBuf;
        pg_dma1_channel4_reg->CNDTR = num;
        bb_dma1_ch4_ccr_en = 1;    //����dma����
    }
    return 0;
}

// =============================================================================
// ����: UART1��DMA�жϣ���uart1���ó�dma��ʽʱ��dma1 ch4�����ͣ�ch5�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART1_DMA1CH5_ISR(ufast_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x000f0000;
    bb_dma1_ch5_ccr_en = 0;    //��ͣdma����
    recvs = UART1_DmaBufLen - pg_dma1_channel5_reg->CNDTR;
    if(recvs == UART1_DmaBufLen)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[CN_UART1],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART1],UART1_DmaRecvBuf,recvs,0);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART1],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel5_reg->CNDTR = UART1_DmaBufLen;
    pg_dma1_channel5_reg->CMAR = (u32)UART1_DmaRecvBuf;
    bb_dma1_ch5_ccr_en = 1;    //����dma����
    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART2_DMA1CH7_ISR(ufast_t tagIntLine)
{
    uint16_t num;

    pg_dma1_reg->IFCR |= 0x0f000000;
    num = UART_PortRead(pUartCB[CN_UART2],UART2_DmaSendBuf,UART2_DmaBufLen,0);
    if(num == 0)
        UART2_DMA_SENDING = false;
    else
    {
        bb_dma1_ch7_ccr_en = 0;    //��ͣdma����
        pg_dma1_channel7_reg->CMAR = (u32)UART2_DmaSendBuf;
        pg_dma1_channel7_reg->CNDTR = num;
        bb_dma1_ch7_ccr_en = 1;    //����dma����
    }
    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART2_DMA1CH6_ISR(ufast_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x00f00000;
    bb_dma1_ch6_ccr_en = 0;    //��ͣdma����
    recvs = UART2_DmaBufLen - pg_dma1_channel6_reg->CNDTR;
    if(recvs == UART2_DmaBufLen)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[CN_UART2],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART2],UART2_DmaRecvBuf,recvs,0);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART2],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel6_reg->CNDTR = UART2_DmaBufLen;
    pg_dma1_channel6_reg->CMAR = (u32)UART2_DmaRecvBuf;
    bb_dma1_ch6_ccr_en = 1;    //����dma����
    return 0;
}

// =============================================================================
// ����: ��uart3���ó�dma��ʽ�ǣ�dma1 ch2�����ͣ�ch3�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART3_DMA1CH2_ISR(ufast_t tagIntLine)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x000000f0;
    num = UART_PortRead(pUartCB[CN_UART3],UART3_DmaSendBuf,UART3_DmaBufLen,0);
    if(num == 0)
        UART3_DMA_SENDING = false;
    else
    {
        bb_dma1_ch2_ccr_en = 0;    //��ͣdma����
        pg_dma1_channel2_reg->CMAR = (u32)UART3_DmaSendBuf;
        pg_dma1_channel2_reg->CNDTR = num;
        bb_dma1_ch2_ccr_en = 1;    //����dma����
    }
    return 0;
}

// =============================================================================
//����: ��uart3���ó�dma��ʽ�ǣ�dma1 ch2�����ͣ�ch3�������
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART3_DMA1CH3_ISR(ufast_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x00000f00;
    bb_dma1_ch3_ccr_en = 0;    //��ͣdma����
    recvs = UART3_DmaBufLen - pg_dma1_channel3_reg->CNDTR;
    if(recvs == UART3_DmaBufLen)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[CN_UART3],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART3],UART3_DmaRecvBuf,recvs,0);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART3],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel3_reg->CNDTR = UART3_DmaBufLen;
    pg_dma1_channel3_reg->CMAR = (u32)UART3_DmaRecvBuf;
    bb_dma1_ch3_ccr_en = 1;    //����dma����
    return 0;
}

// =============================================================================
// ����: UART���жϷ��������������Ϳա����ա����������жϡ�
//      1.�����жϣ��ӽ���FIFO�ж����ݣ�������UART_PortWriteд����ջ������У�
//      2.�����жϣ�����UART_PortRead�����ͻ����������ݣ���д�뷢��FIFO��
//      3.�����жϣ����������¼�
// ����: �жϺ�.
// ����: 0.
// =============================================================================
u32 UART_ISR(ufast_t IntLine)
{
    struct tagUartCB *UCB;
    tagUartReg *Reg;
    u32 num,port;
    u8 ch;

    switch(IntLine)
    {
    case CN_INT_LINE_USART1:   port = CN_UART1;  break;
    case CN_INT_LINE_USART2:   port = CN_UART2;  break;
    case CN_INT_LINE_USART3:   port = CN_UART3;  break;
    case CN_INT_LINE_UART4:    port = CN_UART4;   break;
    case CN_INT_LINE_UART5:    port = CN_UART5;   break;
    default:return 0;
    }

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if(Reg->SR & (1<<5))            //�����ж�
    {
        ch = (u8)Reg->DR;
        num = UART_PortWrite(UCB,&ch,1,0);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->SR & (1<<6)) &&(Reg->CR1 & (1<<7)))    //�����ж�
    {
        num = UART_PortRead(UCB,&ch,1,0);
        if(num != 0)
            Reg->DR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);        //txeie
        }
    }
    if(Reg->SR & (1<<3))        //ORE���ش���
    {
        ch = (u8)Reg->DR;
        num = Reg->SR;     //��һ��sr�Ĵ���
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
    }
    if(Reg->SR & (1<<4))
    {
        Reg->DR;
        Int_TapLine(CN_INT_LINE_DMA1_Ch5);
    }
    return 0;
}

// =============================================================================
// ���ܣ���ʼ��uart1ģ��,��װ�ش����豸�������uart.c��API�������ú�����Ҫ��ɵĹ���
//       ���£�
//       1.��ʼ�����ڵ�Ӳ�����֣���GPIO���жϡ��������õ�COM����������baud = 115200
//         stop bits = 1,����żУ��, data bits = 8
//       2.����uart.c��API����UART_InstallPort��װ�봮���豸
// ���������ںţ�0~3
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
ptu32_t ModuleInstall_UART(u32 serial_no)
{
    struct tagUartParam UART_Param;

    switch(serial_no)
    {
    case CN_UART1://����0
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = UART1_SendBufLen;
        UART_Param.RxRingBufLen = UART1_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://����2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = UART2_SendBufLen;
        UART_Param.RxRingBufLen = UART2_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
        break;
    case CN_UART3://����3
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = UART3_SendBufLen;
        UART_Param.RxRingBufLen = UART3_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
        break;
    case CN_UART4://����4
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = UART4_SendBufLen;
        UART_Param.RxRingBufLen = UART4_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART5://����5
        UART_Param.Name         = "UART5";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART5_BASE;
        UART_Param.TxRingBufLen = UART5_SendBufLen;
        UART_Param.RxRingBufLen = UART5_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }

    //Ӳ����ʼ��
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);

    sUartInited |= (0x01 << serial_no);
    pUartCB[serial_no] = UART_InstallPort(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӷ��ͺ�����������ѯ��ʽ��ֱ��д�Ĵ�����������djy_printk�ȵ��Ի���
// �����������ַ���ָ��
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
u32 Uart_SendServiceDirectly(char *str)
{
    u32 result = 0,len,timeout = 100*mS;
    tagUartReg *Reg;

    if(!strcmp(gc_pCfgStddevName,"UART1")&& (sUartInited & (0x01 << CN_UART1)))
        Reg = (tagUartReg*)CN_UART1_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART2")&& (sUartInited & (0x01 << CN_UART2)))
        Reg = (tagUartReg*)CN_UART2_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART3")&& (sUartInited & (0x01 << CN_UART3)))
        Reg = (tagUartReg*)CN_UART3_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART4")&& (sUartInited & (0x01 << CN_UART4)))
        Reg = (tagUartReg*)CN_UART4_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART5")&& (sUartInited & (0x01 << CN_UART5)))
        Reg = (tagUartReg*)CN_UART5_BASE;
    else
        return 0;
    len = strlen(str);

    Reg->CR1 &= ~(1<<7);                            //disable send INT
    for(result=0; result < len; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
        {
            timeout--;
            Djy_DelayUs(1);
        }
        if(timeout == 0)
            break;
        Reg->DR = str[result];
    }
    Reg->CR1 |= (1<<7);                         //enable send INT
    return result;
}
