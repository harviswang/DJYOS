//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: CPU外设定义
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 包含CPU所有外设的定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

struct gpio_reg volatile * const pg_gpio_reg
                        = (struct gpio_reg *)0x56000000;
struct lcd_reg volatile * const pg_lcd_reg
                        = (struct lcd_reg *)0x4d000000;
struct uart_reg volatile *const pg_uart0_reg
                        = (struct uart_reg *)0x50000000;
struct uart_reg volatile *const pg_uart1_reg
                        = (struct uart_reg *)0x50004000;
struct uart_reg volatile *const pg_uart2_reg
                        = (struct uart_reg *)0x50008000;
struct timer_reg volatile * const pg_timer_reg
                        = (struct timer_reg *)0x51000000;
struct wdt_reg volatile * const pg_wdt_reg
                        = (struct wdt_reg *)0x53000000;
struct nand_reg volatile *const pg_nand_reg = (void*)0x4e000000;
