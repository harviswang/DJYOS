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
//����ģ��: multiplexģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:ʵ�ֶ�·���õĹ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-06-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "rsc.h"
#include "pool.h"
#include "lock.h"
#include "multiplex.h"
#define CN_MULTIPLEX_ACTIVED   (1<<31)  //MultiplexSets�Ƿ��Ѿ�������ֻ��type����
//ΪCN_MULTIPLEX_OBJECT_AND��Ч����ORģʽ
//�£���ActiveQ�жϼ���

static struct tagMemCellPool *g_ptMultiplexSetsPool;
static struct tagMemCellPool *g_ptMultiplexObjectPool;
static struct tagMutexLCB MultiplexMutex;

//��Object��*SrcList����ȡ�����ŵ�*TarList����ͷ��ȥ
static void __ChangeList(struct tagMultiplexObjectCB **SrcList,
    struct tagMultiplexObjectCB **TarList,
    struct tagMultiplexObjectCB *Object)
{
    Object->NextObject->PreObject = Object->PreObject;
    Object->PreObject->NextObject = Object->NextObject;
    if (*SrcList == Object) {
        if (Object->NextObject == Object) {
            *SrcList = NULL;
        } else {
            *SrcList = Object->NextObject;
        }
    }
    //��Object�ŵ�Active������
    if (*TarList == NULL) {
        *TarList = Object;
        Object->NextObject = Object;
        Object->PreObject = Object;
    } else {
        Object->PreObject = (*TarList)->PreObject;
        Object->NextObject = *TarList;
        (*TarList)->PreObject->NextObject = Object;
        (*TarList)->PreObject = Object;

        *TarList = Object;
    }
}

//�ж�һ��Object�Ƿ��Ѿ�����
bool_t __ObjectIsActived(u32 Pending, u32 Sensing, u32 Mode)
{
    if (Mode == CN_MULTIPLEX_SENSINGBIT_OR) {
        if ((Pending & Sensing) != 0)
            return true;
        else
            return false;
    } else {
        if ((Pending & Sensing) == Sensing)
            return true;
        else
            return false;
    }
}

//MultiplexSets���Ѿ���Object�������£��ж����Ƿ񴥷�
bool_t __SetsIsActived(struct tagMultiplexSetsCB *Sets)
{
    if (Sets->SetsActived == true) {
        if (Sets->ActiveQ != NULL)
            return true;
        else
            return false;
    } else {
        if ((Sets->Actived >= Sets->ActiveLevel)
            || (Sets->Actived >= Sets->ObjectSum)) {
            return true;
        } else
            return false;
    }
}


//----ģ���ʼ��---------------------------------------------------------------
//����: ��ʼ��Multiplexģ�顣��������������ʼ���ڴ�ء�
//������para����Ч��
//���أ�0=�ɹ�����0=ʧ�ܡ�
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Multiplex(ptu32_t para)
{
    Lock_MutexCreate_s(&MultiplexMutex, "Multiplex control block");
    //����MultiplexSets���ƿ��ڴ�أ��ڴ�صĳ�ʼ״̬�ǿյġ�
    g_ptMultiplexSetsPool = Mb_CreatePool(NULL, 0, sizeof(struct tagMultiplexSetsCB),
        5, CN_MULTIPLEX_MAX_SETS, "Multiplex control block");
    if (g_ptMultiplexSetsPool == NULL)
        return -1;
    //����Multiplex������ƿ��ڴ�أ��ڴ�صĳ�ʼ״̬�ǿյġ�
    g_ptMultiplexObjectPool = Mb_CreatePool(NULL, 0, sizeof(struct tagMultiplexObjectCB),
        5, CN_MULTIPLEX_MAX_OBJECTS, "Multiplex control block");
    if (g_ptMultiplexObjectPool == NULL) {
        Mb_DeletePool(g_ptMultiplexObjectPool);
        return -1;
    }
    return 0;
}

//----����MultiplexSets----------------------------------------------------------
//���ܣ�����һ��MultiplexSets�������趨���ԡ�
//������ActiveLevel, ����ˮƽ,�ü��ϱ������Ķ���ﵽ������,��������.
//���أ��´�����MultiplexSetsָ��
//----------------------------------------------------------------------------
struct tagMultiplexSetsCB* Multiplex_Creat(u32 ActiveLevel)
{
    struct tagMultiplexSetsCB *SetsQ;
    SetsQ = (struct tagMultiplexSetsCB *)Mb_Malloc(g_ptMultiplexSetsPool,
        CN_TIMEOUT_FOREVER);
    if (SetsQ != NULL) {
        SetsQ->ObjectQ = NULL;
        SetsQ->ActiveQ = NULL;
        SetsQ->ObjectSum = 0;
        SetsQ->ActiveLevel = ActiveLevel;
        SetsQ->Actived = 0;
        SetsQ->SetsActived = false;
        Lock_SempCreate_s(&SetsQ->Lock, 1, 0, CN_SEMP_BLOCK_FIFO, NULL);
        return SetsQ;
    } else
        return NULL;
}

//----��Ӷ���MultiplexSets--------------------------------------------------
//����: MultiplexSets�����һ�����������Object�ĳ�ʼ״̬���Ѿ�����������뵽
//      ActiveQ���У��������ObjectQ���С�
//����: Sets����������MultiplexSetsָ��
//      ObjectHead����������Object����ͷָ���ָ�룬*ObjectHead=NULL��ʾ�ö�����
//          δ�����κ�MultiplexSets����ˣ�*ObjectHead��ʼ��ֵӦ����NULL��Object
//          ���������MultiplexSets��ÿ����һ��MultiplexSets��������һ��
//          struct tagMultiplexObjectCB *���͵Ľ�㣬���н���NextSetsָ������
//          ��һ����������*ObjectHeadָ�������ͷ��*ObjectHead��Ҳ���������ⲿ
//          �޸ģ�����������Ԥ�ϡ�
//      ObjectStatus������ʱ��״̬��31bit��λԪ��,bit31��Ч
//      ObjectID����Multiplex�Ķ����ID��
//      SensingBit����������λ��־��31��bit����Ϊ1��ʾ����������bit��־����
//          bit31��ʾ�������ͣ�CN_SENSINGBIT_AND������CN_SENSINGBIT_OR
//����: true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t Multiplex_AddObject(struct tagMultiplexSetsCB *Sets,
    struct tagMultiplexObjectCB **ObjectHead,
    u32 ObjectStatus, ptu32_t ObjectID, u32 SensingBit)
{
    struct tagMultiplexObjectCB *temp;
    struct tagMultiplexObjectCB **TargetQ;
    bool_t repeat = false;
    u32 ActivedInc = 0;
    if (Sets == NULL)
        return false;
    ObjectStatus &= ~0x80000000;
    //�������¼����Object�Ƿ��Ѿ��������Ծ������뵽MultiplexSets���ĸ�������
    if (__ObjectIsActived(ObjectStatus,
            SensingBit & ~0x80000000,
            SensingBit & 0x80000000)) {
        TargetQ = &Sets->ActiveQ;
        ActivedInc = 1;
    } else
        TargetQ = &Sets->ObjectQ;
    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    temp = *ObjectHead;
    //ѭ�����һ��Object�Ƿ��ظ�����ͬһ��MultiplexSets
    //���ObjectHead=NULL,������ǲ��ظ������������ܹ���ȷ���С�
    while (temp != NULL) {
        if (temp->MySets != Sets)
            temp = temp->NextSets;
        else {
            repeat = true;
            break;
        }
    }
    Lock_MutexPost(&MultiplexMutex);

    if (repeat == false) {
        temp = Mb_Malloc(g_ptMultiplexObjectPool, CN_TIMEOUT_FOREVER);
        if (temp != NULL) {
            Sets->ObjectSum++;
            temp->SensingBit = SensingBit;
            temp->PendingBit = ObjectStatus;
            temp->ObjectID = ObjectID;
            Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
            temp->MySets = Sets;            //�趨��������MultiplexSets
                                            //ͬһ��MultiplexSets�����������NextObject����Щ��������������
            if (*TargetQ == NULL) {
                *TargetQ = temp;
                temp->NextObject = temp;
                temp->PreObject = temp;
            } else {
                //�¼���MultiplexSets�Ķ���������ͷ��
                temp->PreObject = (*TargetQ)->PreObject;
                temp->NextObject = *TargetQ;
                (*TargetQ)->PreObject->NextObject = temp;
                (*TargetQ)->PreObject = temp;

                (*TargetQ) = temp;
            }
            //ͬһ�����󱻶��MultiplexSets��������NextSets���ӡ�
            //NextSets�ǵ��������¶����������ͷ��
            temp->NextSets = *ObjectHead;
            *ObjectHead = temp;
            Lock_MutexPost(&MultiplexMutex);
            if (ActivedInc == 1) {
                Sets->Actived += ActivedInc;
                if ((Sets->Actived >= Sets->ActiveLevel)
                    || (Sets->Actived >= Sets->ObjectSum)) {
                    if (false == Sets->SetsActived) {
                        Sets->SetsActived = true;
                        Lock_SempPost(&Sets->Lock);
                    }
                }
            }
        } else
            return false;
    } else {
        //�ظ����룬�������κδ���
    }
    return true;
}

//----��MultiplexSetsɾ������-----------------------------------------------------
//����: MultiplexAdd���溯��
//����: Sets����������MultiplexSetsָ��
//      ObjectHead����������Object����ͷָ���ָ��
//����: true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t Multiplex_DelObject(struct tagMultiplexSetsCB *Sets,
    struct tagMultiplexObjectCB **ObjectHead)
{
    struct tagMultiplexObjectCB *Object,*following = NULL;
    if ((Sets == NULL) || (ObjectHead == NULL))
        return false;
    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    Object = *ObjectHead;
    while (Object != NULL) {       //���ұ�ɾ���Ķ�����ƿ�
        if (Object->MySets != Sets) {
            following = Object;
            Object = Object->NextSets;
        } else
            break;
    }

    if (Object != NULL) {
        //�����鱻ɾ����Object�Ƿ��Ѿ�������
        if (__ObjectIsActived(Object->PendingBit,
                Object->SensingBit & ~0x80000000,
                Object->SensingBit & 0x80000000)) {
            if (Sets->Actived != 0) {
                Sets->Actived--;
                if (Sets->Actived == 0)
                    Sets->SetsActived = false;
            }
        }

        Sets->ObjectSum--;
        if (Object->NextObject != Object) {    //�����в�ֹһ�����
            Object->NextObject->PreObject = Object->PreObject;
            Object->PreObject->NextObject = Object->NextObject;
            if (Object == Sets->ObjectQ) {
                Sets->ObjectQ = Object->NextObject;
            }
            if (Object == Sets->ActiveQ) {
                Sets->ActiveQ = Object->NextObject;
            }
        } else {                                //����ֻ��һ�����
            if (Object == Sets->ObjectQ) {
                Sets->ObjectQ = NULL;
            }
            if (Object == Sets->ActiveQ) {
                Sets->ActiveQ = NULL;
            }
        }
        if (following == NULL) {
            *ObjectHead = NULL;
        } else {
            following->NextObject = Object->NextObject;
        }
        Mb_Free(g_ptMultiplexObjectPool, Object);
    }
    Lock_MutexPost(&MultiplexMutex);
    return true;
}

//----Multiplexִ��------------------------------------------------------------
//���ܣ���MultiplexSets�еĶ���״̬�����仯�������ģ����ñ�������֪Multiplex
//      ģ�顣
//����: ObjectHead����������Object����ͷָ��
//      Status��Object�ĵ�ǰ״̬
//����: true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t Multiplex_Set(struct tagMultiplexObjectCB *ObjectHead, u32 Status)
{
    struct tagMultiplexObjectCB *Object;
    struct tagMultiplexSetsCB *Sets;
    u32 Sensing, Type;
    u32 OldPend;
    if (ObjectHead == NULL)
        return false;
    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    Object = ObjectHead;
    while (Object != NULL) {
        OldPend = Object->PendingBit;
        Sets = Object->MySets;
        Sensing = Object->SensingBit & ~0x80000000;
        Type = Object->SensingBit & 0x80000000;
        Object->PendingBit = Status & Sensing;          //����PendingBit
        if (__ObjectIsActived(OldPend, Sensing, Type)) {    //����ǰ��Object�Ѵ���
            if (!__ObjectIsActived(Object->PendingBit, Sensing, Type)) {
                //����Multiplex_Set���¶�����δ����
                //��Object��Sets->ActiveQ�����ó����ŵ�ObjectQ������
                __ChangeList(&(Sets->ActiveQ), &(Sets->ObjectQ), Object);
                if (Sets->Actived != 0)
                    Sets->Actived--;
                if (Sets->Actived == 0)
                    Sets->SetsActived = false;
            }
        } else {                                            //����ǰ��Objectδ����
            if (__ObjectIsActived(Object->PendingBit, Sensing, Type)) {
                //����Multiplex_Set���¶��󱻴���

                //��Object��Sets->ObjectQ�����ó����ŵ�ActiveQ������
                __ChangeList(&(Sets->ObjectQ), &(Sets->ActiveQ), Object);
                if (Sets->Actived < Sets->ObjectSum)
                    Sets->Actived++;
                //�첽����ģʽ�����ͷ��ź���
                if ((Sets->Actived >= Sets->ActiveLevel)
                    || (Sets->Actived >= Sets->ObjectSum)) {
                    if (false == Sets->SetsActived) {
                        Sets->SetsActived = true;
                        Lock_SempPost(&Sets->Lock);
                    }
                }
            }
        }
        Object = Object->NextSets;
    }
    Lock_MutexPost(&MultiplexMutex);
    return true;
}

//----�ȴ�MultiplexSets-----------------------------------------------------------
//����: ���ú�MultiplexSets��Ӧ�ó�����ñ������ȴ�MultiplexSets������������
//      Typeֵ���ȴ���ʽ����ѯ���첽�������֡�
//����: Sets����������MultiplexSetsָ��
//      Status�����ض���ǰ״̬��ָ�룬���Ӧ�ó��򲻹�����״̬�����Ը�NULL��
//      Timeout�������ȴ����ʱ�䣬uS��
//���أ����MultiplexSets���������򷵻�MultiplexSets��һ�������������ObjectID��
//      ���򷵻�-1.
//-----------------------------------------------------------------------------
ptu32_t Multiplex_Wait(struct tagMultiplexSetsCB *Sets, u32 *Status, u32 Timeout)
{
    struct tagMultiplexObjectCB *Object;
    ptu32_t result;
    if (Sets == NULL)
        return -1;
    if ((Sets->ActiveQ == NULL) && (Sets->ObjectQ == NULL))
        return -1;

    //�жϻ�ȴ�ֱ��MultiplexSets��������
    if (!__SetsIsActived(Sets)) {
        Lock_SempPend(&Sets->Lock, Timeout);
    }

    if (Sets->ActiveQ != NULL) {   //�����ȴ�������SetsActived�ǿռ���Ϊ������
        Sets->SetsActived = true;
        Object = Sets->ActiveQ;
        result = Object->ObjectID;
        if (Status != NULL)
            *Status = Object->PendingBit;
    } else
        result = -1;

    return result;
}

