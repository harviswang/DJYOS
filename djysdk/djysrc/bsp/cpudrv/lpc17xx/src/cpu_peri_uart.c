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

#include "cpu_peri.h"
#include "string.h"
#include "uart.h"

extern const char *gc_pCfgStddevName;  //��׼�ն�����
// =============================================================================
#define tagUartReg              LPC_UART_TypeDef
#define CN_UART0_BASE           LPC_UART0_BASE
#define CN_UART1_BASE           LPC_UART1_BASE
#define CN_UART2_BASE           LPC_UART2_BASE
#define CN_UART3_BASE           LPC_UART3_BASE
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE};
static u8 tg_UART_PCON[] = {3,4,24,25};                 // UARTxʱ�Ӷ�Ӧ�ı���λ

#define UART0_SendBufLen            32
#define UART0_RecvBufLen            32

#define UART1_SendBufLen            32
#define UART1_RecvBufLen            32

#define UART2_SendBufLen            32
#define UART2_RecvBufLen            32

#define UART3_SendBufLen            32
#define UART3_RecvBufLen            32

static struct tagUartCB *pUartCB[CN_UART_NUM];
//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;
// =============================================================================
static ptu32_t UART_ISR(ufast_t IniLine);

// ----ʹ�ܽ����ж�-------------------------------------------------------------
// ����: ʹ��uart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
//-----------------------------------------------------------------------------
static void __UART_RxIntEnable(tagUartReg volatile *reg)
{
    reg->IER |= (0x01 << 0);
}

// ----��ֹ�����ж�-------------------------------------------------------------
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
//-----------------------------------------------------------------------------
static void __UART_RxIntDisable(tagUartReg volatile *reg)
{
    reg->IER &= ~(0x01 << 0);
}

// ----ʹ�ܷ����ж�-------------------------------------------------------------
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// -----------------------------------------------------------------------------
static void __UART_TxIntEnable(tagUartReg volatile *reg)
{
    reg->IER |= (0x01 << 1);
}

// ----��ֹ�����ж�-------------------------------------------------------------
// ����: ��ֹuart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// -----------------------------------------------------------------------------
static void __UART_TxIntDisable(tagUartReg volatile *reg)
{
    reg->IER &= ~(0x01 << 1);
}

// ----��鴮�ڷ��ͼĴ����Ƿ��--------------------------------------------------
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// -----------------------------------------------------------------------------
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->LSR & (1<<5)));
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 baud)
{
    u8 DLL,DLM,FDR;
    if(Reg == NULL)
        return;
    switch(baud)
    {
    case CN_UART_BAUD_9600:
        DLM = 0;DLL = 0x6C;FDR = 0x21;
        break;
    case CN_UART_BAUD_19200:
        DLM = 0;DLL = 0x36;FDR = 0x21;
        break;
    case CN_UART_BAUD_57600:
        DLM = 0;DLL = 0x12;FDR = 0x21;
        break;
    case CN_UART_BAUD_115200:
        DLM = 0;DLL = 0x9;FDR = 0x21;
        break;
    default:
        DLM = 0;DLL = 0x9;FDR = 0x21;       // 115200
        break;
    }
    Reg->LCR |= (1<<7);         //  DLAB = 1
    Reg->DLL  = DLL;            // 115200
    Reg->FDR  = FDR;
    Reg->DLM  = DLM;
    Reg->LCR &= ~(1<<7);        // DLAB = 0
}

// =============================================================================
// ����: ���ö�ӦUART��IO�ڣ�����ʱ�Ӻ�IO����
// ����: SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_GpioConfig(u8 SerialNo)
{
    GPIO_PowerOn(1);
    switch(SerialNo)
    {
    case CN_UART0:
        GPIO_CfgPinFunc(0,2,1);
        GPIO_CfgPinFunc(0,3,1);
        break;
    case CN_UART1:
        GPIO_CfgPinFunc(2,0,2);
        GPIO_CfgPinFunc(2,1,2);
        break;
    case CN_UART2:
        GPIO_CfgPinFunc(0,10,1);
        GPIO_CfgPinFunc(0,11,1);
        break;
    case CN_UART3:
        GPIO_CfgPinFunc(0,0,2);
        GPIO_CfgPinFunc(0,1,2);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,ptu32_t data)
{
    struct tagCOMParam *COM;
    if((data == 0) || (Reg == NULL))
        return;
    COM = (struct tagCOMParam *)data;
    __UART_BaudSet(Reg,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_5:    Reg->LCR &= ~0x3; Reg->LCR |= 0x00;break;
    case CN_UART_DATABITS_6:    Reg->LCR &= ~0x3; Reg->LCR |= 0x01;break;
    case CN_UART_DATABITS_7:    Reg->LCR &= ~0x3; Reg->LCR |= 0x02;break;
    case CN_UART_DATABITS_8:    Reg->LCR &= ~0x3; Reg->LCR |= 0x03;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    case CN_UART_PARITY_ODD:    Reg->LCR &=~(0x03<<4);Reg->LCR |=0x00<<4;break;
    case CN_UART_PARITY_EVEN:   Reg->LCR &=~(0x03<<4);Reg->LCR |=0x01<<4;break;
    case CN_UART_PARITY_MARK:   Reg->LCR &=~(0x03<<4);Reg->LCR |=0x02<<4;break;
    case CN_UART_PARITY_SPACE:  Reg->LCR &=~(0x03<<4);Reg->LCR |=0x03<<4;break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    case CN_UART_STOPBITS_1:    Reg->LCR &= ~(0x01<<2);break;
    case CN_UART_STOPBITS_1_5:  break;
    case CN_UART_STOPBITS_2:    Reg->LCR |= (0x01<<2);break;
    default:break;
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
    if(SerialNo == CN_UART0)
        IntLine = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_UART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_UART2;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_UART3;
    else
        return;
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
    if(SerialNo > CN_UART3)
        return;
    __UART_GpioConfig(SerialNo);
    LPC_SC->PCONP |= (1 << tg_UART_PCON[SerialNo]); // ����Ӧ����ģ���ʱ��

    tg_UART_Reg[SerialNo]->IIR;             // ��һ����������жϱ�־
    tg_UART_Reg[SerialNo]->LCR |= 0x83;     // 8 bits, no Parity, 1 Stop bit
    tg_UART_Reg[SerialNo]->DLL  = 9;        // 115200
    tg_UART_Reg[SerialNo]->FDR  = 0x21;
    tg_UART_Reg[SerialNo]->DLM  = 0;
    tg_UART_Reg[SerialNo]->LCR  = 0x03;     // DLAB = 0
    tg_UART_Reg[SerialNo]->FCR  = 0x07;     // ʹ��FIFO�������ô���ˮƽ
    tg_UART_Reg[SerialNo]->TER  = 0x80;     // ����ʹ��
    tg_UART_Reg[SerialNo]->IER  = 0x01;     // ʹ�ܽ����ж�
}

// =============================================================================
// ����: ֱ�ӷ��ͺ�����ʹ����ѯ��ʽ��
// ����: PrivateTag,�������Ĵ����豸ָ��.
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
    __UART_TxIntDisable(Reg);
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
        Reg->THR = send_buf[result];
    }
    __UART_TxIntEnable(Reg);
    return result;
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg,u32 timeout)
{
    u8 trans=0,num,ch[16],port;

    switch((u32)Reg)
    {
    case CN_UART0_BASE:   port = CN_UART0;   break;
    case CN_UART1_BASE:   port = CN_UART1;   break;
    case CN_UART2_BASE:   port = CN_UART2;   break;
    case CN_UART3_BASE:   port = CN_UART3;   break;
    default:return 0;
    }

    __UART_TxIntDisable(Reg);
    if(__UART_TxTranEmpty(Reg))
    {
        trans = UART_PortRead(pUartCB[port],ch,16,0);// UART��FIFO��СΪ16�ֽ�
        for(num = 0; num < trans; num++)
        {
            Reg->THR = ch[num];
        }
    }
    __UART_TxIntEnable(Reg);
    return trans;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: Reg,�������Ĵ��ڼĴ���ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, u32 data1,u32 data2)
{
    if(Reg == NULL)
        return 0;
    switch(cmd)
    {
        case CN_UART_START:
            __UART_RxIntEnable(Reg);
            __UART_TxIntEnable(Reg);
            break;
        case CN_UART_STOP:
            __UART_RxIntDisable(Reg);
            __UART_TxIntDisable(Reg);
            break;
        case CN_UART_SET_BAUD:  //����Baud
                __UART_BaudSet(Reg,data1);
            break;
        case CN_UART_RX_PAUSE:      //��ͣ����
            __UART_RxIntDisable(Reg);
            break;
        case CN_UART_RX_RESUME:      //�ָ�����
            __UART_RxIntEnable(Reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        {
            Reg->FCR &= ~(0x03<<6);
            switch (data1)
            {
                case 1:
                    Reg->FCR |= 0x00<<6;//1�ֽڴ���
                    break;
                case 4:
                    Reg->FCR |= 0x01<<6;;//4�ֽڴ���
                    break;
                case 8:
                    Reg->FCR |= 0x02<<6;;//8�ֽڴ���
                    break;
                case 14:
                    Reg->FCR |= 0x03<<6;;//14�ֽڴ���
                    break;
                default :break;
            }
            break;
        }
        case CN_UART_COM_SET:
            __UART_ComConfig(Reg,data1);
            break;
        default: break;
    }
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
static u32 UART_ISR(ufast_t IntLine)
{
    struct tagUartCB *UCB;
    volatile tagUartReg *Reg;
    u32 recv_trans,num,IIR;
    u8 ch[16],port;

    switch(IntLine)
    {
    case CN_INT_LINE_UART0:   port = CN_UART0;   break;
    case CN_INT_LINE_UART1:   port = CN_UART1;   break;
    case CN_INT_LINE_UART2:   port = CN_UART2;   break;
    case CN_INT_LINE_UART3:   port = CN_UART3;   break;
    default:return 0;
    }

    UCB = pUartCB[port];
    Reg =  tg_UART_Reg[port];
    while(1)
    {
        IIR = Reg->IIR;
        if(IIR & 0x01)  //���ж�
            return 0;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = UART_PortRead(UCB,ch,16,0);
                if(recv_trans != 0)
                {
                    for(num = 0; num < recv_trans; num++)
                    {
                        Reg->THR = ch[num];
                    }
                }
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
            {
                for(num = 0; num<16; num++)
                {
                    if(Reg->LSR & 0x01)
                        ch[num] = Reg->RBR;
                    else
                        break;
                }
                UART_PortWrite(UCB,ch,num,0);
            }break;
            case 3:     //�����־
            {
                UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
            }break;
            default: return 0;
        }
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
    case CN_UART0://����0
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = UART0_SendBufLen;
        UART_Param.RxRingBufLen = UART0_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://����1
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
    u32 BaseAddr;
    tagUartReg *Reg;

    if(!strcmp(gc_pCfgStddevName,"UART0") && (sUartInited & (0x01 << CN_UART0)))
        BaseAddr = CN_UART0_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART1")&& (sUartInited & (0x01 << CN_UART1)))
        BaseAddr = CN_UART1_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART2")&& (sUartInited & (0x01 << CN_UART2)))
        BaseAddr = CN_UART2_BASE;
    else if(!strcmp(gc_pCfgStddevName,"UART3")&& (sUartInited & (0x01 << CN_UART3)))
        BaseAddr = CN_UART3_BASE;
    else
        return 0;

    Reg = (tagUartReg *)(BaseAddr);
    len = strlen(str);
    __UART_TxIntDisable(Reg);
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
        Reg->THR = str[result];
    }
    __UART_TxIntEnable(Reg);
    return result;
}
