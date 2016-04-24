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
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ����: 2010-01-20
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: �޸� exp_esr_tick������������tick�ж��е����첽�źŵ�ʹ�ܺͽ�ֹ
//      ����ʱ������en_asyn_signal_counter�Ȳ���δ��ȷ���ã�������⡣
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#include "stdint.h"
#include "arch_feature.h"
#include "cortexm0.h"
#include "exception.h"

extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�
extern struct tagIntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);

void (*user_systick)(u32 inc_ticks);
void exp_systick_handler(void);
void exp_svc_handler(void);


void Exp_Init(void)
{
    pg_scb_reg->pri12_15 |=0xff000000;    //systick��Ϊ������ȼ�,=�첽�ź�
    pg_scb_reg->pri8_11 |= 0xff000000;    //svc�����ȼ����첽�ź���ͬ��
}
void Exp_ConnectNmi(void (*esr)(void))
{
}

void Exp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

void Exp_EsrTick(void)
{
    if((pg_systick_reg->ctrl & bm_systick_ctrl_tickint) == 0)
        return;
    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal = false;
//    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal++;
    user_systick(1);

    tg_int_global.nest_asyn_signal--;
//    tg_int_global.en_asyn_signal = true;
//    tg_int_global.en_asyn_signal_counter = 0;
    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
}
