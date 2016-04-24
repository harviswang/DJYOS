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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
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
//����ģ��: ��Ϣ����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ṩ��Ϣ���й����ܡ�djyos����Ϣ����ģ���볣����RTOS��ʵ����һЩ��
//          ͬ��ɾ����Ϣ����ʱ��������¼������ڸ���Ϣ�����У�����RTOS����������
//          ���������������̣߳���djyos���������ǲ�����ɾ����djyos���ӽ�POSIX��
//          Ҫ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-07-25
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"
#include "systime.h"
#include "int.h"
#include "lock.h"
#include "msgqueue.h"
static struct tagRscNode s_tMsgQ_Node;
static struct tagMutexLCB s_tMsgQ_Mutex;

//----��ʼ��-------------------------------------------------------------------
//����: ��ʼ����Ϣ��������������Ϣ���и���Դ��㣬��ʼ��������
//����: para��������
//����: ������
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_MsgQ (ptu32_t para)
{
    Rsc_AddTree(&s_tMsgQ_Node,sizeof(struct tagRscNode),RSC_RSCNODE,"��Ϣ����");
    Lock_MutexCreate_s(&s_tMsgQ_Mutex,"��Ϣ����");
    return 0;
}

//----����һ����Ϣ����---------------------------------------------------------
//����: ������Ϣ���У�����ʼ����ز�������Ϣ���п��ƿ�ͱ�����Ϣ���ڴ棬���Ƕ�̬
//      ����ģ�����ʧ��(����NULL)��ԭ��ֻ�������ڴ治�㡣
//����: MaxMsgs�����е���Ϣ������
//      MsgLength��������Ϣ���ߴ硣
//      Options��ѡ�CN_MSGQ_TYPE_FIFO or CN_MSGQ_TYPE_PRIO������������Ϣ����
//          ��������ʱ���Ŷӷ�ʽ��
//����: �´�������Ϣ����ָ�룬ʧ���򷵻�NULL.
//-----------------------------------------------------------------------------
struct tagMsgQueue *MsgQ_Create( u32 MaxMsgs,u32  MsgLength,u32 Options)
{
    struct tagMsgQueue *MQ;
    //�����ڴ棬ͬʱ������Ϣ���п��ƿ�ʹ洢��Ϣ���ڴ档
    MQ = M_Malloc(sizeof(struct tagMsgQueue)+MsgLength*MaxMsgs,0);
    if(MQ != NULL)
    {
        Rsc_AddSon(&s_tMsgQ_Node,&MQ->MsgNode,sizeof(struct tagMsgQueue),
                                              RSC_MSGQ,"��Ϣ����");
        if( (Options & CN_MSGQ_TYPE_MASK) == CN_MSGQ_TYPE_PRIO)
        {
            Lock_SempCreate_s(&(MQ->MsgSendSemp),MaxMsgs,MaxMsgs,
                                CN_SEMP_BLOCK_PRIO,"��Ϣ����");
            Lock_SempCreate_s(&(MQ->MsgRecvSemp),MaxMsgs,0,
                                CN_SEMP_BLOCK_PRIO,"��Ϣ����");
        }
        else
        {
            Lock_SempCreate_s(&(MQ->MsgSendSemp),MaxMsgs,MaxMsgs,
                                CN_SEMP_BLOCK_FIFO,"��Ϣ����");
            Lock_SempCreate_s(&(MQ->MsgRecvSemp),MaxMsgs,0,
                                CN_SEMP_BLOCK_FIFO,"��Ϣ����");
        }
        MQ->MsgSize = MsgLength;
        MQ->MsgUsed = 0;
        MQ->ReadOffset = 0;
        MQ->buf = (u8*)(MQ + 1);
    }
    return MQ;
}


//----����һ����Ϣ����---------------------------------------------------------
//����: ������Ϣ���У�����ʼ����ز�������Ϣ���п��ƿ�ͱ�����Ϣ���ڴ棬�����û�
//      �ṩ������ָ�룬��Ϣ����ɾ����Ҳ���û�������ա�
//����: pMsgQ���û��ṩ����Ϣ���п��ƿ�ָ��
//      MaxMsgs�����е���Ϣ������
//      MsgLength��������Ϣ���ߴ硣
//      Options��ѡ�CN_MSGQ_TYPE_FIFO or CN_MSGQ_TYPE_PRIO������������Ϣ����
//          ��������ʱ���Ŷӷ�ʽ��
//      buf��������Ϣ���ڴ��ָ�롣
//����: �´�������Ϣ����ָ�룬ʧ���򷵻�NULL.
//-----------------------------------------------------------------------------
struct tagMsgQueue *MsgQ_Create_s(struct tagMsgQueue *pMsgQ, u32 MaxMsgs,
                                  u32  MsgLength,u32 Options,void *buf)
{
    if((pMsgQ != NULL) && (buf != NULL))
    {
        Rsc_AddSon(&s_tMsgQ_Node,&pMsgQ->MsgNode,sizeof(struct tagMsgQueue),
                                                 RSC_MSGQ,"��Ϣ����");
        if( (Options & CN_MSGQ_TYPE_MASK) == CN_MSGQ_TYPE_PRIO)
        {
            Lock_SempCreate_s(&(pMsgQ->MsgSendSemp),MaxMsgs,MaxMsgs,
                                CN_SEMP_BLOCK_PRIO,"��Ϣ����");
            Lock_SempCreate_s(&(pMsgQ->MsgRecvSemp),MaxMsgs,0,
                                CN_SEMP_BLOCK_PRIO,"��Ϣ����");
        }
        else
        {
            Lock_SempCreate_s(&(pMsgQ->MsgSendSemp),MaxMsgs,MaxMsgs,
                                CN_SEMP_BLOCK_FIFO,"��Ϣ����");
            Lock_SempCreate_s(&(pMsgQ->MsgRecvSemp),MaxMsgs,0,
                                CN_SEMP_BLOCK_FIFO,"��Ϣ����");
        }
        pMsgQ->MsgSize = MsgLength;
        pMsgQ->MsgUsed = 0;
        pMsgQ->ReadOffset = 0;
        pMsgQ->buf = buf;
        return pMsgQ;
    }
    else
        return NULL;
}

//----ɾ����Ϣ����-------------------------------------------------------------
//����: ɾ����Ϣ���У����ر�ע����ǣ�������¼������ڶ����У�ɾ����ʧ�ܣ�������
//      ��������е��¼������������RTOS��������POSIX��׼һ�¡�
//      POSIX�Ĺ涨�Ǻ���ģ�������Ϊ������ʩ��Ӧ��ֻ���ڿ���ʱ�ſ��Բ����
//����: pMsgQ����ɾ������Ϣ���С�
//����: true=�ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t MsgQ_Delete(struct tagMsgQueue *pMsgQ)
{
    bool_t result;
    if(pMsgQ != NULL)
    {
        Lock_MutexPend(&s_tMsgQ_Mutex,CN_TIMEOUT_FOREVER);
        if(Lock_SempCheckBlock(&(pMsgQ->MsgSendSemp))
                || Lock_SempCheckBlock(&(pMsgQ->MsgRecvSemp)) )
        {
            result = false;
        }
        else
        {
            Rsc_DelNode(&pMsgQ->MsgNode);          //ɾ���ź������
            Lock_SempDelete_s(&(pMsgQ->MsgSendSemp));
            Lock_SempDelete_s(&(pMsgQ->MsgRecvSemp));
            free(pMsgQ);
            result = true;
        }
        Lock_MutexPost(&s_tMsgQ_Mutex);
    }
    else
    {
        result = false;
    }
    return result;
}

//----ɾ����Ϣ����-------------------------------------------------------------
//����: ���ܺ�MsgQ_Delete�������ƣ�����ɾ����MsgQ_Create_s������������Ϣ����
//����: pMsgQ����ɾ������Ϣ���С�
//����: true=�ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t MsgQ_Delete_s(struct tagMsgQueue *pMsgQ)
{
    bool_t result=false;
    if(pMsgQ != NULL)
    {
        Lock_MutexPend(&s_tMsgQ_Mutex,CN_TIMEOUT_FOREVER);
        if( !(Lock_SempCheckBlock(&(pMsgQ->MsgSendSemp))
                || Lock_SempCheckBlock(&(pMsgQ->MsgRecvSemp)) ) )
        {
            Rsc_DelNode(&pMsgQ->MsgNode);          //ɾ���ź������
            Lock_SempDelete_s(&(pMsgQ->MsgSendSemp));
            Lock_SempDelete_s(&(pMsgQ->MsgRecvSemp));
            result = true;
        }
        Lock_MutexPost(&s_tMsgQ_Mutex);
    }
    return result;
}

//----������Ϣ-----------------------------------------------------------------
//����: ����һ����Ϣ����Ϣ����
//����: pMsgQ��Ŀ����Ϣ����
//      buffer������Ϣ������
//      nBytes������Ϣ�ĳ���
//      Timeout����ʱʱ�䣬us�������Ϣ���зǿգ���������ֱ����ʱ
//      priority����Ϣ���ȼ���ȡֵΪCN_MSGQ_PRIO_NORMAL��CN_MSGQ_PRIO_URGENT��
//          ��������Ϣ���ڶ���ͷ����β��
//����: true=�ɹ����ͣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t MsgQ_Send(struct tagMsgQueue *pMsgQ,u8 *buffer,u32 nBytes,u32 Timeout,
                 bool_t priority)
{
    u32 WriteOffset,sum,used;
    atom_low_t atom_low;
    if((pMsgQ == NULL) || (buffer == NULL))
        return false;
    if(nBytes > pMsgQ->MsgSize)
        return false;
    // todo �������⣬����Ϣ���б�ɾ����
    if( Lock_SempPend(&(pMsgQ->MsgSendSemp),Timeout) )  //����Ϣ���л��п�λû
    {
        //ȡ��ǰ�����е���Ϣ����
        sum = Lock_SempQueryCapacital(&(pMsgQ->MsgSendSemp));
        atom_low = Int_LowAtomStart();            //����ԭ�Ӳ���״̬
        used = pMsgQ->MsgUsed++;
        //����if���ȡ����Ϣ�ı���λ��
        if(priority == CN_MSGQ_PRIO_URGENT)
        {
            if(pMsgQ->ReadOffset== 0)
                WriteOffset = sum - 1;
            else
                WriteOffset = pMsgQ->ReadOffset-1;
            pMsgQ->ReadOffset = WriteOffset;
        }
        else
        {
            WriteOffset = used + pMsgQ->ReadOffset;
            if(WriteOffset >= sum)
                WriteOffset = WriteOffset - sum;
        }
        WriteOffset *= pMsgQ->MsgSize;
        memcpy(pMsgQ->buf + WriteOffset,buffer,nBytes);
        Int_LowAtomEnd(atom_low);
        Lock_SempPost(&(pMsgQ->MsgRecvSemp));
        return true;
    }
    else
        return false;
}

//----������Ϣ-----------------------------------------------------------------
//����: ����Ϣ�����н���һ����Ϣ
//����: pMsgQ��Ŀ����Ϣ����
//      buffer��������Ϣ�Ļ�����
//      nBytes����Ϣ�ĳ��ȣ����ܳ�����Ϣ���е���Ϣ����MsgSize
//      Timeout����ʱʱ�䣬us�������Ϣ���пգ���������ֱ����ʱ
//����: true=�ɹ����ͣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t MsgQ_Receive(struct tagMsgQueue *pMsgQ,u8 *buffer,u32 nBytes,u32 Timeout)
{
    u32 sum;
    atom_low_t atom_low;
    if((pMsgQ == NULL) || (buffer == NULL))
        return false;
    if(nBytes > pMsgQ->MsgSize)
        return false;
    if( Lock_SempPend(&(pMsgQ->MsgRecvSemp),Timeout) )
    {
        atom_low = Int_LowAtomStart();
        sum = Lock_SempQueryCapacital(&(pMsgQ->MsgSendSemp));
        memcpy(buffer,pMsgQ->buf+pMsgQ->ReadOffset * pMsgQ->MsgSize,nBytes);
        pMsgQ->MsgUsed--;
        if(pMsgQ->ReadOffset >= sum -1)
            pMsgQ->ReadOffset = 0;
        else
            pMsgQ->ReadOffset++;
        Int_LowAtomEnd(atom_low);
        Lock_SempPost(&(pMsgQ->MsgSendSemp));
        return true;
    }
    else
        return false;
}

//----ȡ��Ϣ��-----------------------------------------------------------------
//����: ȡ��Ϣ�����е���Ϣ��
//����: pMsgQ��Ŀ����Ϣ����
//����: ��Ϣ����
//-----------------------------------------------------------------------------
u32 MsgQ_NumMsgs(struct tagMsgQueue *pMsgQ)
{
    if(pMsgQ == NULL)
        return 0;
    else
        return pMsgQ->MsgUsed;
}

