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
//����ģ��:��ȫ����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:��ȫ�����Ǳ����ڿ����󾡿��ܿ��ִ�еĲ��ִ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "cpu_peri.h"

const ufast_t tg_IntUsed[] =
{
		CN_INT_LINE_SUPC    ,
		CN_INT_LINE_RSTC    ,
		CN_INT_LINE_RTC     ,
		CN_INT_LINE_RTT     ,
		CN_INT_LINE_WDT     ,
		CN_INT_LINE_PMC     ,
		CN_INT_LINE_EEFC0   ,
		CN_INT_LINE_EEFC1   ,
		CN_INT_LINE_UART0   ,
		CN_INT_LINE_UART1   ,
		CN_INT_LINE_SMC     ,
		CN_INT_LINE_PIOA    ,
		CN_INT_LINE_PIOB    ,
		CN_INT_LINE_PIOC    ,
		CN_INT_LINE_USART0  ,
		CN_INT_LINE_USART1  ,
		CN_INT_LINE_REV0    ,
		CN_INT_LINE_REV1    ,
        CN_INT_LINE_HSMCI	,
        CN_INT_LINE_TWI0	,
        CN_INT_LINE_TWI1	,
        CN_INT_LINE_SPI		,
        CN_INT_LINE_SSC		,
        CN_INT_LINE_TC0		,
        CN_INT_LINE_TC1		,
        CN_INT_LINE_TC2		,
        CN_INT_LINE_TC3		,
        CN_INT_LINE_TC4		,
        CN_INT_LINE_TC5		,
        CN_INT_LINE_ADC		,
        CN_INT_LINE_DACC	,
        CN_INT_LINE_PWM		,
        CN_INT_LINE_CRCU	,
        CN_INT_LINE_ACC		,
        CN_INT_LINE_UDP
};

const ufast_t tg_IntUsedNum = sizeof(tg_IntUsed)/sizeof(ufast_t);
//----����ȫ��IO��--------------------------------------------------------------
//���ܣ���������⣬ȫ�����óɳ�ʼ̬�������ܿ�����Ӧ�Ĺ���ģ�����ж��塣
//��������
//���أ���
//------------------------------------------------------------------------------
void gpio_init(void)
{
}

void critical(void)
{

}


