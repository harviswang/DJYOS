//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 调度器
//作者:  罗侍田.
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者:  罗侍田.
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

extern uint32_t  u32g_os_ticks;             //操作系统运行ticks数
volatile uint32_t    u32g_delay_10uS = 40;
static u32 u32g_ns_of_u32for; //for(i=j;i>0;i--);语句，i和j都是u32型，每循环纳秒数
static u32 u32g_ns_of_u16for; //for(i=j;i>0;i--);语句，i和j都是u16型，每循环纳秒数
static u32 u32g_ns_of_u8for;  //for(i=j;i>0;i--);语句，i和j都是u8型，每循环纳秒数

//tick选择t64-0的t12

//----创建线程-----------------------------------------------------------------
//功能：为事件类型创建伪虚拟机，初始化上下文环境，安装执行函数，构成完整线程
//参数：evtt_id，待创建的线程所服务的事件类型id
//返回：新创建的线程的虚拟机指针
//注: 移植敏感函数
//-----------------------------------------------------------------------------
struct  thread_vm *__create_thread(struct  event_type *evtt)
{
    struct  thread_vm  *result;
    ptu32_t  len;

    //计算虚拟机栈:线程+最大单个api需求的栈
    len = evtt->stack_size+cn_kernel_stack+sizeof(struct  thread_vm);
    //栈顶需要对齐，malloc函数能保证栈底是对齐的，对齐长度可以使栈顶对齐
    len = align_up_sys(len);
    result=(struct  thread_vm  *)m_malloc_gbl(len,0); //申请内存，移植敏感
    if(result==NULL)
    {
        djy_error_login(enum_mem_tried,NULL);   //内存不足，返回错误
        return result;
    }

    //看实际分配了多少内存，djyos内存分配使用块相联策略，如果分配的内存量大于
    //申请量，可以保证其实际尺寸是对齐的。之所以注释掉，是因为当len大于申请量时，
    //对齐只是实际结果，而不是内存管理的规定动作，如果不注释掉，就要求内存管理
    //模块必须提供对齐的结果，对模块独立性是不利的。
#if cfg_stack_fill != 0
    len = m_check_size(result);
    memset(result,cfg_stack_fill,len);
#endif
    result->stack_top = (uint32_t*)((ptu32_t)result+len); //栈顶地址，移植敏感
    result->next = NULL;
    result->stack_size = len - sizeof(struct thread_vm); //保存栈深度
    result->host_vm = NULL;
    //复位虚拟机并重置线程
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----设置指令延时常数---------------------------------------------------------
//功能: 设置指令延时常数,使y_delay_10us函数的延时常数=10uS，不管用何种编译器和
//      编译优化选项
//参数：无
//返回: 无
//备注: 本函数移植敏感
//-----------------------------------------------------------------------------
void __djy_set_delay(void)
{
    uint32_t u32_fors;
    uint32_t u16_fors;
    uint32_t  u8_fors;
    volatile uint32_t u32loops;
    volatile uint16_t u16loops;
    volatile uint8_t  u8loops1,u8loops2;

    timer_stop(0,0);                    //停止定时器0
    timer_clear(0,0);                   //清除当前计数值
    timer_set_clksrc(0,0,0);            //设置晶振时钟作为定时器时钟
    timer_counter_len_set(0,1);         //设置为双32位计数
    timer_set_mode(0,0,2);              //设为周期性计数
    timer_set_Period(0,0,0xffffffff);   //定时周期值设为最大
//    pg_timer0_reg->TCR |= (1<<10);      //设置t64-0的t12为读清零模式
    timer_start(0,0);
    for(u32loops=10000;u32loops>0;u32loops--); //用32位变量执行循环
    u32_fors = timer_read(0,0);
    timer_stop(0,0);                    //停止定时器0
    timer_clear(0,0);                   //清除当前计数值
    timer_start(0,0);
    for(u16loops=10000;u16loops>0;u16loops--); //用16位变量执行循环
    u16_fors = timer_read(0,0);
    timer_stop(0,0);                    //停止定时器0
    timer_clear(0,0);                   //清除当前计数值
    timer_start(0,0);
    for(u8loops1 = 100; u8loops1 > 0; u8loops1--)
        for(u8loops2=99;u8loops2>0;u8loops2--); //用8位变量执行循环
    u8_fors = timer_read(0,0);
    timer_stop(0,0);                    //停止定时器0

    u32g_ns_of_u32for = ((u64)cn_fine_us * u32_fors>>16) / 10;
    u32g_ns_of_u16for = ((u64)cn_fine_us * u16_fors>>16) / 10;
    u32g_ns_of_u8for  = ((u64)cn_fine_us * u8_fors>>16) / 10;

    u32g_delay_10uS = 10000/u32g_ns_of_u32for;
//    pg_timer0_reg->TCR &= ~((u32)1<<10);       //清除t64-0的t12的读清零模式
}

uint32_t __djy_isr_tick(ufast_t line)
{
    djy_isr_tick();
    g_ptTimer0Reg->INTCTLSTAT |= 2;
    return 0;
}

//----初始化tick---------------------------------------------------------------
//功能: 初始化定时器,并连接tick中断函数,启动定时器.
//参数: 无
//返回: 无
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
void __djy_init_tick(void)
{
    //以下设置定时器参数，需与port_config.h中cn_tick_ms、cn_tick_hz和
    //cn_fine_us、cn_fine_hz的定义一致
    timer_stop(0,0);                    //停止定时器0
    timer_clear(0,0);                   //清除当前计数值
    timer_set_clksrc(0,0,0);            //设置晶振时钟作为定时器时钟
    timer_counter_len_set(0,1);         //设置为双32位计数
    timer_set_Period(0,0,cn_fine_hz/cn_tick_hz);   //t12周期值设为tick周期
    timer_int_ctrl(0,PRDINTEN12,true);  //使能周期中断
    timer_int_ctrl(0,EVTINTEN12,false); //禁止事件捕获中断
    timer_set_mode(0,0,2);              //设为周期性计数
    //连接t64-0的t12作为tick中断
    int_isr_connect(cn_int_line_t64p0_tint12,__djy_isr_tick);
    int_setto_asyn_signal(cn_int_line_t64p0_tint12);    //tick中断被设为异步信号
    int_restore_asyn_line(cn_int_line_t64p0_tint12);    //启动tick中断
    timer_start(0,0);
}

//----读取当前ticks-------------------------------------------------------------
//功能：读取操作系统时钟
//      u32g_os_ticks 为32位无符号变量，ARM是32位机，可以直接读取，非32位系统中
//      读取 u32g_os_ticks 需要超过1个周期,因此访问SysTimer时需要关中断。
//参数：无
//返回：当前时钟
//-----------------------------------------------------------------------------
uint32_t djy_get_time_tick(void)
{
    uint32_t time;
#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_save_asyn_line(cn_int_line_t64p0_tint12);
#endif

    time = u32g_os_ticks;

#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_restore_asyn_line(cn_int_line_t64p0_tint12);
#endif
    return time;
}

//----读取当前ticks-------------------------------------------------------------
//功能：读取操作系统时钟
//      u32g_os_ticks 为32位无符号变量，ARM是32位机，可以直接读取，非32位系统中
//      读取 u32g_os_ticks 需要超过1个周期,因此访问SysTimer时需要关中断。
//参数：无
//返回：当前时钟
//-----------------------------------------------------------------------------
uint32_t djy_get_time(void)
{
    uint32_t time;
#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_save_asyn_line(cn_int_line_t64p0_tint12);
#endif

    time = u32g_os_ticks*cn_tick_us;

#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_restore_asyn_line(cn_int_line_t64p0_tint12);
#endif
    return time;
}

//----读取精密时间-----------------------------------------------------------
//功能：读取精密时钟,单位uS，达4294.967296秒回绕到0.
//参数：无
//返回：时间值
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
uint32_t djy_get_fine_time(void)
{
    u32 time1,time2;
    atom_low_t atom_low;
    atom_low = int_low_atom_start();
    
    time1 = g_ptTimer0Reg->TIM12;
    if(g_ptTimer0Reg->INTCTLSTAT & 0x02)
        time2 = (u32g_os_ticks+1)*cn_tick_us;
    else
        time2 = u32g_os_ticks*cn_tick_us + (time1*cn_fine_us >>16);
    
    int_low_atom_end(atom_low);
    return time2;
}

//----10微秒级延时-------------------------------------------------------------
//功能：利用循环实现的微秒分辨率延时，调用__djy_set_delay函数后才能使用本函数，
//      否则在不同优化级别和不同编译器下,延时数不同.
//参数：time，延时时间，单位为10微秒,最多655毫秒。
//返回：无
//注意：不建议使用此函数做太长延时，长延时请使用函数 djy_timer_sync,
//-----------------------------------------------------------------------------
void djy_delay_10us(volatile u16 time)
{
    volatile u32 i;
    for(; time > 0; time--)
        for(i = u32g_delay_10uS; i >0 ; i--);
}

//----取for循环延时常数--------------------------------------------------------
//功能: 当代码使用for(i=j;i>0;i--);循环来实现短时间延时时，可通过本函数获取单个
//      循环的执行时间。根据循环变量i的类型，使用:
//          i = djy_get_for_time(sizeof(i));
//      返回使用i做循环变量是for循环的执行时间。用下列代码实现延时x纳秒
//          i= x/i;
//          for( ; i > 0; i--);
//      注意:有些编译器会把空for循环当作无用代码删掉，故循环变量须volatile修饰。
//参数: var_len，变量长度(字节数)
//返回: 根据var_len值，返回u32g_ns_of_u32/16/8for中的一个。
u32 djy_get_for_time(ufast_t var_len)
{
    if(var_len == 1)
        return u32g_ns_of_u8for;
    else if(var_len == 2)
        return u32g_ns_of_u16for;
    else 
        return u32g_ns_of_u32for;
}


