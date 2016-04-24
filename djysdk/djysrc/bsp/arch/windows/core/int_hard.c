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
// �����Ȩ����?��ʹ���߷�������������������?����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ��?���۴˰�װ�Ƿ񾭸�����Ȼ?
//
// 1. ���ڱ����Դ�������ɢ��?���뱣�������İ�Ȩ���桢�������б�?��
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ��?�����������ļ��Լ�?����������
//    ��ɢ����װ�е�ý�鷽ʽ?��������֮��Ȩ���桢�������б�?�Լ�����
//    ������������

// ��������?������Ǳ������Ȩ�������Լ�����������״?"as is"?�ṩ?
// �������װ�����κ���ʾ��Ĭʾ֮��������?�����������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮������?�����κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ?������ʧ������ԭ���?����?�����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν������?����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵ�??
// �����κ�����?���ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:�ж�ģ��
//����?lst
//�汾?V1.0.0
//�ļ�����: �ж�ģ����Ӳ����صĴ���?�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ԭint.c�а�Ӳ���޹ش����������
//------------------------------------------------------
#include "windows.h"
#include "stdint.h"
#include "stdlib.h"
#include "cpu_peri.h"
#include "int.h"
#include "int_hard.h"
#include "critical.h"
#include "djyos.h"

CRITICAL_SECTION  int_cs;
ufast_t ufg_int_number;
HANDLE win32_int_event;
HANDLE win32_int_engine;

extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�
extern struct tagIntLine *tg_pIntSrcTable;     //��Int_Init�����з����ڴ�
extern struct tagIntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);
void __Djy_EventReady(struct  tagEventECB *event_ready);

//----����߼�ԭ�Ӳ���---------------------------------------------------------
//����?����ǰ���ж�״̬?Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ��
//      ��ϵĲ�����
//      Int_HighAtomStart--int_high_atom_end��������ʹ��?
//      ��������������"Int_SaveTrunk--Int_RestoreTrunk"�Ŀ��ٰ汾?�ڱ�������
//      ס�Ĵ������?���������"Int_SaveTrunk--Int_RestoreTrunk",��������
//      ���ÿ��������߳��л��ĺ���?���鲻Ҫ�����κ�ϵͳ���á�
//����?��
//����?ԭ��״̬
//��ע?��ֲ����
//-----------------------------------------------------------------------------
atom_high_t Int_HighAtomStart(void)
{
    EnterCriticalSection(&int_cs);
    return true;
}

//----�뿪�߼�ԭ�Ӳ���---------------------------------------------------------
//����?�ָ�����ǰ���ж�״̬?������Ӧ��int_high_atom_start��������
//      Int_HighAtomStart--int_high_atom_end��������ʹ��?
//      ��������������"Int_SaveTrunk--Int_RestoreTrunk"�Ŀ��ٰ汾?�ڱ�������
//      ס�Ĵ������?���������"Int_SaveTrunk--Int_RestoreTrunk",��������
//      ���ÿ��������߳��л��ĺ���?���鲻Ҫ�����κ�ϵͳ���á�
//����?high?int_high_atom_start�����ԭ��״̬
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
void Int_HighAtomEnd(atom_high_t high)
{
    LeaveCriticalSection(&int_cs);

}

//----����ͼ�ԭ�Ӳ���---------------------------------------------------------
//����?��ȡ��ǰ�첽�źſ���״̬?Ȼ���ֹ�첽�źš��ͼ���ԭ�Ӳ�����������ֹ�ٽ�
//      ���ݵĲ�������?�����ܻᱻʵʱ�жϴ�ϡ�
//      Int_LowAtomStart---int_low_atom_end��������ʹ��?��������������
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"�Ŀ��ٰ汾?�ڱ�������ס
//      �Ĵ������?���������int_restore_asyn_signal��int_save_asyn_signal�е�
//      �κ�һ��������������ÿ��������߳��л��ĺ���?���鲻Ҫ�����κ�ϵͳ���á�
//����?��
//����?ԭ��״̬
//��ע?��ֲ����
//-----------------------------------------------------------------------------
atom_low_t Int_LowAtomStart(void)
{
    EnterCriticalSection(&int_cs);
    return true;
}

//----�뿪�ͼ�ԭ�Ӳ���---------------------------------------------------------
//����?�ָ�����ǰ�첽�ź����״̬?������Ӧ��int_low_atom_start�������ס�
//      Int_LowAtomStart---int_low_atom_end��������ʹ��?��������������
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"�Ŀ��ٰ汾?�ڱ�������ס
//      �Ĵ������?���������int_restore_asyn_signal��int_save_asyn_signal�е�
//      �κ�һ��������������ÿ��������߳��л��ĺ���?���鲻Ҫ�����κ�ϵͳ���á�
//����?low?int_low_atom_start�����ԭ��״̬
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
void Int_LowAtomEnd(atom_low_t low)
{
    LeaveCriticalSection(&int_cs);
}

//----��ͨ�첽�źſ���---------------------------------------------------------
//����?��ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�ѷ����������ж�����rINTMSK��Ӧλ��0.
//����?��
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
void Int_ContactAsynSignal(void)
{
    LeaveCriticalSection(&int_cs);
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//����?�Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�������첽�ź�����rINTMSK��Ӧλ��1.
//����?��
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
void Int_CutAsynSignal(void)
{
    if(tg_int_global.en_asyn_signal_counter == 0)
    {
        EnterCriticalSection(&int_cs);
    }
}

//----��ͨ���жϿ���-----------------------------------------------------------
//����?��ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//����?��
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
void Int_ContactTrunk(void)
{

}

//----�Ͽ����жϿ���---------------------------------------------------------
//����?�Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//����?��
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{

}

//----��ͨ�����ж��߿���-------------------------------------------------------
//����?��ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//����?��
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    return  true;
}

//----�Ͽ������ж��߿���-------------------------------------------------------
//����?�Ͽ������ж��߿���?�������жϺ��첽�źſ���״̬���?���ж��߱���ֹ
//����?��
//����?��
//��ע?��ֲ����
//-----------------------------------------------------------------------------
bool_t Int_CutLine(ufast_t ufl_line)
{

    return true;
}

//----Ӧ���ж�?�����Ӧ�ж��ߵ��жϹ���״̬-----------------------------------
//����?Ӳ��Ӧ������Ӧ�Ĺ���?�ṩ����жϹ���Ĳ���?���ǰ?������Ӧͬһ�ж���
//      �ĺ����ж�?�����?�ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//      �����첽�ź��޹�
//����?ufast ufl_line?ָ��Ӧ����ж��ߺ�
//����?��
//��ע?��Щ��ϵ�ж���ӦʱӲ��Ӧ��?������Ϊ�պ�����
//      ����������ֲ���к���
//˵��: windows�汾����ҪӦ���ж�
//-----------------------------------------------------------------------------
bool_t Int_ClearLine(ufast_t ufl_line)
{

    return true;
}

//----�����ж�-----------------------------------------------------------------
//����: ����һ���ж�.����жϱ�������,��������Ӱ��.����������ж��߱�����Ϊʵʱ
//      �жϻ����첽�ź��޹�
//����?ufast ufl_line?���������ж��ߺ�
//����?�����Ӧ���ж���Ӳ�����ṩ����������жϹ���,���� false,���򷵻� true
//��ע: ������ʵ��������Ӳ��,��ЩӲ��ϵͳ��֧�ִ˹���.
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_TapLine(ufast_t ufl_line)
{
    EnterCriticalSection(&int_cs);
    if(tg_int_global.en_trunk_counter == 0) //ʵʱ�ж�ʹ�ܲſ��Է���
    {
        //����ж�����ʹ�ܵ�
        if(tg_int_global.enable_bitmap[ufl_line/CN_CPU_BITS]
            & ((ucpu_t)1<<(ufl_line % CN_CPU_BITS) ))
        {
            //�ж�������ʵʱ�ж�,�����첽�ź�ʹ��
            if( (tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type ==CN_REAL)
                        ||(tg_int_global.en_asyn_signal_counter == 0) )
            {
                ufg_int_number = ufl_line;
                SetEvent(win32_int_event);
            }
        }
    }
    LeaveCriticalSection(&int_cs);
    return true;
}

//----Ӧ��ȫ���ж�?���ȫ���ж��ߵ��жϹ���״̬-------------------------------
//����?Ӳ��Ӧ������Ӧ�Ĺ���?�ṩ����жϹ���Ĳ���
//����?ufast ufl_line?ָ��Ӧ����ж��ߺ�
//����?��
//��ע?��Щ��ϵ�ж���ӦʱӲ��Ӧ��?������Ϊ�պ�����
//      ����������ֲ���к���
//˵��: windows�汾����ҪӦ���ж�
//-----------------------------------------------------------------------------
void __Int_ClearAllLine(void)
{

}

//----��ѯ�ж�������״̬-------------------------------------------------------
//����?��ѯ�������Ӧ�ж���״̬?�����ڲ�ѯʽ�жϳ���
//����?ufl_line?����ѯ���ж���
//����?���жϹ���?����true?���򷵻�false
//��ע: ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
//      ����������ֲ�ؼ�����
//˵��: windows�汾���ܲ�ѯ�ж�������״̬
//-----------------------------------------------------------------------------
bool_t Int_QueryLine(ufast_t ufl_line)
{
    return false;
}

//----��ָ���ж�������Ϊ�첽�ź�--------???---------------------------------
//����?��ָ���ж�������Ϊ�첽�ź�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//����?ufast ufl_line?ָ�������õ��ж��ߺ�
//����?true=�ɹ�?false=ʧ��
//      ��������ֲ����
//˵��: windows�汾���ж�ֻ�����첽�ź�
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

//----��ָ���ж�������Ϊʵʱ�ж�--------???---------------------------------
//����?��ָ���ж�������Ϊʵʱ�ж�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//����?ufast ufl_line?ָ�������õ��ж��ߺ�
//����?true=�ɹ�?false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoReal(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_int_lookup_table[ufl_line] == CN_LIMIT_UFAST) )
        return false;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event != NULL)
        return false;     //���߳��ڵȴ�����ж�?������Ϊʵʱ�ж�
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_REAL;    //�ж�������
    tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = false;   //��ʵ�ֲ�֧��ʵʱ�ж�Ƕ��
    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
            |= 1<<(ufl_line % CN_CPU_BITS);   //����λͼ
    return true;
}

//----ʹ���ж�Ƕ��-------------------------------------------------------------
//����: ʹ��һ���ж�Դ����ռ?ʹ�ܺ�?��Ӧ���жϷ����ڼ�?���ܻᱻ����ж���ռ��
//      ����������ʵʱ�жϻ����첽�ź�?������ʵ���й�?��ֲ��Ӧ�ø���Ӳ������
//      ��Ӧ������?ȷ���Ƿ�����Ƕ�ס�
//      Ƕ�׷�����ʵʱ�ж�֮������첽�ź�֮�䡣ʵʱ�ж���Զ���Դ���첽�ź�?��
//      ����Ƕ�ס�
//      ��������ϵ�е�ARMоƬ�жϹ��������ص�?����ʵʱ�жϵĴ���?�����ַ�ʽ:
//      1����fiqʵ��ʵʱ�ж�?INTOFFSET1��INTOFFSET2�Ĵ����ṩ�˵�ǰ���ڷ����
//         �жϺ�?���������Ĵ�����fiq����Ч?���ʵʱ�ж���fiqʵ��?����Ҫ��λ
//         ��鱻��Ϊʵʱ�жϵ��жϺ���INTPND1��INTPND1�Ĵ����е�ֵ?����ȷ����
//         ������жϺ�?������̿��ܷǳ���������������ܹ�ʵ��ʵʱ�ж�Ƕ��?��
//         ȴ��ȫʧȥ��ʵʱ�жϵ���Ƴ���?djyos for s3c2416����ȡ���ַ�ʽ��
//      2����һ�����������?ϵͳֻ��Ҫһ��ʵʱ�ж�?����ȫ����Ϊ�첽�ź�?����
//         ���?��fiqʵ��ʵʱ�ж�����Ϊ�����?��Ȼֻ��һ���ж�?Ƕ��Ҳ���޴�
//         ̸���ˡ�
//      3��ʵʱ�жϺ��첽�źŶ�����irq��ʽʵ��?cpsr��Iλ����trunk����?�첽�ź�
//         û�ж�������?��ͨ�������е������첽�źŵ��жϺŵĿ���ȫ���ص���ģ��
//         �첽�źſ��صġ���������ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�Ϊʲô��?arm�����
//         Ϊ?ֻҪIλΪ0?�µ��жϾͿ�����ռ���ڷ�����ж�?Ҳ����ζ��?�첽��
//         ����ռʵʱ�ж�?��Ӳ����������ġ�ʵʱ�ж�Ҫʵ��Ƕ��?���ȹص��첽��
//         ��?������I=0?���ʵʱ�ж���Ӧ��?���첽�źű��ص�֮ǰ?���������첽
//         �źŷ����Ļ�?��irq����ͻᷢ��?���ص��첽�ź�Ҳû��?���ж�һ����
//         ����?���첽�źžͻ���ռʵʱ�жϡ�������ռ?������"����"����?������
//         ����������?����ʹϵͳ�����������ռʵʱ�жϵ��첽�ź�?�����ڼ䷢��
//         ���߳��л�?��fiq�����ջ?����pg_event_running��ջ�����?djyos for
//         2416�汾��֧��ʵʱ�ж�Ƕ�ס�
//����: ufl_line?���������ж���
//����: ��
//˵��: windows�汾��֧���ж�Ƕ��
//-----------------------------------------------------------------------------
bool_t Int_EnableNest(ufast_t ufl_line)
{

    return false;
}

//----��ֹ�ж�Ƕ��-------------------------------------------------------------
//����: ���ĳ�жϺŵ��ж�Ƕ��ʹ�ܡ�
//����: ufl_line?���������ж���
//����: ��
//-----------------------------------------------------------------------------
void Int_DisableNest(ufast_t ufl_line)
{

}

//----�趨Ƕ�����ȼ�-----------------------------------------------------------
//����: �趨ָ���ж��ߵ�Ƕ�����ȼ�?��������������Ӳ�����ܡ����Ӳ����֧��?��
//      ���ֿպ�����
//����?ufast ufl_line?ָ�������õ��ж��ߺ�
//����?��
//ע: ��������ֲ����
//˵��: windows�汾�жϲ������ȼ�
//-----------------------------------------------------------------------------
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio)
{
    return false;
}
u32 WINAPI win32_switch( LPVOID lpParameter )
{
    while(1)
    {
        WaitForSingleObject(win32_int_event,INFINITE );
        __Int_EngineAll(ufg_int_number);
    }
}


//----��ʼ���ж�Ӳ����ز���---------------------------------------------------
//����: �����
//����: ��
//����: ��
//ע: ��ֲ����?��Ӳ�����?Ҳ����������й�
//-----------------------------------------------------------------------------
void __Int_InitHard(void)
{
    win32_int_event  = CreateEvent(NULL,FALSE,FALSE,NULL);
    win32_int_engine = CreateThread( NULL, 0, win32_switch, 0,
                                     0, NULL );

    SetPriorityClass(win32_int_engine,THREAD_PRIORITY_HIGHEST);
    SetThreadAffinityMask( win32_int_engine, 1 );
    SetThreadPriority(win32_int_engine,THREAD_PRIORITY_TIME_CRITICAL);
}
//----��ʼ���ж�---------------------------------------------------------------
//����?��ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//      2.�첽�źű��ֽ�ֹ,��������������������д�.
//      3.���ж�����?
//      �û���ʼ������Ӧ���������¹���:
//      1.ϵͳ��ʼʱ���Ѿ���ֹ�����첽�ź�,�û���ʼ��ʱ���뵣���첽�źŷ���.
//      2.��ʼ�������������Ҫ�������ж�/ʵʱ�ж�/�첽�ź�,Ӧ�óɶ�ʹ��.��ֹʹ
//        �첽�ź�ʵ�ʴ�������״̬(���첽�����жϿ���ͬʱ����).
//      3.���Բ����ж���,�������ӡ�������ֹ��,��Ӧ�óɶ�ʹ��.
//      4.����ʹ��save/restore������,��Ҫʹ��enable/disable������.
//����?��
//����?��
//-----------------------------------------------------------------------------
void Int_Init(void)
{
    ufast_t ufl_line;
    InitializeCriticalSection(&int_cs);
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
    Int_CutAsynSignal();
    tg_int_global.en_asyn_signal_counter = 1;   //�첽�źż���
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;   //���жϼ���
    Int_ContactTrunk();                //��ͨ���жϿ���
}

//----���ж�����---------------------------------------------------------------
//����?��Щϵͳ?���ж�����������������ʵʱ�жϻ����첽�źŵ�?���߲�ϣ���ڻ�
//      ��׶�ʹ�ù��ڸ��ӵĴ����?����2416��2440��?���ڻ��׶λ�ȡ�жϺź�?
//      ֱ�ӵ��ñ�����?�ɱ���������������첽�ź������ʵʱ�ж����档
//      ��cortex-m3��omapl138����?���ж��������־Ϳ�������ʵʱ�жϻ����첽��
//      ��?�������ṩ������?�����ڻ�ಿ��ֱ�ӵ�����Ӧ�����档
//����?ufast ufl_line?��Ӧ���ж��ߺ�
//����?��
//-----------------------------------------------------------------------------

void __Int_EngineAll(ufast_t ufl_line)
{
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
    else
        __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
}

//----ʵʱ�ж�����-------------------------------------------------------------
//����?��Ӧʵʱ�ж�?�����жϺŵ����û�ISR
//����?ufast ufl_line?��Ӧ���ж��ߺ�
//����?��
//-----------------------------------------------------------------------------
void __Int_EngineReal(ufast_t ufl_line)
{
    tg_int_global.nest_real++;

    //��if�������ֲ����?��cpu���жϹ������ļ��������й�:
    //1���첽�ź��Ƿ��ж����Ŀ���?����cortex-m3��omapl138�����е�?2440��2416��
    //   2410����û�еġ����û�ж�������?���ڴ����ж�ǰ��ϵ��첽�ź��߿���
    //2���첽�źź�ʵʱ�ж϶����ڿ���״̬�������?�첽�ź��Ƿ������ռʵʱ�жϡ�
    //   ������ԣ��ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
    //3��ʵʱ�ж���Ӧ��?�Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ��֧��ʵʱ�ж�Ƕ��?������ע�͵�
//    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest == true)
//    {
//        real_save_asyn = Int_LowAtomStart();
//        Int_ContactTrunk();
//    }
    Int_ClearLine(ufl_line);        //�ж�Ӧ��
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR != NULL)
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR(ufl_line);  //�����û��жϺ���


    //�����ʵ��֧��ʵʱ�ж�Ƕ��?��������4��
    tg_int_global.nest_real--;

}

//----�첽�¼��ж�����---------------------------------------------------------
//����?��Ӧ�첽�ź�?�����жϺŵ����û�ISR?��󵯳��ж��߿��ƿ��my_evtt_id
//      ��Աָ�����¼�����?����ڷ���ǰ�鿴�Ƿ���Ҫ���������л�?����Ҫ���л�
//      ֮��
//����?ufast ufl_line?��Ӧ���ж��ߺ�
//����?��
//-----------------------------------------------------------------------------
void __Int_EngineAsynSignal(ufast_t ufl_line)
{
    struct tagEventECB *event;
    u32 isr_result;

    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal=1;
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_PRE)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR != NULL)
        isr_result = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR(ufl_line);
    else
    {
        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    }
    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_POST)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    event = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע?�����¼�?���Ƕ���)
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
    tg_int_global.nest_asyn_signal = 0;

    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
    return;
}



