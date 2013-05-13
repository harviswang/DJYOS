//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:中断模块
//作者：lst
//版本：V1.0.0
//文件描述: 中断模块与硬件相关的代码，包含异步信号与实时中断
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2010-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 从原int.c中把硬件无关代码剥离后而成
//------------------------------------------------------
#include "os_inc.h"
#include "exception.h"

struct int_reg volatile * const pg_int_reg
                        = (struct int_reg *)0x4a000000;
//定义中断向量表段，一般而言，异常向量表或异常向量跳转表会和该段一同放在首地址
u32 u32g_vect_table[cn_int_line_last+1] __attribute__((section(".table.vectors")));

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
atom_high_t int_high_atom_start(void)
{
    volatile atom_high_t ret;
    __asm__ volatile(
    		".equ	cn_noint, 0xc0 ;"
    		"mrs	%0, cpsr \n\t"
    		"orr	r0, %0, #cn_noint \n\t"
    		"msr	cpsr_c, r0 \n\t"
    		"mov	r0, %0 \n\t"
    		: "=r" (ret)
    		:
			);
    return ret;
}

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
void int_high_atom_end(atom_high_t high)
{
	__asm__(
			"msr cpsr_c,%0 \n\t" : :"r"(high)
	);
}

//----进入低级原子操作---------------------------------------------------------
//功能：读取当前异步信号开关状态，然后禁止异步信号。低级级原子操作可用来防止临界
//      数据的并发访问，但可能会被实时中断打断。
//      int_low_atom_start---int_low_atom_end必须配套使用，绝对不能嵌套，在可能
//      嵌套的地方，必须使用int_save_asyn_signal来关闭中断(调度)。
//      这两函数可视作"int_restore_asyn_signal--int_save_asyn_signal"的快速版本，
//      除本身不能嵌套调用外，在被他们套住的代码块内，也不允许调用
//      int_restore_asyn_signal、int_save_asyn_signal中的任何一个。
//参数：无
//返回：原子状态
//备注：移植敏感
//-----------------------------------------------------------------------------
atom_low_t int_low_atom_start(void)
{
    register ucpu_t msk_lsb;
	atom_low_t low;
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    low = pg_int_reg->INTMSK;
    msk_lsb = (~tg_int_global.property_bitmap[0]) & cn_int_msk_all_line;
    pg_int_reg->INTMSK |= msk_lsb;
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
	return low;
}
                    
//----离开低级原子操作---------------------------------------------------------
//功能：恢复进入前异步信号许可状态，本函数应与int_low_atom_start函数配套。
//      int_low_atom_start---int_low_atom_end必须配套使用，绝对不能嵌套，在可能
//      嵌套的地方，必须使用int_save_asyn_signal来关闭中断(调度)。
//      这两函数可视作"int_restore_asyn_signal--int_save_asyn_signal"的快速版本，
//      除本身不能嵌套调用外，在被他们套住的代码块内，也不允许调用
//      int_restore_asyn_signal、int_save_asyn_signal中的任何一个。
//参数：low，int_low_atom_start保存的原子状态
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void int_low_atom_end(atom_low_t low)
{
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    pg_int_reg->INTMSK &= ~((~tg_int_global.property_bitmap[0]) & (~low));
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
}

//----接通异步信号开关---------------------------------------------------------
//功能：接通异步信号开关,如果总开关接通且中断线开关接通,该中断将被允许
//      1.当有独立的硬件开关时,把该开关接通即可
//      2.如果没有独立硬件开关,则接通所有被允许的异步信号的线开关.
//      3.2440属于第二种情况,把符合条件的中断线在rINTMSK相应位清0.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void int_contact_asyn_signal(void)
{
    //INTMSK中异步信号且中断线被使能的位被清0
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    pg_int_reg->INTMSK &= ~((~tg_int_global.property_bitmap[0])
                         & tg_int_global.enable_bitmap[0]);
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
}

//----断开异步信号开关---------------------------------------------------------
//功能：断开异步信号开关,所有的异步信号将被禁止
//      1.当有独立的硬件开关时,把该开关断开即可
//      2.如果没有独立硬件开关,则断开所有异步信号的线开关.
//      3.2440属于第二种情况,把所有异步信号线在rINTMSK相应位置1.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void int_cut_asyn_signal(void)
{
    register ucpu_t msk;
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    msk = (~tg_int_global.property_bitmap[0]) & cn_int_msk_all_line;
    pg_int_reg->INTMSK |= msk;
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
}

//----接通总中断开关-----------------------------------------------------------
//功能：接通总中断开关,所有cpu都会有一个总开关,直接操作该开关即可.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void int_contact_trunk(void)
{
    __int_enable_irq();
}

//----断开总中断开关---------------------------------------------------------
//功能：断开总中断开关,所有cpu都会有一个总开关,直接操作该开关即可.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void int_cut_trunk(void)
{
    __int_disable_irq();
}

//----接通单个中断线开关-------------------------------------------------------
//功能：接通单个中断线开关,该中断是否允许还要看后面的开关状态
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
bool_t int_contact_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
#if cfg_asyn_turnkey == 0
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
    {//如果该中断线属于异步信号,且异步信号开关允许,允许该中断线
        if(tg_int_global.en_asyn_signal_counter == 0)
            pg_int_reg->INTMSK &= ~(1<<ufl_line);
    }else  //如果该中断属于实时中断,中断总有独立开关,直接允许该中断线
        pg_int_reg->INTMSK &= ~(1<<ufl_line);
#else
        pg_int_reg->INTMSK &= ~(1<<ufl_line);
#endif
    return  true;
}

//----断开单个中断线开关-------------------------------------------------------
//功能：断开单个中断线开关，无论总中断和异步信号开关状态如何，该中断线被禁止
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
bool_t int_cut_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    pg_int_reg->INTMSK |=1<<ufl_line;
    return true;
}

//----应答中断，清除相应中断线的中断挂起状态-----------------------------------
//功能：硬件应该有相应的功能，提供清除中断挂起的操作，清除前，不能响应同一中断线
//      的后续中断，清除后，才可以响应后续中断。本函数与该中断线被设置为实时中断
//      还是异步信号无关
//参数：ufast ufl_line，指定应答的中断线号
//返回：无
//备注：有些体系中断响应时硬件应答，本函数为空函数。
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
bool_t int_echo_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    pg_int_reg->SRCPND = 1<<ufl_line;
    pg_int_reg->INTPND = 1<<ufl_line;
    return true;
}

//----激发中断-----------------------------------------------------------------
//功能: 触发一个中断.如果中断本已悬挂,本函数无影响.本函数与该中断线被设置为实时
//      中断还是异步信号无关
//参数：ufast ufl_line，欲触发的中断线号
//返回：如果相应的中断线硬件不提供用软件触发中断功能,返回 false,否则返回 true
//备注: 本函数实现依赖于硬件,有些硬件系统不支持此功能.
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
bool_t int_tap_line(ufast_t ufl_line)
{
    return false;    //2440不支持软件触发硬件中断的能力.
}

//----应答全部中断，清除全部中断线的中断挂起状态-------------------------------
//功能：硬件应该有相应的功能，提供清除中断挂起的操作
//参数：ufast ufl_line，指定应答的中断线号
//返回：无
//备注：有些体系中断响应时硬件应答，本函数为空函数。
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
void __int_echo_all_line(void)
{
    pg_int_reg->SRCPND = cn_int_msk_all_line;
    pg_int_reg->INTPND = cn_int_msk_all_line;;
}

//----查询中断线请求状态-------------------------------------------------------
//功能：查询并清除相应中断线状态，可用于查询式中断程序
//参数：ufl_line，欲查询的中断线
//返回：若中断挂起，返回true，否则返回false
//备注: 与硬件结构相关,有些结构可能不提供这个功能,慎用!
//      本函数是移植关键函数
//-----------------------------------------------------------------------------
bool_t int_query_line(ufast_t ufl_line)
{
    ucpu_t  ucl_msk;
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    ucl_msk=1<<ufl_line;
    if(pg_int_reg->SRCPND & ucl_msk)
    {
        pg_int_reg->SRCPND = ucl_msk;
        pg_int_reg->INTPND = ucl_msk;
        return true;
    }else
        return false;
}

//----把指定中断线设置为异步信号--------－－－---------------------------------
//功能：把指定中断线设置为异步信号,若中断正在响应,则当前中断返回后生效
//参数：ufast ufl_line，指定被设置的中断线号
//返回：true=成功，false=失败
//      本函数移植敏感
//-----------------------------------------------------------------------------
bool_t int_setto_asyn_signal(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    tg_int_table[tg_int_lookup_table[ufl_line]].int_type = cn_asyn_signal;   //中断线类型

    tg_int_global.property_bitmap[ufl_line/cn_cpu_bits]
            		&= ~(1<<(ufl_line % cn_cpu_bits));   //设置位图
    return true;
}

//----把指定中断线设置为实时中断--------－－－---------------------------------
//功能：把指定中断线设置为实时中断,若中断正在响应,则当前中断返回后生效
//参数：ufast ufl_line，指定被设置的中断线号
//返回：true=成功，false=失败
//      本函数移植敏感
//-----------------------------------------------------------------------------
bool_t int_setto_real(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].sync_event != NULL)
        return false;     //有线程在等待这个中断，不能设为实时中断
    tg_int_table[tg_int_lookup_table[ufl_line]].int_type = cn_real;    //中断线类型
    tg_int_table[tg_int_lookup_table[ufl_line]].enable_nest = false;   //本实现不支持实时中断嵌套
    tg_int_global.property_bitmap[ufl_line/cn_cpu_bits]
            |= 1<<(ufl_line % cn_cpu_bits);   //设置位图
    return true;
}

//----使能中断嵌套-------------------------------------------------------------
//功能: 使能一个中断源被抢占，使能后，相应的中断服务期间，可能会被别的中断抢占。
//      本函数不分实时中断还是异步信号，与具体的实现有关，移植者应该根据硬件特性
//      和应用需求，确定是否允许嵌套。
//      嵌套发生在实时中断之间或者异步信号之间。实时中断永远可以打断异步信号，这
//      不算嵌套。
//      对于三星系列的ARM芯片中断管理器的特点，对于实时中断的处理，有三种方式:
//      1、用fiq实现实时中断，INTOFFSET1和INTOFFSET2寄存器提供了当前正在服务的
//         中断号，但该两个寄存器在fiq中无效，如果实时中断用fiq实现，则需要逐位
//         检查被设为实时中断的中断号在INTPND1和INTPND1寄存器中的值，才能确定被
//         服务的中断号，这个过程可能非常漫长。这种情况能够实现实时中断嵌套，但
//         却完全失去了实时中断的设计初衷，djyos for s3c2416不采取这种方式。
//      2、有一个特殊情况是，系统只需要一个实时中断，其他全部设为异步信号，这种
//         情况，用fiq实现实时中断是最为理想的，当然只有一个中断，嵌套也就无从
//         谈起了。
//      3、实时中断和异步信号都是用irq方式实现，cpsr的I位用做trunk开关，异步信号
//         没有独立开关，是通过把所有的属于异步信号的中断号的开关全部关掉来模拟
//         异步信号开关的。这种情况是不能实现实时中断嵌套的。为什么呢?arm被设计
//         为，只要I位为0，新的中断就可以抢占正在服务的中断，也就意味着，异步信
//         号抢占实时中断，在硬件上是允许的。实时中断要实现嵌套，须先关掉异步信
//         号，再设置I=0，如果实时中断响应后，在异步信号被关掉之前，紧接着有异步
//         信号发生的话，该irq请求就会发出，随后关掉异步信号也没用，总中断一旦被
//         允许，该异步信号就会抢占实时中断。这种抢占，不仅把"辈分"搞乱，还会引
//         发严重问题，甚至使系统崩溃。如果抢占实时中断的异步信号，服务期间发生
//         了线程切换，把fiq服务的栈，当作pg_event_running的栈。因此，djyos for
//         2416版本不支持实时中断嵌套。
//参数: ufl_line，被操作的中断线
//返回: 无
//-----------------------------------------------------------------------------
bool_t int_enable_nest(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_global.property_bitmap[ufl_line/cn_cpu_bits]
                & (1<<(ufl_line % cn_cpu_bits)))
        return false;       //本实现不支持实时中断嵌套
    else
        tg_int_table[tg_int_lookup_table[ufl_line]].enable_nest = true;
    return true;
}

//----禁止中断嵌套-------------------------------------------------------------
//功能: 清除某中断号的中断嵌套使能。
//参数: ufl_line，被操作的中断线
//返回: 无
//-----------------------------------------------------------------------------
bool_t int_disable_nest(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    tg_int_table[tg_int_lookup_table[ufl_line]].enable_nest = false;
	return true;
}

//----设定嵌套优先级-----------------------------------------------------------
//功能: 设定指定中断线的嵌套优先级，本函数严重依赖硬件功能。如果硬件不支持，可
//      保持空函数。
//参数：ufast ufl_line，指定被设置的中断线号
//返回：无
//注: 本函数移植敏感
//-----------------------------------------------------------------------------
bool_t int_set_prio(ufast_t ufl_line,u32 prio)
{
    return false;
}

//----初始化中断硬件相关部分---------------------------------------------------
//功能: 如标题
//参数: 无
//返回: 无
//注: 移植敏感，与硬件相关，也与软件策略有关
//-----------------------------------------------------------------------------
void __int_init_hard(void)
{
    int_disable_arm_int();     //禁止中断

    pg_int_reg->INTMSK = cn_int_msk_all_line;
    //中断管理器的F位总是禁止,2440的中断管理器有缺陷，除非只允许一
    //个中断配置为fiq，否则fiq检出中断线的过程使得fiq实际比irq慢，故不使用fiq，
    //不管异步信号还是实施中断，均使用irq实现。
    pg_int_reg->INTMOD=0;    //设置所有中断线为IRQ中断
    engine_irq = __int_engine_all;
    // irq_bottom = __int_engine_all;
}
//----初始化中断---------------------------------------------------------------
//功能：初始化中断硬件,初始化中断线数据结构
//      2.异步信号保持禁止,它会在虚拟机启动引擎中打开.
//      3.总中断允许，
//      用户初始化过程应该遵守如下规则:
//      1.系统开始时就已经禁止所有异步信号,用户初始化时无须担心异步信号发生.
//      2.初始化过程中如果需要操作总中断/实时中断/异步信号,应该成对使用.禁止使
//        异步信号实际处于允许状态(即异步和总中断开关同时允许).
//      3.可以操作中断线,比如连接、允许、禁止等,但应该成对使用.
//      4.建议使用save/restore函数对,不要使用enable/disable函数对.
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void int_init(void)
{
    ufast_t ufl_line;
    __int_init_hard();
    __int_echo_all_line();
    for(ufl_line=0;ufl_line <= cn_int_line_last;ufl_line++)
    {
        tg_int_lookup_table[ufl_line] = cn_limit_ufast;
    }
    for(ufl_line = 0; ufl_line < ufg_int_used_num; ufl_line++)
    {
        tg_int_lookup_table[tg_int_used[ufl_line]] = ufl_line;
        tg_int_table[ufl_line].en_counter = 1;               //禁止中断,计数为1
        tg_int_table[ufl_line].int_type = cn_asyn_signal;        //设为异步信号
        //所有中断函数指针指向空函数
        tg_int_table[ufl_line].ISR = NULL_func;
        tg_int_table[ufl_line].sync_event = NULL;                //同步事件空
        tg_int_table[ufl_line].my_evtt_id = cn_invalid_evtt_id;  //不弹出事件
    }

    for(ufl_line=0; ufl_line < cn_int_bits_words; ufl_line++)
    {
        //属性位图清零,全部置为异步信号方式
        tg_int_global.property_bitmap[ufl_line] = 0;
        //中断使能位图清0,全部处于禁止状态
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }
    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.nest_real = 0;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;   //异步信号计数
    int_cut_asyn_signal();
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;   //总中断计数
    int_contact_trunk();                //接通总中断开关								  
}

//----总中断引擎---------------------------------------------------------------
//功能：有些系统，在中断向量表部分难于区分实时中断还是异步信号的，或者不希望在汇
//      编阶段使用过于复杂的代码的，比如2416、2440等，则在汇编阶段获取中断号后，
//      直接调用本函数，由本函数再区别调用异步信号引擎或实时中断引擎。
//      像cortex-m3、omapl138这样，在中断向量表部分就可以区别实时中断还是异步信
//      号，则无须提供本函数，而是在汇编部分直接调用相应的引擎。
//参数：ufast ufl_line，响应的中断线号
//返回：无
//-----------------------------------------------------------------------------

void __int_engine_all(ufast_t ufl_line)
{
    if(tg_int_table[ufl_line].int_type == cn_real)
        __int_engine_real(ufl_line);                //是实时中断
    else
        __int_engine_asyn_signal(ufl_line);         //是异步信号
}

//----实时中断引擎-------------------------------------------------------------
//功能：响应实时中断，根据中断号调用用户ISR
//参数：ufast ufl_line，响应的中断线号
//返回：无
//-----------------------------------------------------------------------------
void __int_engine_real(ufast_t ufl_line)
{
//    tg_int_global.nest_real++;
//    tg_int_global.en_trunk = false;
//    tg_int_global.en_trunk_counter++;

    //本if语句与移植敏感，跟cpu的中断管理器的几个特性有关:
    //1、异步信号是否有独立的开关，例如cortex-m3、omapl138等是有的，2440、2416、
    //   2410等是没有的。如果没有独立开关，则在打开总中断前须断掉异步信号线开关
    //2、异步信号和实时中断都处于开启状态的情况下，异步信号是否可能抢占实时中断。
	//   这种情况实际上是不能实现实时中断嵌套的。
	//3、实时中断响应后，是否自动关闭实时中断
    //4、该具体实现是否支持实时中断嵌套
    //5、本实现支持实时中断嵌套，故整个注释掉
//    if(tg_int_table[ufl_line].enable_nest == true)
//    {
//        real_save_asyn = int_low_atom_start();
//        int_contact_trunk();
//    }
    int_echo_line(ufl_line);        //中断应答
    if(tg_int_table[ufl_line].ISR != NULL)
        tg_int_table[ufl_line].ISR(ufl_line);  //调用用户中断函数


    //如果该实现支持实时中断嵌套，启用以下4句
//    tg_int_global.nest_real--;
//    tg_int_global.en_trunk_counter--;
//    if(tg_int_global.en_trunk_counter == 0)
//        tg_int_global.en_trunk = true;

}

//----异步事件中断引擎---------------------------------------------------------
//功能：响应异步信号，根据中断号调用用户ISR，随后弹出中断线控制块的my_evtt_id
//      成员指定的事件类型，最后在返回前查看是否需要做上下文切换，如需要则切换
//      之。
//参数：ufast ufl_line，响应的中断线号
//返回：无
//-----------------------------------------------------------------------------
void __int_engine_asyn_signal(ufast_t ufl_line)
{
    struct event_ECB *event;
    u32 isr_result;
    tg_int_global.en_asyn_signal_counter = 1;
    bg_schedule_enable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.nest_asyn_signal++;
    int_disable_asyn_line(ufl_line);	//不禁止，可能会形成无限循环
    if(tg_int_table[ufl_line].enable_nest == true)  //允许嵌套的情况
        int_contact_asyn_signal();
    else
	    int_cut_asyn_signal();
    int_echo_line(ufl_line);        //中断应答,
    int_contact_trunk();
    if(tg_int_table[ufl_line].ISR != NULL)
        isr_result = tg_int_table[ufl_line].ISR(ufl_line);
    if(tg_int_table[ufl_line].enable_nest == true)  //支持嵌套的情况
        int_cut_asyn_signal();

    event = tg_int_table[ufl_line].sync_event;
    if(event != NULL)   //看同步指针中有没有事件(注：单个事件，不是队列)
    {
        event->last_status.all = event->event_status.all;
        event->event_status.bit.wait_asyn_signal = 0;
        event->event_result = isr_result;
        __djy_event_ready(event);   //把该事件放到ready队列
        tg_int_table[ufl_line].sync_event = NULL;   //解除同步
    }
    if(tg_int_table[ufl_line].my_evtt_id != cn_invalid_evtt_id)
        djy_event_pop(tg_int_table[ufl_line].my_evtt_id,enum_para_async,NULL,0,
                                    &ufl_line,sizeof(ufl_line),0,0);
    tg_int_global.nest_asyn_signal--;
    if(tg_int_global.nest_asyn_signal == 0)
    {
//        tg_int_global.en_asyn_signal = true;
        tg_int_global.en_asyn_signal_counter = 0;
        if(pg_event_ready != pg_event_running)
            __djy_schedule_asyn_signal();       //执行中断内调度
        bg_schedule_enable = true;
    }
    int_cut_trunk();
    int_enable_asyn_line(ufl_line);
    int_contact_asyn_signal();  //前面关闭了，现在打开
}

