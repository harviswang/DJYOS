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

#ifndef __MSGQUEUE_H__
#define __MSGQUEUE_H__

#include "stdint.h"
#include "rsc.h"
#include "lock.h"
#ifdef __cplusplus
extern "C" {
#endif

//��Ϣ��������ѡ��
#define CN_MSGQ_TYPE_MASK   0x01
#define CN_MSGQ_TYPE_FIFO   0x00    //�Ⱥ��ȡ��Ϣ���̰߳��ȵ��ȵ÷�ʽ�ȴ�
#define CN_MSGQ_TYPE_PRIO   0x01    //�Ⱥ��ȡ��Ϣ���̰߳����ȼ������ȵ�

#define CN_MSGQ_PRIO_NORMAL 0       //��ͨ��Ϣ
#define CN_MSGQ_PRIO_URGENT 1       //�����ȼ���Ϣ

struct tagMsgQueue
{
    struct tagRscNode MsgNode;
    struct tagSemaphoreLCB MsgSendSemp;   //���Ͷ˿����ź���
    struct tagSemaphoreLCB MsgRecvSemp;   //���ն˿����ź���

    u32    MsgSize;                     //
    u32    MsgUsed;                     //
    u32    ReadOffset;                  //
    u8     *buf;                        //
};

struct tagMsgQueue *MsgQ_Create( u32 MaxMsgs,u32  MsgLength,u32 Options);
ptu32_t ModuleInstall_MsgQ (ptu32_t para);
struct tagMsgQueue *MsgQ_Create_s(struct tagMsgQueue *pMsgQ, u32 MaxMsgs,
                                  u32  MsgLength,u32 Options,void *buf);
bool_t MsgQ_Delete(struct tagMsgQueue *pMsgQ);
bool_t MsgQ_Delete_s(struct tagMsgQueue *pMsgQ);
bool_t MsgQ_Send(struct tagMsgQueue *pMsgQ,u8 *buffer,u32 nBytes,u32 Timeout,
                 bool_t priority);
bool_t MsgQ_Receive(struct tagMsgQueue *pMsgQ,u8 *buffer,u32 nBytes,u32 Timeout);

#ifdef __cplusplus
}
#endif

#endif /* __MSGQUEUE_H__ */

