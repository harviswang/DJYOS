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
// ����ʱ��: 25/09.2014
// =============================================================================

#include "stdint.h"
#include "os.h"
#include "cpu_peri_rtc.h"

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))

// =============================================================================
// ���ܣ�RTC�жϣ�ʹ��RTCģ���TICK������������Ϊÿһ���ж�һ�Σ������ж��ж�ȡRTC
//       ʱ�ӣ�������ϵͳʱ��
// �������ж��ߺ�
// ���أ�0
// =============================================================================
u32 RTC_ISR(ufast_t rtc_int_line)
{
    struct tm dtm;

    rRTCCON = 1 ;       //RTC read and write enable
    dtm.tm_year = BcdToHex(rBCDYEAR) + 2000;
    dtm.tm_mon  = BcdToHex(rBCDMON);
    dtm.tm_mday = BcdToHex(rBCDDATE);
    dtm.tm_hour = BcdToHex(rBCDHOUR);
    dtm.tm_min  = BcdToHex(rBCDMIN);
    dtm.tm_sec  = BcdToHex(rBCDSEC);
    rRTCCON &= ~1 ;     //RTC read and write disable

    Tm_SetDateTime(&dtm); // ��OS��Clockģ�鴫���µ�ʱ��ֵ
    return 0;
}

// =============================================================================
// ���ܣ�RTCʱ�Ӹ��£���ϵͳʱ����µ�RTCģ�飬����ʱ��ģ�����洢�����ֵΪ99�����
//       ��RTCģ����ƴ�2000Ϊ��ʼֵ����2014����RTCģ���rBCDYEAR�洢����14
// ������dtm������ʱ��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t RTC_TimeUpdate(s64 time)
{
    bool_t result = false;
    struct tm dtm;
    s64 time_s;

    time_s = time/1000000;
    Tm_LocalTime_r(&time_s,&dtm);

    if((dtm.tm_year > 2000) && (dtm.tm_year < 2099))
    {
        rRTCCON = 1 ;       //RTC read and write enable
        rBCDYEAR = HexToBcd(dtm.tm_year - 2000);
        rBCDMON  = HexToBcd(dtm.tm_mon);
        rBCDDATE = HexToBcd(dtm.tm_mday);
        rBCDHOUR = HexToBcd(dtm.tm_hour);
        rBCDMIN  = HexToBcd(dtm.tm_min);
        rBCDSEC  = HexToBcd(dtm.tm_sec);
        rRTCCON &= ~1 ;     //RTC read and write disable
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ���ȡRTCʱ�ӣ���ȡRTCģ���ʱ�䣬����ʱ��ģ�����洢�����ֵΪ99������轫��
//       ������ݼӻ�����2000���磬��RTCģ���rBCDYEARΪ14����Ϊ2014��
// ������dtm������ʱ��
// ���أ�true
// =============================================================================
bool_t RTC_TimeGet(s64 *time)
{
    struct tm dtm;

    rRTCCON = 1 ;       //RTC read and write enable
    dtm.tm_year = BcdToHex(rBCDYEAR) + 2000;
    dtm.tm_mon  = BcdToHex(rBCDMON);
    dtm.tm_mday = BcdToHex(rBCDDATE);
    dtm.tm_hour = BcdToHex(rBCDHOUR);
    dtm.tm_min  = BcdToHex(rBCDMIN);
    dtm.tm_sec  = BcdToHex(rBCDSEC);
    rRTCCON &= ~1 ;     //RTC read and write disable

    *time = 1000000 * Tm_MkTime(&dtm);
    return true;
}

// =============================================================================
// ���ܣ���ʼ��RTCģ��.��ʱ�Ӽ�������λ����������Ĭ��ʱ�䣬�����RTCʱ�䣬������ϵͳ
//       ʱ��
// ������ģ���ʼ������û�в���
// ���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
ptu32_t RTC_ModuleInit(ptu32_t para)
{
    struct tm DateTime;
    bool_t RtcIsReset = false;

    //�ж�ʱ�Ӽ������Ƿ񱻸�λ�����Ƿ�ϵ��
    rRTCCON = 1 ;
    RtcIsReset = rRTCCON & (1<<3);
    rRTCCON &= ~1 ;

    if(RtcIsReset == true)
    {
        DateTime.tm_sec = 0;
        DateTime.tm_min = 0;
        DateTime.tm_hour = 0;
        DateTime.tm_mday = 1;
        DateTime.tm_mon  = 1;
        DateTime.tm_yday = 2000;
        RTC_TimeUpdate(&DateTime);
        Tm_SetDateTime(&DateTime);
    }
    Rtc_RegisterDev(RTC_TimeGet,RTC_TimeUpdate);
    return true;
}


