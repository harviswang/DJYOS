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
#include "os_inc.h"
#include "cpu_peri.h"

struct st_gpio_reg volatile * const pg_gpio_reg [4] = {
                               (struct st_gpio_reg *)0x50000000,
                               (struct st_gpio_reg *)0x50010000,
                               (struct st_gpio_reg *)0x50020000,
                               (struct st_gpio_reg *)0x50030000,
                               };
struct st_sysctrl_reg volatile * const pg_sysctrl_reg  = 
                               (struct st_sysctrl_reg *)0x40048000;
struct st_nxp_uart_reg volatile * const pg_uart_reg0 = 
                               (struct st_nxp_uart_reg *)0x40008000;
struct st_nxp_uart_reg volatile * const pg_uart_reg1 = 
                               (struct st_nxp_uart_reg *)0x4000c000;
struct st_wdt_reg volatile * const pg_wdt_reg  = 
                               (struct st_wdt_reg *)0x40004000;

