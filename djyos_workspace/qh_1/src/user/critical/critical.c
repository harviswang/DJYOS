//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:安全钩子
//作者：lst
//版本：V1.0.0
//文件描述:安全钩子是必须在开机后尽可能快地执行的部分代码
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"
const ufast_t tg_int_used[] =
{
	cn_int_line_wwdg,            // (0) - Window WatchDog Interrupt
	cn_int_line_PVD,             // (1) - PVD through EXTI Line detection Interrupt
	cn_int_line_TAMPER,          // (2) - Tamper Interrupt
	cn_int_line_RTC,             // (3) - RTC global Interrupt
	cn_int_line_FLASH,           // (4) - FLASH global Interrupt
	cn_int_line_RCC,             // (5) - RCC global Interrupt
	cn_int_line_EXTI0,           // (6) - EXTI Line0 Interrupt
	cn_int_line_EXTI1,           // (7) - EXTI Line1 Interrupt
	cn_int_line_EXTI2,           // (8) - EXTI Line2 Interrupt
	cn_int_line_EXTI3,           // (9) - EXTI Line3 Interrupt
	cn_int_line_EXTI4,           // (0) - EXTI Line4 Interrupt
	cn_int_line_DMA1_Ch1,        // (11) - DMA1 Channel 1 global Interrupt
	cn_int_line_DMA1_Ch2,        // (12) - DMA1 Channel 2 global Interrupt
	cn_int_line_DMA1_Ch3,        // (13) - DMA1 Channel 3 global Interrupt
	cn_int_line_DMA1_Ch4,        // (14) - DMA1 Channel 4 global Interrupt
	cn_int_line_DMA1_Ch5,        // (15) - DMA1 Channel 5 global Interrupt
	cn_int_line_DMA1_Ch6,        // (16) - DMA1 Channel 6 global Interrupt
	cn_int_line_DMA1_Ch7,        // (17) - DMA1 Channel 7 global Interrupt
	cn_int_line_ADC1_2,          // (18) - ADC1 et ADC2 global Interrupt
	cn_int_line_USB_HP_CAN_TX,   // (19) - USB High Priority or CAN TX Interrupts
	cn_int_line_USB_LP_CAN_RX0,  // (20) - USB Low Priority or CAN RX0 Interrupts
	cn_int_line_CAN_RX1,         // (21) - CAN RX1 Interrupt
	cn_int_line_CAN_SCE,         // (22) - CAN SCE Interrupt
	cn_int_line_EXTI9_5,         // (23) - External Line[9:5] Interrupts
	cn_int_line_TIM1_BRK,        // (24) - TIM1 Break Interrupt
	cn_int_line_TIM1_UP,         // (25) - TIM1 Update Interrupt
	cn_int_line_TIM1_TRG_COM,    // (26) - TIM1 Trigger and Commutation Interrupt
	cn_int_line_TIM1_CC,         // (27) - TIM1 Capture Compare Interrupt
	cn_int_line_TIM2,            // (28) - TIM2 global Interrupt
	cn_int_line_TIM3,            // (29) - TIM3 global Interrupt
	cn_int_line_TIM4,            // (30) - TIM4 global Interrupt
	cn_int_line_I2C1_EV,         // (31) - I2C1 Event Interrupt
	cn_int_line_I2C1_ER,         // (32) - I2C1 Error Interrupt
	cn_int_line_I2C2_EV,         // (33) - I2C2 Event Interrupt
	cn_int_line_I2C2_ER,         // (34) - I2C2 Error Interrupt
	cn_int_line_SPI1,            // (35) - SPI1 global Interrupt
	cn_int_line_SPI2,            // (36) - SPI2 global Interrupt
	cn_int_line_USART1,          // (37) - USART1 global Interrupt
	cn_int_line_USART2,          // (38) - USART2 global Interrupt
	cn_int_line_USART3,          // (39) - USART3 global Interrupt
	cn_int_line_EXTI15_10,       // (40) - External Line[15:10] Interrupts
	cn_int_line_RTCAlarm,        // (41) - RTC Alarm through EXTI Line Interrupt
	cn_int_line_USBWakeUp,       // (42) - USB WakeUp from suspend through EXTI Line Interrupt
	cn_int_line_TIM8_BRK,        // (43) - TIM8 Break Interrupt
	cn_int_line_TIM8_UP,         // (44) - TIM8 Update Interrupt
	cn_int_line_TIM8_TRG_COM,    // (45) - TIM8 Trigger and Commutation Interrupt
	cn_int_line_TIM8_CC,         // (46) - TIM8 Capture Compare Interrupt
	cn_int_line_ADC3,            // (47) - ADC3 global Interrupt
	cn_int_line_FSMC,            // (48) - FSMC global Interrupt
	cn_int_line_SDIO,            // (49) - SDIO global Interrupt
	cn_int_line_TIM5,            // (50) - TIM5 global Interrupt
	cn_int_line_SPI3,            // (51) - SPI3 global Interrupt
	cn_int_line_UART4,           // (52) - UART4 global Interrupt
	cn_int_line_UART5,           // (53) - UART5 global Interrupt
	cn_int_line_TIM6,            // (54) - TIM6 global Interrupt
	cn_int_line_TIM7,            // (55) - TIM7 global Interrupt
	cn_int_line_DMA2_Ch1,        // (56) - DMA2 Channel 1 global Interrupt
	cn_int_line_DMA2_Ch2,        // (57) - DMA2 Channel 2 global Interrupt
	cn_int_line_DMA2_Ch3,        // (58) - DMA2 Channel 3 global Interrupt
	cn_int_line_DMA2_Ch4_5,      // (59) - DMA2 Channel 4 and DMA2 Channel 5 global Interrupt
};

const ufast_t ufg_int_used_num = sizeof(tg_int_used)/sizeof(ufast_t);
//定义中断线控制数据结构
struct int_line tg_int_table[sizeof(tg_int_used)/sizeof(ufast_t)];

//----配置全部IO口--------------------------------------------------------------
//功能：除必须的外，全部配置成初始态，各功能口由相应的功能模块自行定义。
//参数：无
//返回：无
//------------------------------------------------------------------------------
void gpio_init(void)
{
}

void critical(void)
{
    gpio_init();        //建议早执行,因为硬件的初始状态可能关乎安全
}
struct int_line *djy_get_int_table()
{
    return tg_int_table;

}
ufast_t djy_get_int_sued_num()
{
    return ufg_int_used_num;
}


