@----------------------------------------------------
@Copyright (C), 2005-2008,  lst.
@版权所有 (C), 2005-2008,   lst.
@所属模块:  中断模块
@作者：     lst
@版本：      V1.00
@初始版本完成日期：2009-08-10
@文件描述:  中断模块中的汇编部分
@其他说明:  无
@修订历史:
@2. ...
@1. 日期:
@   作者:
@   新版本号：
@   修改说明:
@------------------------------------------------------

.syntax		unified
.cpu		cortex-m0
.fpu		softvfp
.thumb
.file	"ints.S"

.extern  __int_engine_asyn_signal

@为什么要保存r4-r11呢，如果被中断的是线程a，而中断服务函数使高优先级的线程b就绪，
@就得返回到线程b中，而把线程a的r4-r11保存到线程a的上下文中，这两个操作都需要使用
@r4-r11，为啥不保存在栈中呢?，因为__asm_switch_context_int函数不知道栈顶在哪里。
@这在si版本中是没问题的，但可能在dlsp版本中出问题
.text
.align 2
.global __start_int
.thumb
.thumb_func
__start_int:
    push    {r0,lr}				@r0只是为了满足栈指针8字节对齐的填充物
    mrs     r1,psp
    subs    r1,r1,#8*4          @后退8个寄存器,用于保存r4~r11
    msr     psp,r1
    stmia   r1!,{r4-r7}         @保护寄存器,调用tick异常可能引起线程切换。
    mov     r2,r8
    mov     r3,r9
    mov     r4,r10
    mov     r5,r11
    stmia   r1!,{r2-r5}         @保护寄存器,中断可能引起线程切换。
    bl      __int_engine_asyn_signal
    pop     {r0,r1}
    mov     lr,r1
    mrs     r0,psp
    ldmfd   r0!,{r4-r7}
    ldmfd   r0!,{r2,r3}
    mov     r8,r2
    mov     r9,r3
    ldmfd   r0!,{r2,r3}
    mov     r10,r2
    mov     r11,r3
    msr     psp,r0
    bx      lr

.text
.align 2
.global __start_int_0
.thumb
.thumb_func
__start_int_0:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#0              @取异常号
    
    ldr     r1,=(vector_table+0*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_1
.thumb
.thumb_func
__start_int_1:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#1              @取异常号
    
    ldr     r1,=(vector_table+1*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_2
.thumb
.thumb_func
__start_int_2:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#2              @取异常号
    
    ldr     r1,=(vector_table+2*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_3
.thumb
.thumb_func
__start_int_3:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#3              @取异常号
    
    ldr     r1,=(vector_table+3*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_4
.thumb
.thumb_func
__start_int_4:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#4              @取异常号
    
    ldr     r1,=(vector_table+4*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_5
.thumb
.thumb_func
__start_int_5:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#5              @取异常号
    
    ldr     r1,=(vector_table+5*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_6
.thumb
.thumb_func
__start_int_6:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#6              @取异常号
    
    ldr     r1,=(vector_table+6*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_7
.thumb
.thumb_func
__start_int_7:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#7              @取异常号
    
    ldr     r1,=(vector_table+7*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_8
.thumb
.thumb_func
__start_int_8:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#8              @取异常号
    
    ldr     r1,=(vector_table+8*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_9
.thumb
.thumb_func
__start_int_9:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#9              @取异常号
    
    ldr     r1,=(vector_table+9*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_10
.thumb
.thumb_func
__start_int_10:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#10              @取异常号
    
    ldr     r1,=(vector_table+10*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_11
.thumb
.thumb_func
__start_int_11:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#11              @取异常号
    
    ldr     r1,=(vector_table+11*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_12
.thumb
.thumb_func
__start_int_12:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#12              @取异常号
    
    ldr     r1,=(vector_table+12*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_13
.thumb
.thumb_func
__start_int_13:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#13              @取异常号
    
    ldr     r1,=(vector_table+13*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_14
.thumb
.thumb_func
__start_int_14:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#14              @取异常号
    
    ldr     r1,=(vector_table+14*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_15
.thumb
.thumb_func
__start_int_15:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#15              @取异常号
    
    ldr     r1,=(vector_table+15*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_16
.thumb
.thumb_func
__start_int_16:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#16              @取异常号
    
    ldr     r1,=(vector_table+16*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_17
.thumb
.thumb_func
__start_int_17:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#17              @取异常号
    
    ldr     r1,=(vector_table+17*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_18
.thumb
.thumb_func
__start_int_18:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#18              @取异常号
    
    ldr     r1,=(vector_table+18*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_19
.thumb
.thumb_func
__start_int_19:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#19              @取异常号
    
    ldr     r1,=(vector_table+19*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_20
.thumb
.thumb_func
__start_int_20:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#20              @取异常号
    
    ldr     r1,=(vector_table+20*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_21
.thumb
.thumb_func
__start_int_21:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#21              @取异常号
    
    ldr     r1,=(vector_table+21*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_22
.thumb
.thumb_func
__start_int_22:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#22              @取异常号
    
    ldr     r1,=(vector_table+22*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_23
.thumb
.thumb_func
__start_int_23:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#23              @取异常号
    
    ldr     r1,=(vector_table+23*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_24
.thumb
.thumb_func
__start_int_24:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#24              @取异常号
    
    ldr     r1,=(vector_table+24*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_25
.thumb
.thumb_func
__start_int_25:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#25              @取异常号
    
    ldr     r1,=(vector_table+25*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_26
.thumb
.thumb_func
__start_int_26:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#26              @取异常号
    
    ldr     r1,=(vector_table+26*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_27
.thumb
.thumb_func
__start_int_27:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#27              @取异常号
    
    ldr     r1,=(vector_table+27*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_28
.thumb
.thumb_func
__start_int_28:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#28              @取异常号
    
    ldr     r1,=(vector_table+28*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_29
.thumb
.thumb_func
__start_int_29:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#29              @取异常号
    
    ldr     r1,=(vector_table+29*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_30
.thumb
.thumb_func
__start_int_30:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#30              @取异常号
    
    ldr     r1,=(vector_table+30*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.text
.align 2
.global __start_int_31
.thumb
.thumb_func
__start_int_31:
    push    {r0,lr}			    @r0只是为了满足栈指针8字节对齐的填充物
    movs    r0,#31              @取异常号
    
    ldr     r1,=(vector_table+31*4) 
    ldr     r1,[r1]
    blx     r1
    pop     {r0,pc}

.end
