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
// ģ������: PIC�������ϵ�TIMER������
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 1:55:12 PM/Mar 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�������ṩ��timer_core.cʹ�ã����Բ������������֮���

#include "config-prj.h"
#if (CN_CFG_SYSTIMER == 1)
#include "cpu_peri.h"
#include "int.h"
#include "cpu.h"
#include "int_hard.h"
#include "cpu_peri_int_line.h"
#include "timer_hard.h"

#define TC_CCR_CLKEN       (1<<0)
#define TC_CCR_CLKDIS      (1<<1)
#define TC_CCR_SWTRG       (1<<2)
#define TC_CMR_CPCTRG      (1<<14)
#define TC_SR_CPCR         (1<<4)
#define TC_IDR_CPCS		   (1<<4)
#define TC_IER_CPCS		   (1<<4)

#define CN_RATIO_UPTO_US    50	//��ʱ��ʱ��ΪMCK/2=100M/2=50M,��50��ʱ������Ϊ1us
#define CN_TIMER_MAX_CYCLE (0xFFFF/CN_RATIO_UPTO_US)

// =============================================================================
#define CN_TC0_BASE        	0x40010000U
#define CN_TC1_BASE        	0x40014000U
// =============================================================================
static tagTcReg volatile * const tg_TIMER_Reg[] = {
											(tagTcReg *)CN_TC0_BASE,
											(tagTcReg *)CN_TC1_BASE};

enum ENUM_ATMEL_TIMER
{
	EN_TC0CH_0=0,
	EN_TC0CH_1,
	EN_TC0CH_2,
	EN_TC1CH_0,
	EN_TC1CH_1,
	EN_TC1CH_2,
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct tagAtTimerHandle
{
	u32     timerno;          //��ʱ����
	u32     irqline;          //�жϺ�
	u32     cycle;            //��ʱ����
	u32     timerstate;       //��ʱ����ʶ
};
#define CN_ATTIMER_NUM   (EN_TC1CH_2 +1)


static struct tagAtTimerHandle  stgTimerHandle[CN_ATTIMER_NUM];

//���λ����timer0 ��������
static u32  gs_dwAtTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_ATTIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u32 sgHaltimerIrq[CN_ATTIMER_NUM]={CN_INT_LINE_TC0,\
											CN_INT_LINE_TC1,\
											CN_INT_LINE_TC2,\
											CN_INT_LINE_TC3,\
											CN_INT_LINE_TC4,\
											CN_INT_LINE_TC5};
//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __AtTimer_GetFirstZeroBit(u32 para)
{
	u8 i;
	for(i = 0; i < 32; i++)
	{
		if(!(para & (1<<(31-i))))
			break;
	}
	return i;
}
// =============================================================================
// ��������:__AtTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __AtTimer_PauseCount(struct tagAtTimerHandle  *timer)
{
	u8 timerno;
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		if(timerno > EN_TC1CH_2)
		{
			return false;
		}
		else
		{
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_CCR = TC_CCR_CLKDIS;
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_SR;
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_IDR = TC_IDR_CPCS;
			timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENCOUNT);
			return true;
		}
	}
	else
	{
		return false;
	}
}
// =============================================================================
// ��������:__AtTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __AtTimer_StartCount(struct tagAtTimerHandle  *timer)
{
	u8 timerno;
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		if(timerno > EN_TC1CH_2)
		{
			return false;
		}
		else
		{
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_SR;
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_IMR = TC_CMR_CPCTRG;
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_IER = TC_IER_CPCS;
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_CCR
					= TC_CCR_CLKEN | TC_CCR_SWTRG;
			timer->timerstate = (timer->timerstate)| (CN_TIMER_ENCOUNT);

			return true;
		}
	}
	else
	{
		return false;
	}

}

// =============================================================================
// ��������:__AtTimer_SetCycle,LPC��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ�΢�룩��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __AtTimer_SetCycle(struct tagAtTimerHandle  *timer, u32 cycle)
{
	u8 timerno;
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		if(timerno > EN_TC1CH_2)
		{
			return false;
		}
		else
		{
			if(cycle > CN_TIMER_MAX_CYCLE)
				cycle = CN_TIMER_MAX_CYCLE;
			tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_RC =
					cycle *CN_RATIO_UPTO_US;	//�趨����
			timer->cycle = cycle;

			return true;
		}
	}
	else
	{
		return false;
	}
}
// =============================================================================
// ��������:__AtTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __AtTimer_SetAutoReload(struct tagAtTimerHandle  *timer, bool_t autoreload)
{
	bool_t result;
	u8 timerno;
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		if(timerno < EN_TC1CH_2)
		{
			if(autoreload == true)
			{
//				tg_TIMER_Reg[timerno]->MCR |= MCR_MR0R_MASK;//MR0R,reset PC
//				tg_TIMER_Reg[timerno]->MCR &= ~MCR_MR0S_MASK;
			}
			else
			{
//				tg_TIMER_Reg[timerno]->MCR &= ~MCR_MR0R_MASK;
//				tg_TIMER_Reg[timerno]->MCR |= MCR_MR0S_MASK;
			}
		}
		else
		{
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}
// =============================================================================
// ��������:__AtTimer_Alloc
//          ���䶨ʱ��
// �������:cycle����ʱ������
//          timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __AtTimer_Alloc(u32 cycle,fnTimerIsr timerisr)
{
	u8 timerno;
	u8 irqline;
	struct tagAtTimerHandle  *timer;
	ptu32_t timerhandle;
	//ԭ�Ӳ�������ֹ��Դ����
	atom_low_t  timeratom;
	timeratom = Int_LowAtomStart();

	//Ѱ�ҿ��е�timer
	timerno = __AtTimer_GetFirstZeroBit(gs_dwAtTimerBitmap);
	if(timerno < CN_ATTIMER_NUM)//���п��еģ������ñ�־λ
	{
		gs_dwAtTimerBitmap = gs_dwAtTimerBitmap | (CN_ATTIMER_BITMAP_MSK<< timerno);
		Int_LowAtomEnd(timeratom);	//ԭ�Ӳ������
	}
	else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
	{
		Int_LowAtomEnd(timeratom);	 //ԭ�Ӳ������
		return NULL;
	}

	PMC_EnablePeripheral(CN_PERI_ID_TC0 + timerno);

	irqline = sgHaltimerIrq[timerno];
	timer = &stgTimerHandle[timerno];
	timer->cycle = cycle;
	timer->timerno = timerno;
	timer->irqline = irqline;
	timer->timerstate = CN_TIMER_ENUSE;
	//���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
	//�����úö�ʱ������
	__AtTimer_PauseCount(timer);
	__AtTimer_SetCycle(timer,cycle);
	//���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
	Int_CutLine(irqline);
	Int_IsrDisConnect(irqline);
	Int_EvttDisConnect(irqline);
	Int_SettoAsynSignal(irqline);
	Int_IsrConnect(irqline, timerisr);
	timerhandle = (ptu32_t)timer;

	return timerhandle;
}


// =============================================================================
// ��������:__AtTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __AtTimer_Free(ptu32_t timerhandle)
{
	u8 timerno;
	u8 irqline;
	atom_low_t  timeratom;	//����������Դ
	struct tagAtTimerHandle  *timer;
	timer = (struct tagAtTimerHandle  *)timerhandle;

	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		irqline = timer->irqline;
		if(timerno < CN_ATTIMER_NUM)//���п��еģ������ñ�־λ
		{	    //�޸�ȫ�ֱ�־һ����ԭ���Ե�
			timeratom = Int_LowAtomStart();
			gs_dwAtTimerBitmap = gs_dwAtTimerBitmap &(~(CN_ATTIMER_BITMAP_MSK<< timerno));
			//������ж�������������
			timer->timerstate = 0;
			Int_CutLine(irqline);
			Int_IsrDisConnect(irqline);
			Int_EvttDisConnect(irqline);

			Int_LowAtomEnd(timeratom);	//ԭ�Ӳ������

			return true;
		}
		else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
		{
			return false;
		}

	}
	else
	{
		return false;
	}
}


// =============================================================================
// ��������:__AtTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __AtTimer_SetIntPro(struct tagAtTimerHandle  *timer, bool_t real_prior)
{
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		if(real_prior)
		{
			timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
			Int_EvttDisConnect(timer->irqline);
			return Int_SettoReal(timer->irqline);
		}
		else
		{
			timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
			return Int_SettoAsynSignal(timer->irqline);
		}
	}
	else
	{
		return false;
	}
}

// =============================================================================
// ��������:__AtTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __AtTimer_EnInt(struct tagAtTimerHandle  *timer)
{
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
		return Int_ContactLine(timer->irqline);
	}
	else
	{
		return false;
	}
}
// =============================================================================
// ��������:__AtTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __AtTimer_DisInt(struct tagAtTimerHandle  *timer)
{
	u8 timerno;
	timerno = timer->timerno;

	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
		return Int_CutLine(timer->irqline);
	}
	else
	{
		return false;
	}
}
// =============================================================================
// ��������:__AtTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��
// =============================================================================
bool_t __AtTimer_GetTime(struct tagAtTimerHandle  *timer, u32 *time)
{
	u8 timerno;
	u32 counter;
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		if(timerno > EN_TC1CH_2)
		{
			return false;
		}

		counter = tg_TIMER_Reg[timerno/3]->TC_CHANNEL[timerno%3].TC_RC;
		counter = timer->cycle - counter * CN_RATIO_UPTO_US;

		return counter;
	}
	else
	{
		return false;
	}
}
// =============================================================================
// ��������:__AtTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __AtTimer_CheckTimeout(struct tagAtTimerHandle  *timer, bool_t *timeout)
{
	bool_t result=true;
	u8 timerno;
	u32 temp;
	u32 status;
	if(timer->timerstate & CN_TIMER_ENUSE)
	{
		timerno = timer->timerno;
		if(timerno > EN_TC1CH_2)
		{
			result = false;
		}
		else
		{
			status = tg_TIMER_Reg[timerno/3]->TC_CHANNEL[temp%3].TC_SR;
			if(status & TC_SR_CPCR)
			{
				*timeout = true;
			}
			else
			{
				*timeout = false;
			}
		}
	}
	else
	{
		result = false;
	}
	return result;
}

// =============================================================================
// �������ܣ�__AtTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __AtTimer_GetID(struct tagAtTimerHandle   *timer,u32 *timerId)
{
	u16 irqno;
	u16 timerno;

	if(NULL == timer)
	{
		return false;
	}
	else
	{
		irqno = (u16)timer->timerno;
		timerno = (u16)timer->irqline;
		*timerId = (timerno<<16)|(irqno);
		return true;
	}
}

// =============================================================================
// �������ܣ�__AtTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(΢��)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __AtTimer_GetCycle(struct tagAtTimerHandle   *timer, u32 *cycle)
{
	if(NULL == timer)
	{
		return false;
	}
	else
	{
		*cycle = timer->cycle;
		return true;
	}
}
// =============================================================================
// �������ܣ�__AtTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __AtTimer_GetState(struct tagAtTimerHandle   *timer, u32 *timerflag)
{

	if(NULL == timer)
	{
		return false;
	}
	else
	{
		*timerflag = timer->timerstate;
		return true;
	}
}
// =============================================================================
// ��������:__AtTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __AtTimer_Ctrl(ptu32_t timerhandle, \
                         enum _ENUM_TIMER_CTRL_TYPE_ ctrlcmd, \
                         ptu32_t inoutpara)
{
	bool_t result;
	struct tagAtTimerHandle  *timer;
	timer = (struct tagAtTimerHandle  *)timerhandle;
	if(NULL == timer)
	{
		result = false;
	}
	else
	{
		switch(ctrlcmd)
		{
			case EN_TIMER_STARTCOUNT:
				result = __AtTimer_StartCount(timer);
				break;
			case EN_TIMER_PAUSECOUNT:
				result = __AtTimer_PauseCount(timer);
				break;
			case EN_TIMER_SETCYCLE:
				result = __AtTimer_SetCycle(timer,(u32)inoutpara);
				break;
			case EN_TIMER_SETRELOAD:
				result = __AtTimer_SetAutoReload(timer, (bool_t)inoutpara);
				break;
			case EN_TIMER_ENINT:
				result = __AtTimer_EnInt(timer);
				break;
			case EN_TIMER_DISINT:
				result = __AtTimer_DisInt(timer);
				break;
			case EN_TIMER_SETINTPRO:
				result = __AtTimer_SetIntPro(timer, (bool_t)inoutpara);
				break;
			case EN_TIMER_GETTIME:
				result = __AtTimer_GetTime(timer, (u32 *)inoutpara);
				break;
			case EN_TIMER_GETCYCLE:
				result = __AtTimer_GetCycle(timer, (u32 *)inoutpara);
				break;
			case EN_TIMER_GETID:
				result = __AtTimer_GetID(timer, (u32 *)inoutpara);
				break;
			case EN_TIMER_GETSTATE:
				result = __AtTimer_GetState(timer, (u32 *)inoutpara);
				break;
			default:
				break;
		};
	}

	return result;
}

// =============================================================================
// ��������:module_init_timer
//          P1020��PICtimer��ʼ��
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
void Timer_ModuleInit(void)
{
	struct tagTimerChip  LPCtimer;
	u32 temp,temp1;

	for(temp1 = 0; temp1 < 2; temp1++)
	{
		for(temp = 0; temp < 3; temp++)
		{
			tg_TIMER_Reg[temp1]->TC_CHANNEL[temp].TC_CCR = TC_CCR_CLKDIS;
			tg_TIMER_Reg[temp1]->TC_CHANNEL[temp].TC_IDR = 0xFFFFFFFF;
			tg_TIMER_Reg[temp1]->TC_CHANNEL[temp].TC_SR;
			tg_TIMER_Reg[temp1]->TC_CHANNEL[temp].TC_IMR = TC_CMR_CPCTRG;
		}
	}

	LPCtimer.chipname = "ATTIMER";
	LPCtimer.timerhardalloc = __AtTimer_Alloc;
	LPCtimer.timerhardfree = __AtTimer_Free;
	LPCtimer.timerhardctrl = __AtTimer_Ctrl;
    TimerHard_RegisterChip(&LPCtimer);

    return ;
}
#endif
