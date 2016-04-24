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
/*========================================================*
 * �ļ�ģ��˵��: omapl138��uart��������ֲ�Զ����߲���ϵͳ����ӵ�а�Ȩ
 * ��S3C2440 uart�����ļ������omapl138����һЩ�޸�
 * �ļ��汾: v1.00                                               *
 * ������Ա: lst                              *
 * ����ʱ��: 2011.06.01                                          *
 * Copyright(c) 2011-2011  ��������������޹�˾             *
 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):							     *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>				 *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/
//������ԭ���߰�Ȩ����
//----------------------------------------------------
//Copyright (C), 2004-2009,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�����Ŷ�
//����ģ��: uart��������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: 
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------
#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

//base address
#define cn_uart0_base   0x01c42000
#define cn_uart1_base   0x01d0c000
#define cn_uart2_base   0x01d0d000
typedef struct         //offset
{
    vu32 RBR_THR;         // 0x0000
    vu32 IER;             // 0x0004
    vu32 IIR_FCR;         // 0x0008
    vu32 LCR;             // 0x000C
    vu32 MCR;             // 0x0010
    vu32 LSR;             // 0x0014
    vu32 MSR;             // 0x0018
    vu32 SCR;             // 0x001C
    vu32 DLL;             // 0x0020
    vu32 DLH;             // 0x0024
    vu32 REV_ID1;         // 0x0028
    vu32 REV_ID2;         // 0x002C
    vu32 PWREMU_MGMT;     // 0x0030
    vu32 MDR;             // 0x0034
}tag_UartReg;


#define uart0_buf_len  512
#define uart1_buf_len  512
#define uart2_buf_len  512

//����״̬���ƽṹ
typedef struct tagUartUCB
{
    struct tagRingBuf send_ring_buf;           //���η��ͻ�����.
    struct tagRingBuf recv_ring_buf;           //���ν��ջ�����.

    struct tagSemaphoreLCB *send_buf_semp;     //���ͻ�������
    struct tagSemaphoreLCB *recv_buf_semp;     //���ջ�������

    u16 evtt_right_write;      //�豸���ֱ�д�󵯳����¼�����
    u16 evtt_left_write;       //�豸���ֱ�д�󵯳����¼�����
    u16 recv_trigger_level;    //���ڽ��մ���ˮƽ,���յ���������ʱ�����¼�
    u32 timeout;               //δ������ˮƽ,�����趨ʱ����û���յ�����,
                                    //Ҳ�����¼�.
    u16 send_trigger_level;    //����ͬ������ˮƽ�������ͻ��λ�����������
                                    //��send_buf_semp������ǰ�̣߳�ֱ�����ͻ���
                                    //������������send_trigger_levelʱ�ͷ�֮��
    tag_UartReg volatile *my_reg;
    u32 baud;
    ufast_t status;
    ufast_t serial_no;
};

//ͨ��״̬������λ����
#define CN_SER_RXBUF_BUSY         (1<<0)
#define CN_SER_RXBUF_EMPTY     (1<<1)
#define CN_SER_RECEIVE_OVER    (1<<2)

//���ڴ�������
enum _UART_ERROR_NO_
{
    EN_UART_RECV_FIFO_OVER,         //����Ӳ��fifo���
    EN_UART_RECV_BUF_OVER,          //���ջ��������
};

//�����豸���������������uart_ctrl������
#define cn_uart_connect_recv_evtt     0  //�豸��������д������ݳ����������ˮ
                                         //ƽʱ�����������¼�
#define cn_uart_disconnect_recv_evtt  1  //�Ͽ������¼�
#define cn_uart_connect_send_evtt     2  //�豸��������д������ʱ�����������¼�
#define cn_uart_disconnect_send_evtt  3  //�Ͽ������¼�
#define CN_UART_START                 4  //��������
#define CN_UART_STOP                  5  //ֹͣ����
#define CN_UART_SET_BAUD              6  //����uartBaud.
#define CN_UART_SEND_DATA             7  //��������
#define CN_UART_RECV_DATA             8  //��������
#define CN_UART_COMPLETED_SEND        9  //��ɷ��͹���
#define CN_UART_RX_PAUSE              10 //��ͣ�������ݵ�������
#define CN_UART_RX_RESUME             11 //���¿�ʼ�������ݵ�������
#define CN_UART_RX_OVERTIME              12 //���ý��ճ�ʱʱ�䣬������
#define CN_UART_RECV_SOFT_LEVEL       13 //���ý����������������ˮƽ
#define CN_UART_SEND_SOFT_LEVEL       14 //���ý����������������ˮƽ
#define CN_UART_RECV_HARD_LEVEL       15 //���ý���fifo����ˮƽ
#define CN_UART_SEND_HARD_LEVEL       16 //���÷���fifo����ˮƽ

ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);
ptu32_t module_init_uart2(ptu32_t para);
ptu32_t module_init_uart3(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


