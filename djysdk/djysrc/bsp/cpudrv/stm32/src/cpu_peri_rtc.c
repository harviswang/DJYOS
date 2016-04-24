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
//����ģ�飺ʱ��ģ��
//���ߣ���ǫ
//�汾��V0.1.0
//�ļ�������DJYOS��Ӳ��RTC�����ļ�
//����˵����
//�޶���ʷ��
//2. ���ڣ�2012-06-26
//   ���ߣ�Merlin
//   �°汾�ţ�
//   �޸�˵�����¼���ʼ����djyos/clock.c�н��У����ļ����ڲ�������ƽ̨�е�RTCӲ��
//1. ����: 2012-05-02
//   ���ߣ���ǫ
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------
#include "stddef.h"
#include "time.h"
#include "stm32f10x.h"
#include "cpu_peri.h"

// =============================================================================
// ���ܣ�RTCӲ���Ĵ�������
// ��������
// ���أ���
// =============================================================================
void RTC_Configuration(void)
{
    // ʹ��RTC��BKP��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);    // ʹ�ܶ�RTC��BKP�ļĴ����ķ���
    BKP_DeInit();   // ��λBKP����ʼ��״̬
    RCC_LSEConfig(RCC_LSE_ON);  // �����ⲿ��Ƶ����
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) // �ȴ���ʼ�����
    {}

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // ѡ���ⲿ����ʱ��
    RCC_RTCCLKCmd(ENABLE);  // ʹ��RTC��ʱ��
    RTC_WaitForSynchro();   // �ȴ�RTC�Ĵ�����APBʱ��ͬ��
    RTC_WaitForLastTask();  // �ȴ����һ��RTCд�Ĵ������

    RTC_ITConfig(RTC_IT_SEC, DISABLE);  // ��ֹRTC�����ж�
    RTC_WaitForLastTask();  // �ȴ����һ��RTCд�Ĵ������

    // ����RTC�ķ�Ƶֵ
    /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();  // �ȴ����һ��RTCд�Ĵ������
}

// =============================================================================
// ���ܣ���ȡRTCʱ�䣬��ȡ��ʱ����S���õ�΢��Ҫ����1000000
// ������time,ʱ�䣬��λΪ΢��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t RTC_TimeGet(s64 *time)
{
    *time = 1000000 * RTC_GetCounter();
    RTC_WaitForLastTask();

    return true;
}

// =============================================================================
// ���ܣ�����RTCʱ�䣬RTC�����ʱ����S�����Ҫ����1000000
// ������time,ʱ�䣬��λΪ΢��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t RTC_TimeUpdate(s64 time)
{
    u32 time_s;

    time_s = (u32)(time/1000000);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);    // ʹ�ܶ�RTC��BKP�ļĴ����ķ���
    RTC_WaitForSynchro();

    RTC_SetCounter((u32)time_s);
    RTC_WaitForLastTask();

    return true;
}

// =============================================================================
// ���ܣ�RTCģ���ʼ��������ע��ӿں���������RTC
// ������para,��ʱ����
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    // �鿴���ݼĴ������������0xA5A5�Ļ������ʾ��δ��ʼ����RTC�������ڶϵ�֮ǰ��
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        RTC_Configuration();    // ����RTC
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);   // ��־RTC�Ѿ���ʼ����
    }

    RTC_WaitForSynchro();

    Rtc_RegisterDev(RTC_TimeGet,RTC_TimeUpdate);
    return true;
}
