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

#ifndef __CPU_PERI_ADC_H__
#define __CPU_PERI_ADC_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

// ADCCON�Ĵ���
#define PRESCALE_DIS   (0 << 14)
#define PRESCALE_EN    (1 << 14)
#define PRSCVL(x)      ((x) << 6)
#define ADC_INPUT(x)   ((x) << 3)
#define ADC_START      (1 << 0)
#define ADC_ENDCVT     (1 << 15)

// ADCTSC�Ĵ���
#define UD_SEN         (1 << 8)
#define DOWN_INT       (UD_SEN*0)
#define UP_INT         (UD_SEN*1)
#define YM_SEN         (1 << 7)
#define YM_HIZ         (YM_SEN*0)
#define YM_GND         (YM_SEN*1)
#define YP_SEN         (1 << 6)
#define YP_EXTVLT      (YP_SEN*0)
#define YP_AIN         (YP_SEN*1)
#define XM_SEN         (1 << 5)
#define XM_HIZ         (XM_SEN*0)
#define XM_GND         (XM_SEN*1)
#define XP_SEN         (1 << 4)
#define XP_EXTVLT      (XP_SEN*0)
#define XP_AIN         (XP_SEN*1)
#define XP_PULL_UP     (1 << 3)
#define XP_PULL_UP_EN  (XP_PULL_UP*0)
#define XP_PULL_UP_DIS (XP_PULL_UP*1)
#define AUTO_PST       (1 << 2)
#define CONVERT_MAN    (AUTO_PST*0)
#define CONVERT_AUTO   (AUTO_PST*1)
#define XP_PST(x)      (x << 0)

#define NOP_MODE 0
#define X_AXIS_MODE 1
#define Y_AXIS_MODE 2
#define WAIT_INT_MODE 3


#define ADCPRS 39

#define BIT_ADC        (0x1<<31)
#define BIT_SUB_ADC_S  (0x1<<10)
#define BIT_SUB_TC     (0x1<<9)

struct adc_reg
{
        vu32 ADCCON;   //0x58000000
        vu32 ADCTSC;   //0x58000004
        vu32 ADCDLY;   //0x58000008
        vu32 ADCDAT0;  //0x5800000c
        vu32 ADCDAT1;  //0x58000010
};


//ADC״̬���ƽṹ
struct adc_port_cb
{
   u32 location_x;    // X ����ֵ
   u32 location_y;    // Y ����ֵ
   u8  isdown;        // ̧�𣬻��ǰ��£�����Ϊ1
   u8  touch_over;    // �Ƿ������
};

ptu32_t module_init_adc(ptu32_t para);

void  ClearPending(u32 bit);

#ifdef __cplusplus
}
#endif

#endif /* __CPU_PERI_ADC_H__ */
