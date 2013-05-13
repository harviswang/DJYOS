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

#define cn_int_line_eint0     (0)
#define cn_int_line_eint1     (1)
#define cn_int_line_eint2     (2)
#define cn_int_line_eint3     (3)
#define cn_int_line_eint4_7   (4)
#define cn_int_line_eint8_23  (5)
#define cn_int_line_resv6     (6)
#define cn_int_line_bat_flt   (7)
#define cn_int_line_tick      (8)
#define cn_int_line_wdt       (9)
#define cn_int_line_timer0    (10)
#define cn_int_line_timer1    (11)
#define cn_int_line_timer2    (12)
#define cn_int_line_timer3    (13)
#define cn_int_line_timer4    (14)
#define cn_int_line_uart2     (15)
#define cn_int_line_lcd       (16)
#define cn_int_line_dma0      (17)
#define cn_int_line_dma1      (18)
#define cn_int_line_dma2      (19)
#define cn_int_line_dma3      (20)
#define cn_int_line_sdi       (21)
#define cn_int_line_spi0      (22)
#define cn_int_line_uart1     (23)
#define cn_int_line_resv24    (24)
#define cn_int_line_usbd      (25)
#define cn_int_line_usbh      (26)
#define cn_int_line_iic       (27)
#define cn_int_line_uart0     (28)
#define cn_int_line_spi1      (29)
#define cn_int_line_rtc       (30)
#define cn_int_line_adc       (31)
#define cn_int_line_last      (31)

#define cn_int_msk_all_line   (0xffffffff)

#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

