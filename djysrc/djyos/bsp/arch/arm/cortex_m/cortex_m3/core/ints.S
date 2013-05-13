/*
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
*/
.syntax		unified
.cpu		cortex-m3
.fpu		softvfp
.thumb
.file	"ints.S"

/* *********************** ********** ***********************
;为什么要保存r4-r11呢，如果被中断的是线程a，而中断服务函数使高优先级的线程b就绪，
;就得返回到线程b中，而把线程a的r4-r11保存到线程a的上下文中，这两个操作都需要使用
;r4-r11，为啥不保存在栈中呢?，因为__asm_switch_context_int函数不知道栈顶在哪里。
;这在si版本中是没问题的，但可能在dlsp版本中出问题
*********************** ********** *********************** */
.text
.align 2
.global __start_asyn_signal
.thumb
.thumb_func
__start_asyn_signal:
    push    {r0,lr}				/* r0只是为了满足栈指针8字节对齐的填充物 */
    mrs     r0,psp
    stmfd   r0!,{r4-r11}        /* 保护寄存器,以及返回地址 */
    msr     psp,r0
    mrs     r0,ipsr             /* 取异常号 */
    sub     r0,r0,#16
    bl      __int_engine_asyn_signal
    pop     {r0,lr}
    mrs     r0,psp
    ldmfd   r0!,{r4-r11}
    msr     psp,r0
    bx      lr

.text
.align 2
.global __start_real
.thumb
.thumb_func
__start_real:
    push    {r0,lr}				/* r0只是为了满足栈指针8字节对齐的填充物 */
    mrs     r0,psp
    stmfd   r0!,{r4-r11}        /* 保护寄存器,以及返回地址 */
    msr     psp,r0
    mrs     r0,ipsr             /* 取异常号 */
    sub     r0,r0,#16
    bl      __int_engine_real
    pop     {r0,lr}
    mrs     r0,psp
    ldmfd   r0!,{r4-r11}
    msr     psp,r0
    bx      lr

/*
@----进入低级原子操作---------------------------------------------------------
@功能：读取当前异步信号开关状态，然后禁止异步信号。低级级原子操作可用来防止临界
@      数据的并发访问，但可能会被实时中断打断。
@      int_low_atom_start---int_low_atom_end必须配套使用，绝对不能嵌套，在可能
@      嵌套的地方，必须使用int_save_asyn_signal来关闭中断(调度)。
@      这两函数可视作"int_restore_asyn_signal--int_save_asyn_signal"的快速版本，
@      除本身不能嵌套调用外，在被他们套住的代码块内，也不允许调用
@      int_restore_asyn_signal、int_save_asyn_signal中的任何一个。
@参数：无
@返回：原子状态
@备注：移植敏感
@函数原型:atom_low_t int_low_atom_start(void)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global int_low_atom_start
.thumb
.thumb_func
int_low_atom_start:
    mrs     r0, basepri
    mov     r1, #0xff
    msr     basepri, r1
    bx      lr

/*
@----离开低级原子操作---------------------------------------------------------
@功能：恢复进入前异步信号许可状态，本函数应与int_low_atom_start函数配套。
@      int_low_atom_start---int_low_atom_end必须配套使用，绝对不能嵌套，在可能
@      嵌套的地方，必须使用int_save_asyn_signal来关闭中断(调度)。
@      这两函数可视作"int_restore_asyn_signal--int_save_asyn_signal"的快速版本，
@      除本身不能嵌套调用外，在被他们套住的代码块内，也不允许调用
@      int_restore_asyn_signal、int_save_asyn_signal中的任何一个。
@参数：low，int_low_atom_start保存的原子状态
@返回：无
@备注：移植敏感
@函数原型:void int_low_atom_end(atom_low_t low)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global int_low_atom_end
.thumb
.thumb_func
int_low_atom_end:
    msr     basepri, r0
    bx      lr

/*           
@----进入高级原子操作---------------------------------------------------------
@功能：读当前总中断状态，然后禁止总中断。高级原子操作是指期间不容许任何原因
@      打断的操作。
@      int_high_atom_start--int_high_atom_end必须配套使用，绝对不能嵌套，在可能
@      嵌套的地方，必须使用int_save_trunk来关闭中断(调度)。
@      这两函数可视作"int_restore_trunk--int_save_trunk"的快速版本，除本身不能
@      嵌套调用外，在被他们套住的代码块内，也不允许调用int_restore_trunk和
@      int_save_trunk中的任何一个。
@参数：无
@返回：原子状态
@备注：移植敏感
@函数原型:atom_high_t int_high_atom_start(void)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global int_high_atom_start
.thumb
.thumb_func
int_high_atom_start:
    mrs     r0, primask
    cpsid   i
    bx      lr

/*                   
@----离开高级原子操作---------------------------------------------------------
@功能：恢复进入前总中断状态，本函数应与int_high_atom_start函数配套
@      int_high_atom_start--int_high_atom_end必须配套使用，绝对不能嵌套，在可能
@      嵌套的地方，必须使用int_save_trunk来关闭中断(调度)。
@      这两函数可视作"int_restore_trunk--int_save_trunk"的快速版本，除本身不能
@      嵌套调用外，在被他们套住的代码块内，也不允许调用int_restore_trunk和
@      int_save_trunk中的任何一个。
@参数：high，int_high_atom_start保存的原子状态
@返回：无
@备注：移植敏感
@函数原型:void int_high_atom_end(atom_high_t high)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global int_high_atom_end
.thumb
.thumb_func
int_high_atom_end:
    msr     primask, r0
    bx      lr


.end
