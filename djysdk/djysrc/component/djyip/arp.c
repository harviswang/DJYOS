//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, 
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, 
//    this list of conditions and the following disclaimer in the documentation 
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

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
// ģ������: ArpЭ��ʵ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 9:23:45 AM/Jul 7, 2014
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

#include "link.h"
#include "rout.h"
#include "arp.h"

//arp�����ݱ�
#pragma pack(1)
typedef struct _tagArpHead 
{
#define CN_ARP_HW_TYPE_ETHERNET        0x01
	u16	hwtype;
#define CN_ARP_PR_TYPE_IP              0x0800
	u16	protype;
#define CN_ARP_HW_LEN_ETHERNET         0x06
	u8	hwaddrlen;
#define CN_ARP_PR_LEN_IP               0x04
	u8	proaddrlen;
#define CN_ARP_OP_REQUEST              0x01
#define CN_ARP_OP_RESPONSE             0x02
	u16	opcode;
	u8	senhwaddr[6];
	u8	senproaddr[4];
	u8	tarhwaddr[6];
	u8	tarproaddr[4];
	u8	rev_blank[18];	// ��̫����Ҫ���ϲ����ݰ�������46�ֽڣ���18�ֽ��������
}tagArpHead;
#pragma pack()
#define CN_ARP_HDRLEN   46         
// =============================================================================
// �������ܣ�____Arp_Process_Request
//          �����յ���ARP�����
// ���������netdev, �յ�ARP���ݰ�������
//          arp���������arp����
// ���������
// ����ֵ  ��true����ɹ���false����ʧ��
// ˵��    :����ARP�������ݰ�
// =============================================================================
bool_t ____Arp_Process_Request(tagNetDev *netdev,tagArpHead *arp)
{
	bool_t             result;
	u32                ipdst;
	tagNetPkg          *pkg;
	tagArpHead         *hdr; 
	u8                 *macsrc;
	u32                ipsrc;

	result = false;
	memcpy(&ipdst,arp->tarproaddr,4);
	ipdst = ntohl(ipdst);
	//���Ŀ������������ǵ�����IP,�򻹱����Ӧ��
	if(Rout_IsLocalIp(ipdst))
	{
		//����RoutItem
		macsrc = arp->senhwaddr;
		memcpy(&ipsrc,arp->senproaddr,4);
		ipsrc = ntohl(ipsrc);
		Rout_ItemUpdate(ipsrc, macsrc); //ARP���İ�����ARP��Ϣ����
		
		//response the arp request

		pkg = Pkg_Alloc(CN_ARP_HDRLEN+ netdev->linklen,CN_PKGFLAG_FREE);
		if(NULL != pkg) //���䲻�ɹ�
		{
			//LEAVE SOME SPACE FOR THE LINK
			pkg->offset += netdev->linklen;
			//��д���ݰ�
			//���ARP����
			hdr = (tagArpHead *)(pkg->buf + pkg->offset);
			memcpy(hdr->tarhwaddr, arp->senhwaddr, 6);
			memcpy(hdr->tarproaddr ,arp->senproaddr,4);
			memcpy(hdr->senproaddr , arp->tarproaddr,4);
			memcpy(hdr->senhwaddr,netdev->mac,6);
			hdr->hwtype = htons(CN_ARP_HW_TYPE_ETHERNET);
			hdr->protype = htons(CN_ARP_PR_TYPE_IP);
			hdr->hwaddrlen = CN_ARP_HW_LEN_ETHERNET;
			hdr->proaddrlen = CN_ARP_PR_LEN_IP;
			hdr->opcode = htons(CN_ARP_OP_RESPONSE);
			pkg->datalen = sizeof(tagArpHead);
			result = Link_Send(netdev,pkg,0,hdr->tarhwaddr,\
					           EN_NET_PROTO_ARP, (ptu32_t)NULL);
			if(false == result)
			{
				Pkg_PartFree(pkg);
			}
		}	
	
	}
	return result;	
}

// =============================================================================
// �������ܣ�____Arp_Process_Response
//          �����յ���ARP��Ӧ��
// ���������netdev, �յ�ARP���ݰ�������
//          arp���������arp����
// ���������
// ����ֵ  ��true����ɹ���false����ʧ��
// ˵��    :����ARP��Ӧ���ݰ�
// =============================================================================
bool_t ____Arp_Process_Response(tagNetDev *netdev,tagArpHead *arp)
{
	u8     *macsrc;
	u32     ipsrc;
	macsrc = arp->senhwaddr;
	memcpy(&ipsrc,arp->senproaddr,4);
	ipsrc = ntohl(ipsrc);
	Rout_ItemUpdate(ipsrc, macsrc); //ARP���İ�����ARP��Ϣ����
	return true;
}
// =============================================================================
// �������ܣ�__Arp_Process
//          �����յ���ARP��
// ���������netdev, �յ�ARP���ݰ�������
//          pkg���յ���ARP��
//          macdst,Ŀ��MAC
//          macsrc,ԴMAC
// ���������
// ����ֵ  ��true����ɹ���false����ʧ��
// ˵��    ��Ŀǰֻ�ᴦ����̫��Ӳ����IP�Ĳ���
//           1,������ARP������Ӧ�𣬱�Ȼ���з��Ͷ˵�ARP��MAC��IP��ҪARP����
//           2,�����ARP������Ŀ��IP�Ǳ�������IP����ARP��Ӧ������Ӧ
// =============================================================================
bool_t __Arp_Process(tagNetDev *netdev,tagNetPkg *pkg,u8 *macdst, u8 *macsrc)
{
	bool_t       result;
	tagArpHead   *hdr;
	u16          opcode;
	if((NULL != netdev)&& (NULL!= pkg) &&(NULL != macdst) &&(NULL != macsrc))
	{
		hdr = (tagArpHead *)(pkg->buf + pkg->offset);
		pkg->offset += sizeof(tagArpHead);
		pkg->datalen -= sizeof(tagArpHead);
		opcode = ntohs(hdr->opcode);
		switch (opcode)
		{
			case CN_ARP_OP_REQUEST:
				result = ____Arp_Process_Request(netdev,hdr);
				break;
			case CN_ARP_OP_RESPONSE:
				result = ____Arp_Process_Response(netdev,hdr);
				break;
			default:
				break;
		}
		Pkg_LstFlagFree(pkg);
		result = true;
	}
	return result;
}
bool_t Arp_PostPkg(tagNetDev *netdev,tagNetPkg *pkglst,u8 *macdst, u8 *macsrc)
{
	return __Arp_Process(netdev,pkglst,macdst,macsrc);
}

// =============================================================================
// �������ܣ�ARP_Request
//          ����ARP����
// ���������netdev,ʹ�õ�ͨ���豸
//          ipdst,�����IP����
//          iplocal,����ı���IP
// ���������
// ����ֵ  ��true����ɹ���false����ʧ��
// ˵��    ���㲥һ��ARP������
// =============================================================================
bool_t Arp_Request(tagNetDev *netdev,u32 ipdst, u32 iplocal)
{
	bool_t            result;
	tagNetPkg         *pkg;
	tagArpHead        *hdr;
	u8                broadmac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};//�㲥MAC��ַ
	
	result = false;
	if(NULL != netdev)
	{
		pkg = Pkg_Alloc(CN_ARP_HDRLEN+ netdev->linklen,CN_PKGFLAG_FREE);
		if(NULL != pkg) //���䲻�ɹ�
		{
			//LEAVE SOME SPACE FOR THE LINK
			pkg->offset += netdev->linklen;
			//��д���ݰ�
			hdr = (tagArpHead *)(pkg->buf + pkg->offset);
			hdr->hwtype = htons(CN_ARP_HW_TYPE_ETHERNET);
			hdr->protype = htons(CN_ARP_PR_TYPE_IP);
			hdr->hwaddrlen = CN_ARP_HW_LEN_ETHERNET;
			hdr->proaddrlen = CN_ARP_PR_LEN_IP;
			hdr->opcode = htons(CN_ARP_OP_REQUEST);
			
			ipdst = htonl(ipdst);
			memcpy(hdr->tarproaddr,&ipdst,4);
			iplocal = htonl(iplocal);
			memcpy(hdr->senproaddr,&iplocal,4);
			memcpy(hdr->senhwaddr,netdev->mac,6);
			pkg->datalen = sizeof(tagArpHead);
			result = Link_Send(netdev,pkg,0,broadmac, EN_NET_PROTO_ARP,(ptu32_t)NULL);
			if(false == result)
			{
				Pkg_PartFree(pkg);
			}
		}	
	
	}
	return result;
}


// =============================================================================
// �������ܣ�Arp_Init
//          ARPģ���ʼ��
// �����������
// �����������
// ����ֵ  ��true �ɹ�  false ʧ��
// ˵��    ����Ҫ����Link��ע��ARPЭ����Ĵ�����
// =============================================================================
bool_t Arp_Init(void)
{
	return true;
}


