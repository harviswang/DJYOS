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
#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu_peri_int_line.h"
#include "omapl138_cpucfg.h"
#include "omapl138_gpio.h"
#include "omapl138_spi.h"
#include "omapl138_sysctrl.h"
#include "omapl138_timer.h"
#include "omapl138_uart.h"
#include "omapl138_lcdc.h"
#include "omapl138_dcore.h"

extern tagGpioReg volatile * const g_ptGpioReg;
extern tagSysconfig0Reg volatile * const g_ptSysconfig0Reg;
extern tagSysconfig1Reg volatile * const g_ptSysconfig1Reg;
extern tagLpscReg volatile * const g_ptLpsc0Reg;
extern tagLpscReg volatile * const g_ptLpsc1Reg;
extern tagPllReg volatile * const g_ptPll0Reg;
extern tagPllReg volatile * const g_ptPll1Reg;
extern tagDdriiReg volatile * const g_ptDdriiReg;
extern tagEmifaReg volatile * const g_ptEmifaReg;
extern tagSpiReg volatile *const g_ptSpi0Reg;
extern tagSpiReg volatile *const g_ptSpi1Reg;
extern tagTimerReg volatile * const g_ptTimer0Reg;
extern tagTimerReg volatile * const g_ptTimer1Reg;
extern tagTimerReg volatile * const g_ptTimer2Reg;
extern tagTimerReg volatile * const g_ptTimer3Reg;
extern struct st_int_reg volatile * const pg_int_reg;
extern tagLcdcReg volatile * const g_ptLcdcReg;
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__

