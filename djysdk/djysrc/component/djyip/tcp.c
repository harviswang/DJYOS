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
// ģ������: TCPЭ���ʵ�֣��о���ĵ�Э��
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
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "endian.h"
#include "string.h"

#include "os.h"

#include "rout.h"
#include "tcp.h"
#include "tpl.h"
#include "ipV4.h"


//define for the register in the tpl layer
static tagTplProtoRscNode     *pgTcpProtoRsc;
static struct tagMutexLCB     *pgTcpSockTreeSync;
static tagSocket              *pgTcpSockLstH;            //tcp sock head
static tagSocket              *pgTcpSockLstT;            //tcp sock tail

//used for the port alloc
#define CN_TCP_PORT_TEMPBASE  0x8000
#define CN_TCP_PORT_INVALID   0x0
#define CN_TCP_PORT_LIMIT     0xFFFF
static  u16                   sgTcpPortCurFree = CN_TCP_PORT_TEMPBASE;

//used for manage the tcb mem
#define CN_TCP_TCBSIZE        5  //100�����ӣ�����ˣ����Զ�̬����
static tagTcpTcb              sgTcpTcbPool[CN_TCP_TCBSIZE];
static struct tagMemCellPool  *pgTcpTcbPool = NULL;

#define CN_TCP_TNODESIZE      5
static tagTcpTnode            sgTcpTnodePool[CN_TCP_TNODESIZE];
static struct tagMemCellPool  *pgTcpTnodePool = NULL;

#define CN_TCP_RECOMB_SECNODE 100
static tagTcpReCombNode       sgTcpReCombNodePool[CN_TCP_RECOMB_SECNODE];
static struct tagMemCellPool  *pgTcpReCombNodePool = NULL;
static tagTcpReCombNode       *pgTcpReCombNodeQueueFree = NULL;
static u16                    sgTcpReCombNodeMemFree = 0;
static u16                    sgTcpReCombNodeMemUsed = 0;

//used for the connection to snd syn (server or client)  tcp option
static tagSynOption           sgSynOption;   //when snd syn,use this const option


bool_t __Tcp_Syn2Snd(tagTcpTcb *tcb);
///////////////////////////FUNCTION METHOD//////////////////////////////////////
// =============================================================================
// �������ܣ�__Tcp_GetSysTime
//        ��ȡϵͳʱ��
// �����������
// ���������
// ����ֵ  ��s64 ��ȡϵͳʱ��
// ˵��    :
// =============================================================================
s64 __Tcp_GetSysTime()
{
	return DjyGetTime();
}

#define CN_TCP_UACKNODE_LIMIT 100
static tagTcpSndUnackNode     sgtTcpSndUnackNodeMem[CN_TCP_UACKNODE_LIMIT];
static tagTcpSndUnackNode     *pgTcpUackNodeQ = NULL;
static u32                    sgTcpUackNodeFreeNum;
static struct tagMemCellPool  *pgTcpUnackNodePool = NULL;
// =============================================================================
// �������ܣ�__Tcp_UnackNodeInit
//         Init the UnackNode
// ���������void
// ���������
// ����ֵ  ��void
// ˵��    :
// =============================================================================
void __Tcp_UnackNodeInit(void)
{
	//add all the free ack node to the queue
	pgTcpUackNodeQ = NULL;
	//create the recombination mem
	pgTcpUnackNodePool = Mb_CreatePool((void *)sgtTcpSndUnackNodeMem,\
						CN_TCP_UACKNODE_LIMIT,sizeof(tagTcpSndUnackNode), \
						CN_TCP_UACKNODE_LIMIT, CN_TCP_UACKNODE_LIMIT*100, NULL);
	if(NULL == pgTcpUnackNodePool)
	{
		printk("Tcp:UnackNode Init Failed!\n\r");
	}
	sgTcpUackNodeFreeNum = 0;
}

u32 UackAllocCalltimes = 0;
#define CN_UACKNODE_USEMASKS 0X5555
// =============================================================================
// �������ܣ�__Tcp_AllocUnackedNode
//       used to alloc an node to hold the unacked pkg
// ���������void
// ���������void
// ����ֵ  ��the alloc node, while NULL means failed
// ˵��    :
// =============================================================================
tagTcpSndUnackNode *__Tcp_AllocUnackedNode()
{
	atom_low_t atomop;
	tagTcpSndUnackNode *result;
	result = NULL;
	UackAllocCalltimes++;
	atomop = Int_LowAtomStart();
	if(NULL != pgTcpUackNodeQ)
	{
		result = pgTcpUackNodeQ;
		pgTcpUackNodeQ = result->nxt;
		result->nxt = NULL;
		result->masks = CN_UACKNODE_USEMASKS;
		sgTcpUackNodeFreeNum--;
		Int_LowAtomEnd(atomop);
	}
	else
	{
		Int_LowAtomEnd(atomop);
		result = Mb_Malloc(pgTcpUnackNodePool, CN_TIMEOUT_FOREVER);
		if(NULL != result)
		{
			result->nxt = NULL;
			result->masks = CN_UACKNODE_USEMASKS;
		}
	}
	return result;
}
u32 UackFreeCalltimes = 0;
#define CN_UACKNODE_FREEMASKS 0Xaaaa
// =============================================================================
// �������ܣ�__Tcp_FreeUnackedNode
//       Free the node that hold the unacked pkg
// ���������the node to free
// ���������
// ����ֵ  ��void
// ˵��    :
// =============================================================================
void __Tcp_FreeUnackedNode(tagTcpSndUnackNode *secbuf)
{
	atom_low_t atomop;
	UackFreeCalltimes++;
	//Free the unacked node to the lst
	atomop = Int_LowAtomStart();
	if(NULL != secbuf)
	{
		secbuf->masks = CN_UACKNODE_FREEMASKS;
		secbuf->nxt = pgTcpUackNodeQ;
		pgTcpUackNodeQ = secbuf;
		sgTcpUackNodeFreeNum++;
	}
	Int_LowAtomEnd(atomop);
	return ;
}
// =============================================================================
// �������ܣ�__Tcp_AllocReCombNode
//       used to alloc an node to hold the recombination pkg
// ���������void
// ���������void
// ����ֵ  ��the alloc node, while NULL means failed
// ˵��    :
// =============================================================================
tagTcpReCombNode *__Tcp_AllocReCombNode()
{
	atom_low_t atomop;
	tagTcpReCombNode *result;
	atomop = Int_LowAtomStart();
	if(NULL == pgTcpReCombNodeQueueFree)
	{
		Int_LowAtomEnd(atomop);
		result = Mb_Malloc(pgTcpReCombNodePool, CN_TIMEOUT_FOREVER);
		if(NULL != result)
		{
			sgTcpReCombNodeMemUsed++;
			result->nxt = NULL;
		}
		else
		{
			printk("Tcp:Alloc Recomb Node Failed\n\r");
		}
	}
	else
	{
		result = pgTcpReCombNodeQueueFree;
		pgTcpReCombNodeQueueFree = result->nxt;
		Int_LowAtomEnd(atomop);
		result->nxt = NULL;
		sgTcpReCombNodeMemFree--;
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_FreeReCombNode
//       Free the node that hold the recombination pkg
// ���������the node to free
// ���������
// ����ֵ  ��void
// ˵��    :
// =============================================================================
void __Tcp_FreeReCombNode(tagTcpReCombNode *secbuf)
{
	atom_low_t atomop;
	if(NULL!=secbuf)
	{
		//Free the unacked node to the lst
		atomop = Int_LowAtomStart();
		secbuf->nxt = pgTcpReCombNodeQueueFree;
		pgTcpReCombNodeQueueFree = secbuf;
		Int_LowAtomEnd(atomop);

		sgTcpReCombNodeMemFree++;
	}
	return ;
}
// =============================================================================
// �������ܣ�__Tcp_InitTcb
//       Init the tcb controller
// ���������tcb, the tcb to init
// ���������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void __Tcp_InitTcb(tagTcpTcb *tcb)
{
#define CN_RCV_WIN_DEFAULT     0x1000    //4KB
//���������������ɵ�ISN��
    // ��������ISN�ĺ����
    #define CN_TCP_ISNPRE_MS    (250)
    #define CN_ISN_PER_CONNECT  (64000)
    static u16 sIsnCnt = 0;    // ��������ISN�ľ�̬����
    
    //fill all the space to zeros, then set some data what we need
    memset((void *)tcb, 0, sizeof(tagTcpTcb));

    //set state default
    tcb->state = EN_TCPSTATE_CREATED;            //set the state create
    //set algorithm default
    tcb->algorithm.nagle = 1;                    //open nagle
    //set the channelstate, permit the kernel receive and snd
    tcb->channelstate.krcv = 1;
    tcb->channelstate.ksnd = 1;
    
    //set the snd and receive buf limit to default
    tcb->sbuf.buflenlimit  = CN_TCP_BUF_LEN_lIMIT;
    tcb->rbuf.buflenlimit  = CN_TCP_BUF_LEN_lIMIT;

    //set the local mss to default
    tcb->lmss = CN_TCP_MSS_DEFAULT;   //Ĭ�ϵİ���С

    //generate the first seqno to snd
    tcb->sndnxt = (u32)((sIsnCnt++)*CN_ISN_PER_CONNECT +
            DjyGetTime()*mS*CN_TCP_ISNPRE_MS); //�������ݺ�ֵ,��һ�����һ��ֵ
    tcb->sndnxt = 0;
    //set the tcb window to default
    tcb->rcvwnd = CN_TCP_RCVWIN_DEFAULT;//Ĭ�ϵı��ش��ڵĴ�С
    tcb->sndwnd = 0;
    tcb->sndwndscale = 0;

    //init the congestion window
    tcb->sa = CN_TCP_SA_INIT;
    tcb->sd = CN_TCP_SD_INIT;
    tcb->rto = tcb->sa + tcb->sd*2; //rto init:rto = A+2D
    tcb->cwnd = CN_TCP_MSS_DEFAULT*10;
    tcb->ssthresh = 65535;
}
// =============================================================================
// �������ܣ�__Tcp_RstTcb
//          TCP���ƿ�ĸ�λ
// ���������tcb,����λ��TCB���ƿ�
// ���������
// ����ֵ  ����
// ˵��    :��TCB���ƿ�ָ����������ʱ��ģ��
// =============================================================================
void __Tcp_RstTcb(tagTcpTcb *tcb)
{
	tagTcpReCombNode   *recombnode;
	tagTcpSndUnackNode *unacknode;

	//release the snd buf
	if(NULL != tcb->sbuf.pkglsthead)
	{
	    Pkg_LstFree(tcb->sbuf.pkglsthead);
	}
	tcb->sbuf.buflen = 0;
    tcb->sbuf.buflenlimit  = CN_TCP_BUF_LEN_lIMIT;
    tcb->sbuf.pkglsthead = NULL;
    tcb->sbuf.pkglsttail = NULL;
    tcb->sbuf.triglevel = 0;
	Lock_SempPost(tcb->sbuf.bufsync);
	
	//release all the unacked queue
	unacknode = tcb->unacksechead;
	while(NULL != unacknode)
	{
		tcb->unacksechead = unacknode->nxt;
		Pkg_PartFree(unacknode->lsthead);
		__Tcp_FreeUnackedNode(unacknode);
		unacknode = tcb->unacksechead;
	}
    tcb->unacksechead = NULL;
    tcb->unacksectail = NULL;
	tcb->snduna = tcb->sndnxt;

	//release the receive buf
	if(NULL != tcb->rbuf.pkglsthead)
	{
	    Pkg_LstFlagFree(tcb->rbuf.pkglsthead);
	}
    tcb->rbuf.buflen = 0;
    tcb->rbuf.buflenlimit  = CN_TCP_BUF_LEN_lIMIT;
    tcb->rbuf.pkglsthead = NULL;
    tcb->rbuf.pkglsttail = NULL;
    tcb->rbuf.triglevel = 0;
	Lock_SempPost(tcb->rbuf.bufsync);
	
	//release the recombination queue
	recombnode = tcb->recomblst;
	while(NULL != recombnode)
	{
		tcb->recomblst = recombnode->nxt;
		Pkg_LstFlagFree(recombnode->lsthead);
		__Tcp_FreeReCombNode(recombnode);
		recombnode = tcb->recomblst;
	}
	tcb->recomblst = NULL;
	//״̬�л�
	tcb->channelstate.arcv = 0;
	tcb->channelstate.asnd = 0;
	tcb->channelstate.krcv = 0;
	tcb->channelstate.ksnd = 0;

	if(tcb->channelstate.close)
	{
		tcb->state = EN_TCPSTATE_2FREE;
	}
	else if(tcb->channelstate.open)
	{
		tcb->state = EN_TCPSTATE_CLOSED;
	}
	else
	{
		tcb->state = EN_TCPSTATE_2FREE;
	}
	//clear all the timer
	tcb->timerflag = 0;

	//set the socket invalid, avoiding the hook to snd some invalid msg
	tcb->owner->valid = false;
	
	printk("Tcp:Rst Tcb\n\r");
}

static tagTcpTcb * sgDebugTcb;    //used for tcb
// =============================================================================
// �������ܣ�__Tcp_NewTcb
//          ����һ��TCB���ƿ�
// ���������NULL
// ���������
// ����ֵ  �������TCB���ƿ�
// ˵��    :��Ҫ�ǿռ���䣬�Լ���Դ�ĳ�ʼ��
// =============================================================================
tagTcpTcb *__Tcp_NewTcb(void)
{
	tagTcpTcb *tcb;
	//����TCB�ڴ���Դ
	tcb = Mb_Malloc(pgTcpTcbPool, CN_TIMEOUT_FOREVER);
	if(NULL != tcb)
	{
		//init the tcb member
		__Tcp_InitTcb(tcb);
		//create two semphore for the rcv and sndbuf
		tcb->sbuf.bufsync = Lock_SempCreate(1,1,CN_SEMP_BLOCK_FIFO,NULL);
		if(NULL == tcb->sbuf.bufsync)
		{
			Mb_Free(pgTcpTcbPool,tcb);
			tcb =NULL;
		}
		else
		{
			tcb->rbuf.bufsync = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,NULL);
			if(NULL == tcb->rbuf.bufsync)
			{
				Lock_SempDelete(tcb->sbuf.bufsync);
				Mb_Free(pgTcpTcbPool,tcb);
				tcb =NULL;
			}
			else
			{
				sgDebugTcb = tcb;
			}
		}
	}

	return tcb;
}
// =============================================================================
// �������ܣ�__Tcp_FreeTcb
//          �ͷ�һ��TCB���ƿ�
// ���������NULL
// ���������
// ����ֵ  ��true�ɹ� fasleʧ��
// ˵��    :��Ҫ�ǿռ����
// =============================================================================
bool_t __Tcp_FreeTcb(tagTcpTcb *tcb)
{
	Lock_SempDelete(tcb->rbuf.bufsync);
	Lock_SempDelete(tcb->sbuf.bufsync);
	Mb_Free(pgTcpTcbPool,tcb);
	return true;
}
static tagTcpTnode*  sgDebugTnode; //used fo debug
// =============================================================================
// �������ܣ�__Tcp_NewTnode
//          new a Tnode
// ���������void
// ���������
// ����ֵ  ��tagTcpTnode
// ˵��    :
// =============================================================================
tagTcpTnode* __Tcp_NewTnode(void)
{
	tagTcpTnode *result = NULL;
	
	result =(tagTcpTnode *) Mb_Malloc(pgTcpTnodePool,CN_TIMEOUT_FOREVER);
	if(NULL != result)
	{
		result->connum = 0;
		result->connum2accept = 0;
		result->connumlimit = CN_TCP_LISTEN_DEFAULT;
		result->clientlst = NULL;
		sgDebugTnode = result;
	}
	
	return  result;
}

// =============================================================================
// �������ܣ�__Tcp_FreeTnode
//          free a Tnode
// ���������tnode
// ���������
// ����ֵ  ��true success while false failed
// ˵��    :
// =============================================================================
bool_t __Tcp_FreeTnode(tagTcpTnode* tnode)
{
	Mb_Free(pgTcpTnodePool,(void *)tnode);
	return true;
}
// =============================================================================
// �������ܣ�  Tcp_Socket
//         ����һ���׽���
// ���������  family,��������AF_INET AF_LOCAL��
//        type,���ͣ���SOCK_STREAM SOCK_DGRAM��
//        protocal,Э�����ͣ�һ���Ĭ��Ϊ0
// ���������
// ����ֵ  ���׽��ֺţ�-1��ʾ����ʧ��
// ˵��    ���׽��ִ������ɹ���������ָ��Э��û���ҵ������ڴ治��
//        �������Ӧ����û���κ�����ģ���Ϊ�׽����ǿ�����Щ�������ҵ���Э���
// =============================================================================
tagSocket * Tcp_Socket(int family, int type, int protocal)
{
	tagSocket    *sockfd;

	sockfd = Socket_New();
	if(NULL != sockfd)
	{
		sockfd->casync = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,NULL);
		if(NULL != sockfd->casync)
		{
			sockfd->laddr.port = CN_TCP_PORT_INVALID;//����Ϊ��Ч�˿ںţ����ڶ�β
			sockfd->proto = &pgTcpProtoRsc->proto;
			sockfd->appstate = EN_SOCKET_LOCAL_CREATE;
			//we don't add the socket to the queue now
			//when bind or connect,it will alloc valid port to the socket. then we will
			//add it to the tcp socket queue
		}
		else
		{
			Socket_Free(sockfd);
			sockfd = NULL;
		}
	}
	return sockfd;
}
// =============================================================================
// �������ܣ�  __Tcp_NewPort
//        Find an valid port to use
// ���������  void
// ���������  void
// ����ֵ  ��u16, the valid port to use in tcp, if CN_TCP_PORT_INVALID means could
//       not alloc the port dynamic
// ˵��    ��
// =============================================================================
u16 __Tcp_NewPort()
{
	int chktimes;
	int chktimeslimit;
	tagSocket *sock;
	u16 result = CN_TCP_PORT_INVALID;

	chktimes = 0;
	chktimeslimit = (int)(CN_TCP_PORT_LIMIT - CN_TCP_PORT_TEMPBASE);

	while(chktimes < chktimeslimit)
	{
		sock = pgTcpSockLstH;
		while(NULL != sock)
		{
			if(sgTcpPortCurFree == sock->laddr.port)
			{
				sock =NULL;
			}
			else
			{
				if(NULL == sock->nxt)
				{
					//chk all the sock, and find that the sock is valid to use
					result = sgTcpPortCurFree;
				}
				else
				{
					sock = sock->nxt;
				}
			}
		}
		if(result != CN_TCP_PORT_INVALID)//find the valid port
		{
			//end the chk sume
			chktimes = chktimeslimit;
		}
		//add the free port
		if(sgTcpPortCurFree == CN_TCP_PORT_LIMIT)
		{
			sgTcpPortCurFree = CN_TCP_PORT_TEMPBASE;
		}
		else
		{
			sgTcpPortCurFree++;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Tcp_ChkBindValid
//        to chk wheather we could bind the specified ip and port
// ���������  ip, port
// ���������
// ����ֵ  ��true, valid while false invalid
// ˵��    ��
// =============================================================================
u16 __Tcp_ChkBindValid(u32 ip, u16 port)
{

	bool_t result;
	tagSocket *sock;

	result = true;
	sock = pgTcpSockLstH;
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
// �������ܣ�  Tcp_Bind
//        Ϊһ���׽��ְ�һ�������ַ
// ��������� sockfd,���󶨵��׽���
//        myaddr,ָ���������ַ
//        addrlen,�����ַ����
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�������ַ��ͻ���˿ںţ�
// =============================================================================
int Tcp_Bind(tagSocket *sockfd,struct sockaddr *myaddr, int addrlen)
{
	int  result;
	u32 ip;
	u32 port;
	struct sockaddr_in *sockaddrin;
	
	result = -1;
	if((NULL != sockfd)&& (NULL != myaddr)&&(NULL != pgTcpProtoRsc))
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			if(EN_SOCKET_LOCAL_CREATE == sockfd->appstate)
			{
				sockaddrin = (struct sockaddr_in *)myaddr;
				ip = sockaddrin->sin_addr.s_addr;
				ip = ntohl(ip);
				port = sockaddrin->sin_port;
				port = ntohs(port);
				if(Rout_IsLocalIp(ip))//�Ǳ���IP
				{
					Lock_MutexPend(pgTcpSockTreeSync, CN_TIMEOUT_FOREVER);
					if(CN_TCP_PORT_INVALID == port)
					{
						//alloc an new port and add the socket to the socket queue
						port = __Tcp_NewPort();
						if(port == CN_TCP_PORT_INVALID)
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
						if(__Tcp_ChkBindValid(ip,port))
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
						if(NULL ==pgTcpSockLstT)
						{
							pgTcpSockLstH = sockfd;
							sockfd->pre = NULL;
						}
						else
						{
							pgTcpSockLstT->nxt = sockfd;
							sockfd->pre = NULL;
						}
						pgTcpSockLstT = sockfd;
					}

					Lock_MutexPost(pgTcpSockTreeSync);
				}
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}
// =============================================================================
// �������ܣ�  Tcp_Listen
//        ָ���׽���Ϊ����״̬
// ���������  sockfd,��ָ�����׽���
//        backlog,��������Ӹ�����Ĭ�ϵ�Ϊ5
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�ظ�ָ��
// =============================================================================
int Tcp_Listen(tagSocket *sockfd, int backlog)
{
	int  result;
	tagTcpTnode *tnode;
	
	result = -1;
	if(NULL != sockfd)
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			if(EN_SOCKET_LOCAL_BIND == sockfd->appstate)
			{
				tnode = __Tcp_NewTnode();
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
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}

// =============================================================================
// �������ܣ�__Tcp_AceptNew
//        acceptһ��������
// ���������  sockfd,���������׽���
// ���������
// ����ֵ  �����ӵĿͻ����׽���
// ˵��    ���ú�������������SOCKFD�в���һ���µ�δ��accept������
//        ��������֮��Ŀ�����ʹ�����Լ�ȥ����
// =============================================================================
tagSocket *__Tcp_AceptNew(tagSocket *sockfd)
{

	tagSocket    *result;
	tagSocket    *clientsock;
	tagTcpTcb    *tcb;
	tagTcpTnode  *tnode;

	result = NULL;
	tnode = (tagTcpTnode *)sockfd->cb;
	clientsock = tnode->clientlst;
	if(tnode->connum2accept > 0)
	{
		while(NULL != clientsock)
		{
			tcb = (tagTcpTcb *)clientsock->cb;
			if((0 == (tcb->channelstate.open))&&\
			   (tcb->state == EN_TCPSTATE_ESTABLISHED))
			{
				tcb->channelstate.open = 1;
				tcb->channelstate.arcv = 1;
				tcb->channelstate.asnd = 1;

				clientsock->status = CN_SOCKET_WRITEALBE;
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
// �������ܣ�  Tcp_Accept
//        �ȴ��ͻ��˵�����
// ���������  sockfd,���������׽���
// ���������  addr,���ӵĿͻ��˵�ַ
//           addrlen,��ַ����
// ����ֵ  �����ӵĿͻ����׽���
// ˵��    ������һ������Ϊ����������û�����ӵĿͻ��ˣ����߳�������������
// =============================================================================
tagSocket *Tcp_Accept(tagSocket *sockfd, struct sockaddr *addr, int *addrlen)
{
	tagSocket  	*result;
	tagTcpTcb   *tcb;

	struct sockaddr_in  *addrin;

	result = NULL;
	if(NULL != sockfd)//�������
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			if(EN_SOCKET_LOCAL_LISTEN == sockfd->appstate) //make sure it is a server
			{
				result = __Tcp_AceptNew(sockfd);
				if((NULL == result)&&(0 == sockfd->constate.noblock))
				{
					while(NULL == result)//һֱ�ȴ�
					{
						Lock_MutexPost(sockfd->sync);
						Lock_SempPend(sockfd->casync,CN_TIMEOUT_FOREVER);
						Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER);
						result = __Tcp_AceptNew(sockfd);
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
		tcb = (tagTcpTcb *)result->cb;
		addrin = (struct sockaddr_in *)addr;
		addrin->sin_family = AF_INET;
		addrin->sin_port = htons(tcb->raddr.port);
		addrin->sin_addr.s_addr = htonl(tcb->raddr.ip);
		*addrlen = sizeof(struct sockaddr);
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_MsgSend
//       ����һ��pkg
// ���������tcb,�����͵�tcb���ƿ�
//        flag,tcp��ͷ��־
//        opt,ѡ�����
//        optlen,ѡ���
//        pkgdata,�������ݰ�
//        seqno,���ݰ������к�
// ���������
// ����ֵ  ����
// ˵��    :ֻ���������ݰ�������TCB��Ա�ĸ���
//        true,֤�����ͳɹ���false����ʧ�ܣ���������Ѱ��·��
// =============================================================================
bool_t __Tcp_MsgSend(tagTcpTcb *tcb,u8 flag,u8 *opt, u8 optlen,\
		          tagNetPkg *pkgdata, u32 seqno)
{
	bool_t result;
	tagNetPkg *pkg;
	tagTcpHdr   *hdr;
	tagSocket   *sockfd;
	u16 translen;
	u32 iplocal, ipremote;

	result = false;
	pkg = Pkg_Alloc(sizeof(tagTcpHdr)+optlen,CN_PKGFLAG_FREE);
	if(NULL != pkg)
	{
		hdr = (tagTcpHdr *)(pkg->offset + pkg->buf);
		sockfd = tcb->owner;
		iplocal = sockfd->laddr.ip;
		hdr->portsrc = htons(sockfd->laddr.port);
		ipremote = tcb->raddr.ip;
		hdr->portdst = htons(tcb->raddr.port);
		hdr->ackno = htonl(tcb->rcvnxt);
		hdr->seqno = htonl(seqno);
		hdr->flags = CN_TCP_FLAG_MSK & flag;
		hdr->reserv1 = 0;
		hdr->urgencyp = 0;//���������������
		hdr->window = htons(tcb->rcvwnd);
		//-TODO, this because the rcv buf is full, so we must set this bit
		//       this action should be done in the rcv state,when the app
		//       read the rcvbuf, they will clear this bit and snd the window
		//       changged msg
		if(tcb->rcvwnd < tcb->mss)
		{
			tcb->rcvwndcongest = true;
		}
		hdr->chksum = 0;
		//opt����������ϣ��opt�Ѿ�������˸�ʽ
		if(NULL != opt)
		{
			memcpy(hdr->opt,opt,optlen);
			hdr->hdrlen = (0x05 + (optlen + 3)/4);
		}
		else
		{
			memset(hdr->opt,0,optlen);
			hdr->hdrlen = 0x05;
		}
		//���
		pkg->datalen = sizeof(tagTcpHdr) + optlen;
		translen = pkg->datalen;
		//�����ݰ������pkg�����
		pkg->partnext = pkgdata;
		if(NULL != pkgdata)
		{
			translen += pkgdata->datalen;
		}
		//���͸�IP��
		result = Ip_Send(iplocal, ipremote, pkg,translen,CN_IPTPL_TCP,\
					     CN_IPDEV_TCPOCHKSUM,&hdr->chksum);
	}
	return result;
}
// =============================================================================
// �������ܣ�  Tcp_Connect
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
int Tcp_Connect(tagSocket *sockfd, struct sockaddr *serv_addr, int addrlen) 
{
	int  result=-1;
	struct sockaddr_in *addrin;
	tagTcpTcb   *tcb;
	
	if((NULL != sockfd) &&(NULL != serv_addr)&&(addrlen == sizeof(struct sockaddr_in)))
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			if(EN_SOCKET_LOCAL_CREATE == sockfd->appstate)
			{
				//û�б�BIND,��Ҫ�����·���˿ں�
				sockfd->laddr.port = __Tcp_NewPort();
				if(CN_TCP_PORT_INVALID != sockfd->laddr.port)
				{
					//add the socket to the tcp sock queue
					Lock_MutexPend(pgTcpSockTreeSync, CN_TIMEOUT_FOREVER);
					sockfd->nxt = NULL;
					if(NULL == pgTcpSockLstT)
					{
						pgTcpSockLstH = sockfd;
					}
					else
					{
						pgTcpSockLstT->nxt = sockfd;
					}
					pgTcpSockLstT = sockfd;
					Lock_MutexPost(pgTcpSockTreeSync);
					sockfd->appstate= EN_SOCKET_LOCAL_BIND;
				}
			}
			if(EN_SOCKET_LOCAL_BIND == sockfd->appstate)
			{
				//�������������Ҫ��״̬,����TCB������ͨ��
				tcb = (tagTcpTcb *)__Tcp_NewTcb();
				if(NULL != tcb)
				{
					sockfd->cb = (void *)tcb;
					sockfd->socktype = EN_SOCKET_TERMINAL;
					tcb->owner = sockfd;
					addrin = (struct sockaddr_in *)serv_addr;
					tcb->raddr.port = ntohs(addrin->sin_port);
					tcb->raddr.ip   = ntohl(addrin->sin_addr.s_addr);
					//����SYN����
                    __Tcp_Syn2Snd(tcb);
                    
					Lock_MutexPost(sockfd->sync);
					Lock_SempPend(sockfd->casync, CN_TIMEOUT_FOREVER);//���ȱ��ض����ӳɹ��󼤻�
					
					Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER);
					if(EN_TCPSTATE_ESTABLISHED == tcb->state)//��Ϊ����������Ƕ���RST������
					{
						tcb->channelstate.open = 1;
						tcb->channelstate.arcv = 1;
						tcb->channelstate.asnd = 1;
						sockfd->status =  CN_SOCKET_WRITEALBE;
						Multiplex_Set(sockfd->multiio, sockfd->status);
						result = 0;
					}
				}
			}
			Lock_MutexPost(sockfd->sync);
		}
	}

	return  result;
}
// =============================================================================
// �������ܣ�  __Tcp_Cpy2Sndbuf
//        ��TCB��sbuf��������
// ��������� tcb,������tcb
//        msg,�����͵���Ϣ
//        len,��Ϣ����
// ���������  
// ����ֵ  ��-1�������򷵻ؿ������ֽ���
// ˵��    ����֤���ǵ�ÿ��PKG��������MSS�Ĵ�С���������ò��
// =============================================================================
int __Tcp_Cpy2Sndbuf(tagTcpTcb *tcb, const void *msg, int len)
{
	int  result;
	int cpylen;
	u16 pkglen;
	u8   *cpybuf;
	u8   *srcbuf;
	tagNetPkg  *pkg;
	
	srcbuf = (u8 *)msg;
	result = 0;
	//���ܻᳬ���أ������ⲻ�ǹؼ�
	//�ȿ�������û��û�����������ݰ����������ǵ�����
	if(NULL != tcb->sbuf.pkglsttail)
	{
		pkg = tcb->sbuf.pkglsttail;
		pkglen = (pkg->bufsize-pkg->offset)>tcb->mss?tcb->mss:(pkg->bufsize-pkg->offset);
		cpylen = pkglen-pkg->datalen;
		if(cpylen > 0)
		{
			if(cpylen > len)
			{
				cpylen = len;
			}
			//�����Կ�������Ŀ
			cpybuf = (u8 *)(pkg->buf + \
				pkg->offset + pkg->datalen);
			memcpy(cpybuf, srcbuf,cpylen);
			len -= cpylen;
			srcbuf += cpylen;
			result += cpylen;
			pkg->datalen += cpylen;
			tcb->sbuf.buflen += cpylen;
		}
	}
	//�����û�������������Ҫ���ϵİ��������
	while(len > 0)
	{
		//��Ҫ�����µ����ݰ�����������
		//pkg����mss��׼��
		pkg = Pkg_Alloc(tcb->mss,CN_PKGFLAG_NULL);
		if(NULL == pkg)
		{
			printk("No mem for pkg alloc!\n\r");
			break;//�ڴ���Դ������
		}
		else
		{
			cpylen = tcb->mss>len?len:tcb->mss;
			//�����Կ�������Ŀ
			cpybuf = (u8 *)(pkg->buf + pkg->offset);
			memcpy(cpybuf, srcbuf,cpylen);
			len -= cpylen;
			srcbuf += cpylen;
			result += cpylen;
			pkg->datalen = cpylen;
			tcb->sbuf.buflen += cpylen;
			if(NULL == tcb->sbuf.pkglsttail)
			{
				tcb->sbuf.pkglsthead = pkg;
				tcb->sbuf.pkglsttail = pkg;
			}
			else
			{
				//��pkg���ڶ�β����
				tcb->sbuf.pkglsttail->partnext = pkg;
				tcb->sbuf.pkglsttail = pkg;
			}
		}
	}
	
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_ChkLen2Snd
//       �����Է��͵�������
// ���������tcb,������TCB
// ���������
// ����ֵ  �����Է��͵��ֽ���Ŀ
// ˵��    ����ʾĿǰ�����������»����Է��Ͷ�������
// =============================================================================
int __Tcp_ChkLen2Snd(tagTcpTcb *tcb)
{
	int result;
	int sndwnd;
	int csndwnd;

	result = 0;
	sndwnd = tcb->sndwnd<<tcb->sndwndscale;
	csndwnd = tcb->cwnd;

	if(tcb->algorithm.nagle)
	{
		if(tcb->sbuf.buflen<tcb->mss)
		{
			if(NULL == tcb->unacksechead)
			{
				result = tcb->mss;
			}
		}
		else
		{
			result = csndwnd > sndwnd?sndwnd:csndwnd;
		}
	}
	else
	{
		result = csndwnd > sndwnd?sndwnd:csndwnd;
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_Data2Snd
//        ��������
// ���������tcb,������TCB
// ���������
// ����ֵ  ��true �ɹ�  false ʧ��
// ˵��    :��������÷������е�����
// =============================================================================
bool_t __Tcp_Data2Snd(tagTcpTcb *tcb)
{
	bool_t result;
	bool_t channelbad;
	u8  flag;
	u8  lastflag;  //the last pkg snt flag,maybe reffered to the state change
	u16  sndlen;
	int  sntlen;
	int  sndlenlimit;
	s64  sndtime;
	s64  sectimeout;
	tagNetPkg *pkgdata;
	tagTcpSndUnackNode *secbuf;
	//1,chk how many msg we could snd
	//2,snd the corresponding msg
	//3,update the tcb member when we snd the msg until unsucess or snd it all
	channelbad = false;
	sntlen = 0;
	lastflag = 0;

	sndlenlimit = __Tcp_ChkLen2Snd(tcb);
	sndtime = __Tcp_GetSysTime();
	sectimeout =sndtime +tcb->rto;
	//ok, we now try to snd all the snd buf if we could
	while(sndlenlimit>0)
	{
		//fetch one pkg from the sbuf to snd
		pkgdata = tcb->sbuf.pkglsthead;
		if(NULL == pkgdata)
		{
			break;//if there is no pkg
		}
		tcb->sbuf.pkglsthead = pkgdata->partnext;
		pkgdata->partnext = NULL;
		if(NULL == tcb->sbuf.pkglsthead)
		{
			tcb->sbuf.pkglsttail = NULL;
		}
		sndlen = pkgdata->datalen;
		//�ɼ�Ҫ���͵ı��
		//�����ͱ��
		flag = CN_TCP_FLAG_ACK;
		if(NULL == tcb->sbuf.pkglsthead)//����ôһ��������
		{
			flag |= CN_TCP_FLAG_PSH;//���һ����Ҫ���PSH
			if(1== tcb->channelstate.fin)//��Ҫ����FIN
			{
				flag |= CN_TCP_FLAG_FIN;
				//����FIN�ش���ʱ��
				sndlen +=1;
			}
		}
		//OK,�����ϣ����Է�����
		result = __Tcp_MsgSend(tcb, flag, NULL, 0, pkgdata, tcb->sndnxt);
		if(result)
		{
			//update sntlen
			lastflag = flag;
			//add the pkgdata to the unack queque
			//because we used the timeoutforever,so we must got the buf when we return
			secbuf = __Tcp_AllocUnackedNode();
			pkgdata->partnext = NULL;
			secbuf->lsthead = pkgdata;
			secbuf->datalen = pkgdata->datalen;
//			secbuf->lsttail = pkgdata;
			secbuf->seqno = tcb->sndnxt;
			secbuf->nxt = NULL;
			secbuf->sndtime = sndtime;
			secbuf->timeout =sectimeout;
			secbuf->rsndtimes = 0;
			secbuf->flags = flag;
			if(NULL == tcb->unacksechead)
			{
				tcb->unacksechead = secbuf;
				tcb->unacksectail = secbuf;
			}
			else
			{
				tcb->unacksectail->nxt = secbuf;
				tcb->unacksectail = secbuf;
			}
			//must update when snd success
			tcb->sndnxt += sndlen;
			sndlenlimit-=sndlen;
			sntlen += sndlen;
		}
		else
		{
			//snd the data failed,so readd it to the snd queue
			pkgdata->partnext = tcb->sbuf.pkglsthead;
			tcb->sbuf.pkglsthead = pkgdata;
			if(NULL == tcb->sbuf.pkglsttail)
			{
				tcb->sbuf.pkglsttail = pkgdata;
			}
			channelbad = true;
			break;
		}

	}//end for while
	//see what we have done
	if(sntlen > 0)
	{
		//update some tcb member
		tcb->sbuf.buflen -= sntlen;
		sntlen = sntlen >>tcb->sndwndscale;
		if(tcb->sndwnd > sntlen)
		{
			tcb->sndwnd -= sntlen;
		}
		else
		{
			tcb->sndwnd = 0;
		}
		//we have snt some data
		if((tcb->sbuf.buflenlimit - tcb->sbuf.buflen)>tcb->sbuf.triglevel)
		{
			if(0 == (tcb->owner->status & CN_SOCKET_WRITEALBE))
			{
				tcb->owner->status |= CN_SOCKET_WRITEALBE;
				Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
			}
			Lock_SempPost(tcb->sbuf.bufsync);
		}
		//�����ǣ�������ͳɹ�����Ҫ�����Ƿ���ȥFIN
		if(lastflag & CN_TCP_FLAG_FIN)
		{
			//������FIN��״̬�л�
			if(EN_TCPSTATE_CLOSE_WAIT == tcb->state)
			{
				tcb->state = EN_TCPSTATE_LAST_ACK;
			}
			else
			{
				tcb->state = EN_TCPSTATE_FIN_WAITE1;
			}
		}

        //turn off the ack timer
        tcb->timerflag &= (~CN_TCP_TIMER_ACK);
		//turn on the resnd timer
		tcb->timerflag |= CN_TCP_TIMER_RETRANSMIT;
		//if the sndwindow is zero, then we'd better to open the persist timer
		if((tcb->sndwnd < tcb->mss)||channelbad)
		{
			tcb->timerflag |= CN_TCP_TIMER_PERSIST;
			tcb->persistimer = CN_TCP_TICK_PERSIST;
		}
		result = true;
	}
	else
	{
		//the window or the rout is error, so probe any time
		tcb->persistimer = CN_TCP_TICK_PERSIST;
		tcb->persistimer = CN_TCP_TICK_PERSIST;

		result = false;
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_DataProbe
//        �ᶨ��ʱ����ʱ����
// ���������tcb,������TCB
// ���������
// ����ֵ  ��true �ɹ�  false ʧ��
// ˵��    :when the snd window is zero and we also has some data to send, then we
//      use this function to snd a pkg to avoid the ackno miss
// =============================================================================
bool_t __Tcp_DataProbe(tagTcpTcb *tcb)
{
	bool_t result;
	u8  flag;
	u16  sndlen;
	tagNetPkg *pkgdata;
	tagTcpSndUnackNode *secbuf;

	//fetch one pkg from the sbuf to snd
	pkgdata = tcb->sbuf.pkglsthead;
	tcb->sbuf.pkglsthead = pkgdata->partnext;
	pkgdata->partnext = NULL;
	if(NULL == tcb->sbuf.pkglsthead)
	{
		tcb->sbuf.pkglsttail = NULL;
	}
	sndlen = pkgdata->datalen;
	//�ɼ�Ҫ���͵ı��
	//�����ͱ��
	flag = CN_TCP_FLAG_ACK;
	if(NULL == tcb->sbuf.pkglsthead)//����ôһ��������
	{
		flag |= CN_TCP_FLAG_PSH;//���һ����Ҫ���PSH
		if(1== tcb->channelstate.fin)//��Ҫ����FIN
		{
			flag |= CN_TCP_FLAG_FIN;
			//����FIN�ش���ʱ��
			sndlen +=1;
		}
	}
	//OK,�����ϣ����Է�����
	result = __Tcp_MsgSend(tcb, flag, NULL, 0, pkgdata, tcb->sndnxt);
	if(result)
	{
		//add the pkgdata to the unack queque
		//because we used the timeoutforever,so we must got the buf when we return
		secbuf = __Tcp_AllocUnackedNode();
		pkgdata->partnext = NULL;
		secbuf->lsthead = pkgdata;
		secbuf->datalen = pkgdata->datalen;
//		secbuf->lsttail = pkgdata;
		secbuf->seqno = tcb->sndnxt;
		secbuf->nxt = NULL;
		secbuf->sndtime = __Tcp_GetSysTime();
		secbuf->timeout =secbuf->sndtime +tcb->rto;
		secbuf->rsndtimes = 0;
		secbuf->flags = flag;

		if(NULL == tcb->unacksechead)
		{
			tcb->unacksechead = secbuf;
			tcb->unacksectail = secbuf;
		}
		else
		{
			tcb->unacksectail->nxt = secbuf;
			tcb->unacksectail = secbuf;
		}

		//must update when snd success
		tcb->sndnxt += sndlen;

		//update some tcb member
		tcb->sbuf.buflen -= sndlen;

		//we have snt some data
		if((tcb->sbuf.buflenlimit - tcb->sbuf.buflen)>tcb->sbuf.triglevel)
		{
			if(0 == (tcb->owner->status & CN_SOCKET_WRITEALBE))
			{
				tcb->owner->status |= CN_SOCKET_WRITEALBE;
				Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
			}
			Lock_SempPost(tcb->sbuf.bufsync);
		}
		//�����ǣ�������ͳɹ�����Ҫ�����Ƿ���ȥFIN
		if(flag & CN_TCP_FLAG_FIN)
		{
			//������FIN��״̬�л�
			if(EN_TCPSTATE_CLOSE_WAIT == tcb->state)
			{
				tcb->state = EN_TCPSTATE_LAST_ACK;
			}
			else
			{
				tcb->state = EN_TCPSTATE_FIN_WAITE1;
			}
		}
        //turn off the ack timer
        tcb->timerflag &= (~CN_TCP_TIMER_ACK);
		//turn on the resnd timer
		tcb->timerflag |= CN_TCP_TIMER_RETRANSMIT;
        //turn off the ack timer
        tcb->timerflag &= (~CN_TCP_TIMER_ACK);

		result = true;
	}
	else
	{
		//snd the data failed,so readd it to the snd queue
		pkgdata->partnext = tcb->sbuf.pkglsthead;
		tcb->sbuf.pkglsthead = pkgdata;
		if(NULL == tcb->sbuf.pkglsttail)
		{
			tcb->sbuf.pkglsttail = pkgdata;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�__Tcp_Syn2Snd
//          ��Ҫ����SYN���
// ���������tcb,������TCB
// ���������
// ����ֵ  ��true �ɹ�  false ʧ��
// ˵��    :ר�Ŵ�����SYN������
// =============================================================================
bool_t __Tcp_Syn2Snd(tagTcpTcb *tcb)
{
	bool_t              result;     //the function result
	u8                  flag;       //the snd flags
	tagTcpSndUnackNode  *secbuf;    //used to storage the snd message

	result = false;
	//the server need to snd syn ack while the client just need to snd syn
	flag =CN_TCP_FLAG_SYN;
	if(EN_TCPSTATE_SSYNRCV ==tcb->state)
	{
		flag |= CN_TCP_FLAG_ACK;
	}
	else if(EN_TCPSTATE_CREATED == tcb->state)
	{
		flag = flag;
	}
	else
	{
		return result;        //other state we could never snd the syn flags
	}
	if(__Tcp_MsgSend(tcb, flag, (u8 *)&sgSynOption, \
			         sizeof(sgSynOption), NULL,tcb->sndnxt))
	{
		//alloc the sndbuf to manage the syn resnd
		secbuf = __Tcp_AllocUnackedNode();
		secbuf->lsthead = NULL;
		secbuf->seqno = tcb->sndnxt;
		secbuf->nxt = NULL;
		secbuf->sndtime = __Tcp_GetSysTime();
		secbuf->timeout =secbuf->sndtime +tcb->rto;
		secbuf->rsndtimes = 0;
		secbuf->flags = flag;
		secbuf->datalen = 1;

		//update the tcb members
		tcb->timerflag |= CN_TCP_TIMER_RETRANSMIT;
		tcb->snduna = tcb->sndnxt;
		tcb->sndnxt ++;
		//changge the tcb state
		if(EN_TCPSTATE_CREATED == tcb->state)
		{
			tcb->state = EN_TCPSTATE_CSYNSNT;
		}
		tcb->unacksechead = secbuf;
		tcb->unacksectail = secbuf;
        //turn off the ack timer
        tcb->timerflag &= (~CN_TCP_TIMER_ACK);

		result = true;
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_Fin2Snd
//          ��Ҫ����FIN���
// ���������tcb,������TCB
// ���������
// ����ֵ  ��true �ɹ�  false ʧ��
// ˵��    :ר�Ŵ�����FIN������
// =============================================================================
bool_t __Tcp_Fin2Snd(tagTcpTcb *tcb)
{
	bool_t              result;     //the function result
	u8                  flag;       //the snd flags
	tagTcpSndUnackNode  *secbuf;    //used to storage the snd message

	result = false;
	//����FIN����
	if((tcb->channelstate.fin ==1)&&((tcb->state == EN_TCPSTATE_ESTABLISHED)||\
	   (tcb->state == EN_TCPSTATE_CLOSE_WAIT)))
	{
		flag = CN_TCP_FLAG_FIN|CN_TCP_FLAG_ACK |CN_TCP_FLAG_PSH;
	}
	else
	{
		return result;
	}

	if(__Tcp_MsgSend(tcb, flag,NULL, 0, NULL,tcb->sndnxt))
	{
		//alloc the sndbuf to manage the fin resnd
		secbuf = __Tcp_AllocUnackedNode();
		secbuf->lsthead = NULL;
//		secbuf->lsttail = NULL;
		secbuf->seqno = tcb->sndnxt;
		secbuf->nxt = NULL;
		secbuf->sndtime = __Tcp_GetSysTime();
		secbuf->timeout =secbuf->sndtime +tcb->rto;
		secbuf->rsndtimes = 0;
		secbuf->flags = flag;

		//update the tcb members
		if(NULL == tcb->unacksechead)
		{
			tcb->unacksechead = secbuf;
			tcb->unacksectail = secbuf;
		}
		else
		{
			tcb->unacksectail->nxt = secbuf;
			tcb->unacksectail = secbuf;
		}
		tcb->timerflag |= CN_TCP_TIMER_RETRANSMIT; //turn on the retransmit timer
		tcb->sndnxt ++;

		//״̬�л�
		if(EN_TCPSTATE_CLOSE_WAIT==tcb->state)
		{
			tcb->state = EN_TCPSTATE_LAST_ACK;
		}
		else
		{
			tcb->state = EN_TCPSTATE_FIN_WAITE1;
		}

        //turn off the ack timer
        tcb->timerflag &= (~CN_TCP_TIMER_ACK);;

		result = true;
	}

	return result;
}


// =============================================================================
// �������ܣ�  Tcp_Send
//         ���׽��ַ�������
// ���������  sockfd,Ŀ���׽���
//        msg,�����͵���Ϣ
//        len,��Ϣ����
//        flags,һ��Ϊ0
// ���������
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��    1,�ȼ������Ƿ�Ϸ�
//        2,Ȼ�󿴿���sockfd�Ƿ�������
//        3,�������������ܷ����ɸ�����,������ȴ��������ܹ�����λ�ã���������ֱ�ӷ���
//        4,�ڿ������ݵ�ʱ�������������û�б���д�������ݣ��������������������µ�PKG
//        5,������׽��ֵķ���(��Ҫ������������Ƿ�������)
//        6,��ʱ����flags
//        7,���͵�ʱ���ǰ���һ֡һ֡�ķ��ͣ����ָ������������ϣ��Ҫô��д��ȥ��Ҫô
//          ��д����ȥ,buf��Ϊ��pkg���У��������ż���ĳ���
//        8,��ÿ��TCB���ԣ���sbuf��pkg�����������Ϊ��MSS�Ĵ�С��������Ч�ʻ����
//          �������Ƶ���Ĳ��������ֲ�ɺܶ�С�����ͣ�
// =============================================================================
int Tcp_Send(tagSocket *sockfd, const void *msg, int len, int flags)
{
	int  result;
	tagTcpTcb    *tcb;

	result = -1;

	if(EN_SOCKET_TERMINAL == sockfd->socktype)//���������򲻱ط�����
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			tcb = (tagTcpTcb *)sockfd->cb;
			if(1 == tcb->channelstate.asnd)
			{
				//if the free space of sbuf is bigger than the triglevel,
				//then we will cpy it directly,else wait until the semp
				if(tcb->sbuf.buflenlimit>tcb->sbuf.buflen )
				{
					//enough,we could cpy the buf to the sbuf;
					result = __Tcp_Cpy2Sndbuf(tcb,msg,len);
				}
				else
				{
					//no buf mem for the cpy, if block operate the we will wait
					//else we will return
					//the snd buf is full, so if block we must wait
					if(sockfd->constate.noblock)
					{
						sockfd->errorno = EN_SOCKET_TRY_AGAIN;
					}
					else
					{
						Lock_MutexPost(sockfd->sync);
						//block here to wait
						while((tcb->sbuf.buflenlimit<tcb->sbuf.buflen)&&\
								tcb->channelstate.asnd)
						{
							Lock_SempPend(tcb->sbuf.bufsync, CN_TIMEOUT_FOREVER);
						}
						Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER);
						//if the channel has been shutdown then the write bit will also been
						//set,but we could send any data any more
						if(tcb->channelstate.asnd)
						{
							//we could cpy the buf to the sbuf;
							result = __Tcp_Cpy2Sndbuf(tcb,msg,len);
						}
						else
						{
							//the snd channel has been shut down
							Lock_SempPost(tcb->sbuf.bufsync);
							sockfd->errorno = EN_SOCKET_CHANNEL_SHUTDOWN_W;
						}
					}
				}
			}
			else
			{
				// the snd channel has been shut down
				sockfd->errorno = EN_SOCKET_CHANNEL_SHUTDOWN_W;
			}
			if(result >= 0)
			{
				//we could chk here weather we could snd it to the ip layer
				//if permit, we will snd it directly
				//which means we have snd some thing
				__Tcp_Data2Snd(tcb);
				//if the fin or send is not normal, then will not clean the trigle
				//which means that the channel is shutdown and you could got -1 result
				if((tcb->sbuf.buflenlimit - tcb->sbuf.buflen) <= tcb->sbuf.triglevel)
				{
					sockfd->status&= (~CN_SOCKET_WRITEALBE);
					Multiplex_Set(sockfd->multiio, sockfd->status);
				}
				if(tcb->sbuf.buflenlimit> tcb->sbuf.buflen)
				{
					Lock_SempPost(tcb->sbuf.bufsync);
				}
			}
			Lock_MutexPost(sockfd->sync);
		}
	}

	return  result;
}
// =============================================================================
// �������ܣ�  __Tcp_CpyfRcvbuf
//        ��TCB��sbuf��������
// ��������� tcb,������tcb
//        buf,�����͵���Ϣ
//        len,��Ϣ����
// ���������
// ����ֵ  ��-1�������򷵻ؿ������ֽ���
// ˵��    ����TCB�п������ݣ�ֱ��len ==0 ���� tcb��PKGΪ��Ϊֹ
// =============================================================================
int __Tcp_CpyfRcvbuf(tagTcpTcb *tcb, void *buf, int len)
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
	pkg = tcb->rbuf.pkglsthead;
	while((len >0) && (NULL != pkg))
	{
		cpylen = len > pkg->datalen?pkg->datalen:len;
		cpybuf = (u8 *)(pkg->buf + pkg->offset);
		memcpy(srcbuf, cpybuf,cpylen);
		pkg->offset += cpylen;
		pkg->datalen -= cpylen;
		tcb->rbuf.buflen -= cpylen;
		result +=cpylen;
		len-= cpylen;
		srcbuf+=cpylen;
		if(0==pkg->datalen)
		{
			//��PKG�Ѿ������ˣ������ͷ���
			tcb->rbuf.pkglsthead = pkg->partnext;
			if(NULL == tcb->rbuf.pkglsthead)
			{
				tcb->rbuf.pkglsttail = NULL;
			}
			Pkg_PartFlagFree(pkg);
			pkg = tcb->rbuf.pkglsthead;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�  Tcp_Recv
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
int Tcp_Recv(tagSocket *sockfd, void *buf,int len, unsigned int flags)
{
	int  result;
	tagTcpTcb    *tcb;

	result = -1;
	if(EN_SOCKET_TERMINAL == sockfd->socktype)//���������򲻽���
	{
		if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
		{
			tcb = (tagTcpTcb *)sockfd->cb;
			if(tcb->rbuf.buflen > 0)
			{
				//OK, the rcv buf has data to read,so just cpy
				result = __Tcp_CpyfRcvbuf(tcb,buf,len);
			}
			else
			{
                if((0 == tcb->channelstate.arcv)||(0 == tcb->channelstate.krcv))
				{
					//the local or the remote has shutdown the rcv channel
					result = 0;
					sockfd->errorno = EN_SOCKET_CHANNEL_SHUTDOWN_R;
				}
				else
				{
					//no data current, if block, we will wait here, else return -1
					if(sockfd->constate.noblock)
					{
						sockfd->errorno = EN_SOCKET_TRY_AGAIN;
					}
					else
					{
						Lock_MutexPost(sockfd->sync);
						//block here
						while(0 ==tcb->rbuf.buflen)
						{
							Lock_SempPend(tcb->rbuf.bufsync, CN_TIMEOUT_FOREVER);
						}
						Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER);

						if(tcb->rbuf.buflen > 0)
						{
							result = __Tcp_CpyfRcvbuf(tcb,buf,len);
						}
						else
						{
							if((0 == tcb->channelstate.arcv)||(0 == tcb->channelstate.krcv))
							{
								//the channel has been shutdown when we wait
								sockfd->errorno = EN_SOCKET_CHANNEL_SHUTDOWN_R;
								result = 0;
							}
						}
					}
				}
			}
            if(result > 0)
			{
                if((tcb->rbuf.buflen < tcb->rbuf.triglevel)&&\
                        !((0 == tcb->channelstate.krcv)||\
                        (0 == tcb->channelstate.arcv)))
				{
					//could not read any more
					sockfd->status &= (~CN_SOCKET_READALBE);
					Multiplex_Set(sockfd->multiio, sockfd->status);
				}
                if(0< tcb->rbuf.buflen)
				{
                	Lock_SempPost(tcb->rbuf.bufsync);
					//not read any more
				}
                //changed the rcv window
                tcb->rcvwnd += result;
                //if the data is zero, we could snd the window
                if((true == tcb->rcvwndcongest)&&(tcb->rcvwnd<tcb->mss))
                {
                	__Tcp_MsgSend(tcb, CN_TCP_FLAG_ACK, NULL, 0, NULL, tcb->sndnxt);
                	tcb->rcvwndcongest = false;
                }
			}
			Lock_MutexPost(sockfd->sync);
		}
	}
	return  result;
}

// =============================================================================
// �������ܣ�  __Tcp_ShutdownRD
//        shutdown the socked  read channel
// ���������  socket, the socket to be shutdown
// ���������
// ����ֵ  ��0 success while -1 failed
// ˵��    ��
// =============================================================================
int __Tcp_ShutdownRD(tagSocket *sock)
{
	int result;
	tagTcpTcb *tcb;
	tagTcpReCombNode *rcnode;

	result = -1;
	if(EN_SOCKET_TERMINAL == sock->socktype)
	{
		tcb = (tagTcpTcb *)sock->cb;
		//clear the rcv buf and recombination queue
		//and set some specified flags at the same time
		tcb->channelstate.arcv = 0;//cant rcv any more
		tcb->channelstate.krcv = 0;//ignaore any reached data
		//release the rcv and recombination queue
		if(NULL != tcb->rbuf.pkglsthead)
		{
		    Pkg_LstFlagFree(tcb->rbuf.pkglsthead);
		}
	    tcb->rbuf.buflen = 0;
	    tcb->rbuf.buflenlimit  = CN_TCP_BUF_LEN_lIMIT;
	    tcb->rbuf.pkglsthead = NULL;
	    tcb->rbuf.pkglsttail = NULL;
	    tcb->rbuf.triglevel = 0;
		Lock_SempPost(tcb->rbuf.bufsync);

		//�ͷ����е���������
		rcnode = tcb->recomblst;
		while(NULL != rcnode)
		{
			tcb->recomblst = rcnode->nxt;
			if(NULL != rcnode->lsthead)
			{
				Pkg_LstFlagFree(rcnode->lsthead);
			}
			__Tcp_FreeReCombNode(rcnode);

			rcnode = tcb->recomblst;
		}
		tcb->recomblst = NULL;
		result = 0;
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Tcp_ShutdownWR
//        shutdown the socked  snd channel
// ���������  socket, the socket to be shutdown
// ���������
// ����ֵ  ��0 success while -1 failed
// ˵��    ��
// =============================================================================
int __Tcp_ShutdownWR(tagSocket *sock)
{
	int result;
	tagTcpTcb *tcb;

	result = -1;
	if(EN_SOCKET_TERMINAL == sock->socktype)
	{
		tcb = (tagTcpTcb *)sock->cb;
		//clear the rcv buf and recombination queue
		//and set some specified flags at the same time
		tcb->channelstate.asnd = 0;//cant snd any more
		                           //app cant snd, but the kernel could snd
		//release the rcv and recombination queue
		if(0 == tcb->channelstate.fin)
		{
			tcb->channelstate.fin = 1;
			//just snd msg,let the snd service to snd the fin
			if(NULL == tcb->sbuf.pkglsthead)
			{
                __Tcp_Fin2Snd(tcb);
			}
			result = 0;
		}
	}

	return result;
}

// =============================================================================
// �������ܣ�  Tcp_Shutdown
//        �ر��׽���
// ���������  sockfd,Ŀ���׽���
//        how,�رշ����� ��SHUT_RD�ȶ���
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��
// =============================================================================
int Tcp_Shutdown(tagSocket *sock, u32 how)
{
	int    result;

	if(NULL != sock)
	{
		Lock_MutexPend(sock->sync,CN_TIMEOUT_FOREVER);
		switch(how)
		{
			case SHUT_RD:
				result = __Tcp_ShutdownRD(sock);
				break;
			case SHUT_WR:
				result = __Tcp_ShutdownWR(sock);
				break;
			case SHUT_RDWR:
				result = __Tcp_ShutdownRD(sock);
				if(result)
				{
					result = __Tcp_ShutdownWR(sock);
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
// �������ܣ�  Tcp_Close
//           �׽��ֹر�
// ���������  sockfd,���رյ��׽���
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��1,�ô����У�ɾ��������������ɾ��,�������״̬��������ɾ��������TCP TICK��
//        ����TICKɨ�跢��2FREE������ɾ��
//        ���ֱ��ض�ΪCLOSED�ģ����û��Զ�˹������£�Ҳ��ɾ��,�����Զ�����Ƚ����Զ��
//      2,close�ķ���FIN����RST(ͨ��Զ��)��linger�رգ���ֱ�ӷ���RST;linger�򿪣�
//        ������ջ�����������������RST������ִ���Ĵ����ֹر�
// =============================================================================
int Tcp_Close(tagSocket *sock)
{
	int    result;
	tagTcpTcb   *tcb;

	result = 0;

	if(Lock_MutexPend(sock->sync,CN_TIMEOUT_FOREVER))
	{
		if(EN_SOCKET_TERMINAL == sock->socktype)
		{
			//it is an communicate point
			tcb = (tagTcpTcb *)sock->cb;
			if(EN_TCPSTATE_CLOSED == tcb->state)
			{
				tcb->state = EN_TCPSTATE_2FREE;
			}
			else
			{
				if(tcb->algorithm.linger)
				{
					if(0 == tcb->rbuf.buflen)
					{
						//if the buf is empty, then we will wait the
						//the socket to disconnect by fin
						tcb->channelstate.arcv= 0;
						tcb->channelstate.asnd = 0;
						tcb->channelstate.krcv = 0;
						tcb->channelstate.close = 1;
						tcb->timerflag |=CN_TCP_TIMER_LINGER;
						//snd msg to make the sndservice to snd the fin
						//release the rcv and recombination queue
						if(0 == tcb->channelstate.fin)
						{
							tcb->channelstate.fin = 1;
							//just snd msg,let the snd service to snd the fin
							if(NULL == tcb->sbuf.pkglsthead)
							{
                                __Tcp_Fin2Snd(tcb);
							}
							result = 0;
						}
					}
					else
					{
						//���ջ����������ݣ�ִ��RST�ر�
						printk("CLose the socket!\n\r");
						__Tcp_RstTcb(tcb);
						tcb->channelstate.close = 1;
						tcb->state = EN_TCPSTATE_2FREE;
					}
				}
				else
				{
					//��ʱû�д�,ִ��RST�ر�
					printk("CLose the socket!\n\r");
					__Tcp_RstTcb(tcb);
					tcb->channelstate.close = 1;
					tcb->state = EN_TCPSTATE_2FREE;
				}
			}
		}
		else
		{
			//if it is the server or not defined yet, then just set the close bit
			//if no communicate is exist, then in tcp tick process will free it
			sock->appstate = EN_SOCKET_LOCAL_CLOSE;
		}
		Lock_MutexPost(sock->sync);
	}

	return result;
}
// =============================================================================
// �������ܣ�  __Tcp_Sol_Socket
//           SOL_SOCKETѡ���
// ���������  sockfd,��������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int __Tcp_Sol_Socket(tagSocket *sockfd,int optname,const void *optval, int optlen)
{
	bool_t result;
	tagTcpTcb *tcb;
	struct linger      *mylinger;

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
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				//Զ�˵Ļ�����һ����OK��
				tcb = (tagTcpTcb *)sockfd->cb;
				if(*(int *)optval)
				{
					tcb->timerflag |= CN_TCP_TIMER_KEEPALIVE;
					tcb->keeptimer = CN_TCP_TICK_KEEPALIVE;
				}
				else
				{
					tcb->timerflag &= (~CN_TCP_TIMER_KEEPALIVE);
				}
				result = 0;
			}
			break;
		case SO_LINGER:
			if((EN_SOCKET_TERMINAL == sockfd->socktype)&&\
					(optlen == sizeof(struct linger)))
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				mylinger  = (struct linger *)optval;
				if(mylinger->l_onff)
				{
					tcb->algorithm.linger = 1;
					tcb->lingertimer = mylinger->l_linger*1000/CN_TCP_TICK_TIME;
				}
				else
				{
					tcb->algorithm.linger = 0;
				}
				result = 0;
			}
			break;
		case SO_OOBINLINE:
			break;
		case SO_RCVBUF:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				if(*(int *)optval>0)
				{
					tcb->rbuf.buflenlimit = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_SNDBUF:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				if(*(int *)optval>0)
				{
					tcb->sbuf.buflenlimit = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_RCVLOWAT:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				if(*(int *)optval>0)
				{
					tcb->rbuf.triglevel = *(int *)optval;
					result = 0;
				}
			}
			break;
		case SO_SNDLOWAT:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				if(*(int *)optval>0)
				{
					tcb->sbuf.triglevel = *(int *)optval;
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
// �������ܣ�  __Tcp_IpProto_Ip
//           IPPROTO_IPѡ���
// ���������         sockfd,��������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int __Tcp_IpProto_Ip(tagSocket *sockfd,int optname,const void *optval, int optlen)
{
	bool_t result;
	tagTcpTcb *tcb;

	result = -1;
	switch(optname)
	{
		case IP_HDRINCL:
			break;
		case IP_OPTIONS:
			break;
		case IP_TOS:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				//Զ�˵Ļ�����һ����OK��
				tcb = (tagTcpTcb *)sockfd->cb;
				tcb->ipoption.tos = *(u8 *)optval;
				result = 0;
			}
			break;
		case IP_TTL:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				tcb->ipoption.ttl = *(u8 *)optval;
				result = 0;
			}
			break;
		default:
			break;
	}
	
	return result;
}
// =============================================================================
// �������ܣ�  __Tcp_IpProto_Tcp
//           IPPROTO_TCPѡ���
// ���������  sockfd,��������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int __Tcp_IpProto_Tcp(tagSocket *sockfd,int optname,const void *optval, int optlen)
{
	bool_t result;
	tagTcpTcb *tcb;

	result = -1;
	switch(optname)
	{
		case TCP_MAXSEG:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				tcb->lmss = *(u8 *)optval;
				result = 0;
			}
			break;
		case TCP_NODELAY:
			if(EN_SOCKET_TERMINAL == sockfd->socktype)
			{
				tcb = (tagTcpTcb *)sockfd->cb;
				if(*(int *)optval)
				{
					tcb->algorithm.nagle = 0;
				}
				else
				{
					tcb->algorithm.nagle = 1;
				}
				result = 0;
			}
			break;
		default:
			break;
	}
	
	return result;
}
// =============================================================================
// �������ܣ�  Tcp_Setsockopt
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
int Tcp_Setsockopt(tagSocket *sockfd, int level, int optname,\
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
					result = __Tcp_Sol_Socket(sockfd,optname,optval,optlen);
					break;
				case IPPROTO_IP:
					result = __Tcp_IpProto_Ip(sockfd,optname,optval,optlen);
					break;
				case IPPROTO_TCP:
					result = __Tcp_IpProto_Tcp(sockfd,optname,optval,optlen);
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
// �������ܣ�  Tcp_Getsockopt
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
int Tcp_Getsockopt(tagSocket *sockfd, int level, int optname, void *optval,\
		       int *optlen)
{
	int  result = -1;


	return  result;
}
// =============================================================================
// �������ܣ�  Tcp_MultiIoAdd
//           used to add an tcp socket to the sets
// ���������  sets,which represent the target  sets we interested
//           objectid, which represents the object who trigger the sets
//           sendingBit, the bits that we are interested
//           sockfd, the socket we need to monitor
// ���������
// ����ֵ  ��if success returns 0 else -1
// ˵��    ��
// =============================================================================
bool_t Tcp_MultiIoAdd(struct tagMultiplexSetsCB  *Sets,\
        ptu32_t ObjectID, u32 SensingBit,tagSocket *sockfd)
{
	bool_t result;
	u32  initstatus;

	result = false;
	if(Lock_MutexPend(sockfd->sync, CN_TIMEOUT_FOREVER))
	{

		initstatus = sockfd->status;
		result = Multiplex_AddObject(Sets,&sockfd->multiio,initstatus,\
				                     ObjectID,SensingBit);
		Lock_MutexPost(sockfd->sync);
	}

	return result;
}

////////////////////////״̬���Ĵ���////////////////////////////////////////////
// =============================================================================
// �������ܣ�  __Tcp_GetSocket
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
tagSocket *__Tcp_GetSocket(u32 iplocal, u32 ipremote, u16 portlocal, u16 portremote)
{
	tagSocket *result = NULL;
	tagSocket *ltmp = NULL;
	tagSocket *rtmp = NULL;
	tagTcpTcb *tcb;
	tagTcpTnode *tnode;

	ltmp = pgTcpSockLstH;
	while(NULL != ltmp)
	{
		if(ltmp->laddr.port == portlocal)
		{
			if(EN_SOCKET_TERMINAL==ltmp->socktype)
			{
				//if the remote port and ip match, then we will return it
				//else, return NULL, which means there is no such sock
				tcb = ltmp->cb;
				if((tcb->raddr.port == portremote)&&(tcb->raddr.ip == ipremote))
				{
					result = ltmp;//this must be an local client socket
				}
			}
			else
			{
				//this is an server node, we chk if any sock hang on the ltmp
				//match it, if not, we will return the ltmp
				result = ltmp;
				tnode= (tagTcpTnode *)ltmp->cb;
				rtmp = tnode->clientlst;
				while(NULL != rtmp)
				{
					tcb = (tagTcpTcb *)rtmp->cb;
					if((tcb->raddr.port == portremote)&&(tcb->raddr.ip == ipremote))
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
// �������ܣ�__Tcp_ChksumRcvPkg
//        tcpУ��������
// ��������� iplocal,���ض�IP
//        ipremote,Զ��IP
// ���������
// ����ֵ  ��true У��ͨ�� falseУ��ʧ��
// ˵��    :�յ���TCP������У��
// =============================================================================
bool_t  __Tcp_ChksumRcvPkg(u32 iplocal,u32 ipremote, tagNetPkg *pkglst)
{
	bool_t  result;
	u16     chksum;
	u16     translen;
	tagNetPkg *pkg;
	translen = 0;
	pkg = pkglst;
	while(pkg != NULL)
	{
		translen += pkg->datalen;
		pkg = pkg->partnext;
	}
	Ip_PkgLstChksumPseudo(iplocal, ipremote, CN_IPTPL_TCP,\
			             pkglst,translen,&chksum);
	if(0 == chksum)
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
// �������ܣ�__Tcp_CheckTcpHdr
//          ���TCPͷ�Ƿ�Ϸ�
// ���������hdr,������TCPͷ
// ���������
// ����ֵ  ��true �Ϸ� false �Ƿ�
// ˵��    :ͨ����TCPͷ�ı�ǵļ�⣬����ֹ���⹥��������������
// =============================================================================
bool_t  __Tcp_CheckTcpHdr(tagTcpHdr *hdr)
{
	bool_t result;
	u8     flag;
	flag = hdr->flags & CN_TCP_FLAG_MSK;
	result = true;
	if((flag &CN_TCP_FLAG_SYN)&& (flag &CN_TCP_FLAG_FIN))
	{
		result = false;//SYN FIN ��
	}
	else if((flag &CN_TCP_FLAG_SYN)&& (flag &CN_TCP_FLAG_RST))
	{
		result = false;//SYN RST��
	}
	else if((flag &CN_TCP_FLAG_RST)&&(0 ==(flag & CN_TCP_FLAG_ACK)))//��RST ������ACK
	{
		result = false;
	}
	else if((flag &CN_TCP_FLAG_FIN)&&(0 == (flag & CN_TCP_FLAG_ACK)))
	{
		result = false;
	}
	else if((flag &CN_TCP_FLAG_SYN)&&(hdr->hdrlen <=0x05)) //SYN���ı���Ҫ��ѡ��
	{
		result = false;
	}
	else if(0 == flag)  
	{
		result = false;//NULL��ǰ���һ���ACK��Ϊ0���Ѱ���Ӧ�ñ��ack
	}
	else
	{
		result = true;
	}
	
	return result;
}

// =============================================================================
// �������ܣ�__Tcp_RstRemoteRaw
//          �����Զ˵��׽���
// ��������� iplocal,���ض�IP
//        portlocal,���ض˶˿�
//        ipremote,Զ��IP
//        portremote,Զ��port
//        ackno, �������ڶԷ��ķ��ʹ��ڣ�����Զ�
// ���������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :��TCPЭ���յ������ı���ʱ����Ȼ����ô���������Ҳ�����Ӧ�Ķ˿ں�
//       �Ҳ���Ϊ�����ķ����������ʱ���׽��ֽ�������������Ȼ������TCB������ֻ��Ҫ
//       �ظ���һ�����TCPͷ�㱨�ļ���
//       ֱ�Ӵ��һ��TCPͷ���ķ��ͳ�ȥ����
//       ��Ҫע�����RST����Ҳ��ҪACK�ţ���ACK��Ӧ��Ϊ�����߷��͹��������
// =============================================================================
bool_t __Tcp_RstRemoteRaw(u32 iplocal, u16 portlocal,u32 ipremote, u16 portremote,u32 ackno)
{
	bool_t result ;	
	tagNetPkg *pkg;
	tagTcpHdr   *tcphdr;
	u16 translen;
	
	result = false;
	pkg = Pkg_Alloc(sizeof(tagTcpHdr),CN_PKGFLAG_FREE);
	if(NULL != pkg)
	{
		tcphdr = (tagTcpHdr *)(pkg->offset + pkg->buf);
		tcphdr->chksum = 0;
		tcphdr->flags = CN_TCP_FLAG_RST|CN_TCP_FLAG_ACK;
		tcphdr->hdrlen = 0x05;
		tcphdr->portdst = htons(portremote);
		tcphdr->portsrc = htons(portlocal);
		tcphdr->ackno  = htonl(ackno);
		tcphdr->seqno = 0;
		tcphdr->urgencyp = 0;	
		pkg->datalen = sizeof(tagTcpHdr);
		translen = pkg->datalen;
		
		result = Ip_Send(iplocal, ipremote, pkg,translen,CN_IPTPL_TCP,\
				 CN_IPDEV_TCPOCHKSUM,&tcphdr->chksum);
		if(false == result)
		{
			Pkg_PartFree(pkg);
		}		
	}
	return result;
}

enum __EN_TCP_OPT_CODE
{
	EN_TCP_OPT_END = 0,
	EN_TCP_OPT_NONE,
	EN_TCP_OPT_MSS ,
	EN_TCP_OPT_SCALE,
	EN_TCP_OPT_SACKP,
	EN_TCP_OPT_SACKR,
	EN_TCP_OPT_TAMP = 8,
};
// =============================================================================
// �������ܣ�__Tcp_OptDeal
//        ͨ��TCP����ͷ��ѡ��������TCB��һЩ����
// ���������void *opt,ѡ��ָ��
//        u8   len, ѡ���
//        bool_t  syn, �����Ƿ����syn��־
// ���������tcb,�����µ�tcb
// ����ֵ  ����
// ˵��    :�����ѡ�����MSS SACK������ѡ����ʱ��֧��
// =============================================================================
void __Tcp_OptDeal(tagTcpTcb *tcb, tagTcpHdr *hdr)
{
	u8  totallen;
	u8  optlen;
	u8  kind;
	u8  data8;
	u16 data16;
	u8  *src;
	u8  *optsrc;

	totallen = (hdr->hdrlen -0x05)*4;
	src = (u8 *)hdr->opt;
	while(totallen > 0)
	{
	    optsrc = src;
		kind = *optsrc++;
		switch(kind)
		{
			case EN_TCP_OPT_END:      //OPTION END
				printk("%s:kind = %d\n\r",\
					   __FUNCTION__,kind);
				totallen = 0;
				break;
			case EN_TCP_OPT_NONE:     //OPTION NONE
				printk("%s:kind = %d\n\r",\
					   __FUNCTION__,kind);
				totallen -= 1;
				src += 1;
				break;
			case EN_TCP_OPT_MSS:      //SET THE REMOTE MSS
				optlen = *optsrc++;
				data16 = *(u16 *)(optsrc);
				tcb->rmss = htons(data16);
				tcb->mss = tcb->lmss>tcb->rmss?tcb->rmss:tcb->lmss;
				printk("%s:kind = %d len = %d data = %d\n\r",\
					   __FUNCTION__,kind,optlen,data16);
				totallen -= optlen;
				src += optlen;
				break;
			case EN_TCP_OPT_SCALE:      //SET THE REMOTE WND SCALE
				optlen = *optsrc++;
				data8 = *(u8 *)(optsrc);
				tcb->sndwndscale = data8;
				printk("%s:kind = %d len = %d data = %d\n\r",\
					   __FUNCTION__,kind,optlen,data8);
				totallen -= optlen;
				src += optlen;
				break;
			case EN_TCP_OPT_TAMP:
				optlen = *optsrc++;
				printk("%s:kind = %d len = %d \n\r",\
					   __FUNCTION__,kind,optlen);
				totallen -= optlen;
				src += optlen;
				break;
			default:
				printk("%s:kind = %d\n\r",\
					   __FUNCTION__,kind);
				totallen = 0;
				break;
		}
	}
	return ;
}
//////////////////////////////////״̬���������/////////////////////////////////
//RST���Ŀ��Կ�����һ�����ⱨ�ģ�������ͳһ�Ĵ�����--��λ
//����ڸ���״̬���Ĵ����У����ǲ�����RST���ĵĴ�������������״̬ת��ͼ������
//����״̬ת����������յ�ĳ���Ķ�����Ҫ�ظ�ʱ��״̬Ǩ�ƿ���ֱ�����մ����н��У�
//�����յ�������Ҫ�ظ����ܽ���״̬Ǩ�ƣ������ڻظ����֮��Ž���״̬Ǩ�ƣ�ÿ��״̬
//Ҫ��ֿ��ǵ��յ��������ĵĴ���
//����
//1,���е�״̬����û�еõ����ض˵�����֮�£��ǲ��ᷢ��FIN�ģ�
//2,ֻ��SHUT_WR SHUT_RDWR CLOSE�Ż�������FIN,�Ż������Ĺر�����
//3,��û���յ����ض˹رյ������£����е�Զ��״̬��ൽ��RST״̬
//4,refered��־ֻ����accept��ʱ����Ϊ�棬����������¶���false����λ���Ḵλ�ñ�־
//5,��״̬���У�pkg��offsetһ��Ҫ�ƶ��������������ݵĽ׶Σ�����hdr��opt
// =============================================================================
// �������ܣ�__TcpRetransmitDeal
//        �ش���ʱ��
// ���������tcb,ָ����TCB���ƿ�
//          fastretrans, trueΪ�����ش���false �����Ƿ��ǳ����ش�
// ���������
// ����ֵ  ��
// ˵��    :�ش���ʱ��������SYN/����/FIN���ش�����ʵ������SYN DATA FIN��������Ҫ�ش��ģ����Ƕ�����Ҫռ��seqno
//      ����ģ�������һ���ش������Ƕ�������Ϊ��flags+data����ʽ������������Ҫ���͵�
// =============================================================================
void __TcpRetransmitDeal(tagTcpTcb *tcb,bool_t fastretrans)
{
	tagTcpSndUnackNode  *secbuf;
	s64 timenow;
	//the tcb timer is running,so we chk weather the tcb has the
	//retransmit secbuf, because the timer is turned on ,so the secbuf
	//must be UNNULL
	//this is an assert chk
	secbuf = tcb->unacksechead;
	if(NULL != secbuf)
	{
		timenow = __Tcp_GetSysTime();
		if((secbuf->timeout < timenow)||(true == fastretrans))
		{
			//this is an timeout secbuf
			if(secbuf->rsndtimes < CN_TCP_RSNDTIMES_LIMIT)
			{
				//still in the retransmit target
				if(secbuf->flags&CN_TCP_FLAG_SYN)
				{
					//the retransmit include the syn flag,must transmit the option
					__Tcp_MsgSend(tcb,secbuf->flags,(u8 *)&sgSynOption,\
							      sizeof(sgSynOption),NULL,secbuf->seqno);
				}
				else
				{
					//maybe the data transmition
					__Tcp_MsgSend(tcb,secbuf->flags,NULL,0,secbuf->lsthead,secbuf->seqno);
				}
				//deal the timeout congestion
				//update the new sec timeout time
				tcb->rto = tcb->rto<<1;
				if(tcb->rto > CN_TCP_RTOMAX)
				{
					tcb->rto = CN_TCP_RTOMAX;
				}
				secbuf->timeout = timenow + tcb->rto;
				secbuf->rsndtimes++;
				//because we has dealt an timeout retransmition, so we must deal the
				//congestion window
				tcb->ssthresh = tcb->cwnd>>1;
				if(tcb->ssthresh < (2*tcb->mss))
				{
					tcb->ssthresh = 2*tcb->mss;
				}
				if(fastretrans)
				{
					tcb->cwnd = tcb->ssthresh + 3*tcb->mss;
				}
				else
				{
					tcb->cwnd = tcb->mss;
				}
				//for we has snd the flags,turn off the acktimer
				tcb->timerflag&=(~CN_TCP_TIMER_ACK);

				printk("Tcp Tick: Rsnd!--RTO=0X%08X\n\r",tcb->rto);
			}
			else
			{
			    //it has exceed the retransmit target, so just reset
				__Tcp_RstTcb(tcb);
			}
		}
	}

	return;
}

// =============================================================================
// �������ܣ�__Tcp_CheckInTargetArea
//          �ж�ĳ����ֵ�Ƿ���ָ���ķ�Χ��
// ���������target_start,ָ����ʼֵ
//          target_end,ָ������ֵ
//          tojudge,���ж�����ֵ
// ���������
// ����ֵ  ��true,��ָ����Χ��false����ָ����Χ
// ˵��    :���ж��迼���ƻ�����
// =============================================================================
bool_t __Tcp_CheckInTargetArea(u32 target_start, u32 target_end, u32 tojudge)
{
	bool_t result = false;

	if(target_end < target_start) //���ƻ�
	{
		if((tojudge>= target_start)|| (tojudge <= target_end))
		{
			result = true;
		}
	}
	else //���ƻ�
	{
		if((tojudge >= target_start) &&(tojudge<= target_end))
		{
			result = true;
		}
	}
	return result;
}

static u32 sgLastAck;
// =============================================================================
// �������ܣ�__Tcp_DealAck
//          ����ACK���
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
// ���������
// ����ֵ  ��bool_t true����ɹ� falseʧ��
// ˵��    :1,��ACK���ĵ���ʱ�����ACK�ź�WINDOW������TCBҪ�������ݣ��򼤻�����������
//        ��س�Ա������
//      2,����ackno,��Ҫע�����Ҫ�ǵ��ͷ�δȷ�ϵ�����
//      3,������ʱ�رն��ԣ�����յ�ACK����ȷ�����ٿɷ���������ȷ�����������ݶ�����
//         �ˣ���ô��Ҫ����FIN�ź�
// =============================================================================
bool_t __Tcp_DealAck(tagTcpTcb *tcb, tagTcpHdr *hdr)
{
	bool_t             result;
	bool_t             retransever;
	u32                newackno;
	u32                newwindow;
	u32                addwnd;
	s64                timenow;
	s64                timesnd;
	s32                err;
	s32                uerr;
	tagTcpSndUnackNode *secbuf;
	
	result = false;
	secbuf = tcb->unacksechead;
	newackno = ntohl(hdr->ackno);
	newwindow = ntohs(hdr->window);
	
	sgLastAck = newackno;              //for the debug--TODO CLEAR
	//acked the unack pkg
	if((newackno == tcb->snduna)&&(NULL != secbuf))
	{
		//the same ack, maybe an need fast retransmit
		tcb->dupacks++;
		if(tcb->dupacks >= CN_TCP_FASTRETRANS)
		{
			//do the fast retransmition
			__TcpRetransmitDeal(tcb,true);
		}	
	}
	else if(NULL == secbuf)
	{
		tcb->dupacks = 0;
	}
	else 
	{
		//update the ackno and free the acked secbuf
		addwnd =0;
		retransever = false;
		//free the secbuf
		while(NULL != secbuf)
		{
			//-1 means that when seqno == newackno, which means that this secbuf could
			//not release;
			//in the windows stack, they could snd the ack not complete pkg.
			//which means they ack the pkg partely,so we could not release the
			//whole pkg
			if(__Tcp_CheckInTargetArea(tcb->snduna, newackno, secbuf->seqno+secbuf->datalen))
			{
				tcb->unacksechead = secbuf->nxt;
				if(secbuf->rsndtimes > 0)
				{
					retransever = true;
					timesnd = secbuf->sndtime;
				}
				if(NULL == tcb->unacksechead)
				{
					tcb->unacksectail = NULL;
				}
				Pkg_PartFree(secbuf->lsthead);
				__Tcp_FreeUnackedNode(secbuf);
				secbuf = tcb->unacksechead;
				addwnd++;
			}
			else
			{
				secbuf = NULL;
			}
		}
		//update rtt measure
		if(false == retransever)
		{
			//do the rtt test,update the rto
			timenow = __Tcp_GetSysTime();
			err = timenow - timesnd - tcb->sa;
			uerr = err>0?err:(-err);
			//scale the sa sd and rto
			tcb->sa = tcb->sa+(err>>3);
			tcb->sd = tcb->sd + ((uerr-tcb->sd)>>2);
			tcb->rto = tcb->sa + (tcb->sd<<2);
			if(tcb->rto < CN_TCP_RTOMIN)
			{
				tcb->rto = CN_TCP_RTOMIN;
			}
		}
		//update the congestion wnd
		if(tcb->dupacks >= CN_TCP_FASTRETRANS)
		{
			//an fast retransmit has happened
			tcb->cwnd = tcb->ssthresh;
		}
		else
		{
			//chk it is slow start or congestion avoid
			if(tcb->cwnd > tcb->ssthresh)
			{
				//it is congestion avoid
				tcb->cwnd += tcb->cwnd/32;
			}
			else
			{
				addwnd = addwnd*tcb->mss;
				tcb->cwnd += addwnd;

			}
		}  
		//update the ackno
		tcb->snduna = newackno;
		tcb->dupacks = 0;
		//retransmit timer
		if(NULL == tcb->unacksechead)
		{
			//turn off the retransmit timer
			tcb->timerflag &= (~CN_TCP_TIMER_RETRANSMIT);
		}
		
        result = true;
	}
	//update the window
	if(tcb->sndnxt >= tcb->snduna)
	{
		addwnd = tcb->sndnxt- tcb->snduna;
	}
	else
	{
		addwnd = 0xFFFFFFFF - tcb->snduna + tcb->sndnxt;
	}

	newwindow = newwindow<<tcb->sndwndscale;
	if(newwindow > addwnd)
	{
		tcb->sndwnd = (newwindow - addwnd)>>tcb->sndwndscale;
	    __Tcp_Data2Snd(tcb);
	}
	else
	{
		tcb->sndwnd = 0;
		if(NULL != tcb->sbuf.pkglsthead)
		{
			tcb->timerflag |=CN_TCP_TIMER_PERSIST;
		}
	}	
	return result;
}

// =============================================================================
// �������ܣ�__Tcp_DealSeqno
//          ����ACK���
// ���������tcb, tcb���ƿ�
//          hdr, tcp ��ͷ
// ���������pkgreserve,�Ǳ���PKG��true��ʾPKG�Ѿ���������У�false��ʾpkg��Ҫ�ͷ�
// ����ֵ  ��true,�����ݰ�������˻��������У������ʾ�����ͷ�PKG
// ˵��    :1,�ڵ��øú���֮ǰ��Ӧ�ü��pkg����Ч�ԣ���Ч�����Ƿ�Ϊ0,�Ƿ񳬳�MSS��
//       2,��Ϊ���ݰ��ǺϷ��ģ�������Ҫ��ACK��ʱ���������ڴ��������
//       3,����Ϊ0ʱ��������ջ������е������Ѿ��ﵽ���ڵ�4������ô�Ͷ��������ݰ�
//                   ���������������ݰ�������ᵼ�����ǵĻ�������������
//       4,��������ݰ�ʱ�����seqnoС��rcvnxt��֤�����ظ��������ݣ�������᣻
//                        ����rcvnxt,������ݰ�ǡ���ô�����ʱ��Ҫ������������Ƿ�
//                                   ���а���Ҫ����
//                        ����rcvnxt����֤�����������ݣ�ֱ�ӷŽ��������м���
// =============================================================================
bool_t __Tcp_DealRcvData(tagTcpTcb *tcb, u32 seqno,tagNetPkg *pkg)
{
	bool_t  result;
	int     rcvlen;
	tagTcpReCombNode *secbuf;
	tagTcpReCombNode *tmpsec1;
	tagTcpReCombNode *tmpsec2;
	
	result = false;
	pkg->partnext = NULL;
	rcvlen = 0;
	if(seqno == tcb->rcvnxt)
	{
		//this is just the pkg what we rcv next
		//this is an sequence pkg what we need,add it to the rcv buf
		if(NULL == tcb->rbuf.pkglsttail)
		{
			tcb->rbuf.pkglsthead = pkg;
		}
		else
		{
			tcb->rbuf.pkglsttail->partnext = pkg;
		}
		tcb->rbuf.pkglsttail = pkg;
		tcb->rbuf.buflen += pkg->datalen;
		tcb->rcvnxt += pkg->datalen;
		tcb->rcvwnd = tcb->rcvwnd > pkg->datalen?(tcb->rcvwnd-pkg->datalen):0;
		rcvlen += pkg->datalen;
		//check if the recombination queue has some corresponding pkg to add
		secbuf = tcb->recomblst;
		while(secbuf != NULL)
		{
			if(secbuf->seqno == tcb->rcvnxt)
			{
				//this is the recombinnation pkg
				tcb->rbuf.pkglsttail->partnext = secbuf->lsthead;
				tcb->rbuf.pkglsttail = secbuf->lsttail;
				tcb->rbuf.buflen += secbuf->datalen;
				tcb->rcvnxt += secbuf->datalen;
				tcb->rcvwnd = tcb->rcvwnd>secbuf->datalen?\
						     (tcb->rcvwnd-secbuf->datalen):0;
				rcvlen += secbuf->datalen;
				tcb->recomblst = secbuf->nxt;
				__Tcp_FreeReCombNode(secbuf);
				secbuf= tcb->recomblst;
			}
			else
			{
				//because we have queue it when we add it into the rb queue
				//if the pre pkg is not the bingo one, then there is no chance
				//for the nxt one, if it is not in the target area, we will
				//realease it to avoid exist in the queue forever
				if(__Tcp_CheckInTargetArea(tcb->rcvnxt, \
						tcb->rcvnxt+CN_TCP_RCVWIN_DEFAULT,secbuf->seqno))
				{
					//this secbuf is the latest one, but can't recombin it yet,
					//so we end the recombin action
					secbuf = NULL;//end for the while
				}
				else
				{
					//release the secbuf,because it is out of the target area,
					//we will recycle the resource it alloc
					tcb->recomblst = secbuf->nxt;
					Pkg_LstFlagFree(secbuf->lsthead);
					__Tcp_FreeReCombNode(secbuf);
					secbuf= tcb->recomblst;
				}
			}
		}
		//rcv the new data, notice the app to read it
		//means the pkg has been cached in the rcb process
		if(tcb->rbuf.buflen >= tcb->rbuf.triglevel)
		{
			if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
			{
				tcb->owner->status |= CN_SOCKET_READALBE;
				Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
			}
			Lock_SempPost(tcb->rbuf.bufsync);
		}
		result = true;
	}
	else if(__Tcp_CheckInTargetArea(tcb->rcvnxt, \
			tcb->rcvnxt+CN_TCP_RCVWIN_DEFAULT,seqno))
	{
		//this is an recombin pkg
		result = true;
		if(NULL == tcb->recomblst)
		{
			secbuf = __Tcp_AllocReCombNode();
			secbuf->lsthead = pkg;
			secbuf->lsttail = pkg;
			secbuf->seqno = seqno;
			secbuf->datalen = pkg->datalen;
			secbuf->nxt = NULL;
			tcb->recomblst = secbuf;
		}
		else
		{
			tmpsec1 = tcb->recomblst;
			tmpsec2 = tmpsec1->nxt;
			if(__Tcp_CheckInTargetArea(tcb->rcvnxt,tmpsec1->seqno,seqno))
			{
				//check if it is just btween rcvnxt and sec1
				if(tmpsec1->seqno == (seqno + pkg->datalen))
				{
					pkg->partnext = tmpsec1->lsthead;
					tmpsec1->lsthead = pkg;
					tmpsec1->seqno = seqno;
					tmpsec1->datalen+=pkg->datalen;
				}
				else
				{
					//alloc a new secbuf insert before sec1
					secbuf = __Tcp_AllocReCombNode();
					secbuf->lsthead = pkg;
					secbuf->lsttail = pkg;
					secbuf->seqno = seqno;
					secbuf->datalen = pkg->datalen;
					secbuf->nxt = tmpsec1;
					tcb->recomblst = secbuf;
				}
			}
			else
			{
				while(NULL != tmpsec2)
				{
					if(__Tcp_CheckInTargetArea(tmpsec1->seqno,tmpsec2->seqno,\
											   seqno))
					{
						break; //binggo, we have got one
					}
					else
					{
						tmpsec1 = tmpsec2;
						tmpsec2 = tmpsec2->nxt;
					}
				}
				if(NULL == tmpsec2)
				{
					if((tmpsec1->seqno+ tmpsec1->datalen) == seqno)
					{

						//just add it to the sec1
						tmpsec1->lsttail->partnext = pkg;
						tmpsec1->lsttail = pkg;
						tmpsec1->datalen+=pkg->datalen;
					}
					else
					{
						//alloc an new secbuf and add it to rb tail
						secbuf = __Tcp_AllocReCombNode();
						secbuf->lsthead = pkg;
						secbuf->lsttail = pkg;
						secbuf->seqno = seqno;
						secbuf->datalen = pkg->datalen;
						secbuf->nxt = NULL;
						tmpsec1->nxt =secbuf;
					}
				}
				else
				{
					if(((tmpsec1->seqno + tmpsec1->datalen)== seqno)&&\
							((seqno + pkg->datalen)== tmpsec2->seqno))
					{
						//this pkg could connect tmpsec1 and tmpsec2
						//so add all sec2 and pkg to sec1
						tmpsec1->lsttail->partnext = pkg;
						pkg->partnext = tmpsec2->lsthead;
						tmpsec1->lsttail = tmpsec2->lsttail;
						tmpsec1->datalen+= pkg->datalen+ tmpsec2->datalen;
					}
					else if((tmpsec1->seqno + tmpsec1->datalen)== seqno)
					{
						//add pkg to sec1 tail
						tmpsec1->lsttail->partnext = pkg;
						tmpsec1->lsttail = pkg;
						tmpsec1->datalen+=pkg->datalen;

					}
					else if((seqno + pkg->datalen)== tmpsec2->seqno)
					{
						//add pkg to sec2 head
						pkg->partnext = tmpsec2->lsthead;
						tmpsec2->lsthead = pkg;
						tmpsec2->datalen+=pkg->datalen;
						tmpsec2->seqno = seqno;
					}
					else
					{
						//alloc an new secbuf to insert between sec1 and sec2
						secbuf = __Tcp_AllocReCombNode();
						secbuf->lsthead = pkg;
						secbuf->lsttail = pkg;
						secbuf->seqno = seqno;
						secbuf->datalen = pkg->datalen;
						secbuf->nxt = NULL;
						tmpsec1->nxt = secbuf;
						secbuf->nxt = tmpsec2;
					}
				}
			}
		}
		//it is an recombin pkg, so we'd better ack it now
		result = true;
	}
	return result;
}

// =============================================================================
// �������ܣ�__Tcp_SSynRcvProcess
//          ��ǰ״̬ΪSSynSent�Ĵ���
// ���������tcb, tcb���ƿ�
//           hdr, tcp ��ͷ
//           pkg, �յ������ݰ�
// ���������
// ����ֵ  ��true,�ð������׽��ֻ������� false�����׽���δ�����׽��ֻ�����
// ˵��    :here we are just interested in the ack to the syn we has snd
// =============================================================================
bool_t __Tcp_SSynRcvProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                    tagNetPkg *pkg)
{
	bool_t result;
	u32    newackno;
	tagSocket *lfd;
	tagSocket *rfd;
	tagTcpTnode *tnode;
	
	result = false;
	if(CN_TCP_FLAG_ACK&hdr->flags)
	{
		newackno = ntohl(hdr->ackno);
		if(newackno == tcb->sndnxt)
		{
			//ת��ESTABLISH״̬
			tcb->state = EN_TCPSTATE_ESTABLISHED;
			//deal the ack
			__Tcp_DealAck(tcb,hdr);
            //���±���״̬
			rfd = tcb->owner;
			lfd = rfd->hungnode;
			tnode = (tagTcpTnode*)lfd->cb;
			//��ACCEPT�����ӣ���Ȼ����״̬���п���Ҫ����CONSYNC�ź�����
			tnode->connum2accept++;
			tnode->connum++;
			Lock_SempPost(lfd->casync);
			if(0 == (CN_SOCKET_ACCEPT & lfd->status))
			{
				lfd->status |= CN_SOCKET_ACCEPT;
				Multiplex_Set(lfd->multiio, lfd->status);
			}
		}	
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_CSynSentSProcess
//        ��ǰ״̬ΪCSynSent�Ĵ���
// ���������tcb, tcb���ƿ�
//           hdr, tcp ��ͷ
//           pkg, �յ������ݰ�
// ���������
// ����ֵ  ��true,�ð������׽��ֻ������� false�����׽���δ�����׽��ֻ�����
// ˵��    :we only intereted in the pkg include syn and ack
// =============================================================================
bool_t __Tcp_CSynSentSProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                      tagNetPkg *pkg)
{
	bool_t result;
	u32  newackno;
	tagSocket *rfd;
	
	result = false;
	if((hdr->flags & CN_TCP_FLAG_ACK)&&(hdr->flags &CN_TCP_FLAG_SYN))
	{
		newackno = ntohl(hdr->ackno);
		if(newackno == tcb->sndnxt) //����������Ҫ�ı���
		{
			//����OPѡ���Ҫ��MSS
			__Tcp_OptDeal(tcb,hdr);
			//���´��ںͽ��պ�
			tcb->rcvnxt = ntohl(hdr->seqno) + 1;
			//ת�뵽�ȶ�����״̬
			tcb->state = EN_TCPSTATE_ESTABLISHED;
			//deal the ack
			__Tcp_DealAck(tcb,hdr);
			__Tcp_MsgSend(tcb, CN_TCP_FLAG_ACK, NULL, 0, NULL, tcb->sndnxt);
			//����ض˵�consync����Ϊ���ض˵�CONNECT���ڵȴ����ǵĽ�������
			rfd = tcb->owner;
			//ֱ�Ӽ��������
			Lock_SempPost(rfd->casync);
		}
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_EstablishedProcess
//        deal the established state
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
//        pkg, �յ������ݰ�
// ���������
// ����ֵ  ��true,�ð������׽��ֻ������� false�����׽���δ�����׽��ֻ�����
// ˵��    :when in established state, we are only intereted in three types pkg:
//      1,normal data
//      2,only fin pkg,this means the pkg == NULL
//      3,fin with normal data,the last two types are dealed in seqno deal
// =============================================================================
bool_t __Tcp_EstablishedProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                         tagNetPkg *pkg)
{
	bool_t result;
	u32    seqno;

	result = false;
	seqno =ntohl(hdr->seqno);
	if(CN_TCP_FLAG_FIN & hdr->flags)
	{
		//this is the only fin,if not normal fin, we just ignore it
		if(seqno == tcb->rcvnxt)
		{
			//this is just the fin that need
			tcb->rcvnxt++;
			seqno++;
			tcb->state = EN_TCPSTATE_CLOSE_WAIT;
			//if any data with this fin
			if(NULL != pkg)
			{
				if(tcb->channelstate.krcv)
				{
					result = __Tcp_DealRcvData(tcb,seqno,pkg);
				}
			}
			tcb->channelstate.krcv = 0;
			if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
			{
				tcb->owner->status |= CN_SOCKET_READALBE;
				Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
			}
			Lock_SempPost(tcb->rbuf.bufsync);
		}
	}
	else if(NULL != pkg)
	{
		if(tcb->channelstate.krcv)
		{
			result = __Tcp_DealRcvData(tcb,seqno,pkg);
			if(tcb->rbuf.buflen > tcb->rbuf.triglevel)
			{
				Lock_SempPost(tcb->rbuf.bufsync);
				if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
				{
					tcb->owner->status |= CN_SOCKET_READALBE;
					Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
				}
			}
		}
	}
	else
	{

	}
	//deal the ack
	 __Tcp_DealAck(tcb, hdr);
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags)||(CN_TCP_FLAG_SYN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}
	return result;
}
// =============================================================================
// �������ܣ�__Tcp_Finwait1SProcess
//          ��ǰ״̬ΪFIN_WAIT1�Ĵ���
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
//        pkg, �յ������ݰ�
// ���������
// ����ֵ  ��true,�ð������׽��ֻ������� false�����׽���δ�����׽��ֻ�����
// ˵��    :when we shutdown the send channel of our own record, we will state at this
//      state.of course, the interesting pkg we need is:
//      1,ack the fin we sent,we will goto the fin_wait2 state
//      2,ack the fin we sent and send the fin, we will goto the time_wait state
//      3,only send the fin, not ack the fin we sent
//      all of above maybe include the valid data
// =============================================================================
bool_t __Tcp_Finwait1SProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                      tagNetPkg *pkg)
{
	bool_t result;
	u32  seqno;
	u32  ackno;
	
	result = false;
	seqno = ntohl(hdr->seqno);
	ackno = ntohl(hdr->ackno);
	if(ackno == tcb->sndnxt)
	{
		//rcv the ack to the fin we sent,if only ack we will turn to fin2, if
		//also rcv the fin, turn to time wait
		if((seqno == tcb->rcvnxt)&&(CN_TCP_FLAG_FIN & hdr->flags))
		{
			//also rcv the fin
			tcb->rcvnxt++;
			seqno++;
			//if any data with this fin
			if(NULL != pkg)
			{
				if(tcb->channelstate.krcv)
				{
					result = __Tcp_DealRcvData(tcb,seqno,pkg);
				}
			}
			tcb->channelstate.krcv = 0;
			if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
			{
				tcb->owner->status |= CN_SOCKET_READALBE;
				Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
			}
			Lock_SempPost(tcb->rbuf.bufsync);

			tcb->state = EN_TCPSTATE_TIME_WAIT;
		}
		else
		{
			//only rcv the ack to the fin we snt
			//if any data
			if(NULL != pkg)
			{
				if(tcb->channelstate.krcv)
				{
					result = __Tcp_DealRcvData(tcb,seqno,pkg);
					if(tcb->rbuf.buflen > tcb->rbuf.triglevel)
					{
						Lock_SempPost(tcb->rbuf.bufsync);
						if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
						{
							tcb->owner->status |= CN_SOCKET_READALBE;
							Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
						}
					}
				}
			}

			tcb->state = EN_TCPSTATE_FIN_WAITE2;
		}
	}
	else if((seqno == tcb->rcvnxt)&&(CN_TCP_FLAG_FIN & hdr->flags))
	{
		//rcv the remote fin, turn to CLOSING
		tcb->rcvnxt++;
		seqno++;
		//if any data with this fin
		if(NULL != pkg)
		{
			if(tcb->channelstate.krcv)
			{
				result = __Tcp_DealRcvData(tcb,seqno,pkg);
			}
		}
		tcb->channelstate.krcv = 0;
		if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
		{
			tcb->owner->status |= CN_SOCKET_READALBE;
			Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
		}
		Lock_SempPost(tcb->rbuf.bufsync);

		tcb->state = EN_TCPSTATE_CLOSING;
	}
	else if(NULL != pkg)
	{
		if(tcb->channelstate.krcv)
		{
			result = __Tcp_DealRcvData(tcb,seqno,pkg);
			if(tcb->rbuf.buflen > tcb->rbuf.triglevel)
			{
				Lock_SempPost(tcb->rbuf.bufsync);
				if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
				{
					tcb->owner->status |= CN_SOCKET_READALBE;
					Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
				}
			}
		}
	}
	else
	{

	}
	//deal the ack
	 __Tcp_DealAck(tcb, hdr);
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags)||(CN_TCP_FLAG_SYN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�__Tcp_Finwait2SProcess
//          ��ǰ״̬ΪFIN_WAIT2�Ĵ���
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
//        pkg, �յ������ݰ�
// ���������
// ����ֵ  ��true,�ð������׽��ֻ������� false�����׽���δ�����׽��ֻ�����
// ˵��    :when we shutdown the send channel of our own record, and we rcv the remote
//      confirm of the fin we snt, we will state at this
//      state.of course, the interesting pkg we need is:
//      1, the remote fin, we turn to the TIME_WAIT
//      all of above maybe include the valid data
// =============================================================================
bool_t __Tcp_Finwait2SProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                      tagNetPkg *pkg)
{
	bool_t result;
	u32  seqno;
	
	result = false;
	seqno = ntohl(hdr->seqno);
	//if rcv the remote fin ,we will turn to time wait
	if((seqno == tcb->rcvnxt)&&(CN_TCP_FLAG_FIN & hdr->flags))
	{
		//also rcv the fin
		tcb->rcvnxt++;
		seqno++;
		//if any data with this fin
		if(NULL != pkg)
		{
			if(tcb->channelstate.krcv)
			{
				result = __Tcp_DealRcvData(tcb,seqno,pkg);
			}
		}
		tcb->channelstate.krcv = 0;
		if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
		{
			tcb->owner->status |= CN_SOCKET_READALBE;
			Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
		}
		Lock_SempPost(tcb->rbuf.bufsync);

		tcb->state = EN_TCPSTATE_TIME_WAIT;
	}
	else if(NULL != pkg)
	{
		if(tcb->channelstate.krcv)
		{
			result = __Tcp_DealRcvData(tcb,seqno,pkg);
			if(tcb->rbuf.buflen > tcb->rbuf.triglevel)
			{
				Lock_SempPost(tcb->rbuf.bufsync);
				if(0 ==(CN_SOCKET_READALBE & tcb->owner->status))
				{
					tcb->owner->status |= CN_SOCKET_READALBE;
					Multiplex_Set(tcb->owner->multiio, tcb->owner->status);
				}
			}
		}
	}
	else
	{

	}
	//deal the ack
	 __Tcp_DealAck(tcb, hdr);
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags)||(CN_TCP_FLAG_SYN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}

	return result;
}
// =============================================================================
// �������ܣ�__Tcp_ClosingProcess
//        ��ǰ״̬ΪCLOSING�Ĵ���
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
//        pkg, �յ������ݰ�
// ���������
// ����ֵ  ��always false
// ˵��    :We state at this state because we has received the fin of remote, because this
//      closing has been started by ourselves, so we just ignore any data of remote
//      1,if any data or fin ,just turn on the acktimer
//      2,if the ack is to the fin we sent, turn to time_wait state
// =============================================================================
bool_t __Tcp_ClosingProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                    tagNetPkg *pkg)
{
	u32  ackno;
	
	ackno = ntohl(hdr->ackno);
	if(ackno == tcb->sndnxt)
	{
		tcb->state = EN_TCPSTATE_TIME_WAIT;
	}
	//deal the ack
	 __Tcp_DealAck(tcb, hdr);
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}
	return false;
}
// =============================================================================
// �������ܣ�__Tcp_TimeWaitSProcess
//          ��ǰ״̬ΪFIN_WAIT2�Ĵ���
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
//        pkg, �յ������ݰ�
// ���������
// ����ֵ  ��always false
// ˵��    :this state means that local and remote agree to turn off the channel, and
//      the channel has been shutdown, so we could ignore any data
//      if the fin or data ,this means the last ack has been missed,
// =============================================================================
bool_t __Tcp_TimeWaitSProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                      tagNetPkg *pkg)
{
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}
	return false;
}
// =============================================================================
// �������ܣ�__Tcp_CloseWaitSProcess
//          ��ǰ״̬ΪFIN_WAIT2�Ĵ���
// ���������tcb, tcb���ƿ�
//        hdr, tcp ��ͷ
//        pkg, �յ������ݰ�
// ���������
// ����ֵ  ��always false
// ˵��    :this state means the remote has shutdown the channel, and we has confirmed
//      the shutdown, but for reason, we can't shutdown it now;maybe the ack to the remote
//      fin missed; at the state we still could transmit the data, so we must
//      deal the ack
// =============================================================================
bool_t __Tcp_CloseWaitSProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                       tagNetPkg *pkg)
{
	//deal the ack
	 __Tcp_DealAck(tcb, hdr);
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}
	return false;
}

// =============================================================================
// �������ܣ�__Tcp_LastAckSProcess
//          ��ǰ״̬ΪLAST ACK�Ĵ���
// ���������tcb, tcb���ƿ�
//           hdr, tcp ��ͷ
//           pkg, �յ������ݰ�
// ���������
// ����ֵ  ��always false
// ˵��    :this condition meas that we also snt the fin at the request of remote
//      if we rcv the confirmed ack to the fin we sent, we turn to another state
// =============================================================================
bool_t __Tcp_LastAckSProcess(tagTcpTcb *tcb, tagTcpHdr *hdr,\
							 tagNetPkg *pkg)
{
	u32    ackno;

	ackno = ntohl(hdr->ackno);
	if(ackno == tcb->sndnxt)
	{
		//the ack to the fin we sent
		if(tcb->channelstate.close)
		{
			tcb->state = EN_TCPSTATE_CLOSED;
		}
		else
		{
			tcb->state = EN_TCPSTATE_2FREE;
		}
	}
	//deal the ack
	 __Tcp_DealAck(tcb, hdr);
	//anyway open the acktimer if data reached
	if((NULL != pkg)||(CN_TCP_FLAG_FIN & hdr->flags))
	{
		if(0 == (tcb->timerflag &CN_TCP_TIMER_ACK))
		{
			tcb->timerflag |= CN_TCP_TIMER_ACK;
			tcb->acktimer = CN_TCP_TICK_ACK;
		}
	}

	return false;
}
// =============================================================================
// �������ܣ�__Tcp_RstPkgDeal
//          RST���Ĵ���
// ���������tcb,�����͸���TCB
//           hdr,�յ���RST����
// ���������
// ����ֵ  ����
// ˵��    :��������RST����
// =============================================================================
void __Tcp_RstPkgDeal(tagTcpTcb *tcb, tagTcpHdr *hdr)
{
	tagSocket *sock;
	
	sock = tcb->owner;
	//ack���ı������Լ����͵ı��ĵ����кŵĴ��ڷ�Χ֮�࣬���ڵ��ڵ�һ��δȷ�ϣ�С�ڵ���
	//���һ����ȷ�ϵģ�����֤���Լ�ͨ���ĶԶ˲���α��Ļ��߶��⹥����
	if(__Tcp_CheckInTargetArea(tcb->snduna,tcb->sndnxt,ntohl(hdr->ackno)))
	{
		printk("Rcv the rst pkg!\n\r");
		__Tcp_RstTcb(tcb);
		
		//�����ǰ�ı��ض˻���������״̬����Ӧ�ü���
		if(EN_SOCKET_LOCAL_CONNECT == sock->appstate)
		{
			Lock_SempPost(sock->casync);
		}
		else
		{
			if(1 == tcb->channelstate.open)
			{
				//�Ѿ������ˣ�Ӧ��֪ͨ�ɶ�д
				tcb->owner->status |= CN_SOCKET_READALBE|CN_SOCKET_WRITEALBE;
				Multiplex_Set(tcb->owner->multiio, tcb->owner->status);

				//post the snd and rcv
				Lock_SempPost(tcb->sbuf.bufsync);
				Lock_SempPost(tcb->rbuf.bufsync);
			}
			else
			{
				//��δ�򿪹��������ͷŵ���
				tcb->state = EN_TCPSTATE_2FREE;
			}
		}
	}
}
// =============================================================================
// �������ܣ�__Tcp_DealRemotePkg
//          ������߳�
// ���������socket,�������Զ���׽���
//           tcphdr,TCP��ͷ
//           pkg,  Զ�˷��������ݰ�
// ���������
// ����ֵ  ��true, ���ݰ�������socket�����У�false��û�й���socket������
// ˵��    :�����ѡ�����MSS SACK������ѡ����ʱ��֧��
// =============================================================================
bool_t __Tcp_DealRemotePkg(tagTcpTcb *tcb, tagTcpHdr *hdr,\
		                 tagNetPkg *pkg)
{
   	bool_t  result;

   	result = false;
   	//���ĵı�Ǵ���
   	if(hdr->flags & CN_TCP_FLAG_RST)//RST����ͳһ���������Ϳ��Լ���
   	{
   		__Tcp_RstPkgDeal(tcb,hdr);
   	}
   	else  
   	{
   		//rcv the pkg means we still in the communication, so reset the keepalive
   		tcb->keeptimer = CN_TCP_TICK_KEEPALIVE;
   		//״̬������
   		switch(tcb->state)
   		{
   			case EN_TCPSTATE_CSYNSNT:
   				result = __Tcp_CSynSentSProcess(tcb, hdr, pkg);
   				break;
   			case EN_TCPSTATE_SSYNRCV:
   				result = __Tcp_SSynRcvProcess(tcb, hdr,pkg);
   				break;
   			case EN_TCPSTATE_ESTABLISHED:
   				result = __Tcp_EstablishedProcess(tcb, hdr,pkg);
   				break;
   			case EN_TCPSTATE_FIN_WAITE1:
   				result = __Tcp_Finwait1SProcess(tcb, hdr, pkg);
   				break;
   			case EN_TCPSTATE_FIN_WAITE2:
   				result = __Tcp_Finwait2SProcess(tcb, hdr,pkg);
   				break;
   			case EN_TCPSTATE_CLOSING:
   				result = __Tcp_ClosingProcess(tcb,hdr,pkg);
   				break;
   			case EN_TCPSTATE_TIME_WAIT:
   				result = __Tcp_TimeWaitSProcess(tcb, hdr, pkg);
   				break;
   			case EN_TCPSTATE_CLOSE_WAIT:
   				result = __Tcp_CloseWaitSProcess(tcb,hdr,pkg);
   				break;
   			case EN_TCPSTATE_LAST_ACK:
   				result = __Tcp_LastAckSProcess(tcb,hdr,pkg);
   				break;
   			default:
   				result = false;
   				break;
   		}
   	}
    return result;
}
// =============================================================================
// �������ܣ�Tcp_RecvProcess
//          TCP���մ�����(����IP�㴫�ݹ��������ݰ�)
// ���������ipsrc,ԴIP
//        ipdst,Ŀ��IP
//        pkg,TCP���ݰ�
//        checksum,�Ƿ��Ѿ�������У��
// ���������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :deal the pkg rcved for tcp
// =============================================================================
bool_t Tcp_RecvProcess(u32 ipsrc, u32 ipdst, \
		        tagNetPkg *pkg, u32 devfunc)
{
	bool_t  result ;  //�����ݰ��Ƿ��Ѿ���ӵ��׽�����
	u16    portdst;
	u16    portsrc;
	tagTcpHdr *hdr;
	tagSocket *sockcon;
	tagSocket *socknode;
	tagTcpTcb   *tcb;
	tagTcpTnode *tnode;	
	tagNetPkg   *datapkg;
	result = false;
	if((INADDR_ANY == ipdst) || (INADDR_ANY == ipsrc) || (NULL == pkg))
	{
		goto __RCVPKG_DEALEND;  //para error
	}
	if(0 == (devfunc &CN_IPDEV_TCPICHKSUM))
	{
		//no hard chksum, we do the soft chksum
		if(false == __Tcp_ChksumRcvPkg(ipdst, ipsrc, pkg))
		{
			goto __RCVPKG_DEALEND;  //chksum failed
		}
	}
	hdr = (tagTcpHdr *)(pkg->buf + pkg->offset);
	if(pkg->datalen > (hdr->hdrlen*4))
	{
		pkg->datalen -= hdr->hdrlen*4;
		datapkg = pkg;
	}
	else
	{
		pkg->datalen = 0;
		datapkg = NULL;
	}
	pkg->offset += hdr->hdrlen*4;

	if(false == __Tcp_CheckTcpHdr(hdr))
	{
		goto __RCVPKG_DEALEND;  //hdr not normal
	}

	//ok,now we deal the pkg now, the pkg check success
	portdst = ntohs(hdr->portdst);
	portsrc = ntohs(hdr->portsrc);
	//����IP��PORT,������ҵ�ͨ�ŵ��Ǹ�port��
	//���ұ���SOCKET,���ж���״̬�����Ƿ�����ټ�������
	Lock_MutexPend(pgTcpSockTreeSync, CN_TIMEOUT_FOREVER);
	socknode = __Tcp_GetSocket(ipdst,ipsrc, portdst,portsrc);
	if(NULL != socknode)
	{
		Lock_MutexPend(socknode->sync, CN_TIMEOUT_FOREVER);
		if(EN_SOCKET_TREENODE == socknode->socktype)
		{
			//no server communication for the talk now, better to create one
			tnode = (tagTcpTnode *)socknode->cb;
			if((tnode->connum < tnode->connumlimit)&&\
			   (EN_SOCKET_LOCAL_LISTEN == socknode->appstate)&&\
			   (hdr->flags & CN_TCP_FLAG_SYN))
			{
				//�½���һ���ͻ���
				sockcon =Socket_New();
				if(NULL != sockcon)
				{
					tcb = __Tcp_NewTcb();
					if(NULL != tcb)
					{
						sockcon->cb = (void *)tcb;
						sockcon->socktype = EN_SOCKET_TERMINAL;
						tcb->owner = sockcon;
					}
					else
					{
						Socket_Free(sockcon);
						sockcon = NULL;
					}
				}
			}
			if(NULL != sockcon)
			{
				//add this socket to the node
				if(NULL != tnode->clientlst)
				{
					sockcon->nxt = tnode->clientlst;
					tnode->clientlst->pre = sockcon;
				}
				tnode->clientlst = sockcon;
				sockcon->hungnode = socknode;
				sockcon->proto = &pgTcpProtoRsc->proto;
				sockcon->laddr.ip = ipdst;
				sockcon->laddr.port = portdst;
				tcb->raddr.ip = ipsrc;
				tcb->raddr.port = portsrc;
				sockcon->constate = socknode->constate;
				tcb->rcvnxt = htonl(hdr->seqno)+1;
				__Tcp_OptDeal(tcb, hdr);
				tcb->state = EN_TCPSTATE_SSYNRCV;
                __Tcp_Syn2Snd(tcb);
			}
			else
			{
				//�������ɴRST����
				__Tcp_RstRemoteRaw(ipdst, portdst,ipsrc, \
						portsrc,ntohl(hdr->seqno)+1);
			}
			Lock_MutexPost(socknode->sync);
			Lock_MutexPost(pgTcpSockTreeSync);
		}
		else
		{
			Lock_MutexPost(pgTcpSockTreeSync);
			sockcon =socknode;
			//���洦��Զ�˵����ݰ�
			tcb = (tagTcpTcb *)(sockcon->cb);
			result = __Tcp_DealRemotePkg(tcb,hdr,datapkg);
			Lock_MutexPost(socknode->sync);
		}
	}//end for fin an sock to deal the pkg
	else
	{
		Lock_MutexPost(pgTcpSockTreeSync);
		//snd the rst to chk
		if(0==(hdr->flags & CN_TCP_FLAG_RST))
		{
			__Tcp_RstRemoteRaw(ipdst, portdst,ipsrc, \
					portsrc,ntohl(hdr->seqno)+1);
		}
	}//�������ɴRST����

__RCVPKG_DEALEND:
	if(false == result)//��PKGδ��ӵ��׽��ֶ�����ȥ
	{
		//if the pkg not cached to the buf, then release it
		Pkg_LstFlagFree(pkg);
		result = true;
	}
	
	return result;
}

// =============================================================================
// �������ܣ�__Tcp_DeleteClient
//        ɾ�����ؿͻ���
// ���������tcb,ָ����TCB���ƿ�
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Tcp_DeleteClient(tagTcpTcb *tcb)
{
	tagSocket *sock;

	sock = tcb->owner;
	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		pgTcpSockLstH = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}
	else
	{
		pgTcpSockLstT = sock->pre;
	}
	//reset and free the tcb
	__Tcp_RstTcb(tcb);
	__Tcp_FreeTcb(tcb);

	Lock_SempDelete(sock->casync);
	Socket_Free(sock);
}
// =============================================================================
// �������ܣ�__Tcp_DeleteServer
//        ɾ�����ط�����
// �����������ɾ����socket
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Tcp_DeleteServer(tagSocket *sock)
{
	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		pgTcpSockLstH = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}
	else
	{
		pgTcpSockLstT = sock->pre;
	}
	//reset and free the tnode
	__Tcp_FreeTnode((tagTcpTnode *)sock->cb);

	Lock_SempDelete(sock->casync);
	Socket_Free(sock);
}
// =============================================================================
// �������ܣ�__Tcp_DeleteServerSession
//        ɾ�����ط�������һ���Ự
// �����������ɾ���ķ�������һ���Ựtcb
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Tcp_DeleteServerSession(tagTcpTcb *tcb)
{
	tagSocket *sock;
	tagTcpTnode *tnode;

	sock = tcb->owner;
	tnode = (tagTcpTnode *)sock->hungnode->cb;

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

	//reset and free the tcb
	__Tcp_RstTcb(tcb);
	__Tcp_FreeTcb(tcb);

	Socket_Free(sock);
}

// =============================================================================
// �������ܣ�__Tcp_DeleteSockNode
//        ɾ��δ֪��socknode,���ض˵�
// �����������ɾ����socket
// ���������
// ����ֵ  ��
// ˵��    :������ɾ���������κεı���
// =============================================================================
void __Tcp_DeleteSockNode(tagSocket *sock)
{
	if(NULL != sock->pre)
	{
		sock->pre->nxt = sock->nxt;
	}
	else
	{
		pgTcpSockLstH = sock->nxt;
	}
	if(NULL != sock->nxt)
	{
		sock->nxt->pre = sock->pre;
	}
	else
	{
		pgTcpSockLstT = sock->pre;
	}

	Lock_SempDelete(sock->casync);
	Socket_Free(sock);
}

// =============================================================================
// �������ܣ�__Tcp_DealTcbTimer
//        ����TCB�Ķ�ʱ������
// ���������tcb,ָ����TCB���ƿ�
// ���������
// ����ֵ  ����sock�Ƿ�Ҫɾ��
// ˵��    :�����Ǵ���ʱ����TIMER
// =============================================================================
bool_t __Tcp_DealTcbTimer(tagTcpTcb *tcb)
{
	bool_t result;
	u8  timerflag;

	result = false;

	if(tcb->state == EN_TCPSTATE_2FREE)
	{
		result = true;
	}
	else
	{
		timerflag = tcb->timerflag;
		//deal variouse timer
		if(timerflag & CN_TCP_TIMER_LINGER)
		{
			//linger timer
			if(tcb->lingertimer == 0)
			{
				result = true;
				goto __TIMER_DEALEND;
			}
			else
			{
				tcb->lingertimer--;
			}
		}
		if(timerflag &CN_TCP_TIMER_2ML)
		{
			//2ML TIMER
			if(0 == tcb->mltimer)
			{
				__Tcp_RstTcb(tcb);
				goto __TIMER_DEALEND;
			}
			else
			{
				tcb->mltimer--;
			}
		}
		if(timerflag &CN_TCP_TIMER_RETRANSMIT)
		{
			//TIME OUT TIMER
			__TcpRetransmitDeal(tcb,false);
		}
		if(timerflag &CN_TCP_TIMER_PERSIST)
		{
			//PERSIST TIMER
			if((0 == tcb->persistimer)&&(NULL != tcb->sbuf.pkglsthead))
			{
				printk("%s:do the persistimer\n\r");
                __Tcp_DataProbe(tcb);                
				tcb->persistimer = CN_TCP_TICK_PERSIST;
			}
			else
			{
				tcb->persistimer--;
			}
		}
		if(timerflag &CN_TCP_TIMER_KEEPALIVE)
		{
			//KEEP ALIVE TIMER
			if(0 == tcb->keeptimer)
			{
				printk("%s:do the keeptimer\n\r");
				 __Tcp_MsgSend(tcb,CN_TCP_FLAG_ACK, NULL, 1, NULL, tcb->snduna-1);
				 tcb->keeptimer = CN_TCP_TICK_KEEPALIVE;
			}
			else
			{
				tcb->keeptimer--;
			}
		}
		else if(timerflag &CN_TCP_TIMER_CORK)
		{
            //CORK TIMER//--TODO
		}
		if(timerflag &CN_TCP_TIMER_ACK)
		{
			//ACK TIMER
			printk("%s:do the acktimer\n\r");
			if(__Tcp_MsgSend(tcb, CN_TCP_FLAG_ACK, NULL, 0, NULL, tcb->sndnxt))
			{
				tcb->timerflag &=(~CN_TCP_TIMER_ACK);
			}
		}
	}

__TIMER_DEALEND:
	return result;
}
// =============================================================================
// �������ܣ�Tcp_Tick
//          TCPЭ��Ķ�ʱ���̵߳Ĵ�����
// �����������
// ���������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void Tcp_Tick(void)
{
	tagSocket    *sockl1;
	tagSocket    *sockl1nxt;
	bool_t       sockl1del;
	tagSocket    *sockl2;
	tagSocket    *sockl2nxt;
	bool_t       sockl2del;
	tagTcpTnode  *tnode;
	tagTcpTcb    *tcb;
	//this is an never return loop
	while(1)
	{
		Lock_MutexPend(pgTcpSockTreeSync, CN_TIMEOUT_FOREVER);
		sockl1 = pgTcpSockLstH;
		while(NULL != sockl1)
		{
			//l1 layer
			Lock_MutexPend(sockl1->sync, CN_TIMEOUT_FOREVER);
			sockl1del = false;
			sockl1nxt = sockl1->nxt;
			if(EN_SOCKET_TREENODE == sockl1->socktype)
			{
				//this is an tree node, must chk its sub
				tnode = (tagTcpTnode *)sockl1->cb;
				sockl2 = tnode->clientlst;
				while(NULL != sockl2)
				{
					//l2 layer
					Lock_MutexPend(sockl2->sync, CN_TIMEOUT_FOREVER);
					sockl2del = false;
					sockl2nxt = sockl2->nxt;
					//do the server session timer deal
					tcb = (tagTcpTcb *)sockl2->cb;
					sockl2del = __Tcp_DealTcbTimer(tcb);

					if(false == sockl2del)
					{
						Lock_MutexPost(sockl2->sync);
					}
					else
					{
						__Tcp_DeleteServerSession(tcb);
					}
					sockl2 = sockl2nxt;
				}
				//tree node has no timer except close
				if((sockl1->appstate == EN_SOCKET_LOCAL_CLOSE)&&\
				   (0 == tnode->connum))
				{
					__Tcp_DeleteServer(sockl1);
					sockl1del = true;
				}
			}
			else if(EN_SOCKET_TERMINAL == sockl1->socktype)
			{
				//do the client timer deal
				tcb = (tagTcpTcb *)sockl1->cb;
				sockl1del = __Tcp_DealTcbTimer(tcb);

				if(true == sockl1del)
				{
					__Tcp_DeleteClient(tcb);
				}
			}
			else
			{
				//not certain yet, if closed ,then free it
				if(sockl1->appstate == EN_SOCKET_LOCAL_CLOSE)
				{
					__Tcp_DeleteSockNode(sockl1);
					sockl1del = true;
				}
			}
			if(false == sockl1del)
			{
				Lock_MutexPost(sockl1->sync);
			}
			sockl1 = sockl1nxt;
		}
		Lock_MutexPost(pgTcpSockTreeSync);
		//the thread delay for the tcp tick
		Djy_EventDelay(CN_TCP_TICK_TIME);
	}
	return;
}

// =============================================================================
// �������ܣ�Tcp_Init
//          TCPЭ���ʼ������
// ���������para,��ʱ������
// ���������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :TCP�ĳ�ʼ���Ķ������������
//       1,����TCB�ڴ��
//       2,������ʱ���߳�
//       3,���������߳�
// =============================================================================
bool_t Tcp_Init(ptu32_t para)
{
	u16  evttid;
	tagTlayerProto proto;
	//�����׽�����Դ�ڵ���
	pgTcpSockTreeSync = Lock_MutexCreate(NULL);
	if(NULL == pgTcpSockTreeSync)
	{
		printk("Tcp:Create Local Mutex Failed\n\r");
		goto createlmutex_failed;
	}
	else
	{
		printk("Tcp:Create Local Mutex  Success\n\r");
	}
	//����TCB�ڴ��
	pgTcpTcbPool = Mb_CreatePool((void *)sgTcpTcbPool, CN_TCP_TCBSIZE,\
			                     sizeof(tagTcpTcb), 10, 0, NULL);
	if(NULL == pgTcpTcbPool)
	{
		printk("Tcp:Create TcpTcbPool Failed\n\r");
		goto createtcbpool_failed;
	}
	else
	{
		printk("Tcp:Create TcpTcbPool  Success\n\r");
	}
	//����TNODE�ڴ��
	pgTcpTnodePool = Mb_CreatePool((void *)sgTcpTnodePool, CN_TCP_TNODESIZE,\
			                     sizeof(tagTcpTnode), 10, 0, NULL);
	if(NULL == pgTcpTnodePool)
	{
		printk("Tcp:Create TcpTnodePool Failed\n\r");
		goto createtnodepool_failed;
	}
	else
	{
		printk("Tcp:Create TcpTnodePool  Success\n\r");
	}
	//create the unacked node mem
	__Tcp_UnackNodeInit();
	//create the recombination mem
	pgTcpReCombNodePool = Mb_CreatePool((void *)sgTcpReCombNodePool, CN_TCP_RECOMB_SECNODE,\
			             sizeof(tagTcpReCombNode), CN_TCP_RECOMB_SECNODE, CN_TCP_RECOMB_SECNODE*100, NULL);
	if(NULL == pgTcpReCombNodePool)
	{
		printk("Tcp:Create UnackedNodePool Failed\n\r");
		goto createrecombnodepool_failed;
	}	
	//ע��TCP������㣬�����׽���ʹ��
	proto.socket = Tcp_Socket;
	proto.accept = Tcp_Accept;
	proto.bind = Tcp_Bind;
	proto.connect = Tcp_Connect;
	proto.getsockopt = Tcp_Getsockopt;
	proto.listen = Tcp_Listen;
	proto.recv = Tcp_Recv;
	proto.recvfrom = NULL;
	proto.send = Tcp_Send;
	proto.sendto = NULL;
	proto.setsockopt = Tcp_Setsockopt;
	proto.shutdown = Tcp_Shutdown;
	proto.close = Tcp_Close;
	proto.multiioadd = Tcp_MultiIoAdd;
	
	pgTcpProtoRsc = TPL_RegisterProto(AF_INET,SOCK_STREAM,0, &proto);
	if(NULL == pgTcpProtoRsc)
	{
		printk("Tcp:Register Tcp in TPL Failed\n\r");
		goto resistertcp_failed;
	}
	else
	{
		printk("Tcp:Register Tcp in TPL Success\n\r");
	}

	//������ʱ���߳�
    evttid= Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1, 
    		(ptu32_t (*)(void))Tcp_Tick,NULL, 0x400, "TcpTick");
	if (evttid != CN_EVTT_ID_INVALID)
	{
		evttid = Djy_EventPop(evttid, NULL, 0, 0, 0, 0);
	}
	else
	{
		printk("Tcp:Register TcpTick Failed\n\r");
	}

	//init the syn option
	//���MSSѡ��
	sgSynOption.mss.kind = EN_TCP_OPT_MSS;
	sgSynOption.mss.len = 4;
	sgSynOption.mss.mss = htons(CN_TCP_MSS_DEFAULT);
	sgSynOption.winscale.resvered = EN_TCP_OPT_NONE;
	sgSynOption.winscale.kind = EN_TCP_OPT_SCALE;
	sgSynOption.winscale.len = 3;
	sgSynOption.winscale.shiftbits = 0;

	return true;
	
resistertcp_failed:
createrecombnodepool_failed:
	Mb_DeletePool(pgTcpTnodePool);
	pgTcpTnodePool = NULL;

createtnodepool_failed:
	Mb_DeletePool(pgTcpTcbPool);
    pgTcpTcbPool = NULL;
    
createtcbpool_failed:
	Lock_MutexDelete(pgTcpSockTreeSync);
	pgTcpSockTreeSync = NULL;

createlmutex_failed:
	return false;
}



