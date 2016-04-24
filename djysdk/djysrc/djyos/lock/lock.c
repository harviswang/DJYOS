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
//3. ����:2013-2-20
//   ����:  ������.
//   �°汾�ţ�1.1.0
//   �޸�˵��: �޸�mutex��ش��룬ʹ֮��ͬһ���¼��ڣ��ɵݹ���ã�����һ���¼�
//      ӵ��mutex�������¼��ٵ���pend�Ļ������������������ٴε���pend��������
//      ���ü����������������������ͬ��������post�󣬲��������ͷ�mutex��
//2. ����:2013-1-4
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������mutex_delete������һ�����󣬸�bug������"��ë��"�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "config-prj.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "int.h"
#include "lock.h"
#include "pool.h"
#include "djyos.h"
#include "rsc.h"
#include "systime.h"

static struct tagRscNode *pg_semaphore_rsc=NULL;
static struct tagRscNode  *pg_mutex_rsc=NULL;
//˵���� gc_u32CfgLockLimit ���û����õģ������û�����֪������ϵͳ��Ҫ�ö����ź�����
//      ���Բ���ϵͳ����ռ�� gc_u32CfgLockLimit ָ�꣬�û�ʹ�õ��ź�����
//      tg_semp_pool������ڴ���з��䣬����ϵͳʹ�õ��ź����Լ����壬����
//      ��أ�����û���ͼ���� Lock_SempDelete ɾ������ϵͳ������ź��������������
//      �ڴ�й©��������ز���ϵͳ��ԴҲ��ʧȥ�ź����������������Ԥ��
//�����ź����ṹ��ʼ�ڴ��
static union lock_MCB *g_ptLockMemBlock;
struct tagMemCellPool *g_ptLockPool;  //�ź����ṹ�ڴ��ͷָ��

//������rscģ���ж���ģ�ר��lockģ��ʹ�õ�һ������
struct  tagRscNode * __Lock_RscAddLockTree(struct  tagRscNode *node,
                                u32 size,char *name);

extern void __Djy_EventReady(struct  tagEventECB *event_ready);
extern void __Djy_CutReadyEvent(struct tagEventECB *event);
extern void __Djy_ResumeDelay(struct  tagEventECB *delay_event);
extern void ___Djy_AddToDelay(u32 u32l_uS);

//----��ʼ����ģ��ģ��step1----------------------------------------------------
//���ܣ���ʼ���ź���ģ��ĵ�һ�����˺���Ե��ó�semp_create��mutex_create�����
//      ��������������semp_create_s��mutex_createe_s��������������ֹ����
//      Ϊmodule_init_memb��û�е��ã�����mudule_init_memb��Ҫʹ���ź�������ֻ��
//      ��module_init_lock1������á�
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Lock1(ptu32_t para)
{
    static struct tagRscNode semp_root;
    static struct tagRscNode mutex_root;
    para = para;        //�����������澯
    pg_semaphore_rsc = __Lock_RscAddLockTree(&semp_root,sizeof(struct tagRscNode),"semaphore");
    pg_mutex_rsc = __Lock_RscAddLockTree(&mutex_root,sizeof(struct tagRscNode),"mutex");
    return 1;
}

//----��ʼ����ģ��ģ��step2----------------------------------------------------
//���ܣ���ʼ���ź���ģ��ĵڶ������˺���Ե��ú�semp_create��mutex_createe���ڵ�
//      ������������
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Lock2(ptu32_t para)
{
    static struct tagMemCellPool lock_pool;
    g_ptLockMemBlock = M_Malloc(gc_u32CfgLockLimit* sizeof(union lock_MCB),0);
    if(g_ptLockMemBlock == NULL)
        return 0;
    //��ʼ�������ƿ��ڴ��
    g_ptLockPool = Mb_CreatePool_s( &lock_pool,
                                    (void*)g_ptLockMemBlock,
                                    gc_u32CfgLockLimit,
                                    sizeof(union lock_MCB),
                                    0,0,
                                    "�����ƿ��");
    return 1;
}

//----����һ���ź���-----------------------------------------------------------
//���ܣ�����һ���ź������Ժ�Ϳ���ʹ������ź����ˡ��ź������ƿ���ڴ���з��䡣
//������semp_limit,�źŵƵ�������cn_limit_uint32=�����޶���źŵơ�
//      init_lamp,��ʼ�źŵ�����
//      name���ź��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//      sync_order,���������¼��Ŷ�˳��ȡֵΪ CN_SEMP_BLOCK_FIFO
//          �� CN_SEMP_BLOCK_PRIO
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct tagSemaphoreLCB *Lock_SempCreate(u32 lamps_limit,u32 init_lamp,
                                        u32 sync_order,char *name)
{
    struct tagSemaphoreLCB *semp;
    if(init_lamp > lamps_limit)
        return NULL;
    semp = Mb_Malloc(g_ptLockPool,0);
    if(semp == NULL)
        return NULL;
    semp->sync_order = sync_order;
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->semp_sync = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    Rsc_AddSon(pg_semaphore_rsc,&semp->node,
                        sizeof(struct tagSemaphoreLCB),RSC_SEMP,name);
    return semp;
}

//----�����ź���---------------------------------------------------------------
//���ܣ�����һ���ź��������������ṩ�ź������ƿ飬�߿ɿ��Ե�Ӧ���У���Ӧ��ʹ�ö�
//      ̬����ķ�ʽ����̬������ɿ���Ȼ���ָ�봫�ݹ������ں���ʹ�õ��ź�������
//      ��ʹ�þ�̬����ġ�Ӧ�ó�������cfg_locks_limitʱ��ֻ��Ҫ�����Լ��������
//      �����ˡ�
//������semp,Ŀ���ź���ָ��
//      semp_limit,�źŵƵ�������cn_limit_uint32=�����޶���źŵơ�
//      init_lamp,��ʼ�źŵ�����
//      name���ź��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct tagSemaphoreLCB *Lock_SempCreate_s( struct tagSemaphoreLCB *semp,
                       u32 lamps_limit,u32 init_lamp,u32 sync_order,char *name)
{
    if(semp == NULL)
        return NULL;
    semp->sync_order = sync_order;
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->semp_sync = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    Rsc_AddSon(pg_semaphore_rsc,&semp->node,
                        sizeof(struct tagSemaphoreLCB),RSC_SEMP,name);

    return semp;
}

//----����һ���źŵ�------------------------------------------------------------
//���ܣ��ͷ�һ���ź����������ź�����1��
//������semp,�ź���ָ��
//���أ���
//-----------------------------------------------------------------------------
void Lock_SempPost(struct tagSemaphoreLCB *semp)
{
    struct  tagEventECB *event;

    if(semp == NULL)
        return;
    Int_SaveAsynSignal();
    if(semp->semp_sync == NULL)     //�ȴ����пգ�����һյ�źŵ�
    {
        if(semp->lamps_limit == CN_LIMIT_UINT32)        //�źŵ���������
        {
            if(semp->lamp_counter != CN_LIMIT_UINT32)   //�źŵ�����δ������
                semp->lamp_counter ++;                  //����һյ�źŵ�
        }else if(semp->lamp_counter < semp->lamps_limit )//�źŵ�����δ������
            semp->lamp_counter++;                       //����һյ�źŵ�
    } else      //���¼��ڵȴ������źŵ�ֱ��ת�������������ȼ���ߵ��¼�
    {
        event = semp->semp_sync;
        event->sync_head = NULL;
        if(event->multi_next == event)
        {//������ֻ��һ���¼�
            semp->semp_sync = NULL;
        }else
        {//�������ж���¼���ȡ������ͷ�����¼�
            semp->semp_sync = event->multi_next;
            event->multi_next->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = event->multi_next;
            event->multi_next = NULL;
            event->multi_previous = NULL;
        }
        if(event->event_status & CN_STS_SYNC_TIMEOUT)
            __Djy_ResumeDelay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
        event->wakeup_from = CN_STS_WAIT_SEMP;
        event->event_status = CN_STS_EVENT_READY;
        __Djy_EventReady(event);
    }
    Int_RestoreAsynSignal();
}

//----����һյ�źŵ�-----------------------------------------------------------
//���ܣ�����Ȼ��Ϩ��һյ�źŵƣ���ʾ������Դ����1��
//������semp,�ź���ָ��
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�true=ȡ���źŷ��ػ���Դ�����źŵƱ���(semp == NULL)��semp==NULLʱ����
//      true��������ģ������ϣ�������ڲ����Ƿ����źŵƱ���������һ�£���semp
//      ��ΪNULL�����ǵ�ѡ�񣬱�����Ҫ����һ�������źŵƱ������豸���ɰѸ��豸
//      struct pan_device��semp��Ա��ΪNULL��
//      false=û��ȡ���ź�(��ʱ���ػ�����ԭ��)
//-----------------------------------------------------------------------------
bool_t Lock_SempPend(struct tagSemaphoreLCB *semp,u32 timeout)
{
    struct  tagEventECB *event;
    bool_t lamp,sch;

    if(semp == NULL)
    {
        g_ptEventRunning->error_no = EN_LOCK_PARA_ERROR;
        return false;
    }
    sch = Djy_QuerySch();
    Int_SaveAsynSignal();
    if(semp->lamps_limit == CN_LIMIT_UINT32)   //���ź��������޶��źŵ�
    {
        if(semp->lamp_counter != 0) //�źŵ�������Ϊ0
            semp->lamp_counter--;   //Ϩ��һյ�źŵ�
        lamp = true;
    }else                           //���ź������źŵ���������
    {
        if(semp->lamp_counter > 0)  //�е������źŵ�
        {
            lamp = true;            //�����Դ����
            semp->lamp_counter--;   //Ϩ��һյ�źŵ�
        }else                       //û�е������źŵ�
            lamp = false;           //�����Դ������
    }

    if(lamp == true)
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return true;    //ȡ���źŵƷ���
    }
    else if(!sch || (timeout == 0))//��ֹ����ʱ,��������.��timeoutΪ0
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return false;   //û��ȡ���źŵƷ���
    }

    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->next = NULL;

    g_ptEventRunning->sync_head = &semp->semp_sync;
    if(semp->semp_sync == NULL)
    {//ͬ�����п�,running�¼��Գ�˫��ѭ������
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
        semp->semp_sync = g_ptEventRunning;
    }else
    {
        event = semp->semp_sync;
        if(semp->sync_order == CN_SEMP_BLOCK_PRIO)   //ͬ�����а����ȼ�����
        {
            do
            { //�ҵ�һ�����ȼ��������¼����¼�.
                if(event->prio <= g_ptEventRunning->prio)
                    event = event->multi_next;
                else
                    break;
            }while(event != semp->semp_sync);
            g_ptEventRunning->multi_next = event;
            g_ptEventRunning->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = g_ptEventRunning;
            event->multi_previous = g_ptEventRunning;
            if(semp->semp_sync->prio > g_ptEventRunning->prio)
                semp->semp_sync = semp->semp_sync->multi_previous;
        }else                               //���Ⱥ�˳�����¼�ֱ�����ڶ�β
        {
            g_ptEventRunning->multi_next = event;
            g_ptEventRunning->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = g_ptEventRunning;
            event->multi_previous = g_ptEventRunning;
        }
    }
    if(timeout != CN_TIMEOUT_FOREVER)
    {
        //�¼�״̬��Ϊ�ȴ��ź��� +  ��ʱ
        g_ptEventRunning->event_status = CN_STS_WAIT_SEMP + CN_STS_SYNC_TIMEOUT;
        ___Djy_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_SEMP;  //�¼�״̬��Ϊ�ȴ��ź���
    }
    Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ��Ŀ���¼���ͬ��������ȡ���¼���
     //��ʱ����ͬ�����¼��϶���û����ɡ�
        return false;
    }else
    {//˵���ǵõ��ź�������
        return true;
    }
}

//----ɾ���ں��ź���-----------------------------------------------------------
//���ܣ�ɾ��һ���ź�������semp_create_s������Ӧ
//������semp����ɾ�����ź���
//���أ���
//-----------------------------------------------------------------------------
bool_t Lock_SempDelete_s(struct tagSemaphoreLCB *semp)
{
    bool_t result;
    if(semp == NULL)    //��������
        return false;
    Int_SaveAsynSignal();
    if(semp->semp_sync == NULL)     //û���¼��ڵȴ��źŵ�
    {
        Rsc_DelNode(&semp->node);  //ɾ���ź������
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----ɾ��һ���ź���-----------------------------------------------------------
//���ܣ�ɾ��һ���ź���
//������semp����ɾ�����ź���
//���أ���
//-----------------------------------------------------------------------------
bool_t Lock_SempDelete(struct tagSemaphoreLCB *semp)
{
    bool_t result;
    if(semp == NULL)                //��������
        return false;
    Int_SaveAsynSignal();
    if(semp->semp_sync == NULL)     //û���¼��ڵȴ��źŵ�
    {
        Rsc_DelNode(&semp->node);  //ɾ���ź������
        Mb_Free(g_ptLockPool,semp); //�ͷ��ڴ�
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----��ѯ�ź�������-----------------------------------------------------------
//���ܣ���ѯһ���ź��������źŵƵ�����
//������semp������ѯ���ź���
//���أ��źŵ�����
//-----------------------------------------------------------------------------
u32 Lock_SempQueryCapacital(struct tagSemaphoreLCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > CN_CPU_BITS)
        atom_low_t atom_low;
        atom_low = Int_LowAtomStart();
#endif
        result = semp->lamps_limit;     //������ʹ�õ��źŵ�����
#if (32 > CN_CPU_BITS)
        Int_LowAtomEnd();
#endif
    }
    return result;
}

//----��ѯ�����źŵ�����-------------------------------------------------------
//���ܣ���ѯһ���ź��������źŵƵ�����
//������semp������ѯ���ź���
//���أ��źŵ�����
//-----------------------------------------------------------------------------
u32 Lock_SempQueryFree(struct tagSemaphoreLCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > CN_CPU_BITS)
        atom_low_t atom_low;
        atom_low = Int_LowAtomStart();
#endif
        result = semp->lamp_counter;     //������ʹ�õ��źŵ�����
#if (32 > CN_CPU_BITS)
        Int_LowAtomEnd();
#endif
    }
    return result;
}

//----��ѯ����-----------------------------------------------------------
//���ܣ���ѯ���ź����Ƿ����¼�������
//������mutex������ѯ�Ļ�����
//���أ�true = �У�false = û��
//-----------------------------------------------------------------------------
bool_t Lock_SempCheckBlock(struct tagSemaphoreLCB *Semp)
{
    if(Semp == NULL)    //��������
        return false;
    if(NULL != Semp->semp_sync)
        return true;
    else
        return false;
}

//----����ͬ����������ʽ-----------------------------------------------------
//����: ���õȴ��ź������¼�������ʽ������Ϊ�����������ʱ�����򣬻��߰�������
//      ���¼������ȼ������ź�����������ʱ��Ĭ���ǰ����ȼ�����ġ�
//      ������ֻӰ����ñ������������е��¼���ԭ���ڶ����е��¼�����Ӱ�졣
//����: semp: �����õ��ź���
//      order: CN_SEMP_BLOCK_PRIO=���ȼ��Ŷӣ�sort_time=�Ⱥ�˳���Ŷ�
//����: ��
//-----------------------------------------------------------------------------
void Lock_SempSetSyncSort(struct tagSemaphoreLCB *semp,u32 order)
{
    if(NULL == semp)
        return;
    semp->sync_order = order;
}
//-----------------------�����ǻ�����------------------------------------------

//----����һ���ź���-----------------------------------------------------------
//���ܣ�����һ����������
//������name�������������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½����Ļ�����ָ��
//-----------------------------------------------------------------------------
struct tagMutexLCB *Lock_MutexCreate(char *name)
{
    struct tagMutexLCB *mutex;
    mutex = Mb_Malloc(g_ptLockPool,0);
    if(mutex == NULL)
        return NULL;
    mutex->enable = 0;   //�������ĳ�ʼ״̬�ǿ��õ�,�������ָ��onwer
    mutex->mutex_sync = NULL;
    mutex->prio_bak = CN_PRIO_INVALID;
    mutex->owner = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    Rsc_AddSon(pg_mutex_rsc,&mutex->node,
                        sizeof(struct tagMutexLCB),RSC_MUTEX,name);
    return mutex;
}

//----����������---------------------------------------------------------------
//���ܣ�����һ�������������������ṩ���������ƿ飬�߿ɿ��Ե�Ӧ���У���Ӧ��ʹ�ö�
//      ̬����ķ�ʽ����̬������ɿ���Ȼ���ָ�봫�ݹ������ں���ʹ�õĻ���������
//      ��ʹ�þ�̬����ġ�Ӧ�ó�������cfg_locks_limitʱ��ֻ��Ҫ�����Լ��������
//      �����ˡ�
//������mutex,������ָ��
//      name�������������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½����Ļ�����ָ��
//-----------------------------------------------------------------------------
struct tagMutexLCB *Lock_MutexCreate_s( struct tagMutexLCB *mutex,char *name)
{
    if(mutex == NULL)
        return NULL;
    mutex->enable = 0;   //�������ĳ�ʼ״̬�ǿ��õ�,�������ָ��onwer
    mutex->mutex_sync = NULL;
    mutex->prio_bak = CN_PRIO_INVALID;
    mutex->owner = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    Rsc_AddSon(pg_mutex_rsc,&mutex->node,
                        sizeof(struct tagMutexLCB),RSC_MUTEX,name);
    return mutex;
}

//----�ͷ�һ��������-----------------------------------------------------------
//���ܣ��ͷŻ�������ֻ�л�������ӵ���߲����ͷŻ�������
//������mutex,������ָ��
//���أ���
//-----------------------------------------------------------------------------
void Lock_MutexPost(struct tagMutexLCB *mutex)
{
    struct  tagEventECB *event;

    if(mutex == NULL)
        return;
    if(Int_GetRunLevel() >0)      //�첽�ź����ͷŻ�����
    {
        if(mutex->enable == -1)
        {
            mutex->enable = 0;
        }
        return;
    }
    else
    {
        if(mutex->enable == -1)
        {
            return;                        //����Ӧ�ò����ߵ�����ݴ�һ��
        }
    }
    if(mutex->owner != g_ptEventRunning)    //������ֻ����ӵ�����ͷ�
        return;
    Int_SaveAsynSignal();
    if(mutex->enable > 0)
        mutex->enable--;
    else
        event = NULL;
    if(mutex->enable == 0)
    {
        if(mutex->mutex_sync == NULL)     //�ȴ����пգ����û�����Ϊ����״̬
        {
            mutex->owner = NULL;
        } else      //���¼��ڵȴ����ѻ�����ֱ��ת�������������ȼ���ߵ��¼�
        {
            event = mutex->mutex_sync;
            event->sync_head = NULL;
            mutex->owner = event;
            mutex->enable = 1;
            if(event->multi_next == event)
            {//������ֻ��һ���¼�
                mutex->mutex_sync = NULL;
            }else
            {//�������ж���¼���ȡ������ͷ�����¼�
                mutex->mutex_sync = event->multi_next;
                event->multi_next->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = event->multi_next;
                event->multi_next = NULL;
                event->multi_previous = NULL;
            }
            if(event->event_status & CN_STS_SYNC_TIMEOUT)
                __Djy_ResumeDelay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
            event->event_status = CN_STS_EVENT_READY;
            event->wakeup_from = CN_STS_WAIT_MUTEX;
            __Djy_EventReady(event);
            if(mutex->prio_bak != CN_PRIO_INVALID)  //�û��������������ȼ��̳�
            {
                //����������ȼ��̳к��¼����Լ��ı������ȼ������������������
                //��Ϊ�����ȼ����û��Լ�����Ϊ����ʱOS������������Ϊ��ָ����ȼ�
                //��һ�������������û�ǡ�øĵ����̳е����ȼ�����ʱ�޷�ʶ��
                if(g_ptEventRunning->prio == event->prio)
                {
                    __Djy_CutReadyEvent(g_ptEventRunning);  //ȡ��running�¼�
                    g_ptEventRunning->prio = mutex->prio_bak;   //�ָ����ȼ�
                     //���°�running�����������(��λ��)
                    __Djy_EventReady(g_ptEventRunning);
                }
                mutex->prio_bak = CN_PRIO_INVALID;
            }
        }
    }
    Int_RestoreAsynSignal();
}

//----���󻥳���-----------------------------------------------------------
//���ܣ����󻥳��������ɹ���ӵ�иû�������������owner��Ա�����������ʺ����¼�֮
//      ����¼����첽�ź�֮��ͬ����ֻ�д���owner��Ա������ʵ�����ȼ��̳С�
//      ������첽�ź������󻥳������򲻻������������ƣ�Ҳ���������ȼ��̳С�
//������mutex,������ָ��
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�true=ȡ���źŷ��ػ���Դ�����źŵƱ���(mutex == NULL)��mutex==NULLʱ����
//      true��������ģ������ϣ�������ڲ����Ƿ����źŵƱ���������һ�£���semp
//      ��ΪNULL�����ǵ�ѡ�񣬱�����Ҫ����һ�������źŵƱ������豸���ɰѸ��豸
//      struct pan_device��semp��Ա��ΪNULL��
//      false=û��ȡ���ź�(��ʱ���ػ�����ԭ��)
//-----------------------------------------------------------------------------
bool_t Lock_MutexPend(struct tagMutexLCB *mutex,u32 timeout)
{
    struct  tagEventECB *pl_ecb;
    bool_t lamp,sch;

    if(mutex == NULL)
    {
        if(Int_GetRunLevel() == 0)
            g_ptEventRunning->error_no = EN_LOCK_PARA_ERROR;
        return false;
    }
    //�첽�ź������󻥳�����������������
    if(Int_GetRunLevel() >0)
    {
        if(mutex->enable == 0)
        {
            mutex->enable = -1;
            return true;
        }
        else
        {
            g_ptEventRunning->error_no = EN_LOCK_BLOCK_IN_INT;
            return false;
        }
    }
    else
    {
        if(mutex->enable == -1)
        {
            return false;                  //����Ӧ�ò����ߵ�����ݴ�һ��
        }
    }
    if(g_ptEventRunning == mutex->owner)
    {
        mutex->enable++;
        return true;
    }
    sch = Djy_QuerySch();
    Int_SaveAsynSignal();
    if(mutex->enable == 0)   //�źŵƿ���
    {
        lamp = true;
        mutex->enable = 1;
        mutex->prio_bak = CN_PRIO_INVALID;
        mutex->owner = g_ptEventRunning;
    }else                       //�źŵƲ�����
    {
        lamp = false;
    }

    if(lamp == true)
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return true;    //ȡ�û���������
    }
    else if(!sch || (timeout == 0))//��ֹ����ʱ,��������.��timeout=0
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return false;   //û��ȡ�û���������
    }

    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->next = NULL;

    g_ptEventRunning->sync_head = &mutex->mutex_sync;
    if(mutex->mutex_sync == NULL)
    {//ͬ�����п�,running�¼��Գ�˫��ѭ������
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
        mutex->mutex_sync = g_ptEventRunning;
    }else
    {//ͬ�����зǿ�,�����ȼ�����
        pl_ecb = mutex->mutex_sync;
        do
        { //�ҵ�һ�����ȼ��������¼����¼�.
            if(pl_ecb->prio <= g_ptEventRunning->prio)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != mutex->mutex_sync);
        g_ptEventRunning->multi_next = pl_ecb;
        g_ptEventRunning->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->multi_previous = g_ptEventRunning;
        if(mutex->mutex_sync->prio > g_ptEventRunning->prio)
            mutex->mutex_sync = mutex->mutex_sync->multi_previous;
    }
    if(timeout != CN_TIMEOUT_FOREVER)
    {
        //�¼�״̬��Ϊ�ȴ��ź��� + ��ʱ
        g_ptEventRunning->event_status = CN_STS_WAIT_MUTEX +CN_STS_SYNC_TIMEOUT;
        ___Djy_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_MUTEX;  //�¼�״̬��Ϊ�ȴ��ź���
    }
    //���濴���Ƿ�Ҫ�����ȼ��̳�
    pl_ecb = mutex->owner;
    if(pl_ecb->prio > g_ptEventRunning->prio) //��Ҫ�̳����ȼ�
    {//1�����ھ���̬��2������ĳ������̬��
        if(pl_ecb->event_status == CN_STS_EVENT_READY)//ռ�û��������¼��ھ���̬
        {
            __Djy_CutReadyEvent(pl_ecb);
            //prio_bak�����¼���ԭʼ���ȼ�����ֹ���ֶ�μ̳��б��޸�Ϊ�м����ȼ�
            if(mutex->prio_bak == CN_PRIO_INVALID)
                mutex->prio_bak = pl_ecb->prio;
            pl_ecb->prio = g_ptEventRunning->prio;
            __Djy_EventReady(pl_ecb);
        }else       //ռ�û��������¼�����ĳ������̬���ݲ�����
        {
        }
    }
    Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ��Ŀ���¼���ͬ��������ȡ���¼���
     //��ʱ����ͬ�����¼��϶���û����ɡ�
        return false;
    }else
    {//˵���ǵõ�����������
        return true;
    }
}

//----ɾ��������-----------------------------------------------------------
//���ܣ�ɾ��һ���ź�������mutex_create_s������Ӧ
//������mutex����ɾ���Ļ�����
//���أ�true=�ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t Lock_MutexDelete_s(struct tagMutexLCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //��������
        return false;
    Int_SaveAsynSignal();
    if(mutex->mutex_sync == NULL)     //û���¼��ڵȴ�������
    {
        Rsc_DelNode(&mutex->node);  //ɾ�����������
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----ɾ��һ��������-----------------------------------------------------------
//���ܣ�ɾ��һ��������
//������mutex����ɾ���Ļ�����
//���أ���
//-----------------------------------------------------------------------------
bool_t Lock_MutexDelete(struct tagMutexLCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //��������
        return false;
    Int_SaveAsynSignal();
    if(mutex->mutex_sync == NULL)     //û���¼��ڵȴ�������
    {
        Rsc_DelNode(&mutex->node);  //ɾ�����������
        Mb_Free(g_ptLockPool,mutex); //�ͷ��ڴ�
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----��ѯһ��������-----------------------------------------------------------
//���ܣ���ѯ�û������Ƿ����
//������mutex������ѯ�Ļ�����
//���أ�true = ���ã�false = ������
//-----------------------------------------------------------------------------
bool_t Lock_MutexQuery(struct tagMutexLCB *mutex)
{
    if(mutex == NULL)    //��������
        return false;
    return ! mutex->enable;
}

//----��ѯ����-----------------------------------------------------------
//���ܣ���ѯ�û������Ƿ����¼�������
//������mutex������ѯ�Ļ�����
//���أ�true = �У�false = û��
//-----------------------------------------------------------------------------
bool_t Lock_MutexCheckBlock(struct tagMutexLCB *mutex)
{
    if(mutex == NULL)    //��������
        return false;
    if(NULL != mutex->mutex_sync)
        return true;
    else
        return false;
}

//----��ѯһ��������-----------------------------------------------------------
//���ܣ���ѯ�û�������ӵ����
//������mutex������ѯ�Ļ�����
//���أ�ӵ���ߵ��¼�ID
//-----------------------------------------------------------------------------
u16 Lock_MutexGetOwner(struct tagMutexLCB *mutex)
{
    if(mutex == NULL)    //��������
        return CN_EVENT_ID_INVALID;
    if( NULL != mutex->owner)
        return mutex->owner->event_id;
    else
        return CN_EVENT_ID_INVALID;
}

void __Lock_ShowLock(void)
{
    struct tagRscNode *current_node,*lock;
    struct tagEventECB *pl_ecb;
    current_node = Rsc_SearchTree("semaphore");
    lock = current_node;
    printf("�ź��� semaphore �б�\r\n");
    printf("����  �ź�������  �����ź���  �����¼�  ����\r\n");
    while(1)
    {
        current_node = Rsc_TraveScion(lock,current_node);
        if(current_node == NULL)
        {
            break;
        }
        if(((struct tagSemaphoreLCB *)current_node)->sync_order
                                        == CN_SEMP_BLOCK_PRIO)
        {
            printf("prio  ");
        }
        else
        {
            printf("fifo  ");
        }
        printf("%8d    %8d    ",
            ((struct tagSemaphoreLCB *)current_node)->lamps_limit,
            ((struct tagSemaphoreLCB *)current_node)->lamp_counter);
        pl_ecb = ((struct tagSemaphoreLCB *)current_node)->semp_sync;
        if(pl_ecb)
            printf("%05d     ",pl_ecb->event_id);
        else
            printf("��        ");
        if(current_node->name != NULL)
        {
            printf("%s\r\n", current_node->name);
        }else
        {
            printf("�����ź���\r\n");
        }
    }

    current_node = Rsc_SearchTree("mutex");
    lock = current_node;
    printf("\n\r������ mutex �б�\r\n");
    printf("״̬  ӵ����  ԭ���ȼ�  �����ȼ�  �����¼�  ����\r\n");
    while(1)
    {
        current_node = Rsc_TraveScion(lock,current_node);
        if(current_node == NULL)
        {
            break;
        }
        if(((struct tagMutexLCB *)current_node)->enable == 0)
        {
            printf("����                              ");
        }
        else
        {
            printf("ռ��  ");
            pl_ecb = ((struct tagMutexLCB *)current_node)->owner;
            if(((struct tagMutexLCB *)current_node)->prio_bak
                                    != CN_PRIO_INVALID)
            {
                printf("%05d   %03d       ",pl_ecb->event_id,
                        ((struct tagMutexLCB *)current_node)->prio_bak);
                printf("%03d       ",pl_ecb->prio);
            }
            else
            {
                printf("%05d   δ�������ȼ��̳�    ",pl_ecb->event_id);
            }
        }

        pl_ecb = ((struct tagMutexLCB *)current_node)->mutex_sync;
        if(pl_ecb)
            printf("%05d     ",pl_ecb->event_id);
        else
            printf("��        ");
        if(current_node->name != NULL)
        {
            printf("%s\r\n", current_node->name);
        }else
        {
            printf("����������\r\n");
        }
    }
    printf("\n\r�û����������ƿ�����:%d��������: %d \r\n",
            gc_u32CfgLockLimit,Mb_QueryFree(g_ptLockPool));
}
