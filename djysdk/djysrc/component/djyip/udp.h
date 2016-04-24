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
// ģ������: Udp.h
// ģ��汾: V1.00
// ������Ա: ZQF
// ����ʱ��: ����11:01:20/2014-12-2
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef UDP_H_
#define UDP_H_

#include "stdint.h"
#include "Socket.h"

typedef struct
{
	u16 portsrc;
	u16 portdst;
	u16 msglen;
	u16 chksum;
}tagUdpHdr;

//UDP communication channel controller
typedef struct
{
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
    u8 open;
    //when closed, this bit will be set
    u8 close;
    //the following control the app read/write and kernel read/write
    //they all means more new data,not include the retransmit
    u8 asnd;    //1 means the app could write while 0 not
    u8 arcv;    //1 means the app could read while 0 not
    u8 ksnd;    //1 means the stack kernel could snd more new data while 0 not
    u8 krcv;    //1 means the stack kernel could rcv more new data while 0 not
}tagUdpChannelstate;
//ÿ��Զ��SOCKET����ֻ��һ��TCB
typedef struct
{
	tagSocket                   *owner;            //��TCB��ӵ����
	tagSocketNetAddr            raddr;             //Զ��ͨ�ŵ�ַ
	u8                          cbstate;
#define CN_UDP_TERMINAL_LIFE    0x10
	u8                          lifetick;          //exist time
    //�㷨���ƿ飬acknodelay��
	tagUdpChannelstate     	    channelstate;      //tcb�Ĵ򿪹ر�״̬
//tcp data buf,snd buf (include unacked buf) and rcv buf (include recombination buf)
#define CN_UDP_BUF_LEN_lIMIT    0x2000      //8KB
	tagSocketBuf     	        sbuf;              //snd buffer
    tagSocketBuf     	        rbuf;              //rcv buffer
}tagUCB;
//tcp server node, used to hange the communication client
typedef struct
{
	u32                        connum;              //�Ѿ�ͨ�ŵ�Զ�˸����������������ӵģ�
	u32                        connumlimit;         //Զ�˸�������
	u32                        connum2accept;
	tagSocket                  *clientlst;          //used to hange the client
}tagUdpTnode;
#endif /* UDP_H_ */
