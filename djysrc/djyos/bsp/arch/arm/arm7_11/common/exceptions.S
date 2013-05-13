/* *********************** ********** ***********************
 *----------------------------------------------------
 *Copyright (C)  2005-2011   lst.
 *版权所有 (C)  2005-2011    lst.
 *所属模块:  异常向量
 *作者：     lst
 *版本：      V1.1.0
 *文件描述:  CPU异常处理例程中必须用汇编语言实现的部分
 *其他说明:  无
 *修订历史:
 *1. 日期: 2011-03-30
 *   作者: lst
 *   新版本号：V1.0.0
 *------------------------------------------------------
*********************** ********** *********************** */
.cpu	arm920t
.fpu	softvfp
.file	"exceptions.S"

.section	.stack.usr, "ax", %note
.align	3
	.space	0x1000
.section	.stack.svc, "ax", %note
.align	3
	.space	0x10000
.section	.stack.irq, "ax", %note
.align	3
	.space	0x1000
.section	.stack.fiq, "ax", %note
.align	3
	.space	0x1000
.section	.stack.undef, "ax", %note
.align	3
	.space	0x1000
.section	.stack.abort, "ax", %note
.align	3
	.space	0x1000

/* 异常向量表 */
.section    .isr_vector, "ax", %progbits
.align	3
.global isr_vector
isr_vector:
    ldr pc, ResetAddr
    ldr pc, UndefinedAddr
    ldr pc, SWI_Addr
    ldr pc, PrefetchAddr
    ldr pc, DataAbortAddr
    b	.
    ldr pc, IRQ_Addr
    ldr pc, FIQ_Addr

ResetAddr:
	.word _start
UndefinedAddr:
	.word except_undef
SWI_Addr:
	.word except_swi
PrefetchAddr:
	.word except_pabort
DataAbortAddr:
	.word except_dabort
Nouse:
	.word 0
IRQ_Addr:
	.word start_int
FIQ_Addr:
	.word start_fiq

except_undef:
    b   .
except_swi:
    b   .
except_pabort:
    b   .
except_dabort:
    b   .
.end
