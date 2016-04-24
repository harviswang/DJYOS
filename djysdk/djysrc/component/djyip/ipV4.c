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
// ģ������: IPV4Э���ʵ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 5:56:55 PM/Jul 11, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "endian.h"
#include "stdio.h"

#include "os.h"

#include "ipV4.h"
#include "link.h"
#include "tcpip_cdef.h"
#include "rout.h"

#define CN_IP_MTU_MIN       46     //���ڸó��ȵ�IP���ݰ���Ҫ��������
#define CN_IP_MTU_MAX       1500   //���ڸó��ȵ�IP���ݰ�Ҫ��Ƭ
#pragma pack(1)
typedef struct _tagIpv4Head
{
#if(CN_CFG_BYTE_ORDER == CN_CFG_BIG_ENDIAN)
  u8    version :4;         //IP VERSION
  u8    headlen :4;         //IP headlen
#else
  u8    headlen :4;         //IP headlen
  u8    version :4;         //IP VERSION
#endif
  u8	tos;				// type of service
  u16	len;		        // total length
  u16	id;			        // identification
#define CN_IP_DF_MSK	0x4000		// dont fragment flag
#define CN_IP_MF_MSK	0x2000		// more fragments flag
#define CN_IP_OFFMASK	0x1FFF	    // mask for fragmenting bits
  u16	fragment;	        // Flags and Fragment Offset
#define CN_IP_TTL_DEFALT	0x40
  u8	ttl;				// time to live
  u8	protocol;			// protocol
  u16	chksum;	            // IP header Checksum
  u32	ipsrc;		        // source IP address
  u32	ipdst;		        // destination IP address
  u8    option[0];          //IPѡ��
}tagIpv4Head;
#pragma pack()

//���ڱ������
typedef struct _tagIpv4Recombin
{
	struct tagIpReComBin *next;       //��֯��������
	u32                  ipsrc;       //ԴIP
	u32                  ipdst;       //Ŀ��IP
	u16                  rebflags;    //������
	u16                  offset;      //Ƭƫ��
	tagNetPkg            *pkg;        //����������ݰ�
}tagIpv4Recombin;

// =============================================================================
// �������ܣ�__Ip_ChksumSoft16
//          IP���У��
// ���������len,����У�����ݵĳ��ȣ���λΪ�ֽڣ�
//           buf,��У�������
//           checksum,ǰ�ε�У���
//           done,�����Ƿ������һ��У��(���һ��У��Ҫ���ȡ��)
// �����������
// ����ֵ  �����ɵ�У����
// ˵��    ��16λ���ȡ��У�������ɣ��ڲ����ã�����������;ȷ��������2�ֽڶ����
// =============================================================================
int __Ip_ChksumSoft16(void *buf,short len,int chksum, bool_t done)
{
	u16 *buf16;

	buf16 = (u16 *)buf;
	for(;len>1;len-=2)
	{
		chksum+=*buf16++;
	}
	if(len ==1)
	{
		chksum += htons((u16)(*(u8 *)buf16) << 8);
	}
	if (done)
	{
		chksum = (chksum>>16) + (chksum&0xFFFF);//����16bit���16bit���
		chksum += (chksum >>16);//����λ����λ��16bit���16bit �����

		return (u16)~chksum;
	}

	return chksum;
}
// =============================================================================
// �������ܣ�Ip_ChksumSoft16
//          IP���У��
// ���������len,����У�����ݵĳ��ȣ���λΪ�ֽڣ�
//           buf,��У�������
//           chksum,ǰ�ε�У���
//           done,�����Ƿ������һ��У��(���һ��У��Ҫ���ȡ��)
// �����������
// ����ֵ  �����ɵ�У����
// ˵��    ��16λ���ȡ��У�������ɣ��ڲ����ã�����������
// =============================================================================
int Ip_ChksumSoft16(void *buf,u16 len,int chksum, bool_t done)
{
	int result = 0;
	if((NULL != buf)&&(len != 0))
	{
		result = __Ip_ChksumSoft16(buf, (short)len ,chksum, done);
	}
	return result;
}

// =============================================================================
// �������ܣ�Ip_Send
//          IP���ݰ��ķ��ͺ���
// ��������� ipsrc, ԴIP,Ϊ0��ʾ��ָ��IP,����·����Ӧ���ң�һ���������һ�η���ͨ��
//           ipdst, Ŀ��IP
//           pkg,��������ݰ�
//           translen,���䳤��
//           proto,�����Э������
//           waithook,�����·��ͨ����ȴ��Ĺ��ӣ�����·ͨ���ִ��
//           hookpara,���ӵĵ�һ���������ڶ���������Э��ջ�ṩ��true��ʾ��·ͨ��false��ʾ��ʱ����ͨ
//           checksumtype,��ҪУ�������
//           u8 *checksum,�洢У����
// �����������
// ����ֵ  ���ο�__EN_IP_SEND_RESULT�Ķ���
// ˵��    ��IP���ĵķ���
// =============================================================================
//ip��־
static u16 Ipv4Flg = 0;
//����һ��PKG
void __Ip_PkgChksum(tagNetPkg *pkg,u16 *chksum)
{
	u16                             chklen;
	int                             chksumtmp;
	void                            *chkbuf;

	chklen = pkg->datalen;
	chkbuf = (u8 *)(pkg->offset + pkg->buf);
	chksumtmp = __Ip_ChksumSoft16((void *)chkbuf, \
			            chklen,0, true);
	*chksum =(u16)chksumtmp;
	return;
}

//���а�����У���
void Ip_PkgLstChksum(tagNetPkg *pkg,u16 *chksum)
{
	u16                             chklen;
	int                             chksumtmp;
	void                            *chkbuf;
	tagNetPkg                       *pkgtmp;

	chksumtmp = 0;
	//У�����е�PKG
	pkgtmp = pkg;
	do{
		chklen = pkgtmp->datalen;
		chkbuf = (u8 *)(pkgtmp->offset + pkgtmp->buf);
		if(NULL == pkgtmp->partnext)
		{
			chksumtmp = __Ip_ChksumSoft16((void *)chkbuf, \
					            chklen,chksumtmp, true);
			*chksum =(u16)chksumtmp;
		}
		else
		{
			chksumtmp = __Ip_ChksumSoft16((void *)chkbuf, \
					            chklen,chksumtmp, false);
		}
		pkgtmp = pkgtmp->partnext;
	}while(NULL != pkgtmp);
}

//���а�����У��Ͳ��Ҽ���α����
void Ip_PkgLstChksumPseudo(u32 iplocal, u32 ipdst, u8  proto,tagNetPkg *pkg,u16 translen,u16 *chksum)
{
	u16                             chklen;
	int                             chksumtmp;
	void                            *chkbuf;
	tagPseudoHdr                    pseudohdr;   //����У���α����
	tagNetPkg                       *pkgtmp;
	
	pseudohdr.ipsrc = htonl(iplocal);
	pseudohdr.ipdst = htonl(ipdst);
	pseudohdr.zero = 0;
	pseudohdr.protocol = proto;
	pseudohdr.len = htons(translen);
	chksumtmp = __Ip_ChksumSoft16((void *)&pseudohdr, \
			                           sizeof(pseudohdr),0, false);
	//У�����е�PKG
	pkgtmp = pkg;
	do{
		chklen = pkgtmp->datalen;
		chkbuf = (u8 *)(pkgtmp->offset + pkgtmp->buf);
		if(NULL == pkgtmp->partnext)
		{
			chksumtmp = __Ip_ChksumSoft16((void *)chkbuf, \
					            chklen,chksumtmp, true);
			*chksum =(u16)chksumtmp;
		}
		else
		{
			chksumtmp = __Ip_ChksumSoft16((void *)chkbuf, \
					            chklen,chksumtmp, false);
		}
		pkgtmp = pkgtmp->partnext;
	}while(NULL != pkgtmp);
}

bool_t Ip_Send(u32 ipsrc, u32 ipdst, tagNetPkg *pkg,u16 translen,u8 proto,\
		       u32 devtask, u16 *chksum)
{

	bool_t                          result;
	u32                             iplocal;	 //ͨ�ŵı���IP
	u32                             devfunc;     //��·���ṩ�Ĺ���
	tagRoutLink                     linkrout;    //��·
	tagNetPkg                       *ippkg;      //ip��
	tagIpv4Head                     *hdr;        //ip����ͷ

	result = false;
	//�ȿ���·�Ƿ�ͨ�������ͨ����û������ı�Ҫ��
	result = Rout_Resolve(ipdst, &linkrout);
	if(false == result)//��·ͨ����������Ͱ�
	{
		return result;
	}

	if(ipsrc != INADDR_ANY)
	{
		iplocal = ipsrc;
	}
	else
	{
		iplocal = linkrout.ip;
	}
	//������ϲ㴫����Ҫ�������������������û�и���������ôӦ����IP����������
	devfunc = NetDev_GetDevFun(linkrout.dev);
	if((0 == (devfunc&devtask))&&(CN_IPDEV_NONE!=devtask))
	{
		//������������ȴ�첻��,��ô�����������
		result = true;
		switch (devtask)
		{
			case  CN_IPDEV_TCPOCHKSUM:
				Ip_PkgLstChksumPseudo(iplocal,ipdst,proto,pkg,translen,chksum);
				break;
			case  CN_IPDEV_UDPOCHKSUM:
				Ip_PkgLstChksumPseudo(iplocal,ipdst,proto,pkg,translen,chksum);
				devtask = 0;
				break;
			case  CN_IPDEV_ICMPOCHKSUM:
				Ip_PkgLstChksum(pkg,chksum);
				break;
			default:
				result = false;
				break;
		}
		devtask = CN_IPDEV_NONE;
	}
	if(false == result)
	{
		return result;  //���û�а��
	}
	//��װIP����ͷ��Ŀǰ��֧��IP����ͷ����ѡ��
	if((translen +20)< linkrout.dev->pkglen)
	{
		ippkg = Pkg_Alloc(sizeof(tagIpv4Head)+linkrout.dev->linklen,CN_PKGFLAG_FREE);
		if(NULL != ippkg)
		{
			ippkg->offset += linkrout.dev->linklen;
			//����IP����ͷ�����
			hdr = (tagIpv4Head *)(ippkg->buf + ippkg->offset);
			hdr->headlen = 5;
			hdr->version = 4;
			hdr->tos = 0;
			hdr->len = htons(translen + 20);
			hdr->id = htons(Ipv4Flg);
			Ipv4Flg++;
			hdr->fragment = htons(CN_IP_DF_MSK);
			hdr->ttl = 64;
			hdr->protocol = proto;
			hdr->chksum = 0;
			hdr->ipsrc = htonl(iplocal);
			hdr->ipdst = htonl(ipdst);
			//PKG���
			ippkg->datalen = sizeof(tagIpv4Head);
			ippkg->partnext = pkg;
			//IP���ĵ�У��������
			if(devfunc & CN_IPDEV_IPOCHKSUM)
			{
				devtask|=CN_IPDEV_IPOCHKSUM;
			}
			else
			{
				__Ip_PkgChksum(ippkg,&hdr->chksum);
			}
			//OK,��װ�ɹ������ͳ�ȥ��
			result= Link_Send(linkrout.dev,ippkg,devtask,\
							  linkrout.mac,EN_NET_PROTO_IP,(ptu32_t)NULL);
			if(false == result)
			{
				Pkg_PartFree(ippkg);
			}
		}
		else
		{
			result = false;
		}
	}
	else
	{
		result = false;   //--TODO, large TPL data, must be devided
	}
	
	return result;
}
// =============================================================================
// �������ܣ�IP���ݰ��ְ�������
// ���������pkg��������İ�
// �����������
// ����ֵ  ������õİ����У������û����������򷵻�NULL
// ˵��    ��
// =============================================================================
tagNetPkg * __Ip_PkgRecover()
{
	return NULL;
}


extern bool_t Icmp_RecvProcess(u32 ipsrc, u32 ipdst, tagNetPkg *pkglst, u32 devfunc);
extern bool_t Udp_RecvProcess(u32 ipsrc, u32 ipdst, tagNetPkg *pkg, u32 devfunc);
extern bool_t Tcp_RecvProcess(u32 ipsrc, u32 ipdst, tagNetPkg *pkg, u32 devfunc);
// =============================================================================
// �������ܣ�IP���ݰ��Ĵ�����
// ���������pkg�����ݸ�IP������ݰ�
//          devfunc,����Ӳ���Ĺ���
// �����������
// ����ֵ  ��true pkg������,���ܱ��ͷţ� falseû�б����棬��Ҫ�Լ��ͷ�
// ˵��    ��
// =============================================================================
bool_t __Ip_Process(tagNetPkg *pkg,u32 devfunc)
{
	bool_t result;
	u8     tplproto;
	u32    headlen;
	u32    ipdst;
	u32    ipsrc;
	tagIpv4Head *hdr;
	tagNetPkg   *tplpkg;           //��������װ������ݰ�

	hdr = (tagIpv4Head *)(pkg->buf + pkg->offset);
	//��������У�飬�����ȷ��û�б�Ҫ����������ȻУ���ȿ���Ӳ���Ƿ��Ѿ�У�飬
	//Ӳ��û��У�������У�飬����У��
	headlen = hdr->headlen*4;
	pkg->offset += headlen;
	pkg->datalen = ntohs(hdr->len) - headlen;
	if(0 == pkg->datalen)
	{
		result = Pkg_LstFlagFree(pkg);
		return result;
	}
	
	if(0 ==(devfunc &CN_IPDEV_IPICHKSUM))
	{
		//do the chksum
		if(0 != __Ip_ChksumSoft16((void *)hdr,headlen,0,true))
		{
			result = Pkg_LstFlagFree(pkg);
			return result;
		}
	}
	//�����Ƿ��Ǳ������ģ������Ҫ����IP
	ipdst = ntohl(hdr->ipdst);
	ipsrc = ntohl(hdr->ipsrc);
	if(false == Rout_IsLocalDst(ipdst)) 
	{
		result = Pkg_LstFlagFree(pkg);//TODO��ת������
	}
	else
	{
		tplproto = hdr->protocol;
		hdr->fragment = ntohs(hdr->fragment);
		if((hdr->fragment & CN_IP_DF_MSK)||\
		  ((0==(hdr->fragment&CN_IP_MF_MSK))&&(0 == (hdr->fragment&CN_IP_OFFMASK))))
		{
			tplpkg = pkg;
		}
		else
		{	
			tplpkg = NULL;//TODO,��Ƭ���鹦��
			result = Pkg_LstFlagFree(pkg); 
		}
		if(NULL != tplpkg)
		{
			switch(tplproto)
			{
				case CN_IPTPL_TCP:
					result =  Tcp_RecvProcess(ipsrc, ipdst,tplpkg, devfunc);
					break;
				case CN_IPTPL_UDP:
					result =  Udp_RecvProcess(ipsrc, ipdst,tplpkg, devfunc);
					break;
				case CN_IPTPL_ICMP:
					result =  Icmp_RecvProcess(ipsrc, ipdst,tplpkg, devfunc);
					break;
				default:
					result = false;
					break;
			}
			if(false == result)
			{
				result = Pkg_LstFlagFree(tplpkg);
			}
		}
	}

	return result;
}

static struct tagMsgQueue   *pgIpMsgBox =NULL;
typedef struct
{
	tagNetDev  *netdev;
	tagNetPkg  *pkglst;
}tagIpPostMsg;

// =============================================================================
// �������ܣ�Ip_PostPkg,��IP��Ͷ�����ݰ���Ϣ
// ���������netdev ,Ͷ�����ݵ�����
//          pkglst���ݰ�
// �����������
// ����ֵ  ��true �ɹ������ݰ����ɹ�����;false,�����ݰ�����ʧ��
// =============================================================================
bool_t Ip_PostPkg(tagNetDev *netdev, tagNetPkg *pkglst)
{
	bool_t result = false;
	tagIpPostMsg  msg;

	if((NULL != netdev)&&(NULL != pkglst))
	{
		msg.netdev = netdev;
		msg.pkglst = pkglst;
		result = MsgQ_Send(pgIpMsgBox,(u8 *)&msg,sizeof(tagIpPostMsg),\
			               CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
	}

	return result;
}
// =============================================================================
// �������ܣ�__Ip_RcvPkg,����Ϣ���н�����Ϣ
// ���������
// ���������msg,���յ�����Ϣ
// ����ֵ  ��true,���ճɹ�, false����ʧ��
// =============================================================================
bool_t __Ip_RcvPkg(tagIpPostMsg *msg)
{
	return MsgQ_Receive(pgIpMsgBox,(u8 *)msg,sizeof(tagIpPostMsg),\
		                CN_TIMEOUT_FOREVER);
}


// =============================================================================
// �������ܣ�__Ip_ProcessTask,����IP��Ľ��յ������ݰ�
// ���������
// �����������
// ����ֵ  �����߳���IP���ݴ�������߳�,û�����������ǲ����˳���
// =============================================================================
ptu32_t __Ip_ProcessTask(void)
{
	tagIpPostMsg msg;
	while(1)
	{
		if(__Ip_RcvPkg(&msg))
		{
			__Ip_Process(msg.pkglst,msg.netdev->devfunc);
		}
	}
}
#define CN_IPMSG_BOX   100          //����256��������Ϣ
// =============================================================================
// �������ܣ�Ip_Init
//          IPģ���ʼ��
// ���������para,��ʾIP������Щ����
// �����������
// ����ֵ  ��Ŀǰ������
// ˵��    ����һЩIPģ��ĳ�ʼ�������幤������
// =============================================================================
bool_t Ip_Init(ptu32_t para)
{
	bool_t result = false;
	u16   evttid;

	pgIpMsgBox = MsgQ_Create(CN_IPMSG_BOX, sizeof(tagIpPostMsg),CN_MSGQ_TYPE_FIFO);
	if(NULL != pgIpMsgBox)
	{
		evttid= Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1, 
				(ptu32_t (*)(void))__Ip_ProcessTask,NULL, 0x800, "Ip_ProcessTask");
		if (evttid != CN_EVTT_ID_INVALID)
		{
			evttid = Djy_EventPop(evttid, NULL, 0, 0, 0, 0);
			if(evttid != CN_EVENT_ID_INVALID)
			{
				result = true;
			}
			else
			{
				printk("IP_INIT:pop the process task failed\n\r");
			}
		}
		else
		{
			printk("IP_INIT:create the process task failed\n\r");
		}

	}
	else
	{
		printk("IP_INIT:Create the msg box failed\n\r");
	}
	
	return result;
}

