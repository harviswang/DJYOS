//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: CPU外设定义
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 包含CPU所有外设的定义，
//      1、不包含内核相关的固定的设备，比如m3的nvic等，所有m3内核均一致。
//      2、不包含内核相关的可选配但固定的定义，比如m3的mpu部件，不一定所有m3内核
//         的芯片都有，但只有有无的区别，不存在变化。
//         以上两部分在相应的内核目录下定义。
//      3、包含内核相关的可变部件定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CPU_PERI_REG_H__
#define __CPU_PERI_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu_peri_dma.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_inflash.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_rcc.h"
#include "cpu_peri_rtc.h"

extern struct st_dma_reg volatile * const pg_dma1_reg;
extern struct st_dma_reg volatile * const pg_dma2_reg;

//dma1的各通道寄存器
extern struct st_dma_channel_reg volatile * const pg_dma1_channel1_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel2_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel3_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel4_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel5_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel6_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel7_reg;
//dma2的各通道寄存器
extern struct st_dma_channel_reg volatile * const pg_dma2_channel1_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel2_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel3_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel4_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel5_reg;

//gpio相关寄存器定义
extern struct st_gpio_reg volatile * const pg_gpio_rega;
extern struct st_gpio_reg volatile * const pg_gpio_regb;
extern struct st_gpio_reg volatile * const pg_gpio_regc;
extern struct st_gpio_reg volatile * const pg_gpio_regd;
extern struct st_gpio_reg volatile * const pg_gpio_rege;
extern struct st_gpio_reg volatile * const pg_gpio_regf;
extern struct st_gpio_reg volatile * const pg_gpio_regg;
extern struct st_gpio_reg volatile * const pg_gpio_reg[];
extern struct st_afio_reg volatile * const pg_afio_reg;

//串口相关寄存器
extern struct st_usart_reg volatile * const pg_uart1_reg;
extern struct st_usart_reg volatile * const pg_uart2_reg;
extern struct st_usart_reg volatile * const pg_uart3_reg;
extern struct st_usart_reg volatile * const pg_uart4_reg;
extern struct st_usart_reg volatile * const pg_uart5_reg;

//rcc(复位和时钟控制寄存器)
extern struct st_rcc_reg volatile * const pg_rcc_reg;

//片内flash控制寄存器
extern struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg;  
extern struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg; 

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_REG_H__

