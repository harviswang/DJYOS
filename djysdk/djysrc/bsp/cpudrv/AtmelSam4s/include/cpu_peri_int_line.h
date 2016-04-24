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
//����ģ��:�ж�ģ��
//����:lst
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-01-20
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __int_line_h__
#define __int_line_h__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_SUPC                (0x00)
#define CN_INT_LINE_RSTC                (0x01)
#define CN_INT_LINE_RTC         	    (0x02)
#define CN_INT_LINE_RTT                 (0x03)
#define CN_INT_LINE_WDT                 (0x04)
#define CN_INT_LINE_PMC                 (0x05)
#define CN_INT_LINE_EEFC0               (0x06)
#define CN_INT_LINE_EEFC1               (0x07)
#define CN_INT_LINE_UART0               (0x08)
#define CN_INT_LINE_UART1               (0x09)
#define CN_INT_LINE_SMC     			(0x0A)
#define CN_INT_LINE_PIOA			    (0x0B)
#define CN_INT_LINE_PIOB			    (0x0C)
#define CN_INT_LINE_PIOC			    (0x0D)
#define CN_INT_LINE_USART0			    (0x0E)
#define CN_INT_LINE_USART1			    (0x0F)
#define CN_INT_LINE_REV0                (0X10)
#define CN_INT_LINE_REV1                (0X11)
#define CN_INT_LINE_HSMCI				(0x12)
#define CN_INT_LINE_TWI0				(0x13)
#define CN_INT_LINE_TWI1			    (0x14)
#define CN_INT_LINE_SPI					(0x15)
#define CN_INT_LINE_SSC					(0x16)
#define CN_INT_LINE_TC0					(0x17)
#define CN_INT_LINE_TC1					(0x18)
#define CN_INT_LINE_TC2					(0x19)
#define CN_INT_LINE_TC3					(0x1a)
#define CN_INT_LINE_TC4					(0x1b)
#define CN_INT_LINE_TC5					(0x1c)
#define CN_INT_LINE_ADC				    (0x1d)
#define CN_INT_LINE_DACC				(0x1e)
#define CN_INT_LINE_PWM				    (0x1f)
#define CN_INT_LINE_CRCU				(0x20)
#define CN_INT_LINE_ACC				    (0x21)
#define CN_INT_LINE_UDP				    (0x22)

#define CN_INT_LINE_LAST                (0x22)

#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

