//========================================================  
// �ļ�ģ��˵��:                                            
// omapl138���������Լ�������ƼĴ������塣                       
// �ļ��汾: v1.00                                          
// ������Ա: lst                                            
// ����ʱ��: 2011.06.01                                     
// Copyright(c) 2011-2011  ��������������޹�˾             
//========================================================  
// �����޸ļ�¼(���µķ�����ǰ��):                            
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>            
//========================================================  
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
#include "os_inc.h"
#include "cpu_peri.h"
tagGpioReg volatile * const g_ptGpioReg 
                            = (tagGpioReg *)0x01e26000;

tagSysconfig0Reg volatile * const g_ptSysconfig0Reg
                      = (tagSysconfig0Reg *)0x01c14000;
tagSysconfig1Reg volatile * const g_ptSysconfig1Reg
                      = (tagSysconfig1Reg *)0x01e2c000;
tagLpscReg volatile * const g_ptLpsc0Reg = (tagLpscReg *)0x01C10000;
tagLpscReg volatile * const g_ptLpsc1Reg = (tagLpscReg *)0x01E27000;

tagPllReg volatile * const g_ptPll0Reg = (tagPllReg*)0x01C11000;
tagPllReg volatile * const g_ptPll1Reg = (tagPllReg*)0x01E1A000;

tagDdriiReg volatile * const g_ptDdriiReg = (tagDdriiReg*)0xb0000000;

tagEmifaReg volatile * const g_ptEmifaReg = (tagEmifaReg*)0x68000000;

tagSpiReg volatile *const g_ptSpi0Reg = (tagSpiReg *)0x01C41000;
tagSpiReg volatile *const g_ptSpi1Reg = (tagSpiReg *)0x01F0E000;

tag_UartReg volatile *const g_ptUart0Reg = (tag_UartReg *)cn_uart0_base;
tag_UartReg volatile *const g_ptUart1Reg = (tag_UartReg *)cn_uart1_base;
tag_UartReg volatile *const g_ptUart2Reg = (tag_UartReg *)cn_uart2_base;

tagTimerReg volatile * const g_ptTimer0Reg = (tagTimerReg *)0x01c20000;
tagTimerReg volatile * const g_ptTimer1Reg = (tagTimerReg *)0x01c21000;
tagTimerReg volatile * const g_ptTimer2Reg = (tagTimerReg *)0x01f0c000;
tagTimerReg volatile * const g_ptTimer3Reg = (tagTimerReg *)0x01f0d000;

struct st_int_reg volatile * const pg_int_reg
                        = (struct st_int_reg *)0xfffee000;

tagLcdcReg volatile * const g_ptLcdcReg 
                        = (tagLcdcReg *)0x01E13000;


