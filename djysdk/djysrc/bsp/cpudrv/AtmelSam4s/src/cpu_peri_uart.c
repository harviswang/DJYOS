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
// ģ������: UARTģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 05/15.2014
// =============================================================================

#include "cpu_peri.h"
#include "uart.h"
#include "string.h"

// =============================================================================
//base address
#define CN_UART0_BASE   (0x400E0600)
#define CN_UART1_BASE   (0x400E0800)
#define CN_USART0_BASE  (0x40024000)
#define CN_USART1_BASE  (0x40028000)
//==============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
													(tagUartReg *)CN_UART1_BASE,
													(tagUartReg *)CN_USART0_BASE,
													(tagUartReg *)CN_USART1_BASE};
bool_t s_UART_DmaUsed[4]={false,false,false,false};



#define UART0_SendBufLen  256
#define UART0_RecvBufLen  256
#define UART0_DMA_BUF_LEN  64
static u32 s_UART0_DmaRcvLen = 1;
uint8_t UART0_DmaSendBuf[UART0_DMA_BUF_LEN];
uint8_t UART0_DmaRecvBuf[UART0_DMA_BUF_LEN];


#define UART1_SendBufLen  256
#define UART1_RecvBufLen  256
#define UART1_DMA_BUF_LEN  64
static u32 s_UART1_DmaRcvLen = 1;
uint8_t UART1_DmaSendBuf[UART1_DMA_BUF_LEN];
uint8_t UART1_DmaRecvBuf[UART1_DMA_BUF_LEN];

#define USART0_SendBufLen  256
#define USART0_RecvBufLen  256
#define USART0_DMA_BUF_LEN  64
static u32 s_USART0_DmaRcvLen = 1;
uint8_t USART0_DmaSendBuf[USART0_DMA_BUF_LEN];
uint8_t USART0_DmaRecvBuf[USART0_DMA_BUF_LEN];

#define USART1_SendBufLen  256
#define USART1_RecvBufLen  256
#define USART1_DMA_BUF_LEN  64
static u32 s_USART1_DmaRcvLen = 1;
uint8_t USART1_DmaSendBuf[USART1_DMA_BUF_LEN];
uint8_t USART1_DmaRecvBuf[USART1_DMA_BUF_LEN];

//�������Ŷ���
static const Pin uart0_pin[] = {
    {PIO_PA9A_URXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PA10A_UTXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}};
static const Pin uart1_485_2[] = {
    {PIO_PB2A_URXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PB3A_UTXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}};
static const Pin usart0_485_1[] = {
    {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}};

ptu32_t UART_ISR(ufast_t IntLine);

static struct tagUartCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;

extern const char *gc_pCfgStddevName;  //��׼�ն�����
//----ʹ�ܽ����ж�-------------------------------------------------------------
//����: ʹ��uart�Ľ����ж�,DMA�ж�ʹ�õ���endrx
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_RecvIntEnable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IER = (1<<0);//rxrdy int
    else
        reg->UART_IER = (1<<3);//endrx int
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�Ľ����ж�,DMA�ж�ʹ�õ���endrx
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_RecvIntDisable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IDR = (1<<0);//rxrdy int
    else
        reg->UART_IDR = (1<<3);//endrx int
}

//----ʹ�ܷ����ж�-------------------------------------------------------------
//����: ʹ��uart�ķ����ж�,DMA�ж�ʹ�õ���endtx �ж�
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_SendIntEnable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IER = (1<<9);//tx empty int
    else
        reg->UART_IER = (1<<11);//txbufe int
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�ķ����ж�,DMA�ж�ʹ�õ���endtx �ж�
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_SendIntDisable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IDR = (1<<9);//tx empty int
    else
        reg->UART_IDR = (1<<11);//txbufe int
}

//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ�����λ�Ĵ�����thr��Ϊ��
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *reg)
{
    return (0 != (reg->UART_SR & (1<<9)));//TXR EMPTY
}
//----����baud-----------------------------------------------------------------
//����: ���ô���baud
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_SetBaud(tagUartReg volatile *reg,u32 baud)
{
    reg->UART_BRGR = (CN_CFG_MCLK/baud)/16;
}


//----��ʼ��uart��Ӧ��io��-----------------------------------------------------
//����: ��ʼ��uart��Ӧ��io��Ϊ��������
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
void __UART_GpioInit(u32 SerialNo)
{
    if(SerialNo == CN_UART0)
    {
        //UART0����ͨ�����ſ����շ�
    	PIO_Configure(uart0_pin, PIO_LISTSIZE(uart0_pin));
    }
    else if(SerialNo == CN_UART1)
    {
        //485���շ��Ϳ���������
    	PIO_Configure(uart1_485_2, PIO_LISTSIZE(uart1_485_2));
    }
    else if(SerialNo == CN_USART0)
    {
        //485���շ��Ϳ���������
    	PIO_Configure(usart0_485_1, PIO_LISTSIZE(usart0_485_1));
    }
    else if(SerialNo == CN_USART1)
    {
    	//
    }
}

//----��ʼ��uart��Ӧ���ж�-----------------------------------------------------
//����: ��ʼ��uart��Ӧ���ж��ߣ�����ʼ���ж���ں���
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
void __UART_IntInit(u32 SerialNo)
{
    uint32_t int_line;
    if(SerialNo == CN_UART0)
        int_line = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        int_line = CN_INT_LINE_UART1;
    else if(SerialNo == CN_USART0)
        int_line = CN_INT_LINE_USART0;
    else
    	int_line = CN_INT_LINE_USART1;

    Int_SetClearType(int_line,CN_INT_CLEAR_PRE);
    Int_IsrConnect(int_line,UART_ISR);
    Int_SettoAsynSignal(int_line);
    Int_ClearLine(int_line);
    Int_RestoreAsynLine(int_line);
}

//----��ʼ��uartʱ��ʹ��-----------------------------------------------------
//����: ʹ����Ӧuart�����ʱ��
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
void __UART_PmcInit(u32 SerialNo)
{
    if(SerialNo == CN_UART0)
        PMC_EnablePeripheral(CN_PERI_ID_UART0);
    else if(SerialNo == CN_UART1)
        PMC_EnablePeripheral(CN_PERI_ID_UART1);
    else if(SerialNo == CN_USART0)
        PMC_EnablePeripheral(CN_PERI_ID_USART0);
    else
    	PMC_EnablePeripheral(CN_PERI_ID_USART1);
}

//----��ʼ��uart��˫��ģʽ-----------------------------------------------------
//����: ����uart��˫��ģʽΪ���ͻ���գ������������ʱһ��ʱ�����IO
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
void __UART_half_duplex_send(u32 SerialNo)
{
//    if(SerialNo == CN_UART1)
//        GPIO_SettoHigh(&uart1_485_2[0]);
//    else if(SerialNo == CN_USART0)
//        GPIO_SettoHigh(&usart0_485_1[0]);
//    else
//        ;
    //MAX485оƬ���ؿ��ƽ��պͷ���ʱ��
    Djy_DelayUs(12);
}

// =============================================================================
// ����: ����uart��˫��ģʽΪ���ͻ����
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
void __UART_half_duplex_recv(u32 SerialNo)
{
//    if(SerialNo == CN_UART1)
//        GPIO_SettoLow(&uart1_485_2[0]);
//    else if(SerialNo == CN_USART0)
//        GPIO_SettoLow(&usart0_485_1[0]);
//    else
//        ;
    //MAX485оƬ���ؿ��ƽ��պͷ���ʱ��
    Djy_DelayUs(12);
}

// =============================================================================
// ����: ���ô��ڲ���DMA��ʽʱ���������ã����� DMA���մ�С��ָ��
// ������reg,�Ĵ���ָ��
//      SerialNo,���ں�
//      DataLen,���ݴ�С
// ����: ��
// =============================================================================
void __UART_dma_recv_config(tagUartReg* reg,u32 SerialNo,u32 DataLen)
{
    u32 length;
    switch(SerialNo)
    {

    case CN_UART0:
    	length = UART0_DMA_BUF_LEN;
    	length = (DataLen/length)?length:DataLen%length;
    	s_UART0_DmaRcvLen = DataLen;				//ʣ����
    	reg->UART_RCR = length;
    	reg->UART_RPR = (vu32)UART0_DmaRecvBuf;
    	break;

	case CN_UART1:
    	length = UART1_DMA_BUF_LEN;
    	length = (DataLen/length)?length:DataLen%length;
    	s_UART1_DmaRcvLen = DataLen;				//ʣ����
    	reg->UART_RCR = length;
		reg->UART_RPR = (vu32)UART1_DmaRecvBuf;
		break;

	case CN_USART0:
    	length = USART0_DMA_BUF_LEN;
    	length = (DataLen/length)?length:DataLen%length;
    	s_USART0_DmaRcvLen = DataLen;				//ʣ����
    	reg->UART_RCR = length;
		reg->UART_RPR = (vu32)USART0_DmaRecvBuf;
		break;

	case CN_USART1:
    	length = USART1_DMA_BUF_LEN;
    	length = (DataLen/length)?length:DataLen%length;
    	s_USART1_DmaRcvLen = DataLen;				//ʣ����
    	reg->UART_RCR = length;
		reg->UART_RPR = (vu32)USART1_DmaRecvBuf;
		break;
	default:
		break;
    }
}

// =============================================================================
// ����: ����uartΪDMA��ʽ������Ϊ��DMA��ʽʱ�����ø��Ĵ�����
// ������reg,���ڼĴ���ָ��
//       cmd,�����
//       Port,�˿ںţ������ں�
// ����: ��
// =============================================================================
void __UART_DMA_Config(tagUartReg* Reg,u32 cmd,u32 Port)
{
    //��������صļĴ���reset�����ܽ��պͷ���
    Reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                (1<<7)|(1<<8));         //���շ��Ͷ�������

    if(cmd == CN_UART_DMA_UNUSED)//��ʹ��DMA
    {
    	s_UART_DmaUsed[Port] = false;

         //�رշ�dmaʱʹ�õ��жϣ���ʼdmaʹ�õ�uart�����ж�
         //�����ж�ֻ����������ʱ����
        __UART_RecvIntDisable(Reg, cn_dma_used);
        __UART_SendIntDisable(Reg, cn_dma_used);
        __UART_RecvIntEnable(Reg, cn_dma_unused);
        Reg->UART_PTCR = ((1<<1)|(1<<9));//disable DMA rx tx
        Reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    }
    else    //ʹ��DMA
    {
    	s_UART_DmaUsed[Port] = true;

        //�رշ�dmaʱʹ�õ��жϣ���ʼdmaʹ�õ�uart�����ж�
        __UART_RecvIntEnable(Reg, cn_dma_used);
        __UART_RecvIntDisable(Reg, cn_dma_unused);
        __UART_SendIntDisable(Reg, cn_dma_unused);

        //����dma����
		Reg->UART_TCR   = 0;
		Reg->UART_TNCR  = 0;
        Reg->UART_RCR   = 0;
		Reg->UART_RNCR  = 0;

        __UART_dma_recv_config(Reg,Port,1);

        Reg->UART_PTCR = ((1<<0)|(1<<8));//enable DMA rx tx
        Reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    }
}

// =============================================================================
// ����: ����Ӳ�����������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//      ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
void __UART_HardInit(u8 SerialNo)
{
	tagUartReg *reg;
	if(SerialNo > CN_UART_MAX)
		return;

	__UART_PmcInit(SerialNo);
	__UART_GpioInit(SerialNo);

	reg = (tagUartReg *)tg_UART_Reg[SerialNo];
    __UART_SetBaud(reg,115200);            //����������
    reg->UART_MR = 0x000;                  //żУ�飬����ģʽ
    reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                    (1<<7)|(1<<8));         //���շ��Ͷ�������

    reg->UART_MR = 0x00C0;//ֹͣλ:1 ��żУ��:ż
    reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
}

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,UART�ļĴ�����ַ.
//       cmd,��������
//       data,������cmd����
// ����: ������.
// =============================================================================
ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, u32 data1,u32 data2)
{
	u8 Port;
    switch((u32)Reg)
    {
    case CN_UART0_BASE: Port = CN_UART0;break;
    case CN_UART1_BASE: Port = CN_UART1;break;
    case CN_USART0_BASE: Port = CN_USART0;break;
    case CN_USART1_BASE: Port = CN_USART1;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_UART_START:
            __UART_RecvIntEnable(Reg,s_UART_DmaUsed[Port]);
            __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
            break;
        case CN_UART_STOP:
            __UART_RecvIntDisable(Reg,s_UART_DmaUsed[Port]);
            __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            break;
        case CN_UART_SET_BAUD:  //����Baud
                __UART_SetBaud(Reg,data1);
            break;
        case CN_UART_RX_PAUSE:      //��ͣ����
            __UART_RecvIntEnable(Reg,s_UART_DmaUsed[Port]);
            break;
        case CN_UART_RX_RESUME:      //�ָ�����
            __UART_RecvIntDisable(Reg,s_UART_DmaUsed[Port]);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //��ΪUARTû��FIFO���������DMA����
        	__UART_dma_recv_config(Reg,Port,data1);
            break;
        case CN_UART_HALF_DUPLEX_SEND:
            __UART_half_duplex_send(Port);
            break;
        case CN_UART_HALF_DUPLEX_RECV:
            __UART_half_duplex_recv(Port);
            break;
        case CN_UART_DMA_USED:
        case CN_UART_DMA_UNUSED:
            __UART_DMA_Config(Reg,cmd,Port);
            break;
        default: break;
    }
    return 0;
}

// =============================================================================
// ����: �����ֱ��д���ں��������ᾭ���¼�����
// ����: Reg,UART�ļĴ�����ַ.
//      send_buf,�����͵Ļ�������
//      len,���͵������ֽ���
//      timeout,��ʱʱ�䣬΢��
// ����: ���͵ĸ���
// =============================================================================
u32 __UART_SendDirectly(tagUartReg* Reg,u8 *send_buf,u32 len,u32 timeout)
{
    u32  result,Port;
    switch((u32)Reg)
    {
    //UART0��UART1��FIFO���Ϊ8����������Ϊ1
    case CN_UART0_BASE:Port = CN_UART0; break;
    case CN_UART1_BASE:Port = CN_UART1;break;
    case CN_USART0_BASE:Port = CN_USART0;break;
    case CN_USART1_BASE:Port = CN_USART1; break;
    default:return 0;
    }

    __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
//    __UART_half_duplex_send(Port);//Ӳ��ʹ�ܷ���

    if(s_UART_DmaUsed[Port] == false)
    {
        for(result=0; result < len; result ++)
        {
            while((0 == __UART_TxTranEmpty(Reg))
                && (timeout > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
            {
                timeout--;
                Djy_DelayUs(1);
            }
            if(timeout == 0)
                break;
            Reg->UART_THR = send_buf[result];
        }

//        //�ȴ��������ٽ�485ͨ��תΪ����
//        while((0 == __UART_TxTranEmpty(Reg))
//                && (timeout > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
//        {
//            timeout--;
//            Djy_DelayUs(1);
//        }
//        if(timeout == 0)
//            result = 0;
    }
    else
    {
        Reg->UART_PTCR = (1<<9);//disable dma send first
    	if((Reg->UART_TCR==0)&&(Reg->UART_TNCR==0))
    	{
    		Reg->UART_TPR  = (uint32_t)send_buf;
    		Reg->UART_TCR  = len;
    		Reg->UART_PTCR = (1<<8);//dma tx enbale
    	}
    	else
            result = 0;
        //ֱ�ӷ��ͷ�ʽ������������DMA����
        while((!__UART_TxTranEmpty(Reg)) && (timeout > 0))
        {
            timeout--;
            Djy_DelayUs(1);
        }
        if(timeout == 0)
            result = 0;
    }
    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
//    __UART_half_duplex_recv(Port);//Ӳ��ʹ�ܽ���
    return result;
}

// =============================================================================
// ����: �������ڷ��ͣ�ֱ�ӷ���FIFO��С�����ݣ����������Ϳ��жϣ����ж��н��������е�
//      ��������������ݵķ��ͣ��൱��������һ�η��͹��ܣ���ΪDMA��ʽ�ͷ�DMA��ʽ
// ����: Reg,�������Ĵ��ڼĴ�����ַ
//       timeout,��ʱʱ�䣬΢��
// ����: ���͵��ֽ���
// =============================================================================
u32 __UART_SendStart(tagUartReg *Reg,u32 timeout)
{
    u8 Port;
    struct tagUartCB *UCB;

    switch((u32)Reg)
    {
    //UART0��UART1��FIFO���Ϊ8����������Ϊ1
    case CN_UART0_BASE:UCB = pUartCB[CN_UART0];  Port = CN_UART0; break;
    case CN_UART1_BASE:UCB = pUartCB[CN_UART1];Port = CN_UART1;break;
    case CN_USART0_BASE:UCB = pUartCB[CN_USART0];Port = CN_USART0;break;
    case CN_USART1_BASE:UCB = pUartCB[CN_USART1];Port = CN_USART1; break;
    default:return 0;
    }

    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
    return 1;
}

// =============================================================================
// ����: ʹ���жϷ�ʽЧ���ܹ�����������ִ��Ч�ʣ��������ա����ͺ��쳣�ȣ����յ�����
//       �󣬵���ͨ�ýӿ�ģ�飬д���������������������ʱ������ͨ�ýӿ�ģ�飬�������
//       �����������ݣ�д��Ӳ�����ͼĴ��������⣬����оƬ����ͨ��DMA��ʽ����
// ����: �жϺ�.
// ����: 0.
// =============================================================================
uint32_t UART_ISR(ufast_t IntLine)
{
    static struct tagUartCB *UCB;
    tagUartReg *Reg;
    uint32_t timeout = 1000,num;
    uint8_t ch,*puart_dma_send_buf,*puart_dma_recv_buf;
    uint32_t IIR=0,Port,DmaBufLen,DmaRcvLen;

    switch(IntLine)
    {
        case CN_INT_LINE_UART0:
        	Port = CN_UART0;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART0_BASE;
            DmaBufLen = UART0_DMA_BUF_LEN;
            DmaRcvLen = s_UART0_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)UART0_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)UART0_DmaRecvBuf;
            break;

        case CN_INT_LINE_UART1:
        	Port = CN_UART1;
        	UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = UART1_DMA_BUF_LEN;
            DmaRcvLen = &s_UART1_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)UART1_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)UART1_DmaRecvBuf;
            break;

        case CN_INT_LINE_USART0:
        	Port = CN_USART0;
        	UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = USART0_DMA_BUF_LEN;
            DmaRcvLen = &s_USART0_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)USART0_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)USART0_DmaRecvBuf;
            break;

        case CN_INT_LINE_USART1:
        	Port = CN_USART1;
        	UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = USART1_DMA_BUF_LEN;
            DmaRcvLen = &s_USART1_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)USART1_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)USART1_DmaRecvBuf;
            break;
        default:
            return 0;
    }

    IIR = Reg->UART_SR;

    if(s_UART_DmaUsed[Port] == cn_dma_unused)//��DMA��ʽ���ͺͽ���
    {
        if((IIR & (1<<0)) && (Reg->UART_IMR &(1<<0)))//rxrdy int
        {
            ch = Reg->UART_RHR;
            num = UART_PortWrite(UCB,&ch,1,0);
            if(num != 1)
            {
                UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
            }
        }
        //tx empty int
        if((IIR &(1<<9)) && (Reg->UART_IMR &(1<<9)))
        {
            num = UART_PortRead(UCB,&ch,1,0);
            while((!__UART_TxTranEmpty(Reg)) && (timeout-- > 0));
            if(num != 0)
                Reg->UART_THR = ch;
            else
            {
                __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            }
        }
    }
    else            //DMA��ʽ���ͺͽ���
    {
        if((IIR & (1<<3)) && (Reg->UART_IMR &(1<<3)))//endrx int
        {
        	if(DmaRcvLen > DmaBufLen)					//�����DMA BUF�ж���������
        		num = DmaBufLen;
        	else
        		num = DmaRcvLen;
            UART_PortWrite(UCB,puart_dma_recv_buf,num,0);

            DmaRcvLen = DmaRcvLen - num;
            if(DmaRcvLen > DmaBufLen)					//�����´�DMA����������
            	Reg->UART_RCR  = DmaBufLen;
            else if(DmaRcvLen > 1)
            	Reg->UART_RCR = DmaRcvLen;
            else										//����DMA����Ϊ1
            {
            	Reg->UART_RCR = 1;
            	DmaRcvLen = 1;
            }

            Reg->UART_RPR = (vu32)puart_dma_recv_buf;
        }
        if((IIR & (1<<11)) && (Reg->UART_IMR &(1<<11)))//txbufe int
        {
        	while(!(Reg->UART_SR & (1<<9)));//wait for empty

            num = UART_PortRead(UCB,puart_dma_send_buf,	DmaBufLen,0);
            if(num > 0)
            {
                Reg->UART_PTCR = (1<<9);//diable dma tx
            	if((Reg->UART_TCR==0)&&(Reg->UART_TNCR==0))
            	{
//                    num = UART_PortRead(UCB,puart_dma_send_buf,
//                    		DmaBufLen,0);
            		Reg->UART_TPR  = (uint32_t)puart_dma_send_buf;
            		Reg->UART_TCR  = num;
            	}
                Reg->UART_PTCR = (1<<8);//enable dma tx
            }
            else
            {
                //�Ƿ����ֹ dma tx int
                while(!__UART_TxTranEmpty(Reg));
                __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            }
        }
    }

    if(IIR &(0xE0)) //�����ж�
    {
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }

    return 0;
}

// =============================================================================
// ���ܣ���ʼ������ģ�飬ʹ��DJYOS�Ĵ��ڱ�׼ģ�ͣ�����Ӳ�����жϺʹ����豸����
// ���������ڳ�ʼ��ģ��Ĳ���Ϊ���ںţ���UART0
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
ptu32_t UART_ModuleInit(ptu32_t SerialNo)
{
    struct tagUartParam UART_Param;

    switch(SerialNo)
    {
    case CN_UART0://����1
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = UART0_SendBufLen;
        UART_Param.RxRingBufLen = UART0_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
		break;
    case CN_UART1://����2
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = UART1_SendBufLen;
        UART_Param.RxRingBufLen = UART1_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_USART0://����1
        UART_Param.Name         = "USART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_USART0_BASE;
        UART_Param.TxRingBufLen = USART0_SendBufLen;
        UART_Param.RxRingBufLen = USART0_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_USART1://����2
        UART_Param.Name         = "USART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_USART1_BASE;
        UART_Param.TxRingBufLen = USART1_SendBufLen;
        UART_Param.RxRingBufLen = USART1_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }

    //Ӳ����ʼ��
    __UART_HardInit(SerialNo);
    __UART_IntInit(SerialNo);

	sUartInited |= (0x01 << SerialNo);
	pUartCB[SerialNo] = UART_InstallPort(&UART_Param);
	if( pUartCB[SerialNo] == NULL)
		return 0;
	else
		return 1;
}

// =============================================================================
// ����: ��ӡ������ֱ��д���ڷ�ʽ��Ŀǰ��Ҫ��djy_printk�������ڵ��Թؼ������
// ����: ����Ҫ���͵��ַ�������Ȼ��ǰ�������ṩ��һ�����ַ���'\0'����
// ����: ���͵��ֽڸ���
// =============================================================================
u32 Uart_SendServiceDirectly(char *str)
{
    u32  result=0,len,timeout=100*mS;
	tagUartReg *Reg;
	u32 BaseAddr,Port;

	if(!strcmp(gc_pCfgStddevName,"UART0") && (sUartInited & (0x01 << CN_UART0)))
	{
		BaseAddr = CN_UART0_BASE;
		Port = CN_UART0;
	}
	else if(!strcmp(gc_pCfgStddevName,"UART1")&& (sUartInited & (0x01 << CN_UART1)))
	{
		BaseAddr = CN_UART1_BASE;
		Port = CN_UART1;
	}
	else if(!strcmp(gc_pCfgStddevName,"USART0")&& (sUartInited & (0x01 << CN_USART0)))
	{
		BaseAddr = CN_USART0_BASE;
		Port = CN_USART0;
	}
	else if(!strcmp(gc_pCfgStddevName,"USART1")&& (sUartInited & (0x01 << CN_USART1)))
	{
		BaseAddr = CN_USART1_BASE;
		Port = CN_USART1;
	}
	else
		return 0;

    len = strlen(str);
    Reg = (tagUartReg *)BaseAddr;

    __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);	//disable send INT
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
        Reg->UART_THR = str[result];
    }
    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);		//enable send INT
    return result;
}

