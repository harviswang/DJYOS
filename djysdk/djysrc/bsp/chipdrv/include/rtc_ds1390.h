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
//����ģ�飺RTCʱ��ģ��
//���ߣ�lst
//�汾��V0.1.0
//�ļ�������DJYOS��Ӳ��RTC���Ӽ�RTC�ײ�����
//����˵����
//�޶���ʷ��
//1. ����: 2013-10-28
//   ���ߣ�
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------

#ifndef _RTC_DS1390_H_
#define _RTC_DS1390_H_

#ifdef  CN_CFG_DJYOS_USED
#define rtc_tm tm

uint32_t rtc_time_get(struct rtc_tm *DateTime);
bool_t rtc_update_time(struct rtc_tm *DateTime);
ptu32_t ModuleInstall_RTC(ptu32_t para);
#endif

#ifdef  cfg_no_os_used
#include "stdint.h"

struct rtc_tm
{
    s32 tm_us;      // ΢��-ȡֵ����[0,999999]
    s32 tm_sec;     // ��Cȡֵ����Ϊ[0,59]
    s32 tm_min;     // �� - ȡֵ����Ϊ[0,59]
    s32 tm_hour;    // ʱ - ȡֵ����Ϊ[0,23]
    s32 tm_mday;    // һ�����е����� - ȡֵ����Ϊ[1,31]
    s32 tm_mon;     // �·ݣ���һ�¿�ʼ��0����һ�£� - ȡֵ����Ϊ[0,11]
    s32 tm_year;    // ��ݣ���ֵ��1900��ʼ
    s32 tm_wday;    // ���ڨCȡֵ����Ϊ[0,6]������0���������죬1��������һ��
                    // �Դ�����
    s32 tm_yday;    // ��ÿ���1��1�տ�ʼ�������Cȡֵ����Ϊ[0,365]������0����
                    //1��1�գ�1����1��2�գ��Դ�����
    s32 tm_isdst;   // ����ʱ��ʶ����ʵ������ʱ��ʱ��tm_isdstΪ������ʵ��
                    //����ʱ�Ľ���tm_isdstΪ0�����˽����ʱ��tm_isdst()Ϊ����
};

uint32_t rtc_time_get(struct rtc_tm *DateTime);
bool_t rtc_update_time(struct rtc_tm *DateTime);
u32 rtc_init(void);
#endif

#endif


