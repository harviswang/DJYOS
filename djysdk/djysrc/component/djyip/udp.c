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
// ģ������: UdpЭ���ʵ�֣��о���ĵ�Э��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 5:55:35 PM/Jul 14, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// 		 1.����FIN�źŵķ�������ʶ������FIN��ʾ�����������е����ݶ��ر����ӣ������Ǳ��뱣֤
//         ���е����ݷ�����ϣ��յ�ȷ�ϣ������FIN�źŵķ���Ҳ�����ش��Ȼ���,������ר��
//         ��FIN��ʱ�����Ǹ����ش���ʱ��
//       2.������FIN�ź�֮���п����ش���ǰ���͵����ݣ���ΪFIN�ź�����Ϊ���ݷ��͵ģ�
//         FIN����ʱ��û��ȷ����ǰ���͵����ݵõ�ȷ�ϣ���˺��п��ܻ��ش�����
//       3.����ͷ�ĺϷ������ͳһ�Ĵ����⣨SYN FIN RST�ļ��ݼ�����⣩
//       4.RST������ͳһ�Ĵ���
//       5.FIN��ǵķ��ͣ���Ҫ����FINʱ����shutdown wr����CLOSE��������Ͷ���Ϊ�գ���ֱ��
//         ����FIN������ȴ��������ݷ��ͺ�Ҫô���ر����з��ͣ�Ҫô��������FIN����
//       6,UNACK�����ǰ��մ�UNACK(ѭ����)��С�����˳������

#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "endian.h"
#include "string.h"

#include "os.h"

#include "rout.h"
#include "tpl.h"
#include "ipV4.h"
#include "udp.h"
//define for the udp msg pkg len
#define CN_UDP_PKGMSGLEN      1472

//define for the register in the tpl layer
static tagTplProtoRscNode     *pgUdpProtoRsc;
static struct tagMutexLCB     *pgUdpSockQSync;
static tagSocket              *pgUdpSockQH;            //Udp sock head
static tagSocket              *pgUdpSockQT;            //Udp sock tail

//used for the port alloc
#define CN_UDP_PORT_TEMPBASE  0x8000
#define CN_UDP_PORT_INVALID   0x0
#define CN_UDP_PORT_LIMIT     0xFFFF
static  u16                   sgUdpPortCurFree = CN_UDP_PORT_TEMPBASE;

//used for manage the ucb mem
#define CN_UCB_POOLSIZE       5  //100�����ӣ�����ˣ����Զ�̬����
static tagUCB                 sgUCBPool[CN_UCB_POOLSIZE];
static struct tagMemCellPool  *pgUCBPool = NULL;

#define CN_USESSION_POOLSIZE  5
static tagUdpTnode            sgUdpSessionPool[CN_USESSION_POOLSIZE];
static struct tagMemCellPool  *pgUdpSessionPool = NULL;

///////////////////////////FUNCTION METHOD//////////////////////////////////////
// =============================================================================
// �������ܣ�__Udp_GetSysTime
//        ��ȡϵͳʱ��
// �����������
// ���������
// ����ֵ  ��s64 ��ȡϵͳʱ��
// ˵��    :
// =============================================================================
s64 __Udp_GetSysTime()
{
	return DjyGetTime();
}
// =============================================================================
// �������ܣ�__Udp_InitCB
//       Init the controller
// ���������ucb, the ucb to init
// ���������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void __Udp_InitCB(tagUCB *ucb)
{
    //fill all the space to zeros, then set some data what we need
    memset((void *)ucb, 0, sizeof(tagUCB));
    //set algorithm default
    //set the channelstate, permit the kernel receive and snd
    ucb->channelstate.krcv = 1;
    ucb->channelstate.ksnd = 1;
    //set the snd and receive buf limit to default
    ucb->sbuf.buflenlimit  = CN_UDP_BUF_LEN_lIMIT;
    ucb->rbuf.buflenlimit  = CN_UDP_BUF_LEN_lIMIT;
    ucb->lifetick = CN_UDP_TERMINAL_LIFE;
}
// =============================================================================
// �������ܣ�__Udp_RstCB
//       UDP���ƿ�ĸ�λ
// ���������udp,����λ��ucb���ƿ�
// ���������
// ����ֵ  ����
// ˵��    :��UDP���ƿ�ָ����������ʱ��ģ��
// =============================================================================
void __Udp_RstCB(tagUCB *ucb)
{
	//release the receive buf
	if(NULL != ucb->rbuf.pkglsthead)
	{
	    Pkg_LstFlagFree(ucb->rbuf.pkglsthead);
	}
    ucb->rbuf.buflen = 0;
    ucb->rbuf.buflenlimit  = CN_UDP_BUF_LEN_lIMIT;
    ucb->rbuf.pkglsthead = NULL;
    ucb->rbuf.pkglsttail = NULL;
    ucb->rbuf.triglevel = 0;
	Lock_SempPost(ucb->rbuf.bufsync);
	//״̬�л�
	ucb->channelstate.arcv = 0;
	ucb->channelstate.asnd = 0;

	//set the socket invalid, avoiding the hook to snd some invalid msg
	ucb->owner->valid = false;
}

static tagUCB * sgDebugucb;    //used for ucb
// =============================================================================
// �������ܣ�__Udp_NewCB
//          ����һ��ucb���ƿ�
// ���������NULL
// ���������
// ����ֵ  �������ucb���ƿ�
// ˵��    :��Ҫ�ǿռ���䣬�Լ���Դ�ĳ�ʼ��
// =============================================================================
tagUCB *__Udp_NewCB(void)
{
	tagUCB *ucb;
	//����ucb�ڴ���Դ
	ucb = Mb_Malloc(pgUCBPool, CN_TIMEOUT_FOREVER);
	if(NULL != ucb)
	{
		//init the ucb member
		__Udp_InitCB(ucb);
		//create two semphore for the rcv and sndbuf
		ucb->rbuf.bufsync = Lock_SempCreate(1,1,CN_SEMP_BLOCK_FIFO,NULL);
		if(NULL == ucb->rbuf.bufsync)
		{
			Mb_Free(pgUCBPool,ucb);
			ucb =NULL;
		}
		else
		{
			sgDebugucb = ucb;
		}
	}

	return ucb;
}
// =============================================================================
// �������ܣ�__Udp_FreeCB
//          �ͷ�һ��ucb���ƿ�
// ���������NULL
// ���������
// ����ֵ  ��true�ɹ� fasleʧ��
// ˵��    :��Ҫ�ǿռ����
// =============================================================================
bool_t __Udp_FreeCB(tagUCB *ucb)
{
	Lock_SempDelete(ucb->rbuf.bufsync);
	Lock_SempDelete(ucb->sbuf.bufsync);
	Mb_Free(pgUCBPool,ucb);
	return true;
}
static tagUdpTnode*  sgDebugTnode; //used fo debug
// =============================================================================
// �������ܣ�__Udp_NewTnode
//          new a Tnode
// ���������void
// ���������
// ����ֵ  ��tagUdpTnode
// ˵��    :
// =============================================================================
tagUdpTnode* __Udp_NewTnode(void)
{
	tagUdpTnode *result = NULL;

	result =(tagUdpTnode *) Mb_Malloc(pgUdpSessionPool,CN_TIMEOUT_FOREVER);
	if(NULL != result)
	{
		result->connum = 0;
		result->connumlimit = CN_UDP_BUF_LEN_lIMIT;
		result->clientlst = NULL;
		sgDebugTnode = result;
	}

	return  result;
}

// =============================================================================
// �������ܣ�__Udp_FreeTnode
//          free a Tnode
// ���������tnode
// ���������
// ����ֵ  ��true success while false failed
// ˵��    :
// =============================================================================
bool_t __Udp_FreeTnode(tagUdpTnode* tnode)
{
	Mb_Free(pgUdpSessionPool,(void *)tnode);
	return true;
}
// =============================================================================
// �������ܣ�  Udp_Socket
//         ����һ���׽���
// ���������  family,��������AF_INET AF_LOCAL��
//        type,���ͣ���SOCK_STREAM SOCK_DGRAM��
//        protocal,Э�����ͣ�һ���Ĭ��Ϊ0
// ���������
// ����ֵ  ���׽��ֺţ�-1��ʾ����ʧ��
// ˵��    ���׽��ִ������ɹ���������ָ��Э��û���ҵ������ڴ治��
//        �������Ӧ����û���κ�����ģ���Ϊ�׽����ǿ�����Щ�������ҵ���Э���
// =============================================================================
tagSocket * Udp_Socket(int family, int type, int protocal)
{
	tagSocket    *sockfd;

	sockfd = Socket_New();
	if(NULL != sockfd)
	{
		sockfd->laddr.port = CN_UDP_PORT_INVALID;//����Ϊ��Ч�˿ںţ����ڶ�β
		sockfd->proto = &pgUdpProtoRsc->proto;
		sockfd->appstate = EN_SOCKET_LOCAL_CREATE;
		//we don't add the socket to the queue now
		//when bind or connect,it will alloc valid port to the socket. then we will
		//add it to the socket queue
	}
	return sockfd;
}
// =============================================================================
// �������ܣ�  __Udp_NewPort
//        Find an valid port to use
// ���������  void
// ���������  void
// ����ֵ  ��u16, the valid port to use, if CN_UDP_PORT_INVALID means could
//       not alloc the port dynamic
// ˵��    ��
// =============================================================================
u16 __Udp_NewPort()
{
	int chktimes;
	int chktimeslimit;
	tagSocket *sock;
	u16 result = CN_UDP_PORT_INVALID;

	chktimes = 0;
	chktimeslimit = (int)(CN_UDP_PORT_LIMIT - CN_UDP_PORT_TEMPBASE);

	while(chktimes < chktimeslimit)
	{
		sock = pgUdpSockQH;
		while(NULL != sock)
		{
			if(sgUdpPortCurFree == sock->laddr.port)
			{
				sock =NULL;
			}
			else
			{
				if(NULL == sock->nxt)
				{
					//chk all the sock, and find that the sock is valid to use
					result = sgUdpPortCurFree;
				}
				else
				{
					sock = sock->nxt;
				}
			}
		}
		if(result != CN_UDP_PORT_INVALID)//find the valid port
		{
			//end the chk sume
			chktimes = chktimeslimit;
		}
		//add the free port
		if(sgUdpPortCurFree == CN_UDP_PORT_LIMIT)
		{
			sgUdpPortCurFree = CN_UDP_PORT_TEMPBASE;
		}
		else
		{
			sgUdpPortCurFree++;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Udp_ChkBindValid
//        to chk wheather we could bind the specified ip and port
// ���������  ip, port
// ���������
// ����ֵ  ��true, valid while false invalid
// ˵��    ��
// =============================================================================
u16 __Udp_ChkBindValid(u32 ip, u16 port)
{

	bool_t result;
	tagSocket *sock;

	result = true;
	sock = pgUdpSockQH;
	while(NULL != sock)
	{
		if(port == sock->laddr.port)
		{
			if(ip == sock->laddr.ip)
			{
				result = false;
				sock =NULL;
			}
			else
			{
				sock = sock->nxt;
			}
		}
		else
		{
			sock = sock->nxt;
		}
	}
	return result;
}

// =============================================================================
// �������ܣ�  Udp_Bind
//        Ϊһ���׽��ְ�һ�������ַ
// ��������� sockfd,���󶨵��׽���
//        myaddr,ָ���������ַ
//        addrlen,�����ַ����
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�������ַ��ͻ���˿ںţ�
// =============================================================================
int Udp_Bind(tagSocket *sockfd,struct sockaddr *addr, int addrlen)
{
	int  result;
	u32 ip;
	u16 port;
	struct sockaddr_in *sockaddrin;

	result = -1;

	if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
	{
		if(EN_SOCKET_LOCAL_CREATE == sockfd->appstate)
		{
			sockaddrin = (struct sockaddr_in *)addr;
			ip = sockaddrin->sin_addr.s_addr;
			ip = ntohl(ip);
			port = sockaddrin->sin_port;
			port = ntohs(port);
			if(Rout_IsLocalIp(ip))//�Ǳ���IP
			{
				Lock_MutexPend(pgUdpSockQSync, CN_TIMEOUT_FOREVER);
				if(CN_UDP_PORT_INVALID == port)
				{
					//alloc an new port and add the socket to the socket queue
					port = __Udp_NewPort();
					if(port == CN_UDP_PORT_INVALID)
					{
						printk("Tcp:bind:no new port to bind");
					}
					else
					{
						sockfd->laddr.port  = port;
						result = 0;
					}
				}
				else
				{
					//chk the specifyed ip and port valid
					if(__Udp_ChkBindValid(ip,port))
					{
						result = 0;
					}
				}
				if(0 == result)
				{
					//add the socket to the tcp socket queue
					sockfd->nxt = NULL;
					sockfd->appstate = EN_SOCKET_LOCAL_BIND;
					sockfd->laddr.ip = ip;
					sockfd->laddr.port = port;
					if(NULL ==pgUdpSockQT)
					{
						pgUdpSockQH = sockfd;
						sockfd->pre = NULL;
					}
					else
					{
						pgUdpSockQT->nxt = sockfd;
						sockfd->pre = NULL;
					}
					pgUdpSockQT = sockfd;
				}

				Lock_MutexPost(pgUdpSockQSync);
			}
		}
		Lock_MutexPost(sockfd->sync);
	}

	return  result;
}
// =============================================================================
// �������ܣ�  Udp_Listen
//        ָ���׽���Ϊ����״̬
// ���������  sockfd,��ָ�����׽���
//        backlog,��������Ӹ�����Ĭ�ϵ�Ϊ5
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�ظ�ָ��
// =============================================================================
int Udp_Listen(tagSocket *sockfd, int backlog)
{
	int  result;
	tagUdpTnode *tnode;

	result = -1;
	if(NULL != sockfd)
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			if(EN_SOCKET_LOCAL_BIND == sockfd->appstate)
			{
				//create the casync
				sockfd->casync = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,NULL);
				if(NULL != sockfd->casync)
				{
					tnode = __Udp_NewTnode();
					if(NULL != tnode)
					{
						if(0 != backlog)
						{
							tnode->connumlimit = backlog;
						}
						sockfd->appstate = EN_SOCKET_LOCAL_LISTEN;
						sockfd->socktype = EN_SOCKET_TREENODE;
						sockfd->cb = (void *)tnode;
						result = 0;
					}
					else
					{
						Lock_SempDelete(sockfd->casync);
						sockfd->casync = NULL;
					}
				}
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}

// =============================================================================
// �������ܣ�__Udp_AceptNew
//        acceptһ��������
// ���������  sockfd,���������׽���
// ���������
// ����ֵ  �����ӵĿͻ����׽���
// ˵��    ���ú�������������SOCKFD�в���һ���µ�δ��accept������
//        ��������֮��Ŀ�����ʹ�����Լ�ȥ����
// =============================================================================
tagSocket *__Udp_AceptNew(tagSocket *sockfd)
{
	tagSocket    *result;
	tagSocket    *clientsock;
	tagUCB       *ucb;
	tagUdpTnode  *tnode;

	result = NULL;
	tnode = (tagUdpTnode *)sockfd->cb;
	clientsock = tnode->clientlst;
	if(tnode->connum2accept > 0)
	{
		while(NULL != clientsock)
		{
			ucb = (tagUCB *)clientsock->cb;
			if(0 == ucb->channelstate.open)
			{
				ucb->channelstate.open = 1;
				ucb->channelstate.arcv = 1;
				ucb->channelstate.asnd = 1;

				clientsock->status = CN_SOCKET_WRITEALBE|CN_SOCKET_READALBE;
				result = clientsock;
				clientsock = NULL;//����ѭ��
			}
			else
			{
				clientsock= clientsock->nxt;
			}
		}
		if(NULL != result)
		{
			tnode->connum2accept--;
			if(tnode->connum2accept == 0)
			{
				sockfd->status &=(~CN_SOCKET_ACCEPT);
				Multiplex_Set(sockfd->multiio, sockfd->status);
			}
		}
	}


	return result;
}
// =============================================================================
// �������ܣ�  Udp_Accept
//        �ȴ��ͻ��˵�����
// ���������  sockfd,���������׽���
// ���������  addr,���ӵĿͻ��˵�ַ
//        addrlen,��ַ����
// ����ֵ  �����ӵĿͻ����׽���
// ˵��    ������һ������Ϊ����������û�����ӵĿͻ��ˣ����߳�������������
// =============================================================================
tagSocket *Udp_Accept(tagSocket *sockfd, struct sockaddr *addr, int *addrlen)
{
	tagSocket  	*result;
	tagUCB   *ucb;

	struct sockaddr_in  *addrin;

	result = NULL;
	if(NULL != sockfd)//�������
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			if(EN_SOCKET_LOCAL_LISTEN == sockfd->appstate) //make sure it is a server
			{
				result = __Udp_AceptNew(sockfd);
				if((NULL == result)&&(0 == sockfd->constate.noblock))
				{
					while(NULL == result)//һֱ�ȴ�
					{
						Lock_MutexPost(sockfd->sync);
						Lock_SempPend(sockfd->casync,CN_TIMEOUT_FOREVER);
						Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER);
						result = __Udp_AceptNew(sockfd);
					}
				}
				else
				{
					sockfd->errorno = EN_SOCKET_TRY_AGAIN;
				}
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	if((NULL != result)&&(NULL != addr) &&(NULL != addrlen))
	{
		//����ַ
		ucb = (tagUCB *)result->cb;
		addrin = (struct sockaddr_in *)addr;
		addrin->sin_family = AF_INET;
		addrin->sin_port = htons(ucb->raddr.port);
		addrin->sin_addr.s_addr = htonl(ucb->raddr.ip);
		*addrlen = sizeof(struct sockaddr);
	}
	return result;
}

// =============================================================================
// �������ܣ�__Udp_MsgSend
//       snd the msg
// ���������ucb,
//        msg,   the msg to snd
//        len,   the msg len
//        flags, msg flags
// ���������
// ����ֵ  ��
// ˵��    :just snd the msg, maybe the route is resolving,but
// =============================================================================
int __Udp_MsgSend(tagUCB *ucb, const void *msg, int len, int flags)
{
	bool_t          sndresult;
	int             result;
	u16             translen;
	u32             iplocal, ipremote;
	u16             portlocal, portremote;
	tagUdpHdr       *hdr;
	tagNetPkg       *pkg;
	u8              *src;
	u8              *dst;

	result = 0;
	if((len <= ucb->sbuf.buflenlimit)&&(1== ucb->channelstate.asnd))
	{
		iplocal = ucb->owner->laddr.ip;
		portlocal =  ucb->owner->laddr.port;
		ipremote = ucb->raddr.ip;
		portremote = ucb->raddr.port;
		src = (u8 *)msg;
		translen = 0;
		while(len > 0)
		{
			translen = len>CN_UDP_PKGMSGLEN?CN_UDP_PKGMSGLEN:len;
			pkg = Pkg_Alloc(translen + sizeof(tagUdpHdr),CN_PKGFLAG_FREE);
			if(NULL != pkg)
			{
				//do the udp header
				hdr = (tagUdpHdr *)(pkg->buf + pkg->offset);
				hdr->portdst = htons(portremote);
				hdr->portsrc = htons(portlocal);
				hdr->msglen = translen + sizeof(tagUdpHdr);
				hdr->msglen = htons(hdr->msglen);
				hdr->chksum = 0;
				//cpy the msg
				dst = (u8 *)hdr + sizeof(tagUdpHdr);
				memcpy(dst,src, translen);
				//ok, the msg has cpy to the pkg, then snd the pkg
				pkg->datalen = translen + sizeof(tagUdpHdr);
				sndresult = Ip_Send(iplocal, ipremote, pkg,translen,CN_IPTPL_UDP,\
						CN_IPDEV_UDPOCHKSUM,&hdr->chksum);
				if(sndresult)
				{
					//update the src and len
					result += translen;
					len-=translen;
					src = src + translen;
				}
				else
				{
					Pkg_PartFree(pkg);
					break;
				}
			}
			else
			{
				printk("Udp:Snd:No more mem--len = 0x%08x!\n\r",translen);
				break;  //no mem for the pkg
			}
		}
	}
	ucb->lifetick = CN_UDP_TERMINAL_LIFE;

	if(0 == result)
	{
		result = -1;
	}
	return result;
}
// =============================================================================
// �������ܣ�  Udp_Connect
//        ���ӷ�����
// ���������  sockfd,�ͻ����׽���
//        addr,���ӵķ�������ַ
//        addrlen����ַ����
// ���������
// ����ֵ  ��-1����0�ɹ�
// ˵��    ��1,���жϸ�sockfd�Ƿ��ʺ�connect
//      2,�����sockfd���½��ģ��������˿ںŲ���ӵ������У����򲻱��ƶ�
//      3,������Ӧ��ͨ��Զ�ˣ����úñ��ص�ַ��Զ�˵�ַ���������ص�ͨ������
//      4,��Զ�˷���һ��SYN���ģ�����SYN�ش���ʱ��
//      5,�ȴ����ض�consync�źŵĵ���
// =============================================================================
int Udp_Connect(tagSocket *sockfd, struct sockaddr *serv_addr, int addrlen)
{
	int  result=-1;
	struct sockaddr_in *addrin;
	tagUCB   *ucb;

	if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
	{
		if(EN_SOCKET_LOCAL_CREATE == sockfd->appstate)
		{
			//û�б�BIND,��Ҫ�����·���˿ں�
			sockfd->laddr.port = __Udp_NewPort();
			if(CN_UDP_PORT_INVALID != sockfd->laddr.port)
			{
				//add the socket to the tcp sock queue
				Lock_MutexPend(pgUdpSockQSync, CN_TIMEOUT_FOREVER);
				sockfd->nxt = NULL;
				if(NULL == pgUdpSockQT)
				{
					pgUdpSockQH = sockfd;
				}
				else
				{
					pgUdpSockQT->nxt = sockfd;
				}
				pgUdpSockQT = sockfd;
				Lock_MutexPost(pgUdpSockQSync);
				sockfd->appstate= EN_SOCKET_LOCAL_BIND;
			}
		}
		if(EN_SOCKET_LOCAL_BIND == sockfd->appstate)
		{
			//�������������Ҫ��״̬,����ucb������ͨ��
			ucb = (tagUCB *)__Udp_NewCB();
			if(NULL != ucb)
			{
				sockfd->cb = (void *)ucb;
				sockfd->socktype = EN_SOCKET_TERMINAL;
				ucb->owner = sockfd;
				addrin = (struct sockaddr_in *)serv_addr;
				ucb->raddr.port = ntohs(addrin->sin_port);
				ucb->raddr.ip   = ntohl(addrin->sin_addr.s_addr);
				ucb->channelstate.open = 1;
				ucb->channelstate.arcv = 1;
				ucb->channelstate.asnd = 1;
				sockfd->status =  CN_SOCKET_WRITEALBE;
				Multiplex_Set(sockfd->multiio, sockfd->status);
				result = 0;
			}
		}
		Lock_MutexPost(sockfd->sync);
	}
	return  result;
}

// =============================================================================
// ��������:Udp_Send
//         ���׽��ַ�������
// �������:sockfd,Ŀ���׽���
//        msg,�����͵���Ϣ
//        len,��Ϣ����
//        flags,һ��Ϊ0
// ���������
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��
// =============================================================================
int Udp_Send(tagSocket *sockfd, const void *msg, int len, int flags)
{
	int                result;
	tagUCB             *ucb;

	result = -1;
	if(EN_SOCKET_TERMINAL == sockfd->socktype)//���������򲻱ط�����
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			ucb = (tagUCB *)sockfd->cb;
			result = __Udp_MsgSend(ucb,msg,len,flags);

			Lock_MutexPost(sockfd->sync);
		}
	}

	return  result;
}
// =============================================================================
// �������ܣ�  __Udp_CpyfRcvbuf
//        ��ucb��sbuf��������
// ��������� ucb,������ucb
//        buf,�����͵���Ϣ
//        len,��Ϣ����
// ���������
// ����ֵ  ��-1�������򷵻ؿ������ֽ���
// ˵��    ����ucb�п������ݣ�ֱ��len ==0 ���� ucb��PKGΪ��Ϊֹ
// =============================================================================
int __Udp_CpyfRcvbuf(tagUCB *ucb, void *buf, int len)
{
	int  result;
	int cpylen;
	u8   *cpybuf;
	u8   *srcbuf;
	tagNetPkg  *pkg;

	srcbuf = (u8 *)buf;
	result = 0;
	//���ܻᳬ���أ������ⲻ�ǹؼ�
	//�ȿ�������û��û�����������ݰ����������ǵ�����
	pkg = ucb->rbuf.pkglsthead;
	while((len >0) && (NULL != pkg))
	{
		cpylen = len > pkg->datalen?pkg->datalen:len;
		cpybuf = (u8 *)(pkg->buf + pkg->offset);
		memcpy(srcbuf, cpybuf,cpylen);
		pkg->offset += cpylen;
		pkg->datalen -= cpylen;
		ucb->rbuf.buflen -= cpylen;
		result +=cpylen;
		len-= cpylen;
		srcbuf+=cpylen;
		if(0==pkg->datalen)
		{
			//��PKG�Ѿ������ˣ������ͷ���
			ucb->rbuf.pkglsthead = pkg->partnext;
			if(NULL == ucb->rbuf.pkglsthead)
			{
				ucb->rbuf.pkglsttail = NULL;
			}
			Pkg_PartFlagFree(pkg);
			pkg = ucb->rbuf.pkglsthead;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Udp_RcvMsg
//        rcv msg from the ucb
// ��������� ucb,the ucb
//        buf,the buf to hold the msg
//        len,the buf len
//        flags,then rcv flags
// ���������
// ����ֵ  ��-1�������򷵻ؿ������ֽ���
// ˵��    ����ucb�п������ݣ�ֱ��len ==0 ���� ucb��PKGΪ��Ϊֹ
// =============================================================================
int __Udp_RcvMsg(tagUCB *ucb,void *buf, int len, int flags)
{
	int result;

	result = -1;
	if(ucb->rbuf.buflen > 0)
	{
		//OK, the rcv buf has data to read,so just cpy
		result = __Udp_CpyfRcvbuf(ucb,buf,len);
	}
	else
	{
        if((0 == ucb->channelstate.arcv)||(0 == ucb->channelstate.krcv))
		{
			//the local or the remote has shutdown the rcv channel
			result = 0;
			ucb->owner->errorno = EN_SOCKET_CHANNEL_SHUTDOWN_R;
		}
		else
		{
			//no data current, if block, we will wait here, else return -1
			if(ucb->owner->constate.noblock)
			{
				ucb->owner->errorno = EN_SOCKET_TRY_AGAIN;
			}
			else
			{
				Lock_MutexPost(ucb->owner->sync);
				//block here
				while(0 ==ucb->rbuf.buflen)
				{
					Lock_SempPend(ucb->rbuf.bufsync, CN_TIMEOUT_FOREVER);
				}
				Lock_MutexPend(ucb->owner->sync, CN_TIMEOUT_FOREVER);

				if(ucb->rbuf.buflen > 0)
				{
					result = __Udp_CpyfRcvbuf(ucb,buf,len);
				}
				else
				{
					if((0 == ucb->channelstate.arcv)||(0 == ucb->channelstate.krcv))
					{
						//the channel has been shutdown when we wait
						ucb->owner->errorno = EN_SOCKET_CHANNEL_SHUTDOWN_R;
						result = 0;
					}
				}
			}
		}
	}
    if(result > 0)
	{
        if((ucb->rbuf.buflen < ucb->rbuf.triglevel)&&\
                !((0 == ucb->channelstate.krcv)||\
                (0 == ucb->channelstate.arcv)))
		{
			//could not read any more
			ucb->owner->status &= (~CN_SOCKET_READALBE);
			Multiplex_Set(ucb->owner->multiio, ucb->owner->status);
		}
        if(0< ucb->rbuf.buflen)
		{
        	Lock_SempPost(ucb->rbuf.bufsync);
		}
	}

    return result;
}

// =============================================================================
// �������ܣ�  Udp_Recv
//        ��ȡ��������
// ���������  sockfd,Ŀ���׽���
//        flags,һ��Ϊ0
// ���������
//       buf,�յ�����Ϣ
//       len,buf����
// ����ֵ  ��-1�������򷵻��յ��ֽ���
// ˵��    ��     1,checkpara
//        2,���Ƿ��ʺ�rcv
//        3,�ҵ����ʵ�ͨ��Զ��
//        4,���Ƿ������ݶ�
//        5,���Ƿ��������ͣ��������Ͷ���û�����ݵĻ�ֱ�ӷ��س���
//        6,��ȡ���ݺ�һ���ǵ��ͷ�PKG
//        7,���û���������Ѿ��յ������FIN��ǵ÷���0������-1
//        8,��ʱ����flags��־
// =============================================================================
int Udp_Recv(tagSocket *sockfd, void *buf,int len, unsigned int flags)
{
	int  result;
	tagUCB    *ucb;

	result =-1;
	if(EN_SOCKET_TERMINAL == sockfd->socktype)//���������򲻽���
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			ucb = (tagUCB *)sockfd->cb;
			result = __Udp_RcvMsg(ucb,buf,len,flags);

			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}
// =============================================================================
// �������ܣ�  __Udp_NewTerminal
//        Create an new terminal socket
// ���������iplocal, ipremote,portlocal,portremote
// ���������
// ����ֵ  ��NULL failed else the created socket
// ˵��    ��
// =============================================================================
tagSocket* __Udp_NewTerminal(u32 iplocal,u32 ipremote,u16 portlocal, u16 portremote)
{
	tagSocket     *result;
	tagUCB        *ucb;

	//�½���һ���ͻ���
	result =Socket_New();
	if(NULL != result)
	{
		result->proto = &pgUdpProtoRsc->proto;
		result->appstate = EN_SOCKET_LOCAL_CREATE;
		ucb = __Udp_NewCB();
		if(NULL != ucb)
		{
			result->cb = (void *)ucb;
			result->socktype = EN_SOCKET_TERMINAL;
			ucb->owner = result;
			result->laddr.ip = iplocal;
			result->laddr.port = portlocal;
			ucb->raddr.ip = ipremote;
			ucb->raddr.port = portremote;
			ucb->channelstate.arcv =1;
			ucb->channelstate.asnd = 1;
		}
		else
		{
			Socket_Free(result);
			result = NULL;
		}
	}

	return result;
}

// =============================================================================
// �������ܣ�  Udp_Sendto
//        ��Ŀ�������ַ��������
// ���������  sockfd,     Ŀ���׽���
//        msg,        �����͵���Ϣ
//        len,        ��Ϣ����
//        flags,      һ��Ϊ0
//        addr,       Ŀ�������ַ
//        addrlen,    Ŀ�������ַ����
// ���������
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��
// =============================================================================
int Udp_Sendto(tagSocket *sockfd, const void *msg,int len, unsigned int flags,\
		      const struct sockaddr *addr, int addrlen)
{
	int                  result;
	u32                  ip;
	u16                  port;
	tagUCB               *ucb;
	tagUdpTnode          *tnode;
	struct sockaddr_in   *sockaddrin;
	tagSocket            *sock;
	tagSocket            *socktmp;

	result = -1;
	if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
	{
		sockaddrin = (struct sockaddr_in *)addr;
		ip = ntohl(sockaddrin->sin_addr.s_addr);
		port = ntohs(sockaddrin->sin_port);
		if(EN_SOCKET_TERMINAL == sockfd->socktype)
		{
			//this is an terminal,so we must check whether the addr maps the socket
			ucb = (tagUCB *)sockfd->cb;
			ucb->channelstate.open = 1;
			if((ucb->raddr.ip == ip)&&(ucb->raddr.port == port))
			{
				//this maps the addr, just snd it
				result = __Udp_MsgSend(ucb,msg,len,flags);
			}
		}
		else if(EN_SOCKET_TREENODE == sockfd->socktype)
		{
			//this is an treenode, if ca't find the ternimal, we'd better create it
			sock = NULL;
			tnode = (tagUdpTnode *)sockfd->cb;
			socktmp = tnode->clientlst;
			while(NULL != socktmp)
			{
				ucb = (tagUCB *)socktmp->cb;
				if((ucb->raddr.port == port)&&(ucb->raddr.ip == ip))
				{
					//find the client
					sock = socktmp;
					socktmp =NULL; //end the find
				}
				else
				{
					//chknxt
					socktmp = socktmp->nxt;
				}
			}
			if(NULL == sock)
			{
                //alloc an new socket,and add it to the tnode
				sock = __Udp_NewTerminal(sockfd->laddr.ip,ip,sockfd->laddr.port,port);
				sock->nxt = tnode->clientlst;
				if(NULL != tnode->clientlst)
				{
					tnode->clientlst->pre = sock;
				}
				tnode->clientlst = sock;
			}
			if(NULL != sock)
			{
				if(Lock_MutexPend(sock->sync, CN_TIMEOUT_FOREVER))
				{
					ucb = (tagUCB *)sock->cb;
					result = __Udp_MsgSend(ucb,msg,len,flags);
					Lock_MutexPost(sock->sync);
				}
			}
		}
		else
		{
			//this must be an new create socket
			//1,alloc an cb
			//2,alloc a new port for this local terminal,and add it
			//  to the socket queue
			//3,snd the msg
			ucb = __Udp_NewCB();
			if(NULL != ucb)
			{
				sockfd->cb = (void *)ucb;
				sockfd->socktype = EN_SOCKET_TERMINAL;
				ucb->raddr.ip = ip;
				ucb->raddr.port = port;
				//alloc an new port and add the socket to the socket queue
				if(Lock_MutexPend(pgUdpSockQSync, CN_TIMEOUT_FOREVER))
				{
					port = __Udp_NewPort();
					if(CN_UDP_PORT_INVALID != port)
					{
						sockfd->laddr.port  = port;
						//add the socket to the tcp socket queue
						sockfd->nxt = NULL;
						sockfd->appstate = EN_SOCKET_LOCAL_BIND;
						sockfd->laddr.ip = INADDR_ANY;
						sockfd->laddr.port = port;
						if(NULL ==pgUdpSockQT)
						{
							pgUdpSockQH = sockfd;
							sockfd->pre = NULL;
						}
						else
						{
							pgUdpSockQT->nxt = sockfd;
							sockfd->pre = NULL;
						}
						pgUdpSockQT = sockfd;
					}
					Lock_MutexPost(pgUdpSockQSync);
				}
			}
			if((sockfd->socktype == EN_SOCKET_TERMINAL) &&\
			   (sockfd->appstate == EN_SOCKET_LOCAL_BIND))
			{
				//the cb and port are all created success, then snd
				result = __Udp_MsgSend(ucb,msg,len,flags);
			}
		}
		Lock_MutexPost(sockfd->sync);
	}
	return  result;
}
// =============================================================================
// �������ܣ� Udp_Recvfrom
//           ��Ŀ�������ַ��ȡ����
// ���������  sockfd,Ŀ���׽���
//           flags,һ��Ϊ0
//           addr,Ŀ�������ַ
//           addrlen,Ŀ�������ַ����
// ���������
//           buf,��ȡ������
//           len,��Ϣ����
// ����ֵ  ��-1�������򷵻��յ��ֽ���
// ˵��    ��
// =============================================================================
int Udp_Recvfrom(tagSocket *sockfd,void *buf, int len, unsigned int flags,\
		    struct sockaddr *addr, int *addrlen)
{
	int                  result;
	u32                  ip;
	u16                  port;
	tagUCB               *ucb;
	tagUdpTnode          *tnode;
	struct sockaddr_in   *sockaddrin;
	tagSocket            *sock;
	tagSocket            *socktmp;

	result = -1;
	if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
	{
		sockaddrin = (struct sockaddr_in *)addr;
		ip = ntohl(sockaddrin->sin_addr.s_addr);
		port = ntohs(sockaddrin->sin_port);
		if(EN_SOCKET_TERMINAL == sockfd->socktype)
		{
			//this is an terminal,so we must check whether the addr maps the socket
			ucb = (tagUCB *)sockfd->cb;
			ucb->channelstate.open = 1;
			if((ucb->raddr.ip == ip)&&(ucb->raddr.port == port))
			{
				//this maps the addr, just snd it
				result = __Udp_RcvMsg(ucb,buf,len,flags);
			}
			Lock_MutexPost(sockfd->sync);
		}
		else if(EN_SOCKET_TREENODE == sockfd->socktype)
		{
			//this is an treenode, if ca't find the ternimal, we'd better create it
			sock = NULL;
			tnode = (tagUdpTnode *)sockfd->cb;
			socktmp = tnode->clientlst;
			while(NULL != socktmp)
			{
				ucb = (tagUCB *)socktmp->cb;
				if((ucb->raddr.port == port)&&(ucb->raddr.ip == ip))
				{
					//find the client
					sock = socktmp;
					socktmp =NULL; //end the find
				}
				else
				{
					//chknxt
					socktmp = socktmp->nxt;
				}
			}
			if(NULL == sock)
			{
                //alloc an new socket
				sock = __Udp_NewTerminal(sockfd->laddr.ip,ip,sockfd->laddr.port,port);

				sock->nxt = tnode->clientlst;
				if(NULL != tnode->clientlst)
				{
					tnode->clientlst->pre = sock;
				}
				tnode->clientlst = sock;
			}

			Lock_MutexPost(sockfd->sync);
			if(NULL != sock)
			{
				if(Lock_MutexPend(sock->sync, CN_TIMEOUT_FOREVER))
				{
					ucb = (tagUCB *)sock->cb;
					//snt the msg
					result = __Udp_RcvMsg(ucb,buf,len,flags);
					Lock_MutexPost(sock->sync);
				}
			}
		}
		else
		{
			//this must be an new create socket
			//1,alloc an cb
			//2,alloc a new port for this local terminal,and add it
			//  to the socket queue
			//3,snd the msg
			ucb = __Udp_NewCB();
			if(NULL != ucb)
			{
				sockfd->cb = (void *)ucb;
				sockfd->socktype = EN_SOCKET_TERMINAL;
				ucb->raddr.ip = ip;
				ucb->raddr.port = port;
				//alloc an new port and add the socket to the socket queue
				if(Lock_MutexPend(pgUdpSockQSync, CN_TIMEOUT_FOREVER))
				{
					port = __Udp_NewPort();
					if(CN_UDP_PORT_INVALID != port)
					{
						sockfd->laddr.port  = port;
						//add the socket to the tcp socket queue
						sockfd->nxt = NULL;
						sockfd->appstate = EN_SOCKET_LOCAL_BIND;
						sockfd->laddr.ip = INADDR_ANY;
						sockfd->laddr.port = port;
						if(NULL ==pgUdpSockQT)
						{
							pgUdpSockQH = sockfd;
							sockfd->pre = NULL;
						}
						else
						{
							pgUdpSockQT->nxt = sockfd;
							sockfd->pre = NULL;
						}
						pgUdpSockQT = sockfd;
					}
					Lock_MutexPost(pgUdpSockQSync);
				}
			}
			if((sockfd->socktype == EN_SOCKET_TERMINAL) &&\
			   (sockfd->appstate == EN_SOCKET_LOCAL_BIND))
			{
				//the cb and port are all created success, then snd
				result = __Udp_RcvMsg(ucb,buf,len,flags);
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}
// =============================================================================
// �������ܣ�  __Udp_ShutdownRD
//        shutdown the socked  read channel
// ���������  socket, the socket to be shutdown
// ���������
// ����ֵ  ��0 success while -1 failed
// ˵��    ��
// =============================================================================
int __Udp_ShutdownRD(tagSocket *sock)
{
	int result;
	tagUCB *ucb;

	result = -1;
	if(EN_SOCKET_TERMINAL == sock->socktype)
	{
		ucb = (tagUCB *)sock->cb;
		//clear the rcv buf and recombination queue
		//and set some specified flags at the same time
		ucb->channelstate.arcv = 0;//cant rcv any more
		//release the rcv and recombination queue
		if(NULL != ucb->rbuf.pkglsthead)
		{
		    Pkg_LstFlagFree(ucb->rbuf.pkglsthead);
		}
	    ucb->rbuf.buflen = 0;
	    ucb->rbuf.buflenlimit  = CN_UDP_BUF_LEN_lIMIT;
	    ucb->rbuf.pkglsthead = NULL;
	    ucb->rbuf.pkglsttail = NULL;
	    ucb->rbuf.triglevel = 0;
		Lock_SempPost(ucb->rbuf.bufsync);

		result = 0;
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Udp_ShutdownWR
//        shutdown the socked  snd channel
// ���������  socket, the socket to be shutdown
// ���������
// ����ֵ  ��0 success while -1 failed
// ˵��    ��
// =============================================================================
int __Udp_ShutdownWR(tagSocket *sock)
{
	int result;
	tagUCB *ucb;

	result = -1;
	if(EN_SOCKET_TERMINAL == sock->socktype)
	{
		ucb = (tagUCB *)sock->cb;
		//clear the rcv buf and recombination queue
		//and set some specified flags at the same time
		ucb->channelstate.asnd = 0; //cant snd any more
	}

	return result;
}

// =============================================================================
// �������ܣ�  Udp_Shutdown
//        �ر��׽���
// ���������  sockfd,Ŀ���׽���
//        how,�رշ����� ��SHUT_RD�ȶ���
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��
// =============================================================================
int Udp_Shutdown(tagSocket *sock, u32 how)
{
	int    result;

	if(NULL != sock)
	{
		Lock_MutexPend(sock->sync,CN_TIMEOUT_FOREVER);
		switch(how)
		{
			case SHUT_RD:
				result = __Udp_ShutdownRD(sock);
				break;
			case SHUT_WR:
				result = __Udp_ShutdownWR(sock);
				break;
			case SHUT_RDWR:
				result = __Udp_ShutdownRD(sock);
				if(result)
				{
					result = __Udp_ShutdownWR(sock);
				}
				break;
			default:
				result = -1;
				break;
		}
		Lock_MutexPost(sock->sync);
	}
	return  result;
}


// =============================================================================
// �������ܣ�  Udp_Close
//        �ر��׽���
// ���������  sockfd,���رյ��׽���
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��1, if it is an terminal, then close the channel and set the app state to
//         close, when the tick comes,it will delete the socket
//      2, if it is an treenode, then set the app state to close, and set all the
//         hung socket that not open yet close, when the tick comes, if the treenode
//         with no socket on it, it will delete the treenode
//      3, if it is not sure yet, then set the app state close, when the tick comes
//         it will delete it
// =============================================================================
int Udp_Close(tagSocket *sock)
{
	int                  result;
	tagUCB               *ucb;
	tagUdpTnode          *tnode;
	tagSocket            *cs;

	result = 0;
	if(Lock_MutexPend(sock->sync,CN_TIMEOUT_FOREVER))
	{
		if(EN_SOCKET_TERMINAL == sock->socktype)
		{
			sock->appstate = EN_SOCKET_LOCAL_CLOSE;
			ucb = (tagUCB *)sock->cb;
			ucb->channelstate.arcv = 0;
			ucb->channelstate.asnd = 0;
			ucb->channelstate.close = 1;
			ucb->lifetick = 0;
			//FREE THE RECV QUEUE
			if(NULL != ucb->rbuf.pkglsthead)
			{
			    Pkg_LstFlagFree(ucb->rbuf.pkglsthead);
			}
		    ucb->rbuf.buflen = 0;
		    ucb->rbuf.buflenlimit  = CN_UDP_BUF_LEN_lIMIT;
		    ucb->rbuf.pkglsthead = NULL;
		    ucb->rbuf.pkglsttail = NULL;
		    ucb->rbuf.triglevel = 0;
			Lock_SempPost(ucb->rbuf.bufsync);
		}
		else if(EN_SOCKET_TREENODE == sock->socktype)
		{
			sock->appstate = EN_SOCKET_LOCAL_CLOSE;
			tnode = (tagUdpTnode *)sock->cb;
			cs = tnode->clientlst;
			while(NULL != cs)
			{
				if(Lock_MutexPend(cs->sync,CN_TIMEOUT_FOREVER))
				{
					ucb = (tagUCB *)cs->cb;
					if(0 == ucb->channelstate.open)
					{
						cs->appstate = EN_SOCKET_LOCAL_CLOSE;
						ucb = (tagUCB *)cs->cb;
						ucb->channelstate.arcv = 0;
						ucb->channelstate.asnd = 0;
						ucb->lifetick = 0;
						//FREE THE RECV QUEUE
						if(NULL != ucb->rbuf.pkglsthead)
						{
						    Pkg_LstFlagFree(ucb->rbuf.pkglsthead);
						}
					    ucb->rbuf.buflen = 0;
					    ucb->rbuf.buflenlimit  = CN_UDP_BUF_LEN_lIMIT;
					    ucb->rbuf.pkglsthead = NULL;
					    ucb->rbuf.pkglsttail = NULL;
					    ucb->rbuf.triglevel = 0;
						Lock_SempPost(ucb->rbuf.bufsync);
					}
					Lock_MutexPost(cs->sync);
				}
                cs = cs->nxt;  //chk all the client
			}
		}
		else
		{
			sock->appstate = EN_SOCKET_LOCAL_CLOSE;
		}
		Lock_MutexPost(sock->sync);
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Udp_Sol_Socket
//           SOL_SOCKETѡ���
// ���������  sockfd,��������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int __Udp_Sol_Socket(tagSocket *sockfd,int optname,const void *optval, int optlen)
{
	bool_t result;
	tagUCB *ucb;

	result = -1;
	switch(optname)
	{
		case SO_BROADCAST:
			break;
		case SO_DEBUG:
			break;
		case SO_DONTROUTE:
			break;
		case SO_ERROR:
			break;
		case SO_KEEPALIVE:
			break;
		case SO_LINGER:
			break;
		case SO_OOBINLINE:
			break;
		case SO_RCVBUF:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				ucb = (tagUCB *)sockfd->cb;
				if(*(int *)optval>0)
				{
					ucb->rbuf.buflenlimit = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_SNDBUF:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				ucb = (tagUCB *)sockfd->cb;
				if(*(int *)optval>0)
				{
					ucb->sbuf.buflenlimit = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_RCVLOWAT:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				ucb = (tagUCB *)sockfd->cb;
				if(*(int *)optval>0)
				{
					ucb->rbuf.triglevel = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_SNDLOWAT:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				ucb = (tagUCB *)sockfd->cb;
				if(*(int *)optval>0)
				{
					ucb->sbuf.triglevel = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_RCVTIMEO:
			break;
		case SO_SNDTIMEO:
			break;
		case SO_REUSERADDR:
			break;
		case SO_TYPE:
			break;
		case SO_BSDCOMPAT:
			break;
		case SO_NOBLOCK:
			if(*(int *)optval)
			{
				sockfd->constate.noblock = 1;
			}
			else
			{
				sockfd->constate.noblock = 0;
			}
			result = 0;
			break;
		default:
			break;
	}
	return result;
}
// =============================================================================
// �������ܣ�  __Udp_IpProto_Ip
//           IPPROTO_IPѡ���
// ���������         sockfd,��������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int __Udp_IpProto_Ip(tagSocket *sockfd,int optname,const void *optval, int optlen)
{
	bool_t result;

	result = -1;
	switch(optname)
	{
		case IP_HDRINCL:
			break;
		case IP_OPTIONS:
			break;
		case IP_TOS:
			break;
		case IP_TTL:
			break;
		default:
			break;
	}

	return result;
}
// =============================================================================
// �������ܣ�  Udp_Setsockopt
//           �����׽���ѡ��
// ���������  sockfd,Ŀ���׽���
//           level,���õĲ�Σ�֧��SOL_SOCKET��IPPROTO_TCP��IPPROTO_IP��IPPROTO_IPV6
//           optname,������ѡ��
//           optval,������
//           optlen,����������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int Udp_Setsockopt(tagSocket *sockfd, int level, int optname,\
		       const void *optval, int optlen)
{
	int  result;

	result = -1;
	if((NULL != sockfd)&&(NULL != optval))
	{
		if(Lock_MutexPend(sockfd->sync,CN_TIMEOUT_FOREVER))
		{
			switch(level)
			{
				case SOL_SOCKET:
					result = __Udp_Sol_Socket(sockfd,optname,optval,optlen);
					break;
				case IPPROTO_IP:
					result = __Udp_IpProto_Ip(sockfd,optname,optval,optlen);
					break;
				case IPPROTO_TCP:
					break;
				default:
					//��ʱ��֧�ֵ�ѡ���
					break;
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}
// =============================================================================
// �������ܣ�  Udp_Getsockopt
//           �����׽���ѡ��
// ���������  sockfd,Ŀ���׽���
//           level,���õĲ�����磬֧�ֵĲ����SOL_SOCKET��IPPROTO_TCP
//           optname,������ѡ��
//           optval,������
//           optlen,����������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int Udp_Getsockopt(tagSocket *sockfd, int level, int optname, void *optval,\
		       int *optlen)
{
	int  result = -1;


	return  result;
}
////////////////////////״̬���Ĵ���////////////////////////////////////////////
// =============================================================================
// �������ܣ�  __Udp_GetSocket
//          ����ָ���������׽��֣�ͨ��ʵ�壩
// ���������  iplocal,����IP
//        ipremote,ͨ�Ŷ�IP
//        portlocal,����port
//        portremote,Զ��port
// ���������
// ����ֵ  ������������ͨ��ʵ�壬���û��ͨ��ʵ�壬�򷵻�ͨ�Žڵ㣬����Ҳ�����Ӧ��
//         ͨ�Žڵ㣬�򷵻�NULL
// ˵��    ��IP����ƥ��Ҳ����ͨ�䣬��ָ��ƥ���ʱ��ᾡ���ҵ�ƥ��ģ��Ҳ����Ļ��򷵻�ͨ���
//        ͨ��ļ�IP = INADDR_ANY
//        ���仰����IP��ָ���Ļ������᷵��PORT��ͬ��IP��ͬ���׽���
// =============================================================================
tagSocket *__Udp_GetSocket(u32 iplocal, u32 ipremote, u16 portlocal, u16 portremote)
{
	tagSocket *result = NULL;
	tagSocket *ltmp = NULL;
	tagSocket *rtmp = NULL;
	tagUCB *ucb;
	tagUdpTnode *tnode;

	ltmp = pgUdpSockQH;
	while(NULL != ltmp)
	{
		if(ltmp->laddr.port == portlocal)
		{
			if(EN_SOCKET_TERMINAL==ltmp->socktype)
			{
				//if the remote port and ip match, then we will return it
				//else, return NULL, which means there is no such sock
				ucb = ltmp->cb;
				if((ucb->raddr.port == portremote)&&(ucb->raddr.ip == ipremote))
				{
					result = ltmp;//this must be an local client socket
				}
			}
			else
			{
				//this is an server node, we chk if any sock hang on the ltmp
				//match it, if not, we will return the ltmp
				result = ltmp;
				tnode= (tagUdpTnode *)ltmp->cb;
				rtmp = tnode->clientlst;
				while(NULL != rtmp)
				{
					ucb = (tagUCB *)rtmp->cb;
					if((ucb->raddr.port == portremote)&&(ucb->raddr.ip == ipremote))
					{
						//just match the socket
						result = rtmp;
						rtmp = NULL;
					}
					else
					{
						//chk nxt;
						rtmp = rtmp->nxt;
					}
				}
			}
			//any way , we end the search for the socket
			ltmp = NULL;
		}
		else
		{
			//chk nxt
			ltmp = ltmp->nxt;
		}
	}
	return result;
}

// =============================================================================
// �������ܣ�__Udp_ChksumRcvPkg
//        tcpУ��������
// ��������� iplocal,���ض�IP
//        ipremote,Զ��IP
// ���������
// ����ֵ  ��true У��ͨ�� falseУ��ʧ��
// ˵��    :�յ���TCP������У��
// =============================================================================
bool_t  __Udp_ChksumRcvPkg(u32 iplocal,u32 ipremote, tagNetPkg *pkg)
{
	bool_t  result;
	int     checksumpsedo;
	u16     checksum;
	void    *buf;
	u16     len;
	tagPseudoHdr  psedohdr;

	psedohdr.ipdst = htonl(iplocal);
	psedohdr.ipsrc = htonl(ipremote);
	psedohdr.len = htons(pkg->datalen);
	psedohdr.protocol = CN_IPTPL_UDP;
	psedohdr.zero = 0;

	buf = (void*)(&psedohdr);
	len = sizeof(psedohdr);
	checksumpsedo = Ip_ChksumSoft16(buf, len,0, false);

	buf = (void *)(pkg->buf + pkg->offset);
	len = pkg->datalen;
	checksum = Ip_ChksumSoft16(buf, len, checksumpsedo, true);

	if(0 == checksum)
	{
		result =true;
	}
	else
	{
		result = false;
	}


	return result;
}

// =============================================================================
// �������ܣ�__Udp_DealPkg
//        deal the rcv data
// ��������� ucb, the udp control block
//        pkg, which hold the msg data
// ���������
// ����ֵ  ��true :the pkg has been cached to the buf, else no cached
// ˵��    :
// =============================================================================
bool_t __Udp_DealPkg(tagUCB *ucb, tagNetPkg *pkg)
{
	bool_t result;

	result = false;
	if((ucb->rbuf.buflen < ucb->rbuf.buflenlimit)&&(ucb->channelstate.arcv))
	{
		//ok ,add the data to the rbuf
		pkg->partnext = NULL;
		if(NULL == ucb->rbuf.pkglsttail)
		{
			ucb->rbuf.pkglsthead = pkg;
		}
		else
		{
			ucb->rbuf.pkglsttail->partnext = pkg;
		}
		ucb->rbuf.pkglsttail = pkg;
		ucb->rbuf.buflen+=pkg->datalen;

		if(ucb->rbuf.buflen > ucb->rbuf.triglevel)
		{
			if(0 ==(CN_SOCKET_READALBE & ucb->owner->status))
			{
				ucb->owner->status |= CN_SOCKET_READALBE;
				Multiplex_Set(ucb->owner->multiio, ucb->owner->status);
			}
			Lock_SempPost(ucb->rbuf.bufsync);
		}
		result = true;
	}

	ucb->lifetick = CN_UDP_TERMINAL_LIFE;

	return result;
}
// =============================================================================
// �������ܣ�Udp_RecvProcess
//          TCP���մ�����(����IP�㴫�ݹ��������ݰ�)
// ���������ipsrc,ԴIP
//          ipdst,Ŀ��IP
//          pkg,udp���ݰ���
//          devfunc,�����Ѿ����Ĺ���
// ���������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :deal the pkg rcved for tcp
// =============================================================================
bool_t Udp_RecvProcess(u32 ipsrc, u32 ipdst, tagNetPkg *pkg, u32 devfunc)
{
	bool_t              result ;      //�����ݰ��Ƿ��Ѿ���ӵ��׽�����
	u16                 portdst;
	u16                 portsrc;
	tagUdpHdr           *hdr;         //udp header register
	tagSocket           *ts;           //terminal socket
	tagSocket           *ss;           //socket found
	tagUCB              *ucb;          //udp control block
	tagUdpTnode         *tnode;        //server tree node
	tagNetPkg           *datapkg;      //pkg include the msg but not the hdr
	
	result = false;
	if((INADDR_ANY == ipdst) || (INADDR_ANY == ipsrc) || (NULL == pkg))
	{
		goto __RCVPKG_DEALEND;  //para error
	}
	hdr = (tagUdpHdr *)(pkg->buf + pkg->offset);
	if(0 == (devfunc&CN_IPDEV_UDPICHKSUM))
	{
		Ip_PkgLstChksumPseudo(ipdst, ipsrc, CN_IPTPL_UDP,\
				       pkg,htons(hdr->msglen),&hdr->chksum);
		if(0 != hdr->chksum)
		{
			goto __RCVPKG_DEALEND;  //chksum failed
		}
	}

	if(pkg->datalen > sizeof(tagUdpHdr))
	{
		pkg->datalen -= sizeof(tagUdpHdr);
		datapkg = pkg;
	}
	else
	{
		pkg->datalen = 0;
		datapkg = NULL;
	}
	pkg->offset += sizeof(tagUdpHdr);

	//ok,now we deal the pkg now, the pkg check success
	portdst = ntohs(hdr->portdst);
	portsrc = ntohs(hdr->portsrc);
	//����IP��PORT,������ҵ�ͨ�ŵ��Ǹ�port��
	//���ұ���SOCKET,���ж���״̬�����Ƿ�����ټ�������
	Lock_MutexPend(pgUdpSockQSync, CN_TIMEOUT_FOREVER);
	ss = __Udp_GetSocket(ipdst,ipsrc, portdst,portsrc);
	if((NULL != ss)&&Lock_MutexPend(ss->sync, CN_TIMEOUT_FOREVER))
	{
		Lock_MutexPost(pgUdpSockQSync);
		if((EN_SOCKET_TREENODE == ss->socktype)&&\
		   (EN_SOCKET_LOCAL_LISTEN == ss->appstate))
		{
			//which means that the server has no session with the remote
			//so create one and add it to the tnode
			tnode = (tagUdpTnode *)ss->cb;
			if(tnode->connum < tnode->connumlimit)
			{
                //alloc an new socket,and add it to the tnode
				ts = __Udp_NewTerminal(ipdst,ipsrc,portdst,portsrc);
				if(ts)
				{
					ts->nxt = tnode->clientlst;
					if(NULL != tnode->clientlst)
					{
						tnode->clientlst->pre = ts;
					}
					tnode->clientlst = ts;
					((tagUdpTnode*)ss->cb)->connum2accept++;
					Lock_SempPost(ss->casync);
					Lock_MutexPend(ts->sync, CN_TIMEOUT_FOREVER);
				}
			}
			Lock_MutexPost(ss->sync);
		}
		else if(EN_SOCKET_TERMINAL == ss->socktype)
		{
			ts = ss;  //ok, this is the terminal we searched
		}
		else
		{
			Lock_MutexPost(ss->sync);
		}
		if(ts)
		{
			ucb = (tagUCB*)ts->cb;
			if(NULL != datapkg)
			{
				result =__Udp_DealPkg(ucb, datapkg);
			}
			Lock_MutexPost(ts->sync);
		}
		else
		{
			//which means no local  udp port for this pkg,so icmp cant reached
		}
	}//end for fin an sock to deal the pkg
	else
	{
		Lock_MutexPost(pgUdpSockQSync);
		//which means no local  udp port for this pkg,so icmp cant reached
	}//�������ɴRST����

__RCVPKG_DEALEND:
	if(false == result)//��PKGδ��ӵ��׽��ֶ�����ȥ
	{
		//if the pkg not cached to the buf, then release it
		Pkg_LstFlagFree(pkg);
		result = true;
	}

	return true;
}
// =============================================================================
// �������ܣ�__Udp_DeleteClient
//        ɾ�����ؿͻ���
// ���������sock,
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Udp_DeleteClient(tagSocket *sock)
{
	tagUCB *ucb;
	ucb = (tagUCB *)sock->cb;
	sock = ucb->owner;
	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		pgUdpSockQH = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}
	else
	{
		pgUdpSockQT = sock->pre;
	}
	//reset and free the ucb
	__Udp_RstCB(ucb);
	__Udp_FreeCB(ucb);

	Socket_Free(sock);
}
// =============================================================================
// �������ܣ�__Udp_DeleteServer
//        ɾ�����ط�����
// �����������ɾ����socket
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Udp_DeleteServer(tagSocket *sock)
{

	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		pgUdpSockQH = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}
	else
	{
		pgUdpSockQT = sock->pre;
	}
	//reset and free the tnode
	__Udp_FreeTnode((tagUdpTnode *)sock->cb);

	Lock_SempDelete(sock->casync);
	Socket_Free(sock);
}
// =============================================================================
// �������ܣ�__Udp_DeleteServerSession
//        ɾ�����ط�������һ���Ự
// �����������ɾ���ķ�������һ���Ựucb
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Udp_DeleteServerSession(tagUCB *ucb)
{
	tagSocket *sock;
	tagUdpTnode *tnode;

	sock = ucb->owner;
	tnode = (tagUdpTnode *)sock->hungnode->cb;

	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		tnode->clientlst = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}

	//update the tnode
	tnode->connum--;

	//reset and free the ucb
	__Udp_RstCB(ucb);
	__Udp_FreeCB(ucb);

	Socket_Free(sock);
}

// =============================================================================
// �������ܣ�__Udp_DeleteSockNode
//        ɾ��δ֪��socknode,���ض˵�
// �����������ɾ����socket
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Udp_DeleteSockNode(tagSocket *sock)
{
	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		pgUdpSockQH = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}
	else
	{
		pgUdpSockQT = sock->pre;
	}

	Lock_SempDelete(sock->casync);
	Socket_Free(sock);
}

// =============================================================================
// �������ܣ�__Udp_DealUcbTimer
//        deal the udp timer,which only deal the tmp socket which created for the
//        server session
// ���������ucb, udp control block
// ���������
// ����ֵ  ��true the tcb to del while false not
// ˵��    :
// =============================================================================
bool_t __Udp_DealUcbTimer(tagUCB *ucb)
{
	bool_t result;

	result = false;
	if(0 == ucb->channelstate.open)
	{
		//which not open yet,this is an tmp socket, so del it if timeout
		if((ucb->owner->appstate == EN_SOCKET_LOCAL_CLOSE)||\
			(ucb->lifetick ==0))
		{
			result = true;
		}
		else
		{
			ucb->lifetick--;
		}
	}
	else
	{
		if(1 == ucb->channelstate.close)
		{
			result = true;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�Udp_Tick
//          TCPЭ��Ķ�ʱ���̵߳Ĵ�����
// �����������
// ���������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void Udp_Tick(void)
{
	tagSocket    *sockl1;
	tagSocket    *sockl1nxt;
	bool_t       sockl1del;
	tagSocket    *sockl2;
	tagSocket    *sockl2nxt;
	bool_t       sockl2del;
	tagUdpTnode  *tnode;
	tagUCB       *ucb;
	//this is an never return loop
	while(1)
	{
		Lock_MutexPend(pgUdpSockQSync, CN_TIMEOUT_FOREVER);
		sockl1 = pgUdpSockQH;
		while(NULL != sockl1)
		{
			//l1 layer
			Lock_MutexPend(sockl1->sync, CN_TIMEOUT_FOREVER);
			sockl1del = false;
			sockl1nxt = sockl1->nxt;
			if(EN_SOCKET_TREENODE == sockl1->socktype)
			{
				//this is an tree node, must chk its sub
				tnode = (tagUdpTnode *)sockl1->cb;
				sockl2 = tnode->clientlst;
				while(NULL != sockl2)
				{
					//l2 layer
					Lock_MutexPend(sockl2->sync, CN_TIMEOUT_FOREVER);
					sockl2del = false;
					sockl2nxt = sockl2->nxt;
					//do the server session timer deal
					ucb = (tagUCB *)sockl2->cb;
					sockl2del = __Udp_DealUcbTimer(ucb);

					if(false == sockl2del)
					{
						Lock_MutexPost(sockl2->sync);
					}
					else
					{
						__Udp_DeleteServerSession(ucb);
					}
					sockl2 = sockl2nxt;
				}
				//tree node has no timer except close
				if((sockl1->appstate == EN_SOCKET_LOCAL_CLOSE)&&\
				   (0 == tnode->connum))
				{
					__Udp_DeleteServer(sockl1);
					sockl1del = true;
				}
			}
			else if(EN_SOCKET_TERMINAL == sockl1->socktype)
			{
				//do the client timer deal
				if(sockl1->appstate == EN_SOCKET_LOCAL_CLOSE)
				{
					__Udp_DeleteClient(sockl1);
				}
			}
			else
			{
				//not certain yet, if closed ,then free it
				if(sockl1->appstate == EN_SOCKET_LOCAL_CLOSE)
				{
					__Udp_DeleteSockNode(sockl1);
					sockl1del = true;
				}
			}
			if(false == sockl1del)
			{
				Lock_MutexPost(sockl1->sync);
			}
			sockl1 = sockl1nxt;
		}
		Lock_MutexPost(pgUdpSockQSync);
		//the thread delay for the tcp tick
		Djy_EventDelay(1000*1000*mS);
	}
	return;
}
// =============================================================================
// �������ܣ�Udp_Init
//          TCPЭ���ʼ������
// ���������para,��ʱ������
// ���������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :TCP�ĳ�ʼ���Ķ������������
//       1,����ucb�ڴ��
//       2,������ʱ���߳�
//       3,���������߳�
// =============================================================================
bool_t Udp_Init(ptu32_t para)
{

	u16  evttid;
	tagTlayerProto proto;
	//�����׽�����Դ�ڵ���
	pgUdpSockQSync = Lock_MutexCreate(NULL);
	if(NULL == pgUdpSockQSync)
	{
		printk("Udp:Create Local Mutex Failed\n\r");
		goto createlmutex_failed;
	}
	else
	{
		printk("Udp:Create Local Mutex  Success\n\r");
	}
	//����ucb�ڴ��
	pgUCBPool = Mb_CreatePool((void *)sgUCBPool, CN_UCB_POOLSIZE,\
			                     sizeof(tagUCB), 10, 0, NULL);
	if(NULL == pgUCBPool)
	{
		printk("Udp:Create UdpucbPool Failed\n\r");
		goto createucbpool_failed;
	}
	else
	{
		printk("Udp:Create UdpucbPool  Success\n\r");
	}
	//����TNODE�ڴ��
	pgUdpSessionPool = Mb_CreatePool((void *)sgUdpSessionPool, CN_USESSION_POOLSIZE,\
			                     sizeof(tagUdpTnode), 10, 0, NULL);
	if(NULL == pgUdpSessionPool)
	{
		printk("Udp:Create UdpTnodePool Failed\n\r");
		goto createtnodepool_failed;
	}
	else
	{
		printk("Udp:Create UdpTnodePool  Success\n\r");
	}
	//ע��Udp������㣬�����׽���ʹ��
	proto.socket = Udp_Socket;
	proto.accept = Udp_Accept;
	proto.bind = Udp_Bind;
	proto.connect = Udp_Connect;
	proto.getsockopt = Udp_Getsockopt;
	proto.listen = Udp_Listen;
	proto.recv = Udp_Recv;
	proto.recvfrom = Udp_Recvfrom;
	proto.send = Udp_Send;
	proto.sendto = Udp_Sendto;
	proto.setsockopt = Udp_Setsockopt;
	proto.shutdown = Udp_Shutdown;
	proto.close = Udp_Close;
	proto.multiioadd = NULL;

	pgUdpProtoRsc = TPL_RegisterProto(AF_INET,SOCK_DGRAM,0, &proto);
	if(NULL == pgUdpProtoRsc)
	{
		printk("Udp:Register Udp in TPL Failed\n\r");
		goto resisterUdp_failed;
	}
	else
	{
		printk("Udp:Register Udp in TPL Success\n\r");
	}


	//������ʱ���߳�
    evttid= Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1,
    		(ptu32_t (*)(void))Udp_Tick,NULL, 0x100, "UdpTick");
	if (evttid != CN_EVTT_ID_INVALID)
	{
		evttid = Djy_EventPop(evttid, NULL,0, NULL, NULL, 0);
	}
	else
	{
		printk("Udp:Register UdpTick Failed\n\r");
	}
	return true;

resisterUdp_failed:
	Mb_DeletePool(pgUdpSessionPool);
	pgUdpSessionPool = NULL;

createtnodepool_failed:
	Mb_DeletePool(pgUCBPool);
	pgUCBPool = NULL;

createucbpool_failed:
	Lock_MutexDelete(pgUdpSockQSync);
	pgUdpSockQSync = NULL;

createlmutex_failed:
	return false;
}



