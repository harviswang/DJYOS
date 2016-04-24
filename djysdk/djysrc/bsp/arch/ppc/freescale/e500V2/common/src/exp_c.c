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
// ģ������: �쳣�����C���֣��ⲿ���漰����CPU�Ĺ���
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 9:30:54 AM/Apr 18, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע
// ����SC TICK EXTERNAL�쳣���⣬��������е��쳣������ʹ���κε�ϵͳ���ã�����
// ����ȵ�API������д�����쳣����ʱ���С�ģ���Ҫ���κε�ϵͳ���ã���¼�쳣д���ʺ���
// ��ر�֤��ֱ��д���������κεĵ��������Ĵ��룩

#include "stdint.h"
#include "stddef.h"

#include "os.h"

#include "int_hard.h"

extern struct tagIntLine        *tg_pIntSrcTable;      //�ж��߿������ݽṹ
extern struct tagIntMasterCtrl  tg_int_global;         //���岢��ʼ�����жϿ��ƽṹ
//int
extern void __Int_EngineReal(ufast_t ufl_line);
extern void __Int_EngineAsynSignal(ufast_t ufl_line);
extern void __Djy_ScheduleAsynSignal(void);
// =============================================================================
// �������ܣ�Exp_DTimer  ϵͳTICK�ж�
// ���������
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    ����E500�����£��ò�����Ϊϵͳ��TICK���д���
// =============================================================================
void (*user_systick)(void) = NULL_func;
void Exp_ConnectSystick(void (*tick)(void))
{
    user_systick = tick;
}

extern struct  tagEventECB  *g_ptEventReady;      //��������ͷ
extern struct  tagEventECB  *g_ptEventRunning;    //��ǰ����ִ�е��¼�
extern struct  tagEventECB  *g_ptEventDelay;      //����ͬ�����б�ͷ
extern bool_t  g_bScheduleEnable;                 //ϵͳ��ǰ����״̬�Ƿ������
extern ufast_t tg_int_lookup_table[CN_INT_LINE_LAST+1];
extern void __Djy_EventReady(struct  tagEventECB *event_ready);

void Exp_DTimer(void)
{
    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal=1;
    user_systick();

    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
    tg_int_global.nest_asyn_signal = 0;
}


// =============================================================================
// �������ܣ�Exp_EInt  �첽�źŵĴ�������
// �����������
// �����������
// ����ֵ     ��
// ˵��          ���첽�źŲ�֧��Ƕ�ף��ϲ���������鴦��
// =============================================================================
void Exp_EInt(void)
{
	ufast_t ufl_line;
    struct tagEventECB *event;
    u32 isr_result[2]={0,0};
	
	ufl_line = Int_GetEIntNumber();
	if(ufl_line < CN_INT_LINE_LAST)
	{
		//valid int number
		   g_bScheduleEnable = false;
		    tg_int_global.nest_asyn_signal=1;
		    
		    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_PRE)
		        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
		    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR != NULL)
		    {
		        isr_result[0] = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR(ufl_line);
		    }
		    else
		    {
		        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_USER)
		            Int_ClearLine(ufl_line);        //�ж�Ӧ��,
		    }
		    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].clear_type == CN_INT_CLEAR_POST)
		        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
		    event = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event;
		    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
		    {
		        event->event_result = isr_result[0];
		        __Djy_EventReady(event);   //�Ѹ��¼��ŵ�ready����
		        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event = NULL;   //���ͬ��
		    }
		    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id != CN_EVTT_ID_INVALID)
		    {
		        isr_result[1] = (u32)ufl_line;
		        Djy_EventPop(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].my_evtt_id,
		                        NULL,0,0,0,0);
		    }

		    if(g_ptEventReady != g_ptEventRunning)
		        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
		    g_bScheduleEnable = true;
		    tg_int_global.nest_asyn_signal = 0;	
	}
    return;
}

// =============================================================================
// �������ܣ�Exp_CInit ʵʱ�źŵĴ�������
// ����������жϺ�
// �����������
// ����ֵ     ��
// ˵��          ��֧��ʵʱ�źŵ�Ƕ�״���;�ϲ���������飬���㲻��
// =============================================================================
void Exp_CInit(void)
{
	ufast_t ufl_line;
	ufl_line = Int_GetCIntNumber();
	if((ufl_line > CN_INT_LINE_LAST)&&
	    (tg_int_lookup_table[ufl_line] != CN_LIMIT_UFAST))
	{
	    tg_int_global.nest_real++;
	    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest == false)
	    {
	        Int_CutTrunk();
	    }

	    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR != NULL)
	        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].ISR(ufl_line);  //�����û��жϺ���

	    if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest == false)
	    {
	        Int_ContactTrunk();
	    }
	    tg_int_global.nest_real--;
	}
	return;
}

// =============================================================================
// �������ܣ��쳣��ʼ��       
// �����������ʱ�ޣ�����������
// �����������
// ����ֵ     :true�ɹ�, falseʧ��
// ˵��          ��
// =============================================================================
bool_t Exp_Init(void)
{
    //�����쳣������
    extern void __AsmSetExpVecTab(void);
    __AsmSetExpVecTab();

    return true;
}




