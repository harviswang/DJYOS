//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:中断模块
//作者：lst
//版本：V1.0.0
//文件描述: 定义中断号
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009--9-28
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __int_line_h__
#define __int_line_h__

#ifdef __cplusplus
extern "C" {
#endif

#define cn_int_line_wwdg            (0x00)  //Window WatchDog Interrupt
#define cn_int_line_PVD             (0x01)  //PVD through EXTI Line detection Interrupt
#define cn_int_line_TAMPER          (0x02)  //Tamper Interrupt
#define cn_int_line_RTC             (0x03)  //RTC global Interrupt
#define cn_int_line_FLASH           (0x04)  //FLASH global Interrupt
#define cn_int_line_RCC             (0x05)  //RCC global Interrupt
#define cn_int_line_EXTI0           (0x06)  //EXTI Line0 Interrupt
#define cn_int_line_EXTI1           (0x07)  //EXTI Line1 Interrupt
#define cn_int_line_EXTI2           (0x08)  //EXTI Line2 Interrupt
#define cn_int_line_EXTI3           (0x09)  //EXTI Line3 Interrupt
#define cn_int_line_EXTI4           (0x0A)  //EXTI Line4 Interrupt
#define cn_int_line_DMA1_Ch1        (0x0B)  //DMA1 Channel 1 global Interrupt
#define cn_int_line_DMA1_Ch2        (0x0C)  //DMA1 Channel 2 global Interrupt
#define cn_int_line_DMA1_Ch3        (0x0D)  //DMA1 Channel 3 global Interrupt
#define cn_int_line_DMA1_Ch4        (0x0E)  //DMA1 Channel 4 global Interrupt
#define cn_int_line_DMA1_Ch5        (0x0F)  //DMA1 Channel 5 global Interrupt
#define cn_int_line_DMA1_Ch6        (0x10)  //DMA1 Channel 6 global Interrupt
#define cn_int_line_DMA1_Ch7        (0x11)  //DMA1 Channel 7 global Interrupt
#define cn_int_line_ADC1_2          (0x12)  //ADC1 et ADC2 global Interrupt
#define cn_int_line_USB_HP_CAN_TX   (0x13)  //USB High Priority or CAN TX Interrupts
#define cn_int_line_USB_LP_CAN_RX0  (0x14)  //USB Low Priority or CAN RX0 Interrupts
#define cn_int_line_CAN_RX1         (0x15)  //CAN RX1 Interrupt
#define cn_int_line_CAN_SCE         (0x16)  //CAN SCE Interrupt
#define cn_int_line_EXTI9_5         (0x17)  //External Line[9:5] Interrupts
#define cn_int_line_TIM1_BRK        (0x18)  //TIM1 Break Interrupt
#define cn_int_line_TIM1_UP         (0x19)  //TIM1 Update Interrupt
#define cn_int_line_TIM1_TRG_COM    (0x1A)  //TIM1 Trigger and Commutation Interrupt
#define cn_int_line_TIM1_CC         (0x1B)  //TIM1 Capture Compare Interrupt
#define cn_int_line_TIM2            (0x1C)  //TIM2 global Interrupt
#define cn_int_line_TIM3            (0x1D)  //TIM3 global Interrupt
#define cn_int_line_TIM4            (0x1E)  //TIM4 global Interrupt
#define cn_int_line_I2C1_EV         (0x1F)  //I2C1 Event Interrupt
#define cn_int_line_I2C1_ER         (0x20)  //I2C1 Error Interrupt
#define cn_int_line_I2C2_EV         (0x21)  //I2C2 Event Interrupt
#define cn_int_line_I2C2_ER         (0x22)  //I2C2 Error Interrupt
#define cn_int_line_SPI1            (0x23)  //SPI1 global Interrupt
#define cn_int_line_SPI2            (0x24)  //SPI2 global Interrupt
#define cn_int_line_USART1          (0x25)  //USART1 global Interrupt
#define cn_int_line_USART2          (0x26)  //USART2 global Interrupt
#define cn_int_line_USART3          (0x27)  //USART3 global Interrupt
#define cn_int_line_EXTI15_10       (0x28)  //External Line[15:10] Interrupts
#define cn_int_line_RTCAlarm        (0x29)  //RTC Alarm through EXTI Line Interrupt
#define cn_int_line_USBWakeUp       (0x2A)  //USB WakeUp from suspend through EXTI Line Interrupt
#define cn_int_line_TIM8_BRK        (0x2B)  //TIM8 Break Interrupt
#define cn_int_line_TIM8_UP         (0x2C)  //TIM8 Update Interrupt
#define cn_int_line_TIM8_TRG_COM    (0x2D)  //TIM8 Trigger and Commutation Interrupt
#define cn_int_line_TIM8_CC         (0x2E)  //TIM8 Capture Compare Interrupt
#define cn_int_line_ADC3            (0x2F)  //ADC3 global Interrupt
#define cn_int_line_FSMC            (0x30)  //FSMC global Interrupt
#define cn_int_line_SDIO            (0x31)  //SDIO global Interrupt
#define cn_int_line_TIM5            (0x32)  //TIM5 global Interrupt
#define cn_int_line_SPI3            (0x33)  //SPI3 global Interrupt
#define cn_int_line_UART4           (0x34)  //UART4 global Interrupt
#define cn_int_line_UART5           (0x35)  //UART5 global Interrupt
#define cn_int_line_TIM6            (0x36)  //TIM6 global Interrupt
#define cn_int_line_TIM7            (0x37)  //TIM7 global Interrupt
#define cn_int_line_DMA2_Ch1        (0x38)  //DMA2 Channel 1 global Interrupt
#define cn_int_line_DMA2_Ch2        (0x39)  //DMA2 Channel 2 global Interrupt
#define cn_int_line_DMA2_Ch3        (0x3A)  //DMA2 Channel 3 global Interrupt
#define cn_int_line_DMA2_Ch4_5      (0x3B)  //DMA2 Channel 4 and DMA2 Channel 5 global Interrupt
#define cn_int_line_last            (0x3B)

#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

