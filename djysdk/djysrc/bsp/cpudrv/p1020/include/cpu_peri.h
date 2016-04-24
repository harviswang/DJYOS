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
#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "board-config.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_dma.h"
#include "cpu_peri_pci.h"
#include "cpu_peri_tsec.h"
#include "cpu_peri_serdes.h"
#include "SysSpace.h"

#define cn_gpio_baddr                (CN_CCSR_NEWADDR_V + 0xf000)
#define cn_uart_baddr                (CN_CCSR_NEWADDR_V + 0x4500)
#define cn_i2c1_baddr                (CN_CCSR_NEWADDR_V + 0x3000)
#define cn_i2c2_baddr                (CN_CCSR_NEWADDR_V + 0x3100)
#define cn_spi_baddr                 (CN_CCSR_NEWADDR_V + 0x7000)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__

