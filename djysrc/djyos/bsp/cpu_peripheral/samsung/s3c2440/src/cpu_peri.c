//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: CPU���趨��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����CPU��������Ķ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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
