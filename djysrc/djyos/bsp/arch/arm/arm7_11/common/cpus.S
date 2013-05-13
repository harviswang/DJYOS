@----------------------------------------------------
@Copyright (C), 2004-2009,  luoshitian.
@版权所有 (C), 2004-2009,   罗侍田.
@所属模块:调度器
@作者:  罗侍田.
@版本：V1.0.1
@文件描述:调度器中与CPU直接相关的汇编代码，主要是线程切换方面的代码。
@其他说明:
@修订历史:
@2. 日期: 2010-01-04
@   作者:  罗侍田.
@   新版本号: V1.0.1
@   修改说明: 从gcc移植到keil
@1. 日期: 2009-01-04
@   作者:  罗侍田.
@   新版本号: V1.0.0
@   修改说明: 原始版本
@------------------------------------------------------
.cpu	arm920t
.fpu	softvfp
.file	"cpus.S"

.equ    MODEMASK, 0x1f
.equ    NOIRQ, 0x80

@注: sp并不保存在栈中，而是在vm->stack中!!

@----重置线程-----------------------------------------------------------------
@功能:  初始化线程的栈，并挂上线程执行函数，新创建线程时将会调用本函数
@参数:  thread_routine，被重置的线程函数
@		vm，线程虚拟机指针
@返回:  初始化结束后的当前栈指针
@函数原型:void __asm_reset_thread(void (*thread_routine)(void),
@                                   struct  thread_vm  *vm);
@-----------------------------------------------------------------------------
.section .text.__asm_reset_thread, "ax", %progbits
.global __asm_reset_thread
__asm_reset_thread:
	stmfd	sp!,{lr}
    ldr     r2,[r1,#4]          @取虚拟机栈顶指针
    ldr     r3,=__djy_vm_engine     @取虚拟机引擎指针
    stmfd   r2!,{r3}            @存pc，
    sub     r2,r2,#13*4         @后退13个寄存器,初始状态r1-r12随意，__vm_engine
    							@函数不返回，lr也无意义
    @__vm_engine的参数，切换上下文时，service_routine将恢复到r0中，根据调用约
    @定，r0的值就是__vm_engine函数的参数。这只在创建并初始化线程上下文时需要这
    @样做，线程启动以后线程切换要保存上下文时，保存r0的实际值即可
    stmfd   r2!,{r0}            @保存 thread_routine指针到r0的位置.
    mrs     r0,CPSR             @取cpsr
    stmfd   r2!,{r0}			@保存cpsr
    str     r2,[r1]             @保存vm的当前栈指针到vm->stack中
    ldmfd	sp!,{pc}
@ end of __asm_reset_thread

@----复位老线程，切换到新线程-------------------------------------------------
@功能:  把old_vm的上下文重新初始化到新创建的状态，然后切换到新线程的
@		上下文中继续执行。
@		当一个在常驻内存的虚拟机中处理的事件完成后，不能删除该虚拟机，必须复位
@		该虚拟机，然后切换到其他就绪线程，这时候就需要调用本函数。因此本函数在
@		被复位的线程上下文里面执行。
@参数:  thread_routine，被重置的线程函数
@		new_vm，目标线程的虚拟机
@		old_vm，被复位的虚拟机
@返回:  无
@函数原型:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  thread_vm *new_vm,struct  thread_vm *old_vm);
@-----------------------------------------------------------------------------
.section .text.__asm_reset_switch, "ax", %progbits
.global __asm_reset_switch
__asm_reset_switch:
    .global __asm_reset_switch
    ldr     sp,[r2,#4]          @取老虚拟机栈顶指针
    ldr     r12,=__djy_vm_engine    @取虚拟机引擎指针
    stmfd   sp!,{r12}           @存pc
    sub     sp,sp,#13*4         @后退13个寄存器,初始状态r1-r12随意.__djy_vm_engine
    							@函数不返回，lr也无意义
                                @至此，完成老虚拟机复位
    stmfd   sp!,{r0}            @保存 thread_routine指针.
    mrs     r5,CPSR             @取cpsr
    stmfd   sp!,{r5}            @保存cpsr,至此，完成老线程重置
    str     sp,[r2]             @保存当前栈指针到old_vm->stack中

    ldr     sp,[r1]             @取得新上下文指针
    bl      int_half_enable_asyn_signal
    ldmfd   sp!,{r4}            @新线程的cpsr-->r4
    ldr     lr,[sp,#52]         @恢复lr
    mov     r5,sp
    add     sp,sp,#60           @即将切换到svc，sp不再可用

@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#0x1f
    orr     r0,#0x93            @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    bl      int_contact_asyn_signal     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复
@ end of __asm_reset_switch

@----切入上下文---------------------------------------------------------------
@功能:  不保存原上下文，直接切入新的上下文执行
@参数:  new_sp，新上下文的栈指针
@返回:  无
@函数原型: void __asm_turnto_context(struct  thread_vm  *new_vm);
@说明:  当事件完成,就没有必要保存旧事件的上下文,直接切换到新事件即可.
@-----------------------------------------------------------------------------
.section .text.__asm_turnto_context, "ax", %progbits
.global __asm_turnto_context
__asm_turnto_context:
    .global __asm_turnto_context
    ldr     sp,[r0]     		@取得新上下文指针
    bl      int_half_enable_asyn_signal @对应done函数开头的 int_save_asyn_signal调用
    ldmfd   sp!,{r4}            @新线程的cpsr-->r4
    ldr     lr,[sp,#52]         @恢复lr
    mov     r5,sp
    add     sp,sp,#60           @即将切换到svc，sp不再可用

@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#0x1f
    orr     r0,#0x93            @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    bl      int_contact_asyn_signal     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复
@ end of __asm_turnto_context

@----上下文切换---------------------------------------------------------------
@功能:  保存当前线程的上下文，切换到新线程的上下文中继续执行。
@参数:  new_sp，新上下文的栈指针
@参数:  old_sp，旧上下文的栈指针的指针，即&vm->stack。无需提供旧上下文栈指针，
@               sp寄存器的当前值就是
@返回:  无
@函数原型: void __asm_switch_context(struct  thread_vm *new_vm,struct  thread_vm *old_vm);
@-----------------------------------------------------------------------------
.section .text.__asm_switch_context, "ax", %progbits
.global __asm_switch_context
__asm_switch_context:
    .global __asm_switch_context
    stmfd   sp!,{lr}            @保存PC
    stmfd   sp!,{r0-r12,lr}     @保存寄存器和LR
    mrs     r5,CPSR
    stmfd   sp!,{r5}         @保存cpsr
    str     sp,[r1]             @保存旧上下文栈指针到old_vm->stack

    ldr     sp,[r0]             @取得新上下文指针
    bl      int_half_enable_asyn_signal
    ldmfd   sp!,{r4}            @新线程的cpsr-->r4
    ldr     lr,[sp,#52]         @恢复lr
    mov     r5,sp
    add     sp,sp,#60           @即将切换到svc，sp不再可用

@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#0x1f
    orr     r0,#0x93            @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    bl      int_contact_asyn_signal     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复
@ end of __asm_switch_context

@中断上下文切换函数,
@本函数由中断函数调用,由于中断发生时间是随机的,编译器没有任何办法保护lr,若发生
@上下文切换,则必需手动把LR_sys保存到上下文中,恢复时才不会出错.
@本函数必需由C语言调用,如果汇编调用,调用前要保存LR,见switch_context函数的注释.
@只有在中断没有嵌套时才可能调用本函数,中断前的状态是sys态.

@----中断中的上下文切换-------------------------------------------------------
@功能:  保存被中断线程的上下文，切换到新线程的上下文中继续执行。本函数虽然在中
@		断服务函数(非用户用int_isr_connect函数连接的中断服务函数)中，但在ARM
@		中，却运行在svc态
@参数:  new_sp，切换目标虚拟机
@参数:  old_sp，被中断线程虚拟机
@返回:  无
@函数原型: void __asm_switch_context_int(struct thread_vm *new_vm,struct thread_vm *old_vm);
@-----------------------------------------------------------------------------
.section .text.__asm_switch_context_int, "ax", %progbits
.global __asm_switch_context_int
__asm_switch_context_int:
    .global __asm_switch_context_int
    stmfd   sp!,{r2-r12,lr} @保存正在服务的中断上下文

    @以下几行把old_vm的上下文从正在服务的中断栈顶转移到虚拟机栈中
    ldr     r2,=irq_top   @取irq栈基址,这里存放着被中断线程的上下文
    ldmea   r2!,{r3-r10}    @按递增式空栈方式弹栈，结果:
    						@[r2-1]=LR_irq->r10，被中断线程的PC+4
    						@[r2-2]=r12->r9，被中断线程的r12
    						@[r2-3]=r11->r8，被中断线程的r11
    						@.......
    						@[r2-8]=r6->r3，被中断线程的r6
    sub     r10,r10,#4      @中断栈中的LR_irq-4=PC

	@以下三句就是取出old_vm的SP_sys，只能通过stmfd指令间接取
    mov     r11,sp			@下一句不能用SP，故先拷贝到r11
    stmfd   r11,{sp}^       @被中断线程的SP_sys压入正在服务的中断栈中
    sub     r11,r11,#4      @这两条指令本可用 stmfd r11!,{sp}^完成，不知为何keil不行
    ldmfd   r11!,{r12}      @从正在服务的中断栈中读取 SP_sys->R12

    stmfd   r12!,{r10}      @保存 PC_sys
    stmfd   r12,{lr}^       @保存 lr_sys
    sub     r12,r12,#4      @这两条指令本可以用 stmfd r12!,{lr}^完成，不知为何keil不行
    stmfd   r12!,{r3-r9}    @保存被中断线程的r12-r6到它的栈中
    ldmea   r2!,{r3-r9}     @读被中断线程的r5-r0->r9-r4,SPSR_irq->r3,递增式空栈
    stmfd   r12!,{r3-r9}    @保存被中断线程的r5-r0,CPSR_sys到它的栈中

    str     r12,[r1]        @换出的上下文的栈指针-->old_sp

    @以下几行把new_vm的上下文copy到IRQ栈顶
    @与递减式满栈对应,此时IRQ栈用递增式空栈的方式访问

    ldr     r12,[r0]        @读取需换入的栈指针
    ldmfd   r12!,{r3-r11}   @读取换入线程的CPSR_sys->r3
                            @读取换入线程的r0-r7->r4-r11
    stmea   r2!,{r3-r11}    @保存换入线程的CPSR_sys->SPSR_irq, r0-r7到IRQ栈
    ldmfd   r12!,{r3-r7}    @读取换入线程的r8-r12->r3-r7
    stmea   r2!,{r3-r7}     @保存换入线程的r8-r12到IRQ栈
    ldmfd   r12,{lr}^       @恢复换入线程的LR_sys到寄存器中
    add     r12,r12,#4      @这两条指令本可用 ldmfd r12!,{lr}^完成，不知为何keil不行
    ldmfd   r12!,{r3}       @读取换入线程的PC->r3
    add     r3,r3,#4        @模拟IRQ保存被中断上下文PC的方式：PC+4->LR_irq
    stmea   r2!,{r3}        @保存换入线程的LR_irq到IRQ栈
    stmfd   r12!,{r12}      @读取SP_sys到r12
    ldmfd   r12,{SP}^       @恢复SP_sys
    add     r12,r12,#4      @这两条指令本可用 ldmfd r12!,{sp}^完成，不知为何keil不行
    mov     r0,r0           @无论是否操作当前状态的SP，操作sp后，不能立即执行函数
                            @返回指令，否则返回指令的结果不可预知。
    ldmfd   sp!,{r2-r12,pc}

.end
