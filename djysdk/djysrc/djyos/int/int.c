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
//�汾��V2.0.1
//�ļ�����: �ж�ģ���Ӳ���޹ز���
//����˵��:
//�޶���ʷ:
//3. ����: 2010-01-12
//   ����:  ������.
//   �°汾��: V2.0.1
//   �޸�˵��:
//   1���鵽��һ���ǳ����ε����⣬��__int_engine_real�����ڣ�������
//      if(tg_int_global.en_trunk_counter == 0)
//          tg_int_global.en_trunk = true;      //ԭ��д��false��
//   2����int_save_trunk,Int_RestoreTrunk,Int_SaveAsynSignal,
//      int_restore_asyn_signal�������������Ӷ�bg_schedule_enable�Ĳ���
//2. ����: 2010-01-04
//   ����:  ������.
//   �°汾��: V2.0.0
//   �޸�˵��: ��ԭ�汾��ȥ��Ӳ����ز��ֶ��ɣ����λ��Ҳ��arch�ĵ�djyos
//1. ����: 2009-12-10
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ��44b0�汾��ֲ����
//------------------------------------------------------

//ʵʱ�ж�ISR������:
// 1������������κ�ϵͳ���á�
// 2��������ı��κ��ж��ߵ��첽�źŻ���ʵʱ�ж����ԡ�
// 3��������ʹ�ܻ��ֹ�첽�źţ������ź��ߺ����첽�źſ��ء�
// 4������ʹ�ܻ��ֹ����Ϊʵʱ�жϵ��ж���

#include "stdint.h"
#include "stddef.h"
#include "int_hard.h"
#include "int.h"
#include "djyos.h"

ufast_t tg_int_lookup_table[CN_INT_LINE_LAST+1];
struct tagIntMasterCtrl  tg_int_global;
struct tagIntLine *tg_pIntSrcTable;     //��Int_Init�����з����ڴ�

extern void __Djy_CutReadyEvent(struct tagEventECB *event);
extern bool_t __Djy_Schedule(void);

#if 0       //��3��������"�߼�ԭ�Ӳ���"�����ظ������ٱ�����
//----�������ж�״̬����ֹ���ж�--------------------------------------------
//���ܣ���������int_restore_trunk()����ú��������ñ�����ʹ��ֹ������1������һ
//      ��int_restore_trunkʹ��ֹ������1��
//      ����ǰ����Ϊ0������Ϊ1����ֹ���жϣ���Ϊ0ʱ�򵥵���1
//��������
//���أ���
//------------------------------------------------------------------------------
void Int_SaveTrunk(void)
{
    if(tg_int_global.nest_real != 0)
        return;
    Int_CutTrunk();
    if(tg_int_global.en_trunk_counter != CN_LIMIT_UCPU)//�����޺��ټӻ���Ƶ�0
        tg_int_global.en_trunk_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_trunk_counter != 0��״̬�������
    //ʹ�жϹرգ���ʹ�û��������õ�int_save_trunk�𲻵�����
    g_bScheduleEnable = false;
    return;
}

//----�ָ���������ж�״̬---------------------------------------------------
//���ܣ���������int_save_trunk()����ú��������ñ�����ʹ��ֹ������1������
//      һ��int_save_trunkʹ��ֹ������1��
//      ����������0ʱ�������ж�,����򵥼�1
//��������
//���أ���
//------------------------------------------------------------------------------
void Int_RestoreTrunk(void)
{
    if(tg_int_global.nest_real != 0)
        return;
    if(tg_int_global.en_trunk_counter != 0)
        tg_int_global.en_trunk_counter--;
    if(tg_int_global.en_trunk_counter == 0)
    {
        Int_ContactTrunk();
        if(tg_int_global.en_asyn_signal_counter == 0)
        {
            g_bScheduleEnable = true;
            if(g_ptEventRunning!= g_ptEventReady)
            {
                Int_CutAsynSignal();
                __Djy_Schedule();
            }
        }
    }else
    {
        Int_CutTrunk();
    }
    return;
}

//----�鿴���ж��Ƿ�����-----------------------------------------------------
//���ܣ�
//��������
//���أ�������true,��ֹ����false
//ע��: �߼�ԭ�Ӳ���Ҳ������첽�źſ��أ�������û�п�����Ӱ��
//-----------------------------------------------------------------------------
bool_t Int_CheckTrunk(void)
{
    if(tg_int_global.en_trunk_counter == 0)
        return true;
    else
        return false;
}
#endif
//----���浱ǰ״̬����ֹ�첽�ź�------------------------------------------------
//���ܣ���������int_restore_asyn_signal()����ú��������ñ�����ʹ��ֹ�������ӣ�
//      ����һ��int_restore_asyn_signal()ʹ��ֹ�������١�
//      ����ǰ����Ϊ0������Ϊ1����ֹ�첽�źţ���Ϊ0ʱ�򵥵���1
//��������
//���أ���
//------------------------------------------------------------------------------
void Int_SaveAsynSignal(void)
{
    if(tg_int_global.nest_asyn_signal != 0)
        return;

    Int_CutAsynSignal();
    //�����޺��ټӻ���Ƶ�0
    if(tg_int_global.en_asyn_signal_counter != CN_LIMIT_UCPU)
        tg_int_global.en_asyn_signal_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_asyn_signal_counter != 0��״̬��
    //�����ʹ�жϹرգ���ʹ�û��������õ�en_asyn_signal_counter�𲻵�����
    g_bScheduleEnable = false;
    return;
}

//----��λ�첽�źſ���------------------------------------------------
//���ܣ����첽�źſ��ػָ�����ʼ״̬����en_asyn_signal_counter=1��״̬����ʼ��
//      �ж�ϵͳ�󣬻�Ҫ��������ģ���ʼ�����������������¼����ȣ����������¼�
//      ����ǰ���ñ�������λ�첽�ź�״̬�����Է�ֹģ���ʼ�������bug�����޸�
//      ���첽�ź�ʹ��״̬��
//��������
//���أ���
//-----------------------------------------------------------------------------
void __Int_ResetAsynSignal(void)
{
    Int_CutAsynSignal();
    tg_int_global.en_asyn_signal_counter =1;
    return;
}

//----��ѯ��ǰ�������---------------------------------------------------------
//����: ����ȷ����ǰ���������첽�ź��жϣ��������߳��С�
//����: ��
//����: 0 = �߳��С�1=�첽�ź��жϣ�����1��ʾ�첽�ź�Ƕ�׼�����
//-----------------------------------------------------------------------------
u32 Int_GetRunLevel(void)
{
    return tg_int_global.nest_asyn_signal;
}

//----�ָ�������첽�ź�״̬----------------------------------------------------
//���ܣ���������int_save_asyn_signal()����ú��������ñ�����ʹ��ֹ�������٣�����
//      һ��int_save_asyn_signal()�ǽ�ֹ�������ӡ�
//      ����������0ʱ�����첽�ź�,����򵥼�1
//��������
//���أ���
//------------------------------------------------------------------------------
void Int_RestoreAsynSignal(void)
{
    if(tg_int_global.nest_asyn_signal != 0)
        return;
    if(tg_int_global.en_asyn_signal_counter != 0)
        tg_int_global.en_asyn_signal_counter--;
    if(tg_int_global.en_asyn_signal_counter==0)
    {
//        tg_int_global.en_asyn_signal = true;   //�첽�ź���Ϊʹ��
//        if(tg_int_global.en_trunk_counter == 0)
//        {
            g_bScheduleEnable = true;
            if(g_ptEventRunning!= g_ptEventReady)
            {
                __Djy_Schedule();
//                Int_ContactAsynSignal();    //������Ѿ��򿪣������ٵ���
            }else
                Int_ContactAsynSignal();
//        }else
//            Int_ContactAsynSignal();
    }else
    {
        Int_CutAsynSignal();    //��ֹcounter>0�ڼ�����(bug)��
        g_bScheduleEnable = false;
    }
    return;
}

//----���첽�ź�ǰ�벿---------------------------------------------------------
//����: ����һ��ר��Ϊ�������л�׼���ĺ���/
//      �����ݽṹ���첽�ź�����Ϊ����״̬������������һ��:�����첽�źſ��ء�
//      �����������л������У��������ú�״̬��ʹ���������л������һ�������´�
//      �ж�ͬʱ��ɡ�������̱�ȻҪ���жϣ���Щϵͳû���ܹ������pcͬʱ��ɵġ�
//      �������첽�źſ��أ���������Ʊػ��ʵʱ�ж�Ҳ�ص�����˰��������ݽṹ��
//      ������������Լ��ٹر�ʵʱ�жϵ�ʱ�䡣��ʹ�ò���cpu����ʵ��"ʵʱ�ж�����
//      �ر�"��Ҫ�󣬵��ǹر�ʱ���Ƿǳ��̵ġ�
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Int_HalfEnableAsynSignal(void)
{
    tg_int_global.en_asyn_signal_counter = 0;
}

//----�鿴�첽�ź��Ƿ�����-----------------------------------------------------
//���ܣ�
//��������
//���أ�������true,��ֹ����false
//ע��: �ͼ�ԭ�Ӳ���Ҳ������첽�źſ��أ�������û�п�����Ӱ��
//-----------------------------------------------------------------------------
bool_t Int_CheckAsynSignal(void)
{
    if( tg_int_global.en_asyn_signal_counter == 0)
        return true;
    else
        return false;
}

//----���浱ǰ״̬����ֹ�첽�ź��ж���-----------------------------------------
//���ܣ���������int_restore_asyn_line()����ú��������ñ�����ʹ��ֹ�������ӣ���
//      ��һ��int_restore_asyn_line�ǽ�ֹ�������١�
//      ����ǰ����Ϊ0������Ϊ1����ֹ�ж��ߣ���Ϊ0ʱ�򵥵���1
//������ufl_line
//���أ���
//-----------------------------------------------------------------------------
bool_t Int_SaveAsynLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        return false;
    Int_CutLine(ufl_line);
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter!=CN_LIMIT_UCPU)//�����޺��ټӻ���Ƶ�0
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_counter != 0��״̬��
    //�����ʹ�жϹرգ���ʹ�û��������õ�en_counter�𲻵�����
    tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                &= ~(1<<(ufl_line % CN_CPU_BITS));
    return true;
}

//----���浱ǰ״̬����ֹ�첽�ź��ж���-----------------------------------------
//���ܣ���������int_restore_real_line()����ú��������ñ�����ʹ��ֹ�������ӣ���
//      ��һ��int_restore_real_line�ǽ�ֹ�������١�
//      ����ǰ����Ϊ0������Ϊ1����ֹ�ж��ߣ���Ϊ0ʱ�򵥵���1
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t Int_SaveRealLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
        return false;
    Int_CutLine(ufl_line);
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter!=CN_LIMIT_UCPU)//�����޺��ټӻ���Ƶ�0
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_counter != 0��״̬��
    //�����ʹ�жϹرգ���ʹ�û��������õ�en_counter�𲻵�����
    tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                &= ~(1<<(ufl_line % CN_CPU_BITS));
    return true;
}

//----�ָ�������첽�ź��ж���״̬---------------------------------------------
//���ܣ���������int_save_line��������ú��������ñ�����ʹ��ֹ�������٣���
//      ��һ��int_save_line�ǽ�ֹ�������ӡ�
//      ����������0ʱ�����ж���,����򵥼�1
//������ufl_line
//���أ���
//ע: ��������������ʵʱ�ж�ISR�е��ã�������ֱ�ӷ���false��
//-----------------------------------------------------------------------------
bool_t Int_RestoreAsynLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter != 0)
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter--;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter==0)
    {
        tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                |= 1<<(ufl_line % CN_CPU_BITS);
        Int_ContactLine(ufl_line);
    }
    return true;
}

//----�ָ������ʵʱ�ж���״̬-------------------------------------------------
//���ܣ���������int_save_line��������ú��������ñ�����ʹ��ֹ�������٣���
//      ��һ��int_save_line�ǽ�ֹ�������ӡ�
//      ����������0ʱ�����ж���,����򵥼�1
//������ufl_line
//���أ���
//ע: ��int_restore_asyn_line��һ�������������첽�źź�ʵʱ�ж�ISR�ж��������
//-----------------------------------------------------------------------------
bool_t Int_RestoreRealLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter != 0)
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter--;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter==0)
    {
        tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                |= 1<<(ufl_line % CN_CPU_BITS);
        Int_ContactLine(ufl_line);
    }
    return true;
}

//----ֱ�ӽ�ֹ�첽�ź��ж���---------------------------------------------------
//���ܣ���������int_enable_asyn_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ�����
//      ��λ���������ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t Int_DisableAsynLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        return false;
    Int_CutLine(ufl_line);
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter = 1;
    tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                &= ~(1<<(ufl_line % CN_CPU_BITS));
    return true;
}

//----ֱ�ӽ�ֹʵʱ�ж���-------------------------------------------------------
//���ܣ���������int_enable_real_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ�����
//      ��λ���������ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t Int_DisableRealLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
        return false;
    Int_CutLine(ufl_line);
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter = 1;
    tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                &= ~(1<<(ufl_line % CN_CPU_BITS));
    return true;
}

//----ֱ�������첽�ź��ж���---------------------------------------------------
//���ܣ���������int_disable_asyn_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ���
//      �����㣬����ͨ�ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t Int_EnableAsynLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        return false;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter = 0;
    tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                |= 1<<(ufl_line % CN_CPU_BITS);
    Int_ContactLine(ufl_line);
    return true;
}

//----ֱ������ʵʱ�ж���-------------------------------------------------------
//���ܣ���������int_disable_real_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ���
//      �����㣬����ͨ�ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t Int_EnableRealLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
        return false;

    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter = 0;
    tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
                |= 1<<(ufl_line % CN_CPU_BITS);
    Int_ContactLine(ufl_line);
    return true;
}

//----��ѯ�ж���ʹ��״̬-------------------------------------------------------
//���ܣ���ѯ�ж����Ƿ�����
//������ufl_line������ѯ���ж���
//���أ�true = ʹ�ܣ�false = ��ֹ��
//-----------------------------------------------------------------------------
bool_t Int_CheckLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].en_counter == 0)
        return true;
    else
        return false;
}

//----�����ж�Ӧ��ʽ---------------------------------------------------------
//����: ����ĳ�ж��ߵ�Ӧ��ʽ
//������ufl_line�������õ��ж���
//      clear_type, Ӧ��ʽ����������֮һ:
//          CN_INT_CLEAR_USER    ϵͳ��Ӧ�����û���ISR��Ӧ��Ĭ��״̬
//          CN_INT_CLEAR_PRE     ����ISR֮ǰӦ��
//          CN_INT_CLEAR_POST    ����ISR����֮���жϷ���ǰӦ��
//����: true=�ɹ���false=ʧ�ܣ����ж��߱���Ϊʵʱ�жϣ��̶�Ϊcn_int_clear_user
//-----------------------------------------------------------------------------
bool_t Int_SetClearType(ufast_t ufl_line,ufast_t clear_type)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        return false;
    else
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type = clear_type;
    return true;
}

//----�����ж�����ISR----------------------------------------------------------
//���ܣ�ָ���ж���ָ���ж���Ӧ�������ú���Ϊ��ͨ������
//������ufl_line,��Ҫ���õ��ж��ߺ�
//      isr���ж���Ӧ���������û��ṩ��ԭ�ͣ�void isr(ufast_t)
//���أ���
//-----------------------------------------------------------------------------
void Int_IsrConnect(ufast_t ufl_line, u32 (*isr)(ufast_t))
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR = isr;
    return;
}

//----�����ж������¼�����-----------------------------------------------------
//���ܣ�Ϊָ�����ж���ָ��һ���¼�����id��������첽�źţ����ڷ���������ǰ����
//      �¼�����Ϊ��id���¼��������ʵʱ�жϣ��򲻵����¼���
//������ufl_line,��Ҫ���õ��ж��ߺ�
//      my_evtt_id���¼�����id
//���أ�true = �ɹ������¼����� , false = ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t Int_EvttConnect(ufast_t ufl_line,uint16_t my_evtt_id)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type != CN_ASYN_SIGNAL)
        return false;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id = my_evtt_id;
    return true;
}

//----�Ͽ��ж������ж���Ӧ�����Ĺ���-------------------------------------------
//���ܣ��Ͽ�ָ���ж���ָ���ж���Ӧ�����Ĺ�������֮�Կպ���
//������ufl_line,��Ҫ���õ��ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void Int_IsrDisConnect(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR = NULL;
    return;
}

//----�Ͽ��ж������¼����͵Ĺ���-----------------------------------------------
//���ܣ��Ͽ�ָ���ж���ָ���¼����͵Ĺ�������֮��cn_invalid_evtt_id
//������ufl_line,��Ҫ���õ��ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void Int_EvttDisConnect(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return;
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id = CN_EVTT_ID_INVALID;
    return;
}

//----�趨�ж�ͬ��-------------------------------------------------------------
//����: �������ڴ�����¼����̣߳�ֱ��ָ�����ж��ߵ��жϷ�������Ӧ�����أ�Ȼ���
//      �����̡߳�����ǰȷ�����ж��߱���ֹ�����ñ������������ж��߱�ʹ��(��ֱ��
//      ʹ�ܣ����ǵ���int_save_asyn_line)�����ڷ��غ�ָ���ֹ״̬��
//����: ufl_line,�ȴ���Ŀ���ж���
//����: false = ���ж��Ѿ��������̵߳ȴ���ֱ�ӷ��ء�
//      true = �ɹ�ͬ�������ڸ��жϷ����󷵻ء�
//��ע: 1.�ж���һ���ٽ���Դ���������жϺ�����̫������飬���ж�ͬ���Ĺ��ܱȽϼ�
//      ����ÿ���ж���ͬһʱ��ֻ����һ���̵߳ȴ���Ҳ���������ó�ʱ�ȴ�
//      2.����djyosһ�����ж�ͬ������ֻ�ܰ��Լ�����ȴ�״̬�������ܿ��Ʊ��
//      �̣߳��ʺ���ԭ�Ͳ����� bool_t int_sync(ufast_t line,uint16_t event_id)
//      3.ʵʱ�ж����õȴ���Ч�����ñ�����ʱ�����line�Ѿ�������Ϊʵʱ�жϣ���
//      ֱ�ӷ���false��������ñ�������line���ܱ�����Ϊʵʱ�жϡ�
//      4.Ϊ��ȷʵ�ֹ��ܣ����ڵ���ǰȷ�����ж����Ǳ���ֹ�ġ�
//-----------------------------------------------------------------------------
bool_t Int_AsynSignalSync(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if( !Djy_QuerySch())
    {   //��ֹ���ȣ����ܽ����첽�ź�ͬ��״̬��
        Djy_SaveLastError(EN_KNL_CANT_SCHED);
        return false;
    }
    Int_SaveAsynSignal();   //�ڲ������������ڼ䲻�ܷ����ж�
    //ʵʱ�жϲ�������ͬ����һ���ж�ֻ����һ��ͬ���¼�
    if((tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
            || (tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event != NULL))
    {
        Int_RestoreAsynSignal();
        return false; //ʵʱ�жϻ��Ѿ���ͬ���¼�
    }else
    {
        if(Int_QueryLine(ufl_line) == true)    //�ж��Ѿ�������ͬ�������ﵽ
        {
            Int_ClearLine(ufl_line);
            Int_RestoreAsynSignal();
            return true;
        }
        //�������дӾ���������ȡ��running�¼�
        __Djy_CutReadyEvent(g_ptEventRunning);
        g_ptEventRunning->next = NULL;
        g_ptEventRunning->previous = NULL;
        g_ptEventRunning->event_status = CN_STS_WAIT_ASYN_SIGNAL;
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event = g_ptEventRunning;
    }
    Int_EnableAsynLine(ufl_line);
    Int_RestoreAsynSignal();      //���ñ������������߳��л������ڴ�����¼���
                                    //����
    Int_DisableAsynLine(ufl_line);
    g_ptEventRunning->wakeup_from = CN_STS_WAIT_ASYN_SIGNAL;
    g_ptEventRunning->event_status = CN_STS_EVENT_READY;
    return true;
}
//��ע: ���ṩ�������ж�ͬ�����ܣ���Ϊdjyos���ṩ���������߻��߹���Ĺ��ܣ�����
//      ����ʱ���ж�Ϊ����һ��ʱ���жϰ��̴߳�������ready�󣬵��´�ʱ���жϵ���
//      ֮ǰ�����߳�Ҫô����ready̬(���ܱ������߳���ռ)����ʱ����Ҫ�ٴδ�����
//      Ҫô�ڵȴ�������������������ȴ��ڴ���䡢�ȴ���ʱ�����ȴ��ź����ȣ���ʱ
//      ����ʱ���жϴ��������ƻ�������Ľṹ���������ж�ͬ�����ö�ε��õ���ͬ��
//      �ķ�����ɣ���ÿ�δ������߳�����˱�Ҫ�Ĺ����Ժ������ٴε��õ���ͬ��
//      �����������������ÿһ��������ȷ��Ŀ�꣬��������Ŀ�ĵ����߻����


