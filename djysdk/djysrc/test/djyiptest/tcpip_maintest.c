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
// ģ������: Э��ջ����
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 9:23:45 AM/Jul 7, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

#include "os.h"
#include "shell.h"

///////////////////////////////////////////////////////////////////////////////
////���Դ�������ݵ�cache lock����
//u32  spider_bss_test __attribute__((section(".Cached_bsssec")));
//u32  spider_data_test __attribute__((section(".Cached_datasec")));
//void code_test() __attribute__((section(".Cached_textsec")));


extern bool_t Ping_Shell(char *param);
extern bool_t TcpServer_SndTask(char *param);
extern bool_t UdpServer_SndTask(char *param);
extern ptu32_t Telnet_Init(char *param);
//------------------------------------------------------------------------------
//monitor���Ա�
//------------------------------------------------------------------------------
struct tagShellCmdTab  shellstackdebug[] =
{
    {
        "ping",
        Ping_Shell,
        "PingӦ��",
        NULL
    },
//    {
//        "netio",
//        netio_main,
//        "netio����",
//        NULL
//    },
    {
        "tcpserversnd",
        TcpServer_SndTask,
        "tcpserver���Ͳ���",
        NULL
    },
    {
        "udpserversnd",
        UdpServer_SndTask,
        "udpserver���Ͳ���",
        NULL
    },
    {
        "telnet",
        Telnet_Init,
        "telnet������",
        NULL
    }
};
#define cn_monitor_test_num  ((sizeof(shellstackdebug))/(sizeof(struct tagShellCmdTab)))
static struct tagShellCmdRsc tg_monitorshell_cmd_rsc[cn_monitor_test_num];
void djyip_main(void)
{
	printk("DJYOS NETSTACK TEST\n\r");

	if(Sh_InstallCmd(shellstackdebug,tg_monitorshell_cmd_rsc,cn_monitor_test_num))
	{
		printk("ADD TEST SHELL SUCCESS!\n\r");
	}
	else
	{
		printk("ADD TEST SHELL FAILED!\n\r");
	}	
	extern bool_t ping_test(void);
	ping_test();
//	TEST FOR THE TCP SEND
//	TcpServer_SndTask(NULL);

}







