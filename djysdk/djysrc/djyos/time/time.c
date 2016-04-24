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
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: Ϊϵͳ�ṩʱ������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-5-1
//   ����:  ������
//   �°汾��: V1.1.0
//   �޸�˵��: ������޸�
// 1. ����: 2011-10-28
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "string.h"
#include "config-prj.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

extern s64 __Rtc_Time(s64 *rtctime);
extern s64 __Rtc_TimeUs(s64 *rtctime);
extern bool_t __Rtc_SetTime(s64 rtctime);

u32 g_u32DaysToMonth[13] = {0,31,59,90,120,151,181,212,243,273,304,334,365};
u32 g_u32MonthDays[]={31,28,31,30,31,30,31,31,30,31,30,31,};

//char g_cTmWdays[][8] = {"������", "����һ", "���ڶ�", "������", "������", "������", "������"};
char g_cTmWdays[][8] = {"������", "����һ", "���ڶ�", "������", "������", "������", "������"};


//----�ѷֽ�ʱ��ת������ʱ��----------------------------------------------
//����: �ѷֽ�ʱ�䣨������ʱ���룩ת��������ʱ��(1970������������),
//����: dt��������ʱ������ʽ��ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
s64 Tm_MkTime(struct tm *dt)
{
    u32 year = dt->tm_year;
    u32 mon  = dt->tm_mon;

    //��ʱ����������£������ж�������·ֽ�
    //ֱ����year/4 - year/100 + year/400
    if(0>= (int)(mon -= 2))
    {
        mon += 12;
        year--;
    }
    //��ǰ���ں�19691101�ֱ���빫Ԫ1��1��1�յ�������
    //Ȼ�����
    return ((((s64)(year/4 - year/100 + year/400 + 367*mon/12 + dt->tm_mday)
        + year*365 -719499) * 24 + dt->tm_hour) *60 + dt->tm_min)*60 + dt->tm_sec;
}

//----�ѷֽ�ʱ��ת������ʱ��(us)-------------------------------------------
//����: �ѷֽ�ʱ�䣨������ʱ���룩ת��������ʱ��(1970��������΢����),
//����: dt��������ʱ������ʽ��ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
s64 Tm_MkTimeUs(struct tm *dt)
{
    s64 us;
    us = Tm_MkTime(dt);
    us *= 1000000;
    us += dt->tm_us;
    return us;
}

//----������ʱ��ת�ɸ������ηֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970����������,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Tm_GmTime_r(s64 *time,struct tm *result)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time -= ((s64)gc_u32CfgTimeZone*3600);
    }
    else
    {
        temp_time = *time - ((s64)gc_u32CfgTimeZone*3600);
    }
    return Tm_LocalTime_r(&temp_time,result);
}

//----������ʱ��ת�ɸ������ηֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,����һ����̬����������
//      �������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970����������,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Tm_GmTime(s64 *time)
{
    static struct tm datetime;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time -= ((s64)gc_u32CfgTimeZone*3600);
    }
    else
    {
        temp_time = *time - ((s64)gc_u32CfgTimeZone*3600);
    }
    return Tm_LocalTime_r(&temp_time,&datetime);
}

//----������ʱ��ת�ɸ������ηֽ�ʱ��(us)---------------------------------------
//����: ������ʱ��(1970��������΢����)ת����������ʱ����+΢��,
//����: time,1970������΢����,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Tm_GmTimeUs_r(s64 *time,struct tm *result)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time -= (((s64)gc_u32CfgTimeZone*3600)*1000000);
    }
    else
    {
        temp_time = *time - ((s64)gc_u32CfgTimeZone*3600)*1000000;
    }
    return Tm_LocalTimeUs_r(&temp_time,result);
}

//----������ʱ��ת�ɸ������ηֽ�ʱ��(us)---------------------------------------
//����: ������ʱ��(1970��������΢����)ת����������ʱ����+΢��,����һ����̬����
//      �����ؽ������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970������΢����,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Tm_GmTimeUs(s64 *time)
{
    static struct tm datetime;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time -= (((s64)gc_u32CfgTimeZone*3600)*1000000);
    }
    else
    {
        temp_time = *time - ((s64)gc_u32CfgTimeZone*3600)*1000000;
    }
    return Tm_LocalTimeUs_r(&temp_time,&datetime);
}

//----������ʱ��(��)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970����������,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Tm_LocalTime_r(s64 *time,struct tm *result)
{
    u32 second, minute, hour;
    u32 day, month, year;

    u32 days;
    u32 years;
    u32 leap_years;
    u32 dayth;    // ����ĵ�x�죨1��1��0ʱ0��0�뼴Ϊ��1���ˣ�
    u32 day_of_week;
    u32 tmp_month_days;
    u32 temp,temp1;
    s64 temp_time;

    if(result == NULL)
        return NULL;
    if(time != NULL)
        temp_time = *time;
    else
        temp_time = __Rtc_Time(NULL);

    second = temp_time % 60;
    minute = temp_time/60 %60;
    hour = temp_time/3600 %24;
    days = (u32)(temp_time/86400);

    years = days / 365;
    leap_years = (years + 1) >> 2; // ��������ȵ���
    if(((years+1970 - 1)/100 != ((years+1970)/100)) && ((years+1970 )%400)!=0 )
        leap_years++;
    leap_years -= (years+1970)/100 - 1970/100;
    leap_years += (years+1970)/400 - 1970/400;
    temp = days%365;
    temp1 = temp;
    if(temp < leap_years)
    {
        years--;
        temp += 365;
    }
    dayth = temp - leap_years + 1;    // ����ȵ�x�죨��1��1��0ʱΪ��1�죩

    year = 1970 + years;    //δ�����������1400���������

    g_u32MonthDays[1] = 28;
    if (year%4 == 0)        // ������������꣬��ô����2�·����һ��
    {
        if ((year%100 == 0) && (year%400 != 0))
        {
            // ���겻���İ�����
        }
        else
        {
            g_u32MonthDays[1] = 29;
            if(temp1 < leap_years)// ������������������С�������������һ��
                dayth +=1;
        }
    }

    month = 1;
    tmp_month_days = g_u32MonthDays[month-1];
    while (dayth > tmp_month_days)
    {
        dayth -= tmp_month_days;
        tmp_month_days = g_u32MonthDays[month];
        month++;
    }
    day = dayth;

    day_of_week = (days + 4) % 7;

    result->tm_yday = day;           // days since January 1 -[0,365]
    result->tm_us = 0;               // us����
    result->tm_sec  = second;        // seconds after the minute - [0,59]
    result->tm_min  = minute;        // minutes after the hour - [0,59]
    result->tm_hour = hour;          // hours since midnight - [0,23]
    result->tm_wday = day_of_week;   // days since Sunday - [0,6]
    result->tm_mday = day;           // day of the month - [1,31]
    result->tm_mon  = month;         // months since January - [0,11]
    result->tm_year = year;          // years 1970-

    return result;
}

//----������ʱ��(��)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,����һ����̬����������
//      �������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970����������,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Tm_LocalTime(s64 *time)
{
    static struct tm datetime;

    return Tm_LocalTime_r(time,&datetime);
}

//----������ʱ��(��)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970������΢����,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Tm_LocalTimeUs_r(s64 *time_us,struct tm *result)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time_us == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time /=1000000;
    }
    else
    {
        temp_time = *time_us/1000000;
    }
    Tm_LocalTime_r(&temp_time,result);
    result->tm_us = temp_time%1000000;
    return result;
}

//----������ʱ��(us)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970������΢����,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Tm_LocalTimeUs(s64 *time_us)
{
    static struct tm datetime;
    s64 temp_time;
    if(time_us == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time /=1000000;
    }
    else
    {
        temp_time = *time_us/1000000;
    }

    Tm_LocalTime_r(&temp_time,&datetime);
    datetime.tm_us = temp_time%1000000;
    return &datetime;
}

//----��������ʱ��-------------------------------------------------------------
//����: �ø�ʽΪ"2011/10/28,22:37:50"���ַ�����ʾ��ʱ����������ʱ�䣬�ֹ�����ʱ
//      ��󣬿ɵ��ñ���������ϵͳ����ʱ��
//����: buf������ʱ��ֵ
//����: 1=�ɹ�������ֵ:��Ӧ�Ĵ������
//-----------------------------------------------------------------------------
s32 Tm_SetDateTimeStr(char *buf)
{
    s64 nowtime;
    struct tm ptDateTime;
    char *sepstr;
    char sepch[] = "/";
    u32 res;

    sepch[0] = '/';
    sepstr = strsep(&buf, sepch);
    res = strtol(sepstr, (char **)NULL, 0);
    if ((1970>res) || (res>2100))
    {
        return EN_CLOCK_YEAR_ERROR;
    }
    ptDateTime.tm_year = res;

    sepstr = strsep(&buf, sepch);
    res = strtol(sepstr, (char **)NULL, 0);
    if ((1>res) || (res>12))
    {
        return EN_CLOCK_MON_ERROR;
    }
    ptDateTime.tm_mon = res;

    sepch[0] = ',';
    sepstr = strsep(&buf, sepch);
    res = strtol(sepstr, (char **)NULL, 0);
    if ((1>res) || (res>31))
    {
        return EN_CLOCK_DAY_ERROR;
    }
    if (res > g_u32MonthDays[ptDateTime.tm_mon-1])
    {
        return EN_CLOCK_DAY_ERROR;
    }
    if (ptDateTime.tm_mon == 2)
    {
        if (res > 29)
        {
            return EN_CLOCK_DAY_ERROR;
        }

        if (ptDateTime.tm_year%4 == 0)
        {
            if (ptDateTime.tm_year%100 == 0)
            {
                if (ptDateTime.tm_year%400)
                {
                    if (res > 28)
                        return EN_CLOCK_DAY_ERROR;
                }
            }
        }
        else
        {
            if (res > 28)
                return EN_CLOCK_DAY_ERROR;
        }
    }
    ptDateTime.tm_mday = res;

    sepch[0] = ':';
    sepstr = strsep(&buf, sepch);
    res = strtol(sepstr, (char **)NULL, 0);
    if (res>23)
    {
        return EN_CLOCK_HOUR_ERROR;
    }
    ptDateTime.tm_hour = res;

    sepstr = strsep(&buf, sepch);
    res = strtol(sepstr, (char **)NULL, 0);
    if (res>59)
    {
        return EN_CLOCK_MIN_ERROR;
    }
    ptDateTime.tm_min = res;

    sepstr = strsep(&buf, sepch);
    res = strtol(sepstr, (char **)NULL, 0);
    if (res>59)
    {
        return EN_CLOCK_SEC_ERROR;
    }
    ptDateTime.tm_sec = res;
    ptDateTime.tm_us = 0;

    nowtime = Tm_MkTimeUs(&ptDateTime);
    //use the rtc module to set
    __Rtc_SetTime(nowtime);
    return 1;
}

//----��������ʱ��-------------------------------------------------------------
//����: �÷ֽ�ʱ���������ʱ�䣬��rtcӲ����ϵͳ�У����Ӳ��ֱ������ֽ�ʱ��ģ�
//      Ӳ���������ñ�������
//      ��Ϊ����ʱ��ֻ��ȷ���룬���tm�е�us��Ա����Ӳ����������ά��������������
//      ȡ�����Ӳ����ά��us��Ա(����tm_connect_rtcʱ����get_rtc_hard_us=NULL),
//      rtc driver��ʹ���жϷ�ʽ����ȷ��������ʱ��������get_rtc_hard_us��ϵͳ��
//      �Դ�ʱ��Ϊ������us����
//����: tm,��ǰʱ�䣬��������ʱ����ֽ�
//����: ��
//-----------------------------------------------------------------------------
void Tm_SetDateTime(struct tm *tm)
{
    s64 nowtime;
    nowtime = Tm_MkTimeUs(tm);
    //use the rtc module to set
    __Rtc_SetTime(nowtime);
}

//----�ѷֽ�ʱ��ת���ַ���-----------------------------------------------------
//����: ��һ���ֽ�ʱ��ת���ַ�������ʽ"2011/10/28,22:37:50"
//����: tm,�ֽ�ʱ��
//      buf�����ڷ��ؽ���������ַ������21�ֽڡ�
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
void Tm_AscTime(struct tm *tm, char buf[])
{
    itoa(tm->tm_year, &buf[0], 10);
    buf[4] = '/';
    itoa(tm->tm_mon, &buf[5], 10);
    if(buf[6] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[6] = buf[5];
        buf[5] = '0';
    }
    buf[7] = '/';
    itoa(tm->tm_mday, &buf[8], 10);
    if(buf[9] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[9] = buf[8];
        buf[8] = '0';
    }
    buf[10] = ',';
    itoa(tm->tm_hour, &buf[11], 10);
    if(buf[12] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[12] = buf[11];
        buf[11] = '0';
    }
    buf[13] = ':';
    itoa(tm->tm_min, &buf[14], 10);
    if(buf[15] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[15] = buf[14];
        buf[14] = '0';
    }
    buf[16] = ':';
    itoa(tm->tm_sec, &buf[17], 10);
    if(buf[18] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[18] = buf[17];
        buf[17] = '0';
    }
    buf[19] = '\0';

    return ;
}

//----�ѷֽ�ʱ��ת���ַ���(ms)-------------------------------------------------
//����: ��һ���ֽ�ʱ��ת���ַ�������ʽ"2011/10/28,22:37:50:000"
//����: tm���ֽ�ʱ��
//      buf�����ڷ��ؽ���������ַ������24�ֽڡ�
//����: �ַ���
//----------------------------------------------------------------------------
void Tm_AscTimeMs(struct tm *tm, char buf[])
{
    Tm_AscTime(tm,buf);
    buf[19] = ':';
    itoa(tm->tm_us/1000, &buf[20], 3);
    buf[23] = '\0';
    return ;
}

//----�ѷֽ�ʱ��ת���ַ���(us)-------------------------------------------------
//����: ��һ���ֽ�ʱ��ת���ַ�������ʽ"2011/10/28,22:37:50:000000"
//����: tm,�ֽ�ʱ��
//      buf�����ڷ��ؽ���������ַ������27�ֽڡ�
//����: �ַ���
//----------------------------------------------------------------------------
void Tm_AscTimeUs(struct tm *tm, char buf[])
{
    Tm_AscTime(tm,buf);
    buf[19] = ':';
    itoa(tm->tm_us, &buf[20], 6);
    buf[26] = '\0';
    return ;
}
//----timeģ���ʼ��-----------------------------------------------------------
//����: ��ʼ������ʱ��ģ��
//����: para����ʹ��
//����: ��ʹ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_TM(ptu32_t para)
{
    return true;
}

//----ȡ����ʱ��(��)-----------------------------------------------------------
//����: ȡ��1970��1��1��0:0:0�����ڵ�ʱ��������
//����: ret,�ǿ��򷵻�����ʱ�䡣
//����: ����ʱ��
//-----------------------------------------------------------------------------
s64 TM_Time(s64 *ret)
{
    return __Rtc_Time(ret);
}
//----ȡ����ʱ��(΢��)---------------------------------------------------------
//����: ȡ��1970��1��1��0:0:0�����ڵ�ʱ����������ȷ��ֻ�ܵ�tick���
//����: ret,�ǿ��򷵻�����ʱ�䡣
//����: ����ʱ��
//-----------------------------------------------------------------------------
s64 TM_TimeUs(s64 *ret)
{
    return __Rtc_TimeUs(ret);
}

