/*
@----------------------------------------------------
@Copyright (C), 2005-2008,  lst.
@版权所有 (C), 2005-2008,   lst.
@所属模块:  异常模块
@作者：     lst
@版本：      V1.00
@初始版本完成日期：2009-08-30
@文件描述:  系统异常模块中的汇编部分
@其他说明:  无
@修订历史:
@2. ...
@1. 日期:
@   作者:
@   新版本号：
@   修改说明:
@------------------------------------------------------
*/

.syntax		unified
.cpu		cortex-m3
.fpu		softvfp
.thumb
.file	"exceptions.S"

/**** 栈大小配置 ****/
.section	.stack.msp, "aw", %note
.align		3
	.space	0x1000


/**** 异常向量表 ****/
.section    .isr_vector, "ax", %progbits
.align		3
.global		isr_vector
isr_vector:
	.word	msp_top
	.word	cpu_init
	.word	nmi_handler
	.word	hardfault_handler
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   exp_svc_handler           @ SVCall Handler
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ PendSV Handler
    .word   exp_systick_handler       @ SysTick Handler

/**** 向量处理函数 ****/
.text
.align 2
.global default_handler
.thumb
.thumb_func
default_handler:
	b		default_handler

.global nmi_handler
.thumb_set nmi_handler, default_handler

.global hardfault_handler
.thumb_set hardfault_handler, default_handler

.text
.align 2
.global exp_systick_handler
.thumb
.thumb_func
exp_systick_handler:
    push    {r0,lr}				/* r0只是为了满足栈指针8字节对齐的填充物 */
    ldr     r0,=0xe000e010
    ldr     r0,[r0]
    mrs     r0,psp
    stmfd   r0!,{r4-r11}        /* 保护寄存器,以及返回地址 */
    msr     psp,r0
    bl      exp_esr_tick
    pop     {r0,lr}
    mrs     r0,psp
    ldmfd   r0!,{r4-r11}
    msr     psp,r0
    bx      lr

.text
.align 2
.global exp_svc_handler
.thumb
.thumb_func
exp_svc_handler:
    push    {lr}
    bl      int_contact_asyn_signal
	pop     {lr}
    TST     LR,#0x4             /* 测试EXC_RETURN的比特2 */
    ITE     EQ                  /* 如果为0, */
    MRSEQ   R0,MSP              /* 则使用的是主堆栈，故把MSP的值取出 */
    MRSNE   R0,PSP              /* 否则, 使用的是进程堆栈，故把MSP的值取出 */
    LDR     R1,[R0,#24]         /* 从栈中读取PC的值 */
    LDRB    R1,[R1,#-2]         /* 从SVC指令中读取立即数放到R0 */
    cbnz    r1,svc_error        /* 目前只支持0号svc调用 */
    add     r0,#32              /* 0号调用用于上下文切换，调整psp指针 */
    ldmfd   r0!,{r4-r11}        /* 手工弹出r4-r11 */
    msr     psp,r0              /* psp指向待切入的上下文 */
svc_error:                      /* 直接返回 */
    bx      lr                  /* 返回，实际弹出的将是待切入上下文 */

.end
