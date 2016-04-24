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
// ģ������: ��·���ʵ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 10:08:24 AM/Jul 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"

#include "os.h"

#include "link.h"
#include "ethernet.h"
extern bool_t Loop_Send(tagNetDev *netdev,tagNetPkg *pkg,u32 netdevtask,\
        u8 *macdst, u8 *macsrc ,u16 type, ptu32_t para);
// =============================================================================
// �������ܣ�Link_Send
//          ��·�㷢�ͺ���
// ��������� netdev,������ʹ�õ�����(����)
//           type,�����Э�����ͣ����룩
//           pkg,�����͵���������ݰ������룩
//           netdevtask,��Ҫ������ɵĹ�����IP TCPУ������ɵȣ�
//           macdst����netdev�����Կ��Ƿ���Ҫ����������豸����ҪMAC
//           macsrc,����dev�Ѿ�ָ�������Բ�����ָ��Դmac
//           para,����netdev��������ȷ��
// �����������
// ����ֵ  ��
// ˵��    ���ú����������Э��ķ��ͺ�������
// =============================================================================
bool_t Link_Send(tagNetDev *netdev,tagNetPkg *pkg,u32 netdevtask,\
		           u8 *macdst,u16 type, ptu32_t para)
{
	bool_t result;
	u8     *macsrc;
	
	result = false;
	if((NULL != netdev)&&(NULL != pkg))
	{
		//�����������·���а�װ����
		switch (netdev->iftype)
		{
			case  EN_LINK_INTERFACE_ETHERNET:
				macsrc = NetDev_GetDevMac(netdev);
				result = Ethernet_Send(netdev,pkg,netdevtask,macdst,macsrc,type,para);
				break;
			case  EN_LINK_INTERFACE_SLIP:
				break;
			case  EN_LINK_INTERFACE_LOOP:
				macsrc = NetDev_GetDevMac(netdev);
				result = Loop_Send(netdev,pkg,netdevtask,macdst,macsrc,type,para);
				break;
			case  EN_LINK_INTERFACE_RAW:
				break;
			default:
				break;
		}
	}
	return result;
}

//��������������Ҫ��Ӧ�ô���ʱ����Ҫ���øú���ʵ�����ݵĽ��մ���,�ú�������������������
extern bool_t Loop_Process(tagNetPkg *pkg, struct tagLinkProtoMsg *Msg);
extern bool_t Ip_PostPkg(tagNetDev *netdev, tagNetPkg *pkglst);
extern bool_t Arp_PostPkg(tagNetDev *netdev,tagNetPkg *pkglst,u8 *macdst, u8 *macsrc);

// =============================================================================
// �������ܣ�Link_Process
//          ��·�����ݰ��������
// ��������� netdev,���յ��豸
//          pkg,���յ������ݰ�
// �����������
// ����ֵ  ��
// ˵��    �����������յ�����ʱ�����,ֻҪ���ݰ����ݸ���һ���ˣ���ô���Ǿ���������PKG
//         ��PKG���ͷŵ�������ȫ������һ��ȥ����
// =============================================================================
bool_t Link_Process(tagNetDev *netdev,tagNetPkg *pkg)
{	
	bool_t result;
	struct tagLinkProtoMsg Msg;
	
	result = false;
	if((NULL != netdev)&&(NULL != pkg))
	{
		//�����������·���н���
		switch (netdev->iftype)
		{
			case  EN_LINK_INTERFACE_ETHERNET:
				result = Ethernet_Process(pkg,&Msg);
				break;
			case  EN_LINK_INTERFACE_SLIP:
				break;
			case  EN_LINK_INTERFACE_LOOP:
				result = Loop_Process(pkg,&Msg);
				break;
			case  EN_LINK_INTERFACE_RAW:
				break;
			default:
				break;
		}
		if(result)
		{
			switch(Msg.type)
			{
				case EN_NET_PROTO_IP:
					result = Ip_PostPkg(netdev,pkg);
					break;
				case EN_NET_PROTO_ARP:
					result = Arp_PostPkg(netdev,pkg,Msg.macdst,Msg.macsrc);
					break;
				default:
					result = false;
					break;
			}
		}
	}
	return result;
}

// =============================================================================
// �������ܣ�Link_Init
//          ��·���ʼ��
// �����������
// ���������
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ���ò�ĳ�ʼ���ܼ򵥣����㲻�漰����Դ�ķ��䣬���ֻ�轫���е�����Э�鴦��
//         ��Դ��ʼ��Ϊ�ռ���
// =============================================================================
bool_t Link_Init(void)
{	
	return true;
}

