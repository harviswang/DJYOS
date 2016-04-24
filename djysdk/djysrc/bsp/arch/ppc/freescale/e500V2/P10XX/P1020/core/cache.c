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
// ģ������: ���ļ���Ҫʵ��CACHE��һЩ�������������ƶ���������CACHE��
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:zhangqf_cyg
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
// ˵��������cache��һЩ�������󲿷ֶ���CACHE��ʹ����ر�
//       һ���Ƚϸ߼��Ĺ��ܾ��ǽ���������CACHE�С�
// =============================================================================
// Copyright (C) 2012-2020 	 All Rights Reserved
// FileDescription:This file used to initial the cache, include the l1cache and l2
// FileVersion    :
// Author         :
// Create Time    :2014/12/12
// =============================================================================
// Instructions:
#include "stdint.h"
#include "SysSpace.h"
#include "Cache.h"

#define CN_CACHEL2_BASEADDR          (CN_CCSR_NEWADDR_V + 0x20000)
#define CN_CACHEL2_L2CTL             (CN_CACHEL2_BASEADDR)
#define CN_CACHEL2_L2CTL_L2E         (1<<31)
#define CN_CACHEL2_L2CTL_L2I         (1<<30)

/*
@-------------------------------------------------------------------------------
@function:initial the l2 cache
@para:void
@return:void
@-------------------------------------------------------------------------------
*/
void Cache_InitL2(void)
{
	u32 value;
	u32 *addr;
	
	addr = (u32 *)CN_CACHEL2_L2CTL;
	value = *(u32 *)(addr);
	
	value = CN_CACHEL2_L2CTL_L2E;
	*(u32 *)(addr)= value;
	
	value |= CN_CACHEL2_L2CTL_L2I;
	while(value & CN_CACHEL2_L2CTL_L2I)
	{
		*(u32 *)(addr)= value;
		value = *(u32 *)(addr);
	}
	return;
}


