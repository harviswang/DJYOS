//----------------------------------------------------
//Copyright (C), 2012-, ��ǫ
//��Ȩ���� (C)��2012-����ǫ
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

#include "os_inc.h"
#include "stm32f10x.h"
#include "cpu_peri.h"

void RTC_Configuration(void)
{
	// ʹ��RTC��BKP��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	// ʹ�ܶ�RTC��BKP�ļĴ����ķ���
	BKP_DeInit();	// ��λBKP����ʼ��״̬
	RCC_LSEConfig(RCC_LSE_ON);	// �����ⲿ��Ƶ����
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	// �ȴ���ʼ�����
	{}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	// ѡ���ⲿ����ʱ��
	RCC_RTCCLKCmd(ENABLE);	// ʹ��RTC��ʱ��
	RTC_WaitForSynchro();	// �ȴ�RTC�Ĵ�����APBʱ��ͬ��
	RTC_WaitForLastTask();	// �ȴ����һ��RTCд�Ĵ������

	RTC_ITConfig(RTC_IT_SEC, DISABLE);	// ��ֹRTC�����ж�
	RTC_WaitForLastTask();	// �ȴ����һ��RTCд�Ĵ������

	// ����RTC�ķ�Ƶֵ
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask();	// �ȴ����һ��RTCд�Ĵ������
}

// RTC�ж�
u32 rtc_isr(ufast_t rtc_int_line)
{
	s64 rtc_cnt;
	struct tm dtm;

	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);	// ���RTC�����ж�
		rtc_cnt = RTC_GetCounter();
		dtm = localtime(rtc_cnt);
		time_set_datatime_tm(&dtm);	// ��OS��Clockģ�鴫���µ�ʱ��ֵ
		RTC_WaitForLastTask();
	}

	return 0;
}

// RTCֵ����
bool_t rtc_time_update(struct tm *DateTime)
{
	s64 rtc_cnt;

	rtc_cnt = __date_to_binary(DateTime);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	// ʹ�ܶ�RTC��BKP�ļĴ����ķ���
	RTC_WaitForSynchro();

	RTC_SetCounter((u32)rtc_cnt);
	RTC_WaitForLastTask();

	return true;
}

//----��ʼ��rtcʵʱʱ��ģ��------------------------------------------------------
//���ܣ���ʼ��RTCģ�顣
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_rtc(ptu32_t para)
{
	// �鿴���ݼĴ������������0xA5A5�Ļ������ʾ��δ��ʼ����RTC�������ڶϵ�֮ǰ��
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		RTC_Configuration();	// ����RTC
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	// ��־RTC�Ѿ���ʼ����
	}

	RTC_WaitForSynchro();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);	// ʹ��RTC�����ж�
	RTC_WaitForLastTask();	// �ȴ����һ��RTCд�Ĵ������

	time_connect_rtc(rtc_time_update);

	int_isr_connect(cn_int_line_RTC, rtc_isr);
	int_setto_asyn_signal(cn_int_line_RTC);
	int_echo_line(cn_int_line_RTC);
	int_restore_asyn_line(cn_int_line_RTC);

	return true;
}
