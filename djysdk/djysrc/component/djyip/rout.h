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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: Э��ջ��·�ɶ���
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 11:35:41 AM/Jul 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef ROUT_H_
#define ROUT_H_

#include "stdint.h"
#include "netdev.h"
//�����������ַ
typedef struct _tagHostIpv4Addr
{
	u32 ip;         //����IP
	u32 ipmsk;      //IP����
	u32 gateway;    //����
}tagHostIpv4Addr;

//ROUTEITEM���״̬
enum _EN_ROUTITEM_STAT
{
	EN_ROUTITEM_FREE = 0,
	EN_ROUTITEM_STABLE,
	EN_ROUTITEM_PENDING,
};
//ROUTEITEM�������
enum _EN_ROUTITEM_PROPERTY
{
	EN_ROUTITEM_DYNAMIC = 0,
	EN_ROUTITEM_STATIC ,
};
//Route����
typedef struct _tagNetRoutItem
{
	struct _tagNetRoutItem  *pre;
	struct _tagNetRoutItem  *next;
	u32                     ip;
	u8                      mac[6];
	u8                      stat:4;
	u8                      type:4;
	u8                      life;
}tagNetRoutItem;
//Route��
typedef struct _tagRoutTab
{
	struct _tagRoutTab   *nxt;        //used for the lst
	tagHostIpv4Addr         netaddr;     //�����ַ
	tagNetDev               *netdev;      //ͨ���豸
	struct tagMutexLCB      *itemsync;    //ITEMͬ����Ϣ
	tagNetRoutItem          *itemlst;     //item queue
}tagRoutTab;

//����Route�Ľ��
typedef struct _tagRoutLink
{
	u8                mac[6];    //�����ҵ�Զ��IP��Ӧ��MAC��ַ
	u32               ip;        //��Զ��ͨ�ŵı���IP
	tagNetDev         *dev;       //�����豸
}tagRoutLink;

bool_t  Rout_Resolve(u32 ipdst,tagRoutLink *outpara);
void    Rout_ItemUpdate(u32 ip, u8 *mac);

bool_t  Rout_IsLocalIp(u32 ip);
bool_t  Rout_IsLocalDst(u32 ip);

tagRoutTab * Rout_AddRout(tagNetDev *netdev,tagHostIpv4Addr  *netaddr);
#endif /* ROUT_H_ */

