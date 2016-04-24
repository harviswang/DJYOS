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
//����ģ��: ��ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �����ź����ͻ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __semp_h__
#define __semp_h__

#include "rsc.h"
#include "errno.h"
#ifdef __cplusplus
extern "C" {
#endif


struct  tagEventECB;

// ���¼������ź������ã���������ź�����ͬ�����У��ȴ������ź������¼��ͷ��ź�
// �����ȴ���������������ʽ�������ȼ�˳���ʱ��˳��ʽ��
// ֻ���ź����������Ⱥ�˳�򣬻�����ֻ�������ȼ�����ʽ��
#define CN_SEMP_BLOCK_FIFO          0       //�������ź������Ⱥ�˳���Ŷ�
#define CN_SEMP_BLOCK_PRIO          1       //�������ź������¼������ȼ��Ŷ�

//---------�������źŵ�����--------------
//1���������źſ��ṩ�ܱ�������Դ��ʹ�ü���
//2��Ϊ������Ҳ�ɲ���������Դ�ṩһ�µĴ��룬�������ṩ������ʱ���õĿ�����
struct tagSemaphoreLCB
{
    struct  tagRscNode node;
    u32 sync_order;
    u32 lamps_limit;   //�źŵ��������ޣ�cn_limit_uint32��ʾ��������
    u32 lamp_counter;  //�����źŵ�������
    struct  tagEventECB *semp_sync;    //�Ⱥ��źŵ��¼�����
};

//�������ǿ��Եݹ�����ģ���ͬһ���¼����������󣬵�Ҫ���ͷŵĴ���������Ĵ���
//��ͬ���������������һ���¼������������Σ���Ҫ�ͷ����β��������ͷš�
struct tagMutexLCB
{
    struct tagRscNode node;
    s32  enable;                        //0=���ã�>0 = ���߳�ռ�ã�<0 = �ж�ռ��
    ufast_t  prio_bak;                  //���ȼ��̳��б���ԭ���ȼ�
    struct  tagEventECB *owner;         //ռ�û��������¼��������ж�ռ�ã�����Ч
    struct  tagEventECB *mutex_sync;    //�Ⱥ򻥳������¼����У����ȼ��Ŷ�
};

//�����ź����ͻ����������ڴ��
union lock_MCB
{
   struct tagSemaphoreLCB sem;
   struct tagMutexLCB  mut;
};

//������Ϣ����
enum _LOCK_ERROR_CODE_
{
    EN_LOCK_NO_ERROR = CN_LOCK_NO_ERROR,      //û�д���
    EN_LOCK_PARA_ERROR,             //��������������
    EN_LOCK_RANGE_ERROR,            //�ź�����Χ��������
    EN_LOCK_BLOCK_IN_INT,           //�ж�ISR����������
};

ptu32_t ModuleInstall_Lock1(ptu32_t para);
ptu32_t ModuleInstall_Lock2(ptu32_t para);
struct tagSemaphoreLCB *Lock_SempCreate(u32 lamps_limit,u32 init_lamp,
                                        u32 sync_order,char *name);
struct tagSemaphoreLCB *Lock_SempCreate_s( struct tagSemaphoreLCB *semp,
                       u32 lamps_limit,u32 init_lamp,u32 sync_order,char *name);
void Lock_SempPost(struct tagSemaphoreLCB *semp);
bool_t Lock_SempPend(struct tagSemaphoreLCB *semp,u32 timeout);
bool_t Lock_SempDelete_s(struct tagSemaphoreLCB *semp);
bool_t Lock_SempDelete(struct tagSemaphoreLCB *semp);
u32 Lock_SempQueryCapacital(struct tagSemaphoreLCB *semp);
bool_t Lock_SempCheckBlock(struct tagSemaphoreLCB *Semp);
u32 Lock_SempQueryFree(struct tagSemaphoreLCB *semp);
void Lock_SempSetSyncSort(struct tagSemaphoreLCB *semp,u32 order);
struct tagMutexLCB *Lock_MutexCreate(char *name);
struct tagMutexLCB *  Lock_MutexCreate_s( struct tagMutexLCB *mutex,char *name);
void Lock_MutexPost(struct tagMutexLCB *mutex);
bool_t Lock_MutexPend(struct tagMutexLCB *mutex,u32 timeout);
bool_t Lock_MutexDelete(struct tagMutexLCB *mutex);
bool_t Lock_MutexDelete_s(struct tagMutexLCB *mutex);
bool_t Lock_MutexQuery(struct tagMutexLCB *mutex);
bool_t Lock_MutexCheckBlock(struct tagMutexLCB *mutex);
u16 Lock_MutexGetOwner(struct tagMutexLCB *mutex);

#ifdef __cplusplus
}
#endif

#endif //__semp_h__

