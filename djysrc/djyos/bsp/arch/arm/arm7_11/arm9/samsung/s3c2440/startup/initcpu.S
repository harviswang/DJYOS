/* *********************** ********** ***********************
 *----------------------------------------------------
 *Copyright (C)  2005-2008   lst.
 *版权所有 (C)  2005-2008    lst.
 *所属模块:  CPU初始化
 *作者：     lst
 *版本：      V1.1.0
 *文件描述:  CPU初始化必须用汇编语言实现的部分
 *其他说明:  无
 *修订历史:
 *2. 日期: 2009-10-30
 *   作者: lst
 *   新版本号：V1.1.0
 *   修改说明: 把内存和时钟用C语言初始化
 *1. 日期: 2009-03-10
 *   作者: lst
 *   新版本号：V1.0.0
 *   修改说明: s3c2440的原始版本
 *------------------------------------------------------
*********************** ********** *********************** */
.cpu	arm920t
.fpu	softvfp
.file	"initcpu.S"

#include "arm_mode_s_file.h"

.section .text._start, "ax", %progbits
.align  2
.global _start
_start:        /* 复位向量地址 */
    mrs     r0, cpsr                  /* 取CPSR */
    bic     r0, r0, #SF_MODEMASK   /* 清模式位 */
    orr     r1, r0, #SF_SVCMODE|SF_IRQ_BIT|SF_FIQ_BIT    /* 设置为管理态，并禁止中断 */
    msr     cpsr_cxsf,r1              /* 切换到管理态,可防止意外返回0地址时出错. */

    ldr     r0,=WTCON           /* 禁止看门狗 */
    ldr     r1,=0x0
    str     r1,[r0]

    mrc     p15, 0, r0, c1, c0, 0
    orr     r0,r0,#0xc0000000   /* 2410特有的控制位，设置cpu工作在异步时钟 */
    bic     r0,r0,#0x1000       /* 禁止Icache */
    bic     r0,r0,#0x0001       /* 禁止mmu */
    bic     r0,r0,#0x0004       /* 禁止Dcache */
    mcr     p15, 0, r0, c1, c0, 0

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

	bl		init_cpu_clk
    bl      init_mem_bus
    bl		init_mmu

    bl      load_preload


.end

