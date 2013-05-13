//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:中断模块
//作者:  罗侍田.
//版本：V2.0.1
//文件描述: s3c2410与中断相关的代码，包含异步信号与实时中断
//其他说明:
//修订历史:
//3. 日期: 2010-01-12
//   作者:  罗侍田.
//   新版本号: V2.0.1
//   修改说明: 
//   1、查到了一个非常隐蔽的问题，在__int_engine_real函数内，这两行
//      if(tg_int_global.en_trunk_counter == 0)
//          tg_int_global.en_trunk = true;      //原误写成false了
//   2、在int_save_trunk,int_restore_trunk,int_save_asyn_signal,
//      int_restore_asyn_signal几个函数中增加对bg_schedule_enable的操作
//2. 日期: 2010-01-04
//   作者:  罗侍田.
//   新版本号: V2.0.0
//   修改说明: 从原版本中去掉硬件相关部分而成，存放位置也从arch改到djyos
//1. 日期: 2009-12-10
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 从44b0版本移植而成
//------------------------------------------------------

//实时中断的限制:
// 1、不允许调用任何系统调用。
// 2、不允许改变任何中断线的异步信号还是实时中断属性。
// 3、不允许使能或禁止异步信号，包括信号线和总异步信号开关。
// 4、允许使能或禁止属性为实时中断的中断线

#include "os_inc.h"

ufast_t tg_int_lookup_table[cn_int_line_last+1];
struct int_master_ctrl  tg_int_global;

//----保存总中断状态并禁止总中断--------------------------------------------
//功能：本函数是int_restore_trunk()的姊妹函数，调用本函数使禁止次数增1，调用一
//      次int_restore_trunk使禁止次数减1。
//      若当前次数为0，增加为1并禁止总中断，不为0时简单地增1
//参数：无
//返回：无
//------------------------------------------------------------------------------
void int_save_trunk(void)
{
    if(tg_int_global.nest_real != 0)
        return;
    int_cut_trunk();
    if(tg_int_global.en_trunk_counter != cn_limit_ucpu)//达上限后再加会回绕到0
        tg_int_global.en_trunk_counter++;
    //原算法是从0->1的过程中才进入，但如果在en_trunk_counter != 0的状态下因故障
    //使中断关闭，将使用户后续调用的int_save_trunk起不到作用
    bg_schedule_enable = false;
    return;
}

//----恢复保存的总中断状态---------------------------------------------------
//功能：本函数是int_save_trunk()的姊妹函数，调用本函数使禁止次数减1，调用
//      一次int_save_trunk使禁止次数增1，
//      当次数减至0时激活总中断,否则简单减1
//参数：无
//返回：无
//------------------------------------------------------------------------------
void int_restore_trunk(void)
{
    if(tg_int_global.nest_real != 0)
        return;
    if(tg_int_global.en_trunk_counter != 0)
        tg_int_global.en_trunk_counter--;
    if(tg_int_global.en_trunk_counter == 0)
    {
        int_contact_trunk();
        if(tg_int_global.en_asyn_signal_counter == 0)
        {
            bg_schedule_enable = true;
            if(pg_event_running != pg_event_ready)
                __djy_schedule();
        }
    }else
    {
        int_cut_trunk();
    }
    return;
}

//----查看总中断是否允许-----------------------------------------------------
//功能：
//参数：无
//返回：允许返回true,禁止返回false
//-----------------------------------------------------------------------------
bool_t int_check_trunk(void)
{
    if(tg_int_global.en_trunk_counter == 0)
        return true;
    else
        return false;
}

//----保存当前状态并禁止异步信号------------------------------------------------
//功能：本函数是int_restore_asyn_signal()的姊妹函数，调用本函数使禁止次数增加，
//      调用一次int_restore_asyn_signal()使禁止次数减少。
//      若当前次数为0，增加为1并禁止异步信号，不为0时简单地增1
//参数：无
//返回：无
//------------------------------------------------------------------------------
void int_save_asyn_signal(void)
{
    if(tg_int_global.nest_asyn_signal != 0)
        return;
    //达上限后再加会回绕到0
    int_cut_asyn_signal();
    if(tg_int_global.en_asyn_signal_counter != cn_limit_ucpu)
        tg_int_global.en_asyn_signal_counter++;
    //原算法是从0->1的过程中才进入，但如果在en_asyn_signal_counter != 0的状态下
    //因故障使中断关闭，将使用户后续调用的en_asyn_signal_counter起不到作用
    bg_schedule_enable = false;
    return;
}

//----复位异步信号开关------------------------------------------------
//功能：把异步信号开关恢复到初始状态，即en_asyn_signal_counter=1的状态，初始化
//      中断系统后，还要做大量的模块初始化工作才能启动多事件调度，在启动多事件
//      调度前调用本函数复位异步信号状态，可以防止模块初始化代码的bug意外修改
//      了异步信号使能状态。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void __int_reset_asyn_signal(void)
{
    int_cut_asyn_signal();
    tg_int_global.en_asyn_signal_counter =1;
    return;
}

//----恢复保存的异步信号状态----------------------------------------------------
//功能：本函数是int_save_asyn_signal()的姊妹函数，调用本函数使禁止次数减少，调用
//      一次int_save_asyn_signal()是禁止次数增加。
//      当次数减至0时激活异步信号,否则简单减1
//参数：无
//返回：无
//------------------------------------------------------------------------------
void int_restore_asyn_signal(void)
{
    if(tg_int_global.nest_asyn_signal != 0)
        return;
    if(tg_int_global.en_asyn_signal_counter != 0)
        tg_int_global.en_asyn_signal_counter--;
    if(tg_int_global.en_asyn_signal_counter==0)
    {
//        tg_int_global.en_asyn_signal = true;   //异步信号设为使能
        if(tg_int_global.en_trunk_counter == 0) 
        {
            bg_schedule_enable = true;
            if(pg_event_running != pg_event_ready)
                __djy_schedule();
            else
                int_contact_asyn_signal();
        }else
            int_contact_asyn_signal();
    }else
    {
        int_cut_asyn_signal();    //防止counter>0期间意外(bug)打开
    }
    return;
}

//----开异步信号前半部---------------------------------------------------------
//功能: 这是一个专门为上下文切换准备的函数/
//      在数据结构中异步信号设置为开启状态，但不完成最后一步:合上异步信号开关。
//      用于上下文切换过程中，先行设置好状态，使得上下文切换的最后一步和重新打开
//      中断同时完成。这个过程必然要关中断，有些系统没有能够与操作pc同时完成的、
//      独立的异步信号开关，这个步骤势必会把实时中断也关掉，因此把设置数据结构的
//      部分提出来，以减少关闭实时中断的时间。这使得部分cpu不能实现"实时中断永不
//      关闭"的要求，但是关闭时间是非常短的。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void int_half_enable_asyn_signal(void)
{
    tg_int_global.en_asyn_signal_counter = 0;
}

//----查看异步信号是否允许-----------------------------------------------------
//功能：
//参数：无
//返回：允许返回true,禁止返回false
//-----------------------------------------------------------------------------
bool_t int_check_asyn_signal(void)
{
    if( tg_int_global.en_asyn_signal_counter == 0)
        return true;
    else
        return false;
}

//----保存当前状态并禁止异步信号中断线-----------------------------------------
//功能：本函数是int_restore_asyn_line()的姊妹函数，调用本函数使禁止次数增加，调
//      用一次int_restore_asyn_line是禁止次数减少。
//      若当前次数为0，增加为1并禁止中断线，不为0时简单地增1
//参数：ufl_line
//返回：无
//-----------------------------------------------------------------------------
bool_t int_save_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    int_cut_line(ufl_line);
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter!=cn_limit_ucpu)//达上限后再加会回绕到0
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter++;
    //原算法是从0->1的过程中才进入，但如果在en_counter != 0的状态下
    //因故障使中断关闭，将使用户后续调用的en_counter起不到作用
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----保存当前状态并禁止异步信号中断线-----------------------------------------
//功能：本函数是int_restore_real_line()的姊妹函数，调用本函数使禁止次数增加，调
//      用一次int_restore_real_line是禁止次数减少。
//      若当前次数为0，增加为1并禁止中断线，不为0时简单地增1
//参数：ufl_line
//返回：无
//------------------------------------------------------------------------------
bool_t int_save_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;
    int_cut_line(ufl_line);
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter!=cn_limit_ucpu)//达上限后再加会回绕到0
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter++;
    //原算法是从0->1的过程中才进入，但如果在en_counter != 0的状态下
    //因故障使中断关闭，将使用户后续调用的en_counter起不到作用
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----恢复保存的异步信号中断线状态---------------------------------------------
//功能：本函数是int_save_line（）的姊妹函数，调用本函数使禁止次数减少，调
//      用一次int_save_line是禁止次数增加。
//      当次数减至0时激活中断线,否则简单减1
//参数：ufl_line
//返回：无
//注: 本函数不允许在实时中断ISR中调用，若调用直接返回false。
//-----------------------------------------------------------------------------
bool_t int_restore_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter != 0)
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter--;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter==0)
    {
        tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
        int_contact_line(ufl_line);
    }
    return true;
}

//----恢复保存的实时中断线状态-------------------------------------------------
//功能：本函数是int_save_line（）的姊妹函数，调用本函数使禁止次数减少，调
//      用一次int_save_line是禁止次数增加。
//      当次数减至0时激活中断线,否则简单减1
//参数：ufl_line
//返回：无
//注: 与int_restore_asyn_line不一样，本函数在异步信号和实时中断ISR中都允许调用
//-----------------------------------------------------------------------------
bool_t int_restore_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter != 0)
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter--;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter==0)
    {
        tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
        int_contact_line(ufl_line);
    }
    return true;
}

//----直接禁止异步信号中断线---------------------------------------------------
//功能：本函数是int_enable_asyn_line()的姊妹函数，调用本函数使中断线的使能计数器
//      置位，并掐断中断线
//参数：ufl_line
//返回：无
//------------------------------------------------------------------------------
bool_t int_disable_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    int_cut_line(ufl_line);
    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 1;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----直接禁止实时中断线-------------------------------------------------------
//功能：本函数是int_enable_real_line()的姊妹函数，调用本函数使中断线的使能计数器
//      置位，并掐断中断线
//参数：ufl_line
//返回：无
//------------------------------------------------------------------------------
bool_t int_disable_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;
    int_cut_line(ufl_line);
    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 1;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----直接允许异步信号中断线---------------------------------------------------
//功能：本函数是int_disable_asyn_line()的姊妹函数，调用本函数使中断线的使能计数
//      器归零，并接通中断线
//参数：ufl_line
//返回：无
//------------------------------------------------------------------------------
bool_t int_enable_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 0;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
    int_contact_line(ufl_line);
    return true;
}

//----直接允许实时中断线-------------------------------------------------------
//功能：本函数是int_disable_real_line()的姊妹函数，调用本函数使中断线的使能计数
//      器归零，并接通中断线
//参数：ufl_line
//返回：无
//------------------------------------------------------------------------------
bool_t int_enable_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;

    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 0;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
    int_contact_line(ufl_line);
    return true;
}

//----查询中断线使能状态-------------------------------------------------------
//功能：查询中断线是否允许
//参数：ufl_line，欲查询的中断线
//返回：true = 使能，false = 禁止。
//-----------------------------------------------------------------------------
bool_t int_check_line(ufast_t ufl_line)
{
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter == 0)
        return true;
    else
        return false;
}

//----关联中断线与ISR----------------------------------------------------------
//功能：为指定中断线指定中断响应函数，该函数为普通函数，
//参数：ufl_line,需要设置的中断线号
//      isr，中断响应函数，由用户提供，原型：void isr(ufast_t)
//返回：无
//-----------------------------------------------------------------------------
void int_isr_connect(ufast_t ufl_line, u32 (*isr)(ufast_t))
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].ISR = isr;
    return;
}

//----关联中断线与事件类型-----------------------------------------------------
//功能：为指定该中断线指定一个事件类型id，如果是异步信号，则在返回主程序前弹出
//      事件类型为该id的事件，如果是实时中断，则不弹出事件。
//参数：ufl_line,需要设置的中断线号
//      my_evtt_id，事件类型id
//返回：无
//-----------------------------------------------------------------------------
void int_evtt_connect(ufast_t ufl_line,uint16_t my_evtt_id)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].my_evtt_id = my_evtt_id;
    return;
}

//----断开中断线与中断响应函数的关联-------------------------------------------
//功能：断开指定中断线指定中断响应函数的关联，代之以空函数
//参数：ufl_line,需要设置的中断线号
//返回：无
//-----------------------------------------------------------------------------
void int_isr_disconnect(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].ISR = NULL;
    return;
}

//----断开中断线与事件类型的关联-----------------------------------------------
//功能：断开指定中断线指定事件类型的关联，代之以cn_invalid_evtt_id
//参数：ufl_line,需要设置的中断线号
//返回：无
//-----------------------------------------------------------------------------
void int_evtt_disconnect(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].my_evtt_id = cn_invalid_evtt_id;
    return;
}

//----设定中断同步-------------------------------------------------------------
//功能: 阻塞正在处理的事件的线程，直到指定的中断线的中断发生、响应并返回，然后才
//      激活线程。不管中断线原来状态如何，调用本函数将导致中断线被使能(是直接使
//      能，不是调用int_save_asyn_line)，并在返回后恢复禁止状态。
//参数: ufl_line,等待的目标中断线
//返回: false = 该中断已经被其他线程等待，直接返回。
//      true = 成功同步，并在该中断发生后返回。
//备注: 1.中断是一种临界资源，不宜在中断函数中太多的事情，故中断同步的功能比较简
//      单，每条中断线同一时刻只能有一个线程等待，也不允许设置超时等待
//      2.秉承djyos一贯风格，中断同步函数只能把自己置入等待状态，而不能控制别的
//      线程，故函数原型不能是 bool_t int_sync(ufast_t line,uint16_t event_id)
//      3.实时中断设置等待无效，调用本函数时，如果line已经被设置为实时中断，则
//      直接返回false，如果调用本函数后，line不能被设置为实时中断。
//      4.为正确实现功能，须在调用前确保该中断线是被禁止的。
//-----------------------------------------------------------------------------
bool_t int_asyn_signal_sync(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if( !djy_query_sch())
    {   //禁止调度，不能进入异步信号同步状态。
        djy_error_login(enum_knl_cant_sched,NULL);
        return false;
    }
    int_save_asyn_signal();   //在操作就绪队列期间不能发生中断
    //实时中断不能设置同步，一个中断只接受一个同步事件
    if((tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
            || (tg_int_table[tg_int_lookup_table[ufl_line]].sync_event != NULL))
    {
        int_restore_asyn_signal();
        return false; //实时中断或已经有同步事件
    }else
    {
        if(int_query_line(ufl_line) == true)    //中断已经发生，同步条件达到
        {
            int_echo_line(ufl_line);
            int_restore_asyn_signal();
            return true;
        }
        //以下三行从就绪链表中取出running事件
        __djy_cut_ready_event(pg_event_running);
        pg_event_running->next = NULL;
        pg_event_running->previous = NULL;
        pg_event_running->last_status.all
                        = pg_event_running->event_status.all;
        pg_event_running->event_status.bit.wait_asyn_signal = 1;
        tg_int_table[tg_int_lookup_table[ufl_line]].sync_event = pg_event_running;
    }
    int_enable_asyn_line(ufl_line);
    int_restore_asyn_signal();      //调用本函数将引发线程切换，正在处理的事件被
                                    //挂起。
    int_disable_asyn_line(ufl_line);
    return true;
}
//特注: 不提供周期性中断同步功能，因为djyos不提供无条件休眠或者挂起的功能，已周
//      期性时钟中断为例，一次时钟中断把线程触发进入ready后，到下次时钟中断到来
//      之前，该线程要么还在ready态(可能被其他线程抢占)，此时不需要再次触发，
//      要么在等待其他触发条件，比如等待内存分配、等待延时到、等待信号量等，此时
//      若被时钟中断触发，则破坏了软件的结构。周期性中断同步可用多次调用单次同步
//      的方法完成，即每次触发后，线程完成了必要的工作以后重新再次调用单次同步
//      函数，这样，程序的每一步都有明确的目标，而不是无目的的休眠或挂起


