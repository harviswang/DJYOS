//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 调度器
//作者：lst
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

extern u32  u32g_os_ticks;             //操作系统运行ticks数
volatile u32    u32g_delay_10uS = 40;
static u32 u32g_ns_of_u32for; //for(i=j;i>0;i--);语句，i和j都是u32型，每循环纳秒数
static u32 u32g_ns_of_u16for; //for(i=j;i>0;i--);语句，i和j都是u16型，每循环纳秒数
static u32 u32g_ns_of_u8for;  //for(i=j;i>0;i--);语句，i和j都是u8型，每循环纳秒数

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
#if cfg_stack_fill != 0
    len = m_check_size(result);
    memset(result,cfg_stack_fill,len);
#endif

    //看实际分配了多少内存，djyos内存分配使用块相联策略，如果分配的内存量大于
    //申请量，可以保证其实际尺寸是对齐的。之所以注释掉，是因为当len大于申请量时，
    //对齐只是实际结果，而不是内存管理的规定动作，如果不注释掉，就要求内存管理
    //模块必须提供对齐的结果，对模块独立性是不利的。
//    len = m_check_size(result);
    result->stack_top = (u32*)((ptu32_t)result+len); //栈顶地址，移植敏感
    result->next = NULL;
    result->stack_size = len - sizeof(struct thread_vm); //保存栈深度
    result->host_vm = NULL;
    //复位虚拟机并重置线程
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

static volatile ufast_t uf_delay_counter;
u32 set_delay_isr(ufast_t timer_int_line)
{
    uf_delay_counter++;
    return 0;
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
    u32 counter,u32_fors=10000,temp,phase_ns;
    uint16_t u16_fors=10000,clksum;
    u8  u8_fors1 = 100;
    u8  u8_fors2 = 99;
    volatile u32 u32loops;
    volatile uint16_t u16loops;
    volatile u8  u8loops1,u8loops2;

    timer_set_clk_source(3,0);         //pclk预分频数的1/2分频
    timer_set_precale(1,7);            //时钟输入频率pclk的8分频
    timer_set_type(3,1);                //自动加载连续工作
    phase_ns = 10E8/cn_timer_clk * 16;  //每脉冲的ns数
    clksum = (uint16_t)(cn_timer_clk/16000);     //1000uS，减计数
    int_isr_connect(cn_int_line_timer3,set_delay_isr);
    int_setto_real(cn_int_line_timer3);          //timer3中断被设为实时中断
    int_enable_real_line(cn_int_line_timer3);
    while(1)  //测量32位变量循环时间(nS)
    {
        timer_set_counter(3,clksum);
        uf_delay_counter = 0;
        timer_reload(3);
        timer_start(3);
        for(u32loops=u32_fors;u32loops>0;u32loops--); //循环u32_fors次
        timer_stop(3);
        counter = timer_read(3);                  //读取循环u32_fors次所需时间
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65535;
                u32_fors = u32_fors / (temp / 65535 +1);
            }
        }else
            break;      //循环过程中没有发生中断，满足要求。
    };
    counter = clksum - counter;    //取实际脉冲数。
    u32g_ns_of_u32for = (uint64_t)counter * phase_ns /u32_fors;//防溢出，用64位

    while(1)  //测量16位变量循环时间(nS)
    {
        timer_set_counter(3,clksum);
        uf_delay_counter = 0;
        timer_reload(3);
        timer_start(3);
        for(u16loops=u16_fors;u16loops>0;u16loops--); //循环u16_fors次
        timer_stop(3);
        counter = timer_read(3);                  //读取循环u32_fors次所需时间
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65535;
                u16_fors = u16_fors / (temp / 65535 +1);
            }
        }else
            break;      //循环过程中没有发生中断，满足要求。
    };
    counter = clksum - counter;    //取实际脉冲数。
    u32g_ns_of_u16for = (uint64_t)counter * phase_ns /u16_fors;//防溢出，用64位

    while(1)  //测量8位变量循环时间(nS)
    {
        timer_set_counter(3,clksum);
        uf_delay_counter = 0;
        timer_reload(3);
        timer_start(3);
        for(u8loops1 = u8_fors1; u8loops1 > 0; u8loops1--)
            for(u8loops2=u8_fors2;u8loops2>0;u8loops2--); //循环u8_fors次
        timer_stop(3);
        counter = timer_read(3);                  //读取循环u8_fors次所需时间
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65535;
                u8_fors1 = u8_fors1 / (temp / 65535 +1);
            }
        }else
            break;      //循环过程中没有发生中断，满足要求。
    };
    counter = clksum - counter;    //取实际脉冲数。
    //防溢出，用64位
    u32g_ns_of_u8for = (uint64_t)counter * phase_ns /(u8_fors1*(u8_fors2+1));

    u32g_delay_10uS = 10000/u32g_ns_of_u32for;
}

u32 __djy_isr_tick(ufast_t line)
{
    djy_isr_tick();
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
	int_cut_line(cn_int_line_timer3);
    //连接timer3作为tick中断
    int_isr_connect(cn_int_line_timer3,__djy_isr_tick);
    int_setto_asyn_signal(cn_int_line_timer3);          //tick中断被设为异步信号
    //以下设置定时器参数，需与port_config.h中cn_tick_us、cn_tick_hz和
    //cn_fine_ns、cn_fine_hz的定义一致
    timer_set_clk_source(3,0);          //pclk预分频数的1/2分频
    timer_set_precale(1,cn_timer_clk/cn_fine_hz/2 -1);
    //以上把定时器输入时钟的频率设为cn_fine_hz
    //timer_set_counter(3,(uint16_t)((u32)cn_tick_us*1000/cn_fine_ns));
    timer_set_counter(3, cn_fine_hz/cn_tick_hz);
    timer_set_type(3,1);                //设置tick定时器连续工作
    timer_reload(3);                    //重载定时值
    timer_start(3);                     //启动定时器
    int_restore_asyn_line(cn_int_line_timer3);//启动tick中断S
}

//----读取当前ticks-------------------------------------------------------------
//功能：读取操作系统时钟
//      u32g_os_ticks 为32位无符号变量，ARM是32位机，可以直接读取，非32位系统中
//      读取 u32g_os_ticks 需要超过1个周期,因此访问SysTimer时需要关中断。
//参数：无
//返回：当前时钟
//-----------------------------------------------------------------------------
u32 djy_get_time_tick(void)
{
    u32 time;
#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_save_asyn_line(cn_int_line_timer3);
#endif

    time = u32g_os_ticks;

#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_restore_asyn_line(cn_int_line_timer3);
#endif
    return time;
}

//----读取当前时间(uS)---------------------------------------------------------
//功能：读取操作系统时钟
//      u32g_os_ticks 为32位无符号变量，ARM是32位机，可以直接读取，非32位系统中
//      读取 u32g_os_ticks 需要超过1个周期,因此访问SysTimer时需要关中断。
//参数：无
//返回：当前时钟
//-----------------------------------------------------------------------------
u32 djy_get_time(void)
{
    u32 time;
#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_save_asyn_line(cn_int_line_timer3);
#endif

    time = u32g_os_ticks*cn_tick_us;

#if (32 > cn_cpu_bits)
    //若处理器字长不是32位,需要多个周期才能读取os_ticks,该过程不能被时钟中断打断.
    int_restore_asyn_line(cn_int_line_timer3);
#endif
    return time;
}

//----读取精密时间-----------------------------------------------------------
//功能：读取操作系统精密时钟,时间单位在配置文件中设置,依硬件不同而不同,一般设为
//      tick的百分之一或者千分之一,44b0x中设为千分之一.结果是从上次ticks改变到
//      现在的时间间隔/
//参数：无
//返回：时间值
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
u32 djy_get_fine_time(void)
{
    //return (((u32)cn_tick_us*1000/cn_fine_ns)-pg_timer_reg->TCNTO3) *cn_fine_ns;

    u32 time1,time2;
    atom_low_t atom_low;
    atom_low = int_low_atom_start();

    time1 = pg_timer_reg->TCNTO3;
//    if(pg_timer_reg->INTCTLSTAT & 0x02)
//        time2 = (u32g_os_ticks+1)*cn_tick_us;
//    else
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
