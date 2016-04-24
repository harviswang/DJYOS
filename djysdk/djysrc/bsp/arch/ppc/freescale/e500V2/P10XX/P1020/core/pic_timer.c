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
#include "stdint.h"
#include "stddef.h"
#include "timer_hard.h"
#include "Pic.h"
#include "config-prj.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"

enum ENUM_P1020PIC_TIMER
{
    EN_P1020PICTIMER_A0=0,
    EN_P1020PICTIMER_A1,
    EN_P1020PICTIMER_A2,
    EN_P1020PICTIMER_A3,
    EN_P1020PICTIMER_B0,
    EN_P1020PICTIMER_B1,
    EN_P1020PICTIMER_B2,
    EN_P1020PICTIMER_B3,
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct tagP1020PicTimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
};
#define CN_P1020PICTIMER_NUM   (EN_P1020PICTIMER_B3 +1)


static struct tagP1020PicTimerHandle  stgP1020TimerHandle[CN_P1020PICTIMER_NUM];

//���λ����timer0 ��������
static u32  gs_dwP1020PicTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_P1020PICTIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u32 sgHaltimerIrq[CN_P1020PICTIMER_NUM]={cn_int_line_gbtimer_a0,\
                                                cn_int_line_gbtimer_a1,\
                                                cn_int_line_gbtimer_a2,\
                                                cn_int_line_gbtimer_a3,\
                                                cn_int_line_gbtimer_b0,\
                                                cn_int_line_gbtimer_b1,\
                                                cn_int_line_gbtimer_b2,\
                                                cn_int_line_gbtimer_b3};
//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __P1020PicTimer_GetFirstZeroBit(u32 para)
{
   //����Ӳ��ָ������ȡ
    para = ~para;
    extern u8 __asm_find_f1bits(u32 para);
    return  __asm_find_f1bits(para);
}
// =============================================================================
// ��������:__P1020PicTimer_Time2Counter
//          ���ṩ��ʱ�任��Ϊ��ʱ���ļ�����λ
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __P1020PicTimer_Time2Counter(u32 time, u32 *counter)
{
    u32 time_max;
    u32 time_set;
    time_max = (cn_pic_timer_countermsk/1000) *cfg_core_tb_clk_time;
    time_set = time > time_max ? time_max : time;
    if(time_max > time)
    {
        time_set = time;
        *counter =  time_set *(1000/cfg_core_tb_clk_time);
        return true;
    }
    else
    {
        time_set = time_max;
        *counter =  time_set *(1000/cfg_core_tb_clk_time);
        return false;
    }
}
// =============================================================================
// ��������:__P1020PicTimer_Counter2Time
//          ����ʱ���ļ�����λ����Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __P1020PicTimer_Counter2Time(u32 counter,u32 *time)
{
    u32 time_max;
    u64 time_ns;
    time_max = (cn_pic_timer_countermsk/1000) *cfg_core_tb_clk_time;

    if(counter > cn_pic_timer_countermsk)
    {
        *time = time_max;
        return false;
    }
    else
    {
        time_ns = (u64)counter*cfg_core_tb_clk_time;
        *time = (u32)(time_ns/1000);
        return true;
    }
}
// =============================================================================
// ��������:__P1020PicTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __P1020PicTimer_PauseCount(struct tagP1020PicTimerHandle  *timer)
{
    u8 timerno;
    u32 temp;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_P1020PICTIMER_B3)
        {
            return false;
        }
        else
        {
            if(timerno > EN_P1020PICTIMER_A3)
            {
                temp = cn_pic_timer_gtbcrb_base + \
                        (timerno - EN_P1020PICTIMER_B0)*cn_pic_timer_gt_step;
                *((u32 *)temp) = *((u32 *)temp) | cn_pic_timer_msk;
            }
            else
            {
                temp = cn_pic_timer_gtbcra_base + \
                        (timerno - EN_P1020PICTIMER_A0)*cn_pic_timer_gt_step;
                *((u32 *)temp) = *((u32 *)temp) | cn_pic_timer_msk;
            }
            temp = *((u32 *)temp);//do the sync
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
// ��������:__P1020PicTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __P1020PicTimer_StartCount(struct tagP1020PicTimerHandle  *timer)
{
    u8 timerno;
    u32 temp;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_P1020PICTIMER_B3)
        {
            return false;
        }
        else
        {
            if(timerno > EN_P1020PICTIMER_A3)
            {
                temp = cn_pic_timer_gtbcrb_base + \
                        (timerno - EN_P1020PICTIMER_B0)*cn_pic_timer_gt_step;
                *((u32 *)temp) = *((u32 *)temp) &(~cn_pic_timer_msk);
            }
            else
            {
                temp = cn_pic_timer_gtbcra_base + \
                        (timerno - EN_P1020PICTIMER_A0)*cn_pic_timer_gt_step;
                *((u32 *)temp) = *((u32 *)temp) &(~cn_pic_timer_msk);
            }
            temp = *((u32 *)temp);//do the sync
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
// ��������:__P1020PicTimer_SetCycle
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ�΢�룩��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __P1020PicTimer_SetCycle(struct tagP1020PicTimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        __P1020PicTimer_Time2Counter(cycle,&counter);

        timerno = timer->timerno;
        if(timerno > EN_P1020PICTIMER_B3)
        {
            return false;
        }
        else
        {
            if(timerno > EN_P1020PICTIMER_A3)
            {
                temp = cn_pic_timer_gtbcrb_base + \
                        (timerno - EN_P1020PICTIMER_B0)*cn_pic_timer_gt_step;
                *((u32 *)temp) = (*((u32 *)temp) &(~cn_pic_timer_countermsk)) |
                        (counter & cn_pic_timer_countermsk);
            }
            else
            {
                temp = cn_pic_timer_gtbcra_base + \
                        (timerno - EN_P1020PICTIMER_A0)*cn_pic_timer_gt_step;
                *((u32 *)temp) = (*((u32 *)temp) &(~cn_pic_timer_countermsk)) |
                        (counter & cn_pic_timer_countermsk);
            }
            temp = *((u32 *)temp);//do the sync

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
// ��������:__P1020PicTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __P1020PicTimer_SetAutoReload(struct tagP1020PicTimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    ptu32_t temp;
    u32 value;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if((timerno >= EN_P1020PICTIMER_A0) &&(timerno < EN_P1020PICTIMER_A3))
        {
            result = true;
            timerno = timerno - EN_P1020PICTIMER_A0;
            temp = (ptu32_t)cn_pic_timer_tcra;
            value = read32(temp);
            if(autoreload)
            {
                value = value &(u32)(~((1<<24)<<timerno));
                timer->timerstate = timer->timerstate | CN_TIMER_RELOAD;
            }
            else
            {
                value = value |(u32)((1<<24)<<timerno);
                timer->timerstate = timer->timerstate &(~CN_TIMER_RELOAD);
            }
            write32(temp, value);
        }
        else if((timerno >= EN_P1020PICTIMER_B0) &&(timerno < EN_P1020PICTIMER_B3))
        {
            result = true;
            timerno = timerno - EN_P1020PICTIMER_B0;
            temp = (ptu32_t)cn_pic_timer_tcrb;
            value = read32(temp);
            if(autoreload)
            {
                value = value &(u32)(~((1<<24)<<timerno));
                timer->timerstate = timer->timerstate | CN_TIMER_RELOAD;
            }
            else
            {
                value = value |(u32)((1<<24)<<timerno);
                timer->timerstate = timer->timerstate &(~CN_TIMER_RELOAD);
            }
            write32(temp, value);
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
// ��������:__P1020PicTimer_Alloc
//          ���䶨ʱ��
// �������:cycle����ʱ������
//          timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __P1020PicTimer_Alloc(u32 cycle,fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct tagP1020PicTimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __P1020PicTimer_GetFirstZeroBit(gs_dwP1020PicTimerBitmap);
    if(timerno < CN_P1020PICTIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwP1020PicTimerBitmap = gs_dwP1020PicTimerBitmap | (CN_P1020PICTIMER_BITMAP_MSK<< timerno);
        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return NULL;
    }
    irqline = sgHaltimerIrq[timerno];
    timer = &stgP1020TimerHandle[timerno];
    timer->cycle = cycle;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __P1020PicTimer_PauseCount(timer);
    __P1020PicTimer_SetCycle(timer,cycle);
    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
    Int_CutLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);
    Int_SetClearType(irqline,CN_INT_CLEAR_PRE);
    Int_IsrConnect(irqline, timerisr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������:__P1020PicTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __P1020PicTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct tagP1020PicTimerHandle  *timer;
    timer = (struct tagP1020PicTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_P1020PICTIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwP1020PicTimerBitmap = gs_dwP1020PicTimerBitmap &(~(CN_P1020PICTIMER_BITMAP_MSK<< timerno));
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
// ��������:__P1020PicTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __P1020PicTimer_SetIntPro(struct tagP1020PicTimerHandle  *timer, bool_t real_prior)
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
// ��������:__P1020PicTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __P1020PicTimer_EnInt(struct tagP1020PicTimerHandle  *timer)
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
// ��������:__P1020PicTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __P1020PicTimer_DisInt(struct tagP1020PicTimerHandle  *timer)
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
// ��������:__P1020PicTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��
// =============================================================================
bool_t __P1020PicTimer_GetTime(struct tagP1020PicTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 temp;
    u32 counter;
    u32 basecounter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_P1020PICTIMER_B3)
        {
            return false;
        }
        else
        {
            if(timerno > EN_P1020PICTIMER_A3)
            {
                //got the current counter
                temp = cn_pic_timer_gtccrb_base + \
                        (timerno - EN_P1020PICTIMER_B0)*cn_pic_timer_gt_step;
                counter = (*(u32 *)temp)&cn_pic_timer_countermsk;
                //got the base counter
                temp = cn_pic_timer_gtbcrb_base + \
                        (timerno - EN_P1020PICTIMER_B0)*cn_pic_timer_gt_step;
                basecounter = (*(u32 *)temp)&cn_pic_timer_countermsk;

                counter = basecounter - counter;
            }
            else
            {
                temp = cn_pic_timer_gtccra_base + \
                        (timerno - EN_P1020PICTIMER_A0)*cn_pic_timer_gt_step;
                counter = (*(u32 *)temp)&cn_pic_timer_countermsk ;
                //got the base counter
                temp = cn_pic_timer_gtbcra_base + \
                        (timerno - EN_P1020PICTIMER_A0)*cn_pic_timer_gt_step;
                basecounter = (*(u32 *)temp)&cn_pic_timer_countermsk;

                counter = basecounter - counter;

            }
            return __P1020PicTimer_Counter2Time(counter, time);
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__P1020PicTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __P1020PicTimer_CheckTimeout(struct tagP1020PicTimerHandle  *timer, bool_t *timeout)
{
    bool_t result;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > EN_P1020PICTIMER_B3)
        {
            result = false;
        }
        else
        {
            result = true;
            if(timerno > EN_P1020PICTIMER_A3)
            {
                //got the current counter
                temp = cn_pic_timer_gtccrb_base + \
                        (timerno - EN_P1020PICTIMER_B0)*cn_pic_timer_gt_step;
            }
            else
            {
                temp = cn_pic_timer_gtccra_base + \
                        (timerno - EN_P1020PICTIMER_A0)*cn_pic_timer_gt_step;
            }
            counter = (*(u32 *)temp)&cn_pic_timer_togmsk;
            if(counter)
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
// �������ܣ�__P1020PicTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __P1020PicTimer_GetID(struct tagP1020PicTimerHandle   *timer,u32 *timerId)
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
// �������ܣ�__P1020PicTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(΢��)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __P1020PicTimer_GetCycle(struct tagP1020PicTimerHandle   *timer, u32 *cycle)
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
// �������ܣ�__P1020PicTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __P1020PicTimer_GetState(struct tagP1020PicTimerHandle   *timer, u32 *timerflag)
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
// ��������:__P1020PicTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __P1020PicTimer_Ctrl(ptu32_t timerhandle, \
                         enum _ENUM_TIMER_CTRL_TYPE_ ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result;
    struct tagP1020PicTimerHandle  *timer;
    timer = (struct tagP1020PicTimerHandle  *)timerhandle;
    if(NULL == timer)
    {
        result = false;
    }
    else
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __P1020PicTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __P1020PicTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __P1020PicTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __P1020PicTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __P1020PicTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __P1020PicTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __P1020PicTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __P1020PicTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __P1020PicTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __P1020PicTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __P1020PicTimer_GetState(timer, (u32 *)inoutpara);
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
void module_init_pictimer(void)
{
    struct tagTimerChip  p1020pictimer;
    u32 temp;
    //�������ĳ�ʼ��
    *((u32 *)cn_pic_timer_tcra) = cn_pic_timer_tcr_set;
    temp = *((u32 *)cn_pic_timer_tcra);//sync
    *((u32 *)cn_pic_timer_tcrb) = cn_pic_timer_tcr_set;
    temp = *((u32 *)cn_pic_timer_tcrb);//sync

    *((u32 *)cn_pic_timer_tfrra) = cfg_core_tb_clk;  //same as the tb
    temp = *((u32 *)cn_pic_timer_tfrra);
    *((u32 *)cn_pic_timer_tfrrb) = cfg_core_tb_clk;  //same as the tb
    temp = *((u32 *)cn_pic_timer_tfrra);

    //all the timers are disabled,not count
    //initial the group a
    for(temp =0; temp < cn_pic_timer_gnumber; temp++)
    {
         *((u32 *)(cn_pic_timer_gtbcra_base + temp * cn_pic_timer_gt_step)) = cn_pic_timer_msk;
    }

    //all the timers are disabled,not count
    //initial the group b
    for(temp =0; temp < cn_pic_timer_gnumber; temp++)
    {
         *((u32 *)(cn_pic_timer_gtbcrb_base + temp * cn_pic_timer_gt_step)) = cn_pic_timer_msk;
    }
    p1020pictimer.chipname = "P1020PICTIMER";
    p1020pictimer.timerhardalloc = __P1020PicTimer_Alloc;
    p1020pictimer.timerhardfree = __P1020PicTimer_Free;
    p1020pictimer.timerhardctrl = __P1020PicTimer_Ctrl;

    TimerHard_RegisterChip(&p1020pictimer);

    return ;
}
