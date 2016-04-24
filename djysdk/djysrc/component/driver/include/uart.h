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
//����ģ��: uart��������(�����ӿڲ���)
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2013-11-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//-----------------------------------------------------------------------------
#ifndef __UART_H__
#define __UART_H__
#include "driver.h"
#include "ring.h"

#ifdef __cplusplus
extern "C" {
#endif
struct tagUartCB;
// ���ڲ����ʶ���
enum tagUartBaud
{
    CN_UART_BAUD_110     =  110,
    CN_UART_BAUD_300     =  300,
    CN_UART_BAUD_600     =  600,
    CN_UART_BAUD_1200    =  1200,
    CN_UART_BAUD_2400    =  2400,
    CN_UART_BAUD_4800    =  4800,
    CN_UART_BAUD_9600    =  9600,
    CN_UART_BAUD_19200   =  19200,
    CN_UART_BAUD_38400   =  38400,
    CN_UART_BAUD_57600   =  57600,
    CN_UART_BAUD_115200  =  115200,
    CN_UART_BAUD_230400  =  230400,
    CN_UART_BAUD_460800  =  460800,
    CN_UART_BAUD_921600  =  921600,
};

// ��������λ
enum tagUartDataBits
{
    CN_UART_DATABITS_5  = 5,
    CN_UART_DATABITS_6  = 6,
    CN_UART_DATABITS_7  = 7,
    CN_UART_DATABITS_8  = 8,
    CN_UART_DATABITS_9  = 9
};

// ������żУ��
enum tagUartParity
{
    CN_UART_PARITY_NONE = 0,
    CN_UART_PARITY_ODD,
    CN_UART_PARITY_EVEN,
    CN_UART_PARITY_MARK,
    CN_UART_PARITY_SPACE
};

// ����ֹͣλ
enum tagUartStopBits
{
    CN_UART_STOPBITS_1 = 0,
    CN_UART_STOPBITS_1_5,
    CN_UART_STOPBITS_2
};
// ���ڴ�������ṹ��
struct tagCOMParam
{
    enum tagUartBaud       BaudRate;
    enum tagUartDataBits   DataBits;
    enum tagUartParity     Parity;
    enum tagUartStopBits   StopBits;
};

typedef u32 (* UartStartSend)(ptu32_t PrivateTag,u32 timeout);
typedef u32 (* UartDirectSend)(ptu32_t PrivateTag,u8 *send_buf,u32 len,u32 timeout);
typedef ptu32_t (*UartControl)(ptu32_t PrivateTag,u32 cmd, u32 data1,u32 data2);

// ����ģ���ʼ���ṹ��
struct tagUartParam
{
    const char *Name;					//UART���ƣ���UART0
    u32 TxRingBufLen;					//���ͻ����������ֽ���
    u32 RxRingBufLen;					//���ջ����������ֽ���
    u32 Baud;							//Ĭ�ϵĲ�����
    ptu32_t UartPortTag;				//UART˽�б�ǩ����Ĵ�����ַ
    UartStartSend StartSend;			//�������ͻص�����ָ��
    UartDirectSend DirectlySend;		//ֱ����ѯ���ͻص�����ָ��
    UartControl UartCtrl;				//���ƺ����ص�����ָ��
};

//���ڴ�������,ʹ��MULPLEX��SENSINGBIT��4�����ص���31������
#define CN_UART_FIFO_OVER_ERR    (1<<3) //Ӳ���������������
#define CN_UART_BUF_OVER_ERR     (1<<4) //���ջ������������
#define CN_UART_HARD_COMM_ERR    (1<<5) //����Ӳ��ͨ�Ŵ���

//�����豸���������������uart_ctrl������
#define CN_UART_START                 0   //��������
#define CN_UART_STOP                  1   //ֹͣ����
#define CN_UART_SET_BAUD              2   //����uartBaud.
#define CN_UART_COM_SET               3   //���ô��ڲ���
#define CN_UART_SEND_DATA             4   //��������
#define CN_UART_RECV_DATA             5   //��������
#define CN_UART_COMPLETED_SEND        6   //��ɷ��͹���
#define CN_UART_RX_PAUSE              7   //��ͣ�������ݵ�������
#define CN_UART_RX_RESUME             8   //���¿�ʼ�������ݵ�������
#define CN_UART_RX_OVERTIME           9   //���ý��ճ�ʱʱ�䣬������
#define CN_UART_RECV_SOFT_LEVEL       10  //���ý����������������ˮƽ
#define CN_UART_SEND_SOFT_LEVEL       11  //���ý����������������ˮƽ
#define CN_UART_RECV_HARD_LEVEL       12  //���ý���fifo����ˮƽ
#define CN_UART_SEND_HARD_LEVEL       13  //���÷���fifo����ˮƽ
#define CN_UART_RECV_ERROR_EVTT       14  //���ô��󵯳��¼�����
#define CN_UART_EN_RTS                15  //ʹ��rts����
#define CN_UART_DIS_RTS               16  //��ֹrts����
#define CN_UART_EN_CTS                17  //ʹ��cts����
#define CN_UART_DIS_CTS               18  //��ֹcts����
#define CN_UART_DMA_USED              19  //ʹ��dma����
#define CN_UART_DMA_UNUSED            20  //��ֹdma����
#define CN_UART_HALF_DUPLEX_SEND      21   //��������
#define CN_UART_HALF_DUPLEX_RECV      22   //��������
#define CN_UART_SETTO_ALL_DUPLEX      23  //����Ϊȫ˫����ʽ


ptu32_t UART_AppWrite(struct tagUartCB *UCB,u8* src_buf,u32 len,
        u32 offset,bool_t block_option,u32 timeout);
ptu32_t UART_AppRead(struct tagUartCB *UCB,u8* dst_buf,u32 len,
        u32 offset,u32 timeout);
ptu32_t UART_PortWrite(struct tagUartCB *UCB,u8* buf,u32 len,u32 res);
ptu32_t UART_PortRead(struct tagUartCB *UCB,u8* dst_buf,u32 len,u32 res);
ptu32_t UART_ErrHandle(struct tagUartCB *UCB,u32 ErrNo);
ptu32_t UART_Ctrl(struct tagUartCB *UCB,u32 cmd,ptu32_t data1,ptu32_t data2);
struct tagUartCB *UART_InstallPort(struct tagUartParam *Param);
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


