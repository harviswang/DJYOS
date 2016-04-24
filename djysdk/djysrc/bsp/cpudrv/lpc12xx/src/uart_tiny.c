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
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:lst
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------
#include "stdint.h"
#include <string.h>
#include "cpu_peri.h"
#include "uart_tiny.h"


//#define UART0_BRD 57600   //���ڲ�����
//#define UART1_BRD 57600   //���ڲ�����

struct uart_line_buf
{
    uint8_t tagLineBuf[uart1_buf_len];
    u32     pt;
};

#define UART0_BRD   2500000 //���ڲ�����
#define UART1_BRD   2500000 //���ڲ�����


#define UART1_RTS_HIGH()    GPIO_SettoHigh(0,1<<27) // set_pin(0,27)
#define UART1_RTS_LOW()     GPIO_SettoLow(0,1<<27)      //clr_pin(0,27)

#if(CN_CFG_UART0_USED == 1)
static struct tagUartUCB tg_uart0_cb;
uint8_t uart0_drv_send_buf[uart0_buf_len];
uint8_t uart0_drv_recv_buf[uart0_buf_len];
#endif

#if(CN_CFG_UART1_USED == 1)
static struct tagUartUCB tg_uart1_cb;
static uint8_t uart1_drv_send_buf[uart1_buf_len];
static uint8_t uart1_drv_recv_buf[uart1_buf_len];
static struct uart_line_buf uart1_line_buf1,uart1_line_buf2,*pg_inuse_line_buf;
#endif
u32 __uart0_int(ufast_t uart_int_line);
u32 __uart1_int(ufast_t uart_int_line);
void __uart0_pause_send_int(void);
void __uart1_pause_send_int(void);

#if(CN_CFG_UART0_USED == 1)

//----����uart0 baud--------------------------------------------------------
//����: ����uart0 baud,ע:������δ����С��baud
//����: baud
//����: ��
//-----------------------------------------------------------------------------
void __uart0_set_baud(u32 baud)
{
    u32 n;
    n = (CN_CFG_MCLK / baud)>>4;
    pg_uart_reg0->LCR |= UART_LCR_DLAB_EN;
    pg_uart_reg0->RTD.DLL = (u8)(n % 256);
    pg_uart_reg0->DI.DLM = (u8)(n / 256);
    pg_uart_reg0->LCR &= (~UART_LCR_DLAB_EN) & 0xff;
    pg_uart_reg0->FDR = 0;    //��δ����С��
}

//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ�飬��ʼ��״̬Ϊ��ʹ��dma���͵ķ�ʽ��
//������para���ڱ���������Ч
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart0(ptu32_t para)
{
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart0_cb.send_buf_semp = Lock_SempCreate(1,0,"uart0 tx buf");
    if(tg_uart0_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart0_cb.recv_buf_semp = Lock_SempCreate(1,0,"uart0 rx buf");
    if(tg_uart0_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //��ʼ��IO�˿�λuart����
    sys_enable_peri(cn_peri_uart0);
    sys_enable_peri_clk(cn_peri_clksrc_uart0);
    sys_set_uart0_clkdiv(1);


    __uart0_set_baud(UART0_BRD);     //���ò�����Ϊ�궨���е���ֵ

    pg_uart_reg0->IF.IIR;     //��һ����������жϱ�־
//    LPC_UART0->IIR_FCR |= 1;   //set FIFOEN
    pg_uart_reg0->LCR |= UART_LCR_DLAB_EN;
    pg_uart_reg0->IF.FCR = 0x1;   //ʹ��ifo��
    pg_uart_reg0->IF.FCR = 0x47;  //����4�ֽڴ����жϣ����շ�fifo��
    pg_uart_reg0->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    pg_uart_reg0->MCR = 0;
    pg_uart_reg0->TER = UART_TER_TXEN;
    pg_uart_reg0->DI.IER =  UART_INTCFG_RBR;       //ʹ���շ��жϣ�
    pg_uart_reg0->RS485CTRL =      UART_RS485_RECEIVER_ENABLE
                            |   UART_RS485_AUTO_DIRECTION_ENABLE
                            |   UART_RS485_DIRPIN_HIGH
                            |   UART_RS485_DIRPIN_RTS;


    tg_uart0_cb.baud = UART0_BRD;

    tg_uart0_cb.sendding = false;
    Ring_Init(  &tg_uart0_cb.send_ring_buf,
                uart0_drv_send_buf,
                uart0_buf_len);
    Ring_Init(  &tg_uart0_cb.recv_ring_buf,
                uart0_drv_recv_buf,
                uart0_buf_len);

    Int_IsrConnect(cn_int_line_uart0,__uart0_int);
    Int_SettoAsynSignal(cn_int_line_uart0);

    Int_ClearLine(cn_int_line_uart0);
    Int_RestoreAsynLine(cn_int_line_uart0);
    return true;

exit_from_right_buf_semp:
    Lock_SempDelete(tg_uart0_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart1�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
u32 __uart0_int(ufast_t uart_int_line)
{
    u32 recv_trans,num;
    u32 IIR;
    uint8_t ch[16];
    while(1)
    {
        IIR = pg_uart_reg0->IF.IIR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = Ring_Read(&tg_uart0_cb.send_ring_buf,ch,16);
                if(recv_trans != 0)
                {
                    for(num = 0; num < recv_trans; num++)
                    {
                        pg_uart_reg0->RTD.THR = ch[num];
                    }
                }
                if(recv_trans < 16)
                    __uart0_pause_send_int();
                if(Ring_Check(&tg_uart0_cb.send_ring_buf) < 16)
                    Lock_SempPost(tg_uart0_cb.send_buf_semp);
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
            {
                for(num = 0; num<16; num++)
                {
                    if(pg_uart_reg0->LSR & 1)
                        ch[num] = pg_uart_reg0->RTD.THR;
                    else
                        break;
                }
                Ring_Write(&tg_uart0_cb.recv_ring_buf,ch,num);
                Lock_SempPost(tg_uart0_cb.recv_buf_semp);
            }break;
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

void __uart0_pause_send_int(void)
{
    pg_uart_reg0->DI.IER &=  ~(u32)UART_INTCFG_THRE;          //��ֹ�����жϣ�
}
void __uart0_resume_send_int(void)
{
    pg_uart_reg0->DI.IER |=  (u32)UART_INTCFG_THRE;           //ʹ�ܷ����жϣ�
}
void uart0_send(u8 *buf,u32 len)
{
    u32 completed = 0,sended;
    if((len==0) || (buf == NULL))
        return;
    __uart0_pause_send_int();
    if(Ring_IsEmpty(&tg_uart0_cb.send_ring_buf))
    {
        if(pg_uart_reg0->LSR &UART_LS_TX_EMPTY)
        {
            if(len < 16)
            {
                for(completed = 0; completed < len; completed++)
                    pg_uart_reg0->RTD.THR = buf[completed];
            }else
            {
                for(completed = 0; completed < 16; completed++)
                    pg_uart_reg0->RTD.THR = buf[completed];
            }
        }
    }
    if(completed < len)
    {
        while(1)
        {
            sended = Ring_Write(&tg_uart0_cb.send_ring_buf,
                                (uint8_t*)buf+completed,len-completed);
            __uart0_resume_send_int();
            if(sended != len-completed)     //����������û���ͳ�ȫ������
            {
                completed += sended;
                Lock_SempPend(tg_uart0_cb.send_buf_semp,CN_TIMEOUT_FOREVER);
                __uart0_pause_send_int();
            }else
            {
                break;
            }
        }
    }
}

//========================================================
// �������ܣ�����0��������
// �����������
// �����������
// ����ֵ  ����
//========================================================
u32 uart0_read(u8 *buf,u32 len,u32 timeout)
{
    u32 readed,completed=0,start_time,time=0;
    start_time = DjyGetTime();
    while(1)
    {

        readed = Ring_Read(&tg_uart0_cb.recv_ring_buf,buf+completed,len-completed);

        if(readed != len-completed)
        {
            completed += readed;
            if(Lock_SempPend(tg_uart0_cb.recv_buf_semp,timeout-time) == false)
            {
                break;
            }else
            {
                time = DjyGetTime() - start_time;     //�ص�while���ٴζ����ڡ�
            }
        }else
        {
            completed = len;                            //�Ѿ���ɣ�
            break;
        }

    }
//  if(completed>len)
//      time=0;
    return completed;
}
#endif  //for (CN_CFG_UART0_USED == 1)

#if(CN_CFG_UART1_USED == 1)

u32 __uart1_tx(ufast_t uart_tx_line);
u32 __uart1_rx(ufast_t uart_rx_line);

//----����uart0 baud--------------------------------------------------------
//����: ����uart0 baud,ע:������δ����С��baud
//����: baud
//����: ��
//-----------------------------------------------------------------------------
void __uart1_set_baud(u32 baud)
{
    u32 n;
    n = (CN_CFG_MCLK / baud)>>4;
    pg_uart_reg1->LCR |= UART_LCR_DLAB_EN;
    pg_uart_reg1->RTD.DLL = (u8)(n % 256);
    pg_uart_reg1->DI.DLM = (u8)(n / 256);
    pg_uart_reg1->LCR &= (~UART_LCR_DLAB_EN) & 0xff;
    pg_uart_reg1->FDR = 0;    //��δ����С��
}

//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ�飬��ʼ��״̬Ϊ��ʹ��dma���͵ķ�ʽ��
//������para���ڱ���������Ч
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart1(ptu32_t para)
{
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart1_cb.send_buf_semp = Lock_SempCreate(1,0,"uart0 tx buf");
    if(tg_uart1_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart1_cb.recv_buf_semp = Lock_SempCreate(1,0,"uart0 rx buf");
    if(tg_uart1_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //��ʼ��IO�˿�λuart����
    sys_enable_peri(cn_peri_uart1);
    sys_enable_peri_clk(cn_peri_clksrc_uart1);
    sys_set_uart1_clkdiv(1);


    __uart1_set_baud(UART1_BRD);     //���ò�����Ϊ�궨���е���ֵ

  //���ò�����Ϊ1843200
//  LPC_UART1->LCR |= UART_LCR_DLAB_EN;
//  LPC_UART1->RTD.DLL = 1;
//  LPC_UART1->DI.DLM = 0;
//  LPC_UART1->LCR &= (~UART_LCR_DLAB_EN) & 0xff;
//  LPC_UART1->FDR = 0x92; //0x92=1843200   0x01=2.25M
  //�������ò�����

/*  ////////////ADD BY HUANGCW
    LPC_UART1->FDR = 0x10;
    LPC_UART1->IF.FCR = 0;
    while (LPC_UART1->LSR & 0x01)
    {
        tmp = LPC_UART1->RTD.RBR;        // Dummy reading
    }
    LPC_UART1->TER = UART_TER_TXEN ;
    while (!(LPC_UART1->LSR & UART_LS_TX_EMPTY));  // Wait for current transmit complete
    LPC_UART1->TER = 0;             // Disable Tx
    LPC_UART1->DI.IER = 0;             // Disable interrupt
    LPC_UART1->LCR = 0;             // Set LCR to default state
    LPC_UART1->ACR = 0;             // Set ACR to default state

    tmp = LPC_UART1->LSR;           // Dummy reading

    //����uart
    uart_set_divisors(LPC_UART1,115200);
    LPC_UART1->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    LPC_UART1->IF.FCR = 0x81;
    LPC_UART1->TER |= UART_TER_TXEN;
    LPC_UART1->DI.IER |=  UART_INTCFG_RBR;
    NVIC->ISER[0] = (1 << ((u32)(18) & 0x1F));
    ////////////END
  */
    pg_uart_reg1->IF.IIR;     //��һ����������жϱ�־
//    LPC_UART1->IIR_FCR |= 1;   //set FIFOEN
    pg_uart_reg1->LCR |= UART_LCR_DLAB_EN;
    pg_uart_reg1->IF.FCR = 0x1;   //ʹ��ifo��
    pg_uart_reg1->IF.FCR = 0x47;  //����4�ֽڴ����жϣ����շ�fifo��
    pg_uart_reg1->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    pg_uart_reg1->MCR = 0;
    pg_uart_reg1->TER = UART_TER_TXEN;
    pg_uart_reg1->DI.IER =  UART_INTCFG_RBR;       //ʹ���շ��жϣ�
    pg_uart_reg1->RS485CTRL =      UART_RS485_RECEIVER_ENABLE
                            |   UART_RS485_AUTO_DIRECTION_ENABLE
                            |   UART_RS485_DIRPIN_HIGH
                            |   UART_RS485_DIRPIN_RTS;


    tg_uart1_cb.baud = UART1_BRD;

    tg_uart1_cb.sendding = false;
    Ring_Init(  &tg_uart1_cb.send_ring_buf,
                uart1_drv_send_buf,
                uart1_buf_len);
    Ring_Init(  &tg_uart1_cb.recv_ring_buf,
                uart1_drv_recv_buf,
                uart1_buf_len);

    pg_inuse_line_buf = &uart1_line_buf1;

    Int_IsrConnect(cn_int_line_uart1,__uart1_int);
    Int_SettoReal(cn_int_line_uart1);

    Int_IsrConnect(cn_int_line_wakeup1,__uart1_rx);
    Int_SettoAsynSignal(cn_int_line_wakeup1);
    Int_RestoreAsynLine(cn_int_line_wakeup1);


    Int_IsrConnect(cn_int_line_wakeup2,__uart1_tx);
    Int_SettoAsynSignal(cn_int_line_wakeup2);
    Int_RestoreAsynLine(cn_int_line_wakeup2);

    Int_ClearLine(cn_int_line_uart1);
    Int_RestoreRealLine(cn_int_line_uart1);
    return true;

exit_from_right_buf_semp:
    Lock_SempDelete(tg_uart1_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart1�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
u32 __uart1_int(ufast_t uart_int_line)
{
    u32 recv_trans,num;
    u32 IIR;
    uint8_t ch[16];
    GPIO_SettoLow(2,1<<14);   //dbg
    while(1)
    {
        IIR = pg_uart_reg1->IF.IIR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = Ring_Read(&tg_uart1_cb.send_ring_buf,ch,16);
                if(recv_trans != 0)
                {
                    for(num = 0; num < recv_trans; num++)
                    {
                        pg_uart_reg1->RTD.THR = ch[num];
                    }
                }
                if(recv_trans < 16)
                    __uart1_pause_send_int();
                if(Ring_Check(&tg_uart1_cb.send_ring_buf) < 16)
                    Int_TapLine(cn_int_line_wakeup2);
//                    Lock_SempPost(tg_uart1_cb.send_buf_semp);
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
            {
                for(num = 0; num<16; num++)
                {
                    if(pg_uart_reg1->LSR & 1)
                    {
                        pg_inuse_line_buf->tagLineBuf[pg_inuse_line_buf->pt]
                                        = pg_uart_reg1->RTD.THR;
                        pg_inuse_line_buf->pt++;
                    }else
                        break;
                }
//                Ring_Write(&tg_uart1_cb.recv_ring_buf,ch,num);                  dbg
                Int_TapLine(cn_int_line_wakeup1);
//                Lock_SempPost(tg_uart1_cb.recv_buf_semp);
            }break;
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
    }
    GPIO_SettoHigh(2,1<<14);   //dbg
    return 0;
}

void __uart1_pause_send_int(void)
{
    pg_uart_reg1->DI.IER &=  ~(u32)UART_INTCFG_THRE;          //��ֹ�����жϣ�
}
void __uart1_resume_send_int(void)
{
    pg_uart_reg1->DI.IER |=  (u32)UART_INTCFG_THRE;           //ʹ�ܷ����жϣ�
}

void __uart1_pause_recv_int(void)
{
    pg_uart_reg1->DI.IER &=  ~(u32)UART_INTCFG_RBR;          //��ֹ�����жϣ�
}
void __uart1_resume_recv_int(void)
{
    pg_uart_reg1->DI.IER |=  (u32)UART_INTCFG_RBR;           //ʹ�ܷ����жϣ�
}
void uart1_send(u8 *buf,u32 len)
{
    u32 completed = 0,sended;
    if((len==0) || (buf == NULL))
        return;

    UART1_RTS_HIGH();   //ʹ��485����ģʽ --by huangcw  2012-1-31
    __uart1_pause_send_int();
    if(Ring_IsEmpty(&tg_uart1_cb.send_ring_buf))
    {
        if(pg_uart_reg1->LSR &UART_LS_TX_EMPTY)
        {
            if(len < 16)
            {
                for(completed = 0; completed < len; completed++)
                    pg_uart_reg1->RTD.THR = buf[completed];
            }else
            {
                for(completed = 0; completed < 16; completed++)
                    pg_uart_reg1->RTD.THR = buf[completed];
            }
        }
    }
    if(completed < len)
    {
        while(1)
        {
            sended = Ring_Write(&tg_uart1_cb.send_ring_buf,
                                (uint8_t*)buf+completed,len-completed);
            __uart1_resume_send_int();
            if(sended != len-completed)     //����������û���ͳ�ȫ������
            {
                completed += sended;
                Lock_SempPend(tg_uart1_cb.send_buf_semp,CN_TIMEOUT_FOREVER);
                __uart1_pause_send_int();
            }else
            {
                break;
            }
        }
    }

    //add by huangcw --2012-1-31
    while( !( pg_uart_reg1->LSR & UART_LS_TX_EMPTY_ALL ) )//�ȴ����з������
            {
            }
        UART1_RTS_LOW();    //�ر�485����ģʽ
    //end  add
}

//========================================================
// �������ܣ�����0��������
// �����������
// �����������
// ����ֵ  ����
//========================================================
u32 uart1_read(u8 *buf,u32 len,u32 timeout)
{
    u32 readed,completed=0,start_time,time=0;
    start_time = DjyGetTime();

    while(1)
    {

        __uart1_pause_recv_int();
        readed = Ring_Read(&tg_uart1_cb.recv_ring_buf,buf+completed,len-completed);
        __uart1_resume_recv_int();

        if(readed != len-completed)
        {
            completed += readed;
            if(Lock_SempPend(tg_uart1_cb.recv_buf_semp,timeout-time) == false)
            {
                break;
            }else
            {
                time = DjyGetTime() - start_time;     //�ص�while���ٴζ����ڡ�
            }
        }else
        {
            completed = len;                            //�Ѿ���ɣ�
            break;
        }

    }
//  if(completed>len)
//      time=0;
    return completed;
}

u32 __uart1_tx(ufast_t uart_tx_line)
{
    Lock_SempPost(tg_uart1_cb.send_buf_semp);
    Int_ClearLine(uart_tx_line);
}

u32 __uart1_rx(ufast_t uart_rx_line)
{
    struct uart_line_buf *buf;
    buf = pg_inuse_line_buf;
    __uart1_pause_recv_int();
    if(buf == &uart1_line_buf1)
    {
        pg_inuse_line_buf = &uart1_line_buf2;
        uart1_line_buf2.pt = 0;
    }else
    {
        pg_inuse_line_buf = &uart1_line_buf1;
        uart1_line_buf1.pt = 0;
    }
    __uart1_resume_recv_int();
    Ring_Write(&tg_uart1_cb.recv_ring_buf,buf->tagLineBuf,buf->pt);
    Lock_SempPost(tg_uart1_cb.recv_buf_semp);
    Int_ClearLine(uart_rx_line);
    return 0;
}

#endif  //for (CN_CFG_UART1_USED == 1)





