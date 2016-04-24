
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
//����ģ��:����ģ��
//����:  zqf
//�汾��V1.0.0
//�ļ�����:Ϊϵͳά������ʱ��;�ṩRTC�豸ע��ӿ�
//����˵��:
//�޶���ʷ:
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "int.h"
#include "systime.h"
#include "time.h"

s64 __DjyGetTime(void);

static s64               sgRtcTimeSet;             //�ֶ����û���ϵͳ��RTC�豸�ж�ȡ��RTCʱ��
static s64               sgRtcUpdateTime2SysTime;  //��ȡ��������RTCʱ��ʱϵͳ������ʱ��
static __rtcdev_gettime  fnRtcGetTime = NULL;      //��ȡRTC�豸RTCʱ��
static __rtcdev_settime  fnRtcSetTime = NULL;      //����RTC�豸RTCʱ��
// =============================================================================
// �������ܣ�Rtc_RegisterDev
//        ע��RTC�豸
// ���������gettimefunc����ȡRTC�豸ʱ�䣬��ȡ�ɹ�true,ʧ��false
//        settimefunc,����RTC�豸ʱ�䣬���óɹ�true,ʧ��false
//        ʱ����Ǿ���1970�꿪ʼ��΢����
// ���������
// ����ֵ  ��true ע��ɹ�  falseע��ʧ��
// ˵��������ͬʱ�ṩ��������ע��ɹ�����ʹ���ṩRTC���ù��ܣ�ҲӦ������Ϊ�պ���������false
// =============================================================================
bool_t Rtc_RegisterDev(__rtcdev_gettime gettimefunc,\
                       __rtcdev_settime settimefunc)
{
    bool_t result = false;

    if((NULL != gettimefunc) && (NULL != settimefunc))
    {
        fnRtcGetTime = gettimefunc;
        fnRtcSetTime = settimefunc;

        result = true;
    }
    return result;
}

#define CN_RTC_UNIT_SECOND        1000000
// =============================================================================
// �������ܣ�Rtc_Time
//        ��ȡ����ʱ�䣬��λ��
// ���������
// ���������rtctime,�洢��ȡ��RTCʱ�䣬����
// ����ֵ  ����ȡ��RTCʱ�䣬����
// ˵��������1970���ʱ��;�������Ķ�λ�ȡRTCʱ��ʱ�����ܻ�ȡ��ֵ��ͬ����Ϊ����
//      ���Ƶ���Ϊ��һ���ڵ�ֵ��࣬������ͬһ���޿ɺ�ǣ���Ϊ�������õ��룬
//      ������Ȼ���뼶
// =============================================================================
s64 __Rtc_Time(s64 *rtctime)
{
    s64 result;
    s64 systime;
    atom_low_t atom;

    systime = DjyGetTime();

    if((sgRtcUpdateTime2SysTime/CN_RTC_UNIT_SECOND) ==(systime/CN_RTC_UNIT_SECOND))
    {
        //which means that we get the RTC time at the same second
        //we could return the same time as last time
#if (64 > CN_CPU_BITS)
        atom = Int_LowAtomStart();
        result = sgRtcTimeSet;
        Int_LowAtomEnd(atom);
#else
        result = sgRtcTimeSet;
#endif
    }
    else
    {
        //we'd better to get the RTC time now
        if(NULL == fnRtcGetTime)
        {
            //no rtc dev yet,we could cal the rtc time use the sysrunning time
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
            Int_LowAtomEnd(atom);
#else
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
#endif
        }
        else
        {
            if(false == fnRtcGetTime(&result))
            {
                //failed to get the rtc dev time
#if (64 > CN_CPU_BITS)
                atom = Int_LowAtomStart();
                result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
                Int_LowAtomEnd(atom);
#else
                result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
#endif
            }
            else
            {
#if (64 > CN_CPU_BITS)
                atom = Int_LowAtomStart();
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
                Int_LowAtomEnd(atom);
#else
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
#endif
            }
        }
    }

    result = result/CN_RTC_UNIT_SECOND;
    if(NULL != rtctime)
    {
        *rtctime = result;
    }

    return result;
}
// =============================================================================
// �������ܣ�Rtc_TimeUs
//        ��ȡ����ʱ�䣬��λ΢��
// ���������
// ���������rtctime,�洢��ȡ��RTCʱ�䣬����
// ����ֵ  ����ȡ��RTCʱ�䣬����
// ˵��������1970���ʱ��;�����RTC�豸����ֱ�Ӷ�ȡRTC�豸��US�����û��RTCʱ�䣬���Լ���ϵͳ����ʱ�����
// =============================================================================
s64 __Rtc_TimeUs(s64 *rtctime)
{
    s64 result;
    s64 systime;
    atom_low_t atom;

    systime = DjyGetTime();
    //we'd better to get the RTC time now
    if(NULL == fnRtcGetTime)
    {
        //no rtc dev yet,we could cal the rtc time use the sysrunning time
#if (64 > CN_CPU_BITS)
        atom = Int_LowAtomStart();
        result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
        sgRtcUpdateTime2SysTime = systime;
        sgRtcTimeSet = result;
        Int_LowAtomEnd(atom);
#else
        result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
        sgRtcUpdateTime2SysTime = systime;
        sgRtcTimeSet = result;
#endif
    }
    else
    {
        if(false == fnRtcGetTime(&result))
        {
            //failed to get the rtc dev time
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
            Int_LowAtomEnd(atom);
#else
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
#endif
        }
        else
        {
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
            Int_LowAtomEnd(atom);
#else
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
#endif
        }
    }
    result = result;
    if(NULL != rtctime)
    {
        *rtctime = result;
    }

    return result;
}
// =============================================================================
// �������ܣ�Rtc_SetTime
//        ��������ʱ�䣬��λ΢��
// ���������rtctime,����RTCʱ�䣬΢����
// ���������
// ����ֵ  ����ȡ��RTCʱ�䣬����
// ˵��������1970���ʱ��;������RTCģ�������ʱ�䣻�����RTC�豸��ͬ���ǵ�����RTC�豸ʱ��
// =============================================================================
bool_t __Rtc_SetTime(s64 rtctime)
{
    bool_t result = true;
    atom_low_t atom;

    s64 systime;

    systime = DjyGetTime();

    if(NULL == fnRtcSetTime)
    {
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            sgRtcTimeSet = rtctime;
            sgRtcUpdateTime2SysTime = systime;
            Int_LowAtomEnd(atom);
#else
            sgRtcTimeSet = rtctime;
            sgRtcUpdateTime2SysTime = systime;
#endif
    }
    else
    {
        if(fnRtcSetTime(rtctime))
        {
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            sgRtcTimeSet = rtctime;
            sgRtcUpdateTime2SysTime = systime;
            Int_LowAtomEnd(atom);
#else
            sgRtcTimeSet = rtctime;
            sgRtcUpdateTime2SysTime = systime;
#endif
        }
        else
        {
            //rtc set failed, so don't update the rtc module time,otherwise will
            //make the rtc dev and rtc module time not sync
            result = false;
        }
    }
    return result;
}


