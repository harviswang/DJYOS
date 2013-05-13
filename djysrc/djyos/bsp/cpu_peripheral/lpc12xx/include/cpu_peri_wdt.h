//----------------------------------------------------
//Copyright (C), 2004-2009,  都江堰操作系统研发团队.
//版权所有 (C), 2004-2009,   都江堰操作系统研发团队.
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
#ifndef __LPC12XX_WDT_H__
#define __LPC12XX_WDT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup WDT_Public_Macros
 * @{
 */ 


/** @defgroup WDT_Clock_Selection 
  * @{
  */
typedef uint8_t WDT_CLKSEL_OPT; 
#define WDT_CLKSEL_IRCOSC  ((uint8_t) 0x00)   /* Clock source:iRC osc.(default)*/
#define WDT_CLKSEL_WDOSC   ((uint8_t) 0x01)   /* Clock source:watchdog osc.  */

#define PARAM_WDT_CLKSEL_OPT(OPTION)   ((OPTION ==WDT_CLKSEL_IRCOSC)||\
                                       (OPTION ==WDT_CLKSEL_WDOSC))
/**
 * @}
 */

/** @defgroup WDT_Mode 
  * @{
  */
typedef uint8_t WDT_MODE_OPT; 
#define WDT_MODE_INT_ONLY  ((uint8_t) 0x00)  /* generate interrupt only */
#define WDT_MODE_RESET     ((uint8_t) 0x01)  /* generate interrupt & reset MCU*/

#define PARAM_WDT_MODE_OPT(OPTION)     ((OPTION ==WDT_MODE_INT_ONLY)||\
                                       (OPTION ==WDT_MODE_RESET))
/**
 * @}
 */


/** @defgroup WDT_TimeOut_Option 
  * @{
  */
typedef uint8_t  WDT_TIMEOUT_OPT;

#define WDT_TIMEOUT_TICKVAL  ((uint8_t) 0x00)  /* timeout in absolute value */
#define WDT_TIMEOUT_USVAL    ((uint8_t) 0x01)  /* timeout in microsecond value*/

#define PARAM_WDT_TIMEOUT_OPT(OPT)	(( OPT == WDT_TIMEOUT_TICKVAL)|| \
					( OPT == WDT_TIMEOUT_USVAL))

/**
 * @}
 */

/* Define divider index for microsecond ( us ) */
#define WDT_US_INDEX	((u32)(1000000))
/* WDT Time out minimum value */
#define WDT_TIMEOUT_MIN	((u32)(0xFF))
/* WDT Time out maximum value */
#define WDT_TIMEOUT_MAX	((u32)(0x00FFFFFF))
/* WDT constant register default value */
#define WDT_WDTC_DEFAULTVALUE  ((u32)(0x00FFFF))


struct st_wdt_reg
{
    vu32 MOD;                    /*!< Offset: 0x000 Watchdog mode register (R/W) */
    vu32 TC;                     /*!< Offset: 0x004 Watchdog timer constant register (R/W) */
    vu32 FEED;                   /*!< Offset: 0x008 Watchdog feed sequence register ( /W) */
    vu32 TV;                     /*!< Offset: 0x00C Watchdog timer value register (R/ ) */
    vu32 WDCLKSEL;               /*!< Offset: 0x010 Watchdog clock source selectuin register (R/W ) */ 
    vu32 WARNINT;                /*!< Offset: 0x014 Watchdog Warning Interrupt compare value register (R/W ) */
    vu32 WINDOW;                 /*!< Offset: 0x018 Watchdog Window compare value register (R/W ) */  
} ;


#ifdef __cplusplus
}
#endif

#endif /*__LPC12XX_WDT_H__*/





