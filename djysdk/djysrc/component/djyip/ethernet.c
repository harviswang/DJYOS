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
// ģ������: ��̫��Э���ʵ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 4:24:26 PM/Jul 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "endian.h"
#include "string.h"

#include "os.h"

#include "ethernet.h"

#pragma pack(1)
typedef struct _tagEthernetHead 
{
	u8 macdst[6];
	u8 macsrc[6];
	u16 type;
}tagEthernetHead;
#pragma pack()
#define CN_ETHERNET_HEADLEN  14
// =============================================================================
// �������ܣ�Ethernet_Send
//          ��·�㷢�ͺ���
// ��������� netdev,������ʹ�õ�����(����)
//           type,�����Э�����ͣ����룩
//           pkg,�����͵���������ݰ������룩
//           netdevtask,��Ҫ������ɵĹ���
//           macsrc��macdst����netdev�����Կ��Ƿ���Ҫ����������豸����ҪMAC
//           para,����netdev��������ȷ��
// �����������
// ����ֵ  ��
// ˵��    ���ú�������·��������ã��÷��Ͳ������������ڴ���亯������Ϊ��·Э��ͷ����
//        �ǳ�С����˺�������Э��ͷ��ͬһ��PKG�У������������ʱ��������ռ�
// =============================================================================
bool_t Ethernet_Send(tagNetDev *netdev,tagNetPkg *pkg,u32 netdevtask,\
		           u8 *macdst, u8 *macsrc ,u16 type, ptu32_t para)
{
	bool_t result;
	tagEthernetHead  *head;
	
	result = false;
	if((NULL != macdst) && (NULL != macsrc))
	{
       //chk whether the PKG has enough space for the ethnet
		if(pkg->offset >= CN_ETHERNET_HEADLEN)
		{
			pkg->offset -= CN_ETHERNET_HEADLEN;
			pkg->datalen += CN_ETHERNET_HEADLEN;
			head = (tagEthernetHead  *)(pkg->buf + pkg->offset);
			memcpy(head->macdst, macdst, 6);
			memcpy(head->macsrc, macsrc, 6);
			head->type = htons(type);			
			//�����豸�Ķ˿ڷ���
			result = NetDev_DevSend(netdev, pkg, netdevtask);
		}
	}
	return result;
}

u32 sgEthBroadFrames = 0;
u32 sgEthSpeciFrames = 0;
u8  sgEthBroadMac[6] = {0xFF,0xff,0xff,0xff,0xff,0xff};
// =============================================================================
// �������ܣ�Ethernet_Process
//          ��������һ֡���ݵ�Э��ջ
// ���������pkg,��·�����ݰ�
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܣ��޶�Ӧ������Э����ߴ���ʧ�ܣ�
// ˵��    ����һ֡���ݴ��ݵ���·�����̫��
// =============================================================================
bool_t Ethernet_Process(tagNetPkg *pkg, struct tagLinkProtoMsg *Msg)
{
	tagEthernetHead *head;

	head = (tagEthernetHead *)(pkg->buf +pkg->offset);	
	pkg->datalen -= sizeof(tagEthernetHead);
	pkg->offset +=sizeof(tagEthernetHead);
	Msg->type = ntohs(head->type);
	Msg->macdst = head->macdst;
	Msg->macsrc = head->macsrc;
	
	return true;
}


