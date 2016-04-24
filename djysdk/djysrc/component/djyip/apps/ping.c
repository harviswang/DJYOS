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
// ģ������: PING����
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
#include "string.h"

#include "os.h"

#include "shell.h"
#include "icmp.h"

static u32 sgIpdst = 0;
static u16 sgEvttid =CN_EVTT_ID_INVALID;
// =============================================================================
// �������ܣ�Ping_Shell
//           Ping ����
// ���������
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ�ܣ���ʱ������·��ͨ����
// ˵��    :
// =============================================================================
bool_t Ping_Shell(char *param)
{
	bool_t result;
    char *word,*next_param;
    u32  indata;
    
    result = true;
    //replace the . to blank
    next_param = param;
    while(*next_param != '\0')
    {
    	if(*next_param == '.')
    	{
    		*next_param = ' ';
    	}
    	next_param++;
    }
    
    sgIpdst = 0;
    word = Sh_GetWord(param,&next_param);
    indata = atol(word);
    sgIpdst |= (indata&0xff)<<24;

    word = Sh_GetWord(next_param,&next_param);
    indata = atol(word);
    sgIpdst |= (indata&0xff)<<16;	
    
    word = Sh_GetWord(next_param,&next_param);
    indata = atol(word);
    sgIpdst |= (indata&0xff)<<8;	
	
    word = Sh_GetWord(next_param,&next_param);
    indata = atol(word);
    sgIpdst |= (indata&0xff)<<0;
    
    printk("Begin to ping:%08x\n\r",sgIpdst);
	Djy_EventPop(sgEvttid, NULL, 0, 0, 0, 0);
   
	return result;
}

void Ping(void)
{
	u32 i;
	u32 timeused;
	s64 timestart;
	s64 timeend;
    u8  datapad[32];    
    memset(datapad,0x55,32);
    while(1)
    {
        Djy_WaitEvttPop(sgEvttid, NULL,CN_TIMEOUT_FOREVER);
        for(i = 0; i <4; i++)
        {
        	timestart = DjyGetTime();
            if(Icmp_EchoRequest(sgIpdst,datapad,32))
            {
            	timeend = DjyGetTime();
            	timeused = (u32)(timeend - timestart);
            	printk("Ping 0x%08x  :Time = 0x%08x us\n\r",sgIpdst,timeused);
            }
            else
            {
            	printk("Ping 0x%08x  :TimeOut\n\r",sgIpdst);
            }
            Djy_EventDelay(1000*mS);
        }
    }
}


bool_t ping_test(void)
{

   if(sgEvttid == CN_EVTT_ID_INVALID)
   {
	    sgEvttid = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1, 
	    		(ptu32_t (*)(void))Ping,NULL, 0x1000, "Ping");
   }
	if (sgEvttid != CN_EVTT_ID_INVALID)
	{
		Djy_EventPop(sgEvttid, NULL, 0, 0, 0, 0);
	}
	return true;
}

