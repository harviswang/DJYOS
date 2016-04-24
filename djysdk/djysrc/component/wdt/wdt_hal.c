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
// ģ������: ���Ź�Ӳ�������
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 7:52:28 PM/Apr 14, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�����Ź�Ӳ������㣬ר��Ϊ���⿴�Ź�����
#include "stdint.h"
#include "stddef.h"

#include "os.h"

#include "wdt_hal.h"

#define CN_WDTHAL_HARDCYCLE_SCALE    8/10      //���ű���
///////��ֹ�����ļ����κμ����ֶ������޸ĸ��ļ������ȫ�־�̬����������Ը���///////
static struct tagWdtHalChipInfo    s_tWdtHalChipInfo; //�洢ע���WDT��Ϣ
static fnWdtBootFeedStart          fnBootFeedStart = NULL;
static fnWdtBootFeedEnd            fnBootFeedEnd = NULL;

// =============================================================================
// �������ܣ�WdtHal_BootStart
//          BOOT��ι������
// ���������bootfeedtime,��boot������ι��ʱ�䣬��λΪ΢��
// �����������
// ����ֵ  ��true�ɹ� falseʧ��(û�п��Ź�Ӳ����������������ʱ��ι��)
// ˵��    ������Ĳ����������û��Լ���ע��Ӳ�����Ź���ʱ���ṩ
// =============================================================================
bool_t WdtHal_BootStart(u32 bootfeedtime)
{
    bool_t result = true;
    if(NULL != fnBootFeedStart)
    {
    	result = fnBootFeedStart(bootfeedtime);
    }
    
    return result;
}

// =============================================================================
// �������ܣ�WdtHal_BootEnd
//          BOOT��ι������
// ���������bootfeedtimes
// �����������
// ����ֵ  ��true �ɹ�  fasleʧ��
// ע��    ��
// =============================================================================
bool_t WdtHal_BootEnd(void)
{
	bool_t result = true;
	if(NULL != fnBootFeedEnd)
	{
		result = fnBootFeedEnd();
	}
	return result;
}

// =============================================================================
// �������ܣ�WdtHal_GetChipPara
//          ��ȡӲ��оƬ����
// ���������
// ���������hardpara,�洢��ȡ�Ŀ��Ź�оƬ��Ϣ
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ����ؼ�鷵��ֵ
// =============================================================================
bool_t WdtHal_GetChipPara(struct tagWdtHalChipInfo *hardpara)
{
    if(NULL != s_tWdtHalChipInfo.wdtchip_feed)
    {
        *hardpara = s_tWdtHalChipInfo;
        hardpara->wdtchip_name = s_tWdtHalChipInfo.wdtchip_name;
        hardpara->wdtchip_cycle = s_tWdtHalChipInfo.wdtchip_cycle;
        hardpara->wdtchip_feed = s_tWdtHalChipInfo.wdtchip_feed;
        return true;
    }
    else
    {
        return false;
    }
}

// =============================================================================
// �������ܣ�WdtHal_RegisterWdtChip
//          ע��Ӳ�����Ź�оƬ
// ���������chipname,оƬ����
//           yipcycle,оƬ�����ڣ�����ʵʵ�Ľ�����OK���ڲ���������������
//           ��������CYCLE��WDT����ֵΪ���úú��CYCLE
//           wdtchip_feed,Ӳ�����Ź���ι������
//           bootfeedstart,�����������п�ʼι��
//           bootfeedend,�����������н���ι��
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ��ʧ��һ���ǲ�������ȷ��name�洢�ĵط�һ����const�ͣ�ǧ����Ǿֲ�����
// =============================================================================
bool_t WdtHal_RegisterWdtChip(char *chipname, u32 yipcycle,\
		                      bool_t (*wdtchip_feed)(void),\
		                      fnWdtBootFeedStart bootfeedstart,\
		                      fnWdtBootFeedEnd   bootfeedend)
{
    bool_t  result;

    if(NULL == wdtchip_feed)
    {
        result = false;
    }
    else
    {
        s_tWdtHalChipInfo.wdtchip_name = chipname;
        s_tWdtHalChipInfo.wdtchip_cycle = yipcycle*CN_WDTHAL_HARDCYCLE_SCALE;
        s_tWdtHalChipInfo.wdtchip_feed  = wdtchip_feed;
        fnBootFeedStart = bootfeedstart;
        fnBootFeedEnd = bootfeedend;
        
        result = true;
    }

    return result;
}

