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

#include "cpu_peri.h"
#include "int.h"
#include "cpu.h"
#include "int_hard.h"
#include "cpu_peri_int_line.h"
#include "timer_hard.h"
#include "timer.h"

#define TCR_CE_MASK       (1<<0)
#define TCR_CR_MASK       (1<<1)

#define MCR_MR0I_MASK     (1<<0)
#define MCR_MR0R_MASK     (1<<1)
#define MCR_MR0S_MASK     (1<<2)

#define IR_MR0_INT_FLAG_MASK   (1<<0)

#define CN_TIMER_MAX_CYCLE (0xFFFFFFFF)

// =============================================================================
#define tagTimerReg             LPC_TIM_TypeDef
#define CN_TIMER0_BASE          LPC_TIM0_BASE
#define CN_TIMER1_BASE          LPC_TIM1_BASE
#define CN_TIMER2_BASE          LPC_TIM2_BASE
#define CN_TIMER3_BASE          LPC_TIM3_BASE
// =============================================================================
static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                            (tagTimerReg *)CN_TIMER0_BASE,
                                            (tagTimerReg *)CN_TIMER1_BASE,
                                            (tagTimerReg *)CN_TIMER2_BASE,
                                            (tagTimerReg *)CN_TIMER3_BASE};

enum ENUM_LPC_TIMER
{
    EN_LPCTIMER_0=0,
    EN_LPCTIMER_1,
    EN_LPCTIMER_2,
    EN_LPCTIMER_3,
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct tagLPCTimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
};
#define CN_LPCTIMER_NUM   (EN_LPCTIMER_3 +1)


static struct tagLPCTimerHandle  stgTimerHandle[CN_LPCTIMER_NUM];

//���λ����timer0 ��������
static u32  gs_dwLPCTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_LPCTIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u32 sgHaltimerIrq[CN_LPCTIMER_NUM]={CN_INT_LINE_TIMER0,\
                                            CN_INT_LINE_TIMER1,\
                                            CN_INT_LINE_TIMER2,\
                                            CN_INT_LINE_TIMER3};
//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __LPCTimer_GetFirstZeroBit(u32 para)
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
// ��������:__LPCTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __LPCTimer_PauseCount(struct tagLPCTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_LPCTIMER_3)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->MCR &= ~MCR_MR0I_MASK;
            tg_TIMER_Reg[timerno]->IR  |= IR_MR0_INT_FLAG_MASK;
            tg_TIMER_Reg[timerno]->TCR &= ~TCR_CE_MASK;
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
// ��������:__LPCTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __LPCTimer_StartCount(struct tagLPCTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_LPCTIMER_3)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->TC = 0;
            tg_TIMER_Reg[timerno]->IR  |= IR_MR0_INT_FLAG_MASK;
            tg_TIMER_Reg[timerno]->MCR |= MCR_MR0I_MASK;
            tg_TIMER_Reg[timerno]->TCR |= TCR_CE_MASK;
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
// ��������:__LPCTimer_SetCycle,LPC��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ�΢�룩��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __LPCTimer_SetCycle(struct tagLPCTimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_LPCTIMER_3)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->MR0 = cycle;
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
// ��������:__LPCTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __LPCTimer_SetAutoReload(struct tagLPCTimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if((timerno >= EN_LPCTIMER_0) &&(timerno < EN_LPCTIMER_3))
        {
            if(autoreload == true)
            {
                tg_TIMER_Reg[timerno]->MCR |= MCR_MR0R_MASK;//MR0R,reset PC
                tg_TIMER_Reg[timerno]->MCR &= ~MCR_MR0S_MASK;
            }
            else
            {
                tg_TIMER_Reg[timerno]->MCR &= ~MCR_MR0R_MASK;
                tg_TIMER_Reg[timerno]->MCR |= MCR_MR0S_MASK;
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
// ��������:__LPCTimer_Alloc
//          ���䶨ʱ��
// �������:cycle����ʱ������
//          timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __LPCTimer_Alloc(u32 cycle,fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct tagLPCTimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __LPCTimer_GetFirstZeroBit(gs_dwLPCTimerBitmap);
    if(timerno < CN_LPCTIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwLPCTimerBitmap = gs_dwLPCTimerBitmap | (CN_LPCTIMER_BITMAP_MSK<< timerno);
        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return 0;
    }
    irqline = sgHaltimerIrq[timerno];
    timer = &stgTimerHandle[timerno];
    timer->cycle = cycle;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __LPCTimer_PauseCount(timer);
    __LPCTimer_SetCycle(timer,cycle);
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
// ��������:__LPCTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __LPCTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct tagLPCTimerHandle  *timer;
    timer = (struct tagLPCTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_LPCTIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwLPCTimerBitmap = gs_dwLPCTimerBitmap &(~(CN_LPCTIMER_BITMAP_MSK<< timerno));
            //������ж�������������
            timer->timerstate = 0;
            Int_CutLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);

            Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������

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
// ��������:__LPCTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __LPCTimer_SetIntPro(struct tagLPCTimerHandle  *timer, bool_t real_prior)
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
// ��������:__LPCTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __LPCTimer_EnInt(struct tagLPCTimerHandle  *timer)
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
// ��������:__LPCTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __LPCTimer_DisInt(struct tagLPCTimerHandle  *timer)
{
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
// ��������:__LPCTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��
// =============================================================================
bool_t __LPCTimer_GetTime(struct tagLPCTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_LPCTIMER_3)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->TC;    //TCR����΢����
        return counter;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__LPCTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __LPCTimer_CheckTimeout(struct tagLPCTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_LPCTIMER_3)
        {
            result = false;
        }
        else
        {
            temp = tg_TIMER_Reg[timerno]->MR0;
            counter = tg_TIMER_Reg[timerno]->TC;
            if(temp == counter)
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
// �������ܣ�__LPCTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __LPCTimer_GetID(struct tagLPCTimerHandle   *timer,u32 *timerId)
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
// �������ܣ�__LPCTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(΢��)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __LPCTimer_GetCycle(struct tagLPCTimerHandle   *timer, u32 *cycle)
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
// �������ܣ�__LPCTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __LPCTimer_GetState(struct tagLPCTimerHandle   *timer, u32 *timerflag)
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
// ��������:__LPCTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __LPCTimer_Ctrl(ptu32_t timerhandle, \
                         enum _ENUM_TIMER_CTRL_TYPE_ ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result;
    struct tagLPCTimerHandle  *timer;
    timer = (struct tagLPCTimerHandle  *)timerhandle;
    if(NULL == timer)
    {
        result = false;
    }
    else
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __LPCTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __LPCTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __LPCTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __LPCTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __LPCTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __LPCTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __LPCTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __LPCTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __LPCTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __LPCTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __LPCTimer_GetState(timer, (u32 *)inoutpara);
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
void ModuleInstall_HardTimer(void)
{
    struct tagTimerChip  LPCtimer;
    u32 temp;
    //�������ĳ�ʼ������ʱ��ʹ���ķ�Ƶ����25Mʱ��
    LPC_SC->PCONP |= (1<<1)|(1<<2) | (1<<22)|(1<<23);//timer 0/1/2/3ʱ��ʹ��
    //
    for(temp = 0; temp < CN_LPCTIMER_NUM; temp++)
    {
        tg_TIMER_Reg[temp]->IR = 0;
        tg_TIMER_Reg[temp]->TCR = 0;
        tg_TIMER_Reg[temp]->CTCR &= ~(3<<0);    //ʹ�ö�ʱģʽ
        tg_TIMER_Reg[temp]->MR0 = 0xFFFFFFFE;   //��ʼΪ���
        tg_TIMER_Reg[temp]->MCR |= MCR_MR0S_MASK;//MR0S,MR0R�����ж�
        //25Mʱ�ӣ�Ԥ��ƵΪ25������PC����һ��Ϊ1uS������ʱһ����Сʱ
        tg_TIMER_Reg[temp]->PR = 24;
    }

    LPCtimer.chipname = "LPCTIMER";
    LPCtimer.timerhardalloc = __LPCTimer_Alloc;
    LPCtimer.timerhardfree = __LPCTimer_Free;
    LPCtimer.timerhardctrl = __LPCTimer_Ctrl;
    TimerHard_RegisterChip(&LPCtimer);

    return ;
}

