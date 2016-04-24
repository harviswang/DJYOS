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
// ģ������: ���ļ���Ҫ�ǹ����׽��ֵ�ʵ��,�ṩ����BSD��ͳһ�ӿ�ģ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:15:23 PM/Jul 15, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "os.h"

#include "socket.h"
#include "tpl.h"
//������ڴ��
//�׽����ڴ��
#define CN_SOCKET_POOLSIZE   0x10
static tagSocket  sgSocketPool[CN_SOCKET_POOLSIZE];
static struct tagMemCellPool  *pgSocketPool = NULL;

#define CN_SOCKET_MAP_2USE         1
static tagSocket *sgSocketMap[CN_SOCKET_POOLSIZE];

// =============================================================================
// �������ܣ�  __Socket_NewNo
//        alloc an new no to link the socket
// ���������  ��
// ��������� 
// ����ֵ  �� the link no if success, otherwise -1 failed
// ˵��    ��
// =============================================================================
int __Socket_NewNo()
{
	static int newno = 0;
	int result;
	int i ;
	
	result = -1;
	atom_low_t atom;
	atom = Int_LowAtomStart();
	for(i =0; i <CN_SOCKET_POOLSIZE; i++ )
	{
		if(NULL == sgSocketMap[newno])
		{
			sgSocketMap[newno]= (tagSocket *)CN_SOCKET_MAP_2USE;
			result = newno;
			newno = (newno+1)%CN_SOCKET_POOLSIZE;
			i = CN_SOCKET_POOLSIZE;
		}
	}
	Int_LowAtomEnd(atom);
	
	return result;
}

// =============================================================================
// �������ܣ�  __Socket_FreeInt
//        Free an link no
// ���������  the no to free
// ��������� 
// ����ֵ  ��true success while false failed
// ˵��    ��
// =============================================================================
bool_t __Socket_FreeInt(int fd)
{
	bool_t result;
	atom_low_t atom;

	result = false;	
	atom = Int_LowAtomStart();
	if((fd < CN_SOCKET_POOLSIZE)&&(fd >= 0))
	{
		sgSocketMap[fd] = (tagSocket *)NULL;
		result = true;
	}
	Int_LowAtomEnd(atom);
	
	return result;
}

// =============================================================================
// �������ܣ�  __Socket_GetSocket
//           ͨ���׽��ֺŻ�ȡ�׽���
// ���������  �׽��ֺ�
// ��������� 
// ����ֵ  ����ȡ���׽���
// ˵��    ��NULL��ʾ��������ȷ������û�и��׽���
// =============================================================================
tagSocket * __Socket_GetSocket(int fd)
{
	tagSocket *result;	
	atom_low_t atom;
	
	result = NULL;
	atom = Int_LowAtomStart();
	if((fd < CN_SOCKET_POOLSIZE)&&(fd >= 0))
	{
		result = sgSocketMap[fd]; 
	}
	Int_LowAtomEnd(atom);
	
	return result;
}
// =============================================================================
// �������ܣ�  socket
//        create an new socket
// ��������� family,��������AF_INET AF_LOCAL��
//        type,���ͣ���SOCK_STREAM SOCK_DGRAM��
//        protocol,Э�����ͣ�һ���Ĭ��Ϊ0
// ���������
// ����ֵ  ���׽��ֺţ�-1��ʾ����ʧ��
// ˵��    ���׽��ִ������ɹ���������ָ��Э��û���ҵ������ڴ治��
// =============================================================================
//����һ���׽���
int socket(int family, int type, int protocol)
{
	int sockfd;
	tagTlayerProto  *proto;
	tagSocket      *sockstr;
	
	sockstr = NULL;
	sockfd = __Socket_NewNo();
	if(-1 != sockfd)
	{
		proto = TPL_GetProto(family, type, protocol);
		if(NULL != proto)
		{
			if(NULL != proto->socket)
			{
				sockstr = proto->socket(family, type, protocol);
				if(NULL != sockstr)
				{
					sgSocketMap[sockfd] = sockstr;
				}
			}
		}
	}
	if(NULL == sockstr)
	{
		__Socket_FreeInt(sockfd);
		sockfd = -1;
	}

	return  sockfd;
}

// =============================================================================
// �������ܣ�  bind
//           Ϊһ���׽��ְ�һ�������ַ
// ���������  sockfd,���󶨵��׽���
//           myaddr,ָ���������ַ
//           addrlen,�����ַ����
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�������ַ��ͻ���˿ںţ�
// =============================================================================
//����һ���׽���
int bind(int sockfd,struct sockaddr *myaddr, int addrlen)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->bind)
			{
				result = sockstr->proto->bind(sockstr, myaddr, addrlen);
			}
		}
	}

	
	return  result;
}
// =============================================================================
// �������ܣ�  listen
//           ָ���׽���Ϊ����״̬
// ���������  sockfd,��ָ�����׽���
//           backlog,��������Ӹ�����Ĭ�ϵ�Ϊ5
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�ظ�ָ��
// =============================================================================
int listen(int sockfd, int backlog)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->listen)
			{
				result = sockstr->proto->listen(sockstr, backlog);
			}
		}
	}

	return  result;

}
// =============================================================================
// �������ܣ�  accept
//           �ȴ��ͻ��˵�����
// ���������  sockfd,���������׽���
// ���������  addr,���ӵĿͻ��˵�ַ
//           addrlen����ַ����
// ����ֵ  �����ӵĿͻ����׽��֣�-1����
// ˵��    ������һ������Ϊ����������û�����ӵĿͻ��ˣ����߳�������������
// =============================================================================
int accept(int sockfd, struct sockaddr *addr, int *addrlen)
{
	int  result;
	tagSocket *sockstr;
	tagSocket *consocket;
	
	sockstr = NULL;
	result = __Socket_NewNo();
	if(-1 != result)
	{

		sockstr = __Socket_GetSocket(sockfd);
		if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
		{
			if(NULL != sockstr->proto)
			{
				if(NULL != sockstr->proto->accept)
				{
					consocket = sockstr->proto->accept(sockstr, addr,addrlen);
					if(NULL != consocket)
					{
						sgSocketMap[result] = consocket;
					}
				}
			}
		}
	
		if(NULL == consocket)
		{
			__Socket_FreeInt(result);
			result = -1;
		}
	}
	
	return  result;
}

// =============================================================================
// �������ܣ�  connect
//           ���ӷ�����
// ���������  sockfd,�ͻ����׽���
//           addr,���ӵķ�������ַ
//           addrlen����ַ����
// ���������  
// ����ֵ  ��-1����0�ɹ�
// ˵��    ��
// =============================================================================
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen) 
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->connect)
			{
				result = sockstr->proto->connect(sockstr, serv_addr, addrlen);
			}
		}
	}

	return  result;
}
// =============================================================================
// �������ܣ�  send
//           ���׽��ַ�������
// ���������  sockfd,Ŀ���׽���
//           msg,�����͵���Ϣ
//           len,��Ϣ����
//           flags,һ��Ϊ0
// ���������  
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��
// =============================================================================
int send(int sockfd, const void *msg, int len, int flags)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->send)
			{
				result = sockstr->proto->send(sockstr, msg, len, flags);
			}
		}
	}

	return  result;	
}
// =============================================================================
// �������ܣ�  recv
//           ��ȡ��������
// ���������  sockfd,Ŀ���׽���
//           flags,һ��Ϊ0
// ���������  
//           buf,�յ�����Ϣ
//           len,buf����
// ����ֵ  ��-1�������򷵻��յ��ֽ���
// ˵��    ��
// =============================================================================
int recv(int sockfd, void *buf,int len, unsigned int flags)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->recv)
			{
				result = sockstr->proto->recv(sockstr, buf, len, flags);
			}
		}
	}

	return  result;
}
// =============================================================================
// �������ܣ�  sendto
//           ��Ŀ�������ַ��������
// ���������  sockfd,Ŀ���׽���
//           msg,�����͵���Ϣ
//           len,��Ϣ����
//           flags,һ��Ϊ0
//           addr,Ŀ�������ַ
//           addrlen,Ŀ�������ַ����
// ��������� 
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��
// =============================================================================
int sendto(int sockfd, const void *msg,int len, unsigned int flags,\
		  const struct sockaddr *addr, int addrlen)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->sendto)
			{
				result = sockstr->proto->sendto(sockstr, msg, len, flags,addr, addrlen);
			}
		}	
	}

	
	return  result;	
}
// =============================================================================
// �������ܣ�  recvfrom
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
int recvfrom(int sockfd,void *buf, int len, unsigned int flags,\
		    struct sockaddr *addr, int *addrlen)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{	
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->recvfrom)
			{
				result = sockstr->proto->recvfrom(sockstr, buf, len, flags,addr, addrlen);
			}
		}
	}

	return  result;	
}
// =============================================================================
// �������ܣ�  shutdown
//           �ر��׽���ͨ��
// ���������  sockfd,Ŀ���׽���
//           how,�رշ����� ��SHUT_RD�ȶ���
// ��������� 
// ����ֵ  ��falseʧ��  true �ɹ�
// ˵��    ��
// =============================================================================
bool_t  shutdown(int sockfd, u32 how)
{
	bool_t  result;
	tagSocket *sockstr;
	
	result = false;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->shutdown)
			{
				result = sockstr->proto->shutdown(sockstr, how);
			}
		}	
	}

	return  result;	
}

// =============================================================================
// �������ܣ�  close
//           �ر��׽���
// ���������  sockfd,Ŀ���׽���
// ���������
// ����ֵ  ��falseʧ��  true �ɹ�
// ˵��    ��
// =============================================================================
bool_t closesocket(int sockfd)
{
	bool_t  result;
	tagSocket *sockstr;

	result = false;
	sockstr = __Socket_GetSocket(sockfd);
	if((NULL != sockstr)&&((tagSocket *)CN_SOCKET_MAP_2USE != sockstr))
	{
		if(NULL != sockstr->proto)
		{
			if(NULL != sockstr->proto->close)
			{
				result = sockstr->proto->close(sockstr);
				if(result)
				{
					__Socket_FreeInt(sockfd);
				}
			}
		}
	}

	return  result;
}
// =============================================================================
// �������ܣ�  setsockopt
//        �����׽���ѡ��
// ���������  sockfd,Ŀ���׽���
//        level,���õĲ�Σ�֧��SOL_SOCKET��IPPROTO_TCP��IPPROTO_IP��IPPROTO_IPV6
//        optname,������ѡ��
//        optval,������
//        optlen,����������
// ��������� 
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int setsockopt(int sockfd, int level, int optname,\
		       const void *optval, int optlen)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if(NULL != sockstr->proto)
	{
		if(NULL != sockstr->proto->setsockopt)
		{
			result = sockstr->proto->setsockopt(sockstr, level, optname,\
					                            optval,optlen);
		}
	}
	return  result;		
}
// =============================================================================
// �������ܣ�  getsockopt
//        �����׽���ѡ��
// ���������  sockfd,Ŀ���׽���
//        level,���õĲ�����磬֧�ֵĲ����SOL_SOCKET��IPPROTO_TCP
//        optname,������ѡ��
//        optval,������
//        optlen,����������
// ��������� 
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
int getsockopt(int sockfd, int level, int optname, void *optval,\
		       int *optlen)
{
	int  result;
	tagSocket *sockstr;
	
	result = -1;
	sockstr = __Socket_GetSocket(sockfd);
	if(NULL != sockstr->proto)
	{
		if(NULL != sockstr->proto->getsockopt)
		{
			result = sockstr->proto->getsockopt(sockstr, level, optname,\
					                            optval,optlen);
		}
	}

	return  result;		
}

// =============================================================================
// �������ܣ�����׽��ֵ�ָ���Ķ��ø��ü���     
// ���������  MultiplexSets,ָ�����׽��ּ���
//        sock,��������׽���
//        SensingBit,��Ҫ������λ
// ��������� 
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    ��
// =============================================================================
bool_t Socket_MultiplexAdd(struct tagMultiplexSetsCB *MultiplexSets,
                           int sock,u32 SensingBit)
{
	bool_t result;
	tagSocket *sockstr;
	
	result = false;
	if(NULL != MultiplexSets)
	{
		sockstr = __Socket_GetSocket(sock);
		if((NULL != sockstr)&&(NULL != sockstr->proto))
		{
			if(NULL != sockstr->proto->multiioadd)
			{
				result = sockstr->proto->multiioadd(MultiplexSets,\
						(ptu32_t)sock, SensingBit, sockstr);
			}
		}
	}
    
	return result;
}
// =============================================================================
// �������ܣ����׽��ִ�ָ���Ķ��ø��ü���ɾ��     
// ���������  MultiplexSets,ָ�����׽��ּ���
//        sock,��ɾ�����׽���
// ��������� 
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    ��
// =============================================================================
bool_t Socket_MultiplexDel(struct tagMultiplexSetsCB *MultiplexSets,int sock)
{
	bool_t result;
	tagSocket *sockstr;
	
	result = false;
	if(NULL != MultiplexSets)
	{
		sockstr = __Socket_GetSocket(sock);
		if((NULL != sockstr)&&(NULL != sockstr->proto))
		{
			if(NULL != sockstr->proto->multiiodel)
			{
				result = sockstr->proto->multiiodel(MultiplexSets,\
						(ptu32_t)sock, sockstr);
			}
		}
	}
    
	return result;
}

static tagSocket* sgDebugSocket;
// =============================================================================
// �������ܣ�  Socket_New
//           �����׽����ڴ�ռ�
// ��������� ��
// ��������� ��
// ����ֵ  ��NULL��ʾ���䲻�ɹ����ڴ治�㣩�����򷵻ط�����׽��ֽṹ
// ˵��    ������ʱ�ռ��Ѿ�����ʼ�����
// =============================================================================
tagSocket* Socket_New(void)
{
	tagSocket *result;
	result = Mb_Malloc(pgSocketPool, CN_TIMEOUT_FOREVER);
	if(NULL != result)
	{
		memset(result,0,sizeof(tagSocket));
		result->sync = Lock_MutexCreate(NULL);
		if(NULL != result->sync)
		{
			//init the socket
			result->valid = true;
			result->socktype = EN_SOCKET_NOTSURE;
			sgDebugSocket = result;
		}
		else
		{
			Mb_Free(pgSocketPool, (void *)result);
			result = NULL;
		}
	}
	return result;
}
// =============================================================================
// �������ܣ�  Socket_Free
//           �ͷ��׽����ڴ�ռ�
// ��������� fd, ���ͷŵ��׽���
// ��������� ��
// ����ֵ  ����
// ˵��    ��
// =============================================================================
bool_t Socket_Free(tagSocket *sockfd)
{
	if(NULL != sockfd)
	{
		Lock_MutexDelete(sockfd->sync);
		sockfd->valid = false;
		sockfd->socktype = EN_SOCKET_NOTSURE;

		Mb_Free(pgSocketPool, (void *)sockfd);
		return true;
	}
	else
	{
		return false;
	}
}

// =============================================================================
// �������ܣ�  Socket_Init
//           
// ���������para��Ŀǰ������ 
// ��������� 
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ��1,�����׽�������Ҫ���ڴ��
// =============================================================================
bool_t Socket_Init(ptu32_t para)
{
	bool_t result;
	int i;
	//�׽����ڴ��
	pgSocketPool = Mb_CreatePool((void *)sgSocketPool, CN_SOCKET_POOLSIZE,\
			                     sizeof(tagSocket), 0, 0, "Socket Pool");
	if (NULL == pgSocketPool)
	{
		printk("Socket:Creat SocketPool Failed\n\r");
		result =  false;
	}
	else
	{
		
		printk("Socket:Creat SocketPool Success\n\r");
		for(i =0; i < CN_SOCKET_POOLSIZE; i++)
		{
			sgSocketMap[i] = NULL;
		}
		result = true;
	}	
	
	return result;
}




