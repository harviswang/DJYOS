/* *********************** ********** ***********************
 *----------------------------------------------------
 *Copyright (C), 2005-2008,  lst.
 *版权所有 (C), 2005-2008,   lst.
 *所属模块:  中断模块
 *作者：     lst
 *版本：      V1.0.1
 *文件描述:  中断模块中的汇编部分
 *其他说明:  无
 *修订历史:
 *3. 日期:2010-3-06
 *   作者:lst
 *   新版本号：1.0.2
 *   修改说明:     stmfd   sp!,{r0-r3,lr}         保存lr_svc,
 *           改为  stmfd   sp!,{r0,lr}             保存lr_svc,
 *2. 日期:2010-01-04
 *   作者:lst
 *   新版本号：1.0.1
 *   修改说明: 从gcc移植到keil，并无修改代码
 *1. 日期:2009-03-10
 *   作者:lst
 *   新版本号：1.0.0
 *   修改说明: 原始版本
 *------------------------------------------------------
*********************** ********** *********************** */
.cpu	arm920t
.fpu	softvfp
.file	"ints.S"

/* *********************** ********** ***********************
 *在2440中谨慎使用fiq中断,除非你只有一个中断被设置成fiq,否则,没有任何直接的信息
 *会告诉你正在响应哪个中断.FIQ模式中断号寄存器，你只能通过F_ISPR来确认中断源，
 *ARM9没有直接返回前导0个数的指令，只能用循环移位的方法。确定中断源所需的时间开
 *销很大，故2440中FIQ变得毫无意义，因此djyos在2440版本并不使用FIQ中断.
*********************** ********** *********************** */

#include "arm_mode_s_file.h"
.equ    INTOFFSET, 0x4a000014

/* cpsr的IF位清0，允许全部中断 */
.section .text.int_enable_arm_int, "ax", %progbits
.align 2
.global int_enable_arm_int
int_enable_arm_int:
	mrs r0, cpsr
	bic r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT
	msr cpsr_cxsf, r0
	bx	lr

/* cpsr的IF位置1，禁止全部中断 */
.section .text.int_disable_arm_int, "ax", %progbits
.align 2
.global int_disable_arm_int
int_disable_arm_int:
	mrs  r0, cpsr
	orr  r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT
	msr  cpsr_cxsf, r0
	bx	lr

/* cpsr的I位置1，禁止irq中断 */
.section .text.__int_disable_irq, "ax", %progbits
.align 2
.global __int_disable_irq
__int_disable_irq:
	mrs  r0, cpsr
	orr  r0, r0, #SF_IRQ_BIT
	msr  cpsr_cxsf, r0
	bx	lr

/* cpsr的I位清0，允许irq中断 */
.section .text.__int_enable_irq, "ax", %progbits
.align 2
.global __int_enable_irq
__int_enable_irq:
	mrs  r0, cpsr
	bic  r0, r0, #SF_IRQ_BIT
	msr  cpsr_cxsf, r0
	bx	lr

/* cpsr的F位置1，禁止fiq中断 */
.section .text.__int_disable_fiq, "ax", %progbits
.align 2
.global __int_disable_fiq
__int_disable_fiq:
	mrs r0,cpsr
	orr r0,r0,#SF_FIQ_BIT
	msr cpsr_cxsf,r0
	bx	lr

/* cpsr的F位清0，允许fiq中断 */
.section .text.__int_enable_fiq, "ax", %progbits
.align 2
.global __int_enable_fiq
__int_enable_fiq:
	mrs r0, cpsr
	bic r0, r0, #SF_FIQ_BIT
	msr cpsr_cxsf, r0
	bx	lr

/* *********************** ********** ***********************
//----进入高级原子操作---------------------------------------------------------
//功能：读当前总中断状态，然后禁止总中断。高级原子操作是指期间不容许任何原因
//      打断的操作。
//      int_high_atom_start--int_high_atom_end必须配套使用，绝对不能嵌套，在可能
//      嵌套的地方，必须使用int_save_trunk来关闭中断(调度)。
//      这两函数可视作"int_restore_trunk--int_save_trunk"的快速版本，除本身不能
//      嵌套调用外，在被他们套住的代码块内，也不允许调用int_restore_trunk和
//      int_save_trunk中的任何一个。
//参数：无
//返回：原子状态
//备注：移植敏感
//-----------------------------------------------------------------------------
*********************** ********** *********************** *
.section .text.int_high_atom_start, "ax", %progbits
.global int_high_atom_start
int_high_atom_start:
	mrs	r0, cpsr
	mov r1, r0
	orr	r1, r1, #SF_IRQ_BIT|SF_FIQ_BIT
	msr	cpsr_c, r1

* *********************** ********** ***********************
//----离开高级原子操作---------------------------------------------------------
//功能：恢复进入前总中断状态，本函数应与int_high_atom_start函数配套
//      int_high_atom_start--int_high_atom_end必须配套使用，绝对不能嵌套，在可能
//      嵌套的地方，必须使用int_save_trunk来关闭中断(调度)。
//      这两函数可视作"int_restore_trunk--int_save_trunk"的快速版本，除本身不能
//      嵌套调用外，在被他们套住的代码块内，也不允许调用int_restore_trunk和
//      int_save_trunk中的任何一个。
//参数：high，int_high_atom_start保存的原子状态
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
*********************** ********** *********************** *
.section .text.int_high_atom_end, "ax", %progbits
.global int_high_atom_end
int_high_atom_end:
	msr cpsr_c, r0
*/

/* *********************** ********** ***********************
 *中断相关的栈安排：
 *1、irq_stack放被中断的上下文，顺序:lr，r12-r0，cpsr，共14字
 *2、svc_stack运行中断服务函数，包括用户编写的中断服务函数。
 *3、SYS_stack，除非要在中断里切换上下文，否则无关
*********************** ********** *********************** */
.section .text.start_int, "ax", %progbits
.global start_int
start_int:
    stmfd   sp!,{r0-r12,lr}          /* 保护寄存器,以及返回地址 */
    ldr r0,=INTOFFSET
    ldr r0,[r0]

    mrs     r1,spsr
    stmfd   sp!,{r1}                 /* 保护SPSR_irq，以支持中断嵌套 */
    msr     cpsr_c,#SF_SVCMODE|SF_IRQ_BIT   /* 进入SVCMODE,以便允许中断嵌套 */
    stmfd   sp!,{r0,lr}             /* 保存lr_svc,r0是保持栈8字节对齐的填充物 */

    /*ldr     r2,=irq_bottom			 取异步信号地址 */
    ldr		r2, =engine_irq		/* 取异步信号地址 */

    mov     lr,pc               /* 这执行4G空间内的函数调用 */
    ldr     pc,[r2]             /* intn用r0传递 */

    ldmfd   sp!,{r0,lr}     /* 恢复lr_svc, */
    msr     cpsr_c,#SF_IRQMODE|SF_IRQ_BIT|SF_FIQ_BIT   /* 更新cpsr,进入IRQ模式并禁止中断,由于lr寄存
                             		器的问题,进入irq或者fiq模式，均要禁止中断以防嵌套，
                             		只有在进入svc状态才允许嵌套 */
    ldmfd   sp!,{r0}        /* spsr->r0 */
    msr     spsr_cxsf,r0    /* 恢复spsr */
    ldmfd   sp!,{r0-r12,lr}
    subs    pc,lr,#4        /* 此后，中断被重新打开 */

.section .text.start_fiq, "ax", %progbits
.global start_fiq
start_fiq:
    b       .
.end
