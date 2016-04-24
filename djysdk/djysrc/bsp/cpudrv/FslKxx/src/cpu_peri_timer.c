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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: �������ϵ�TIMER������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 1:55:12 PM/Mar 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�������ṩ��timer_core.cʹ�ã����Բ������������֮���

#include "config-prj.h"

#include "timer_hard.h"
#include "cpu_peri.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"


enum ENUM_FREESCALE_PIT
{
    EN_FREESCALE_PIT0=0,
    EN_FREESCALE_PIT1,
    EN_FREESCALE_PIT2,
    EN_FREESCALE_PIT3,
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct tagFslPitHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
};
#define CN_FSLPIT_NUM   (EN_FREESCALE_PIT3 +1)

#define s_FslPitReg ((PIT_Type *)PIT_BASE)
static struct tagFslPitHandle  stgFslPitHandle[CN_FSLPIT_NUM];
#define CN_PIT_MAX_COUNTER     0xFFFFFFFF
#define CN_PIT_MAX_TIME_US     (0xFFFFFFFF/50) //PIT���ʱΪ85��

//���λ����timer0 ��������
static u32  gs_dwFslPitBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_FSLPIT_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u32 sgHaltimerIrq[CN_FSLPIT_NUM]={   CN_INT_LINE_PIT0,\
                                            CN_INT_LINE_PIT1,\
                                            CN_INT_LINE_PIT2,\
                                            CN_INT_LINE_PIT3
                                              };
//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __FslPit_GetFirstZeroBit(u32 para)
{
   //����Ӳ��ָ������ȡ
    u8 i;

    for(i = 0; i < 32; i ++)
    {
        if(!(para & (0x80000000>>i)))
            break;
    }

    return i;
}

// =============================================================================
// ��������:__FslPit_Time2Counter
//          ���ṩ��ʱ�任��Ϊ��ʱ���ļ�����λ,PITʱ����50M�Ķ�ʱƵ��
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __FslPit_Time2Counter(u32 time, u32 *counter)
{
    u32 time_set;
    time_set = time > CN_PIT_MAX_TIME_US ? CN_PIT_MAX_TIME_US : time;
    if(CN_PIT_MAX_TIME_US > time)
    {
        time_set = time;
        *counter =  time_set * 50;
        return true;
    }
    else
    {
        time_set = CN_PIT_MAX_TIME_US;
        *counter =  time_set * 50;
        return false;
    }
}
// =============================================================================
// ��������:__FslPit_Counter2Time
//          ����ʱ���ļ�����λ����Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t __FslPit_Counter2Time(u32 counter,u32 *time)
{
    u64 time_ns;

    if(counter > CN_PIT_MAX_COUNTER)
    {
        *time = CN_PIT_MAX_TIME_US;
        return false;
    }
    else
    {
        time_ns = (u64)counter * 1000/50;
        *time = (u32)(time_ns/1000);
        return true;
    }
}

// =============================================================================
// ��������:__FslPit_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __FslPit_PauseCount(struct tagFslPitHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_FREESCALE_PIT3)
        {
            return false;
        }
        else
        {
            s_FslPitReg->CHANNEL[timerno].TCTRL &= ~PIT_TCTRL_TEN;
            s_FslPitReg->CHANNEL[timerno].TFLG  = PIT_TFLG_TIF_MASK;//CLEAR FLAG
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
// ��������:__FslPit_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __FslPit_StartCount(struct tagFslPitHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_FREESCALE_PIT3)
        {
            return false;
        }
        else
        {
            s_FslPitReg->CHANNEL[timerno].TFLG  = PIT_TFLG_TIF_MASK;
            s_FslPitReg->CHANNEL[timerno].TCTRL |= PIT_TCTRL_TEN;
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
// ��������:__FslPit_SetCycle
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ�΢�룩��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __FslPit_SetCycle(struct tagFslPitHandle  *timer, u32 cycle)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_FREESCALE_PIT3)
        {
            return false;
        }
        else
        {
            __FslPit_Time2Counter(cycle,&counter);
            s_FslPitReg->CHANNEL[timerno].LDVAL = counter - 1;
            cycle = counter * 20 /1000;//turn to us

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
// ��������:__FslPit_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __FslPit_SetAutoReload(struct tagFslPitHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno <= EN_FREESCALE_PIT3)
        {
            if(autoreload)
            {
                result = true;
                timer->timerstate = timer->timerstate | CN_TIMER_RELOAD;
            }
            else
            {
                result = false;     //PIT NOT SUPPORT AUTORELOAD
//              timer->timerstate = timer->timerstate &(~CN_TIMER_RELOAD);
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
// ��������:__FslPit_Alloc
//          ���䶨ʱ��
// �������:cycle����ʱ������
//          timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __FslPit_Alloc(u32 cycle,fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct tagFslPitHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __FslPit_GetFirstZeroBit(gs_dwFslPitBitmap);
    if(timerno < CN_FSLPIT_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwFslPitBitmap = gs_dwFslPitBitmap | (CN_FSLPIT_BITMAP_MSK >> timerno);
        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return 0;
    }
    irqline = sgHaltimerIrq[timerno];
    timer = &stgFslPitHandle[timerno];
    timer->cycle = cycle;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __FslPit_PauseCount(timer);
    __FslPit_SetCycle(timer,cycle);
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
// ��������:__FslPit_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __FslPit_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct tagFslPitHandle  *timer;
    timer = (struct tagFslPitHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_FSLPIT_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwFslPitBitmap = gs_dwFslPitBitmap &(~(CN_FSLPIT_BITMAP_MSK>> timerno));
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
// ��������:__FslPit_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __FslPit_SetIntPro(struct tagFslPitHandle  *timer, bool_t real_prior)
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
// ��������:__FslPit_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __FslPit_EnInt(struct tagFslPitHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        //PIT_TIE�ж�ʹ��
        s_FslPitReg->CHANNEL[timer->timerno].TFLG = 0x01;//���жϱ�־
        s_FslPitReg->CHANNEL[timer->timerno].TCTRL |= PIT_TCTRL_TIE;
        return Int_ContactLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__FslPit_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __FslPit_DisInt(struct tagFslPitHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        //PIT_TIE�ж�ʹ��
        s_FslPitReg->CHANNEL[timer->timerno].TFLG = 0x01;//���жϱ�־
        s_FslPitReg->CHANNEL[timer->timerno].TCTRL &= ~PIT_TCTRL_TIE;
        return Int_CutLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__FslPit_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��
// =============================================================================
bool_t __FslPit_GetTime(struct tagFslPitHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    u32 basecounter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_FREESCALE_PIT3)
        {
            return false;
        }
        else
        {
            basecounter = s_FslPitReg->CHANNEL[timerno].LDVAL;
            counter = s_FslPitReg->CHANNEL[timerno].CVAL;

            counter = basecounter - counter;
            *time = counter * 20/1000;
            return true;
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__FslPit_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __FslPit_CheckTimeout(struct tagFslPitHandle  *timer, bool_t *timeout)
{
    bool_t result;
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_FREESCALE_PIT3)
        {
            result = false;
        }
        else
        {
            result = true;

            counter = s_FslPitReg->CHANNEL[timerno].CVAL;
            if(!counter)
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
// �������ܣ�__FslPit_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __FslPit_GetID(struct tagFslPitHandle   *timer,u32 *timerId)
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
// �������ܣ�__FslPit_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(΢��)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __FslPit_GetCycle(struct tagFslPitHandle   *timer, u32 *cycle)
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
// �������ܣ�__FslPit_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __FslPit_GetState(struct tagFslPitHandle   *timer, u32 *timerflag)
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
// ��������:__FslPit_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __FslPit_Ctrl(ptu32_t timerhandle, \
                         enum _ENUM_TIMER_CTRL_TYPE_ ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result;
    struct tagFslPitHandle  *timer;
    timer = (struct tagFslPitHandle  *)timerhandle;
    if(NULL == timer)
    {
        result = false;
    }
    else
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __FslPit_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __FslPit_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __FslPit_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __FslPit_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __FslPit_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __FslPit_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __FslPit_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __FslPit_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __FslPit_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __FslPit_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __FslPit_GetState(timer, (u32 *)inoutpara);
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
void TimerHard_ModuleInit(void)
{
    struct tagTimerChip  FslPittimer;
    u8 loop;
    //�������ĳ�ʼ��
    //1.PIT��ʼ��
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    s_FslPitReg->MCR = 0x01;    //enable the module
    for(loop = 0; loop < 4; loop++)
    {
        s_FslPitReg->CHANNEL[loop].TFLG = 0x01; //clear the flag
    }

    FslPittimer.chipname = "FslPit";
    FslPittimer.timerhardalloc = __FslPit_Alloc;
    FslPittimer.timerhardfree = __FslPit_Free;
    FslPittimer.timerhardctrl = __FslPit_Ctrl;
    TimerHard_RegisterChip(&FslPittimer);

    return ;
}

