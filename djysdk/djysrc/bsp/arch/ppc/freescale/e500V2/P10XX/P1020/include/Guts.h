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
// ģ������:p1020_guts_ctl.h/p1020-core0
// ģ��汾: V1.0.0
// ������Ա: zhangqf_cyg
// ����ʱ��: 10:37:49 AM/Nov 7, 2013
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef __GUTS_H
#define __GUTS_H


#include "SysSpace.h"

//machine check summary register
#define cn_p1020_guts_mcpsumr  (CN_CCSR_NEWADDR_V + 0xe0090)
#define cn_p1020_guts_mcpsumr_mcp0in  (1<<0)
#define cn_p1020_guts_mcpsumr_sreset  (1<<1)
#define cn_p1020_guts_mcpsumr_wrs0    (1<<2)
#define cn_p1020_guts_mcpsumr_mcp1in  (1<<4)
#define cn_p1020_guts_mcpsumr_wrs1    (1<<5)
#define cn_p1020_guts_mcpsumr_ckstp1  (1<<6)
#define cn_p1020_guts_mcpsumr_ckstp0  (1<<7)

//reset request status and control register
#define cn_p1020_guts_rstrscr   (CN_CCSR_NEWADDR_V + 0xe0094)
#define cn_p1020_guts_rstrscr_nflash_rr  (1<<22)
#define cn_p1020_guts_rstrscr_bs_rr      (1<<21)
#define cn_p1020_guts_rstrscr_wdt0_rr    (1<<20)
#define cn_p1020_guts_rstrscr_sw_rr      (1<<19)
#define cn_p1020_guts_rstrscr_wdt1_rr    (1<<18)

//response to check stop
#define  cn_p1020_guts_ectrstcr   (CN_CCSR_NEWADDR_V + 0xe0098)

#define  cn_p1020_guts_autorstsr (CN_CCSR_NEWADDR_V + 0xe009c)
//guts reset control register
#define cn_p1020_guts_rstcr       (CN_CCSR_NEWADDR_V + 0xe00b0)
#define cn_p1020_guts_rstcr_hardreset_r  (1<<1)

//PLL registers
#define cn_guts_porpllsr_addr   (CN_CCSR_NEWADDR_V + 0x0e0000)

//guts


#endif /* __GUTS_H */
