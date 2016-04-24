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
// ģ������: timer��Ӳ���ӿ�
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 1:53:42 PM/Mar 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�����ļ���������ʹ�ö�ʱ��������һ��ͳһ�Ľӿڣ�ʵ�ֻ�����timer_driver.c�ļ�
#include "timer_hard.h"
#include "stddef.h"
static struct tagTimerChip   gs_tTimerHardChip = {NULL};//ע���оƬ����
// =============================================================================
// �������ܣ�TimerHard_RegisterChip
//          ע�ᶨʱ��оƬ��ϵͳ��ʱ��ģ��
// �������:timerchip,��ʱ��оƬ
// �����������
// ����ֵ  ��true �ɹ� falseʧ�ܣ�����ʵ��
// =============================================================================
bool_t  TimerHard_RegisterChip(struct tagTimerChip *timerchip)
{
	if(NULL != timerchip)
	{
		gs_tTimerHardChip.chipname  = timerchip->chipname;
		gs_tTimerHardChip.timerhardalloc = timerchip->timerhardalloc;
		gs_tTimerHardChip.timerhardfree =  timerchip->timerhardfree;
		gs_tTimerHardChip.timerhardctrl = timerchip->timerhardctrl;
		return true;
	}
	else
	{
		return false;
	}
}
// =============================================================================
// �������ܣ�TimerHard_UnRegisterChip
//          ��ʱ��оƬע��
// �����������
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :Ŀǰ������û���ã���Ҫ��register֮���Ȼ��unregister������ʵ��
// =============================================================================
bool_t TimerHard_UnRegisterChip(void)
{
	gs_tTimerHardChip.timerhardalloc = NULL;
	gs_tTimerHardChip.timerhardfree = NULL;
	gs_tTimerHardChip.timerhardctrl = NULL;
	return true;
}
// =============================================================================
// �������ܣ�TimerHard_Alloc
//          HAL��ʱ������
// ���������
//     cycle, ָ�����䶨ʱ���Ķ�ʱ���ڣ������Կ���ʹ��API�������и����趨����λΪ΢�룩
//     timerisr,����Ķ�ʱ�����жϷ�����,�ж��е���
// �����������
// ����ֵ  ��NULL ���䲻�ɹ������򷵻ض�ʱ��������þ���Ľṹ�ɶ�ʱ��оƬ�Լ�����
// ˵��    :��һ����Ըշ���Ķ�ʱ�������ʱֹͣ���жϽ�ֹ������оƬ����ʵ��
//        �տ�ʼ����Ķ�ʱ��Ӧ���Ǹ������Զ��رյģ�������Ա����Լ���������
//        Ĭ�ϣ�ֹͣ�������첽�жϣ�reload,�жϽ�ֹ
// =============================================================================
ptu32_t TimerHard_Alloc(u32 cycle, fnTimerIsr timerisr)
{
	if(NULL == gs_tTimerHardChip.timerhardalloc)
	{
		return 0;
	}
	else
	{
		return gs_tTimerHardChip.timerhardalloc(cycle,timerisr);
	}
}

// =============================================================================
// �������ܣ�TimerHard_Free
//          HAL��ʱ���ͷ�
// ���������timerhandle�����ͷŶ�ʱ��
// �����������
// ����ֵ  ��true �ɹ� falseʧ�� 
// ˵��    : ����оƬ����ʵ��
// =============================================================================
bool_t  TimerHard_Free(ptu32_t timerhandle)
{
	if(NULL == gs_tTimerHardChip.timerhardfree)
	{
		return false;
	}
	else
	{
		return gs_tTimerHardChip.timerhardfree(timerhandle);
	}
}
// =============================================================================
// �������ܣ�TimerHard_Ctrl
//          ����Ӳ����ʱ��
// ���������timerhard�������ö�ʱ���� 
//          ctrlcmd, ��������
// ���������inoutpara������������������ݲ�ͬ���������
// ����ֵ  ��true �����ɹ� false����ʧ�� 
// ˵����ctrlcmd��Ӧ��inoutpara�����Զ���˵��
//	EN_TIMER_STARTCOUNT  //ʹ�ܼ�����inoutpara������
//	EN_TIMER_PAUSECOUNT, //ֹͣ������inoutpara������
//	EN_TIMER_SETCYCLE,     //�������ڣ�inoutparaΪu32,�����õ����ڣ�΢�룩
//	EN_TIMER_SETRELOAD,      //reloadģʽor not����inoutparaΪbool_t,true����reload
//	EN_TIMER_ENINT,        //�ж�ʹ�ܣ�inoutpara������
//	EN_TIMER_DISINT,       //�жϽ�ֹ��inoutpara������
//	EN_TIMER_SETINTPRO,       //�ж��������ã�inoutparaΪbool_t,true����ʵʱ�ź�
//	EN_TIMER_GETTIME,      //��ȡ��ʱʱ�䣬inoutparaΪu32 *,��λΪ΢��
//	EN_TIMER_GETCYCLE,     //��ȡ��ʱ���ڣ�inoutparaΪu32 *,��λΪ΢��
//	EN_TIMER_GETID,        //��ȡ��ʱ��ID��inoutparaΪu32 *����16λΪintID����16ΪtimerID
//	EN_TIMER_GETSTATE,     //��ȡ��ʱ��״̬��inoutparaΪu32 *
// =============================================================================
bool_t TimerHard_Ctrl(ptu32_t timerhandle, \
                      enum _ENUM_TIMER_CTRL_TYPE_ ctrlcmd, \
                      ptu32_t inoutpara)
{
	bool_t result;
	if(NULL == gs_tTimerHardChip.timerhardctrl)
	{
		result = false;
	}
	else
	{
		result = gs_tTimerHardChip.timerhardctrl(timerhandle, ctrlcmd, inoutpara);
	}
	return result;
}
