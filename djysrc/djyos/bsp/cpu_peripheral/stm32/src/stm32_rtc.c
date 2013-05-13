//----------------------------------------------------
//Copyright (C), 2012-, 林谦
//版权所有 (C)，2012-，林谦
//所属模块：时钟模块
//作者：林谦
//版本：V0.1.0
//文件描述：DJYOS与硬件RTC连接文件
//其他说明：
//修订历史：
//2. 日期：2012-06-26
//   作者：Merlin
//   新版本号：
//   修改说明：事件初始化在djyos/clock.c中进行，本文件用于操作运行平台中的RTC硬件
//1. 日期: 2012-05-02
//   作者：林谦
//   新版本号：V0.1.0
//   修改说明：原始版本
//------------------------------------------------------

#include "os_inc.h"
#include "stm32f10x.h"
#include "cpu_peri.h"

void RTC_Configuration(void)
{
	// 使用RTC及BKP的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	// 使能对RTC及BKP的寄存器的访问
	BKP_DeInit();	// 复位BKP到初始化状态
	RCC_LSEConfig(RCC_LSE_ON);	// 配置外部低频振荡器
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	// 等待初始化完成
	{}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	// 选择外部低速时钟
	RCC_RTCCLKCmd(ENABLE);	// 使能RTC的时钟
	RTC_WaitForSynchro();	// 等待RTC寄存器与APB时钟同步
	RTC_WaitForLastTask();	// 等待最后一次RTC写寄存器完成

	RTC_ITConfig(RTC_IT_SEC, DISABLE);	// 禁止RTC的秒中断
	RTC_WaitForLastTask();	// 等待最后一次RTC写寄存器完成

	// 设置RTC的分频值
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask();	// 等待最后一次RTC写寄存器完成
}

// RTC中断
u32 rtc_isr(ufast_t rtc_int_line)
{
	s64 rtc_cnt;
	struct tm dtm;

	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);	// 清除RTC的秒中断
		rtc_cnt = RTC_GetCounter();
		dtm = localtime(rtc_cnt);
		time_set_datatime_tm(&dtm);	// 向OS的Clock模块传递新的时间值
		RTC_WaitForLastTask();
	}

	return 0;
}

// RTC值更新
bool_t rtc_time_update(struct tm *DateTime)
{
	s64 rtc_cnt;

	rtc_cnt = __date_to_binary(DateTime);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	// 使能对RTC及BKP的寄存器的访问
	RTC_WaitForSynchro();

	RTC_SetCounter((u32)rtc_cnt);
	RTC_WaitForLastTask();

	return true;
}

//----初始化rtc实时时钟模块------------------------------------------------------
//功能：初始化RTC模块。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_rtc(ptu32_t para)
{
	// 查看备份寄存器，如果不是0xA5A5的话，则表示从未初始化过RTC（包括在断电之前）
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		RTC_Configuration();	// 配置RTC
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	// 标志RTC已经初始化过
	}

	RTC_WaitForSynchro();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);	// 使能RTC的秒中断
	RTC_WaitForLastTask();	// 等待最后一次RTC写寄存器完成

	time_connect_rtc(rtc_time_update);

	int_isr_connect(cn_int_line_RTC, rtc_isr);
	int_setto_asyn_signal(cn_int_line_RTC);
	int_echo_line(cn_int_line_RTC);
	int_restore_asyn_line(cn_int_line_RTC);

	return true;
}
