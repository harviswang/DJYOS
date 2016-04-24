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
// �ļ���     ��cpu_peri_rtc.c
// ģ������: RTCģ�飬��ϵͳ��ʱʱ������ΪRTCʱ��ͨ��RTC���жϣ�����ϵͳʱ�䣬ʹϵͳ
//           ��ʱʱ���ڶϵ��������Ȼ��������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 18/08.2014
// =============================================================================

#ifndef __CPU_PERI_RTC_H__
#define __CPU_PERI_UART_H__

#define RTC_ILR_RTCCIF_MASK         (1<<0)
#define RTC_ILR_RTCALF_MASK         (1<<1)

#define RTC_CCR_CLKEN_MASK          (1<<0)
#define RTC_CCR_CTCRST_MASK         (1<<1)
#define RTC_CCR_CCALEN_MASK         (1<<4)

#define RTC_CIIR_IMSEC_MASK         (1<<0)
#define RTC_CIIR_IMMIN_MASK         (1<<1)
#define RTC_CIIR_IMHOUR_MASK        (1<<2)
#define RTC_CIIR_IMDOM_MASK         (1<<3)
#define RTC_CIIR_IMDOW_MASK         (1<<4)
#define RTC_CIIR_IMDOY_MASK         (1<<5)
#define RTC_CIIR_IMMON_MASK         (1<<6)
#define RTC_CIIR_IMYEAR_MASK        (1<<7)

#define RTC_AMR_AMRSEC_MASK         (1<<0)
#define RTC_AMR_AMRMIN_MASK         (1<<1)
#define RTC_AMR_AMRHOUR_MASK        (1<<2)
#define RTC_AMR_AMRDOM_MASK         (1<<3)
#define RTC_AMR_AMRDOW_MASK         (1<<4)
#define RTC_AMR_AMRDOY_MASK         (1<<5)
#define RTC_AMR_AMRMON_MASK         (1<<6)
#define RTC_AMR_AMRYEAR_MASK        (1<<7)

#define RTC_AUX_OSCF_MASK           (1<<4)
#define RTC_AUX_PDOUT_MASK          (1<<6)
#define RTC_AUXEN_OSCFEN_MASK       (1<<4)

#define RTC_CTIME0_SEC(x)           ((x<<0) & 0x3F)
#define RTC_CTIME0_MIN(x)           ((x<<8) & 0x3F)
#define RTC_CTIME0_HOUR(x)          ((x<<16) & 0x1F)
#define RTC_CTIME0_DWEEK(x)         ((x<<24) & 0x07)
#define RTC_CTIME1_DMON(x)          ((x<<0) & 0x1F)
#define RTC_CTIME1_MON(x)           ((x<<8) & 0x0F)
#define RTC_CTIME1_YEAR(x)          ((x<<16) & 0xFFF)
#define RTC_CTIME2_DYEAR(x)         ((x<<0) & 0xFFF)

#define RTC_CALIBRATION_CALVAL(x)   ((x<<0) & 0xFFFF)
#define RTC_CALIBRATION_CALVAL_MASK (1<<17)

ptu32_t ModuleInstall_RTC(ptu32_t para);

#endif /* __CPU_PERI_UART_H__ */
