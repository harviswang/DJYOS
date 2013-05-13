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

#include "cpu_peri_int_line.h"
#include "omapl138_cpucfg.h"
#include "omapl138_gpio.h"
#include "omapl138_spi.h"
#include "omapl138_sysctrl.h"
#include "omapl138_timer.h"
#include "omapl138_uart.h"
#include "omapl138_lcdc.h"
#include "omapl138_dcore.h"

extern tagGpioReg volatile * const g_ptGpioReg;
extern tagSysconfig0Reg volatile * const g_ptSysconfig0Reg;
extern tagSysconfig1Reg volatile * const g_ptSysconfig1Reg;
extern tagLpscReg volatile * const g_ptLpsc0Reg;
extern tagLpscReg volatile * const g_ptLpsc1Reg;
extern tagPllReg volatile * const g_ptPll0Reg;
extern tagPllReg volatile * const g_ptPll1Reg;
extern tagDdriiReg volatile * const g_ptDdriiReg;
extern tagEmifaReg volatile * const g_ptEmifaReg;
extern tagSpiReg volatile *const g_ptSpi0Reg;
extern tagSpiReg volatile *const g_ptSpi1Reg;
extern tagTimerReg volatile * const g_ptTimer0Reg;
extern tagTimerReg volatile * const g_ptTimer1Reg;
extern tagTimerReg volatile * const g_ptTimer2Reg;
extern tagTimerReg volatile * const g_ptTimer3Reg;
extern struct st_int_reg volatile * const pg_int_reg;
extern tagLcdcReg volatile * const g_ptLcdcReg;
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__

