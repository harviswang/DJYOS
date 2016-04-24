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
//����ģ��: CPU���趨��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����CPU��������Ķ��壬
//      1���������ں���صĹ̶����豸������m3��nvic�ȣ�����m3�ں˾�һ�¡�
//      2���������ں���صĿ�ѡ�䵫�̶��Ķ��壬����m3��mpu��������һ������m3�ں�
//         ��оƬ���У���ֻ�����޵����𣬲����ڱ仯��
//         ��������������Ӧ���ں�Ŀ¼�¶��塣
//      3�������ں���صĿɱ䲿������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "cpu_peri_dma.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_inflash.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_rcc.h"
#include "cpu_peri_rtc.h"
#include "cpu_peri_wdt.h"
#include "cpu_peri_timer.h"

extern struct tagDMA_StReg volatile * const pg_dma1_reg;
extern struct tagDMA_StReg volatile * const pg_dma2_reg;

//dma1�ĸ�ͨ���Ĵ���
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel1_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel2_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel3_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel4_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel5_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel6_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma1_channel7_reg;
//dma2�ĸ�ͨ���Ĵ���
extern struct tagDMA_StChannelReg volatile * const pg_dma2_channel1_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma2_channel2_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma2_channel3_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma2_channel4_reg;
extern struct tagDMA_StChannelReg volatile * const pg_dma2_channel5_reg;

//gpio��ؼĴ�������
extern struct tagGPIO_StReg volatile * const pg_gpio_rega;
extern struct tagGPIO_StReg volatile * const pg_gpio_regb;
extern struct tagGPIO_StReg volatile * const pg_gpio_regc;
extern struct tagGPIO_StReg volatile * const pg_gpio_regd;
extern struct tagGPIO_StReg volatile * const pg_gpio_rege;
extern struct tagGPIO_StReg volatile * const pg_gpio_regf;
extern struct tagGPIO_StReg volatile * const pg_gpio_regg;
extern struct tagGPIO_StReg volatile * const pg_gpio_reg[];
extern struct tagAFIO_StReg volatile * const pg_afio_reg;

//������ؼĴ���
extern struct st_usart_reg volatile * const pg_uart1_reg;
extern struct st_usart_reg volatile * const pg_uart2_reg;
extern struct st_usart_reg volatile * const pg_uart3_reg;
extern struct st_usart_reg volatile * const pg_uart4_reg;
extern struct st_usart_reg volatile * const pg_uart5_reg;

//rcc(��λ��ʱ�ӿ��ƼĴ���)
extern struct st_rcc_reg volatile * const pg_rcc_reg;

//Ƭ��flash���ƼĴ���
extern struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg;
extern struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg;

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__

