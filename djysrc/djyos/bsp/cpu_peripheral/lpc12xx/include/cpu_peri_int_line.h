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

#define cn_int_line_wakeup0         0x00  //wakeup from gpio0.0 
#define cn_int_line_wakeup1         0x01  //wakeup from gpio0.1 
#define cn_int_line_wakeup2         0x02  //wakeup from gpio0.2 
#define cn_int_line_wakeup3         0x03  //wakeup from gpio0.3 
#define cn_int_line_wakeup4         0x04  //wakeup from gpio0.4 
#define cn_int_line_wakeup5         0x05  //wakeup from gpio0.5 
#define cn_int_line_wakeup6         0x06  //wakeup from gpio0.6 
#define cn_int_line_wakeup7         0x07  //wakeup from gpio0.7 
#define cn_int_line_wakeup8         0x08  //wakeup from gpio0.8 
#define cn_int_line_wakeup9         0x09  //wakeup from gpio0.9 
#define cn_int_line_wakeup10        0x0A  //wakeup from gpio0.10
#define cn_int_line_wakeup11        0x0B  //wakeup from gpio0.11
#define cn_int_line_iic             0x0C
#define cn_int_line_timer16_0       0x0D
#define cn_int_line_timer16_1       0x0E
#define cn_int_line_timer32_0       0x0F
#define cn_int_line_timer32_1       0x10
#define cn_int_line_ssp             0x11
#define cn_int_line_uart0           0x12
#define cn_int_line_uart1           0x13
#define cn_int_line_comparator      0x14
#define cn_int_line_adc             0x15
#define cn_int_line_wdt             0x16
#define cn_int_line_bod             0x17
#define cn_int_line_fmc             0x18
#define cn_int_line_PIOINT0         0x19
#define cn_int_line_PIOINT1         0x1A
#define cn_int_line_PIOINT2         0x1B
#define cn_int_line_pmu             0x1C
#define cn_int_line_dma             0x1D
#define cn_int_line_rtc             0x1E
#define cn_int_line_edm             0x1F
#define cn_int_line_last            0x1F

#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

