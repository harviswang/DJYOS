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
#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "cpu_peri_sysctrl.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_wdt.h"
#include "cpu_peri_int_line.h"
extern struct st_gpio_reg volatile * const pg_gpio_reg [4];
extern struct st_sysctrl_reg volatile * const pg_sysctrl_reg;
extern struct st_nxp_uart_reg volatile * const pg_uart_reg0;
extern struct st_nxp_uart_reg volatile * const pg_uart_reg1;
extern struct st_wdt_reg volatile * const pg_wdt_reg;
#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_H__*/


