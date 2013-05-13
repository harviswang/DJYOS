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

#include "cpu_peri_dma.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_inflash.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_rcc.h"
#include "cpu_peri_rtc.h"

extern struct st_dma_reg volatile * const pg_dma1_reg;
extern struct st_dma_reg volatile * const pg_dma2_reg;

//dma1�ĸ�ͨ���Ĵ���
extern struct st_dma_channel_reg volatile * const pg_dma1_channel1_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel2_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel3_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel4_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel5_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel6_reg;
extern struct st_dma_channel_reg volatile * const pg_dma1_channel7_reg;
//dma2�ĸ�ͨ���Ĵ���
extern struct st_dma_channel_reg volatile * const pg_dma2_channel1_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel2_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel3_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel4_reg;
extern struct st_dma_channel_reg volatile * const pg_dma2_channel5_reg;

//gpio��ؼĴ�������
extern struct st_gpio_reg volatile * const pg_gpio_rega;
extern struct st_gpio_reg volatile * const pg_gpio_regb;
extern struct st_gpio_reg volatile * const pg_gpio_regc;
extern struct st_gpio_reg volatile * const pg_gpio_regd;
extern struct st_gpio_reg volatile * const pg_gpio_rege;
extern struct st_gpio_reg volatile * const pg_gpio_regf;
extern struct st_gpio_reg volatile * const pg_gpio_regg;
extern struct st_gpio_reg volatile * const pg_gpio_reg[];
extern struct st_afio_reg volatile * const pg_afio_reg;

//������ؼĴ���
extern struct st_usart_reg volatile * const pg_uart1_reg;
extern struct st_usart_reg volatile * const pg_uart2_reg;
extern struct st_usart_reg volatile * const pg_uart3_reg;
extern struct st_usart_reg volatile * const pg_uart4_reg;
extern struct st_usart_reg volatile * const pg_uart5_reg;

//rcc(��λ��ʱ�ӿ��ƼĴ���)
extern struct st_rcc_reg volatile * const pg_rcc_reg;

//Ƭ��flash���ƼĴ���
extern struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg;  
extern struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg; 

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_REG_H__

