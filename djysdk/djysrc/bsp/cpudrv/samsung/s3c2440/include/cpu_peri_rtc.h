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
// �ļ���     ��cpu_peri_rtc.h
// ģ������: RTCʱ��ģ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 25/09.2014
// =============================================================================
#ifndef __CPU_PERI_RTC_H__
#define __CPU_PERI_RTC_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// RTC
#ifdef __BIG_ENDIAN
#define rRTCCON    (*(volatile unsigned char *)0x57000043)  //RTC control
#define rTICNT     (*(volatile unsigned char *)0x57000047)  //Tick time count
#define rRTCALM    (*(volatile unsigned char *)0x57000053)  //RTC alarm control
#define rALMSEC    (*(volatile unsigned char *)0x57000057)  //Alarm second
#define rALMMIN    (*(volatile unsigned char *)0x5700005b)  //Alarm minute
#define rALMHOUR   (*(volatile unsigned char *)0x5700005f)  //Alarm Hour
#define rALMDATE   (*(volatile unsigned char *)0x57000063)  //Alarm date   //edited by junon
#define rALMMON    (*(volatile unsigned char *)0x57000067)  //Alarm month
#define rALMYEAR   (*(volatile unsigned char *)0x5700006b)  //Alarm year
#define rRTCRST    (*(volatile unsigned char *)0x5700006f)  //RTC round reset
#define rBCDSEC    (*(volatile unsigned char *)0x57000073)  //BCD second
#define rBCDMIN    (*(volatile unsigned char *)0x57000077)  //BCD minute
#define rBCDHOUR   (*(volatile unsigned char *)0x5700007b)  //BCD hour
#define rBCDDATE   (*(volatile unsigned char *)0x5700007f)  //BCD date  //edited by junon
#define rBCDDAY    (*(volatile unsigned char *)0x57000083)  //BCD day   //edited by junon
#define rBCDMON    (*(volatile unsigned char *)0x57000087)  //BCD month
#define rBCDYEAR   (*(volatile unsigned char *)0x5700008b)  //BCD year

#else //Little Endian
#define rRTCCON    (*(volatile unsigned char *)0x57000040)  //RTC control
#define rTICNT     (*(volatile unsigned char *)0x57000044)  //Tick time count
#define rRTCALM    (*(volatile unsigned char *)0x57000050)  //RTC alarm control
#define rALMSEC    (*(volatile unsigned char *)0x57000054)  //Alarm second
#define rALMMIN    (*(volatile unsigned char *)0x57000058)  //Alarm minute
#define rALMHOUR   (*(volatile unsigned char *)0x5700005c)  //Alarm Hour
#define rALMDATE   (*(volatile unsigned char *)0x57000060)  //Alarm date  // edited by junon
#define rALMMON    (*(volatile unsigned char *)0x57000064)  //Alarm month
#define rALMYEAR   (*(volatile unsigned char *)0x57000068)  //Alarm year
#define rRTCRST    (*(volatile unsigned char *)0x5700006c)  //RTC round reset
#define rBCDSEC    (*(volatile unsigned char *)0x57000070)  //BCD second
#define rBCDMIN    (*(volatile unsigned char *)0x57000074)  //BCD minute
#define rBCDHOUR   (*(volatile unsigned char *)0x57000078)  //BCD hour
#define rBCDDATE   (*(volatile unsigned char *)0x5700007c)  //BCD date  //edited by junon
#define rBCDDAY    (*(volatile unsigned char *)0x57000080)  //BCD day   //edited by junon
#define rBCDMON    (*(volatile unsigned char *)0x57000084)  //BCD month
#define rBCDYEAR   (*(volatile unsigned char *)0x57000088)  //BCD year
#endif  //RTC

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_RTC_H__

