//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: ���ڶ�ʱ��ϵͳ���ƺ͹���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "./include/gdd_private.h"

static  list_t  list_system_timer;

//----���䶨ʱ���ڴ�-------------------------------------------------------------
//����: ��
//��������
//���أ���ʱ���ڴ�ָ��
//------------------------------------------------------------------------------
static  TIMER*  TMR_Alloc(void)
{
    TIMER *tmr;

    tmr =malloc(sizeof(TIMER));
    if(NULL!=tmr)
    {
        tmr->Tag =TMR_TAG;
    }
    return tmr;
}

//----�ͷŶ�ʱ���ڴ�-------------------------------------------------------------
//����: �ͷ�һ����TMR_Alloc����Ķ�ʱ���ڴ�.
//��������ʱ���ڴ�ָ��
//���أ���
//------------------------------------------------------------------------------
static  void    TMR_Free(TIMER *ptmr)
{
    ptmr->Tag =INVALD_TAG;
    free(ptmr);
}

//----������ʱ��----------------------------------------------------------------
//����: ������ʱ��,���ڶԶ�ʱ���������,�ú�������TRUEʱ,�������TMR_Unlock����;
//      �����ú�������FALSE,���������TMR_Unlock.
//��������ʱ������ָ��
//���أ��ɹ�:TRUE; ʧ��:FLASE;
//------------------------------------------------------------------------------
BOOL    TMR_Lock(TIMER *ptmr)
{
    GDD_Lock();
    if(NULL!=ptmr)
    if(TMR_TAG==ptmr->Tag)
    {
        return TRUE;
    }

    GDD_Unlock();
    return  FALSE;
}

//----������ʱ��----------------------------------------------------------------
//����: ����ʱ�������ɹ���,�ɸú������н�������.
//��������ʱ������ָ��
//���أ���
//------------------------------------------------------------------------------
void    TMR_Unlock(TIMER *ptmr)
{
    GDD_Unlock();
}

//----GDD��ʱ��ģ���ʼ��--------------------------------------------------------
//����: �ú�����GDD�ڲ�����
//��������
//���أ���
//------------------------------------------------------------------------------
void    GDD_TimerInit(void)
{
    list_init(&list_system_timer);
}

//----������ʱ��----------------------------------------------------------------
//����: ��
//������hwnd: ��ʱ����������.
//      Id:   ��ʱ��Id.
//      Flag: ��ʱ�����.
//      IntervalMS: ��ʱ���ʱ��(������).
//���أ���ʱ������ָ��
//------------------------------------------------------------------------------
TIMER*  GDD_CreateTimer(HWND hwnd,u16 Id,u32 IntervalMS,u16 Flag)
{
    TIMER *ptmr=NULL;

    if(HWND_Lock(hwnd))
    {
        ptmr =TMR_Alloc();;
        if(NULL!=ptmr)
        {

            list_init(&ptmr->node_sys);
            list_init(&ptmr->node_hwnd);
            list_init(&ptmr->node_msg_timer);

            ptmr->hwnd          =hwnd;
            ptmr->Id            =Id;
            ptmr->Flag          =Flag;
            ptmr->Interval      =IntervalMS;
            ptmr->HoldTime      =GUI_GetTickMS();

            GDD_Lock();
            list_insert_before(&list_system_timer,&ptmr->node_sys);
            list_insert_before(&hwnd->list_timer,&ptmr->node_hwnd);
            GDD_Unlock();

        }
        HWND_Unlock(hwnd);
    }
    return  ptmr;
}

//----���Ҷ�ʱ��----------------------------------------------------------------
//����: ��Id����ָ�����ڵĶ�ʱ��
//������hwnd: Ҫ���ҵĶ�ʱ�����ڴ���.
//      Id: Ҫ���ҵĶ�ʱ��Id
//���أ��ɹ����ض�ʱ������ָ��,���򷵻�NULL
//------------------------------------------------------------------------------
TIMER*  GDD_FindTimer(HWND hwnd,u16 Id)
{
    list_t *lst,*n;
    TIMER *ptmr=NULL;

    if(GDD_Lock())
    {
        if(HWND_Lock(hwnd))
        {
            lst =&hwnd->list_timer;
            n   =lst->next;
            while(n!=lst)
            {
                ptmr =(TIMER*)list_entry(n,TIMER,node_hwnd);
                if(NULL!=ptmr)
                {
                    if(ptmr->Id==Id)
                    {
                        break;
                    }
                }
                n=n->next;
            }
            HWND_Unlock(hwnd);
        }

        GDD_Unlock();
    }
    return ptmr;
}

//----���ö�ʱ��----------------------------------------------------------------
//����: �������ö�ʱ������
//������ptmr: ��ʱ������.
//      IntervalMS: ��ʱ���ʱ��(������).
//���أ�TRUE:�ɹ�; FALSE:ʧ��;
//------------------------------------------------------------------------------
BOOL    GDD_ResetTimer(TIMER *ptmr,u32 IntervalMS,u32 Flag)
{
    BOOL res=FALSE;

    if(TMR_Lock(ptmr))
    {
        if(HWND_Lock(ptmr->hwnd))
        {
            ptmr->Flag      =Flag;
            ptmr->Interval  =IntervalMS;
            ptmr->HoldTime  =GUI_GetTickMS();
            list_remove(&ptmr->node_msg_timer);

            res =TRUE;
            HWND_Unlock(ptmr->hwnd);
        }

        TMR_Unlock(ptmr);
    }

    return res;
}

//----ɾ����ʱ��----------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,�����в������.
//������ptmr: ��ʱ������.
//���أ���
//------------------------------------------------------------------------------
static void _DeleteTimer(TIMER*ptmr)
{

    list_remove(&ptmr->node_hwnd);
    list_remove(&ptmr->node_sys);
    list_remove(&ptmr->node_msg_timer);

    TMR_Free(ptmr);

}

//----ɾ����ʱ��----------------------------------------------------------------
//����: ��
//������ptmr: ��ʱ������.
//���أ�TRUE:�ɹ�; FALSE:ʧ��;
//------------------------------------------------------------------------------
BOOL    GDD_DeleteTimer(TIMER *ptmr)
{
    HWND hwnd;

    if(TMR_Lock(ptmr))
    {
        hwnd =ptmr->hwnd;
        if(HWND_Lock(hwnd))
        {
            _DeleteTimer(ptmr);
            HWND_Unlock(hwnd);
            return TRUE;
        }

        TMR_Unlock(ptmr);
    }
    return FALSE;
}

//----ɾ���������ж�ʱ��---------------------------------------------------------
//����: �ú���ΪGDD�ڲ�����
//������hwnd: ���ھ��.
//���أ���.
//------------------------------------------------------------------------------
void _RemoveWindowTimer(HWND hwnd)
{
    list_t *lst,*n,*next;
    TIMER *ptmr;

    lst =&hwnd->list_timer;
    n   =lst->next;

    while(!list_isempty(n))
    {
        next =n->next;
        ptmr =(TIMER*)list_entry(n,TIMER,node_hwnd);
        if(NULL!=ptmr)
        {
            if(TMR_Lock(ptmr))
            {
                _DeleteTimer(ptmr);
                TMR_Unlock(ptmr);
            }
        }

        n =next;
    }
}

//----��ʱ��������-------------------------------------------------------------
//����: �ú�����GDD�ڲ�����
//������ptmr: ��ʱ������ָ��;
//     tick_time_ms: ��ǰtickʱ��,���������GUI_GetTickMS�ṩ.
//���أ�TRUE:��ʱ����ʱ; FALSE:��ʱ��δ��ʱ.
//------------------------------------------------------------------------------
BOOL    _TimerHandler(TIMER *ptmr,u32 tick_time_ms)
{
    u32 time;

    if(ptmr->Flag&TMR_START)
    {
        if(tick_time_ms < ptmr->HoldTime)
        {
            //32λʱ��ֵ�������
            time =(0xFFFFFFFF-ptmr->HoldTime)+tick_time_ms;
        }
        else
        {
            time =tick_time_ms - ptmr->HoldTime;
        }

        //����Ƿ�ʱ
        if(time >= ptmr->Interval)
        {
            //����HoldTime
            ptmr->HoldTime =tick_time_ms;

            if(ptmr->Flag&TMR_SINGLE)
            {
                //��������˵��ζ�ʱ���,��ֹͣ�ö�ʱ��
                ptmr->Flag &=~TMR_START;
            }
            return TRUE;
        }

    }
    return FALSE;
}

//----GDDϵͳ��ʱ��ģ��ִ�к���--------------------------------------------------
//����: �ú�����GDD�ڲ�����,ÿ����һ�θú���,����ϵͳ���ж�ʱ��ɨ��һ���Ƿ���
//      ��ʱ�¼�,����г�ʱ�¼�����,����ö�ʱ���������ڷ��Ͷ�ʱ����ʱ��Ϣ.
//������tick_time_ms: ��ǰtickʱ��(������).
//���أ���
//------------------------------------------------------------------------------
void    GDD_TimerExecu(u32 tick_time_ms)
{
    list_t *lst,*n;
    TIMER *ptmr;

    if(GDD_Lock())
    {
        lst =&list_system_timer;
        n =lst->next;
        while(n!=lst)
        {
            ptmr =(TIMER*)list_entry(n,TIMER,node_sys);
            if(TMR_Lock(ptmr))
            {
                if(_TimerHandler(ptmr,tick_time_ms))
                {
                    _PostTimerMessage(ptmr);
                }
                TMR_Unlock(ptmr);
            }

            n =n->next;
        }

        GDD_Unlock();
    }
}


