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
//�汾��V1.0.1
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_UART_H__
#define __CPU_UART_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

//UTRSTAT״̬λ����
#define UartTXShiftEmpty    (1<<2)  //1:������λ�Ĵ�����,0:�ǿ�
#define UartTxBufEmpty      (1<<1)  //1:���ͻ�������,0:�ǿ�
#define UartRxBufNotEmpty   (1<<0)  //1:���ܻ�����������,0:������
//UFCON FIFO���ƼĴ���
#define cn_tx_fifo_reset    (1<<2)  //д1��λ����fifo,������0
#define cn_rx_fifo_reset    (1<<1)  //д1��λ����fifo,������0
//UFSTAT FIFO״̬�Ĵ���
#define cn_rx_over          (1<<6)  //1:����fifo���,0:����

//ͨ��״̬������λ����
#define CN_SER_RXBUF_BUSY         (1<<0)
#define CN_SER_RXBUF_EMPTY     (1<<1)
#define CN_SER_RECEIVE_OVER    (1<<2)

//�жϿ���λ����2410���жϿ������У�ÿ������ֻ������һ���жϣ������ж�ԭ����ͨ��
//��ѯ SUBSRCPND �Ĵ�����Ӧ��λ���õ��ģ����� INTSUBMSK ����Ӧλ�ɶ����ؽ�ֹ
//����/�����жϡ�
#define BIT_SUB_ERR2    (0x1<<8)
#define BIT_SUB_TXD2    (0x1<<7)
#define BIT_SUB_RXD2    (0x1<<6)
#define BIT_SUB_ERR1    (0x1<<5)
#define BIT_SUB_TXD1    (0x1<<4)
#define BIT_SUB_RXD1    (0x1<<3)
#define BIT_SUB_ERR0    (0x1<<2)
#define BIT_SUB_TXD0    (0x1<<1)
#define BIT_SUB_RXD0    (0x1<<0)
#define BIT_ALL_UART    (0x1ff)

#define SUB_RXD_FLG     0
#define SUB_TXD_FLG     1
#define SUB_ERR_FLG     2
#define BIT_SUB_INT(m,n)   (0x1<<(m*3 + n))
//#define BIT_SUB_INT(m,n)   (0x1<<((m-1)*3 + n))

#define UART_TX_FIFO_SIZE  16  //Ӳ���ķ���fifo���
#define UART_RX_FIFO_SIZE  16  //Ӳ���Ľ���fifo���

typedef struct
{
    vu32 ULCON;     //0x50000000 0x50004000 0x50008000 Line control
    vu32 UCON;      //0x50000004 0x50004004 0x50008004 Control
    vu32 UFCON;     //0x50000008 0x50004008 0x50008008 FIFO control
    vu32 UMCON;     //0x5000000c 0x5000400c 0x5000800c Modem control
    vu32 UTRSTAT;   //0x50000010 0x50004010 0x50008010 Tx/Rx status
    vu32 UERSTAT;   //0x50000014 0x50004014 0x50008014 Rx error status
    vu32 UFSTAT;    //0x50000018 0x50004018 0x50008018 FIFO status
    vu32 UMSTAT;    //0x5000001c 0x5000401c 0x5000801c Modem status

#ifdef __BIG_ENDIAN
    vu8  reserve;
    vu8  reserve1;
    vu8  reserve2;
    vu8  UTXH;      //0x50000023 0x50004023 0x50008023
    vu8  reserve3;
    vu8  reserve4;
    vu8  reserve5;
    vu8  URXH;      //0x50000027 0x50004027 0x50008027

#else //Little Endian
    vu8  UTXH;      //0x50000020 0x50004020 0x50008020
    vu8  reserve;   //UTXH��URXH���ֽڷ��ʼĴ���,reserve��ռλ����.
    vu8  reserve1;
    vu8  reserve2;
    vu8  URXH;      //0x50000024 0x50004024 0x50008024
    vu8  reserve3;
    vu8  reserve4;
    vu8  reserve5;
 #endif
    vu32 UBRDIV;    //0x50000028 0x50004028 0x50008028 Baud rate divisor
}tagUartReg;


#define CN_UART0       0
#define CN_UART1       1
#define CN_UART2       2
#define CN_UART_NUM    3

ptu32_t UART_ModuleInit(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __UART_H__

