/*========================================================*
 * �ļ�ģ��˵��:                                                 *
 * omapl138���ж�Ӳ������ļ�����ֲ�Զ����߲���ϵͳ����ӵ�а�Ȩ
 * ��S3C2440�ж�Ӳ������ļ������omapl138����һЩ�޸�
 * �ļ��汾: v1.00                                               *
 * ������Ա: lst                              *
 * ����ʱ��: 2011.06.01                                          *
 * Copyright(c) 2011-2011  ��������������޹�˾             *
 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):                               *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>              *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/
//������ԭ���߰�Ȩ����
//----------------------------------------------------
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
//�ļ�����: �ж�ģ����Ӳ����صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ��ԭint.c�а�Ӳ���޹ش����������
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "critical.h"

extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�
extern struct tagIntLine *tg_pIntSrcTable;     //��Int_Init�����з����ڴ�
extern struct tagIntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);
void __Djy_EventReady(struct  tagEventECB *event_ready);

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
    atom_high_t result;
    result = pg_int_reg->GER;
    pg_int_reg->GER = 0;
    return result;
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
    pg_int_reg->GER = high;
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
    atom_low_t result;
    result = pg_int_reg->HIER;
    pg_int_reg->HIER &= ~((u32)reg_hier_irq_bit);
    return result;
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
    pg_int_reg->HIER = low;
}

//----��ͨ�첽�źſ���---------------------------------------------------------
//���ܣ���ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.L138���ڵ�һ�����.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactAsynSignal(void)
{
    pg_int_reg->HIER |= (u32)reg_hier_irq_bit;
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//���ܣ��Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.L138���ڵ�һ�����.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutAsynSignal(void)
{
    pg_int_reg->HIER &= ~((u32)reg_hier_irq_bit);
}

//----��ͨ���жϿ���-----------------------------------------------------------
//���ܣ���ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactTrunk(void)
{
    pg_int_reg->GER = 1;
}

//----�Ͽ����жϿ���---------------------------------------------------------
//���ܣ��Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
    pg_int_reg->GER = 0;
}

//----��ͨ�����ж��߿���-------------------------------------------------------
//���ܣ���ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    byte_offset = ufl_line/32;
    bit_offset = ufl_line%32;
    pg_int_reg->ESR[byte_offset] = ((u32)1)<<bit_offset;
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
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    byte_offset = ufl_line/32;
    bit_offset = ufl_line%32;
    pg_int_reg->ECR[byte_offset] = ((u32)1)<<bit_offset;
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
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    byte_offset = ufl_line/32;
    bit_offset = ufl_line%32;
    pg_int_reg->SECR[byte_offset] = ((u32)1)<<bit_offset;
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
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    byte_offset = ufl_line/32;
    bit_offset = ufl_line%32;
    pg_int_reg->SRSR[byte_offset] = ((u32)1)<<bit_offset;
    return true;
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
    pg_int_reg->SECR[0] = 0xffffffff;
    pg_int_reg->SECR[1] = 0xffffffff;
    pg_int_reg->SECR[2] = 0xffffffff;
    pg_int_reg->SECR[3] = 0xffffffff;
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
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    byte_offset = ufl_line/32;
    bit_offset = ufl_line%32;
    if(pg_int_reg->SRSR[byte_offset] & ((u32)1)<<bit_offset)
        return true;
    else
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
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_ASYN_SIGNAL;   //�ж�������

    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                        &= ~(1<<(ufl_line % CN_CPU_BITS));  //����λͼ
    byte_offset = ufl_line/4;
    bit_offset = (ufl_line%4)*8;
    //��ufl_lineӳ�䵽channel31������Ϊirq����Ϊ�첽�ź�
    pg_int_reg->CMR[byte_offset] = (pg_int_reg->CMR[byte_offset]
                                    | ((u32)0xff<<bit_offset ))
                                    & ((u32)31<<bit_offset);
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
    u32 byte_offset,bit_offset;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event != NULL)
        return false;     //���߳��ڵȴ�����жϣ�������Ϊʵʱ�ж�
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_REAL;    //�ж�������
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = false;   //��ʼ��Ϊ��ֹǶ��״̬
    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                        |= 1<<(ufl_line % CN_CPU_BITS);   //����λͼ
    byte_offset = ufl_line/4;
    bit_offset = (ufl_line%4)*8;
    //��ufl_lineӳ�䵽channel1������Ϊfiq����Ϊʵʱ�ж�
    pg_int_reg->CMR[byte_offset] = (pg_int_reg->CMR[byte_offset]
                                    | ((u32)0xff<<bit_offset ))
                                    & ((u32)1<<bit_offset);
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
    u32 i;
    int_disable_arm_int();      //��ֹARM���ж�
    pg_int_reg->CR = (0<<4)     //��ȡ�жϺŵȼĴ���ʱ����������ؼĴ���
                    +(1<<2);    //�ж϶���Ƕ�׿���
    __Int_ClearAllLine();      //ϵͳ����ǰ������жϹ���Ӧ���
    pg_int_reg->ECR[0] = 0xffffffff;    //����4�г��������жϺ�
    pg_int_reg->ECR[1] = 0xffffffff;
    pg_int_reg->ECR[2] = 0xffffffff;
    pg_int_reg->ECR[3] = 0xffffffff;
    pg_int_reg->SECR[0] = 0xffffffff;   //����4����������жϱ�־
    pg_int_reg->SECR[1] = 0xffffffff;
    pg_int_reg->SECR[2] = 0xffffffff;
    pg_int_reg->SECR[3] = 0xffffffff;
    pg_int_reg->HIER = 0;       //��ֹL138���жϿ������ж�
    pg_int_reg->VBR = NULL;     //���ǲ��������жϻ�ַ�Ĵ�����
    pg_int_reg->VSR = 0;
    pg_int_reg->VNR = (u32)NULL_func;    //��Ч�жϺ��ÿ�
    for(i = 0; i<26; i++)       //�����ж���Ϊ�첽�źŵ�������ȼ�
        pg_int_reg->CMR[i] = 0x1f1f1f1f;
    int_enable_arm_int();      //ʹ��ARM���ж�
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
    __Int_InitHard();
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
        tg_pIntSrcTable[ufl_line].my_evtt_id = CN_INVALID_EVTT_ID;  //�������¼�
        tg_pIntSrcTable[ufl_line].enable_nest = false;
    }

    for(ufl_line =0; ufl_line < CN_INT_BITS_WORDS; ufl_line++)
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
//void __Int_EngineAll(ufast_t ufl_line)
//{
//    if(tg_pIntSrcTable[ufl_line].int_type == CN_REAL)
//        __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
//    else
//        __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
//}

//----ʵʱ�ж�����-------------------------------------------------------------
//���ܣ���Ӧʵʱ�жϣ������жϺŵ����û�ISR
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineReal(ufast_t ufl_line)
{
    //�����ʵ��֧��ʵʱ�ж�Ƕ�ף���������3��
    tg_int_global.nest_real++;

    //��if�������ֲ���У���cpu���жϹ������ļ��������й�:
    //1���첽�ź��Ƿ��ж����Ŀ��أ�����cortex-m3��omapl138�����еģ�2440��2416��
    //   2410����û�еġ����û�ж������أ����ڴ����ж�ǰ��ϵ��첽�ź��߿���
    //2���첽�źź�ʵʱ�ж϶����ڿ���״̬������£��첽�ź��Ƿ������ռʵʱ�жϡ�
    //   �������ʵ�����ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
    //3��ʵʱ�ж���Ӧ���Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ��֧��ʵʱ�ж�Ƕ��
    if(tg_pIntSrcTable[ufl_line].enable_nest == true)
    {
        Int_DisableRealLine(ufl_line);  //����ֹ�����ܻ��γ�����ѭ��
        __int_enable_fiq();
    }
    if(tg_pIntSrcTable[ufl_line].ISR != NULL)
        tg_pIntSrcTable[ufl_line].ISR(ufl_line);  //�����û��жϺ���

    if(tg_pIntSrcTable[ufl_line].enable_nest == true)  //֧��Ƕ�׵����
    {
        __int_disable_fiq();
        Int_EnableRealLine(ufl_line);
    }

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
    tg_pIntSrcTable = malloc(tg_IntUsedNum * sizeof(struct tagIntLine));
    if(tg_pIntSrcTable == NULL)
        return;
//    tg_int_global.en_asyn_signal_counter = 1;
    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.nest_asyn_signal++;
    if(tg_pIntSrcTable[ufl_line].enable_nest == true)  //����Ƕ�׵����
    {
        Int_DisableAsynLine(ufl_line);    //����ֹ�����ܻ��γ�����ѭ��
        __int_enable_irq(); //L138���첽�ź���irqʵ�֣���Ӧ�ж�ʱ��ARM��Ibit�ر��ˡ�
    }
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_PRE)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    if(tg_pIntSrcTable[ufl_line].ISR != NULL)
        isr_result = tg_pIntSrcTable[ufl_line].ISR(ufl_line);
    else
    {
        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    }
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_POST)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    if(tg_pIntSrcTable[ufl_line].enable_nest == true)  //֧��Ƕ�׵����
    {
        __int_disable_irq();
        Int_EnableAsynLine(ufl_line);
    }

    event = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
    {
        event->event_result = isr_result;
        __Djy_EventReady(event);   //�Ѹ��¼��ŵ�ready����
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event = NULL;   //���ͬ��
    }
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id != CN_INVALID_EVTT_ID)
    {
        Djy_EventPop(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id,
                        NULL,0,(ptu32_t)isr_result, (ptu32_t)ufl_line,0);
    }
    tg_int_global.nest_asyn_signal--;
    if(tg_int_global.nest_asyn_signal == 0)
    {
//        tg_int_global.en_asyn_signal = true;
//        tg_int_global.en_asyn_signal_counter = 0;
        if(g_ptEventReady != g_ptEventRunning)
            __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
        g_bScheduleEnable = true;
    }
}

