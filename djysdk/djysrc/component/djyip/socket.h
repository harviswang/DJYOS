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
// ģ������: ���ļ��׽��ֵĶ��壬�Լ�ʹ�õĴ����Ľӿڱ�׼�Ķ���
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:15:08 PM/Jul 15, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef SOCKET_H_
#define SOCKET_H_
#include "stdint.h"
#include "stddef.h"

#include "os.h"
#include "multiplex.h"

#include "pkg.h"
#include "tcpip_cdef.h"
// family
#define AF_UNSPEC		(0) /* unspecified */
#define AF_LOCAL		(1) /* local to host (pipes, portals) */
#define AF_UNIX			AF_LOCAL /* backward compatibility */
#define AF_INET			(2) /* internetwork: UDP, TCP, etc. */
#define AF_IMPLINK		(3) /* arpanet imp addresses */
#define AF_PUP			(4) /* pup protocols: e.g. BSP */
#define AF_CHAOS        (5) /* mit CHAOS protocols */
#define AF_NS			(6) /* XEROX NS protocols */
#define AF_ISO			(7) /* ISO protocols */
#define AF_OSI			AF_ISO
#define AF_ECMA			(8) /* european computer manufacturers */
#define AF_DATAKIT		(9) /* datakit protocols */
#define AF_CCITT        (10) /* CCITT protocols, X.25 etc */
#define AF_SNA			(11) /* IBM SNA */
#define AF_DECnet		(12) /* DECnet */
#define AF_DLI			(13) /* DEC Direct data link interface */
#define AF_LAT			(14) /* LAT */
#define AF_HYLINK		(15) /* NSC Hyperchannel */
#define AF_APPLETALK    (16) /* Apple Talk */
#define AF_ROUTE        (17) /* Internal Routing Protocol */
#define AF_LINK			(18) /* Link layer interface */
#define pseudo_AF_XTP	(19) /* eXpress Transfer Protocol (no AF) */
#define AF_COIP			(20) /* connection-oriented IP, aka ST II */
#define AF_CNT			(21) /* Computer Network Technology */
#define pseudo_AF_RTIP	(22) /* Help Identify RTIP packets */
#define AF_IPX			(23) /* Novell Internet Protocol */
#define AF_SIP			(24) /* Simple Internet Protocol */
#define pseudo_AF_PIP	(25) /* Help Identify PIP packets */

// type
#define SOCK_STREAM		1        // �ɿ���˫���ֽ������� - TCP
#define SOCK_DGRAM		2        // ��õ���������ݱ����� - UDP
#define SOCK_RAW        3        // ��õ���������ݱ����� - ICMP IGMP ԭʼIP
#define SOCK_RDW        4        // �ɿ������ݱ�����δʵ�֣� - n/a
#define SOCK_SEQPACKET	5        // �ɿ���˫���¼������ - n/a

//BSD��׼�������ַ�ṹ
//�ں�ʹ�õ�sockaddr�ṹ
struct sockaddr
{
    u16 sa_family;    // ��ַ�壬AF_INET/AF_ISO/AP_UNIX������AF_xxx
    char sa_data[14]; // 14 �ֽڵ�Э���ַ �������socket�Ķ˿ںź�IP��ַ
};
struct in_addr 
{ 
	union 
	{ 
		struct { u8 s_b1,s_b2,s_b3,s_b4; } S_un_b; 
		struct { u16 s_w1,s_w2; } S_un_w; 
		unsigned long S_addr; 
	} S_un;
#define s_addr S_un.S_addr
};
// Ϊ�˸�����ʹ��sockaddr�������Զ���һ��socketaddr�ṹ���û�����ʹ�ñ��ṹ�����ò���
struct sockaddr_in
{
    u16             sin_family;    // ��ַ�壬AF_INET/AF_ISO/AP_UNIX������AF_xxx
    u16             sin_port;      // �˿ڵ�ַ�������ֽ���
    struct in_addr  sin_addr;      // IP��ַ
    u8              sin_zero[8];   // ����ֽ�
};

//�׽���ѡ�����ö���:level
enum _EN_SETSOCKOPT_LEVEL
{
	//LEVEL        //MEANS
	SOL_SOCKET=0,  //ͨ���׽���ѡ��
	IPPROTO_IP,    //IPѡ��
	IPPROTO_TCP,   //tcpѡ��
};

//SOL_SOCKET
enum _EN_SOL_SOCKET_OPTNAME
{
	//OPTNAME              MEANS                              optvaltype	
	SO_BROADCAST = 0 , //�����͹㲥���ݡ�����������������������int
	SO_DEBUG,          //������ԡ�������������������������������int
	SO_DONTROUTE,      //������·�ɡ�����������������������������int
	SO_ERROR,          //����׽��ִ��󡡡�����������������������int
	SO_KEEPALIVE,      //�������ӡ�������������������������������int
	SO_LINGER,         //�ӳٹر����ӡ���������������������������struct linger
	SO_OOBINLINE,      //�������ݷ�������������������������������int
	SO_RCVBUF,         //���ջ�������С��������������������������int
	SO_SNDBUF,         //���ͻ�������С��������������������������int
	SO_RCVLOWAT,       //���ջ��������ޡ�������������������������int
	SO_SNDLOWAT,       //���ͻ��������ޡ�������������������������int
	SO_RCVTIMEO,       //���ճ�ʱ��������������������������������struct timeval
	SO_SNDTIMEO,       //���ͳ�ʱ��������������������������������struct timeval
	SO_REUSERADDR,     //�������ñ��ص�ַ�Ͷ˿ڡ�����������������int
	SO_TYPE,           // ����׽������͡�����������������������  int
	SO_BSDCOMPAT,      //��BSDϵͳ���ݡ������������������������� int
	SO_NOBLOCK,        //DJY ������                                                                         int
};
//IPPROTO_IP
enum _EN_IPPROTO_IP_OPTNAME
{
	//OPTNAME              MEANS                           optvaltype	
	IP_HDRINCL = 0,//�����ݰ��а���IP�ײ���������������������int
	IP_OPTIONS    ,//IP�ײ�ѡ�����������������������������int
	IP_TOS        ,//��������
	IP_TTL        ,//����ʱ�䡡������������������������������int	
};
//IPPROTO_TCP
enum _EN_IPPROTO_TCP_OPTNAME
{
	//OPTNAME              MEANS                           optvaltype	
	TCP_MAXSEG = 0,  //TCP������ݶδ�С��������������������                   int
	TCP_NODELAY   ,  //��ʹ��Nagle�㷨������������������������������int
};
//ȷ��һ��ͨ�ſռ�Ķ���Դ
typedef struct _tagSocketNetAddr
{
	u16 port;
	u32 ip;
}tagSocketNetAddr;

typedef struct _tagSocketConState
{
	u8 noblock: 1;  //������
	u8 asyn:    1;  //�첽
}tagSocketConState;


enum _EN_SOCKET_TYPE
{
	EN_SOCKET_NOTSURE =0,   //not sure yet
	EN_SOCKET_TERMINAL,     //the communication node
	EN_SOCKET_TREENODE,     //the tree node,sometimes means it is an server node
};

enum _EN_SOCKEY_ERRORNO
{
	EN_SOCKET_ERROR_PARAERR =0 ,      //para error
	EN_SOCKET_TRY_AGAIN,              //when there is no data what we need, and
	                                  //the socket is noblock, then the no will be set
	EN_SOCKET_ERROR_SNDPKG2LARGE ,    //the snd buf exceed the sndbuf len
	EN_SOCKET_CHANNEL_SHUTDOWN_W,     //the communication has been shutdown WRITE
	EN_SOCKET_CHANNEL_SHUTDOWN_R,     //the communication has been shutdown READ
};
struct _tagTlayerProto;
typedef struct _tagSocket
{
	struct _tagSocket               *pre;         //pre node
	struct _tagSocket               *nxt;         //nxt node
	struct _tagSocket               *hungnode;    //which means this socket hung on
	u8                              socktype;     //�׽������ͣ���_EN_SOCKET_TYPE����
	bool_t                          valid;        //�Ƿ���Ч
	u8                              appstate;     //Ӧ��״̬��bind connect��
    struct tagMutexLCB              *sync;        //��Դ����ڵ�
    struct tagSemaphoreLCB          *casync;      //used for accept and connect sync

	tagSocketNetAddr                laddr;        //���ض˵�ַ
	tagSocketConState               constate;     //���ض�SOCKET��ͨ�ŷ�ʽ
	struct _tagTlayerProto          *proto;	      //�����Э�� 
	void                            *cb;          //the control node
	                                              //when terminal it is tcb else treenode

	struct tagMultiplexObjectCB     *multiio;     //��·����
	u32                             status;       //�ɶ���д��״̬ 
	u32                             errorno;      //�����������ʱ�����ʹ��
}tagSocket;

enum _EN_SOCKET_LOCALSTATE
{
	EN_SOCKET_LOCAL_RST =0,    //��λ״̬
	EN_SOCKET_LOCAL_CREATE ,   //�����ɹ���
	EN_SOCKET_LOCAL_BIND,      //����BIND��
	EN_SOCKET_LOCAL_LISTEN,    //����Listen��
	EN_SOCKET_LOCAL_CONNECT,   //����connect��
	EN_SOCKET_LOCAL_CLOSE,     //����close�󣬵������ڹ��ص�RFDԭ�򣬵�ʱɾ������
	EN_SOCKET_LOCAL_2FREE,     //���ͷŵģ��Ѿ�ɾ�������е�RFD�������ͷ���
};

// socket�������ṹ
typedef struct _tagSocketBuf
{
	int                       buflen;      //�Ѿ�������������
	int                       buflenlimit; //��buf�ĳ������ƣ������˳�����Ϊ���
    int                       triglevel;   //����ˮƽ
    tagNetPkg                 *pkglsthead; //���ݰ�����ͷ�����ͻ���������Pkg�������ջ���������part����
	tagNetPkg                 *pkglsttail; //����β��
    struct tagSemaphoreLCB    *bufsync;    //��BUF�ռ��б仯ʱ��Ҫ������
}tagSocketBuf;

//lingerѡ��
struct linger
{
	int l_onff;//0 off while 1 on
	int l_linger;//linger time,Units:Seconds
};

//�����Э��ӿڶ���
//����㶨��Ľӿ�,���������Э�飬�ȷ�TCP,UDP�ȱ��밴������ӿ���ʵ��
//���ڲ���Ҫ�ĺ����ӿڣ������ÿ�
//����ֵΪ��׼��BSD����ֵ
typedef struct _tagTlayerProto
{
	//����һ���׽���
	tagSocket* (*socket)(int family, int type, int protocol);
	//��һ���˿ں�
	int (*bind)(tagSocket *sockfd,struct sockaddr *myaddr, int addrlen);
	//��һ���˿ڴ��ڼ���״̬
	int (*listen)(tagSocket *sockfd, int backlog);
	//��һ�������˴��ڽ���״̬(�������Ż�)
	tagSocket* (*accept)(tagSocket *sockfd, struct sockaddr *addr, int *addrlen);
	//��һ���ͻ���ȥ���ӷ�����
	int (*connect)(tagSocket *sockfd, struct sockaddr *serv_addr, int addrlen); 
	//��������
	int (*send)(tagSocket *sockfd, const void *msg, int len, int flags);
	//��������
	int (*recv)(tagSocket *sockfd, void *buf,int len, unsigned int flags);
	//ֱ�ӷ������ݵ�Ŀ�Ķ�
	int (*sendto)(tagSocket * sockfd, const void *msg,int len, unsigned int flags,\
			  const struct sockaddr *addr, int addrlen);
	//ֱ�Ӵ�Ŀ�Ķ˶�ȡ����
	int (*recvfrom)(tagSocket * sockfd,void *buf, int len, unsigned int flags,\
			    struct sockaddr *addr, int *addrlen);
	//�ر�һ���׽ӿ�
#define SHUT_RD   0  //�رն�
#define SHUT_WR   1  //�ر�д
#define SHUT_RDWR 2  //�رն�д
	int (*shutdown)(tagSocket *sockfd, u32 how);
	int (*close)(tagSocket *sockfd);
	
	//�����׽���ѡ��
	int (*setsockopt)(tagSocket *sockfd, int level, int optname,\
			       const void *optval, int optlen);
	//��ȡ�׽���ѡ��
	int (*getsockopt)(tagSocket *sockfd, int level, int optname, void *optval,\
			       int *optlen);
	
	//defines for the socket multiIO 
#define  CN_SOCKET_READALBE   CN_MULTIPLEX_SENSINGBIT_READ   //COULE  READ
#define  CN_SOCKET_WRITEALBE  CN_MULTIPLEX_SENSINGBIT_WRITE  //COULD  WRITE, which means the 
#define  CN_SOCKET_ERROR      CN_MULTIPLEX_SENSINGBIT_ERROR  //SOCKET ERROR
#define  CN_SOCKET_OR         CN_MULTIPLEX_SENSINGBIT_OR 

#define  CN_SOCKET_ACCEPT     CN_SOCKET_ERROR<<1             //COULD  ACCEPT
#define  CN_SOCKET_OPEN       CN_SOCKET_ERROR<<2             //COULD BE ACCEPT OR CONNECT
	bool_t (*multiioadd)(struct tagMultiplexSetsCB  *Sets,\
	        ptu32_t ObjectID, u32 SensingBit, tagSocket *sockfd);
	bool_t (*multiiodel)(struct tagMultiplexSetsCB  *Sets,\
	        ptu32_t ObjectID, tagSocket *sockfd);
}tagTlayerProto;

tagSocket *Socket_New(void);
bool_t     Socket_Free(tagSocket *sock);

#define INVALID_SOCKET -1
//�׽��ֽӿ�
int socket(int family, int type, int protocol);
int bind(int sockfd,struct sockaddr *myaddr, int addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, int *addrlen);
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); 
int send(int sockfd, const void *msg, int len, int flags);
int recv(int sockfd, void *buf,int len, unsigned int flags);
int sendto(int sockfd, const void *msg,int len, unsigned int flags,\
		  const struct sockaddr *addr, int addrlen);
int recvfrom(int sockfd,void *buf, int len, unsigned int flags,\
		    struct sockaddr *addr, int *addrlen);
bool_t shutdown(int sockfd, u32 how);
bool_t closesocket(int sockfd);
int setsockopt(int sockfd, int level, int optname,\
		        const void *optval, int optlen);
int getsockopt(int sockfd, int level, int optname, void *optval,int *optlen);
bool_t Socket_MultiplexAdd(struct tagMultiplexSetsCB *MultiplexSets,
                           int sock,u32 SensingBit);
bool_t Socket_MultiplexDel(struct tagMultiplexSetsCB *MultiplexSets,int sock);

#endif /* SOCKET_H_ */

