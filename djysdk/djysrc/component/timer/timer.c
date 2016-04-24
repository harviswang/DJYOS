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
// ģ������: �����ʱ��ģ��
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:36:18 PM/Apr 1, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:���ļ���Ҫ�ṩһ��Ӳ����ʱ������Ҫһ���������ʱ��64λ��ʱ��
#include "config-prj.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "os.h"

#include "timer.h"
#include "timer_hard.h"

#define CN_TIMER_ALARMNEVER      CN_LIMIT_SINT64             //����ʱ��
#define CN_TIMERSOFT_PRECISION   ((1E+10)/CN_CFG_MCLK)       //�����ʱ�����ȣ���λ΢��

static bool_t  sbUsingHardTimer = false;
static tagTimerSoft* ptTimerSoftHead = NULL;                 //��ʱ������ͷ
static tagTimerSoft* ptTimerSoftTail = NULL;                 //�����ʱ����β
static tagTimerSoft           *ptTimerSoftMem = NULL;        //��̬�����ڴ�
static struct tagMemCellPool  *ptTimerSoftMemPool = NULL;    //�ڴ��ͷָ�롣
static struct tagMutexLCB     *ptTimerSoftQSync =NULL;       //API��Դ������

//ʹ�õ�Ӳ����ʱ����ָ��ʹ��Ӳ����ʱ����
static ptu32_t sgTimerHardDefault = (ptu32_t)NULL;           //������ʱ��32λ��ʱ��
//ʹ���߳�ģ��Ӳ����ʱ��
enum __EN_TIMERSOFT_CMD
{
    EN_TIMERSOFT_ADD = EN_TIMER_LASTOPCODE+1,
    EN_TIMERSOFT_REMOVE,
};

static struct tagMsgQueue    *ptTimerSoftMsgQ = NULL;
typedef struct
{
    tagTimerSoft*     timer;
    u32               type;
    u32               para;
}tagTimerSoftMsg;
#define CN_TIMERSOFT_MSGLEN  0X10
// =============================================================================
// �������ܣ�__TimerSoft_Get64Time
//           ��ȡS64timer��ʱ��
// �������: ��
// ���������time,��λΪ΢��
// ����ֵ  ��true �ɹ�  falseʧ��
// ˵��    ���ڲ����ú���,ע��ú�������ֲ���к�����S64timer,����Ϊ��ͣ������ϵ�
//           64λ��ʱ����ÿ��ϵͳ����CPU�����õĶ���һ��
// =============================================================================
bool_t __TimerSoft_Get64Time(s64 *time)
{
    *time = DjyGetTime();
    return true;
}
// =============================================================================
// �������ܣ�__TimerSoft_ExeAlarmHandler
//          ִ��timersoft�Ĺ��Ӻ���
// ���������timersoft,��ִ�еĶ�ʱ��.
// �����������
// ����ֵ  ��true �ɹ�  falseʧ��
// ˵��    ���ڲ����ú�����û��HOOK��ִ��
// =============================================================================
bool_t __TimerSoft_ExeAlarmHandler(tagTimerSoft *timer)
{
    if(NULL != timer->isr)
    {
        timer->isr(timer);
    }
    return true;
}

// =============================================================================
// �������ܣ�__TimerSoft_Remove
//          ��ָ���Ķ�ʱ���Ӷ�ʱ���������Ƴ�
// ���������timer,���Ƴ��Ķ�ʱ��
// �����������
// ����ֵ  �������Ķ�ʱ��
// ˵��    ��
// =============================================================================
void __TimerSoft_Remove(tagTimerSoft *timer)
{
    if((timer == ptTimerSoftHead)&&(timer == ptTimerSoftTail)) //only one
    {
        ptTimerSoftHead = NULL;
        ptTimerSoftTail = NULL;
    }
    else if(timer == ptTimerSoftHead)  //head to remove
    {
        ptTimerSoftHead = ptTimerSoftHead->nxt;
        ptTimerSoftHead->pre = NULL;
    }
    else if(timer == ptTimerSoftTail) //tail to remove
    {
        ptTimerSoftTail = ptTimerSoftTail->pre;
        ptTimerSoftTail->nxt = NULL;
    }
    else //normal to remove
    {
        timer->pre->nxt = timer->nxt;
        timer->nxt->pre = timer->pre;
    }
    return;
}
// =============================================================================
// �������ܣ�__TimerSoft_Add
//          ��Ӷ�ʱ������ʱ������
// ���������timer,����ӵĶ�ʱ��
// �����������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
void __TimerSoft_Add(tagTimerSoft *timer)
{
    tagTimerSoft *tmp;
    if(NULL == ptTimerSoftHead) //the queue is empty
    {
        ptTimerSoftHead = timer;
        ptTimerSoftTail = timer;
        timer->pre = NULL;
        timer->nxt = NULL;
    }
    else        //find proper place to add
    {
        tmp = ptTimerSoftHead;
        while((NULL != tmp)&&(tmp->deadline < timer->deadline))
        {
            tmp = tmp->nxt;
        }
        if(NULL == tmp) //add the timer to the tail
        {
            timer->pre = ptTimerSoftTail;
            timer->nxt = NULL;
            ptTimerSoftTail->nxt = timer;
            ptTimerSoftTail = timer;
        }
        else
        {
            timer->pre = tmp->pre;
            timer->nxt = tmp;
            if(NULL != tmp->pre)
            {
                tmp->pre->nxt = timer;
            }
            else
            {
                ptTimerSoftHead = timer;
            }
            tmp->pre = timer;
        }
    }
    return;
}

// =============================================================================
// �������ܣ�__TimerSoft_AddLast
//          ��Ӷ�ʱ������ʱ������β
// ���������timer,����ӵĶ�ʱ��
// �����������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
void __TimerSoft_AddLast(tagTimerSoft *timer)
{
    if(NULL == ptTimerSoftHead) //the queue is empty
    {
        ptTimerSoftHead = timer;
        ptTimerSoftTail = timer;
        timer->pre = NULL;
        timer->nxt = NULL;
    }
    else
    {
        timer->pre = ptTimerSoftTail;
        timer->nxt = NULL;
        ptTimerSoftTail->nxt = timer;
        ptTimerSoftTail = timer;
    }
    return;
}

// =============================================================================
// �������ܣ�__TimerSoft_ChkTimeout
// �����������
// �����������
// ����ֵ  ��true���ö�ʱ����ʱ������ö�ʱ������ʱ
// ˵��     :
// =============================================================================
bool_t  __TimerSoft_ChkTimeout(tagTimerSoft *timer, s64 timenow)
{
    bool_t result = false;
    if((timer->deadline -CN_TIMERSOFT_PRECISION)<timenow)
    {
        result = true;
    }
    return result;
}
// =============================================================================
// �������ܣ�__TimerSoft_DealTimeout
//          ����ʱ�Ķ�ʱ������
// ���������
// ���������
// ����ֵ  ���´εȴ���ʱ��
// ˵��    ���ڲ����ú���
// =============================================================================
u32 __TimerSoft_DealTimeout(void)
{
    u32           result;
    s64           timenow;
    tagTimerSoft *timer;

    result = CN_TIMEOUT_FOREVER;
    timer = ptTimerSoftHead;
    while(timer) //ִ�������е�TIMER ALARM
    {
        __TimerSoft_Get64Time(&timenow);
        if(timer->stat &CN_TIMER_ENCOUNT)
        {
            if(__TimerSoft_ChkTimeout(timer, timenow))
            {
                __TimerSoft_Remove(timer);
                if(timer->stat & CN_TIMER_RELOAD)
                {
                    timer->deadline = timenow + timer->cycle;
                    __TimerSoft_Add(timer);
                }
                else
                {
                    timer->deadline = CN_TIMER_ALARMNEVER;
                    __TimerSoft_AddLast(timer);
                }
                __TimerSoft_ExeAlarmHandler(timer); //execute the hook

                timer = ptTimerSoftHead;
            }
            else
            {
                //compute the wait time
                result = (u32)(timer->deadline - timenow);
                timer = NULL;
            }
        }
        else
        {
            timer = NULL; //����ѭ����֤�����е�TIMER������PAUSE����û�ж�ʱ��״̬
        }
    }

    return result;
}
// =============================================================================
// �������ܣ�TimerSoft_ISR
//          ��ʱ���жϷ���HOOK
// ���������irq_no,�жϺ�
// �����������
// ����ֵ     ����ʱδ����
// =============================================================================
u32 TimerSoft_ISR(ufast_t irq_no)
{
    u32 waittime;
    //��ʱ��ֹͣ����
    TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);

    waittime = __TimerSoft_DealTimeout();
    if(waittime != CN_TIMEOUT_FOREVER)
    {
        TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
        TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
    }
    else
    {
        //all the timer is paused or no timer, so no need to start the counter
    }

    return 0;
}

// =============================================================================
// �������ܣ�TimerSoft_Create_s
//          ����һ����ʱ��(�Լ��ṩ��Դ)
// ���������name,��ʱ������
//          cycle,��ʱ������,��λ΢��
//          isr,��ʱ����ʱʱ�䵽ִ��HOOk���ж��п��ܱ�����
//          timer,�ṩ�����ⶨʱ������Դ�ռ�
// �����������
// ����ֵ  �� NULL����ʧ��  ���򷵻ش����ɹ��Ķ�ʱ�����
//           �����Ķ�ʱ��Ĭ�ϵ�reloadģʽ�������Ҫ�ֶ��Ļ�����ô����֮���Լ����ã�
//           �����Ķ�ʱ�����Ǵ���pause״̬����Ҫ�ֶ������ö�ʱ��
// =============================================================================
tagTimerSoft*  TimerSoft_Create_s(const char *name, u32 cycle, fnTimerSoftIsr isr,\
                                  tagTimerSoft *timer)
{
    tagTimerSoft*      result = NULL;
    u32                waittime;
    tagTimerSoftMsg    msg;
    if(NULL == timer)  //�������
    {
        result =NULL;
    }
    else
    {
        if(sbUsingHardTimer)
        {
           if(Lock_MutexPend(ptTimerSoftQSync,CN_TIMEOUT_FOREVER))
            {
                //��ͣ���ӣ������жϻ���
                TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);
                //��ʼִ��ָ������Ĳ���
                //���ⶨʱ���ĳ�ʼ��
                timer->name = (char*)name;
                timer->cycle = cycle;
                timer->isr = isr;
                timer->deadline = CN_TIMER_ALARMNEVER;
                timer->stat = CN_TIMER_RELOAD;
                //�����������ⶨʱ�����timer��ʱ��������
                __TimerSoft_AddLast(timer);
                waittime = __TimerSoft_DealTimeout();
                if(waittime != CN_TIMEOUT_FOREVER)
                {
                    TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                    TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
                }
                Lock_MutexPost(ptTimerSoftQSync);
                result = timer;
            }
        }
        else
        {
            timer->name = (char*)name;
            timer->cycle = cycle;
            timer->isr = isr;
            timer->deadline = CN_TIMER_ALARMNEVER;
            timer->stat = CN_TIMER_RELOAD;
            msg.timer = timer;
            msg.type = EN_TIMERSOFT_ADD;
            if(MsgQ_Send(ptTimerSoftMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL))
            {
                result = timer;
            }
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�TimerSoft_Delete_s
//           ɾ��һ����ʱ��(�Լ��ṩ��Դ�Ķ�ʱ��)
// ���������timersoft,��ɾ���Ķ�ʱ��
// �����������
// ����ֵ  ����ɾ���Ķ�ʱ����NULLɾ�����ɹ�
// =============================================================================
tagTimerSoft* TimerSoft_Delete_s(tagTimerSoft* timer)
{
    tagTimerSoft*      result = NULL;
    u32                waittime;
    tagTimerSoftMsg    msg;
    if(NULL == timer)  //�������
    {
        result =NULL;
    }
    else
    {
        if(sbUsingHardTimer)
        {
           if(Lock_MutexPend(ptTimerSoftQSync,CN_TIMEOUT_FOREVER))
            {
                //��ͣ���ӣ������жϻ���
                TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);

                //�����������ⶨʱ�����timer��ʱ��������
                __TimerSoft_Remove(timer);
                waittime = __TimerSoft_DealTimeout();
                if(waittime != CN_TIMEOUT_FOREVER)
                {
                    TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                    TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
                }
                Lock_MutexPost(ptTimerSoftQSync);
                result = (tagTimerSoft*)timer;
            }
        }
        else
        {
            msg.timer = timer;
            msg.type = EN_TIMERSOFT_REMOVE;
            if(MsgQ_Send(ptTimerSoftMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL))
            {
                result = timer;
            }
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�TimerSoft_Create
//          ����һ����ʱ��
// ���������name,��ʱ������
//          cycle,��ʱ�����ڣ���λ΢��
//          isr,��ʱ����ʱʱ�䵽ִ��HOOk���ж��п��ܱ�����
// �����������
// ����ֵ  ��NULL����ʧ��  ���򷵻ط��䵽�Ķ�ʱ�����
//           �����Ķ�ʱ��Ĭ�ϵ�reloadģʽ�������Ҫ�ֶ��Ļ�����ô����֮���Լ����ã�
//           �����Ķ�ʱ�����Ǵ���pause״̬����Ҫ�ֶ������ö�ʱ��
// =============================================================================
tagTimerSoft* TimerSoft_Create(const char *name, u32 cycle,fnTimerSoftIsr isr)
{
    tagTimerSoft *timer;
    tagTimerSoft *result;

    result = NULL;
    timer = Mb_Malloc(ptTimerSoftMemPool,CN_TIMEOUT_FOREVER);
    if(NULL != timer)
    {
        result = TimerSoft_Create_s(name, cycle, isr,timer);
        if(NULL == result)
        {
            Mb_Free(ptTimerSoftMemPool,timer);
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�TimerSoft_Delete
//           ɾ��һ����ʱ��
// ���������timersoft,��ɾ���Ķ�ʱ��
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t TimerSoft_Delete(tagTimerSoft* timer)
{
    tagTimerSoft* result;

    result = TimerSoft_Delete_s(timer);
    if(result != (tagTimerSoft*)NULL)
    {
        Mb_Free(ptTimerSoftMemPool, (void *)timer);
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// �������ܣ�TimerSoft_Ctrl
//          ���ö�ʱ����ʶ��״̬
// ���������timersoft,���趨�Ķ�ʱ��.
//          opcode,��ʱ���Ĳ�����
//          inoutpara,���ݲ���������Ĳ������ȷ��趨cycle��ʱ��inoutpara����cyle
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵����opcode��Ӧ��para�����Զ���˵��
//  EN_TIMER_STARTCOUNT  //ʹ�ܼ�����inoutpara������
//  EN_TIMER_PAUSECOUNT, //ֹͣ������inoutpara������
//  EN_TIMER_SETCYCLE,   //�������ڣ�inoutparaΪu32,�����õ����ڣ�΢�룩
//  EN_TIMER_SETRELOAD,  //reloadģʽor not����inoutparaΪbool_t,true����reload
//  ����CMD��֧��
// =============================================================================
bool_t TimerSoft_Ctrl(tagTimerSoft* timer,u32 opcode, u32 para)
{
    bool_t  result = false;
    u32     waittime;
    s64     timenow;               //��ǰʱ��
    tagTimerSoftMsg msg;
    if(timer)                      //�������
    {
        if(sbUsingHardTimer)
        {
            Lock_MutexPend(ptTimerSoftQSync,CN_TIMEOUT_FOREVER);
            //��ͣ���ӣ������жϻ���
            TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);
            result = true;
            switch(opcode)
            {
                case EN_TIMER_STARTCOUNT:
                    if(0 ==(CN_TIMER_ENCOUNT & timer->stat))//������ʹ��
                    {
                        timer->stat |= CN_TIMER_ENCOUNT;
                        __TimerSoft_Get64Time(&timenow);
                        timer->deadline = timenow + timer->cycle;
                        __TimerSoft_Remove(timer);
                        __TimerSoft_Add(timer);
                    }
                    break;
                case EN_TIMER_PAUSECOUNT:
                    if(CN_TIMER_ENCOUNT & timer->stat)//��������ͣ
                    {
                        timer->stat &= (~CN_TIMER_ENCOUNT);
                        timer->deadline = CN_TIMER_ALARMNEVER;
                        __TimerSoft_Remove(timer);
                        __TimerSoft_AddLast(timer);
                    }
                    break;
                case EN_TIMER_SETCYCLE:
                    timer->cycle = para;
                    if(CN_TIMER_ENCOUNT&timer->stat)
                    {
                        __TimerSoft_Get64Time(&timenow);
                        timer->deadline = timenow + timer->cycle;
                        __TimerSoft_Remove(timer);
                        __TimerSoft_Add(timer);
                    }
                    break;
                case EN_TIMER_SETRELOAD:
                    if(para)
                    {
                        timer->stat |= CN_TIMER_RELOAD;
                    }
                    else
                    {
                        timer->stat &= (~CN_TIMER_RELOAD);
                    }
                    break;
                default:
                    result = false;
                    break;
            }
            //�����������ܻ��ж�ʱ����ʱ,����ʱ�����еĳ�ʱ����
            //����ʱ�����еĳ�ʱ����
            waittime = __TimerSoft_DealTimeout();
            if(waittime != CN_TIMEOUT_FOREVER)
            {
                TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                TimerHard_Ctrl(sgTimerHardDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
            }
            Lock_MutexPost(ptTimerSoftQSync);
        }
        else
        {
            msg.timer = timer;
            msg.type = opcode;
            msg.para = para;
            result = MsgQ_Send(ptTimerSoftMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL);
        }
    }

    return result;
}
// =============================================================================
// �������ܣ�TimerSoft_VMTask �߳�ģ��Ӳ����ʱ��
// �����������
// �����������
// ����ֵ  ��
// ˵��      :��ʹ���߳���Ϊģ�ⶨʱ����ʱ�򣬳�����Ӻ�ɾ�����⣬���еĶԶ�ʱ�����еĲ�����
//         ���ڸ��߳������
// =============================================================================
ptu32_t  TimerSoft_VMTask(void)
{
    u32              waittime;
    u32              opcode;
    u32              para;
    s64              timenow;
    tagTimerSoftMsg  msg;
    tagTimerSoft     *timer;

    while(1)
    {
        //any way, we scan the queue to chk if any timer is timeout
        waittime = __TimerSoft_DealTimeout();
        if(MsgQ_Receive(ptTimerSoftMsgQ,(u8 *)&msg, sizeof(msg),waittime))
        {
            //has got some msg from the api
            opcode = msg.type;
            timer = msg.timer;
            para = msg.para;
            switch(opcode)
            {
                case EN_TIMER_STARTCOUNT:
                    if(0 ==(CN_TIMER_ENCOUNT & timer->stat))//������ʹ��
                    {
                        timer->stat |= CN_TIMER_ENCOUNT;
                        __TimerSoft_Get64Time(&timenow);
                        timer->deadline = timenow + timer->cycle;
                        __TimerSoft_Remove(timer);
                        __TimerSoft_Add(timer);
                    }
                    break;
                case EN_TIMER_PAUSECOUNT:
                    if(CN_TIMER_ENCOUNT & timer->stat)//��������ͣ
                    {
                        timer->stat &= (~CN_TIMER_ENCOUNT);
                        timer->deadline = CN_TIMER_ALARMNEVER;
                        __TimerSoft_Remove(timer);
                        __TimerSoft_AddLast(timer);
                    }
                    break;
                case EN_TIMER_SETCYCLE:
                    timer->cycle = para;
                    if(CN_TIMER_ENCOUNT&timer->stat)
                    {
                        __TimerSoft_Get64Time(&timenow);
                        timer->deadline = timenow + timer->cycle;
                        __TimerSoft_Remove(timer);
                        __TimerSoft_Add(timer);
                    }
                    break;
                case EN_TIMER_SETRELOAD:
                    if(para)
                    {
                        timer->stat |= CN_TIMER_RELOAD;
                    }
                    else
                    {
                        timer->stat &= (~CN_TIMER_RELOAD);
                    }
                    break;
                case EN_TIMERSOFT_ADD:
                    __TimerSoft_Add(timer);
                    break;
                case EN_TIMERSOFT_REMOVE:
                    __TimerSoft_Remove(timer);
                    break;
                default:
                    break;
            }

        }

    }
    return 0;
}
// =============================================================================
// �������ܣ�ModuleInstall_TimerSoft
//          ���ⶨʱ����ʼ��ģ��
// ���������para,�ö�ʱ��ģ��ľ��ȣ���λ΢��
// �����������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// =============================================================================
ptu32_t ModuleInstall_TimerSoft(ptu32_t para)
{
    u16 u16EvttId;
    u16 u16EventId;
    printk("TimerSoft:Init Start....\n\r");
    ptTimerSoftMem = M_Malloc(gc_u32CfgTimerLimit * sizeof(tagTimerSoft),\
                              CN_TIMEOUT_FOREVER);
    if(ptTimerSoftMem == NULL)
    {
        goto EXIT_MEMFAILED;
    }
    ptTimerSoftMemPool = Mb_CreatePool(ptTimerSoftMem,gc_u32CfgTimerLimit,
                                sizeof(tagTimerSoft),0,0,"TimerSoft");
    if(NULL ==ptTimerSoftMemPool)
    {
        goto EXIT_POOLFAILED;
    }
    sbUsingHardTimer = (bool_t)para;
    if(sbUsingHardTimer == CN_TIMER_SOURCE_HARD)
    {
        //ʹ��Ӳ����ʱ����ʱ��Ż�ʹ�ø�ͬ�����
        ptTimerSoftQSync = Lock_MutexCreate("TimerSoft");
        if(NULL == ptTimerSoftQSync)
        {
            goto EXIT_TIMERFAILED;
        }
        sgTimerHardDefault = TimerHard_Alloc(0,TimerSoft_ISR);
        if((ptu32_t)NULL == sgTimerHardDefault)
        {
            Lock_MutexDelete(ptTimerSoftQSync);
            goto EXIT_TIMERFAILED;
        }
        //ʹ�ܶ�ʱ���жϣ�����û��ʹ�ܶ�ʱ��,����API�ĵ���
        TimerHard_Ctrl(sgTimerHardDefault,EN_TIMER_ENINT,(ptu32_t)NULL);
        TimerHard_Ctrl(sgTimerHardDefault,EN_TIMER_SETRELOAD,(ptu32_t)(false));
    }
    else
    {
        //����ͨ���õ�����
        ptTimerSoftMsgQ = MsgQ_Create(CN_TIMERSOFT_MSGLEN, \
                                      sizeof(tagTimerSoftMsg),CN_MSGQ_TYPE_FIFO);
        if(NULL == ptTimerSoftMsgQ)
        {
            goto EXIT_TIMERFAILED;
        }
        else
        {
            u16EvttId = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS-2,0,0,
                                   TimerSoft_VMTask,NULL,0x400,NULL);
            if(CN_EVTT_ID_INVALID == u16EvttId)
            {
                MsgQ_Delete(ptTimerSoftMsgQ);
                ptTimerSoftMsgQ = NULL;
                goto EXIT_TIMERFAILED;
            }
            else
            {
                u16EventId = Djy_EventPop(u16EvttId,NULL,0,0,0,0);
                if(CN_EVENT_ID_INVALID == u16EventId)
                {
                    MsgQ_Delete(ptTimerSoftMsgQ);
                    ptTimerSoftMsgQ = NULL;
                    //ע���¼����ͣ�ò��Ŀǰû��--TODO
                    goto EXIT_TIMERFAILED;
                }
            }
        }
    }

    printk("TimerSoft:Init Success\n\r");
    return 0;

EXIT_TIMERFAILED:
    Mb_DeletePool(ptTimerSoftMemPool);
EXIT_POOLFAILED:
    free(ptTimerSoftMem);
EXIT_MEMFAILED:
    printk("TimerSoft:Init Failed\n\r");
    return -1;
}
