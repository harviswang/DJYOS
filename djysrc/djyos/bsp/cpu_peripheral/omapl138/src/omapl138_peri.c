//========================================================  
// 文件模块说明:                                            
// omapl138公共外设以及外设控制寄存器定义。                       
// 文件版本: v1.00                                          
// 开发人员: lst                                            
// 创建时间: 2011.06.01                                     
// Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             
//========================================================  
// 程序修改记录(最新的放在最前面):                            
//  <版本号> <修改日期>, <修改人员>: <修改功能概述>            
//========================================================  
//例如：
// V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核
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


