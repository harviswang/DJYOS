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
// ģ������: telnet.c
// ģ��汾: V1.00
// ������Ա: ZQF
// ����ʱ��: ����2:12:24/2015-1-26
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "endian.h"
#include "string.h"

#include "os.h"
#include "stddev.h"

#include "socket.h"

#define CN_TELNET_SERVERPORT  23

//��������ϢʱӦ���ǰ���Ϣ�㲥�����еĿͻ���
typedef struct _tagTelCB
{
	struct _tagTelCB *pre;
	struct _tagTelCB *nxt;
	int sockfd;
	u32  status; //����������֤��״̬
}tagTelCB;

struct tagMutexLCB *pgTelClientSync;       //�ͻ��˶���ͬ��
tagTelCB  *pgTelClst = NULL;               //���еĿͻ��˶���
tagTelCB  *pgTelCBLst = NULL;              //���еĿͻ��˶���
#define CN_TELNET_CLIENTMAX    0X10
tagTelCB  tgTelCBTab[CN_TELNET_CLIENTMAX];               //�ͻ��˿��ƿ�
struct tagMultiplexSetsCB        *pgTelReadSets = NULL;  //�ͻ��˷�����Ϣ��
static s32  sgTelnetDevStdin = -1;                       //telnetģ��������ն�
static s32  sgTelnetDevStdout = -1;                      //telnetģ�������ն�
// =============================================================================
// �������ܣ�����һ���ͻ��˿��ƿ�
// �����������
// �����������
// ����ֵ  �����䵽�Ŀͻ�����Դ���ƿ�
// ˵��    :
// =============================================================================
tagTelCB *__Telnet_MallocCB()
{
	atom_low_t  atom;
	tagTelCB *result = NULL;

	atom = Int_LowAtomStart();
	if(NULL != pgTelCBLst)
	{
		result = pgTelCBLst;
		pgTelCBLst = result->nxt;
		pgTelCBLst->pre = NULL;
		result->nxt = NULL;
		result->sockfd = -1;
	}
	Int_LowAtomEnd(atom);
	return result;
}
// =============================================================================
// �������ܣ��ͷ�һ���ͻ��˿��ƿ�
// �����������
// �����������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void __Telnet_FreeCB(tagTelCB *client)
{
	atom_low_t  atom;
	atom = Int_LowAtomStart();
	if(NULL != client)
	{
		client->pre = NULL;
		client->nxt = pgTelCBLst;
		if(NULL != pgTelCBLst)
		{
			pgTelCBLst->pre = client;
		}

	}
	Int_LowAtomEnd(atom);
	return ;
}
// =============================================================================
// �������ܣ����һ���ͻ���
// ���������client,�ͻ��˿��ƿ�
// �����������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void __Telnet_AddClient(tagTelCB *client)
{
	if(Lock_MutexPend(pgTelClientSync, CN_TIMEOUT_FOREVER))
	{
		client->pre = NULL;
		client->nxt = pgTelClst;
		if(NULL != pgTelClst)
		{
			pgTelClst->pre = client;
		}
		pgTelClst = client;
		Lock_MutexPost(pgTelClientSync);
	}
	return;
}

// =============================================================================
// �������ܣ�ɾ��һ���ͻ���
// ���������client,�ͻ��˿��ƿ�
// �����������
// ����ֵ  ����
// ˵��    :
// =============================================================================
void __Telnet_DelClient(tagTelCB *client)
{
	if(Lock_MutexPend(pgTelClientSync, CN_TIMEOUT_FOREVER))
	{
		if(client == pgTelClst)
		{
			pgTelClst = client->nxt;
			if(NULL != pgTelClst)
			{
				pgTelClst->pre = NULL;
			}
		}
		else
		{
			if(NULL != client->nxt)
			{
				client->nxt->pre = client->pre;
				client->pre->nxt = client->nxt;
			}
			else
			{
				client->pre->nxt = NULL;
			}
		}
		Lock_MutexPost(pgTelClientSync);
	}
	return;
}
// =============================================================================
// �������ܣ�����socket��Ӧ�Ŀͻ���
// ���������client,�ͻ��˿��ƿ�
// �����������
// ����ֵ  ��tagTelCB�����ҵ��Ŀͻ���
// ˵��    :
// =============================================================================
tagTelCB* __Telnet_FindClient(int socket)
{
	tagTelCB *result;
	if(Lock_MutexPend(pgTelClientSync, CN_TIMEOUT_FOREVER))
	{
		result = pgTelClst;
		while(NULL != result)
		{
			if(result->sockfd == socket)
			{
				break;
			}
			else
			{
				result = result->nxt;
			}
		}
		Lock_MutexPost(pgTelClientSync);
	}
	return result;
}

#define CN_TELNET_WELCOM   "Welcome to the djyos telnet console!\n\r"

// =============================================================================
// �������ܣ���������������
// ���������
// ���������
// ����ֵ  ��
// ˵��    :
// =============================================================================
ptu32_t Telnet_Listen(void)
{
	struct sockaddr_in sa_server;
	int sockserver;
	int sockclient;
	tagTelCB *client;
	int sockopt;

	sockserver = socket(AF_INET, SOCK_STREAM, 0);
	sa_server.sin_family = AF_INET;
	sa_server.sin_port = htons(CN_TELNET_SERVERPORT);
	sa_server.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockserver, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
	{
		printk("bind()");
		closesocket(sockserver);
		return 0;
	}

	if (listen(sockserver, 2) != 0)
	{
		printk("listen()");
		closesocket(sockserver);
		return 0;
	}
	//here we accept all the client
	while(1)
	{
		sockclient = accept(sockserver, NULL, 0);
		if(sockclient > 0)
		{
			//set it no nagle
			sockopt = 0;
			if(0 !=setsockopt(sockclient,IPPROTO_TCP,TCP_NODELAY,&sockopt,sizeof(sockopt)))
			{
				printk("Telnet:Set client nodelay failed");
			}
		    //snd the welcom msg
			send(sockclient,CN_TELNET_WELCOM,strlen(CN_TELNET_WELCOM)+1,0);
			//add the client to the read sets
			Socket_MultiplexAdd(pgTelReadSets,sockclient,CN_SOCKET_READALBE);
			client = __Telnet_MallocCB();
			if(NULL != client)
			{
				client->sockfd = sockclient;
				client->status = 0;
				__Telnet_AddClient(client);
			}
		}
	}
	return 0;//never reach here
}
#define CN_TELNET_MSGLENMAX 0x100
// =============================================================================
// �������ܣ����������մӿͻ��˷���������Ϣ
// ���������
// ���������
// ����ֵ  ��
// ˵��    :
// =============================================================================
ptu32_t Telnet_Rcv(void)
{
	int sockclient;
	tagTelCB *client;
	struct tagCharTermineralMsg msg={0,0};
	while(1)
	{
		sockclient = Multiplex_Wait(pgTelReadSets,NULL, CN_TIMEOUT_FOREVER);
		if(sockclient > 0)
		{
			msg.num = recv(sockclient,msg.input_char,CN_CHAR_BUF_LIMIT,0);
			if(msg.num > 0)
			{
//				msg.input_char[msg.num] = '\0';
//				printk("%s",msg.input_char);
				//ok, snd the msg to the stdin device
				msg.time = DjyGetTimeTick();
				Stddev_InputMsg(sgTelnetDevStdin,(u8*)&msg,sizeof(msg));
				//and snd the msg to the terminal, this will means that echo display
//				send(sockclient,msg.input_char,msg.num,0);
			}
			else
			{
				//find the client and del it from the queue, then free it
				client = __Telnet_FindClient(sockclient);
				__Telnet_DelClient(client);
				__Telnet_FreeCB(client);
				//del the multiio from the sets,and then shudown the client	
				Socket_MultiplexDel(pgTelReadSets,sockclient);
				shutdown(sockclient,SHUT_RDWR);
				closesocket(sockclient);
			}
		}
	}
	return 0;
}
// =============================================================================
// �������ܣ�����ϵͳ����Ϣ���͸��ͻ���
// ���������
// ���������
// ����ֵ  �����͵��ֽ�����-1 ����ʧ��
// ˵��    :
// =============================================================================
void Telnet_Snd(char *msg)
{
	int       len;
	tagTelCB  *client;
	if(NULL != msg)
	{
		len = strlen(msg);
		if(Lock_MutexPend(pgTelClientSync, CN_TIMEOUT_FOREVER))
		{
			//check all the client list
			client = pgTelClst;
			while(NULL != client)
			{
				if(client->sockfd > 0)
				{
					send(client->sockfd,msg,len,0);
					client = client->nxt;
				}
			}
			Lock_MutexPost(pgTelClientSync);
		}
	}
	return ;
}

bool_t __Telnet_TaskCreate(ptu32_t (*taskfunc)(void), char *taskname)
{
	bool_t result = false;
	u16    taskID;
	taskID = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1, 
			taskfunc,NULL, 0x400, taskname);
	if (taskID != CN_EVTT_ID_INVALID)
	{
		taskID = Djy_EventPop(taskID, NULL, 0, 0, 0, 0);
		if(taskID != CN_EVENT_ID_INVALID)
		{
			result = true;
		}
	}
	return result;
}

// =============================================================================
// �������ܣ�TelNet�������ĳ�ʼ��
// ���������
// ���������
// ����ֵ  ��true �ɹ�  falseʧ��
// ˵��    :
//       1,�����������������߳�
//       2,�����ͻ��˽������߳�
// =============================================================================
ptu32_t Telnet_Init(char *param)
{
	u32    i;
	//init the client CB
	memset((char *)&tgTelCBTab[0],0, sizeof(tagTelCB)*CN_TELNET_CLIENTMAX);
	pgTelCBLst = &tgTelCBTab[0];
	pgTelCBLst->sockfd = -1;
	for(i = 1;i <CN_TELNET_CLIENTMAX; i++)
	{
		tgTelCBTab[i].nxt = pgTelCBLst;
		pgTelCBLst->pre = &tgTelCBTab[i];
		pgTelCBLst = pgTelCBLst->pre;
		pgTelCBLst->sockfd = -1;
	}
	//create the read sets and mutex sync
	pgTelReadSets = Multiplex_Creat(CN_SOCKET_OR | CN_SOCKET_READALBE);
	if(NULL == pgTelReadSets)
	{
		printk("Telnet:Create ReadSets Failed\n\r");
	}
	pgTelClientSync = Lock_MutexCreate(NULL);
	if(NULL == pgTelClientSync)
	{
		printk("Telnet:Create Client Sync Failed\n\r");
	}
	
	//add the telnet to the stdin device tree
    sgTelnetDevStdin = Stddev_InstallDevice("netconsole", EN_CHAR_TERMINERAL,NULL);
    if(-1 == sgTelnetDevStdin)
    {
    	printk("Telnet:Install Stdin Device Failed\n\r");
    }
	//add the telnet to the stdout device tree

	//create server listen task
	if(false == __Telnet_TaskCreate(Telnet_Listen,"TelnetServerListen"))
	{
		printk("Telnet:Create Listen Task Failed!\n\r");
	}
	//create server rcv task
	if(false == __Telnet_TaskCreate(Telnet_Rcv,"TelnetServerRcv"))
	{
		printk("Telnet:Create Rcv Task Failed!\n\r");
	}
	
	//set the printf stdout device
	//printf_setstdout(Telnet_Snd);

	return 0;
}

