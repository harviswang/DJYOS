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
// ģ������: NetDevHal.c:
// ģ��汾: V1.00
// ������Ա: Zqf
// ����ʱ��: 9:45:16 AM/Sep 9, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "os.h"

#include "link.h"
#include "netdev.h"

static tagNetDev  *pgNetDevLstH =NULL;

// =============================================================================
// �������ܣ�NetDev_AddDev
//           ���һ�������豸
// ���������netdevpara,�����豸�����Բ���
// ���������errno,����ԭ��
// ����ֵ  ���������
// =============================================================================
tagNetDev  *NetDev_AddDev(tagNetDevPara *para)
{

    tagNetDev  *result;
    atom_low_t atom;

    result = M_Malloc(sizeof(tagNetDev),CN_TIMEOUT_FOREVER);
    if(NULL != result)
    {
        result->ifsend = para->ifsend;
        result->private = para->private;
        memcpy(result->mac, para->mac, 6);
        result->iftype = para->iftype;
        result->linklen= para->linklen;
        result->pkglen= para->pkglen;
        result->devfunc= para->devfunc;
        result->ifstate = EN_LINK_INTERFACE_STATE_OK;

        atom = Int_LowAtomStart();
        if(NULL == pgNetDevLstH)
        {
            result->nxt = NULL;
            result->pre = NULL;
            pgNetDevLstH = result;
        }
        else
        {
            result->nxt = pgNetDevLstH;
            pgNetDevLstH->pre = result;
            result->pre = NULL;
            pgNetDevLstH = result;
        }
        Int_LowAtomEnd(atom);
    }
    return result;
}

// =============================================================================
// �������ܣ�NetDev_DelDev ɾ��һ�������豸
// ���������netdev,��ɾ��������
// ���������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
bool_t NetDev_DelDev(tagNetDev *netdev) //��ʱ���ṩ
{
    bool_t   result = false;

    return result;
}



// =============================================================================
// �������ܣ�NetDev_GetDevPrivate
//          ����������յ�������Ϣ
// ���������netdev,ע��ɹ�������
// ���������
// ����ֵ  ��if para err or something unnormal then return zero,otherwise return the
//          private property that applied when registered
// ˵��    ��
// =============================================================================
ptu32_t NetDev_GetDevPrivate(tagNetDev *netdev)
{
    ptu32_t result = 0;

    if(NULL != netdev)
    {
        result= netdev->private;
    }

    return result;
}

// =============================================================================
// �������ܣ�NetDev_DevSend
//          �����豸����PKG��
// ���������netdev,ʹ�õ�����
//        pkglst,�����͵����ݰ�����
//        netdevtask,��Ҫ������ɵĹ���
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// ˵��    �������������ڻ��߼�����ע�����ߴ���ͣ��״̬ʱ������ʧ��
// =============================================================================
bool_t NetDev_DevSend(tagNetDev *netdev,tagNetPkg *pkglst,u32 netdevtask)
{
    bool_t result = false;

    if((NULL != netdev)&&(NULL != netdev->ifsend))
    {
        result = netdev->ifsend(netdev,pkglst,netdevtask);
    }

    return result;
}

// =============================================================================
// �������ܣ�NetDev_PostPkg,�����������øýӿڷ������ݰ���Э��ջ
// ���������netdev,ʹ�õ�����
//        pkglst,�����͵����ݰ�����
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// ˵��    �������������ڻ��߼�����ע�����ߴ���ͣ��״̬ʱ������ʧ��
// =============================================================================
bool_t NetDev_PostPkg(tagNetDev *netdev,tagNetPkg *pkglst)
{
    return Link_Process(netdev,pkglst);
}
// =============================================================================
// �������ܣ�NetDev_GetDevMac
//          ��ȡ�����豸��MAC��ַ
// ���������netdev,����ȡ������
// ���������
// ����ֵ  ��������MAC��ַ��NULL��ʾΪ��ȡ����
// ˵��    �������������ԣ��󲿷��Ǵ���Ψһ��MAC;��ȡ����һ������Ϊ��������
// =============================================================================
u8 *NetDev_GetDevMac(tagNetDev *netdev)
{
    u8 *result = NULL;

    if(NULL != netdev)
    {
        result = netdev->mac;
    }

    return result;
}

// =============================================================================
// �������ܣ�NetDev_GetDevProperty
//          ��ȡ�����豸�Ĺ�������
// ���������netdev,����ȡ������
// ���������
// ����ֵ  �������Ĺ�������
// ˵��    ��
// =============================================================================
u32 NetDev_GetDevFun(tagNetDev *netdev)
{
    u32 devfun = 0;

    if(NULL != netdev)
    {
        devfun = netdev->devfunc;
    }

    return devfun;
}

// =============================================================================
// �������ܣ�NetDev_Init
//          �����豸ģ���ʼ��
// �����������
// �����������
// ����ֵ  ��true��ʼ���ɹ� ��false ��ʼ��ʧ��
// ˵��    ��ʧ�ܵĿ�����̫С��Ψһ�Ŀ�����δ֪ԭ��
// =============================================================================
bool_t NetDev_Init(void)
{
    return true;
}
