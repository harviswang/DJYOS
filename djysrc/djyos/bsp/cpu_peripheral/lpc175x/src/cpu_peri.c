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

//�ܽ���������Ĵ���
struct st_pin_config_reg volatile * const pg_pin_config_reg
                            = (struct st_pin_config_reg *)0x4002c000;

//gpio��ؼĴ���
struct st_gpio_reg volatile * const pg_gpio0_reg
                            = (struct st_gpio_reg *)0x2009c000;
struct st_gpio_reg volatile * const pg_gpio1_reg
                            = (struct st_gpio_reg *)0x2009c020;
struct st_gpio_reg volatile * const pg_gpio2_reg
                            = (struct st_gpio_reg *)0x2009c040;
struct st_gpio_reg volatile * const pg_gpio4_reg
                            = (struct st_gpio_reg *)0x2009c080;
//ϵͳ���ƼĴ���
struct st_sysctrl_reg volatile * const pg_sysctrl_reg
                            = (struct st_sysctrl_reg *)0x400fc000;
//���Ź��Ĵ���
struct st_wdt_reg volatile * const pg_wdt_reg
                            = (struct st_wdt_reg *)0x40000000;
//���ڼĴ���
struct st_nxp_uart_reg volatile * const pg_uart0_reg
                        = (struct st_nxp_uart_reg *)0x4000c000;
struct st_nxp_uart_reg volatile * const pg_uart1_reg
                        = (struct st_nxp_uart_reg *)0x40010000;
struct st_nxp_uart_reg volatile * const pg_uart2_reg
                        = (struct st_nxp_uart_reg *)0x40098000;
struct st_nxp_uart_reg volatile * const pg_uart3_reg
                        = (struct st_nxp_uart_reg *)0x4009c000;

