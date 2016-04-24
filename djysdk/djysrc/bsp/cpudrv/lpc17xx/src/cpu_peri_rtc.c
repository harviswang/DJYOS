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
#include "cpu_peri.h"
#include "time.h"
// =============================================================================
// ���ܣ�RTC���ú������������RTCģ�鸴λ��Ȼ��ʹ����������������ؼĴ���
// ��������
// ���أ���
// =============================================================================
static void RTC_Configuration(void)
{
    // ʹ�����ж�
    LPC_RTC->ILR |= RTC_ILR_RTCCIF_MASK;
    LPC_RTC->CIIR |= RTC_CIIR_IMSEC_MASK;
    // ʱ��ʹ��
    LPC_RTC->CCR |= RTC_CCR_CLKEN_MASK;
}

// =============================================================================
// ���ܣ�RTCʵʱʱ�Ӹ��£���RTCģ���е�ʱ�����Ϊ����ʱ�䣬������ģ�����
// ������DateTime,ϵͳʱ�䣬�軻�����
// ���أ�1
// =============================================================================
bool_t RTC_TimeUpdate(s64 time)
{
    bool_t result = false;
    struct tm *DateTime;

    DateTime = Tm_GmTime(&time);
    if(DateTime != NULL)
    {
        LPC_RTC->SEC = DateTime->tm_sec;
        LPC_RTC->MIN = DateTime->tm_min;
        LPC_RTC->HOUR = DateTime->tm_hour;
        LPC_RTC->DOM = DateTime->tm_mday;
        LPC_RTC->MONTH  = DateTime->tm_mon;
        LPC_RTC->YEAR = DateTime->tm_year;
        result = true;
    }

    return result;
}

// =============================================================================
// ���ܣ�RTCʵʱʱ�Ӷ�ȡ����ȡRTCʱ�䣬������ϵͳʱ��
// ������DateTime,ϵͳʱ�䣬�軻�����
// ���أ�1
// =============================================================================
bool_t RTC_TimeGet(s64 *time)
{
    struct tm DateTime;

    DateTime.tm_sec = LPC_RTC->SEC;
    DateTime.tm_min = LPC_RTC->MIN;
    DateTime.tm_hour = LPC_RTC->HOUR;
    DateTime.tm_mday = LPC_RTC->DOM;
    DateTime.tm_mon  = LPC_RTC->MONTH;
    DateTime.tm_year = LPC_RTC->YEAR;

    *time = 1000000 * Tm_MkTime(&DateTime);
    return true;
}

// =============================================================================
// ���ܣ���ʼ��RTCʵʱʱ��ģ�顣
// ������ģ���ʼ������û�в���
// ���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    // ʹ��RTCʱ�ӣ����ṩϵͳʱ��
    LPC_SC->PCONP |= (1<<9);

    //  RTC����ʧЧ����־
    if(LPC_RTC->RTC_AUX & RTC_AUX_OSCF_MASK)
    {
        LPC_RTC->RTC_AUX |= RTC_AUX_OSCF_MASK;//�����־λ
    }

    // �ж��Ƿ�ʱ����Ч�����ж��Ƿ���Ҫ��ʼ��ʱ��
    // һ���ǵ�ص����������λ����Ҫ���³�ʼ��
    if(!(LPC_RTC->CCR & RTC_CCR_CLKEN_MASK))
    {
        RTC_Configuration();
    }

    Rtc_RegisterDev(RTC_TimeGet,RTC_TimeUpdate);

    return true;
}

