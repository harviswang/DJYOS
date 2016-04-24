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
//����:  lst
//�汾��V1.0.0
//����˵��:
//�޶���ʷ:
//1. ����: 2013-05-29
//   ����:  lst.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_PERI_ID_SUPC                 (0x00)
#define CN_PERI_ID_RSTC                 (0x01)
#define CN_PERI_ID_RTC         	        (0x02)
#define CN_PERI_ID_RTT                  (0x03)
#define CN_PERI_ID_WDT                  (0x04)
#define CN_PERI_ID_PMC                  (0x05)
#define CN_PERI_ID_EEFC0                (0x06)
#define CN_PERI_ID_EEFC1                (0x07)
#define CN_PERI_ID_UART0                (0x08)
#define CN_PERI_ID_UART1                (0x09)
#define CN_PERI_ID_SMC     			    (0x0A)
#define CN_PERI_ID_PIOA			        (0x0B)
#define CN_PERI_ID_PIOB			        (0x0C)
#define CN_PERI_ID_PIOC			        (0x0D)
#define CN_PERI_ID_USART0			    (0x0E)
#define CN_PERI_ID_USART1			    (0x0F)
#define CN_PERI_ID_REV0                 (0X10)
#define CN_PERI_ID_REV1                 (0X11)
#define CN_PERI_ID_HSMCI				(0x12)
#define CN_PERI_ID_TWI0				    (0x13)
#define CN_PERI_ID_TWI1			        (0x14)
#define CN_PERI_ID_SPI					(0x15)
#define CN_PERI_ID_SSC					(0x16)
#define CN_PERI_ID_TC0					(0x17)
#define CN_PERI_ID_TC1					(0x18)
#define CN_PERI_ID_TC2					(0x19)
#define CN_PERI_ID_TC3					(0x1a)
#define CN_PERI_ID_TC4					(0x1b)
#define CN_PERI_ID_TC5					(0x1c)
#define CN_PERI_ID_ADC				    (0x1d)
#define CN_PERI_ID_DACC				    (0x1e)
#define CN_PERI_ID_PWM				    (0x1f)
#define CN_PERI_ID_CRCU				    (0x20)
#define CN_PERI_ID_ACC				    (0x21)
#define CN_PERI_ID_UDP				    (0x22)


#include "os.h"
#include "sam4s16c.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_pmc.h"
#include "cpu_peri_efc.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_pio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_tc.h"

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__
