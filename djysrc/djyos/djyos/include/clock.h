//----------------------------------------------------
//Copyright (C), 2004-2011,  都江堰操作系统研发团队
//版权所有 (C), 2004-2011,   都江堰操作系统研发团队
//所属模块:时钟模块
//作者:  王建忠
//版本：V1.0.0
//文件描述: 为系统提供时钟
//其他说明:
//修订历史:
// 1. 日期: 2011-10-28
//   作者:  王建忠
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#ifndef __clock_h__
#define __clock_h__

#ifdef __cplusplus
extern "C" {
#endif

struct tm 
{
    s32 tm_sec;     // 秒–取值区间为[0,59] 
    s32 tm_min;     // 分 - 取值区间为[0,59] 
    s32 tm_hour;    // 时 - 取值区间为[0,23] 
    s32 tm_mday;    // 一个月中的日期 - 取值区间为[1,31] 
    s32 tm_mon;     // 月份（从一月开始，0代表一月） - 取值区间为[0,11] 
    s32 tm_year;    // 年份，其值从1900开始 
    s32 tm_wday;    // 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，
                    // 以此类推 
    s32 tm_yday;    // 从每年的1月1日开始的天数–取值区间为[0,365]，其中0代表
                    //1月1日，1代表1月2日，以此类推 
    s32 tm_isdst;   // 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行
                    //夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
};

extern char tm_wdays[][8];

ptu32_t module_init_datatime(ptu32_t para);
s64 djy_time(s64 *ret);
struct tm localtime(s64 time);

s64 __date_to_binary(struct tm *dt);
void time_set_datatime_tm(struct tm *tm);
struct tm gmtime(s64 time);
void time_connect_rtc(bool_t (*set_rtc_hard)(struct tm *DateTime));

#ifdef __cplusplus
}
#endif

#endif

