//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//版权所有 (C), 2005-2009,   lst.
//所属模块:  CPU初始化
//作者：     lst
//版本：      V1.0.0
//初始版本完成日期：2009-08-05
//文件描述:  cpu时钟初始化
//其他说明:  无
//修订历史:
//2. ...
//1. 日期: 2009-08-05
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cortexm0.h"
#include "cmsis_cm0.h"
#include "cpu_peri.h"

#define SYSOSCCTRL_Val      0x00000000      //bit0:0=晶体，1=晶振
                                            //bit1:0=1~20M,1=15~25M
#define SYSPLLCLKSEL_Val    0x00000001      //选pll时钟，0=rc,1=晶振，2=wdtclk

#define cn_SYSPLLCTRL_MSEL  3               //pll除数=MSEL+1
#define cn_SYSPLLCTRL_PSEL  2               //pll乘数
//主频配置为40M，配置公式较复杂，参看nxp的手册
#define SYSPLLCTRL_Val      ((cn_SYSPLLCTRL_PSEL<<5) + cn_SYSPLLCTRL_MSEL)

//选主时钟，0=rc，1=pll输入，2=wdt振荡器，3=pll输出
#define MAINCLKSEL_Val        0x00000003    

//AHB总线时钟除数，1~255,0=禁止总线时钟
#define SYSAHBCLKDIV_Val      0x00000001

#define CLKOUTCLKSEL_Val      0x00000003    //clkout选择主时钟
#define CLKOUTCLKDIV_Val      0x00000001    //clkout时钟除法
extern   uint32_t   msp_top[ ];

void cpu_init (void)
{
    u32 i;

	__set_PSP(msp_top);
	__set_MSP(msp_top);
	__set_PRIMASK(1);
	__set_CONTROL(0);

    pg_sysctrl_reg->SYSMEMREMAP = 0x2;    // remap to internal FLASH 
	pg_sysctrl_reg->PRESETCTRL &= ~(1<<15);	//ADD BY HUANGCW  设置flash为多周期读取。周期数配置FLASHCFG寄存器

#if 1
//    First, below lines are for debugging only. For future release, WDT is 
//    enabled by bootrom, thus, unless a feed to WDT continuously, or WDT timeout 
//    will occur. If it's happen, WDT interrupt will be pending until a INT_CLEAR
//    is applied. Below logic is to prevent system from going to the WDT interrupt
//    during debugging. 
//    Second, all the peripheral clocks seem to be enabled by bootrom, it's
//    not consistent with the UM. In below lines, only SYS, ROM, RAM, FLASHREG,
//    FLASHARRAY, and I2C are enabled per UM dated July 14th. 
    pg_wdt_reg->MOD = 0x00;  
    pg_wdt_reg->FEED = 0xAA;		// Feeding sequence 
    pg_wdt_reg->FEED = 0x55;

    pg_nvic_reg->clrpend |= 0xFFFFFFFF; 
    pg_sysctrl_reg->SYSAHBCLKCTRL = 0x00000001F; 
#endif 

    // bit 0 default is crystal bypass, 
    //bit1 0=0~20Mhz crystal input, 1=15~50Mhz crystal input. 
    pg_sysctrl_reg->SYSOSCCTRL = 0x00;
    // main system OSC run is cleared, bit 5 in PDRUNCFG register 
    pg_sysctrl_reg->PDRUNCFG     &= ~(1 << 5);          // Power-up System Osc      
    pg_sysctrl_reg->SYSOSCCTRL    = SYSOSCCTRL_Val;
    // Wait 200us for OSC to be stablized, 此时用的是rc振荡，200us是可信的. 
    for (i = 0; i < 400; i++) ;

    pg_sysctrl_reg->SYSPLLCLKSEL  = SYSPLLCLKSEL_Val;   // Select PLL Input         
    pg_sysctrl_reg->SYSPLLCLKUEN  = 0x01;               // Update Clock Source      
    pg_sysctrl_reg->SYSPLLCLKUEN  = 0x00;               // Toggle Update Register   
    pg_sysctrl_reg->SYSPLLCLKUEN  = 0x01;
    while (!(pg_sysctrl_reg->SYSPLLCLKUEN & 0x01));     // Wait Until Updated       
                                // System PLL Setup         
    pg_sysctrl_reg->SYSPLLCTRL    = SYSPLLCTRL_Val;
    pg_sysctrl_reg->PDRUNCFG     &= ~(1 << 7);          // Power-up SYSPLL          
    while (!(pg_sysctrl_reg->SYSPLLSTAT & 0x01));	      // Wait Until PLL Locked    

    pg_sysctrl_reg->MAINCLKSEL    = MAINCLKSEL_Val;     // Select PLL Clock Output  
    pg_sysctrl_reg->MAINCLKUEN    = 0x01;               // Update MCLK Clock Source 
    pg_sysctrl_reg->MAINCLKUEN    = 0x00;               // Toggle Update Register   
    pg_sysctrl_reg->MAINCLKUEN    = 0x01;
    while (!(pg_sysctrl_reg->MAINCLKUEN & 0x01));       // Wait Until Updated       

    pg_sysctrl_reg->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;

    pg_sysctrl_reg->CLKOUTCLKSEL = CLKOUTCLKSEL_Val;
    pg_sysctrl_reg->CLKOUTDIV = CLKOUTCLKDIV_Val;

    // System clock to the IOCON needs to be enabled or
    //most of the I/O related peripherals won't work. 
    pg_sysctrl_reg->SYSAHBCLKCTRL |= (1<<16);

    pre_start();
}

