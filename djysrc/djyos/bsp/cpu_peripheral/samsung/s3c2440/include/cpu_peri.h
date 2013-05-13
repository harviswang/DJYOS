//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
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
#ifndef __CPU_PERI_REG_H__
#define __CPU_PERI_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu_peri_gpio.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_lcddrv.h"
#include "cpu_peri_nand.h"
#include "cpu_peri_rtc.h"
#include "cpu_peri_sdi.h"
#include "cpu_peri_timer.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_wdt.h"

extern struct gpio_reg volatile * const pg_gpio_reg;
extern struct lcd_reg volatile * const pg_lcd_reg;
extern struct uart_reg volatile * const pg_uart0_reg;
extern struct uart_reg volatile * const pg_uart1_reg;
extern struct uart_reg volatile * const pg_uart2_reg;
extern struct timer_reg volatile * const pg_timer_reg;
extern struct wdt_reg volatile * const pg_wdt_reg;
extern volatile struct nand_reg *const pg_nand_reg;
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_REG_H__

