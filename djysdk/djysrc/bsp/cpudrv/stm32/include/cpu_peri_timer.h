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
// ģ������: ��ʱ��Ӳ���ӿڲ����ݶ����Լ��ӿ�����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:39:26 PM/Apr 1, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef __CPU_PERI_TIMER_H__
#define __CPU_PERI_TIMER_H__

#include "stdint.h"

//timerλ��bo_Ϊλƫ�ƣ�1λ����λ����ַ��bb_ǰ׺����λ�����룬bm_ǰ׺
//uart1 sr�Ĵ���λ����
#define bb_tim1_cr1_base   (0x42000000 + 0x13800*32)           //λ����ַ
#define bb_tim1_cr1_cen    (*(vu32*)(bb_tim1_cr1_base+4*0))
#define bb_tim1_cr1_udis   (*(vu32*)(bb_tim1_cr1_base+4*1))
#define bb_tim1_cr1_urs    (*(vu32*)(bb_tim1_cr1_base+4*2))
#define bb_tim1_cr1_opm    (*(vu32*)(bb_tim1_cr1_base+4*3))
#define bb_tim1_cr1_dir    (*(vu32*)(bb_tim1_cr1_base+4*4))

//tim1 at 0x40012c00
//tim2 at 0x40000000
//tim3 at 0x40000400
//tim4 at 0x40000800
//tim5 at 0x40000c00
//tim6 at 0x40001000
//tim7 at 0x40001400
//tim8 at 0x40013400
typedef struct
{//             offset
    u32 CR1;    //0x00
    u32 CR2;    //0x04
    u32 SMCR;   //0x08
    u32 DIER;   //0x0c
    u32 SR;     //0x10
    u32 EGR;    //0x14
    u32 CCMR1;  //0x18
    u32 CCMR2;  //0x1c
    u32 CCER;   //0x20
    u32 CNT;    //0x24
    u32 PSC;    //0x28
    u32 ARR;    //0x2c
    u32 RCR;    //0x30
    u32 CCR1;   //0x34
    u32 CCR2;   //0x38
    u32 CCR3;   //0x3c
    u32 CCR4;   //0x40
    u32 BDTR;   //0x44
    u32 DCR;    //0x48
    u32 DMAR;   //0x4c
}tagTimerReg;

#define TIM_CR1_CEN_MASK        (1<<0)
#define TIM_CR1_OPM_MASK        (1<<3)
#define TIM_CR1_ARPE_MASK       (1<<7)

#define TIM_DIER_UIE_MASK       (1<<0)
#define TIM_SR_UIF_MASK         (1<<0)

#endif /* __CPU_PERI_TIMER_H__ */
