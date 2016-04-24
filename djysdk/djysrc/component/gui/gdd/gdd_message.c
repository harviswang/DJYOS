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
//�ļ�����: ��Ϣϵͳ����ʵ��
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

/*============================================================================*/

typedef struct  tagQMSG
{
    MSG Msg;
    struct  tagQMSG *Next;

}QMSG;

struct  tagMSGQ
{
    struct  tagMutexLCB *mutex_lock;      //��Ϣ������
    struct  tagSemaphoreLCB   *sem_msg;         //��Ϣ�ź���
    struct  tagSemaphoreLCB   *sem_sync_send;   //ͬ����Ϣ�����ź���
    struct  tagSemaphoreLCB   *sem_sync_recv;   //ͬ����Ϣ�����ź���
    MSG     sync_msg;           //ͬ����Ϣ
    MSG     quit_msg;           //�˳���Ϣ
    list_t  list_msg_close;     //CLOSE��Ϣ����
    list_t  list_msg_ncpaint;   //NCPAINT��Ϣ����
    list_t  list_msg_paint;     //PAINT��Ϣ����
    list_t  list_msg_timer;     //TIMER��Ϣ����

    //Post������Ϣ����
    void    *post_pbuf;     //��¼ԭʼ��post��Ϣ��������ַ(���ڶ�̬�ڴ��ͷ�)
    QMSG    *post_free;     //ָ����е�post��Ϣ��������
    QMSG    *post_first;    //ָ���һ��post��Ϣ����
    QMSG    *post_last;     //ָ�����һ��post��Ϣ����ڵ�


};

//----����һ����Ϣ--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������dst: Ŀ����Ϣָ��.
//      src: Դ��Ϣָ��.
//���أ���.
//------------------------------------------------------------------------------
static  void _CopyMsg(MSG *dst,const MSG* src)
{
    dst->hwnd   =src->hwnd;
    dst->Code   =src->Code;
    dst->Param1 =src->Param1;
    dst->Param2 =src->Param2;
    //dst->Time     =src->Time;
    dst->ExData =NULL;
}

//----��ʼ��һ����Ϣ-------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������msg: ��Ϣ�ṹ��ָ��.
//     hwnd: ��Ϣ���ھ��.
//     code: ��Ϣ�����.
//     param1: ��Ϣ����1.
//     param2: ��Ϣ����2.
//���أ���.
//------------------------------------------------------------------------------
static  void _InitMsg(MSG *msg,HWND hwnd,u32 code,u32 param1,ptu32_t param2)
{
    msg->hwnd   =hwnd;
    msg->Code   =code;
    msg->Param1 =param1;
    msg->Param2 =param2;
    //msg->Time     =WDD_GetTickMS();
    msg->ExData =NULL;
}

//----����һ����Ϣ����-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//������size: �첽��Ϣ����������.
//���أ���Ϣ����ָ��.
//------------------------------------------------------------------------------
MSGQ*   GUI_CreateMsgQ(u8 size)
{
    MSGQ *pMsgQ=NULL;
    int i;

    pMsgQ =(MSGQ*)malloc(sizeof(MSGQ));
    if(NULL==pMsgQ)
    {
        return NULL;
    }

    //������Ϣ������
    pMsgQ->mutex_lock =Lock_MutexCreate(NULL);
    if(NULL==pMsgQ->mutex_lock)
    {
        goto err1;
    }

    //������Ϣ�ź���
    pMsgQ->sem_msg  =Lock_SempCreate(1000,0,CN_SEMP_BLOCK_PRIO,NULL);
    if(NULL==pMsgQ->sem_msg)
    {
        goto err2;
    }

    pMsgQ->sem_sync_send =Lock_SempCreate(1,0,CN_SEMP_BLOCK_PRIO,NULL);
    pMsgQ->sem_sync_recv =Lock_SempCreate(1,0,CN_SEMP_BLOCK_PRIO,NULL);

    //����post��Ϣ��������
    pMsgQ->post_pbuf =(void*)malloc(size*sizeof(QMSG));
    if(NULL==pMsgQ->post_pbuf)
    {
        goto err3;
    }

    //��ʼ��post��Ϣ��������
    pMsgQ->post_free =(QMSG*)pMsgQ->post_pbuf;
    for(i=0;i<(size-1);i++)
    {
        pMsgQ->post_free[i].Next =  &pMsgQ->post_free[i+1];
    }
    pMsgQ->post_free[size-1].Next =NULL;

    pMsgQ->post_first =NULL;
    pMsgQ->post_last  =NULL;

    //...
    list_init(&pMsgQ->list_msg_close);
    list_init(&pMsgQ->list_msg_ncpaint);
    list_init(&pMsgQ->list_msg_paint);
    list_init(&pMsgQ->list_msg_timer);

    _InitMsg(&pMsgQ->quit_msg,0,0,0,0);

    return pMsgQ;

    ////

err3:
    Lock_SempDelete(pMsgQ->sem_msg);
err2:
    Lock_MutexDelete(pMsgQ->mutex_lock);
err1:
    free(pMsgQ);
    return NULL;
}

void    GUI_DeleteMsgQ(MSGQ *pMsgQ)
{
    Lock_SempDelete(pMsgQ->sem_msg);
    Lock_SempDelete(pMsgQ->sem_sync_send);
    Lock_SempDelete(pMsgQ->sem_sync_recv);

    Lock_MutexDelete(pMsgQ->mutex_lock);

    free(pMsgQ->post_pbuf);
    free(pMsgQ);

}

//----�ɷ���Ϣ------------------------------------------------------------------
//����: ͬ������,ֱ������Ϣ�����ڹ��̴������,�ú����Ż᷵��.
//������pMsg: ��Ҫ�ɷ�����Ϣָ��.
//���أ���Ϣ������.
//------------------------------------------------------------------------------
u32 DispatchMessage(MSG *pMsg)
{
    u32 res=0;
    WNDPROC *WndProc;

    if(NULL!=pMsg)
    {
        if(_GetWindowEvent(pMsg->hwnd) == Djy_MyEventId())
        { //ͬ�߳���,ֱ�ӵ��ô��ڹ���.
            WndProc =_GetWindowProc(pMsg->hwnd);
            res =WndProc(pMsg);
        }
    }

    return res;
}

//----����ͬ����Ϣ--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,�����ڿ��̷߳���ͬ����Ϣ.
//������hwnd: ���ھ��.
//      msg: ��Ϣ����.
//      param1: ��Ϣ����1.
//      param2: ��Ϣ����2.
//���أ���Ϣ������.
//------------------------------------------------------------------------------
static u32 _PostSyncMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    MSGQ *pMsgQ;
    u32 res;


    pMsgQ =_GetWindowMsgQ(hwnd);

    Lock_MutexPend(pMsgQ->mutex_lock,5000000);

    _InitMsg(&pMsgQ->sync_msg,hwnd,msg,param1,param2);


    Lock_SempPost(pMsgQ->sem_sync_recv);
    Lock_SempPost(pMsgQ->sem_msg);    //������Ϣ�ź���

    Lock_SempPend(pMsgQ->sem_sync_send,CN_TIMEOUT_FOREVER);
    res=pMsgQ->sync_msg.Param1;

    Lock_MutexPost(pMsgQ->mutex_lock);

    return res;

}

//----����ͬ����Ϣ--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������pMsgQ:��Ϣ����ָ��.
//���أ���.
//------------------------------------------------------------------------------
static void _HandleSyncMessage(MSGQ *pMsgQ)
{
    if(Lock_SempPend(pMsgQ->sem_sync_recv,1000))
    {
        WNDPROC *WndProc;
        MSG *pMsg;

        pMsg =&pMsgQ->sync_msg;
        WndProc =_GetWindowProc(pMsg->hwnd);
        pMsgQ->sync_msg.Param1 =WndProc(pMsg);

        Lock_SempPost(pMsgQ->sem_sync_send);

    }
}

//----������Ϣ------------------------------------------------------------------
//����: ͬ������,ֱ������Ϣ�����ڹ��̴������,�����Ż᷵��,�ú������Զ������߳�
//      �Ϳ��߳�ͬ����Ϣ���ʹ���.
//������hwnd: ���ھ��.
//      msg: ��Ϣ����.
//      param1: ��Ϣ����1.
//      param2: ��Ϣ����2.
//���أ���Ϣ������.
//------------------------------------------------------------------------------
u32 SendMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    u32 res=0;
    MSG Msg;

    if(msg==MSG_CLOSE)
    {
        //MSG_CLOSE �����첽����
        PostMessage(hwnd,msg,param1,param2);
    }
    else
    {
        _InitMsg(&Msg,hwnd,msg,param1,param2);
        if(HWND_Lock(hwnd))
        {
            if(_GetWindowEvent(hwnd) == Djy_MyEventId())
            {
                //�����ͬһ�߳���,ֱ�ӵ��ô��ڹ���
                WNDPROC *WndProc;

                WndProc =_GetWindowProc(hwnd);
                res =WndProc(&Msg);
                HWND_Unlock(hwnd);

            }
            else
            {
                HWND_Unlock(hwnd);
                res=_PostSyncMessage(hwnd,msg,param1,param2);
            }

        }
    }

    return res;
}

//----���ͷǿͻ���������Ϣ-------------------------------------------------------
//����:����һ���ǿͻ���������Ϣ����Ϣ����.�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ: ��Ϣ����ָ��.
//      hwnd: ��Ҫ���ƵĴ��ھ��.
//���أ���.
//------------------------------------------------------------------------------
void    _PostNCPaintMessage(MSGQ *pMsgQ,HWND hwnd)
{
    //�жϴ����Ƿ�Ϊ�ɼ�,������ڲ��ɼ�,�򲻷��ͻ�����Ϣ.
    if(_IsWindowVisible(hwnd))
    {
        //�жϴ���node_msg_ncpaint�Ƿ�Ϊ��,�����Ϊ��,
        //���ʾ�ô��ڽڵ��Ѿ����뵽��Ϣ���е� list_msg_ncpaint��.
        if(list_isempty(&hwnd->node_msg_ncpaint))
        {
            //���뵽��Ϣ����list_msg_ncpaint������ĩβ.
            list_insert_before(&pMsgQ->list_msg_ncpaint,&hwnd->node_msg_ncpaint);
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);
    }

}

//----���Ϳͻ���������Ϣ---------------------------------------------------------
//����:����һ���ͻ���������Ϣ����Ϣ����.�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ: ��Ϣ����ָ��.
//      hwnd: ��Ҫ���ƵĴ��ھ��.
//���أ���.
//------------------------------------------------------------------------------
void    _PostPaintMessage(MSGQ *pMsgQ,HWND hwnd)
{
    //�жϴ����Ƿ�Ϊ�ɼ�,������ڲ��ɼ�,�򲻷��ͻ�����Ϣ.
    if(_IsWindowVisible(hwnd))
    {
        //�жϴ���node_msg_paint�Ƿ�Ϊ��,�����Ϊ��,
        //���ʾ�ô��ڽڵ��Ѿ����뵽��Ϣ���е� list_msg_paint��.
        if(list_isempty(&hwnd->node_msg_paint))
        {
            //���뵽��Ϣ����list_msg_paint������ĩβ.
            list_insert_before(&pMsgQ->list_msg_paint,&hwnd->node_msg_paint);
        }
        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);
    }
}

//----���Ͷ�ʱ����Ϣ---------------------------------------------------------
//����:���Ͷ�ʱ����Ϣ����Ϣ����.�ú���Ϊ�ڲ�����.
//������ptmr: ��ʱ���ṹָ��.
//���أ���.
//------------------------------------------------------------------------------
void    _PostTimerMessage(TIMER *ptmr)
{
    MSGQ *pMsgQ;
    HWND hwnd=ptmr->hwnd;

    if(HWND_Lock(hwnd))
    {
        pMsgQ =_GetWindowMsgQ(hwnd);

        if(list_isempty(&ptmr->node_msg_timer))
        {
            //���뵽��Ϣ����list_msg_msg_timer������ĩβ.
            list_insert_before(&pMsgQ->list_msg_timer,&ptmr->node_msg_timer);
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);

        HWND_Unlock(hwnd);
    }
}

//----���ʹ��ڹر���Ϣ-----------------------------------------------------------
//����:�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ:��Ϣ����ָ��.
//      hwnd:��Ҫ�رյĴ��ھ��.
//���أ���.
//------------------------------------------------------------------------------
void    _PostCloseMessage(MSGQ *pMsgQ,HWND hwnd)
{

    //�жϴ���node_msg_close�Ƿ�Ϊ��,�����Ϊ��,
    //���ʾ�ô��ڽڵ��Ѿ����뵽��Ϣ���е� list_msg_close��.
    if(list_isempty(&hwnd->node_msg_close))
    {
        //���뵽��Ϣ����list_msg_close������ĩβ.
        list_insert_before(&pMsgQ->list_msg_close,&hwnd->node_msg_close);
    }
    //������Ϣ�ź���
    Lock_SempPost(pMsgQ->sem_msg);

}


//----�����첽��Ϣ--------------------------------------------------------------
//����: ������Ϣ����Ϣ������,���ȴ�����,��������.�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ:��Ϣ����ָ��.
//      hwnd:���ھ��.
//      msg:��Ϣ����.
//      param1:��Ϣ����1.
//      param2:��Ϣ����2.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    _PostMessage(MSGQ *pMsgQ,HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    QMSG *new_msg;
    BOOL res=FALSE;

    //�ж��Ƿ��п��е�QMSG
    if(NULL!=pMsgQ->post_free)
    {
        //�ӿ��е�post����ȡһ��QMSG
        new_msg = pMsgQ->post_free;
        pMsgQ->post_free  = new_msg->Next;

        //��ʼ���µ���Ϣ����
        new_msg->Next =NULL;
        _InitMsg(&new_msg->Msg,hwnd,msg,param1,param2);

        if(NULL==pMsgQ->post_first)
        {
            pMsgQ->post_first =new_msg;
        }

        //������Ϣ���뵽post����ĩβ.
        if(NULL==pMsgQ->post_last)
        {
            pMsgQ->post_last =new_msg;
        }
        else
        {
            pMsgQ->post_last->Next =new_msg;
            pMsgQ->post_last =new_msg;
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);
        res =TRUE;
    }

    return res;
}

//----���ʹ����˳���Ϣ-----------------------------------------------------------
//����: ��.
//������hwnd:���ھ��.
//     exit_code: �����˳�����.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    PostQuitMessage(HWND hwnd,u32 exit_code)
{
    MSGQ *pMsgQ;

    if(HWND_Lock(hwnd))
    {
        pMsgQ =_GetWindowMsgQ(hwnd);
        _InitMsg(&pMsgQ->quit_msg,hwnd,MSG_QUIT,0,0);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----�����첽��Ϣ--------------------------------------------------------------
//����: ������Ϣ����Ϣ������,���ȴ�����,��������.
//������hwnd:���ھ��.
//      msg:��Ϣ����.
//      param1:��Ϣ����1.
//      param2:��Ϣ����2.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    PostMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    MSGQ *pMsgQ;
    BOOL res=FALSE;

    if(HWND_Lock(hwnd))
    {
        pMsgQ =_GetWindowMsgQ(hwnd);
        if(NULL!=pMsgQ)
        {
            switch(msg)
            {
                case MSG_PAINT:
                    _PostPaintMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;
                    ////

                case MSG_NCPAINT:
                    _PostNCPaintMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;
                    ////

                case MSG_CLOSE:
                    _PostCloseMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;
                    ////

                default:
                    res=_PostMessage(pMsgQ,hwnd,msg,param1,param2);
                    break;
                    ////
            }

        }

        HWND_Unlock(hwnd);
    }
    return res;
}

//----��ȡ�첽��Ϣ--------------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ���첽��Ϣ.�ú���Ϊ�ڲ�����,����麯��
//      �����ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  BOOL    _PeekPostMessage(MSGQ *pMsgQ,MSG *pMsg)
{
    QMSG *new_msg;
    BOOL res=FALSE;

    //�ж��Ƿ�����Ϣ
    if(NULL!=pMsgQ->post_first)
    {
        //��post����ȡһ��QMSG
        new_msg = pMsgQ->post_first;
        pMsgQ->post_first  = new_msg->Next;

        //��ʼ���µ���Ϣ����
        new_msg->Next =NULL;
        _CopyMsg(pMsg,&new_msg->Msg);

        //�����ǰ��ȡ�������һ����Ϣ,��post_last ����Ϊ NULL
        if(new_msg==pMsgQ->post_last)
        {
            pMsgQ->post_last =NULL;
        }

        //���������QMSG��ӵ�����������
        if(NULL==pMsgQ->post_free)
        {
            pMsgQ->post_free =new_msg;
        }
        else
        {
            new_msg->Next =pMsgQ->post_free;
            pMsgQ->post_free =new_msg;
        }

        res =TRUE;
    }

    return res;
}

//----��ȡ�ǿͻ���������Ϣ-------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ���ǿͻ���������Ϣ.�ú���Ϊ�ڲ�����,�����
//      ���������ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  BOOL    _PeekNCPaintMessage(MSGQ *pMsgQ,MSG *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!list_isempty(&pMsgQ->list_msg_ncpaint))
    {
        n =pMsgQ->list_msg_ncpaint.next;
        list_remove(n);
        hwnd =list_entry(n,WINDOW,node_msg_ncpaint);
        _InitMsg(pMsg,hwnd,MSG_NCPAINT,0,0);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ�ͻ���������Ϣ---------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ���ͻ���������Ϣ.�ú���Ϊ�ڲ�����,�����
//      ���������ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  BOOL    _PeekPaintMessage(MSGQ *pMsgQ,MSG *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!list_isempty(&pMsgQ->list_msg_paint))
    {
        n =pMsgQ->list_msg_paint.next;
        list_remove(n);
        hwnd =list_entry(n,WINDOW,node_msg_paint);
        _InitMsg(pMsg,hwnd,MSG_PAINT,0,0);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ��ʱ����Ϣ------------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ����ʱ����Ϣ.�ú���Ϊ�ڲ�����,����麯��
//      �����ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  BOOL    _PeekTimerMessage(MSGQ *pMsgQ,MSG *pMsg)
{
    list_t *n;
    TIMER *ptmr;

    if(!list_isempty(&pMsgQ->list_msg_timer))
    {
        n =pMsgQ->list_msg_timer.next;
        list_remove(n);
        ptmr =list_entry(n,TIMER,node_msg_timer);
        _InitMsg(pMsg,ptmr->hwnd,MSG_TIMER,ptmr->Id,(u32)ptmr);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ���ڹر���Ϣ-----------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ�����ڹر���Ϣ.�ú���Ϊ�ڲ�����,����麯��
//      �����ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  BOOL    _PeekCloseMessage(MSGQ *pMsgQ,MSG *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!list_isempty(&pMsgQ->list_msg_close))
    {
        n =pMsgQ->list_msg_close.next;
        list_remove(n);
        hwnd =list_entry(n,WINDOW,node_msg_close);
        _InitMsg(pMsg,hwnd,MSG_CLOSE,0,0);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ��Ϣ------------------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ����Ϣ.
//������pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//     hwnd: ���ھ��.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
BOOL    PeekMessage(MSG *pMsg,HWND hwnd)
{
    MSGQ *pMsgQ;
    BOOL res=FALSE;

    if(NULL!=pMsg)
    if(HWND_Lock(hwnd))
    {
        pMsgQ =_GetWindowMsgQ(hwnd);
        if(NULL!=pMsgQ)
        {
            do
            {
                if(pMsgQ->quit_msg.Code == MSG_QUIT)
                {
                    _InitMsg(pMsg,hwnd,MSG_QUIT,0,0);
                    _DeleteMainWindowData(hwnd);

                    res=TRUE;
                    break;
                }
                if(_PeekCloseMessage(pMsgQ,pMsg))
                {
                    res=TRUE;
                    break;
                }

                HWND_Unlock(hwnd);
                _HandleSyncMessage(pMsgQ);
                HWND_Lock(hwnd);

                if(_PeekPostMessage(pMsgQ,pMsg))
                {
                    res=TRUE;
                    break;
                }

                if(_PeekNCPaintMessage(pMsgQ,pMsg))
                {
                    res=TRUE;
                    break;
                }

                if(_PeekPaintMessage(pMsgQ,pMsg))
                {
                    res=TRUE;
                    break;
                }

                if(_PeekTimerMessage(pMsgQ,pMsg))
                {
                    res=TRUE;
                    break;
                }


            }while(0);
        }

        HWND_Unlock(hwnd);
    }

    return res;
}

/*============================================================================*/

//----��ȡ��Ϣ------------------------------------------------------------------
//����: ����Ϣ��������������ʽ��ȡһ����Ϣ.
//������pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//     hwnd: ���ھ��.
//���أ��������MSG_QUITʱ,�ú�������FALSE,���򷵻�TRUE.
//------------------------------------------------------------------------------
BOOL    GetMessage(MSG *pMsg,HWND hwnd)
{
    int res=FALSE;

    while(1)
    {
        if(PeekMessage(pMsg,hwnd))
        {
            if(pMsg->Code!=MSG_QUIT)
            {
                res=TRUE;
            }
            else
            {
                res=FALSE;
            }
            break;
        }
        else
        {
            MSGQ *pMsgQ;

            if(HWND_Lock(hwnd))
            {
                pMsgQ =_GetWindowMsgQ(hwnd);
                HWND_Unlock(hwnd);

                if(NULL!=pMsgQ)
                {
                    Lock_SempPend(pMsgQ->sem_msg,CN_TIMEOUT_FOREVER);
                }

            }
        }
    }
    return res;
}

/*============================================================================*/
