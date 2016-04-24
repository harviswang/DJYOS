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
// Copyright (C) 2012-2020 ��������������޹�˾ All Rights Reserved
// ģ������: ���Ź�ʵ��
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 11:45:07 AM/Sep 20, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// 1,�ڿ��Ź��������������֮ǰ�����еĿ��Ź�API����ֱ�Ӳ������Ź�����
// 2,�ڿ��Ź��������������֮�����еĿ��Ź�API����ֱ�ӷ��͸����Ź�������������
// =============================================================================
#include "config-prj.h"
#include "arch_feature.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "int.h"
#include "pool.h"
#include "msgqueue.h"
#include "endian.h"
#include "wdt_soft.h"
#include "wdt_hal.h"
#include "systime.h"
#include "djyos.h"
#define CN_WDT_YIP_NEVER      CN_LIMIT_SINT64     //��timeoutΪ��ʱ��ʱ��ʾ�ÿ��Ź���ͣ״̬
#define CN_WDT_YIP_PRECISION  CN_CFG_TICK_US      //������Ź�ģ��ľ���

//������Ź�����Ϣ����
typedef struct
{
    tagWdt       *pwdt;        //�������Ŀ��Ź�
    u32          opcode;       //��������
    ptu32_t      para;         //��������ָ���Ĳ���
}tagWdtMsg;

//��ģ��ʹ�õľ�̬ȫ�ֱ���
static tagWdt                     *g_ptWdtPool; //���Ź��ڴ����Դ
static struct tagMemCellPool      *ptWdtPool = NULL;             //���Ź��ڴ��
static tagWdt                     *ptWdtHead = NULL;             //���Ź�����ͷ
static tagWdt                     *ptWdtTail = NULL;             //���Ź�����β
static tagWdt                     *ptWdtHard = NULL;             //Ӳ�����Ź���Ӧ��������Ź�
static bool_t (*fnWdtHardFeed)(void) = NULL;       //Ӳ�����Ź���ι������
#define CN_WDTMSG_LIMIT       0x20                 //���Ź���Ϣ���е���󳤶�
static struct tagMsgQueue     *ptWdtMsgBox = NULL; //���Ź���Ϣ����

//���Ź����е�ԭ��
//�����Ŀ��Ź�����У�ֻ��û�а�ʱι�Ĺ��Ż�У���˿��Ź�������Ϊû�а�ʱι
//û�а�ʱι��Ϊ���¼��֣�
//1,��ʱ��ȴ��ι�������Ź������������㹻������ʱ��ȴ��ȥι�����������ǳ�֮Ϊ�߼�ԭ��
//2,û��ʱ��ι�������Ź���������ò����㹻������ʱ�䵼�������겻�ɶ�û�а�ʱι������������
//            ��֮Ϊ����ԭ��
//3,����ԭ�򣬼�û��ָ�����Ź�������������������Ź��оͽ��ˣ�����Ī��ԭ��
enum __EN_WDTYIP
{
    EN_WDT_NOYIP          = 0,     //���Ź�����û�н�
    EN_WDT_YIPFORLOGIC,            //�߼���������Ŀ��Ź�����
    EN_WDT_YIPFORSHEDULE,          //����ԭ������Ŀ��Ź�����
    EN_WDT_YIPOTHERS,              //δ֪ԭ������Ŀ��Ź�����
};

#include "exp_api.h"
#define CN_WDT_EXPDECODERNAME           "WDTEXP DECODER"   //���Ź��쳣��Ϣ������
//���Ź��쳣��Ϣ��֯�ṹ
typedef struct
{
    char       wdtname[CN_EXP_NAMELEN_LIMIT];    //�쳣�Ŀ��Ź�������
    tagWdt     wdt;                              //�쳣�Ŀ��Ź�
}tagWdtExpInfo;
// =============================================================================
// �������ܣ��׳�����Ŀ��Ź��쳣
// ���������result, ���Ź����쳣ʱ��Ҫ���Ĵ���
//          wdt, �쳣�Ŀ��Ź�
// ���������
// ����ֵ  �����յĴ�����
// ˵��    ����Ҫ���Ѽ�WDT�쳣ʱ����Ϣ���ú��������ڲ����ã������ֿ��Ź��쳣������Ҫ�����
//        �쳣��ʱ����øú���
// =============================================================================
enum _EN_EXP_DEAL_TYPE_ __Wdt_TrowWdtExp(enum _EN_EXP_DEAL_TYPE_ result,\
                                            tagWdt *wdt)
{
    struct tagExpThrowPara  parahead;
    tagWdtExpInfo wdtexp;
    wdtexp.wdt = *wdt;
    memcpy(wdtexp.wdtname,wdt->pname,CN_EXP_NAMELEN_LIMIT);
    parahead.name = CN_WDT_EXPDECODERNAME;
    parahead.dealresult = result;
    parahead.para = (u8 *)&wdtexp;
    parahead.para_len = sizeof(wdtexp);
    parahead.validflag = true;
    Exp_Throw(&parahead, (u32*)&result);
    return  result;
}

// =============================================================================
// �������ܣ�WDT�쳣��Ϣ�ֽ���ת��
// ���������wdt, ��ת���Ŀ��Ź�,
//          endian, ��˻���С��
//          todo:ǰ��,ע�Ͳ�������д,˭֪����ô��ʾ��˺�С��?
// ���������wdt, ת����Ŀ��Ź�
// ����ֵ  ����
// ˵��    �������쳣��Ϣ�ǰ����쳣ʱ�Ļ����ı��ض˸�ʽ�洢�ģ���������쳣�Ļ������쳣��Ϣ
//        �Ļ����Ĵ�С�˲�һ�£���ô����Ҫת�����ò�����Ϊ�쳣�洢�������쳣����������ͬ��
//        ����
// =============================================================================
void __Wdt_SwapWdtInfoByEndian(tagWdt *wdt, u32 endian)
{
    //��С��ת��
    if(CN_CFG_BYTE_ORDER != endian)
    {
        wdt->action = swapl(wdt->action);
        wdt->cycle =  swapl(wdt->cycle);
        wdt->runtimelevel = swapl(wdt->runtimelevel);
        wdt->sheduletimeslevel = swaps(wdt->sheduletimeslevel);
        wdt->shyiptimes = swaps(wdt->shyiptimes);
        wdt->taskownerid = swapl(wdt->taskownerid);
        wdt->timeoutreason = swapl(wdt->timeoutreason);
    }
}
// =============================================================================
// �������ܣ����Ź��쳣��Ϣ����
// ���������wdtinfo,���Ź��쳣��Ϣ
//          endian, ��С��
// ���������
// ����ֵ  ��
// ˵��    ��ע���쳣��������ʱ��Ὣ���쳣������ע���ȥ
// =============================================================================
bool_t __Wdt_WdtExpInfoDecoder(tagWdtExpInfo  *wdtinfo,u32 endian)
{
    tagWdt  *wdt;

    wdt = &wdtinfo->wdt;
    __Wdt_SwapWdtInfoByEndian(wdt, endian);

    printk("wdtinfo:wdtname                :%s\n\r",    wdtinfo->wdtname);
    printk("wdtinfo:wdtowner event_id      :0x%08x\n\r",wdt->taskownerid);
    printk("wdtinfo:wdt_action             :0x%08x\n\r",wdt->action);
    printk("wdtinfo:yip_cycle              :0x%08x\n\r",wdt->cycle);
    printk("wdtinfo:yip_reason             :0x%08x\n\r",wdt->timeoutreason);
    printk("wdtinfo:yip_consumed_time_level:0x%08x\n\r",wdt->runtimelevel);
    printk("wdtinfo:yip_for_shedule_times  :0x%08x\n\r",wdt->shyiptimes);
    printk("wdtinfo:yip_shedule_times_level:0x%08x\n\r",wdt->sheduletimeslevel);
    return true;
}

// =============================================================================
// �������ܣ�Ӳ�����Ź������ƺ���
// ���������wdt���������Ŀ��Ź�
// ���������
// ����ֵ  ��
// ˵��    ����Ҫ�Ƕ�ע���Ӳ�����Ź���HOOK���а�װ;����Ҫ�������ķ���ֵ�Ǻ��ԣ�����ϵͳ��
//        Ӳ�����Ź���Ӧ��������Ź����ж�����
// =============================================================================
static u32 __Wdt_HardWdtYipHook(tagWdt *wdt)
{
    if(NULL != fnWdtHardFeed)//��Ӳ������ִ��ι��
    {
        fnWdtHardFeed();
    }
    return EN_EXP_DEAL_IGNORE;
}
// =============================================================================
// �������ܣ������Ź���ӽ������У��ÿ��Ź��Ѿ����ú���deadlineʱ��
// ���������wdt���������Ŀ��Ź�(�ÿ��Ź��Ѿ����ú��˹���ʱ��)
// ���������
// ����ֵ  ��
// ˵��    ���ÿ��Ź�����һ�����ڶ�����
// =============================================================================
static void __Wdt_Add2Queque(tagWdt *wdt)
{
    tagWdt *wdtcomp;
    s64    deadline;
    if(NULL == ptWdtHead)
    {
        ptWdtHead = wdt;
        ptWdtTail = wdt;
        wdt->pnxt = NULL;
        wdt->ppre = NULL;
    }
    else if(wdt->deadtime == CN_WDT_YIP_NEVER) //the wdt to add is pause
    {
        ptWdtTail->pnxt = wdt;
        wdt->ppre = ptWdtTail;
        wdt->pnxt = NULL;
        ptWdtTail = wdt;
    }
    else //compare each the deadline of wdt of the queue
    {
        deadline = wdt->deadtime;
        wdtcomp = ptWdtHead;
        while((wdtcomp != NULL)&&(wdtcomp->deadtime < deadline))
        {
            wdtcomp = wdtcomp->pnxt;
        }
        if(NULL == wdtcomp)
        {
            //no deadline is bigger than the one to add, so add it to the tail
            ptWdtTail->pnxt = wdt;
            wdt->ppre = ptWdtTail;
            wdt->pnxt = NULL;
            ptWdtTail = wdt;
        }
        else
        {
            //there exist one whose deadline is bigger than the one to add
            if(wdtcomp == ptWdtHead)
            {
                wdt->pnxt = wdtcomp;
                wdt->ppre = NULL;
                wdtcomp->ppre = wdt;
                ptWdtHead = wdt;
            }
            else
            {
                wdtcomp->ppre->pnxt = wdt;
                wdt->ppre = wdtcomp->ppre;
                wdtcomp->ppre = wdt;
                wdt->pnxt = wdtcomp;
            }
        }
    }
}
// =============================================================================
// �������ܣ��ӿ��Ź��������Ƴ����Ź�
// ���������wdt���������Ŀ��Ź�
// ���������
// ����ֵ  ��
// ˵��    ���ÿ��Ź�����ڶ�����
// =============================================================================
static void __Wdt_RemovefQueue(tagWdt *wdt)
{
    if(wdt == ptWdtHead)  //the head to remove
    {
        if(NULL == wdt->pnxt)
        {
            //only one wdt in the queue
            ptWdtHead = NULL;
            ptWdtTail = NULL;
        }
        else
        {
            //only the head
            ptWdtHead = wdt->pnxt;
            wdt->pnxt->ppre = NULL;
        }
    }
    else if(wdt == ptWdtTail) // the tail to remove
    {
        ptWdtTail = wdt->ppre;
        wdt->ppre->pnxt = NULL;
    }
    else
    {
        wdt->ppre->pnxt = wdt->pnxt;
        wdt->pnxt->ppre = wdt->ppre;
    }
    wdt->ppre = NULL;
    wdt->pnxt = NULL;
}
// =============================================================================
// �������ܣ������ݸ������������Ϣָʾ
// ���������msg, �����������ostime����ǰϵͳʱ��
// ���������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
static void __Wdt_DealMsg(tagWdtMsg *msg)
{
    u32                  opcode;
    u32                  oppara;
    tagWdt               *wdt;
    struct tagEventInfo  eventinfo;
    s64                  ostime;

    wdt = msg->pwdt;
    opcode = msg->opcode;
    oppara = msg->para;
    if(NULL != wdt)
    {
        //����WDT�߳�ӵ�������һ�β���WDTʱ��
        if(CN_EVENT_ID_INVALID != wdt->taskownerid)
        {
            if(Djy_GetEventInfo(wdt->taskownerid, &eventinfo))
            {
                wdt->runtime = eventinfo.consumed_time;
            }
        }
        //����������г�Ա
        wdt->shyiptimes = 0;
        wdt->timeoutreason = EN_WDT_NOYIP;

        ostime = DjyGetTime();
        switch (opcode)
        {
            case EN_WDTCMD_ADD:
            {
                //add an new wdt to the queue
                wdt->deadtime = wdt->cycle + ostime;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_DEL:
            {
                __Wdt_RemovefQueue(wdt);
                break;
            }
            case EN_WDTCMD_PAUSE:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->deadtime = CN_WDT_YIP_NEVER;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_RESUME:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->deadtime = wdt->cycle + ostime;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_CLEAN:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->deadtime = wdt->cycle + ostime;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SETTASKID:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->taskownerid = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SETCYCLE:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->cycle = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SETYIPACTION:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->action = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SETYIPHOOK:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->fnhook = (fnYipHook)oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SETCONSUMEDTIMELEVEL:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->runtimelevel = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SETSCHLEVEL:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->sheduletimeslevel = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return;
}
// =============================================================================
// �������ܣ����Ź����н������
// ���������result���쳣��������wdt���쳣�Ŀ��Ź�
// ���������
// ����ֵ  ��
// ˵��    ��ֻ����Ҫ��¼���������Ķ����Ż��������׳��쳣
// =============================================================================
static void __Wdt_DealWdtYipResult(u32 result, tagWdt *wdt)
{
    //befor we do the wdt exception, we'd better feed the hard wdt to avoid
    //reseting during the dealing
    if(result > EN_EXP_DEAL_DEFAULT)
    {
        __Wdt_HardWdtYipHook(NULL);
        __Wdt_TrowWdtExp(result,wdt);
    }
}
// =============================================================================
// �������ܣ��������Ź�����ԭ��
// ���������wdt���������Ŀ��Ź�
// ���������
// ����ֵ  ����
// ˵��    ��Ŀǰ�������������������ʱ�����Լ������߼��������û��ָ����������Ϊ��others
// �жϱ�׼��
//         ������ϴβ������Ź���ʼ�������Ȱ����������һ��ָ���ʱ��(time)���û�
//         �趨�ı�׼��level����Ƚ�
//         1, time> level,�����������㹻ʱ������ȴû��ι���������߼������Լ�����
//             ��ʱ�����ô���Ҳ���ڴ��ࣩ��һ������ھ���while(1)����
//             ����EN_WDT_YIPFORLOGIC
//         2, time <level,��������û�л�ȡ�㹻������ʱ�䵼�µģ����ڱ���������ʱ��
//             ̫��Զ������EN_WDT_YIPFORSHEDULE
//         3,  ��ȡ����ʱ��ʧ������Ϊ��OTHERS
// =============================================================================
static void __Wdt_AnalyzeYipReason(tagWdt *wdt)
{
    u32    Tm_TimeRun;
    struct tagEventInfo wdt_event_info;

    if( Djy_GetEventInfo((u16)wdt->taskownerid, &wdt_event_info))
    {
        Tm_TimeRun = (u32)(wdt_event_info.consumed_time - wdt->runtime);
        if(Tm_TimeRun < wdt->runtimelevel) //����ʱ�䲻��
        {
            wdt->timeoutreason = EN_WDT_YIPFORSHEDULE;
            wdt->shyiptimes++;

        }
        else //ʱ�乻��ȴ��ι���������ʲô��̬
        {
            wdt->timeoutreason = EN_WDT_YIPFORLOGIC;
        }
        wdt->runtime = wdt_event_info.consumed_time;
    }
    else
    {
        wdt->timeoutreason = EN_WDT_YIPOTHERS;   //Ī�����쳣
    }
    return;
}
// =============================================================================
// �������ܣ�ɨ���������Ź����в�����ʱ�Ŀ��Ź�
// ���������
// ���������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
static void __Wdt_ScanWdtQueque(void)
{
    s64             timenow;
    u32             result;
    tagWdt          *wdt;

    wdt = ptWdtHead;
    while(NULL != wdt)//���������ڽеĹ�
    {
        timenow = DjyGetTime();//ʵʱ����ʱ��

        if((timenow + CN_WDT_YIP_PRECISION)>= wdt->deadtime) // the wdt has been timeout
        {
            __Wdt_AnalyzeYipReason(wdt);
            if(NULL !=  wdt->fnhook)
            {
                result = wdt->fnhook(wdt);//����Ľаɣ�����
                if(result == EN_EXP_DEAL_DEFAULT)
                {
                    result = wdt->action;
                }
            }
            else
            {
                result = wdt->action;
            }
            if(wdt->shyiptimes >= wdt->sheduletimeslevel)
            {
                result = result;
            }
            else//�������޶�����
            {
                result = EN_EXP_DEAL_RECORD;
            }
            //this wdt should be relocate in the queue
            __Wdt_DealWdtYipResult(result,wdt);
            __Wdt_RemovefQueue(wdt);
            wdt->deadtime = wdt->cycle + timenow;
            __Wdt_Add2Queque(wdt);
            //ok, do another wdt
            wdt = ptWdtHead;
        }
        else
        {
            //the head is not timeout, so we believe that all others are all not
            //time out
            wdt = NULL;
        }
    }
}

// =============================================================================
// �������ܣ����Ź���������
// ���������
// ���������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
static ptu32_t Wdt_Service(void)
{
    s64           timenow;
    tagWdt        *wdt;
    u32           waittime;
    tagWdtMsg     wdtmsg;
    //��Ӳ�������㹷�нӹܹ���
    WdtHal_BootEnd();
    // deal all the msg cached in the msgbox
    timenow = DjyGetTime();
    while(MsgQ_Receive(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),0))
    {
        __Wdt_DealMsg(&wdtmsg); //all the wdt will be queued in the wdt queue
    }

    //now we  come into the never get out loop, deal all the msg and timeout wdt
    while(1)
    {
        //preapara the wait time
        wdt = ptWdtHead;
        if((NULL == wdt)||(wdt->deadtime == CN_WDT_YIP_NEVER))
        {
            waittime = CN_TIMEOUT_FOREVER;
        }
        else
        {
            timenow = DjyGetTime();
            waittime = (u32)(wdt->deadtime - timenow);
        }
        //ok, now we wait the msg box during the expect time
        if(MsgQ_Receive(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),waittime))
        {
            //must look out that:there maybe more than one msgs in the msgbox
            //some app has send some msg to the
            __Wdt_DealMsg(&wdtmsg); //all the wdt will be queued in the wdt queue
            // deal all the msg cached in the msgbox
            while(MsgQ_Receive(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),0))
            {
                __Wdt_DealMsg(&wdtmsg); //all the wdt will be queued in the wdt queue
            }
        }
        //scan the queue to deal the timeout wdt
        __Wdt_ScanWdtQueque();
    }

    return 1; //if no exp, this could not be reached
}

// =============================================================================
// �������ܣ����Ź�ģ��ĳ�ʼ��
// ���������
// ���������
// ����ֵ  ��1�ɹ�  0ʧ��
// ˵��    ���������Ź�Ӳ����Ӧ�����Ź���ע�ῴ�Ź��쳣��Ϣ������
// =============================================================================
ptu32_t ModuleInstall_Wdt(ptu32_t para)
{
    bool_t  result_bool;
    u16     evttid;

    g_ptWdtPool = M_Malloc(gc_u32CfgWdtLimit * sizeof(tagWdt),0);
    if(g_ptWdtPool == NULL)
        return 0;
    ptWdtPool = Mb_CreatePool(g_ptWdtPool,gc_u32CfgWdtLimit,sizeof(tagWdt),0,0,"wdt pool");
    //init the queue
    ptWdtHead = NULL;
    ptWdtTail = NULL;
    ptWdtHard = NULL;

    //create the msg box for the api to snd msg to the wdt service task
    ptWdtMsgBox = MsgQ_Create(CN_WDTMSG_LIMIT,sizeof(tagWdtMsg),CN_MSGQ_TYPE_FIFO);

    //create the main service
    evttid = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_WDT,0,0,Wdt_Service,
                                NULL,0x1000,"wdt service");
    if(evttid == CN_EVTT_ID_INVALID)
        return 0;
    if( Djy_EventPop(evttid,NULL,0,0,0,0) == CN_EVENT_ID_INVALID)
    {
        printk("WDT MODULE:POP SERVICE FAILED!\n\r");
        Djy_EvttUnregist(evttid);
        return 0;
    }

    //create the soft wdt match the hard wdt
    struct tagWdtHalChipInfo hardpara;
    result_bool = WdtHal_GetChipPara(&hardpara);
    if(true == result_bool)//����Ӳ�����Ź����򴴽�Ӳ�����Ź�
    {
        fnWdtHardFeed = hardpara.wdtchip_feed;
        ptWdtHard = Wdt_Create(hardpara.wdtchip_name,\
                               hardpara.wdtchip_cycle,\
                               __Wdt_HardWdtYipHook,\
                               EN_EXP_DEAL_IGNORE, NULL);
    }
//todo:�˴��о���
    extern bool_t Exp_RegisterThrowinfoDecoder(fnExp_ThrowinfoDecoderModule decoder,const char *name);
    if(false ==Exp_RegisterThrowinfoDecoder(__Wdt_WdtExpInfoDecoder,\
                                            CN_WDT_EXPDECODERNAME))
    {
        printk("WDT MODULE: Register Wdt Exp Decoder Failed!\n\r");
    }

    printk("WDT MODULE:Init end ...\n\r");
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////���������ṩ��ʹ�ÿ��Ź����û�//////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//��ע�����е��û�API��������������WDT���漰�����Ź��Ķ���ͷʱ���޸Ķ��еĲ����ͻ�ֱ��
//      �����û��������ĵ��У�����ͷ�������service������service���ж��еĲ���
//      ���Ź�ģ��δ����֮ǰ����ֱ�Ӳ������Ź����еġ�

// =============================================================================
// �������ܣ��������Ź�
// ���������
//          dogname,���Ź�����
//          yip_cylce,�������ڣ���λΪ΢��
//          yiphook,�����ƺ���ָ��
//          yip_action,���ж�������yiphookΪ�ջ��߷���defaultʱ���ô�ֵ
//          levepara,���Ź���ʱ��ԭ���Լ���ʱ���̴��������Ʋ���
// ���������
// ����ֵ  �����������⿴�Ź���NULL��ʾʧ��
// ˵��    ����ر�֤���Ź��ƺ�������ֵ�Ķ�����ϱ�׼����enum _EN_EXP_DEAL_TYPE
//           ��������exp_api.h�ļ���
//           �����Ŀ��Ź����ɹ������´ι���ʱ��Ϊ��ǰʱ��+yip_cycle=time
// =============================================================================
tagWdt *Wdt_Create(char *dogname,u32 yip_cycle,\
        fnYipHook yiphook,ptu32_t yip_action, tagWdtTolerate *levelpara)
{
    tagWdt  *result;
    tagWdt  *wdt;

    result = NULL;
    wdt = Mb_Malloc(ptWdtPool,0);
    if(NULL != wdt)
    {
        result = Wdt_Create_s(wdt,dogname,yip_cycle,yiphook,yip_action, levelpara);
        if(NULL == result)
        {
            Mb_Free(ptWdtPool,wdt);
        }
    }
    return result;
}
// =============================================================================
// �������ܣ��������⿴�Ź�
// ���������
//          wdt,�û��Լ��ṩ�Ĵ洢�ռ�
//          dogname,���Ź�����
//          yip_cylce,�������ڣ���λΪ΢��
//          yiphook,�����ƺ���ָ��
//          yip_action,���ж�������yiphookΪ�ջ��߷���defaultʱ���ô�ֵ
//          levepara,���Ź���ʱ��ԭ���Լ���ʱ���̴��������Ʋ���
// ���������
// ����ֵ  �����������⿴�Ź���NULL��ʾʧ��
// ˵��    ����ر�֤���Ź��ƺ�������ֵ�Ķ�����ϱ�׼����enum _EN_EXP_DEAL_TYPE
//           ��������exp_api.h�ļ���
//           �����Ŀ��Ź����ɹ������´ι���ʱ��Ϊ��ǰʱ��+yip_cycle
// =============================================================================
tagWdt *Wdt_Create_s(tagWdt *wdt, char *dogname,u32 yip_cycle,\
        fnYipHook yiphook,ptu32_t yip_action, tagWdtTolerate *levelpara)
{

    tagWdt     *result;
    tagWdtMsg  wdtmsg;

    result = NULL;
    if((NULL != wdt)&&( 0 != yip_cycle))
    {
        memset((u8 *)wdt, 0, sizeof(tagWdt));
        wdt->pname = dogname;
        wdt->cycle = yip_cycle;
        wdt->fnhook = yiphook;
        wdt->action = yip_action;
        if(NULL != levelpara)
        {
            wdt->runtimelevel = levelpara->runtimelevel;
            wdt->sheduletimeslevel = levelpara->shtimeoutlevel;
        }
        wdt->taskownerid = CN_EVENT_ID_INVALID;

        //snd the msg to the service task
        wdtmsg.pwdt = wdt;
        wdtmsg.opcode = EN_WDTCMD_ADD;
        wdtmsg.para = 0;
        if(MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg, sizeof(tagWdtMsg),CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL))
        {
            result = wdt;
        }
        else
        {
            memset((u8 *)wdt, 0, sizeof(tagWdt));
        }
    }

    return result;
}

// =============================================================================
// �������ܣ�ɾ�����Ź�
// �������:wdt,��ɾ���Ŀ��Ź�
// ���������
// ����ֵ  ��true�ɹ���falseʧ��
// ˵��    ����Wdt_Create�ɶԵ���
// =============================================================================
bool_t Wdt_Delete(tagWdt *wdt)
{
    bool_t result = false;
    if(NULL != wdt)
    {
        result = Wdt_Delete_s(wdt);
        if(result)
        {
            Mb_Free(ptWdtPool,wdt);
        }
    }

    return result;
}
// =============================================================================
// �������ܣ�ɾ�����Ź�
// ���������wdt,��ɾ���Ŀ��Ź�
// ���������
// ����ֵ  ��true�ɹ���falseʧ��
// ˵��    ����Wdt_Create_s�ɶԵ���
// =============================================================================
bool_t Wdt_Delete_s(tagWdt *wdt)
{
    bool_t       result;
    tagWdtMsg    wdtmsg;

    result = false;
    if(NULL != wdt)
    {
        wdtmsg.pwdt = wdt;
        wdtmsg.opcode = EN_WDTCMD_DEL;
        wdtmsg.para = 0;
        result = MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),\
                           CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    }

    return result;
}
// =============================================================================
// �������ܣ���λ���Ź�
// ���������wdt,�������Ŀ��Ź�
// ���������
// ����ֵ  ��true success while false failed
// ˵��    ��
// =============================================================================
bool_t Wdt_Clean(tagWdt *wdt)
{
    bool_t       result;
    tagWdtMsg    wdtmsg;
    atom_low_t   atom;
    s64          ostime;

    result = false;
    if(NULL != wdt)
    {
        atom = Int_LowAtomStart();
        if(ptWdtHead != wdt)
        {
            //����ֱ���޸�wdtqueue
            ostime = DjyGetTime();
            __Wdt_RemovefQueue(wdt);
            wdt->deadtime = wdt->cycle + ostime;
            __Wdt_Add2Queque(wdt);
            Int_LowAtomEnd(atom);
        }
        else
        {
            //snd msg to the monitor task
            Int_LowAtomEnd(atom);
            wdtmsg.pwdt = wdt;
            wdtmsg.opcode = EN_WDTCMD_CLEAN;
            wdtmsg.para = 0;
            result = MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),\
                               CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
        }

    }
    return result;
}
// =============================================================================
// �������ܣ����Ź�����������
// ���������wdt,�������Ŀ��Ź�
//          type,�������ͣ��μ�enum EN_WDTCMD�Ķ���
//          para,����type�Ĳ�ͬ����ͬ
// ���������
// ����ֵ  ��������ϵĿ��Ź�
// ˵��    ��������ֻ����EN_WDTCMD_PAUSE��������
// =============================================================================
bool_t Wdt_Ctrl(tagWdt *wdt, u32 type, ptu32_t para)
{
    bool_t       result;
    tagWdtMsg    wdtmsg;

    result = false;
    if((NULL != wdt)&&(type >= EN_WDTCMD_PAUSE)&&(type < EN_WDTCMD_INVALID))
    {
        wdtmsg.pwdt = wdt;
        wdtmsg.opcode = type;
        wdtmsg.para = para;
        result = MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),\
                           CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    }
    return result;
}

