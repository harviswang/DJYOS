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
// ģ������: Tcp.h:
// ģ��汾: V1.00
// ������Ա: Zqf
// ����ʱ��: 11:26:50 AM/Sep 10, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================


#ifndef TCP_H_
#define TCP_H_

#include "stdint.h"
#include "stddef.h"

#include "os.h"

#include "socket.h"

#pragma pack(1)

typedef struct _tagTcpHdr
{
	u16 portsrc;     //source port
	u16 portdst;     //dstination port
	u32 seqno;       //sequence number
	u32 ackno;       //acknowledge number
#if(CN_CFG_BYTE_ORDER == CN_CFG_BIG_ENDIAN)
	u8  hdrlen:4;    //header len (word 4bytes)
	u8  reserv1:4;   //��4λ
#else
	u8  reserv1:4;   //��4λ
	u8  hdrlen:4;    //header len (word 4bytes)
#endif
	u8  flags;       //some flags like sync and ack
#define CN_TCP_FLAG_FIN  (1<<0)
#define CN_TCP_FLAG_SYN  (1<<1)
#define CN_TCP_FLAG_RST  (1<<2)
#define CN_TCP_FLAG_PSH  (1<<3)
#define CN_TCP_FLAG_ACK  (1<<4)
#define CN_TCP_FLAG_URG  (1<<5)
#define CN_TCP_FLAG_MSK  0x3f
	u16 window;      //the remote window size
	u16 chksum;      //package checksum
	u16 urgencyp;    //urgency pointer
	u8  opt[0];      //ѡ��
}tagTcpHdr;
// ����TCPУ���α����
typedef struct _tagTcpPseudoHdr 
{
	u32 ipsrc;
	u8	ipdst;
	u8	zero;
	u8	protocol;
	u16	len;	       // �����ĳ���
}tagTcpPseudoHdr;

typedef struct _tagWindowScale
{
	u8 resvered;
	u8 kind;
	u8 len;
	u8 shiftbits;
}tagWindowScale;

typedef struct _tagMss
{
	u8  kind;
	u8  len;
	u16 mss;
}tagMss;

typedef struct _tagSynOption
{
	tagMss mss;
	tagWindowScale winscale;
}tagSynOption;
#pragma pack()
//ͨ�Ŷ˵�״̬
enum _EN_TCPSTATE
{
	EN_TCPSTATE_CLOSED = 0,   //�Ѿ����ر�
	EN_TCPSTATE_CREATED ,     //�ձ�������״̬��һ�����ֻ�����������ĲŻ��ڴ�״̬
	EN_TCPSTATE_SSYNRCV,      //���������յ���FIN
	EN_TCPSTATE_CSYNSNT,      //�ͻ����Ѿ�������FIN�ź�
	EN_TCPSTATE_ESTABLISHED,  //�ȶ�����״̬
	EN_TCPSTATE_FIN_WAITE1,   //�����رշ�����FIN
	EN_TCPSTATE_FIN_WAITE2,   //�����رշ�����FIN�����յ���ACK
	EN_TCPSTATE_CLOSING,      //�����رշ�����FIN(δ�յ�ACK)���յ������FIN
	EN_TCPSTATE_TIME_WAIT,    //�����رշ���FIN���ҽ��յ���FIN��2ML��ʱ
	EN_TCPSTATE_CLOSE_WAIT,   //�����ر��յ��˶����FIN
	EN_TCPSTATE_LAST_ACK,     //�����رշ�����FIN
	EN_TCPSTATE_2FREE,        //���ͷ�״̬
	EN_TCPSTATE_FREE,         //�ͷ�״̬
};

enum _EN_TCPSEND_REASON
{
	EN_TCPSEND_SYN2SEND         = 0,    //��Ҫ����SYN,һ�㷢�����������ӵ�ʱ��
	EN_TCPSEND_DATA2SEND           ,    //�����ݴ���
	EN_TCPSEND_DATAPROBE           ,    //̽�����˿ڣ�һ���Ϊ��ǰ���ʹ���Ϊ0����������Ҫ����
	                                    //���ҽ�����һ�����ݣ������봰�ڹ�����
	EN_TCPSEND_FIN2SEND            ,    //Ҫ����FIN
};

typedef struct _tagTcpAlgorithm
{
    //TCPЭ�����
    u8 oobinline   :1;     //1,�������ݲ��ڶ��0 ���������ڶ�������
    u8 trigerlevel :1;     //1,�շ��д���ˮƽ��0�շ��޴���ˮƽ
    u8 nagle       :1;     //1,����nagle�㷨
    u8 linger      :1;     //��ʱ�ر��Ƿ��
}tagTcpAlgorithm;

//tcp communication channel controller
typedef struct
{
    //tcp channel control
	//fin,0 means no fin to snd,1 means fin to snd
//    u8 fin :1;
//    //when accept. the sestablished and open =0 will bingo
//    u8 open :1;
//    //when closed, this bit will be set
//    u8 close:1;
//    //the following control the app read/write and kernel read/write
//    //they all means more new data,not include the retransmit
//    u8 asnd:1;    //1 means the app could write while 0 not
//    u8 arcv:1;    //1 means the app could read while 0 not
//    u8 ksnd:1;    //1 means the stack kernel could snd more new data while 0 not
//    u8 krcv:1;    //1 means the stack kernel could rcv more new data while 0 not
    u8 fin;
    //when accept. the sestablished and open =0 will bingo
    u8 open;
    //when closed, this bit will be set
    u8 close;
    //the following control the app read/write and kernel read/write
    //they all means more new data,not include the retransmit
    u8 asnd;    //1 means the app could write while 0 not
    u8 arcv;    //1 means the app could read while 0 not
    u8 ksnd;    //1 means the stack kernel could snd more new data while 0 not
    u8 krcv;    //1 means the stack kernel could rcv more new data while 0 not
}tagTcpChannelstate;
//rcv fin makes krcv 0
//snt fin makes ksnd 0
//shutdown_rd makes krcv and arcv 0
//shutdown_wr makes asnd 0
//close makes the krcv 0 arcv 0 asnd 0

typedef struct _tagTcpIpOption
{
	u8 ttl;
	u8 tos;
}tagTcpIpOption;

#define CN_TCP_LISTEN_DEFAULT   5
//TCP�ķֶ����ݣ�UNACK �����������ݵȣ�
#define CN_TCP_RSNDTIMES_LIMIT  8

//the node used to hold the snd but not acked data,waiting the remote to ack
typedef struct _tagTcpSndUnackNode
{
	struct _tagTcpSndUnackNode  *nxt; //used for the list
	u32 secno;           //the node number for the debug
	u32 masks;           //set the maks for the debug
	u32 seqno;           //the pkg seqno
	u16 datalen;         //the len that the sec hold
	u8  flags;           //the tcp flags used to snd
	u8  rsndtimes;       //which means how many times the section has been rsnd
	s64 sndtime;         //which means when the pkg is snt
	s64 timeout;         //the time when the pkg timeout,used for retransmiton
	tagNetPkg  *lsthead; //head pkg that not acked yet
//	tagNetPkg  *lsttail; //tail pkg that not acked yet
}tagTcpSndUnackNode;
//the node used to hold the not sequence data, waiting the remote resnd to queue
typedef struct _tagTcpReCombNode
{
	struct _tagTcpReCombNode  *nxt;            //used for the list
	u32                       seqno;           //the pkg seqno
	u32                       datalen;         //the total len of pkg
	tagNetPkg                 *lsthead;        //pkg head
	tagNetPkg                 *lsttail;        //pkg tail
}tagTcpReCombNode;
//ÿ��Զ��SOCKET����ֻ��һ��TCB
typedef struct _tagTcpTCB
{
	tagSocket                   *owner;            //��TCB��ӵ����
	tagSocketNetAddr            raddr;             //Զ��ͨ�ŵ�ַ
    u8                          state;             //TCPͨ�ŵ�״̬��Զ��socket��״̬��
    //�㷨���ƿ飬acknodelay��
    tagTcpAlgorithm  	        algorithm;
    tagTcpChannelstate     	    channelstate;      //tcb�Ĵ򿪹ر�״̬
//tcp data buf,snd buf (include unacked buf) and rcv buf (include recombination buf)
#define CN_TCP_BUF_LEN_lIMIT           0x2000      //8KB
	tagSocketBuf     	        sbuf;              //snd buffer
    tagSocketBuf     	        rbuf;              //rcv buffer
    tagTcpSndUnackNode          *unacksechead;     //unacked queue head
    tagTcpSndUnackNode          *unacksectail;     //unacked queue tail
    tagTcpReCombNode            *recomblst;        //queue for recombination
//tcp basic window
#define CN_TCP_MSS_DEFAULT     1400
	u16	                        lmss;         //����mss��Ĭ��536
	u16	                        rmss;         //�Է�mss������ʱ����֪
	u16	                        mss;          //����MSS��Է�MSS�еĽ�Сֵ���������ӵ�ʱ��ֵ
	u32	                        snduna;       // ��ȷ�ϵĵ�һ���ֽ���ţ�������յ���ȷ�Ϻ�
	u32	                        sndnxt;       // ��һ��Ҫ���͵���ţ�����ڵ��ô�����ͺ����ɹ������
	u32	                        rcvnxt;       // �ڴ����յ��ֽ����
#define  CN_TCP_RCVWIN_DEFAULT  0xF000  //64KB
	u16	                        rcvwnd;       // �������ջ�������С
	u16	                        sndwnd;       // �Է��Ľ��մ��ڴ�С���������յ����ĵĴ��ڳ�Ա
    u8                          sndwndscale;  // ��������
	bool_t                      rcvwndcongest;// ���մ�������
	//tcp congestion window member
    /* RTT(round trip time)��ر��� */
    /*
    RTT���㷽��:
    Err = M-A    		//A��RTTƽ��ֵ��M��ʵ�ʲ���ֵ��Err�����
    A��A + gErr    		//��������ƽ��ֵ
    D��D + h( | Err |-D)    //D�Ǿ�ֵƫ��������¾�ֵƫ��
    RTO = A + 4D    	//����õ�RTO (�ش���ʱʱ��)  (��ʼ��ʱRTO = A + 2D  )
    g=0.125 ; h=0.25
    */
    s32 sa, sd; 	/* ���ڼ���RTO��sa = 8*A��sv = 4*D */
    s32 rto;    	/* RTO */
#define CN_TCP_RTOMAX            (120*1000*mS)
#define CN_TCP_RTOMIN            (200*mS)
#define CN_TCP_SA_INIT           0
#define CN_TCP_SD_INIT           (3*100*mS)       //Units��Micro Seconds
    /* ӵ��������ر��� */
    u16 cwnd;        /* ӵ�����ⴰ��, Ĭ��ΪdMSS */
    u16 ssthresh;    /* ���������ޣ�Ĭ��Ϊ65535 */
    /* �����ش��Ϳ��ٻָ� */
    u8 dupacks;    	/* �ظ�ACK���� */
#define CN_TCP_FASTRETRANS   3
#define CN_TCP_CWND_LIMIT    60000
//tcp tick timer
#define CN_TCP_TIMER_ACK                   (1<<0)   //ack timer control bit
#define CN_TCP_TICK_ACK                    (2)
#define CN_TCP_TIMER_2ML                   (1<<1)   //2ml timer control bit
#define CN_TCP_TIMER_LINGER                (1<<2)   //linger timer control bit
#define CN_TCP_TIMER_KEEPALIVE             (1<<3)   //keepalive timer control bit
#define CN_TCP_TICK_KEEPALIVE              (10000)
#define CN_TCP_TIMER_PERSIST               (1<<4)   //persis timer control bit
#define CN_TCP_TICK_PERSIST                (10)
#define CN_TCP_TIMER_RETRANSMIT            (1<<5)   //retransmit timer control bit
#define CN_TCP_TIMER_CORK                  (1<<6)   //cork timer control bit
    u8                          timerflag;
    u16                         acktimer;          //(unit:tcp tick,use for ack echo)
    u16                         mltimer;           //(unit:tcp tick,used for 2ml wait)
    u16                         lingertimer;       //(unit:tcp tick,used for linger close)
    u16                         keeptimer;         //(unit:tcp tick,used for keep alive)
    u16                         persistimer;       //(unit:tcp tick,used for probe window)
    u16                         corktimer;         //(unit:tcp tick,used for cork timeout)
    //�����ʱ���IPѡ��
    tagTcpIpOption              ipoption;          //use for ip optionnal
}tagTcpTcb;
#define  CN_TCP_TICK_TIME     (100*mS)        //Units:Micro Seconds


//tcp server node, used to hange the communication client
typedef struct _tagTcpTnode
{
	u32                        connum;              //�Ѿ�ͨ�ŵ�Զ�˸����������������ӵģ�
	u32                        connum2accept;       //���Ա�accept�ĸ���
	u32                        connumlimit;         //Զ�˸�������
	tagSocket                  *clientlst;          //used to hange the client
}tagTcpTnode;
/////////////////////FUNCTION EXPORT DECLARATION//////////////////////////////

#endif /* TCP_H_ */
