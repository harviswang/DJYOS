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
#include "cortexm3.h"
#include "cpu_peri.h"
#include "exception.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);

extern void SRAM_Init(void);
extern void load_preload(void);

struct scb_reg volatile * const pg_scb_reg  = (struct scb_reg *)0xe000ed00;

void cpu_init(void)
{

	__set_PSP((uint32_t)msp_top);
	__set_PRIMASK(1);
	__set_FAULTMASK(1);
	__set_CONTROL(0);
	switch(pg_scb_reg->CPUID)
    {
        case cn_revision_r0p0:
        	break;    //市场没有版本0的芯片
        case cn_revision_r1p0:
            pg_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case cn_revision_r1p1:
            pg_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case cn_revision_r2p0:break;    //好像没什么要做的
    }

    pg_inflash_fpec_reg->ACR &= ~(u32)0x1f;
    pg_inflash_fpec_reg->ACR |= (cn_mclk-1)/24000000;   //设置等待周期。
    pg_inflash_fpec_reg->ACR |= 0x10;       //开启预取


    if(((pg_rcc_reg->CR & cn_cr_check_mask) != cn_cr_check)
                || ((pg_rcc_reg->CFGR & cn_cfgr_check_mask) != cn_cfgr_check))
    {
        //开始初始化时钟
        //step1:复位时钟控制寄存器
        pg_rcc_reg->CR |= (uint32_t)0x00000001;
        // 复位 SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] MCO[2:0] 位
        pg_rcc_reg->CFGR &= (uint32_t)0xF8FF0000;
        // 复位 HSEON, CSSON and PLLON 位
        pg_rcc_reg->CR &= (uint32_t)0xFEF6FFFF;
        // 复位 HSEBYP 位
        pg_rcc_reg->CR &= (uint32_t)0xFFFBFFFF;
        // 复位 PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 位
        pg_rcc_reg->CFGR &= (uint32_t)0xFF80FFFF;
        // 禁止所有中断
        pg_rcc_reg->CIR = 0x00000000;

        //step2:设置各时钟控制位以及倍频、分频值
        pg_rcc_reg->CFGR = cn_cfgr_set+(7<<24);   // set clock configuration register
        pg_rcc_reg->CR   = cn_cr_set;     // set clock control register

        while(bb_rcc_cr_hserdy ==0);
        while(bb_rcc_cr_pllrdy ==0);
    }
    SRAM_Init();

    load_preload();
}

