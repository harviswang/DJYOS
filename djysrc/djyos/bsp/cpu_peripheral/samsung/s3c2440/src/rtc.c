//----------------------------------------------------
//Copyright (C), 2012-, Merlin
//��Ȩ���� (C)��2012-��Merlin
//����ģ�飺ʱ��ģ��
//���ߣ�Merlin
//�汾��V0.1.0
//�ļ�������DJYOS��Ӳ��RTC�����ļ�
//����˵����
//�޶���ʷ��
//1. ����: 2012-07-05
//   ���ߣ�Merlin
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
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

// RTC�ж�
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

        time_set_datatime_tm(&dtm); // ��OS��Clockģ�鴫���µ�ʱ��ֵ
    }
    rRTCCON &= ~1 ;     //RTC read and write disable

    return 0;
}

// RTCֵ����
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

//----��ʼ��rtcʵʱʱ��ģ��------------------------------------------------------
//���ܣ���ʼ��RTCģ�顣
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_rtc(ptu32_t para)
{
    rRTCCON = 1 ;       //RTC read and write enable
    rALMSEC = 1;        // 1S�ж�һ��
    rRTCALM = (1<<6) | (1<<0);     // ʹ�����ж�
    rRTCCON &= ~1 ;     //RTC read and write disable

    time_connect_rtc(rtc_time_update);

    int_isr_connect(cn_int_line_rtc, rtc_isr);
    int_setto_asyn_signal(cn_int_line_rtc);
    int_echo_line(cn_int_line_rtc);
    int_restore_asyn_line(cn_int_line_rtc);

    return true;
}


