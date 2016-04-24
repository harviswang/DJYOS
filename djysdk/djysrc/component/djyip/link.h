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
// ģ������: Э��ջ����·��Ķ���
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 10:07:33 AM/Jul 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef NETLINK_H_
#define NETLINK_H_

#include "pkg.h"
#include "netdev.h"

//�ú�������ע�������Э�飬�����Э��ֻ��Ҫ�ṩ���������ɡ�������Ҫ�����¼���
enum _EN_NET_PROTO_TYPE
{
	EN_NET_PROTO_IP     = 0x0800,
	EN_NET_PROTO_ARP    = 0x0806,
	EN_NET_PROTO_RARP   = 0x8035,
	EN_NET_PROTO_GOOSE  = 0x9000,
	EN_NET_PROTO_SV     = 0x9001,
};
//�ú�������ע����·��Э�飬����̫����SLIP��RAWLINK��
typedef bool_t (*fnLinkProcess)(tagNetDev *netdev,tagNetPkg *pkg);
typedef bool_t (*fnLinkSend)(tagNetDev *netdev,tagNetPkg *pkg,\
		        u8 *macdst, u8 *macsrc, u16 type, ptu32_t para);
struct tagLinkProtoMsg
{
	u16 type;       //�����Э��
	u8  *macsrc;    //ԴMAC
	u8  *macdst;    //Ŀ��MAC
	ptu32_t para;   //����ʹ�õ���·��Э������Ĳ���
};


//�ú�����������Э�����ʹ��
bool_t Link_Send(tagNetDev *netdev,tagNetPkg *pkg,u32 netdevtask,\
		           u8 *macdst,u16 type, ptu32_t para);
//��������������Ҫ��Ӧ�ô���ʱ����Ҫ���øú���ʵ�����ݵĽ��մ���,�ú�������������������
bool_t Link_Process(tagNetDev *netdev,tagNetPkg *pkg);


#endif /* NETLINK_H_ */

