 @----------------------------------------------------
 @Copyright (C)  2005-2008   lst.
 @版权所有 (C)  2005-2008    lst.
 @所属模块:  CPU初始化
 @作者：     lst
 @版本：      V1.1.0
 @文件描述:  CPU初始化必须用汇编语言实现的部分
 @其他说明:  无
 @修订历史:
 @2. 日期: 2009-10-30
 @   作者: lst
 @   新版本号：V1.1.0
 @   修改说明: 把内存和时钟用C语言初始化
 @1. 日期: 2009-03-10
 @   作者: lst
 @   新版本号：V1.0.0
 @   修改说明: s3c2440的原始版本
 @------------------------------------------------------
.cpu	arm926ej-s
.fpu	softvfp
.file	"initcpu.S"

#include "arm_mode_s_file.h"

.section .text._start, "ax", %progbits
.align  2
.global _start

.long 0x11
_start:        /* 复位向量地址 */
	ldr		r0, =0x68000010		@cs2(flash)配置地址
	ldr		r1, =0x00a0050d		@cs2设置为13个访问周期，默认值是88个周期，启动太慢
	str		r1, [r0]
	
    mrs     r0, cpsr                  /* 取CPSR */
    bic     r0, r0, #SF_MODEMASK   /* 清模式位 */
    orr     r1, r0, #SF_SVCMODE|SF_IRQ_BIT|SF_FIQ_BIT    /* 设置为管理态，并禁止中断 */
    msr     cpsr_cxsf,r1              /* 切换到管理态,可防止意外返回0地址时出错. */
	LDR     r0, =0xffff2000
	mov		sp, r0

    mrc     p15,0,r0,c1,c0,0
    orr     r0,r0,#0x00002000       @使能高端向量
    bic     r0,r0,#0x1000           @禁止Icache
    bic     r0,r0,#0x1              @禁止mmu
    bic     r0,r0,#0x4              @禁止Dcache
    mcr     p15,0,r0,c1,c0,0

#ifdef release
	BL	    config_clocks
	BL	    config_ddr
#endif
	
/* 初始化栈指针 */
    mrs     r0, cpsr
    bic     r0, r0, #SF_MODEMASK

    orr		r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT

    orr     r1, r0, #SF_UNDEFMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =undef_top

    orr     r1, r0, #SF_ABORTMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =abort_top

    orr     r1, r0, #SF_IRQMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =irq_top

    orr     r1, r0, #SF_FIQMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =fiq_top

    orr     r1, r0, #SF_SVCMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =svc_top

    orr     r1, r0, #SF_SYSMODE
    msr     cpsr_cxsf, r1
	
	LDR     r0, =0xffff2000
	mov		sp, r0
    bl		init_mmu
    ldr     sp, =usr_top

    bl      load_preload
    
@----热启动-----------------------------------------------------------------
@功能: 热启动地址，用于DSP给ARM软复位
@参数: 无
@返回: 不返回
@函数原型:void __asm_hot_restart(void );
@-----------------------------------------------------------------------------
.section .text.__asm_hot_restart, "ax", %progbits
.global __asm_hot_restart
__asm_hot_restart:
    mrs     r0, cpsr                  /* 取CPSR */
    bic     r0, r0, #SF_MODEMASK   /* 清模式位 */
    orr     r1, r0, #SF_SVCMODE|SF_IRQ_BIT|SF_FIQ_BIT    /* 设置为管理态，并禁止中断 */
    msr     cpsr_cxsf,r1              /* 切换到管理态,可防止意外返回0地址时出错. */
	LDR     r0, =0xffff2000
	mov		sp, r0

/* 初始化栈指针 */
    mrs     r0, cpsr
    bic     r0, r0, #SF_MODEMASK

    orr		r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT

    orr     r1, r0, #SF_UNDEFMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =undef_top

    orr     r1, r0, #SF_ABORTMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =abort_top

    orr     r1, r0, #SF_IRQMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =irq_top

    orr     r1, r0, #SF_FIQMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =fiq_top

    orr     r1, r0, #SF_SVCMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =svc_top

    orr     r1, r0, #SF_SYSMODE
    msr     cpsr_cxsf, r1
    ldr     sp, =usr_top
	
    bl      load_preload

@ end of __asm_hot_restart

.end
