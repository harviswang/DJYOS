//----------------------------------------------------
//Copyright (C), 2004-2009,  �����߲���ϵͳ�з��Ŷ�.
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�з��Ŷ�.
//����ģ��: CPU���趨��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����CPU��������Ķ��壬
//      1���������ں���صĹ̶����豸������m3��nvic�ȣ�����m3�ں˾�һ�¡�
//      2���������ں���صĿ�ѡ�䵫�̶��Ķ��壬����m3��mpu��������һ������m3�ں�
//         ��оƬ���У���ֻ�����޵����𣬲����ڱ仯��
//         ��������������Ӧ���ں�Ŀ¼�¶��塣
//      3�������ں���صĿɱ䲿������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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





