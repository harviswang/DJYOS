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
//����ģ��: uart��������(������������)
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2013-11-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------
#include "stdint.h"

#include "uart.h"
#include "cpu_peri_uart.h"
#include <cdef21469.h>
#include "def21469.h"



static struct tagUartUCB tg_uart1_CB;
static djy_handle_t pg_uart1_rhdl;

#define uart1_buf_len  2048

uint8_t uart1_send_buf[uart1_buf_len];
uint8_t uart1_recv_buf[uart1_buf_len];

//----ʹ�ܽ����ж�-------------------------------------------------------------
//����: ʹ��uart�Ľ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __Uart_RecvIntEnable(tag_UartReg volatile *reg)
{
    reg->rUART0LCR &= ~UARTDLAB;    // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

    reg->rUART0IER_DLH   |= UARTRBFIE;      // enables UART0 receive interrupt

}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�Ľ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __Uart_RecvIntDisable(tag_UartReg volatile *reg)
{
/// reg->rUART0LCR &= ~UARTDLAB;                // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

/// reg->rUART0IER_DLH   &= ~UARTRBFIE; // disables UART0 receive interrupt

}

//----ʹ�ܷ����ж�-------------------------------------------------------------
//����: ʹ��uart�ķ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __Uart_SendIntEnable(tag_UartReg volatile *reg)
{
    reg->rUART0LCR &= ~UARTDLAB;                // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

    reg->rUART0IER_DLH   |= UARTTBEIE;  // enables UART0 transmit interrupt

}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�ķ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __Uart_SendIntDisable(tag_UartReg volatile *reg)
{
    reg->rUART0LCR &= ~UARTDLAB;                // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

    reg->rUART0IER_DLH   &= ~UARTTBEIE; // disables UART0 transmit interrupt

}

//----����baud-----------------------------------------------------------------
//����: ���ô���baud
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __Uart_SetBaud(tag_UartReg volatile *reg,u32 baud)
{
    u32 temp;
    /* Sets the Baud rate for UART0 */
    reg->rUART0LCR |= UARTDLAB;  //enables access to Divisor register to set bauda rate
    temp = (u32)(CN_CFG_PCLK1)/(baud*16);
    reg->rUART0THR_RBR_DLL = (u8)temp;      //0x5b8 = 1464 for divisor value and gives a baud rate of 9600 for core clock 262.144MHz
    reg->rUART0IER_DLH = (u8)(temp>>8);

}
//----����Ӳ������-------------------------------------------------------------
//����: ����Ӳ�����������������ʡ�ֹͣλ��У��λ������λ
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __Uart_DefaultSet(tag_UartReg volatile *reg)
{
    __Uart_SetBaud(reg,115200);            //����������

    /* Configures UART0 LCR */
    reg->rUART0LCR = UARTWLS8;              // word length 8
                                        // parity enable None parity
                                        // One stop bits

    *pUART0RXCTL = UARTEN;       //enables UART0 in receive mode

    *pUART0TXCTL = UARTEN;       //enables UART0 in core driven mode

    __Uart_RecvIntEnable((tag_UartReg *)reg);
    __Uart_SendIntEnable((tag_UartReg *)reg);

}
//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
bool_t __Uart_TxTranEmpty(volatile tag_UartReg *reg)
{
    return ((0 != (reg->rUART0LSR & UARTTHRE))&&(0 != (reg->rUART0LSR & UARTTEMT)));
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: uart_dev,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
ptu32_t __Uart_Ctrl(djy_handle_t uart_dev,
                            u32 cmd, u32 data1,u32 data2)
{
    struct tagUartUCB *uart_port;
    uart_port = (struct tagUartUCB*)Driver_DevGetMyTag(uart_dev);

    switch(cmd)
    {
        case CN_UART_START:
            __Uart_RecvIntEnable((tag_UartReg *)uart_port->my_reg);
            __Uart_SendIntEnable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_UART_STOP:
            __Uart_RecvIntDisable((tag_UartReg *)uart_port->my_reg);
            __Uart_SendIntDisable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_UART_SET_BAUD:  //����Baud
            if(uart_port->baud !=data1)
            {
                uart_port->baud = data1;
                __Uart_SetBaud((tag_UartReg *)uart_port->my_reg,data1);
            }
            break;
        case CN_UART_RX_PAUSE:      //��ͣ����
            __Uart_RecvIntDisable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_UART_RX_RESUME:      //�ָ�����
            __Uart_RecvIntEnable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        {
            break;
        }
        default: break;
    }
    return 0;
}


//----���з��ͷ���ֱ��д�նˣ�----------------------------------------------
//����: �����ֱ��д���ں��������ᾭ���¼�����
//����: uart_dev,�������Ĵ����豸ָ��.
//      send_buf,�����͵Ļ�������
//      len,���͵������ֽ���
//      timeout,��ʱ
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
u32 __Uart_SendDirectly(djy_handle_t uart_dev,
                                      u8 *send_buf,u32 len,u32 timeout)
{
    u32  result,timecount;
    struct tagUartUCB   *uart_port;

    uart_port = (struct tagUartUCB*)Driver_DevGetMyTag(uart_dev);
    __Uart_SendIntDisable((tag_UartReg *)uart_port->my_reg);
    for(result=0; result < len; result ++)
    {
        timecount=timeout;
        while((false == __Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg))
            && (timecount > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        {
            timecount--;
            Djy_DelayUs(1);
        }
        if(timecount == 0)
            break;

        ((tag_UartReg *)uart_port->my_reg)->rUART0THR_RBR_DLL = send_buf[result];

        timecount=timeout;
        while((false == __Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg))
            && (timecount > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        {
            timecount--;
            Djy_DelayUs(1);
        }
        if(timecount == 0)
            break;

    }
    __Uart_SendIntEnable((tag_UartReg *)uart_port->my_reg);
    return result;
}

//----�������ڷ��ͺ���---------------------------------------------------------
//����: �������ڷ��ͣ�ֱ�ӷ���fifo��С�����ݣ����������Ϳ��жϣ����ж��н�
//      send_ring_buf�е���������������ݵķ��ͣ��൱��������һ�η��͹���
//����: uart_dev,�������Ĵ����豸ָ��.
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
u32 __Uart_SendStart(djy_handle_t uart_dev)
{
    u8 trans,num,ch[16];
    struct tagUartUCB *uart_port;
    uart_port = (struct tagUartUCB *)Driver_DevGetMyTag(uart_dev);

    __Uart_SendIntDisable((tag_UartReg *)uart_port->my_reg);
    if(__Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg))
    {
        trans = Driver_DevReadRight(uart_dev,(ptu32_t)ch,1,0);
        for(num = 0; num < trans; num++)
        {
            ((tag_UartReg *)uart_port->my_reg)->rUART0THR_RBR_DLL = ch[num];
        }
        while (!__Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg));
    }
    __Uart_SendIntEnable((tag_UartReg *)uart_port->my_reg);
    return trans;
}
//----uart�ж�---------------------------------------------------------------
//����: ���գ���ȡfifo�е����ݲ�������д���豸
//      ���ͣ������ֶ����ݲ�����
//����: �жϺ�.
//����: 0.
//-----------------------------------------------------------------------------
uint32_t Uart_Int(ufast_t uart_int_line)
{

    static djy_handle_t *uart_rhdl;
    tag_UartReg *reg;
    uint32_t recv_trans,num;

    uint8_t ch[20],IIR=0;

    reg =  (tag_UartReg *)tg_uart1_CB.my_reg;
    IIR = reg->rUART0IIR;
    uart_rhdl = &pg_uart1_rhdl;

    IIR = (IIR>>1) &0x7;
    switch(IIR)
    {
        case 1:     //Transmitter data
        {
            while (!__Uart_TxTranEmpty((tag_UartReg *)reg));
            recv_trans = Driver_DevReadRight(*uart_rhdl,(ptu32_t)ch,1,0);
            for(num = 0; num < recv_trans; num++)
            {
                reg->rUART0THR_RBR_DLL = ch[num];
            }
            while (!__Uart_TxTranEmpty((tag_UartReg *)reg));
        }break;
        case 2:     //Receive data
        {
            for(num = 0; num<1; num++)
            {
                ch[num] = reg->rUART0THR_RBR_DLL;

            }
            Driver_DevWriteRight(*uart_rhdl,(ptu32_t)ch,num,0);
        }break;
       default:
       {
            while (!__Uart_TxTranEmpty((tag_UartReg *)reg));
            break;
       }
    }
    return 0;
}

//----��ʼ��uart1ģ��----------------------------------------------------------
//���ܣ���ʼ��uart1ģ��,��װ�ش����豸
//���������ڳ�ʼ��ģ��Ĳ���Ϊ1��2��3...���ں�
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t Uart_ModuleInit(ptu32_t para)//todo fix
{
    static djy_handle_t *uart_rhdl;
    struct tagUartUCB *UCB;
    char *name;
    uint8_t *sendbuf,*recvbuf;
    uint32_t sendbuflen,recvbuflen;
    ptu32_t base_addr;

    switch(para)
    {
    case 1://����1
        UCB = &tg_uart1_CB;
        base_addr = cn_uart1_base;
        name = "uart1";
        sendbuf = uart1_send_buf;
        sendbuflen = uart1_buf_len;
        recvbuf = uart1_recv_buf;
        recvbuflen = uart1_buf_len;
        uart_rhdl = &pg_uart1_rhdl;
        break;
    default:
        return 0;
    }
    UCB->my_reg = (tag_UartReg *)base_addr;
    __Uart_DefaultSet((tag_UartReg *)UCB->my_reg);
    UCB->baud = 115200;
    UCB->start_send  = __Uart_SendStart;
    UCB->direct_send = __Uart_SendDirectly;
    UCB->Uart_Ctrl   = __Uart_Ctrl;
    //�ж��ߵĳ�ʼ��
//    Int_IsrConnect(cn_int_line_SP7I,Uart_Int);    Int_IsrConnect(cn_int_line_UARTI,Uart_Int);
//    Int_SettoAsynSignal(cn_int_line_SP7I);
//    Int_ClearLine(cn_int_line_SP7I);     //�����ʼ�������ķ���fifo�յ��ж�
//    Int_RestoreAsynLine(cn_int_line_SP7I);

    Int_IsrConnect(cn_int_line_UARTI,Uart_Int);
    Int_SettoAsynSignal(cn_int_line_UARTI);
    Int_ClearLine(cn_int_line_UARTI);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(cn_int_line_UARTI);

    *uart_rhdl = Uart_InstallPort(UCB,name,sendbuf,
                                sendbuflen,recvbuf,recvbuflen);
    if(*uart_rhdl != NULL)
        Driver_DevOpenRightFast(*uart_rhdl,0);

    return 0;
}


////////   test use only
u32 Uart_SendServiceDirectly(char *str)
{
    return 0;
}

