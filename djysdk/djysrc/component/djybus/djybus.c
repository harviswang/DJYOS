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
// �ļ���     ��djybus.c
// ģ������: djybusģ����Ҫ��DJYOS���߹������������߷��࣬�γɽڵ�ֱ�ҵ�������
//           "DjyBus"���棬ÿ�����Ϳ����ж������ߣ���ÿ�����߿���Ҳ�ж���������γ�
//           ���������ߡ��������͡���DjyBus������Ҷ����������������
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 17/06.2014
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "rsc.h"

static struct tagRscNode tg_djybus_root;                //���徲̬����DjyBus�ĸ����
// =============================================================================
// ���ܣ���������ʼ��DjyBus���߸��ڵ㣬�����������ͽڵ�ĸ����
// ������para,��ʵ������
// ���أ�true=�ɹ�, false=ʧ��.
// =============================================================================
bool_t ModuleInstall_DjyBus (ptu32_t Para)
{
    //����Դ�����н���һ������㣬���н��������߽�㶼���ڸý���ϡ�
    if(Rsc_AddTree(&tg_djybus_root,sizeof(struct  tagRscNode),RSC_RSCNODE,"DjyBus"))
    {
        printk("DjyBus Install Successed !\r\n");
        return true;
    }
    return false;
}

// =============================================================================
// ���ܣ���DjyBus����������������µ��������ͽ�㣬�����������ͽ����DjyBus���ӽ��,
//       ʹ���ڴ�ط���ķ�ʽ�����ڴ�
// ������NewBusTypeName,������������
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct tagRscNode * DjyBus_BusTypeAdd (char* NewBusTypeName)
{
    struct tagRscNode * NewBusType;

    NewBusType = (struct tagRscNode *)M_Malloc(sizeof(struct tagRscNode),0);
    if(NewBusType == NULL)
        return NULL;

    //�����ظ�����ͬ������������
    if(NULL != Rsc_SearchSon(&tg_djybus_root,NewBusTypeName))
        return NULL;

    Rsc_AddSon(&tg_djybus_root,NewBusType,
                sizeof(struct tagRscNode),RSC_RSCNODE,NewBusTypeName);

    return NewBusType;
}

// =============================================================================
// ���ܣ���DjyBus����������������µ��������ͽ�㣬�����������ͽ����DjyBus���ӽ��,
//       �ú�����r����ʾ�ɵ������ṩ�ڴ棬��NewBusType���û�����
// ������NewBusTypeName,����������������
//       NewBusType,�����������ͽ�㣬�üǶ���
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct tagRscNode * DjyBus_BusTypeAdd_s(char* NewBusTypeName,
                                      struct tagRscNode * NewBusType)
{
    if((NewBusType == NULL) && (NewBusTypeName == NULL))
        return NULL;

    //�����ظ�����ͬ������������
    if(NULL != Rsc_SearchSon(&tg_djybus_root,NewBusTypeName))
        return NULL;

    Rsc_AddSon(&tg_djybus_root,NewBusType,
                sizeof(struct tagRscNode),RSC_RSCNODE,NewBusTypeName);

    return NewBusType;
}

// =============================================================================
// ���ܣ�ɾ��DjyBus���߸�����ϵ�һ���������ͽ�㣬���ͷŶ�̬������ڴ�飬,�����ͽ�
//       �㲻�����ӽ��
// ������DelBusType,��ɾ�����������ͽ��
// ���أ�TRUE,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t DjyBus_BusTypeDelete(struct tagRscNode * DelBusType)
{
    bool_t result;
    if(DelBusType == NULL)
        return false;

    if(NULL == Rsc_DelNode(DelBusType))
    {
        result = false;
    }
    else
    {
        free(DelBusType);
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�ɾ��DjyBus���߸�����ϵ�һ���������ͽ��,�����ͽ�㲻�����ӽ��
// ������DelBusType,��ɾ�����������ͽ��
// ���أ�TRUE,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t DjyBus_BusTypeDelete_s(struct tagRscNode * DelBusType)
{
    bool_t result;
    if(DelBusType == NULL)
        return false;

    if(NULL == Rsc_DelNode(DelBusType))
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ����������������Ʋ����������ͽڵ㣬���ز��ҵ��Ľ��ָ��
// ������BusTypeName,�����ҵ��������ͽ������
// ���أ����ָ�룬NULLʱ����ʧ��
// =============================================================================
struct tagRscNode * DjyBus_BusTypeFind(char * BusTypeName)
{
    return Rsc_SearchSon(&tg_djybus_root,BusTypeName);
}

