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
// ģ������: NetStackInit.c
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:07:01 PM/Jul 11, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
// =============================================================================
// �������ܣ�ModuleInstall_DjyIp
//          TcpIPЭ��ջ��ʼ��
// ���������para,������
// �����������
// ����ֵ  ��Ŀǰ������
// ˵��    :1,�ȳ�ʼ���������ʵ�ģ��
//       2,�ٳ�ʼ��Э����ģ��
//       3,�ٳ�ʼ������Э��
//       4,�������Ĳ���ɵ�ʱ��Ӧ�ÿ���ע��������ע��ROUT�Լ��׽��ֱ����
// =============================================================================
ptu32_t ModuleInstall_DjyIp(ptu32_t para)
{
	//���ݰ�����ģ��ĳ�ʼ����ֻ�г�ʼ����Ϻ�Ŵ���Э��ջ�����ݰ��Ĺ���
	extern bool_t Pkg_Init(void);
	if(false == Pkg_Init())
	{
		printk("DJY_NETSTACK_INIT: Pkg Init Failed\n\r");
		goto TCPIP_INITERR;
	}
	else
	{
		printk("DJY_NETSTACK_INIT: Pkg Init  Success\n\r");
	}
	//ROUT��ĳ�ʼ������ʼ����Ϻ�ſ��������·
	extern bool_t  Rout_Init(void);
	if(false == Rout_Init())
	{
		printk("DJY_NETSTACK_INIT: Rout Init Failed\n\r");
		goto TCPIP_INITERR;

		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
	}
	else
	{
		printk("DJY_NETSTACK_INIT: Rout Init  Success\n\r");
	}
	//�豸����ӿڣ���ʼ����ſ���ע������
	extern bool_t NetDev_Init(void);
	if(false == NetDev_Init())
	{
		printk("DJY_NETSTACK_INIT: NetDev Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: NetDev Init  Success\n\r");
	}
	//��·���ʼ�������֮�����ע��ARP RARP IP������Э��Ĵ�����
	extern bool_t Link_Init(void);
	if(false == Link_Init())
	{
		printk("DJY_NETSTACK_INIT: Link Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Link Init  Success\n\r");
	}
	//ARPЭ���ʼ��
	extern bool_t Arp_Init(void);
	if(false == Arp_Init())
	{
		printk("DJY_NETSTACK_INIT: Arp Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Arp Init  Success\n\r");
	}
	
	//IPЭ���ʼ��
	extern bool_t Ip_Init(ptu32_t para);
	if(false == Ip_Init((ptu32_t)0))
	{
		printk("DJY_NETSTACK_INIT: Ip Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Ip Init  Success\n\r");
	}
	//������ʼ��
	extern bool_t TPL_Init(ptu32_t para);
	if(false == TPL_Init((ptu32_t)0))
	{
		printk("DJY_NETSTACK_INIT: TPL Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: TPL Init  Success\n\r");
	}

	//socket��ʼ��
	extern bool_t Socket_Init(ptu32_t para);
	if(false == Socket_Init((ptu32_t)0))
	{
		printk("DJY_NETSTACK_INIT: Socket Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Socket Init  Success\n\r");
	}
	//TCPЭ���ʼ��
	extern bool_t Tcp_Init(ptu32_t para);

	if(false == Tcp_Init((ptu32_t)0))
	{
		printk("DJY_NETSTACK_INIT: Tcp Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Tcp Init  Success\n\r");
	}
	//udp proto init
	bool_t Udp_Init(ptu32_t para);
	if(false == Udp_Init((ptu32_t)0))
	{
		printk("DJY_NETSTACK_INIT: Udp Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Udp Init  Success\n\r");
	}
	//ICMPЭ���ʼ��
	extern bool_t Icmp_Init(ptu32_t para);
	if(false == Icmp_Init((ptu32_t)0))
	{
		printk("DJY_NETSTACK_INIT: Icmp Init Failed\n\r");
		//��ʼ��ʧ�ܣ�Ӧ�ò�ȡ���ʩ
		goto TCPIP_INITERR;

	}
	else
	{
		printk("DJY_NETSTACK_INIT: Icmp Init  Success\n\r");
	}
	printk("*********DJY TCP/IP INIT SUCCESS************************\n\r");
	return 0;

TCPIP_INITERR:
	printk("*********DJY TCP/IP INIT  FAILED************************\n\r");
	return -1;
}

