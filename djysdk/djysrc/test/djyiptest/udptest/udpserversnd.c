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
// ģ������: TCP���������Ͳ���
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 9:23:45 AM/Jul 7, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "os.h"

#include "Socket.h"
#include "endian.h"

#define CN_UDP_SERVERPORT        2048
#define CN_BUF_LEN               1400
#define CN_SOCKBUF_LEN           (CN_BUF_LEN*10)

bool_t UdpServer_Snd(void)
{
	int sockfd;
	int clientfd;
	int addrlen;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	
	char *rcvbuf;
	int  rcvlen;
	int sndopt;


	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sockfd)
	{
		printk("socket failed!\n\r");
		return false;
	}
	else
	{
		printk("socket success!\n\r");
	}

	serveraddr.sin_port = htons(CN_UDP_SERVERPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(-1==bind(sockfd, &serveraddr, sizeof(serveraddr)))
	{
		printk("bind failed!\n\r");
		return false;
	}
	else
	{
		printk("bind success!\n\r");
	}

	if(-1 == listen(sockfd, 100))
	{
		printk("listen failed!\n\r");
		return false;
	}
	else
	{
		printk("listen success!\n\r");
	}

	while(1)
	{
    	clientfd = accept(sockfd, &clientaddr, &addrlen);
    	if(clientfd != -1)
    	{
    		printk("Got an client:ip = %08x  port = %04x\n\r",\
    				     ntohl(clientaddr.sin_addr.s_addr),ntohs(clientaddr.sin_port));
    		//�ر�NAGLE
    		sndopt = 1;
    		if(0 == setsockopt(clientfd, IPPROTO_TCP,TCP_NODELAY,&sndopt,4))
    		{
    			printk("Client:close nagle success!\n\r");
    		}
    		else
    		{
    			printk("Client:close nagle  failed!\n\r");
    		}
    		//���÷��ͻ�����
    		sndopt = CN_SOCKBUF_LEN;
    		if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDBUF,&sndopt,4))
    		{
    			printk("Client:set client sndbuf success!\n\r");
    		}
    		else
    		{
    			printk("Client:set client sndbuf failed!\n\r");
    		}
    		//���÷��ͻ���������ˮƽ
    		rcvbuf = M_Malloc(CN_BUF_LEN,CN_TIMEOUT_FOREVER);
    		while(1)
    		{
				rcvlen = recv(clientfd,rcvbuf,CN_BUF_LEN,0);
				if(rcvlen > 0)
				{
					rcvbuf[rcvlen] = '\0';
					printk("RcvMsg: %s\n\r",rcvbuf);
				}
				else
				{
					printk("TcpServerSnd:snd failed!\n\r");
					closesocket(clientfd);
					break;
				}
				Djy_EventDelay(1000*mS);
    		}
    	}
    	else
    	{
    		printk("Bad Accept!\n\r");
    	}
	}
	closesocket(sockfd);
	return true;
}


bool_t UdpServer_SndTask(char *param)
{
   u16   evtt_id = CN_EVTT_ID_INVALID;
   evtt_id = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1,
   		(ptu32_t (*)(void))UdpServer_Snd,NULL, 0x1000, "UdpServer_Snd");
	if (evtt_id != CN_EVTT_ID_INVALID)
	{
		Djy_EventPop(evtt_id, NULL, 0, NULL, 0, 0);
	}
	return true;
}





