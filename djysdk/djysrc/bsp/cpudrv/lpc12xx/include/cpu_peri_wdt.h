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





