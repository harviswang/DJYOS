//----------------------------------------------------
//Copyright (C), 2012-, Merlin
//版权所有 (C)，2012-，Merlin
//所属模块：时钟模块
//作者：Merlin
//版本：V0.1.0
//文件描述：DJYOS与硬件RTC连接文件
//其他说明：
//修订历史：
//1. 日期: 2012-07-05
//   作者：Merlin
//   新版本号：V0.1.0
//   修改说明：原始版本
//------------------------------------------------------

#include "os_inc.h"
#include "cpu_peri.h"

//unsigned long power(int base, int times)
//{
//    int i;
//    unsigned long rslt = 1;
//    for(i=0; i<times; i++)
//        rslt *= base;
//    return rslt;
//}
//
//unsigned long  BCDtoDec(const unsigned char *bcd, int length)
//{
//     int i, tmp;
//     unsigned long dec = 0;
//     for(i=0; i<length; i++)
//     {
//        tmp = ((bcd[i]>>4)&0x0F)*10 + (bcd[i]&0x0F);
//        dec += tmp * power(100, length-1-i);
//     }
//     return dec;
//}
//
//int DectoBCD(int Dec, unsigned char *Bcd, int length)
//{
//     int i;
//     int temp;
//     for(i=length-1; i>=0; i--)
//     {
//         temp = Dec%100;
//         Bcd[i] = ((temp/10)<<4) + ((temp%10) & 0x0F);
//         Dec /= 100;
//     }
//     return 0;
//}

// RTC中断
u32 rtc_isr(ufast_t rtc_int_line)
{
    struct tm dtm;

    rRTCCON = 1 ;       //RTC read and write enable
    if (rALMSEC == 0)
    {
        rALMSEC = 1;
        dtm.tm_year = rBCDYEAR;
        dtm.tm_mon = rBCDMON;
        dtm.tm_mday = rBCDDAY;
        dtm.tm_hour = rBCDHOUR;
        dtm.tm_min = rBCDMIN;
        dtm.tm_sec = rBCDSEC;

        time_set_datatime_tm(&dtm); // 向OS的Clock模块传递新的时间值
    }
    rRTCCON &= ~1 ;     //RTC read and write disable

    return 0;
}

// RTC值更新
bool_t rtc_time_update(struct tm *dtm)
{
    rRTCCON = 1 ;       //RTC read and write enable
    rBCDYEAR = dtm->tm_year;
    rBCDMON = dtm->tm_mon;
    rBCDDAY = dtm->tm_mday;
    rBCDYEAR = dtm->tm_hour;
    rBCDYEAR = dtm->tm_min;
    rBCDYEAR = dtm->tm_sec;
    rRTCCON &= ~1 ;     //RTC read and write disable

    return true;
}

//----初始化rtc实时时钟模块------------------------------------------------------
//功能：初始化RTC模块。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_rtc(ptu32_t para)
{
    rRTCCON = 1 ;       //RTC read and write enable
    rALMSEC = 1;        // 1S中断一次
    rRTCALM = (1<<6) | (1<<0);     // 使能秒中断
    rRTCCON &= ~1 ;     //RTC read and write disable

    time_connect_rtc(rtc_time_update);

    int_isr_connect(cn_int_line_rtc, rtc_isr);
    int_setto_asyn_signal(cn_int_line_rtc);
    int_echo_line(cn_int_line_rtc);
    int_restore_asyn_line(cn_int_line_rtc);

    return true;
}


