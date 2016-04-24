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
// ģ������: LoopIf.c:
// ģ��汾: V1.00
// ������Ա: Zqf
// ����ʱ��: 9:04:50 AM/Sep 18, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

#include "os.h"

#include "netdev.h"
#include "rout.h"
#include "link.h"
#include "tcpip_cdef.h"

static u8   sgLoopMac[6]={0,0,0,0,0,0};
static u32  sgLoopIpAddr = 0x7F000001;//127.0.0.1
static u32  sgLoopIpMsk = 0xFFFFFF00 ; //255.255.255.0
static u32  sgLoopGateWay = 0x7F000001; //192.168.1.1

#define CN_LOOPPKG_LEN  8*1024    //8KB
#define CN_LOOPLINK_LEN 0

static struct tagMsgQueue  *pgLoopMsgQ = NULL;
static tagNetDev           *pgLoopNetDev = NULL;


// =============================================================================
// �������ܣ�Loop_Send
//          ���ط��ͺ���
// ��������� netdev,������ʹ�õ�����(����)
//           type,�����Э�����ͣ����룩
//           pkg,�����͵���������ݰ������룩
//           netdevtask,��Ҫ������ɵĹ���
//           macsrc��macdst����netdev�����Կ��Ƿ���Ҫ����������豸����ҪMAC
//           para,����netdev��������ȷ��
// �����������
// ����ֵ  ��
// ˵��    ���ú�������·�����
// =============================================================================
bool_t Loop_Send(tagNetDev *netdev,tagNetPkg *pkg,u32 netdevtask,\
		           u8 *macdst, u8 *macsrc ,u16 type, ptu32_t para)
{
	bool_t result;

	result = false;
	if((NULL == netdev) || (NULL == pkg)||(EN_NET_PROTO_IP != type))
	{
		Pkg_LstFlagFree(pkg);    //�ͷ����е�pkg
	}
	else
	{
		result = NetDev_DevSend(netdev,pkg,netdevtask);
	}
	return result;
}

// =============================================================================
// �������ܣ�Loop_Process
//          ��������һ֡���ݵ�Э��ջ
// ���������pkg,��·�����ݰ�
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܣ��޶�Ӧ������Э����ߴ���ʧ�ܣ�
// ˵��    ����һ֡���ݴ��ݵ���·�����̫��
// =============================================================================
bool_t Loop_Process(tagNetPkg *pkg, struct tagLinkProtoMsg *Msg)
{
	bool_t result = true;
	
	Msg->type = EN_NET_PROTO_IP;	
	Msg->macdst = NULL;
	Msg->macsrc = NULL;
	return result;
}
// =============================================================================
// �������ܣ�__Loop_IfSnd
//          �����������ݰ�
// ���������netdev,ʹ�õ�����
//          pkg,�����͵����ݰ�
//          netdevtask����Ҫ������ɵĹ�����TCP IPУ��͵�
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// ˵��    �����ÿ����ķ�ʽ���ͣ���������ʹ��������
// =============================================================================
bool_t __Loop_IfSnd(tagNetDev *netdev,tagNetPkg *pkg,u32 netdevtask)
{
	bool_t  result;
	tagNetPkg *tmp;
	tagNetPkg *sndpkg;
	u8 *src;
	u8 *dst;
	u32 sndlen;
	
	result = false;
	if((pgLoopNetDev == netdev)&&(NULL != pkg))
	{
		sndlen = 0;
		
		//�鿴�ж������ݲ������ڴ汣��
		tmp = pkg;
		while(NULL != tmp)
		{
			sndlen += tmp->datalen;
			tmp = tmp->partnext;
		}
		sndpkg = Pkg_Alloc(sndlen,CN_PKGFLAG_FREE);
		if(NULL != sndpkg)
		{
			tmp = pkg;
			while(NULL != tmp)
			{
				src = (u8 *)(tmp->buf + tmp->offset);
				dst = (u8 *)(sndpkg->buf + sndpkg->offset + sndpkg->datalen);
				memcpy(dst, src, tmp->datalen);
				sndpkg->datalen += tmp->datalen;
				tmp = tmp->partnext;				
			}

			//������Ϣ��
			result = MsgQ_Send(pgLoopMsgQ,(u8 *)&sndpkg,sizeof(void *),\
			               CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
		}
		Pkg_LstFlagFree(pkg);
	}

	return result;
}

bool_t __Loop_IfRcvIntTask(void)
{
	tagNetPkg  *pkgsnd;
	while(1)
	{
		if(MsgQ_Receive(pgLoopMsgQ,(u8 *)&pkgsnd,sizeof(void *),\
		                CN_TIMEOUT_FOREVER))
		{
			if(false == NetDev_PostPkg(pgLoopNetDev,pkgsnd))
			{
				Pkg_LstFlagFree(pkgsnd);
			}
		}
	}
}

#define CN_LOOP_MSGQSIZE 0X10
// =============================================================================
// �������ܣ�Loop_Init
//          ��Э��ջ���һ�������豸
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// ˵��    ��1,����豸
//        2,���·��
// =============================================================================
bool_t Loop_Init(void)
{
	bool_t  result = false;
	tagNetDevPara  devpara;
	tagHostIpv4Addr devaddr;
	u16 evttid;

	pgLoopMsgQ = MsgQ_Create(CN_LOOP_MSGQSIZE, sizeof(void *),CN_MSGQ_TYPE_FIFO);
	if(NULL != pgLoopMsgQ)
	{

		//create the loop rcv int task
		evttid= Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1, 
				(ptu32_t (*)(void))__Loop_IfRcvIntTask,NULL, 0x400, "loopdevrcvint_task");
		if (evttid != CN_EVTT_ID_INVALID)
		{
			evttid = Djy_EventPop(evttid, NULL, 0, 0, 0, 0);
			if(evttid != CN_EVENT_ID_INVALID)
			{
				devpara.ifsend = __Loop_IfSnd;
				devpara.iftype = EN_LINK_INTERFACE_LOOP;
				memcpy(devpara.mac, sgLoopMac,6); 
				devpara.name = "LoopIf";
				devpara.private = 0;
				devpara.linklen = CN_LOOPLINK_LEN;
				devpara.pkglen = CN_LOOPPKG_LEN;
				devpara.devfunc = CN_IPDEV_ALL;

				pgLoopNetDev = NetDev_AddDev(&devpara);
				if(NULL != pgLoopNetDev)
				{
					devaddr.ip = sgLoopIpAddr;
					devaddr.gateway = sgLoopGateWay;
					devaddr.ipmsk = sgLoopIpMsk;
					if(NULL != Rout_AddRout(pgLoopNetDev, &devaddr))
					{
						result = true;
					}
				}
			}
			else
			{
				printk("LOOP_INIT:pop the LOOP RCV task failed\n\r");
			}
		}
		else
		{
			printk("LOOP_INIT:create the LOOP_INIT RCV task failed\n\r");
		}

	}
	else
	{
		printk("LOOP_INIT: Create msg queue failed\n\r");
	}


	return result;
}

// =============================================================================
// �������ܣ�Loop_Shell
//          ��Э��ջ���һ�������豸
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// ˵��    ��1,����豸
//        2,���·��
// =============================================================================
bool_t Loop_AddDevShell(char *param)
{
	bool_t result;
	
	result = Loop_Init();
	if(result)
	{
		printk("Loop Init Success!\n\r");
	}
	else
	{
		printk("Loop Init Failed\n\r");
	}
	return result;
}

