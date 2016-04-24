/*
 * fram_alm_timer.h
 *
 *  Created on: Sep 26, 2013
 *      Author: Administrator
 */

#ifndef FRAM_ALM_TIMER_H_
#define FRAM_ALM_TIMER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "sunri_types.h"

typedef struct                  // �ṹ��������BCD����ʽ��ʾ����day��0x16������16�ţ�������22��
{
    UINT8      byAlmHundredSec;          // ��İٷ�֮һ
    UINT8      byAlmSecond;              // ��
    UINT8      byAlmMinute;            // ��
    UINT8      byAlmHour;                // Сʱ
    UINT8      byAlmDate;              // �� 2009-9-14 14:18:18
//  UINT8      byAlmDay;                //
//    UINT8      byAlmMonth;                // ��
//  UINT8      byAlmYearL;                // ��
} tagAlmTime;



STATUS FRAM_Set_Alm_Time(tagAlmTime *pTime);
STATUS FRAM_Get_Alm_Time(tagAlmTime *pTime);

#ifdef __cplusplus
}
#endif


#endif
