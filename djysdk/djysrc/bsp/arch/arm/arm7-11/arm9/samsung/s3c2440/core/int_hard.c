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
//�汾��V1.0.0
//�ļ�����: �ж�ģ����Ӳ����صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ԭint.c�а�Ӳ���޹ش����������
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "cpu_peri.h"
#include "int.h"
#include "int_hard.h"
#include "critical.h"
#include "djyos.h"

extern void (* engine_irq)(ufast_t ufl_line);

extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�
extern struct tagIntLine *tg_pIntSrcTable;     //��Int_Init�����з����ڴ�
extern struct tagIntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);
void __Djy_EventReady(struct  tagEventECB *event_ready);

//�����ж�������Σ�һ����ԣ��쳣��������쳣������ת���͸ö�һͬ�����׵�ַ
u32 u32g_vect_table[CN_INT_LINE_LAST+1] __attribute__((section(".table.vectors")));

//----����߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ�����ǰ���ж�״̬��Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ��
//      ��ϵĲ�����
//      Int_HighAtomStart--int_high_atom_end��������ʹ�ã�
//      ��������������"Int_SaveTrunk--Int_RestoreTrunk"�Ŀ��ٰ汾���ڱ�������
//      ס�Ĵ�����ڣ����������"Int_SaveTrunk--Int_RestoreTrunk",��������
//      ���ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_high_t Int_HighAtomStart(void)
{
    volatile atom_high_t ret;
    __asm__ volatile(
            ".equ   cn_noint, 0xc0 ;"
            "mrs    %0, cpsr \n\t"
            "orr    r0, %0, #cn_noint \n\t"
            "msr    cpsr_c, r0 \n\t"
            "mov    r0, %0 \n\t"
            : "=r" (ret)
            :
            );
    return ret;
}

//----�뿪�߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ�����ǰ���ж�״̬��������Ӧ��int_high_atom_start��������
//      Int_HighAtomStart--int_high_atom_end��������ʹ�ã�
//      ��������������"Int_SaveTrunk--Int_RestoreTrunk"�Ŀ��ٰ汾���ڱ�������
//      ס�Ĵ�����ڣ����������"Int_SaveTrunk--Int_RestoreTrunk",��������
//      ���ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//������high��int_high_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_HighAtomEnd(atom_high_t high)
{
    __asm__(
            "msr cpsr_c,%0 \n\t" : :"r"(high)
    );
}

//----����ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ���ȡ��ǰ�첽�źſ���״̬��Ȼ���ֹ�첽�źš��ͼ���ԭ�Ӳ�����������ֹ�ٽ�
//      ���ݵĲ������ʣ������ܻᱻʵʱ�жϴ�ϡ�
//      Int_LowAtomStart---int_low_atom_end��������ʹ�ã���������������
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"�Ŀ��ٰ汾���ڱ�������ס
//      �Ĵ�����ڣ����������int_restore_asyn_signal��int_save_asyn_signal�е�
//      �κ�һ��������������ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_low_t Int_LowAtomStart(void)
{
    register ucpu_t msk_lsb;
    atom_low_t low;
    low = pg_int_reg->INTMSK;
    msk_lsb = (~tg_int_global.property_bitmap[0]) & cn_int_msk_all_line;
    pg_int_reg->INTMSK |= msk_lsb;
    return low;
}

//----�뿪�ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ�����ǰ�첽�ź����״̬��������Ӧ��int_low_atom_start�������ס�
//      Int_LowAtomStart---int_low_atom_end��������ʹ�ã���������������
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"�Ŀ��ٰ汾���ڱ�������ס
//      �Ĵ�����ڣ����������int_restore_asyn_signal��int_save_asyn_signal�е�
//      �κ�һ��������������ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//������low��int_low_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_LowAtomEnd(atom_low_t low)
{
    pg_int_reg->INTMSK &= ~((~tg_int_global.property_bitmap[0]) & (~low));
}

//----��ͨ�첽�źſ���---------------------------------------------------------
//���ܣ���ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�ѷ����������ж�����rINTMSK��Ӧλ��0.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactAsynSignal(void)
{
    //INTMSK���첽�ź����ж��߱�ʹ�ܵ�λ����0
    pg_int_reg->INTMSK &= ~((~tg_int_global.property_bitmap[0])
                         & tg_int_global.enable_bitmap[0]);
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//���ܣ��Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�������첽�ź�����rINTMSK��Ӧλ��1.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutAsynSignal(void)
{
    register ucpu_t msk;
#if(CN_CFG_REG_ATOM == 0)
    atom_high_t high;
    high = Int_HighAtomStart();
#endif      //for #if(CN_CFG_REG_ATOM == 0)
    msk = (~tg_int_global.property_bitmap[0]) & cn_int_msk_all_line;
    pg_int_reg->INTMSK |= msk;
#if(CN_CFG_REG_ATOM == 0)
    Int_HighAtomEnd(high);
#endif      //for #if(CN_CFG_REG_ATOM == 0)
}

//----��ͨ���жϿ���-----------------------------------------------------------
//���ܣ���ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactTrunk(void)
{
    __int_enable_irq();
}

//----�Ͽ����жϿ���---------------------------------------------------------
//���ܣ��Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
    __int_disable_irq();
}

//----��ͨ�����ж��߿���-------------------------------------------------------
//���ܣ���ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
    {//������ж��������첽�ź�,���첽�źſ�������,������ж���
        if(tg_int_global.en_asyn_signal_counter == 0)
            pg_int_reg->INTMSK &= ~(1<<ufl_line);
    }else  //������ж�����ʵʱ�ж�,�ж����ж�������,ֱ��������ж���
        pg_int_reg->INTMSK &= ~(1<<ufl_line);
    return  true;
}

//----�Ͽ������ж��߿���-------------------------------------------------------
//���ܣ��Ͽ������ж��߿��أ��������жϺ��첽�źſ���״̬��Σ����ж��߱���ֹ
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_CutLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    pg_int_reg->INTMSK |=1<<ufl_line;
    return true;
}

//----Ӧ���жϣ������Ӧ�ж��ߵ��жϹ���״̬-----------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ��������ǰ��������Ӧͬһ�ж���
//      �ĺ����жϣ�����󣬲ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//      �����첽�ź��޹�
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_ClearLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    pg_int_reg->SRCPND = 1<<ufl_line;
    pg_int_reg->INTPND = 1<<ufl_line;
    return true;
}

//----�����ж�-----------------------------------------------------------------
//����: ����һ���ж�.����жϱ�������,��������Ӱ��.����������ж��߱�����Ϊʵʱ
//      �жϻ����첽�ź��޹�
//������ufast ufl_line�����������ж��ߺ�
//���أ������Ӧ���ж���Ӳ�����ṩ����������жϹ���,���� false,���򷵻� true
//��ע: ������ʵ��������Ӳ��,��ЩӲ��ϵͳ��֧�ִ˹���.
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_TapLine(ufast_t ufl_line)
{
    return false;    //2440��֧���������Ӳ���жϵ�����.
}

//----Ӧ��ȫ���жϣ����ȫ���ж��ߵ��жϹ���״̬-------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ���
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
void __Int_ClearAllLine(void)
{
    pg_int_reg->SRCPND = cn_int_msk_all_line;
    pg_int_reg->INTPND = cn_int_msk_all_line;;
}

//----��ѯ�ж�������״̬-------------------------------------------------------
//���ܣ���ѯ�������Ӧ�ж���״̬�������ڲ�ѯʽ�жϳ���
//������ufl_line������ѯ���ж���
//���أ����жϹ��𣬷���true�����򷵻�false
//��ע: ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
//      ����������ֲ�ؼ�����
//-----------------------------------------------------------------------------
bool_t Int_QueryLine(ufast_t ufl_line)
{
    ucpu_t  ucl_msk;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    ucl_msk=1<<ufl_line;
    if(pg_int_reg->SRCPND & ucl_msk)
    {
        pg_int_reg->SRCPND = ucl_msk;
        pg_int_reg->INTPND = ucl_msk;
        return true;
    }else
        return false;
}

//----��ָ���ж�������Ϊ�첽�ź�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊ�첽�ź�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_ASYN_SIGNAL;   //�ж�������

    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                    &= ~(1<<(ufl_line % CN_CPU_BITS));   //����λͼ
    return true;
}

//----��ָ���ж�������Ϊʵʱ�ж�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊʵʱ�ж�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoReal(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event != NULL)
        return false;     //���߳��ڵȴ�����жϣ�������Ϊʵʱ�ж�
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_REAL;    //�ж�������
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = false;   //��ʵ�ֲ�֧��ʵʱ�ж�Ƕ��
    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
            |= 1<<(ufl_line % CN_CPU_BITS);   //����λͼ
    return true;
}

//----ʹ���ж�Ƕ��-------------------------------------------------------------
//����: ʹ��һ���ж�Դ����ռ��ʹ�ܺ���Ӧ���жϷ����ڼ䣬���ܻᱻ����ж���ռ��
//      ����������ʵʱ�жϻ����첽�źţ�������ʵ���йأ���ֲ��Ӧ�ø���Ӳ������
//      ��Ӧ������ȷ���Ƿ�����Ƕ�ס�
//      Ƕ�׷�����ʵʱ�ж�֮������첽�ź�֮�䡣ʵʱ�ж���Զ���Դ���첽�źţ���
//      ����Ƕ�ס�
//      ��������ϵ�е�ARMоƬ�жϹ��������ص㣬����ʵʱ�жϵĴ��������ַ�ʽ:
//      1����fiqʵ��ʵʱ�жϣ�INTOFFSET1��INTOFFSET2�Ĵ����ṩ�˵�ǰ���ڷ����
//         �жϺţ����������Ĵ�����fiq����Ч�����ʵʱ�ж���fiqʵ�֣�����Ҫ��λ
//         ��鱻��Ϊʵʱ�жϵ��жϺ���INTPND1��INTPND1�Ĵ����е�ֵ������ȷ����
//         ������жϺţ�������̿��ܷǳ���������������ܹ�ʵ��ʵʱ�ж�Ƕ�ף���
//         ȴ��ȫʧȥ��ʵʱ�жϵ���Ƴ��ԣ�djyos for s3c2416����ȡ���ַ�ʽ��
//      2����һ����������ǣ�ϵͳֻ��Ҫһ��ʵʱ�жϣ�����ȫ����Ϊ�첽�źţ�����
//         �������fiqʵ��ʵʱ�ж�����Ϊ����ģ���Ȼֻ��һ���жϣ�Ƕ��Ҳ���޴�
//         ̸���ˡ�
//      3��ʵʱ�жϺ��첽�źŶ�����irq��ʽʵ�֣�cpsr��Iλ����trunk���أ��첽�ź�
//         û�ж������أ���ͨ�������е������첽�źŵ��жϺŵĿ���ȫ���ص���ģ��
//         �첽�źſ��صġ���������ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�Ϊʲô��?arm�����
//         Ϊ��ֻҪIλΪ0���µ��жϾͿ�����ռ���ڷ�����жϣ�Ҳ����ζ�ţ��첽��
//         ����ռʵʱ�жϣ���Ӳ����������ġ�ʵʱ�ж�Ҫʵ��Ƕ�ף����ȹص��첽��
//         �ţ�������I=0�����ʵʱ�ж���Ӧ�����첽�źű��ص�֮ǰ�����������첽
//         �źŷ����Ļ�����irq����ͻᷢ�������ص��첽�ź�Ҳû�ã����ж�һ����
//         �������첽�źžͻ���ռʵʱ�жϡ�������ռ��������"����"���ң�������
//         ���������⣬����ʹϵͳ�����������ռʵʱ�жϵ��첽�źţ������ڼ䷢��
//         ���߳��л�����fiq�����ջ������pg_event_running��ջ����ˣ�djyos for
//         2416�汾��֧��ʵʱ�ж�Ƕ�ס�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t Int_EnableNest(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                & (1<<(ufl_line % CN_CPU_BITS)))
        return false;       //��ʵ�ֲ�֧��ʵʱ�ж�Ƕ��
    else
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = true;
    return true;
}

//----��ֹ�ж�Ƕ��-------------------------------------------------------------
//����: ���ĳ�жϺŵ��ж�Ƕ��ʹ�ܡ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t Int_DisableNest(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = false;
    return true;
}

//----�趨Ƕ�����ȼ�-----------------------------------------------------------
//����: �趨ָ���ж��ߵ�Ƕ�����ȼ�����������������Ӳ�����ܡ����Ӳ����֧�֣���
//      ���ֿպ�����
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ���
//ע: ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio)
{
    return false;
}

//----��ʼ���ж�Ӳ����ز���---------------------------------------------------
//����: �����
//����: ��
//����: ��
//ע: ��ֲ���У���Ӳ����أ�Ҳ����������й�
//-----------------------------------------------------------------------------
void __Int_InitHard(void)
{
    int_disable_arm_int();     //��ֹ�ж�

    pg_int_reg->INTMSK = cn_int_msk_all_line;
    //�жϹ�������Fλ���ǽ�ֹ,2440���жϹ�������ȱ�ݣ�����ֻ����һ
    //���ж�����Ϊfiq������fiq����ж��ߵĹ���ʹ��fiqʵ�ʱ�irq�����ʲ�ʹ��fiq��
    //�����첽�źŻ���ʵʩ�жϣ���ʹ��irqʵ�֡�
    pg_int_reg->INTMOD=0;    //���������ж���ΪIRQ�ж�
    engine_irq = __Int_EngineAll;
    // irq_bottom = __Int_EngineAll;
}
//----��ʼ���ж�---------------------------------------------------------------
//���ܣ���ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//      2.�첽�źű��ֽ�ֹ,��������������������д�.
//      3.���ж�����
//      �û���ʼ������Ӧ���������¹���:
//      1.ϵͳ��ʼʱ���Ѿ���ֹ�����첽�ź�,�û���ʼ��ʱ���뵣���첽�źŷ���.
//      2.��ʼ�������������Ҫ�������ж�/ʵʱ�ж�/�첽�ź�,Ӧ�óɶ�ʹ��.��ֹʹ
//        �첽�ź�ʵ�ʴ�������״̬(���첽�����жϿ���ͬʱ����).
//      3.���Բ����ж���,�������ӡ�������ֹ��,��Ӧ�óɶ�ʹ��.
//      4.����ʹ��save/restore������,��Ҫʹ��enable/disable������.
//��������
//���أ���
//-----------------------------------------------------------------------------
void Int_Init(void)
{
    ufast_t ufl_line;
    tg_pIntSrcTable = malloc(tg_IntUsedNum * sizeof(struct tagIntLine));
    if(tg_pIntSrcTable == NULL)
        return;
    __Int_InitHard();
    __Int_ClearAllLine();
    for(ufl_line=0;ufl_line <= CN_INT_LINE_LAST;ufl_line++)
    {
        tg_int_lookup_table[ufl_line] = (ufast_t)CN_LIMIT_UFAST;
    }
    for(ufl_line = 0; ufl_line < tg_IntUsedNum; ufl_line++)
    {
        tg_int_lookup_table[tg_IntUsed[ufl_line]] = ufl_line;
        tg_pIntSrcTable[ufl_line].en_counter = 1;               //��ֹ�ж�,����Ϊ1
        tg_pIntSrcTable[ufl_line].int_type = CN_ASYN_SIGNAL;    //��Ϊ�첽�ź�
        tg_pIntSrcTable[ufl_line].clear_type = CN_INT_CLEAR_PRE;//��Ϊ����ISRǰӦ��
        //�����жϺ���ָ��ָ��պ���
        tg_pIntSrcTable[ufl_line].ISR = (u32 (*)(ufast_t))NULL_func;
        tg_pIntSrcTable[ufl_line].sync_event = NULL;                //ͬ���¼���
        tg_pIntSrcTable[ufl_line].my_evtt_id = CN_EVTT_ID_INVALID;  //�������¼�
        tg_pIntSrcTable[ufl_line].enable_nest = false;
    }

    for(ufl_line=0; ufl_line < CN_INT_BITS_WORDS; ufl_line++)
    {
        //����λͼ����,ȫ����Ϊ�첽�źŷ�ʽ
        tg_int_global.property_bitmap[ufl_line] = 0;
        //�ж�ʹ��λͼ��0,ȫ�����ڽ�ֹ״̬
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }
    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.nest_real = 0;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;   //�첽�źż���
    Int_CutAsynSignal();
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;   //���жϼ���
    Int_ContactTrunk();                //��ͨ���жϿ���
}

//----���ж�����---------------------------------------------------------------
//���ܣ���Щϵͳ�����ж�����������������ʵʱ�жϻ����첽�źŵģ����߲�ϣ���ڻ�
//      ��׶�ʹ�ù��ڸ��ӵĴ���ģ�����2416��2440�ȣ����ڻ��׶λ�ȡ�жϺź�
//      ֱ�ӵ��ñ��������ɱ���������������첽�ź������ʵʱ�ж����档
//      ��cortex-m3��omapl138���������ж��������־Ϳ�������ʵʱ�жϻ����첽��
//      �ţ��������ṩ�������������ڻ�ಿ��ֱ�ӵ�����Ӧ�����档
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------

void __Int_EngineAll(ufast_t ufl_line)
{
    if(tg_pIntSrcTable[ufl_line].int_type == CN_REAL)
        __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
    else
        __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
}

//----ʵʱ�ж�����-------------------------------------------------------------
//���ܣ���Ӧʵʱ�жϣ������жϺŵ����û�ISR
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineReal(ufast_t ufl_line)
{
    tg_int_global.nest_real++;

    //��if�������ֲ���У���cpu���жϹ������ļ��������й�:
    //1���첽�ź��Ƿ��ж����Ŀ��أ�����cortex-m3��omapl138�����еģ�2440��2416��
    //   2410����û�еġ����û�ж������أ����ڴ����ж�ǰ��ϵ��첽�ź��߿���
    //2���첽�źź�ʵʱ�ж϶����ڿ���״̬������£��첽�ź��Ƿ������ռʵʱ�жϡ�
    //   �������ʵ�����ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
    //3��ʵʱ�ж���Ӧ���Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ�ֲ�֧��ʵʱ�ж�Ƕ�ף�������ע�͵�
//    if(tg_pIntSrcTable[ufl_line].enable_nest == true)
//    {
//        Int_DisableRealLine(ufl_line);    //����ֹ�����ܻ��γ�����ѭ��
//        Int_ContactTrunk();
//    }
    if(tg_pIntSrcTable[ufl_line].ISR != NULL)
        tg_pIntSrcTable[ufl_line].ISR(ufl_line);  //�����û��жϺ���

//    if(tg_pIntSrcTable[ufl_line].enable_nest == true)  //֧��Ƕ�׵����
//    {
//        Int_CutTrunk();
//        Int_EnableRealLine(ufl_line);
//    }

    //�����ʵ��֧��ʵʱ�ж�Ƕ�ף���������4��
    tg_int_global.nest_real--;

}

//----�첽�¼��ж�����---------------------------------------------------------
//���ܣ���Ӧ�첽�źţ������жϺŵ����û�ISR����󵯳��ж��߿��ƿ��my_evtt_id
//      ��Աָ�����¼����ͣ�����ڷ���ǰ�鿴�Ƿ���Ҫ���������л�������Ҫ���л�
//      ֮��
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineAsynSignal(ufast_t ufl_line)
{
    struct tagEventECB *event;
    u32 isr_result;
    tg_int_global.en_asyn_signal_counter = 1;    //�޶����첽�źſ��صģ������
    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.nest_asyn_signal++;
    if(tg_pIntSrcTable[ufl_line].enable_nest == true)  //����Ƕ�׵����
    {
        Int_DisableAsynLine(ufl_line);  //����ֹ�����ܻ��γ�����ѭ��
        Int_ContactAsynSignal();
    }
    else
        Int_CutAsynSignal();
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_PRE)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    Int_ContactTrunk();
    if(tg_pIntSrcTable[ufl_line].ISR != NULL)
        isr_result = tg_pIntSrcTable[ufl_line].ISR(ufl_line);
    else
    {
        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    }
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_POST)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    Int_CutAsynSignal();
    Int_EnableAsynLine(ufl_line);

    event = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
    {
        event->event_result = isr_result;
        __Djy_EventReady(event);   //�Ѹ��¼��ŵ�ready����
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event = NULL;   //���ͬ��
    }
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id != CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id,
                        NULL,0,(ptu32_t)isr_result, (ptu32_t)ufl_line,0);
    }
    tg_int_global.nest_asyn_signal--;
    if(tg_int_global.nest_asyn_signal == 0)
    {
//        tg_int_global.en_asyn_signal = true;
        tg_int_global.en_asyn_signal_counter = 0; //�޶����첽�źſ��صģ������
        if(g_ptEventReady != g_ptEventRunning)
            __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
        g_bScheduleEnable = true;
    }
    Int_CutTrunk();
    Int_ContactAsynSignal();
}

