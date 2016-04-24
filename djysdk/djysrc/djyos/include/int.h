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
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ���ж���صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __INT_H__
#define __INT_H__


#include "arch_feature.h"
#include "cpu_peri_int_line.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  CN_ASYN_SIGNAL     (0)
#define  CN_REAL            (1)

//���жϷ�ʽ
#define  CN_INT_CLEAR_PRE    0   //����ISR֮ǰ��ϵͳ�Զ���
#define  CN_INT_CLEAR_USER   1   //ϵͳ���壬���û���ISR���壬Ĭ�Ϸ�ʽ
#define  CN_INT_CLEAR_POST   2   //����ISR����֮���жϷ���ǰ��ϵͳ�Զ���

//��ʾ���ж���״̬��λͼռucpu_t���͵�����
#define CN_INT_BITS_WORDS   ((CN_INT_LINE_LAST+CN_CPU_BITS)/CN_CPU_BITS)

//�ж������ݽṹ��ÿ�ж�һ��
//��ֲ����
struct tagIntLine
{
    u32 (*ISR)(ufast_t line);
    struct  tagEventECB *sync_event;       //���ڵȴ����жϷ������¼�
    ucpu_t en_counter;          //��ֹ��������,����0ʱ��ʾ�����ж�
    ufast_t int_type;           //1=ʵʱ�ж�,0=�첽�ź�
    ufast_t clear_type;          //Ӧ��ʽ
    bool_t enable_nest;         //true=���ж���Ӧ�ڼ�����Ƕ�ף���Ӳ������������
                                //�ԣ�Ҳ����������йء�����cortex-m3�汾���첽
                                //�źű�����Ϊ������ȼ����Ӷ������첽�źŶ���
                                //����Ƕ�ס�
                                //�ر�ע�⣬ʵʱ�ж��ܹ�������Ƕ���첽�źš�
                                //�ж���Ӧ�����ж��������enable_nest��ֵʹ��
                                //���ֹ�ж��������Ƿ�����Ƕ�ף��������Ӧ�ж�
                                //��Ӳ��û��������ֹ�жϣ�����һ��СС��"����"
                                //���ڸô����ڣ�������Ƕ�׵ġ�����cm3��ʵʱ�ж�
    uint16_t my_evtt_id;
    u32  prio;                  //���ȼ���������ʹ���߽���
};

//�ж��ܿ����ݽṹ.
struct tagIntMasterCtrl
{
    //�ж�������λͼ��0=�첽�źţ�1=ʵʱ�ж�,�����λ���պÿ��������ж�����,��
    //�ж������ݽṹ��int_type��Ա������ͬ��
    ucpu_t  property_bitmap[CN_INT_BITS_WORDS];
    ucpu_t nest_asyn_signal;   //�ж�Ƕ�����,������=0,��һ�ν����ж�=1,���εݼ�
    ucpu_t nest_real;   //�ж�Ƕ�����,������=0,��һ�ν����ж�=1,���εݼ�
    //�ж���ʹ��λͼ,1=ʹ��,0=��ֹ,��ӳ��Ӧ���ж��ߵĿ���״̬,
    //���ܿ���/�첽�źſ��ص�״̬�޹�.
    ucpu_t  enable_bitmap[CN_INT_BITS_WORDS];
//    bool_t  en_trunk;           //1=���ж�ʹ��,  0=���жϽ�ֹ
//    bool_t  en_asyn_signal;         //1=�첽�ź�ʹ��,0=�첽�źŽ�ֹ
    ucpu_t en_trunk_counter;   //ȫ���жϽ�ֹ����,=0��ʾ����ȫ���ж�
    ucpu_t en_asyn_signal_counter; //�첽�źŽ�ֹ����,=0��ʾ�����첽�ź�
};

atom_high_t Int_HighAtomStart(void);
void Int_HighAtomEnd(atom_high_t high);
atom_low_t Int_LowAtomStart(void);
void Int_LowAtomEnd(atom_low_t low);
void Int_SaveTrunk(void);
void Int_RestoreTrunk(void);
bool_t Int_CheckTrunk(void);
void Int_SaveAsynSignal(void);
u32 Int_GetRunLevel(void);
void Int_RestoreAsynSignal(void);
void Int_HalfEnableAsynSignal(void);
bool_t Int_CheckAsynSignal(void);
bool_t Int_SaveAsynLine(ufast_t ufl_line);
bool_t Int_SaveRealLine(ufast_t ufl_line);
bool_t Int_RestoreAsynLine(ufast_t ufl_line);
bool_t Int_RestoreRealLine(ufast_t ufl_line);
bool_t Int_DisableAsynLine(ufast_t ufl_line);
bool_t Int_DisableRealLine(ufast_t ufl_line);
bool_t Int_EnableAsynLine(ufast_t ufl_line);
bool_t Int_EnableRealLine(ufast_t ufl_line);
bool_t Int_CheckLine(ufast_t ufl_line);
bool_t Int_SetClearType(ufast_t ufl_line,ufast_t clear_type);
void Int_IsrConnect(ufast_t ufl_line, u32 (*isr)(ufast_t));
bool_t Int_EvttConnect(ufast_t ufl_line,uint16_t my_evtt_id);
void Int_IsrDisConnect(ufast_t ufl_line);
void Int_EvttDisConnect(ufast_t ufl_line);
void Int_ResetAsynSync(ufast_t ufl_line);
bool_t Int_AsynSignalSync(ufast_t ufl_line);

//���º�����int_hard.c��ʵ��
void Int_ContactAsynSignal(void);
void Int_CutAsynSignal(void);
void Int_ContactTrunk(void);
void Int_CutTrunk(void);
bool_t Int_ContactLine(ufast_t ufl_line);
bool_t Int_CutLine(ufast_t ufl_line);
bool_t Int_ClearLine(ufast_t ufl_line);
bool_t Int_TapLine(ufast_t ufl_line);
bool_t Int_QueryLine(ufast_t ufl_line);
bool_t Int_SettoAsynSignal(ufast_t ufl_line);
bool_t Int_SettoReal(ufast_t ufl_line);
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio);
void Int_Init(void);


#ifdef __cplusplus
}
#endif

#endif //__INT_H__
