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
// ģ������: ���Ź�Ӳ����������ݽṹ�Լ��ӿ�����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 7:53:48 PM/Apr 14, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef WDT_HAL_H_
#define WDT_HAL_H_
#include "stdint.h"

//���Ź�Ӳ���Ļ�����Ϣ
 struct tagWdtHalChipInfo
{
	char*  wdtchip_name;             //chip����             
	u32    wdtchip_cycle;            //chip cycle,Ӳ������
	bool_t (*wdtchip_feed)(void);    //chip feed	
};

//�������׶Σ�ʵʱ�ж϶�����ʹ�ã�����û���ȫ�����Լ�����ʹ��Ӳ����ʱ��ι��
//������Щ�����ι���ǿ����Լ��Զ����õģ����WDTHAL�����������붨ʱ��ι������ȫ��
//�û��Լ��������ú��ַ�ʽ��ֻ���û��ṩע��HOOK�Ľӿڡ�
typedef bool_t (*fnWdtBootFeedStart)(u32 bootfeedtime);  //�������׶�ι����ʼHOOK
typedef bool_t (*fnWdtBootFeedEnd)(void);                //�������׶�ι������HOOK
 
bool_t WdtHal_BootStart(u32 bootfeedtimes);
bool_t WdtHal_BootEnd(void);
bool_t WdtHal_GetChipPara(struct tagWdtHalChipInfo *hardpara);
bool_t WdtHal_RegisterWdtChip(char *chipname, u32 yipcycle,\
		                      bool_t (*wdtchip_feed)(void),\
		                      fnWdtBootFeedStart bootfeedstart,\
		                      fnWdtBootFeedEnd   bootfeedend);

#endif /* WDT_HAL_H_ */
