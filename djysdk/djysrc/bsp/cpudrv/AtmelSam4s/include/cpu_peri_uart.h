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

#include "os.h"
#include "cpu_peri.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct         //offset
{
    vu32 UART_CR;       //���ƼĴ���
    vu32 UART_MR;       //ģʽ�Ĵ���
    vu32 UART_IER;      //�ж�ʹ�ܼĴ���
    vu32 UART_IDR;      //�жϳ��ܼĴ���
    vu32 UART_IMR;      //�ж���Ļ�Ĵ���
    vu32 UART_SR;
    vu32 UART_RHR;
    vu32 UART_THR;
    vu32 UART_BRGR;
    vu32 Reserved1[55];
    vu32 UART_RPR;      /**< \brief (Uart Offset: 0x100) Receive Pointer Register */
    vu32 UART_RCR;      /**< \brief (Uart Offset: 0x104) Receive Counter Register */
    vu32 UART_TPR;      /**< \brief (Uart Offset: 0x108) Transmit Pointer Register */
    vu32 UART_TCR;      /**< \brief (Uart Offset: 0x10C) Transmit Counter Register */
    vu32 UART_RNPR;     /**< \brief (Uart Offset: 0x110) Receive Next Pointer Register */
    vu32 UART_RNCR;     /**< \brief (Uart Offset: 0x114) Receive Next Counter Register */
    vu32 UART_TNPR;     /**< \brief (Uart Offset: 0x118) Transmit Next Pointer Register */
    vu32 UART_TNCR;     /**< \brief (Uart Offset: 0x11C) Transmit Next Counter Register */
    vu32 UART_PTCR;     /**< \brief (Uart Offset: 0x120) Transfer Control Register */
    vu32 UART_PTSR;
}tagUartReg;

#define cn_dma_unused   0
#define cn_dma_used     1

#define CN_UART0        0
#define CN_UART1        1
#define CN_USART0       2
#define CN_USART1       3
#define CN_UART_MAX     3
#define CN_UART_NUM     4

ptu32_t UART_ModuleInit(ptu32_t para);//���ڳ�ʼ��
u32 UART_SendServiceDirectly(char *str);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


