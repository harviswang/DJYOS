//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:核心模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述:事件类型、事件管理以及多事件调度相关的代码全在这里了。
//其他说明:
//修订历史://5. 日期: 2011-07-19
//   作者:  罗侍田.
//   新版本号: V1.3.0
//   修改说明:解决一个可能导致内存泄漏的bug。该bug发作条件：参数尺寸大于32字节，
//   且在调用djy_event_pop时要求动态分配参数所需内存空间，djy_task_completed和
//   djy_event_exit函数释放参数时将不释放该动态分配的内存。
//   修改方法是在struct  para_record结构中增加成员dynamic_mem，用于表示参数是否
//   动态分配得到的，如是，则在djy_task_completed和djy_event_exit函数释放参数时
//   调用m_free释放之。
//4. 日期: 2009-12-15
//   作者:  罗侍田.
//   新版本号: V1.3.0
//   修改说明:
//   1、djy_evtt_regist中判断同名事件类型时，无论该事件类型控制块有效否，都调用
//      strcmp函数比较，效率较低，改为先判断是否空，然后才比较。
//   2、原来在djy_evtt_regist中因不能创建虚拟机导致登记失败时，没有处理事件类型
//      控制块，导致仍然处于registered=1的状态。
//3. 日期: 2009-10-05
//   作者:  罗侍田.
//   新版本号: V1.2.0
//   修改说明:
//   1、不再以tg_int_global.nest_asyn_signal==0作为是否允许调度的依据，这会导致
//      中断模块和调度模块间隐含的耦合关系。
//      以全局变量bg_schedule_enable作为是否允许调度的标志
//   2、改掉了一些gcc（以及c99）才支持的特征，以适应更广泛的编译器
//   3、前面的版本并无实现djy_evtt_unregist的全部功能，本版本补全之。
//   4、__djy_resume_delay函数中这一行：
//      pg_event_delay = pg_event_delay->previous;改为
//      pg_event_delay = pg_event_delay->next;
//      否则会导致闹钟同步队列错乱
//   5、原版本中,__turnto_context函数身兼2职，内核初始化完成后，首次调用本函数还
//      有启动线程调度的功能，这在arm中是可以的，但不确保其他cpu也可以，例如cm3
//      上就不行，先把这个功能分离出来，增加一个函数__asm_start_thread。但在arm
//      版本中用宏定义：#define __asm_start_thread __asm_turnto_context
//   6、int_restore_asyn_signal函数中，先执行__int_contact_asyn_signal打开中断，
//      然后再执行：
//            if(pg_event_running != pg_event_ready)
//                __djy_schedule();
//      在打开中断与__djy_schedule函数重新关闭中断之间，如果有中断抢占的话，可能
//      导致严重错误，int_restore_trunk函数也有同样的问题。
//      新版本中，__djy_schedule函数调用__asm_switch_context时才打开中断，杜绝了
//      上述问题，__djy_schedule函数整个都是在关闭中断的条件下执行的。
//2. 日期: 2009-03-18
//   作者:  罗侍田.
//   新版本号: V1.1.0
//   修改说明:
//   1、根据无为小僧（曾波）发现的隐患，事件类型的名字如果用指针索引的话，如果
//      用户调用y_evtt_regist时使用的是局部数组，将存在保存名字的内存被释放的危
//      险。改为数组存储，最长31字符，在struct event_type结构中把evtt_name由指针
//      改为数组，修改涉及下列函数：
//          __djy_init_sys、y_evtt_regist、y_evtt_unregist、y_get_evtt_id
//   2、在y_evtt_unregist函数中释放虚拟机处发现一个bug，存在访问非法内存的危险，
//      改之。
//   3、y_evtt_done_sync函数中处理同步队列处存在严重bug，可能导致访问非法内存，
//      且使同步队列混乱，改之。
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

struct process_vm *pg_my_process;
//为cn_events_limit条事件控制块分配内存
struct event_ECB tg_ECB_table[cn_events_limit];
static struct para_record  tg_event_para_table[cn_paras_limit];
//为cn_evtts_limit个事件类型控制块分配内存
struct event_type tg_evtt_table[cn_evtts_limit];
static struct  event_ECB  *pg_event_free; //空闲链表头,不排序
static struct  para_record  *pg_para_free; //空闲链表头,不排序
//轮转调度时间片，0表示禁止轮转调度，默认1，RRS = "round robin scheduling"缩写。
static u32  u32g_RRS_slice = 1;

struct  event_ECB  *pg_event_ready;      //就绪队列头
struct  event_ECB  *pg_event_running;    //当前正在执行的事件
struct  event_ECB  *pg_event_delay;      //闹钟同步队列表头
u32 u32g_running_start_time;        //当前运行中事件的开始执行时间.
u32  u32g_os_ticks;            //操作系统运行ticks数
bool_t bg_schedule_enable;     //系统当前运行状态是否允许调度

#if(cfg_debug_info == 1)

void debug_scan(void)
{
    u32 *para;
    u32 pl_ecb;
    u32 time1;
    while(1)
    {
        para = (u32*)djy_get_event_para(NULL); 
        if(para != NULL)
        {
            switch(*para)
            {
                case cn_db_info_event:
                {
                    printf_ng("事件号  所属类型  使用   优先级 CPU  内存\n\r");
                    for(pl_ecb = 0; pl_ecb < cn_events_limit; pl_ecb++)
                    {
                        
                        if(tg_ECB_table[pl_ecb].previous != 
                                        (struct  event_ECB*)&pg_event_free)
                        {
                            printf_ng("%05d   %05d     ",pl_ecb,tg_ECB_table[pl_ecb].evtt_id);
                            printf_ng("使用中 ");
                            printf_ng("%03d    ",tg_ECB_table[pl_ecb].prio);
                            time1 = tg_ECB_table[pl_ecb].consumed_time_second/10000;
                            printf_ng("%2d\%%  %8x",time1,tg_ECB_table[pl_ecb].vm->stack_size);
                        }
                        else
                        {
                            printf_ng("%5d             ",pl_ecb);
                            printf_ng("空闲");
                        }
                        printf_ng("\n\r");
                    }
                }break;
                case cn_db_info_evtt:
                {
                    printf_ng("类型号  登记   优先级 处理函数    内存   名字\n\r");
                    for(pl_ecb = 0; pl_ecb < cn_evtts_limit; pl_ecb++)
                    {
                        
                        if(tg_evtt_table[pl_ecb].property.registered)
                        {
                            printf_ng("%05d   ",pl_ecb);
                            printf_ng("已注册 ");
                            printf_ng("%03d    ",tg_evtt_table[pl_ecb].default_prio);
                            printf_ng("%8x  %8x ",
                                        tg_evtt_table[pl_ecb].thread_routine,
                                        tg_evtt_table[pl_ecb].stack_size);
                            if(tg_evtt_table[pl_ecb].evtt_name[0] != '\0')
                                printf_ng("%s",&tg_evtt_table[pl_ecb].evtt_name);
                            else
                                printf_ng("无名");
                        }
                        else
                        {
                            printf_ng("%5d   ",pl_ecb);
                            printf_ng("空闲");
                        }
                        printf_ng("\n\r");
                    }
                    
                }break;
                default:break;
            }
        }
        djy_task_completed(0);
        for(pl_ecb = 0; pl_ecb < cn_events_limit; pl_ecb++)
        {
            tg_ECB_table[pl_ecb].consumed_time_second =
                              tg_ECB_table[pl_ecb].consumed_time
                            - tg_ECB_table[pl_ecb].consumed_time_record;
            tg_ECB_table[pl_ecb].consumed_time_record = 
                            tg_ECB_table[pl_ecb].consumed_time;
        }
        
        djy_wait_evtt_pop(djy_my_evtt_id(),1,1000*mS);
    }
}

//----初始化调试信息模块-------------------------------------------------------
//功能: 创建调试信息事件类型并启动之
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void init_debug_info(ptu32_t para)
{
    u16 evtt_debug;
    evtt_debug = djy_evtt_regist(enum_correlative,1,0,1,
                                 debug_scan,100,"debug_info");
    if(evtt_debug == cn_invalid_evtt_id)
        return;
    djy_event_pop(evtt_debug,enum_para_async,NULL,0,NULL,0,0,0);
}
#endif

//----实时串长度---------------------------------------------------------------
//功能: strlen函数的执行时间不可控，不宜在实时环境中使用。
//参数: s，被测字符串
//      over，最长检测长度,over=0则不限长度，与strlen函数无异。
//返回: 串长度，在over范围内没有检测到串结束则返回-1
//-----------------------------------------------------------------------------
sint32_t strnlen(const char *s,s32 maxlen)
{
    u32 len;
    if(maxlen == 0)
        return strlen(s);
    else
    {
        for(len = 0; len < maxlen; len++)
        {
            if(s[len] =='\0')
            {
                return len;
            }
        }
        return cn_limit_uint32;
    }
}

//以下为多任务管理函数

//----tick中断-----------------------------------------------------------------
//功能：为操作系统产生时钟源，并比较闹钟同步队列中的事件到期时间，如果有闹铃时间到的
//      事件，就把他们激活到事件就绪队列中。如果时间片轮转调度被允许，还要看是否要
//      执行轮转。ticks被设置为异步信号。
//参数：line，ticks中断的中断线号，实际上不用。
//返回：无
//-----------------------------------------------------------------------------
void  djy_isr_tick(void)
{
    struct  event_ECB *pl_ecb,*pl_ecbp,*pl_ecbn;
    u32g_os_ticks++;    //系统时钟,4294967296次后溢出,将环绕

    if(pg_event_delay != NULL)
    {
        pl_ecb = pg_event_delay;
        while(1)
        {
            if(pl_ecb->delay_end_tick == u32g_os_ticks)
            {
                //事件在某同步队列中，应该从该队列取出
                if(pl_ecb->sync_head != NULL)   
                {
                    if(*(pl_ecb->sync_head) == pl_ecb)//本事件位于队列头部
                    {
                        if(pl_ecb->multi_next == pl_ecb)//该队列中只有一个事件
                        {
                            *(pl_ecb->sync_head) = NULL;
                            
                        	pl_ecb->multi_next = NULL;
                            pl_ecb->multi_previous = NULL;
                        }else                   //该同步队列中有多个事件
                        {
                            //头指针指向下一个事件
                            *pl_ecb->sync_head = pl_ecb->multi_next;
                            pl_ecb->multi_previous->multi_next
                                                = pl_ecb->multi_next;
                            pl_ecb->multi_next->multi_previous
                                                = pl_ecb->multi_previous;
                        }
                    }else           //本事件不是队列头
                    {
                        pl_ecb->multi_previous->multi_next
                                            = pl_ecb->multi_next;
                        pl_ecb->multi_next->multi_previous
                                            = pl_ecb->multi_previous;
                    }
                    pl_ecb->sync_head = NULL;   //事件头指针置空
                }
                if(pl_ecb->next == pl_ecb)      //这是闹钟同步队列最后一个结点.
                {
                    pg_event_delay = NULL;
                    pl_ecb->last_status.all = pl_ecb->event_status.all;
                    pl_ecb->event_status.bit.event_delay = 0;
                    pl_ecb->event_status.bit.wait_overtime = 0;
                    __djy_event_ready(pl_ecb);
                    break;
                }else
                {
                    pg_event_delay = pl_ecb->next;
                    pl_ecb->next->previous = pl_ecb->previous;
                    pl_ecb->previous->next = pl_ecb->next;
                    pl_ecb->last_status.all = pl_ecb->event_status.all;
                    pl_ecb->event_status.bit.event_delay = 0;
                    pl_ecb->event_status.bit.wait_overtime = 0;
                    __djy_event_ready(pl_ecb);
                    pl_ecb = pg_event_delay;
                }
            }else
                break;
        }
    }
    
    //下面处理时间片轮转调度.
    //因在开异步信号(允许调度)才可能进入__djy_isr_tick，即使因闹钟响导致新事件加
    //入，pg_event_running也必定在优先级单调队列中，但可能不等于pg_event_ready
    if(u32g_RRS_slice != 0)      //允许轮转调度
    {
        if( (pg_event_running->prio == pg_event_running->next->prio)
                    &&(pg_event_running != pg_event_running->next) )
        {//该优先级有多个事件，看轮转时间是否到
            if(u32g_os_ticks % u32g_RRS_slice == 0) //时间片用完
            {
                //先处理优先级单调队列，把pg_event_running从队列中取出，代之以
                //pg_event_running->next。
                pl_ecb = pg_event_running->next;
                pl_ecbn = pg_event_running->multi_next;
                pl_ecbp = pg_event_running->multi_previous;
                pl_ecbp->multi_next = pl_ecb;
                pl_ecb->multi_previous = pl_ecbp;
                pl_ecbn->multi_previous = pl_ecb;
                pl_ecb->multi_next = pl_ecbn;
                pg_event_running->multi_next = NULL;
                pg_event_running->multi_previous = NULL;

                //再处理就绪链表,把pg_event_running从就绪队列中拿出来，然后把它
                //放到同优先级的最后。

                //如果没有高优先级事件因同步时间到而加入ready队列，条件将成立
                if(pg_event_ready == pg_event_running)
                    pg_event_ready = pl_ecb;
//                pl_ecbn = pg_event_ready->multi_next;
//                pl_ecbp = pg_event_running->previous;
                //这两行把pg_event_running从队列中取出
                pg_event_running->previous->next = pl_ecb;
                pl_ecb->previous = pg_event_running->previous;
                //这两行把pg_event_running插入目标位置
                pg_event_running->previous = pl_ecbn->previous;
                pg_event_running->next = pl_ecbn;
                pl_ecbn->previous->next = pg_event_running;
                pl_ecbn->previous = pg_event_running;
            }
        }
    }
    return;
}

//----登记错误信息--------------------------------------------------------------
//功能：把系统错误信息登记到一个环形缓冲区里
//参数：error_type，错误类型
//      text，文字说明
//返回：无
//还需添加文本串的功能，---db
//-----------------------------------------------------------------------------
void djy_error_login(u32 error_type,const char *text)
{
    pg_event_running->error_no = error_type;
}

//----提取最后一条错误信息-----------------------------------------------------
//功能：如名
//参数：text，返回该错误的文字说明
//返回：错误号
//还需添加文本串和时间的功能，时间使用ticks数---db
//-----------------------------------------------------------------------------
u32 djy_get_last_error(char *text)
{
    return pg_event_running->error_no;
}

//----从ready队列中取出一个事件------------------------------------------------
//功能: 把一个事件从ready队列中取出，并使ready队列重新连接
//参数: event，待取出的事件指针
//返回: 无
//注: 调用者请保证在异步信号(调度)被禁止的情况下调用本函数
//-----------------------------------------------------------------------------
void __djy_cut_ready_event(struct event_ECB *event)
{
    struct event_ECB *pl_ecb;
    if(event != pg_event_ready)         //event不是ready队列头
    {
        if(event->multi_next == NULL)   //不在优先级单调队列中
        {
            event->next->previous = event->previous;
            event->previous->next = event->next;
        }else                           //在优先级单调队列中
        {
            pl_ecb = event->next;
            event->next->previous = event->previous;
            event->previous->next = event->next;
            if(pl_ecb->prio == event->prio)     //相应优先级不止一个事件
            {
                event->multi_previous->multi_next = pl_ecb;
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_next->multi_previous = pl_ecb;
                pl_ecb->multi_next = event->multi_next;
            }else                               //相应优先级只有一个事件
            {
                //pl_ecb即event->multi_next.
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = pl_ecb;
            }
        }
    }else                               //event是ready队列头
    {
        pg_event_ready = event->next;
        pl_ecb = event->next;
        event->next->previous = event->previous;
        event->previous->next = event->next;
        if(pl_ecb->prio == event->prio)     //相应优先级不止一个事件
        {
            event->multi_previous->multi_next = pl_ecb;
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_next->multi_previous = pl_ecb;
            pl_ecb->multi_next = event->multi_next;
        }else                               //相应优先级只有一个事件
        {
            //pl_ecb即event->multi_next.
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pl_ecb;
        }
    }
}

//----设置轮转调度时间片-------------------------------------------------------
//功能: djyos总所有参与轮转调度的事件时间片都是相同的，在这里设置。如果设为0，
//      表示不允许时间片轮转。
//参数: slices，新的轮转调度时间片，微秒数，将被向上取整为整数个ticks时间
//返回: 无
//-----------------------------------------------------------------------------
void djy_set_RRS_slice(u32 slices)
{
#if (32 > cn_cpu_bits)
    atom_low_t atom_low;
    //若处理器字长＜32位,需要多个周期才能更新u32g_RRS_slice,该过程不能被时钟中断打断.
    atom_low = int_low_atom_start( );   //本函数对不能嵌套调用
#endif

    u32g_RRS_slice =(slices + cn_tick_us -1)/cn_tick_us;

#if (32 > cn_cpu_bits)
    //若处理器字长＜32位,需要多个周期才能更新u32g_RRS_slice,该过程不能被时钟中断打断.
    int_low_atom_end( atom_low );
#endif
}

//----查询轮转调度时间片-------------------------------------------------------
//功能: djyos总所有参与轮转调度的事件时间片都是相同的，可用本函数查询。
//参数: 无
//返回: 当前时间片长度，微秒数。
//-----------------------------------------------------------------------------
u32 djy_get_RRS_slice(void)
{
    u32 temp;
#if (32 > cn_cpu_bits)
    atom_low_t atom_low;
    //处理器字长＜32位,需要多个周期才能读取u32g_RRS_slice,该过程不能被时钟中断打断.
    atom_low = int_low_atom_start( );   //本函数对不能嵌套调用
#endif

    temp = u32g_RRS_slice;

#if (32 > cn_cpu_bits)
    //处理器字长＜32位,需要多个周期才能读取u32g_RRS_slice,该过程不能被时钟中断打断.
    int_low_atom_end( atom_low );
#endif
    return temp * cn_tick_us;
}

//----查找可运行线程----------------------------------------------------------
//功能: 在 pg_event_ready队列中获得一个可以运行的线程.
//      1.从pg_event_ready队列头开始查找,如果该事件还没有连接线程,则试图创建之.
//      2.如果不能建立,肯定是因内存不足，则把该事件放到内存等待表中,继续往下查找.
//      3.如此反复,直到找到一个可以运行的线程.然后把pg_event_ready指针指向该事件
//参数: 无
//返回: 无
//备注: 本函数由操作系统调用,调用前保证关异步信号。
//      由于系统服务事件总是ready,所以总是能找到可以运行的线程.
//----------------------------------------------------------------------------
void __djy_select_event_to_run(void)
{
    struct  event_ECB *pl_ecb;
    struct  event_type *pl_evtt;  //被操作的事件的类型指针


    while(pg_event_ready->vm == NULL)
    {
        pl_evtt =& tg_evtt_table[pg_event_ready->evtt_id];
        if(pl_evtt->my_free_vm != NULL)     //该事件类型有空闲的线程,直接使用
        {
            pg_event_ready->vm = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = pl_evtt->my_free_vm->next;
        }else       //该事件类型没有空闲的线程,试图创建之
        {
            pg_event_ready->vm = __create_thread(pl_evtt);  //创建线程
            if(pg_event_ready->vm == NULL)                  //创建线程失败
            {
                pl_ecb = pg_event_ready;
                __djy_cut_ready_event(pl_ecb);
#if (cfg_dynamic_mem == 1)
                __m_wait_memory(pl_ecb);
#endif
            }else                                           //成功创建线程
            {
                if(pl_evtt->property.correlative == enum_independence)
                    pl_evtt->vpus++;        //独立事件vpus表示线程数
                                            //关联事件的vpus=参数数量
            }
        }
    }
}

//----创建进程虚拟机-----------------------------------------------------------
//功能: 创建进程虚拟机。
//参数: 无
//返回: 无
//备注: 这只是占一个位而已，在mp模式才有实用价值
//-----------------------------------------------------------------------------
void djy_create_process_vm(void)
{
    static struct process_vm my_process;
    pg_my_process = &my_process;
}


//----事件调度-----------------------------------------------------------------
//功能：剥夺running事件的cpu,把cpu交给ready队列的第一个事件.如果ready队列的第一
//      个事件尚未拥有线程,则建立之.建立线程时如果内存不足,则把这个事件放到
//      内存等待链表中,ready指针指向ready队列的下一个事件,依次推之,直到找到一个
//      线程虚拟机指针非空或者成功创建线程虚拟机的事件.
//参数：无
//返回：true = 成功切换，false = 未切换直接返回
//备注：1.本函数由操作系统调用
//      2.djyos系统认为,用户禁止中断就是为了能够连续执行,或者保护临界资源.
//      djyos中异步信号相当于高优先级线程,所以全局和异步信号禁止状态
//      下,是不允许事件调度的.
//      3.实时中断是否禁止,与调度无关.
//      4.由于最低优先级的系统服务事件总是ready,因此本函数总是能够找到目标事件
//-----------------------------------------------------------------------------
bool_t __djy_schedule(void)
{
    struct  event_ECB *event;
    u32 time;

    if(!djy_query_sch())
        return false;
    int_cut_asyn_signal();
    __djy_select_event_to_run();
    if(pg_event_ready != pg_event_running)
    {//当running事件仍在ready队列中,且内存不足以建立新虚拟机时,可能会出现优先
     //级高于running的事件全部进入内存等待队列的可能.此时执行else子句.
        event = pg_event_running;
#if(cfg_debug_info == 1)
        time = djy_get_fine_time();
        event->consumed_time += time - u32g_running_start_time;
        u32g_running_start_time = time;
#endif
        pg_event_running=pg_event_ready;
        __asm_switch_context(pg_event_ready->vm ,event->vm);
    }else
    {//优先级高于running的事件全部进入内存等待队列,下一个要处理的事件就是
     //running事件,无须执行任何操作
        return false;
    }
    return true;
}

//----中断内执行的事件调度------------------------------------------------------
//功能：剥夺running事件的cpu,把cpu交给ready队列的第一个事件.如果ready队列的第一
//      个事件尚不拥有线程,则创建之.建立线程时如果内存不足,则把这个事件放到
//      内存等待链表中,ready指针指向ready队列的下一个事件,依次推之,直到找到一个
//      线程虚拟机指针非空或者成功创建线程虚拟机的事件.
//参数：无
//返回：无
//备注：本函数由操作系统在异步信号引擎返回前调用
//      由于最低优先级的y_idle_service事件总是ready,因此总是能够找到调度对象的.
//-----------------------------------------------------------------------------
void __djy_schedule_asyn_signal(void)
{
    struct  event_ECB *event;
    u32 time;

    __djy_select_event_to_run();
    if(pg_event_ready != pg_event_running)
    {//当running事件仍在ready队列中,且内存不足以建立新虚拟机时,可能会出现优先
     //级高于running的事件全部进入内存等待队列的可能.此时执行else子句.
         event=pg_event_running;
#if(cfg_debug_info == 1)
         time = djy_get_fine_time();
         event->consumed_time += time - u32g_running_start_time;
         u32g_running_start_time = time;
#endif
         pg_event_running=pg_event_ready;
         __asm_switch_context_int(pg_event_ready->vm,event->vm);
    }else
    {//优先级高于running的事件全部进入内存等待队列,下一个要处理的事件就是
     //running事件,无须执行任何操作
    }
    return;
}

//----登记事件类型------------------------------------------------------------
//功能：登记一个事件类型到系统中,事件类型经过登记后,就可以pop了,否则,系统会
//      拒绝pop该类型事件
//参数：relation，取值为enum_independence或enum_correlative
//      default_prio，本事件类型的默认优先级。
//      vpus_res，系统繁忙时为本类型事件保留的虚拟机数量，
//      vpus_limit，同一事件本类型事件所能拥有的线程数量的最大值
//      thread_routine，线程入口函数(事件处理函数)
//      stack_size，执行thread_routine需要的栈尺寸，不包括thread_routine函数可能
//          调用的系统服务。
//      evtt_name，事件类型名，不同模块之间要交叉弹出事件的话，用事件类型名。
//          本参数允许是NULL，但只要非NULL，就不允许与事件类型表中已有的名字重名
//          名字不超过31个单字节字符
//返回：新事件类型的类型号
//------------------------------------------------------------------------------
u16 djy_evtt_regist(enum event_relation relation,
                       ufast_t default_prio,
                       u16 vpus_res,
                       u16 vpus_limit,
                       void (*thread_routine)(void),
                       u32 stack_size,
                       char *evtt_name)
{
    u16 i,evtt_id;
    if((default_prio >= cn_prio_sys_service) || (default_prio == 0))
    {
        djy_error_login(enum_knl_invalid_prio,"事件类型优先级非法");
        return cn_invalid_evtt_id;
    }
    int_save_asyn_signal();      //禁止调度也就是禁止异步事件
    for(evtt_id=0; evtt_id<cn_evtts_limit; evtt_id++)   //查找空闲的事件控制块
        if( tg_evtt_table[evtt_id].property.registered == 0)
            break;
    if(evtt_id == cn_evtts_limit)     //没有空闲事件控制块
    {
        djy_error_login(enum_knl_etcb_exhausted,"没有空闲事件控制块");
        int_restore_asyn_signal();
        return cn_invalid_evtt_id;
    }else if(evtt_name != NULL) //新类型有名字，需检查有没有重名
    {
        for(i=0; i<cn_evtts_limit; i++)
        {
            if(tg_evtt_table[i].property.registered == 1)
            {
                if(strcmp(tg_evtt_table[i].evtt_name,evtt_name) == 0)
                {
                    djy_error_login(enum_knl_evtt_homonymy,"事件类型重名");
                    int_restore_asyn_signal();
                    return cn_invalid_evtt_id;
                }
            }
        }
        if(strnlen(evtt_name,31) != cn_limit_uint32)
            strcpy(tg_evtt_table[evtt_id].evtt_name,evtt_name);
        else
        {
            memcpy(tg_evtt_table[evtt_id].evtt_name,evtt_name,31);
            tg_evtt_table[evtt_id].evtt_name[31] = '\0';
        }
    }else
    {
        tg_evtt_table[evtt_id].evtt_name[0] = '\0';   //清空事件类型名
    }
    
    tg_evtt_table[evtt_id].default_prio = default_prio;
    tg_evtt_table[evtt_id].pop_sum = 0;
    tg_evtt_table[evtt_id].done_sum = 0;
    tg_evtt_table[evtt_id].vpus_limit = vpus_limit;
    if(relation == enum_independence)
    {
        tg_evtt_table[evtt_id].vpus_res = vpus_res;
    }else
    {
        tg_evtt_table[evtt_id].vpus_res = 0;    //关联型事件，vpus_res无效
    }
    tg_evtt_table[evtt_id].thread_routine = thread_routine;
    tg_evtt_table[evtt_id].stack_size = stack_size;
    tg_evtt_table[evtt_id].mark_event = NULL;
    tg_evtt_table[evtt_id].done_sync = NULL;
    tg_evtt_table[evtt_id].pop_sync = NULL;
    tg_evtt_table[evtt_id].vpus = 0;
    if((cn_run_mode!=cn_mode_mp)||(default_prio<0x80))
    {//运行模式为si或dlsp，或该事件类型拥有紧急优先级，需预先创建一个线程虚拟机
        tg_evtt_table[evtt_id].my_free_vm =
                                __create_thread(&tg_evtt_table[evtt_id]);
        if(tg_evtt_table[evtt_id].my_free_vm == NULL)
        {//内存不足，不能创建虚拟机
            djy_error_login(enum_mem_tried,"登记事件类型时内存不足");
            int_restore_asyn_signal();
            return cn_invalid_evtt_id;
        }else
        {
            if(relation == enum_independence) //关联型事件的vpus=参数数量
                tg_evtt_table[evtt_id].vpus = 1;
        }
    }else
    {
        tg_evtt_table[evtt_id].my_free_vm = NULL;
    }
    tg_evtt_table[evtt_id].property.correlative = relation;
    tg_evtt_table[evtt_id].property.registered = 1;
    tg_evtt_table[evtt_id].property.inuse = 0;
    tg_evtt_table[evtt_id].property.deleting = 0;
    int_restore_asyn_signal();
    return evtt_id;
}

//----取事件类型id-------------------------------------------------------------
//功能：根据事件类型的名字，返回该事件类型的id号，不查找没名字的事件类型。
//参数：evtt_name，事件类型名
//返回：事件类型id号，如果没有找到则返回cn_invalid_id。
//备注：只能找到有名字的事件类型，没名字的事件类型又叫隐身事件类型。
//----------------------------------------------------------------------------
u16 djy_get_evtt_id(char *evtt_name)
{
    u16 loop;
    if(evtt_name == NULL)
        return cn_invalid_evtt_id;
    for(loop = 0; loop < cn_evtts_limit; loop++)
    {
        if(strcmp(tg_evtt_table[loop].evtt_name,evtt_name) ==0)
            return loop;
    }
    return cn_invalid_evtt_id;
}

//----删除事件类型-------------------------------------------------------------
//功能: 删除一个事件类型,如果队列中还有该类型事件(inuse == 1),只标记该类型为被
//      删除,真正的删除工作是在 done函数里完成的.如果队列中已经没有该类型事件了,
//      将会执行真正的删除操作.无论哪种情况,系统均会拒绝弹出该类型的新事件.
//参数: evtt_id,待删除的事件类型号
//返回: true = 成功，包括成功标记；false = 失败
//-----------------------------------------------------------------------------
bool_t djy_evtt_unregist(u16 evtt_id)
{
    struct thread_vm *next_vm,*temp;
    struct event_type *evtt;
    struct event_ECB *pl_ecb,*pl_ecb_temp;
    bool_t result = true;
    if(evtt_id >= cn_evtts_limit)
        return false;
    int_save_asyn_signal();
    evtt = &tg_evtt_table[evtt_id];
    if(evtt->property.registered == false)
    {//该事件类型是无效事件类型
        result = false;
    }
    if(evtt->done_sync != NULL)     //若完成同步队列中有事件，取出到就绪队列
    {
        pl_ecb = evtt->done_sync;     //取同步队列头
        while(pl_ecb != NULL)
        {
            pl_ecb->last_status.all = pl_ecb->event_status.all;   //保存当前状态
//            pl_ecb->event_status.bit.evtt_done_sync = 0;     //取消"同步中"状态
            if(pl_ecb->event_status.bit.wait_overtime)   //是否在超时队列中
            {
                __djy_resume_delay(pl_ecb);    //结束超时等待
//                pl_ecb->event_status.bit.wait_overtime = 0;  //取消"超时等待中"状态
            }
            pl_ecb->event_status.all = 0;   
            pl_ecb_temp = pl_ecb;
            if(pl_ecb->multi_next == evtt->done_sync)   //是最后一个事件
            {
                evtt->done_sync = NULL;
                pl_ecb = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;
            }
            __djy_event_ready(pl_ecb_temp);           //把事件加入到就绪队列中
        }
    }
    if(evtt->pop_sync != NULL)     //若弹出同步队列中有事件，取出到就绪队列
    {
        pl_ecb = evtt->pop_sync;     //取同步队列头
        while(pl_ecb != NULL)
        {
            pl_ecb->last_status.all = pl_ecb->event_status.all;   //保存当前状态
//            pl_ecb->event_status.bit.evtt_pop_sync = 0;     //取消"同步中"状态
            if(pl_ecb->event_status.bit.wait_overtime)   //是否在超时队列中
            {
                __djy_resume_delay(pl_ecb);    //结束超时等待
//                pl_ecb->event_status.bit.wait_overtime = 0;  //取消"超时等待中"状态
            }
            pl_ecb->event_status.all = 0;   
            pl_ecb_temp = pl_ecb;
            if(pl_ecb->multi_next == evtt->done_sync)  //是最后一个事件
            {
                evtt->pop_sync = NULL;  //置空事件同步队列
                pl_ecb = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;      //取队列中下一个事件
            }
            __djy_event_ready(pl_ecb_temp);       //把事件加入到就绪队列中
        }
    }
    if(evtt->property.inuse)
    {
        //事件类型正在使用，或完成同步和弹出同步队列非空，只标记删除
        evtt->property.deleting = 1;
    }else
    {
        //回收事件类型控制块，只需把registered属性清零。
        evtt->property.registered = 0;
        evtt->evtt_name[0] = '\0';      //清空事件类型名
        next_vm = evtt->my_free_vm;
        while(next_vm != NULL)          //释放该事件类型拥有的空闲虚拟机
        {
            temp = next_vm;
            next_vm = next_vm->next;
            m_free((void *)temp);
        }
    }
    int_restore_asyn_signal();
    return result;
}
const struct event_ECB cn_sys_event = {
                        tg_ECB_table,tg_ECB_table,//next,previous
                        tg_ECB_table,tg_ECB_table,//multi_next,multi_previous
                        NULL,                          //vm
                        NULL,                       //para_high_prio
                        NULL,                       //para_low_prio
                        NULL,                       //para_current
                        NULL,                       //sync
                        NULL,                       //sync_head
#if(cfg_debug_info == 1)
                        0,                          //start_time
                        0,                          //consumed_time
                        0,                          //consumed_time_second
                        0,                          //consumed_time_record
#endif
                        0,                          //delay_start_tick
                        0,                          //delay_end_tick
                        enum_knl_no_error,          //error_no
                        0,                          //event_result
                        0,                          //wait_mem_size
                        {0},                        //last_status
                        {0},                        //event_status
                        cn_prio_sys_service,        //prio
                        0,                          //evtt_id
                        0,                          //sync_counter
                        0,                          //event_id
                        0                           //local_memory
                        };

//----建立事件链表-------------------------------------------------------------
//功能：1.根据系统设定初始化操作系统虚拟机和事件表指针
//      2.把 cn_events_limit 个事件控制块结构连接成一个空闲队列,队列的结构参见
//        文档，由指针pg_free_event索引,
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void __djy_init_sys(void)
{
    u16 i;
    struct thread_vm *vm;
    pg_event_delay=NULL;    //延时事件链表空

    //把事件类型表全部置为没有注册,0为系统服务类型
    for(i=1; i<cn_evtts_limit; i++)
    {
        tg_evtt_table[i].property.registered = 0;
    }
    for(i = 1; i < cn_events_limit; i++)    //i=0为系统服务事件
    {
        if(i==(cn_events_limit-1))
            tg_ECB_table[i].next = NULL;
        else
            tg_ECB_table[i].next = &tg_ECB_table[i+1];
        //向前指针指向pg_event_free的地址,说明这事个空闲事件块.
        //没有别的含义,只是找一个唯一且不变的数值,全局变量地址在整个运行期
        //是不会变化的.
        tg_ECB_table[i].previous = (struct  event_ECB*)&pg_event_free;
        tg_ECB_table[i].event_id = i;    //本id号在程序运行期维持不变
    }
    pg_event_free = &tg_ECB_table[1];

    //初始化参数缓冲区
    for(i = 0; i < cn_paras_limit; i++)
    {
        if(i==(cn_paras_limit-1))
            tg_event_para_table[i].next = NULL;
        else
            tg_event_para_table[i].next = &tg_event_para_table[i+1];
    }
    pg_para_free = tg_event_para_table;

    tg_evtt_table[0].property.correlative = 1;
//    tg_evtt_table[0].property.overlay = 0;
    tg_evtt_table[0].property.registered = 1;
    tg_evtt_table[0].property.inuse = 1;
    tg_evtt_table[0].property.deleting = 0;
    tg_evtt_table[0].my_free_vm = NULL;
    tg_evtt_table[0].evtt_name[0] = '\0';
    tg_evtt_table[0].default_prio = cn_prio_sys_service;
    tg_evtt_table[0].pop_sum = 0;
    tg_evtt_table[0].done_sum = 0;
    tg_evtt_table[0].vpus_res =0;
    tg_evtt_table[0].vpus_limit =5;
    tg_evtt_table[0].vpus = 1;
    tg_evtt_table[0].thread_routine = __djy_service;
    tg_evtt_table[0].stack_size = cn_syssvc_stack;
    tg_evtt_table[0].mark_event = tg_ECB_table;
    tg_evtt_table[0].done_sync = NULL;
    tg_evtt_table[0].pop_sync = NULL;

    vm = __create_thread(&tg_evtt_table[0]);
    if(vm == NULL)      //内存不足，不能创建常驻虚拟机
    {
        djy_error_login(enum_mem_tried,"创建虚拟机时内存不足");
        tg_evtt_table[0].vpus = 0;
        return ;
    }
    //以下模拟popup函数,弹出系统服务事件.当事件队列为空时,调用popup的结果
    //是不可预知的.由于系统运行时,系统服务事件总在队列中,所以事件队列是不会空
    //的,这里模拟popup事件,而不是直接调用,可以使popup函数中省掉一个判断队列是否
    //空的操作.
    tg_ECB_table[0] = cn_sys_event;
    tg_ECB_table[0].vm = vm;
    tg_ECB_table[0].event_status.all = 0;
    pg_event_ready = tg_ECB_table;
    pg_event_running = pg_event_ready;
}

//--------检查是否允许调度------------------------------------------------------
//功能: 检查是否允许调度,允许异步信号且运行在线程态时，允许调度
//参数: 无
//返回: 允许调度返回true,否则返回false
//-----------------------------------------------------------------------------
bool_t djy_query_sch(void)
{
    return bg_schedule_enable;
}

//----把事件插入就绪队列中----------------------------------------------------
//功能：把事件插入到就绪队列中合适的位置,该事件原来不在就绪队列中.本函数只处理
//      事件队列，不设置event_status的值
//参数：event_ready,待插入的事件,该事件原来不在就绪队列中
//返回：无
//------------------------------------------------------------------------------
void __djy_event_ready(struct  event_ECB *event_ready)
{
    struct  event_ECB *event;
    atom_low_t atom_low;
    atom_low = int_low_atom_start(  );             //本函数对不能嵌套调用
    event = pg_event_ready;
    do
    { //找到一个优先级低于新事件的事件.由于系统服务事件prio=250总是ready,因此总是能找到.
        if(event->prio <= event_ready->prio)
            event = event->multi_next;
        else
            break;
    }while(event != pg_event_ready);
    event_ready->next = event;
    event_ready->previous = event->previous;
    event->previous->next = event_ready;
    event->previous = event_ready;

    //新插入的事件在同优先级的最后，故这样能够判断新事件是否该优先级的唯一事件。
    //若是该优先级的唯一事件，就要插入优先级单调队列
    if(event_ready->prio != event_ready->previous->prio)
    {//此时event == event_ready->next;
        event->multi_previous->multi_next = event_ready;
        event_ready->multi_previous = event->multi_previous;
        event->multi_previous = event_ready;
        event_ready->multi_next = event;
    }else       //无须插入优先级单调队列
    {
        event_ready->multi_next = NULL;
        event_ready->multi_previous = NULL;
    }
    if(event_ready->prio < pg_event_ready->prio)
        pg_event_ready = event_ready;
    int_low_atom_end( atom_low );
}

//----退出闹钟同步队列---------------------------------------------------------
//功能: 把一个事件从闹钟同步队列中取出,不管闹铃时间是否已经到达.用在带超时的同步
//      功能中，当同步条件先于超时时限到达时，从闹钟同步队列取出事件。
//参数: delay_event,待处理的事件.
//返回: 无
//备注: 1.本函数不开放给用户,仅仅是操作系统内部使用.操作系统不给用户提供提前
//      退出延时的功能,这样可以防止事件间互相干涉
//      2.本函数应该在关闭调度条件下调用,调用者保证,函数内部不检查中断状态.
//      3.本函数只把事件从闹钟同步链表中取出，并不放到就绪队列中。
//-----------------------------------------------------------------------------
void __djy_resume_delay(struct  event_ECB *delay_event)
{
    if(pg_event_delay->next == pg_event_delay)  //队列中只有一个事件
        pg_event_delay = NULL;
    else
    {
        if(delay_event == pg_event_delay)
            pg_event_delay = pg_event_delay->next;
        delay_event->next->previous = delay_event->previous;
        delay_event->previous->next = delay_event->next;
    }
    delay_event->next = NULL;
    delay_event->previous = NULL;
    delay_event->delay_end_tick = djy_get_time_tick();
}

//----加入延时队列------------------------------------------------------------
//功能：由正在执行的事件调用,直接把自己加入延时队列，不引起调度也不操作ready
//      队列，一般由同步函数调用，在timeout!=0时把事件加入闹钟同步队列实现
//      timeout功能，是特定条件下对y_timer_sync函数的简化。
//参数：u32l_uS,延迟时间,单位是微秒，将被向上调整为cn_tick_us的整数倍
//返回：无
//备注：1、操作系统内部使用的函数，且需在关闭中断（禁止调度）的条件下使用。
//      2、调用本函数前running事件已经从就绪表中取出，本函数不改变就绪表。
//      3、与其他内部函数一样，由调用方保证参数合理性，即u32l_uS>0.
//-----------------------------------------------------------------------------
void __djy_addto_delay(u32 u32l_uS)
{
    struct  event_ECB * event;
    u32 start,delay;

    pg_event_running->delay_start_tick = djy_get_time_tick(); //事件延时开始时间
    delay = u32l_uS + cn_tick_us -(u32)1;
    //如果u32l)uS接近0xffffffff时，会出现这种情况，若不处理，会出现
    //delay_start_tick和delay_end_tick相等的情况。
    if(delay < cn_tick_us)
        delay = 0xffffffff;
    pg_event_running->delay_end_tick = pg_event_running->delay_start_tick
                    + delay/cn_tick_us;  //闹铃时间

    if(pg_event_delay==NULL)    //延时队列空
    {
        pg_event_running->next = pg_event_running;
        pg_event_running->previous = pg_event_running;
        pg_event_delay=pg_event_running;
    }else
    {
        event = pg_event_delay;
        start=pg_event_running->delay_start_tick;
        delay = pg_event_running->delay_end_tick - start;
        do
        {//本循环找到第一个剩余延时时间大于新延时事件的事件.
            //如果1tick=1mS,那么49.7天后将回绕,djyos不能提供超过49.7天的延时
            //如果直接比较delay_end,则一个发生回绕而另一个不回绕时,将会出错,
            //我们利用固定位数无符号数相减后，高位被截断的特点，用delay_end
            //-start来进行比较，可以避免回绕时发生错误
            if((event->delay_end_tick - start) <= delay)
                event = event->next;
            else
                break;
        }while(event != pg_event_delay);
        //如果没有找到剩余延时时间比新延时事件长的事件,新事件插入队列尾,
        //而队列尾部就是pg_event_delay的前面,event恰好等于pg_event_delay
        //如果找到剩余延时时间长于新事件的事件,新事件插入该事件前面.
        //所以无论前面循环找到与否,均可使用下列代码
        pg_event_running->next = event;
        pg_event_running->previous = event->previous;
        event->previous->next = pg_event_running;
        event->previous = pg_event_running;
        if(pg_event_delay->delay_end_tick -start 
                    > pg_event_running->delay_end_tick-start)
            //新事件延时小于原队列中的最小延时.
            pg_event_delay = pg_event_running;
    }
}

//----设置事件优先级-----------------------------------------------------------
//功能: 事件处理中，可以调用本函数，改变自身的优先级。如果优先级被改低了，可能
//      立即调度，阻塞本事件运行。
//参数: new_prio,设置的新优先级
//返回: true = 成功设置，false=失败，一般是优先级不合法
//-----------------------------------------------------------------------------
bool_t djy_set_event_prio(ufast_t new_prio)
{
    if((new_prio >= cn_prio_sys_service) || (new_prio == 0))
        return false;
    int_save_asyn_signal();
    if(new_prio <= pg_event_running->prio)
        pg_event_running->prio = new_prio;
    else
    {
        __djy_cut_ready_event(pg_event_running);
        pg_event_running->next = NULL;
        pg_event_running->previous = NULL;
        pg_event_running->prio = new_prio;
        __djy_event_ready(pg_event_running);
    }
    int_restore_asyn_signal();
    return true;
}

//----闹钟同步-----------------------------------------------------------------
//功能：由正在执行的事件调用,使自己暂停u32l_uS微秒后继续运行.
//参数：u32l_uS,延迟时间,单位是微秒，0且允许轮转调度则把事件放到同优先级的
//      最后。非0值将被向上调整为cn_tick_us的整数倍
//返回：实际延时时间(ticks)数
//备注：延时队列为双向循环链表
//-----------------------------------------------------------------------------
u32 djy_event_delay(u32 u32l_uS)
{
    struct  event_ECB * event;
    u32 start,delay;

    if( !djy_query_sch())
    {   //禁止调度，不能进入闹钟同步状态。
        djy_error_login(enum_knl_cant_sched,NULL);
        return 0;
    }
    int_save_asyn_signal();
    //延时量为0的算法:就绪队列中有同优先级的，把本事件放到轮转最后一个，
    //注意:这里不管轮转调度是否允许
    if(u32l_uS == 0)
    {
        if((pg_event_running->prio == pg_event_running->next->prio)
                    && (pg_event_running != pg_event_running->next)   )
        {
            pg_event_running->delay_start_tick = djy_get_time_tick();//闹铃时间
            __djy_cut_ready_event(pg_event_running);      //从同步队列取出
            __djy_event_ready(pg_event_running);            //放回同步队列尾部
        }else
        {
            int_restore_asyn_signal();
            return 0;   //延时量为0，且不符合轮转条件
        }
    }else
    {
        pg_event_running->delay_start_tick =djy_get_time_tick();//设定闹铃的时间
        delay = u32l_uS + cn_tick_us -(u32)1;
        //如果u32l)uS接近0xffffffff时，会出现这种情况，若不处理，会出现
        //delay_start_tick和delay_end_tick相等的情况。
        if(delay < cn_tick_us)
            delay = 0xffffffff;
        pg_event_running->delay_end_tick = pg_event_running->delay_start_tick
                        + delay/cn_tick_us;  //闹铃时间

        __djy_cut_ready_event(pg_event_running);

        pg_event_running->last_status.all = pg_event_running->event_status.all;
        pg_event_running->event_status.bit.event_delay=1;
        if(pg_event_delay==NULL)    //闹钟同步队列空
        {
            pg_event_running->next = pg_event_running;
            pg_event_running->previous = pg_event_running;
            pg_event_delay=pg_event_running;
        }else
        {
            event = pg_event_delay;
            start = pg_event_running->delay_start_tick;
            delay = pg_event_running->delay_end_tick - start;
            do
            {//本循环找到第一个闹铃时间晚于新事件的事件.
                //如果1tick=1mS,那么49.7天后将回绕,djyos不能提供超过49.7天的延时
                //如果直接比较delay_end,则一个发生回绕而另一个不回绕时,将会出错,
                //我们利用固定位数无符号数相减后，高位被截断的特点，用delay_end
                //-start来进行比较，可以避免回绕时发生错误
                if((event->delay_end_tick - start) <= delay)
                    event = event->next;
                else
                    break;
            }while(event != pg_event_delay);
            //下面把新事件插入前述找到的事件前面，如没有找到，则event将等于
            //pg_event_delay，因是双向循环队列，g_event_delay前面也就刚好是队列尾。
            pg_event_running->next = event;
            pg_event_running->previous = event->previous;
            event->previous->next = pg_event_running;
            event->previous = pg_event_running;
            if(pg_event_delay->delay_end_tick -start
                    >pg_event_running->delay_end_tick-start)
                //新事件延时小于原队列中的最小延时.
                pg_event_delay = pg_event_running;
        }
    }
    int_restore_asyn_signal();
    return (djy_get_time_tick() -pg_event_running->delay_start_tick)*cn_tick_us;
}

//----同步事件----------------------------------------------------------------
//功能: 把正在运行的事件加入到指定事件的同步队列中去,然后重新调度。当指定事件
//      处理完成，或者超时时间到，将唤醒当前事件。
//参数: event_id,目标事件id号
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: cn_sync_success=同步条件成立返回，
//      cn_sync_timeout=超时返回，
//      enum_knl_cant_sched=禁止调度时不可执行同步操作
//      enum_knl_event_free,event_id是空闲id
//----------------------------------------------------------------------------
u32 djy_wait_event_completed(u16 event_id,u32 timeout)
{
    struct  event_ECB * pl_ecb;
    pl_ecb = &tg_ECB_table[event_id];

    if(djy_query_sch() == false)  //不能在禁止调度的情况下执行同步操作
        return (u32)enum_knl_cant_sched;
    if(timeout == 0)
        return (u32)cn_sync_timeout;
    int_save_asyn_signal();
    if(pl_ecb->previous == (struct event_ECB *)&pg_event_free)
    {//目标事件控制块是空闲事件控制块
        int_restore_asyn_signal();
        return (u32)enum_knl_event_free;
    }
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->next = NULL;
    pg_event_running->previous = NULL;

    //以下把pg_event_running加入到目标事件的同步队列中

    pg_event_running->sync_head = &pl_ecb->sync;
    if(pl_ecb->sync != NULL)
    {//被同步事件的同步队列不是空的
        pg_event_running->multi_next = pl_ecb->sync;
        pg_event_running->multi_previous = pl_ecb->sync->multi_previous;
        pl_ecb->sync->multi_previous->multi_next = pg_event_running;
        pl_ecb->sync->multi_previous = pg_event_running;
    }else
    {//被同步事件的同步队列是空的
        pl_ecb->sync = pg_event_running;
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
    }

    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.event_sync = 1;
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    int_restore_asyn_signal();  //恢复中断会引发重新调度

    int_save_asyn_signal();
    //检查从哪里返回，是超时还是同步事件完成。
    if(pg_event_running->event_status.bit.event_sync)
    {//说明同步条件未到，从超时返回。
        pg_event_running->event_status.bit.event_sync = 0;
        int_restore_asyn_signal();
        return (u32)cn_sync_timeout;
    }else if(pg_event_running->event_status.bit.event_sync_exit)
    {//被同步事件被异常终止，可能没有被正确执行
        pg_event_running->event_status.bit.event_sync_exit = 0;
        int_restore_asyn_signal();
        return (u32)enum_knl_event_sync_exit;
    }else
    {
        int_restore_asyn_signal();
        return (u32)cn_sync_success;
    }
}

//----事件类型完成同步----------------------------------------------------------
//功能: 把正在运行的事件加入到指定事件类型的前同步队列中去,然后重新调度。完成
//      同步以目标事件类型的完成次数为同步条件。
//参数: evtt_id,目标事件类型号
//      done_times,完成次数，0表示待最后一条该类型事件完成.
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: cn_sync_success=同步条件成立返回，
//      cn_sync_timeout=超时返回，
//      enum_knl_cant_sched=禁止调度时不可执行同步操作
//      enum_knl_evtt_free=evtt_id是空闲id
//      本函数必须联系共享文档的相关章节才容易读懂，注释难于解释那么清楚的。
//----------------------------------------------------------------------------
u32 djy_wait_evtt_completed(u16 evtt_id,u16 done_times,u32 timeout)
{
    struct  event_type *evtt;
    struct event_ECB *pl_ecb;
    evtt = &tg_evtt_table[evtt_id];
    if(djy_query_sch() == false)  //不能在禁止调度的情况下执行同步操作
        return (u32)enum_knl_cant_sched;
    if(timeout == 0)
        return (u32)cn_sync_timeout;
    int_save_asyn_signal();
    if((evtt->property.registered == false)     //事件类型未注册
        ||(evtt->property.deleting == 1))       //事件类型正在等待注销
    {
        int_restore_asyn_signal();
        return (u32)enum_knl_evtt_free;
    }
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->next = NULL;
    pg_event_running->previous = NULL;
    pg_event_running->sync_counter = done_times;

    //以下把pg_event_running加入到目标事件的同步队列中
    pg_event_running->sync_head = &evtt->done_sync;
    if(evtt->done_sync != NULL)
    {//被同步事件类型的同步队列不是空的
        pl_ecb = evtt->done_sync;
        do
        {//本循环找到第一个剩余完成次数不小于新事件的事件.
            if(pl_ecb->sync_counter < done_times)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != evtt->done_sync);
        //下面把新事件(pg_event_running)插入到队列中
        pg_event_running->multi_next = pl_ecb;
        pg_event_running->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = pg_event_running;
        pl_ecb->multi_previous = pg_event_running;
        if(evtt->done_sync->sync_counter > done_times)
            evtt->done_sync = pg_event_running;
    }else
    {//被同步事件类型的同步队列是空的
        evtt->done_sync = pg_event_running;
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
    }

    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.evtt_done_sync = 1;
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    int_restore_asyn_signal();  //恢复调度会引发事件切换

    int_save_asyn_signal();
    //检查从哪里返回，是超时还是同步事件完成。
    if(pg_event_running->event_status.bit.evtt_done_sync)
    {//说明同步条件未到，从超时返回，此时，被同步的事件肯定还没有完成。
        pg_event_running->event_status.bit.evtt_done_sync = 0;
        int_restore_asyn_signal();
        return (u32)cn_sync_timeout;
    }else if(pg_event_running->event_status.bit.event_sync_exit)
    {//被同步事件被异常终止，可能没有被正确执行
        pg_event_running->event_status.bit.event_sync_exit = 0;
        int_restore_asyn_signal();
        return (u32)enum_knl_event_sync_exit;
    }else
    {
        int_restore_asyn_signal();
        return (u32)cn_sync_success;
    }
}

//----事件类型弹出同步---------------------------------------------------------
//功能: 把正在运行的事件加入到指定事件类型的弹出同步队列中去,然后重新调度。弹出
//      同步是指以该事件类型的事件弹出若干次为同步条件。
//参数: evtt_id,目标事件类型号
//      pop_times,弹出次数，不能是0.
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: cn_sync_success=同步条件成立返回，
//      cn_sync_timeout=超时返回，
//      cn_sync_error=出错，
//      本函数必须联系共享文档的相关章节才容易读懂，注释难于解释那么清楚的。
//----------------------------------------------------------------------------
u32 djy_wait_evtt_pop(u16 evtt_id,u16 pop_times,u32 timeout)
{
    struct  event_type *evtt;
    struct event_ECB *pl_ecb;
    evtt = &tg_evtt_table[evtt_id];
    //不能在禁止调度的情况下执行同步操作,弹出次数不能设为0
    if((djy_query_sch() == false) || (pop_times == 0))
        return cn_sync_error;
    if(timeout == 0)
        return cn_sync_timeout;
    int_save_asyn_signal();
    if((evtt->property.registered == false)  //事件类型未注册
        ||(evtt->property.deleting == 1))   //事件类型正在等待注销
    {
        int_restore_asyn_signal();
        return cn_sync_error;
    }
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->next = NULL;
    pg_event_running->previous = NULL;
    pg_event_running->sync_counter = pop_times;

    //以下把pg_event_running加入到目标事件的同步队列中
    pg_event_running->sync_head = &evtt->pop_sync;
    if(evtt->pop_sync != NULL)
    {//被同步事件类型的同步队列不是空的
        pl_ecb = evtt->pop_sync;
        do
        {//本循环找到第一个剩余弹出次数不小于新事件的事件.
            if(pl_ecb->sync_counter < pop_times)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != evtt->pop_sync);
        //下面把新事件(pg_event_running)插入到队列中
        pg_event_running->multi_next = pl_ecb;
        pg_event_running->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = pg_event_running;
        pl_ecb->multi_previous = pg_event_running;
        if(evtt->pop_sync->sync_counter > pop_times)
            evtt->pop_sync = pg_event_running;
    }else
    {//被同步事件的同步队列是空的
        evtt->pop_sync = pg_event_running;
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
    }
    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.evtt_pop_sync = 1;
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
        
    int_restore_asyn_signal();  //恢复中断会引发重新调度

    int_save_asyn_signal();
    //检查从哪里返回，是超时还是同步事件完成。
    if(pg_event_running->event_status.bit.evtt_pop_sync)
    {//说明同步条件未到，从超时返回，此时，同步条件肯定还没有达成。
        pg_event_running->event_status.bit.evtt_pop_sync = 0;
        int_restore_asyn_signal();
        return cn_sync_timeout;
    }else
    {
        int_restore_asyn_signal();
        return cn_sync_success;
    }
}

//----弹出事件-----------------------------------------------------------------
//功能：向操作系统报告发生事件,操作系统接报后,将分配或建立合适的线程处理事件.
//参数：evtt_id，事件类型id
//      para_sync，enum_para_sync=等待事件处理完毕后返回，
//            enum_para_async=弹出事件后立即返回.
//            如果参数队列中的参数个数已经达到事件类型控制块中的vpus_limit值，则
//            自动按enum_para_sync处理-db,未实现，暂按超出vpus_limit直接返回错误
//      pop_result,如果函数返回了合法的事件id，且para_sync=enum_para_sync，则
//              pop_result = cn_sync_success，表示事件被处理完成后返回
//              pop_result = cn_sync_timeout，表示事件未被处理，超时返回
//              如果sync=false，则pop_result无意义。
//          如果函数返回了cn_invalid_event_id，则返回具体的出错信息
//      timeout,如果sync=true，定义超时时间，如果=0，则弹出事件后，立即按超时
//             返回，但事件将会得到处理。单位是us
//      para,事件参数指针，其含义由事件处理函数解析，尺寸最好不超过32字节。
//      para_size，参数长度，最好不超过32.
//      para_options, 参数选项，两个bit有用:
//          bit0:1=总是创建新参数到参数队列，0=如果相应队列尚无参数，则
//              创建新参数，否则替代该队列的最后一个参数的数值。关联型才有效
//          bit1:参数超过32bytes时的处理方法，
//              0=直接使用调用方提供的缓冲区，此时应用程序应该自行解决内存访问冲
//              突的问题，一个可行的方法是，使para_sync=enum_para_sync且
//              timoout=cn_timeout_forever，
//              1=malloc一个内存块。
//      prio,事件优先级,0表示使用默认值(存在事件类型表pg_event_type_table中),
//           关联型事件的主优先级只能是pg_event_type_table中的数值，prio参数设定
//           的是本次事件的相对优先级，1=低，0=高
//返回：若不能获取事件控制块，返回-1，否则返回事件id。
//注: 不会因不能获得事件控制块而阻塞。
//-----------------------------------------------------------------------------
u16 djy_event_pop(  u16  evtt_id,
                    enum pop_para_sync para_sync,
                    u32 *pop_result,
                    u32 timeout,    //如果阻塞，定义超时时间，
                                    //只有para_sync=enum_para_sync才有效
                    void *para,
                    u32 para_size,
                    ufast_t para_options, 
                    ufast_t prio)
{
    struct  event_ECB *pl_ecb;
    struct  event_type *pl_evtt;
    struct  event_ECB *pl_ecb_temp;
    struct  para_record *op_queue=NULL,**para_temp;
    u16 return_result;
    bool_t schbak;          //是否允许调度
    bool_t new_ecb = false; //是否分配了新的事件控制块，用于出错退出时释放资源
    bool_t new_para = false;//是否分配了新的参数控制块，用于出错退出时释放资源
    bool_t creat_new_para;  //参见para_options的参数说明
    bool_t malloc32;        //参见para_options的参数说明
    if(evtt_id >= cn_evtts_limit)
    {
        djy_error_login(enum_knl_evttid_limit,NULL);
        if(pop_result != NULL)
            *pop_result = (ptu32_t)enum_knl_evttid_limit;
        return cn_invalid_event_id;
    }
    schbak = djy_query_sch( );
    if((para_sync == enum_para_sync) && (timeout != 0) && (!schbak))
    {   //要求事件处理完才返回，但因不允许调度而无法执行
        djy_error_login(enum_knl_cant_sched,NULL);
        if(pop_result != NULL)
            *pop_result = (ptu32_t)enum_knl_cant_sched;
        return cn_invalid_event_id;
    }
    pl_evtt =&tg_evtt_table[evtt_id];
    if((pl_evtt->property.registered == 0)      //类型未登记
        ||(pl_evtt->property.deleting == 1))    //事件类型正在等待注销
    {
        djy_error_login(enum_knl_evtt_unregister,NULL);
        if(pop_result != NULL)
            *pop_result = (u32)enum_knl_evtt_unregister;
        return cn_invalid_event_id;
    }else if(prio >= cn_prio_sys_service)       //非法优先级
    {
        djy_error_login(enum_knl_invalid_prio,NULL);
        if(pop_result != NULL)
            *pop_result = (u32)enum_knl_invalid_prio;
        return cn_invalid_event_id;
    }

    malloc32 = para_options && cn_malloc_over_32;
    creat_new_para = para_options && cn_creat_new_para;
    int_save_asyn_signal();                     //关异步信号(关调度)
    pl_evtt->pop_sum++;
    //下面处理事件类型弹出同步，参看djy_evtt_pop_sync的说明
    pl_ecb = pl_evtt->pop_sync;
    if(pl_ecb != NULL)
    {
        do
        {//链表中的事件都是要么没有指定超时，要么时限未到，其他情况不会在此链表
         //中留下痕迹，evtt_done_sync位也已经清除

            //同步条件达成的条件: 1、同步计数器已减至1。
            if(pl_ecb->sync_counter == 1)
            {
                pl_ecb->last_status.all = pl_ecb->event_status.all;
//                pl_ecb->event_status.bit.evtt_pop_sync = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)  //指定的超时未到
                {
                    __djy_resume_delay(pl_ecb);     //从闹钟队列中移除事件
//                    pl_ecb->event_status.bit.wait_overtime = 0;
                }
                pl_ecb->event_status.all = 0;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //是最后一个事件
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next 
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous 
                                        = pl_ecb_temp->multi_previous;
                }
                __djy_event_ready(pl_ecb_temp);
            }else
            {
                pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->pop_sync);
    }

    //需要创建新事件的情况为两者之一:
    //1、事件队列中尚无该类型事件。
    //2、独立型事件
    if((pl_evtt->property.inuse == 0) || (pl_evtt->property.correlative == 0))
    {
        if((pg_event_free==NULL)||(pg_para_free == NULL)) //没有空闲的事件控制块
        {
            if(pg_event_free==NULL)
            {
                djy_error_login(enum_knl_ecb_exhausted,NULL);
                if(pop_result != NULL)
                    *pop_result = (u32)enum_knl_ecb_exhausted;
            }else
            {
                djy_error_login(enum_knl_pcb_exhausted,NULL);
                if(pop_result != NULL)
                    *pop_result = (u32)enum_knl_pcb_exhausted;
            }
            return_result = cn_invalid_event_id;
            goto end_pop;
        }else if((pl_evtt->property.correlative == 0)
                && ((pl_evtt->pop_sum-pl_evtt->done_sum)>= pl_evtt->vpus_limit))
        {
            djy_error_login(enum_knl_para_over,NULL);
            return_result = cn_invalid_event_id;
            if(pop_result != NULL)
                *pop_result = (u32)enum_knl_para_over;
            goto end_pop;
        }else                       //有空闲事件控制块
        {
            pl_ecb = pg_event_free;         //从空闲链表中提取一个事件控制块
            pg_event_free=pg_event_free->next;  //空闲事件控制块数量减1
            op_queue = pg_para_free;
            pg_para_free = pg_para_free->next;
            new_ecb = true;
            new_para = true;
            //设置新事件的事件控制块
            pl_ecb->next = NULL;
            pl_ecb->previous = NULL;
            pl_ecb->multi_next = NULL;
            pl_ecb->multi_previous = NULL;
            pl_ecb->vm = NULL;
            pl_ecb->para_high_prio = NULL;
            pl_ecb->para_low_prio = NULL;
            pl_ecb->sync = NULL;
            pl_ecb->sync_head = NULL;
#if(cfg_debug_info == 1)
            pl_ecb->start_time=djy_get_time_tick();   //事件发生时间
            pl_ecb->consumed_time = 0;
            pl_ecb->consumed_time_second = 0;
            pl_ecb->consumed_time_record = 0;
#endif
            pl_ecb->delay_start_tick = 0;
            pl_ecb->delay_end_tick = 0;
            pl_ecb->error_no = 0;
            pl_ecb->wait_mem_size = 0;
            pl_ecb->last_status.all = 0;
            pl_ecb->event_status.all = 0;
            pl_ecb->evtt_id=evtt_id;          //设置事件类型
            pl_ecb->sync_counter = 0;
    //      pl_ecb->event_id = xxx      //注:event_id无须在这里初始化。
            pl_ecb->local_memory = 0;
            pl_evtt->property.inuse = 1;
            pl_evtt->mark_event = pl_ecb;
            op_queue->start_time = pl_ecb->start_time;
            op_queue->dynamic_mem = false;
            
            if(para_size != 0)
            {
                if(para_size > 32)
                {
                    if(malloc32 == true)    //动态分配内存，然后copy参数
                    {
                        //参数是申请给被弹出的事件用的，故用全局内存
                        op_queue->para = m_malloc_gbl(para_size,0);
                        if(op_queue->para == NULL)
                        {
                            djy_error_login(enum_knl_memory_over,NULL);
                            return_result = cn_invalid_event_id;
                            if(pop_result != NULL)
                                *pop_result = (u32)enum_knl_memory_over;
                            goto end_pop_save;
                        }else
                        {
                            memcpy(op_queue->para,para,para_size);
                            op_queue->dynamic_mem = true;
                        }
                    }else
                        op_queue->para = para;  //直接使用调用者提供的缓冲区，
                                                //无须copy参数
                }else   //参数尺寸小于32，直接使用参数控制块自带的32字节缓冲区
                {
                    op_queue->para = op_queue->static_para;
                    memcpy(op_queue->para,para,para_size);
                }
            }
            if(pl_evtt->property.correlative == 0)
            {
                pl_ecb->para_current = op_queue;
                if(prio != 0)
                {
                    pl_ecb->prio = prio;                    //设置事件优先级,
                }else
                    pl_ecb->prio =pl_evtt->default_prio;//从事件类型中继承优先级
            }else
            {
                pl_evtt->vpus = 1;          //表示队列中有一个参数
                pl_ecb->para_current = NULL;
                pl_ecb->prio = pl_evtt->default_prio;
                op_queue->next = op_queue;
                op_queue->previous = op_queue;
                if(prio == 0)
                {
                    pl_ecb->para_high_prio = op_queue;
                    pl_ecb->para_low_prio = NULL;
                }else
                {
                    pl_ecb->para_high_prio = NULL;
                    pl_ecb->para_low_prio = op_queue;
                }
            }
            __djy_event_ready(pl_ecb);
        }
        return_result = pl_ecb->event_id;
    }else       //关联型事件，且队列中已经有该类型事件
    {
        pl_ecb = pl_evtt->mark_event;
        if(prio == 0)
            para_temp = &pl_ecb->para_high_prio;   //待操作队列是高优先级队列
        else
            para_temp = &pl_ecb->para_low_prio;  //待操作队列是低优先级队列
        if(creat_new_para == false)     //不强制创建新参数
        {
            if(*para_temp == NULL)        //prio相应的队列中尚无参数
            {
                //无空闲参数控制块或者该事件类型所使用的参数控制块已经达到限制
                if((pg_para_free==NULL) || (pl_evtt->vpus>=pl_evtt->vpus_limit))
                {
                    djy_error_login(enum_knl_pcb_exhausted,NULL);
                    return_result = cn_invalid_event_id;
                    if(pop_result != NULL)
                        *pop_result = (u32)enum_knl_pcb_exhausted;
                    goto end_pop_save;
                }else
                {
                    pl_evtt->vpus++;            //队列中的参数个数增加
                    new_para = true;            //标记申请了新参数
                    op_queue = pg_para_free;    //以下两行分配参数控制块
                    pg_para_free = pg_para_free->next;
                    op_queue->next = op_queue;  //参数控制块自成双向循环链表
                    op_queue->previous = op_queue;
                    op_queue->start_time = djy_get_time_tick();
                    op_queue->dynamic_mem = false;
                    *para_temp = op_queue;      //新参数加入事件的参数队列
                }
            }else
            {
                op_queue = (*para_temp)->previous;//待操作的参数是原队列中的最后一个
            }
        }else                   //创建新参数
        {
            //无空闲参数控制块或者该事件类型所使用的参数控制块已经达到限制
            if((pg_para_free==NULL) || (pl_evtt->vpus>=pl_evtt->vpus_limit))
            {
                if(pg_para_free==NULL)
                {
                    djy_error_login(enum_knl_pcb_exhausted,NULL);
                    if(pop_result != NULL)
                        *pop_result = (u32)enum_knl_pcb_exhausted;
                }else
                {
                    djy_error_login(enum_knl_para_over,NULL);
                    if(pop_result != NULL)
                        *pop_result = (u32)enum_knl_para_over;
                }
                return_result = cn_invalid_event_id;
                goto end_pop_save;
            }else
            {
                pl_evtt->vpus++;                //队列中的参数个数增加
                new_para = true;                //标记申请了新参数
                op_queue = pg_para_free;        //以下两行分配参数控制块
                pg_para_free = pg_para_free->next;
                if(*para_temp == NULL)          //原队列空
                {
                    op_queue->next = op_queue;
                    op_queue->previous = op_queue;
                    *para_temp = op_queue;
                }else                           //原队列非空，新参数加到最后
                {
                    op_queue->next = *para_temp;
                    op_queue->previous = (*para_temp)->previous;
                    (*para_temp)->previous->next = op_queue;
                    (*para_temp)->previous = op_queue;
                }
            }
        }
        if(para_size != 0)
        {
            if(para_size > 32)
            {
                if(malloc32 == true)    //动态分配内存，然后copy参数
                {
                    //参数是申请给被弹出的事件用的，故用全局内存
                    op_queue->para = m_malloc_gbl(para_size,0);
                    if(op_queue->para == NULL)
                    {
                        djy_error_login(enum_knl_memory_over,NULL);
                        return_result = cn_invalid_event_id;
                        if(pop_result != NULL)
                            *pop_result = (u32)enum_knl_memory_over;
                        goto end_pop_save;
                    }else
                    {
                        memcpy(op_queue->para,para,para_size);
                        op_queue->dynamic_mem = true;
                    }
                }else
                    op_queue->para = para;  //直接使用调用者提供的缓冲区，
                                            //无须copy参数
            }else   //参数尺寸小于32，直接使用参数控制块自带的32字节缓冲区
            {
                op_queue->para = op_queue->static_para;
                memcpy(op_queue->para,para,para_size);
            }
        }
        return_result = pl_ecb->event_id;
    }

    if(para_sync == enum_para_sync)        //可能引发调度
    {
        if(timeout == 0)    //超时时间为0，立即按超时返回
        {
            if(pop_result != NULL)
                *pop_result = (u32)cn_sync_timeout;
        }else               //设定了超时时间，将重新调度
        {
            __djy_cut_ready_event(pg_event_running);
            pg_event_running->sync_counter = 1;
            
            //以下把pg_event_running加入到目标事件的同步队列中
            pg_event_running->sync_head = &(op_queue->sync);
            op_queue->sync = pg_event_running;
            pg_event_running->multi_next = pg_event_running;
            pg_event_running->multi_previous = pg_event_running;
            
            pg_event_running->last_status.all = 
                    pg_event_running->event_status.all;
            pg_event_running->event_status.bit.event_sync = 1;
            if(timeout != cn_timeout_forever)
            {
                pg_event_running->event_status.bit.wait_overtime =1;
                __djy_addto_delay(timeout);
            }
            int_restore_asyn_signal();  //恢复中断会引发重新调度

//注:事件处理结果在djy_task_completed函数中给            
            int_save_asyn_signal();
            //检查从哪里返回，是超时还是同步事件完成。
            if(pg_event_running->event_status.bit.event_sync)
            {//说明同步条件未到，从超时返回。
                pg_event_running->event_status.bit.event_sync = 0;
                if(pop_result != NULL)
                    *pop_result = (u32)cn_sync_timeout;
            }else if(pg_event_running->event_status.bit.event_sync_exit)
            {//被同步事件被异常终止，可能没有被正确执行
                pg_event_running->event_status.bit.event_sync_exit = 0;
                if(pop_result != NULL)
                    *pop_result = (u32)enum_knl_event_sync_exit;
            }else
            {
                if(pop_result != NULL)
                    *pop_result = (u32)cn_sync_success;
            }
        }
    }
    goto end_pop;

end_pop_save:
    if(new_ecb == true)
    {
        pl_ecb->next = pg_event_free;//释放pl_ecb
        pg_event_free = pl_ecb;
        pl_ecb->previous = (struct  event_ECB*)&pg_event_free;
    }
    if(new_para == true)
    {
        op_queue->next = pg_para_free;//释放op_queue
        pg_para_free = op_queue;
        
    }
end_pop:
    int_restore_asyn_signal();  //恢复中断状态
    return return_result;
}

//----取事件处理结果-----------------------------------------------------------
//功能: 一个事件在处理过程中，如果弹出了新事件，并且等待事件处理结果(调用
//      djy_event_pop函数是sync=true)。如果调用djy_event_pop时返回了合法的事件
//      id，且不是超时返回，则可以用本函数获取新事件的处理结果。只能取最后一次
//      成功处理的事件结果。
//参数: 无
//返回: 事件处理结果。
//-----------------------------------------------------------------------------
ptu32_t djy_get_event_result(void)
{
    return pg_event_running->event_result;
}

//----提取事件参数-------------------------------------------------------------
//功能: 提取处理中事件或子事件的参数，规则顺序:1、高优先级的队列非空，取第一个
//      并把它从队列中删除。2、ppara指针非空,取其值并清空。3、低优先级队列非空，
//      取第一个并从队列删除
//参数: time，如果非NULL，则返回所获取的参数进入队列的时间，ticks数
//返回: 事件参数指针
//注: 本函数只能取正在处理中的事件自己的参数
//-----------------------------------------------------------------------------
void *djy_get_event_para(u32 *time)
{
    struct para_record *sub;
    void *result;
    atom_low_t  atom_bak;
    atom_bak = int_low_atom_start();
    if(pg_event_running->para_current != NULL)
    {
        result = (void*)pg_event_running->para_current->para;
    }else if(pg_event_running->para_high_prio != NULL) //高优先级子事件队列非空
    {
        sub = pg_event_running->para_high_prio;  //取高优先级子事件队列
        if(sub->next == sub)                    //队列中只有一个子事件
        {
            pg_event_running->para_high_prio = NULL;
        }else                                   //队列中超过一个子事件
        {
            sub->next->previous = sub->previous;
            sub->previous->next = sub->next;
            pg_event_running->para_high_prio = sub->next;
        }
        pg_event_running->para_current = sub;
        result = (void*)sub->para;
    }else if(pg_event_running->para_low_prio != NULL)//低优先级子事件队列非空
    {
        sub = pg_event_running->para_low_prio;   //取低优先级子事件队列
        if(sub->next == sub)                    //队列中只有一个子事件
        {
            pg_event_running->para_low_prio = NULL;
        }else                                   //队列中超过一个子事件
        {
            sub->next->previous = sub->previous;
            sub->previous->next = sub->next;
            pg_event_running->para_low_prio = sub->next;
        }
        pg_event_running->para_current = sub;
        result = (void*)sub->para;
    }else
    {
        result = NULL;
    }
    int_low_atom_end(atom_bak);
    if((result != NULL) && (time != NULL))
        *time = pg_event_running->para_current->start_time;
    return result;
}

//----取自身的事件类型id-------------------------------------------------------
//功能: 由应用程序调用，取正在处理的事件的事件类型id
//参数: 无
//返回: 事件类型id
//-----------------------------------------------------------------------------
u16 djy_my_evtt_id(void)
{
    return pg_event_running->evtt_id;
}

//----启动操作系统--------------------------------------------------------------
//功能: 初始化时钟嘀嗒，复位异步信号使能状态，选出最高优先级事件，调用
//      __asm_turnto_context把上下文切换到这个事件
//参数: 无
//返回: 无
//_____________________________________________________________________________
void __djy_start_os(void)
{
    //本句为容错性质，以防用户模块初始化过程中没有成对调用异步信号使能与禁止函数
    __int_reset_asyn_signal();
    __djy_init_tick();
    __djy_select_event_to_run();
    pg_event_running = pg_event_ready;
    bg_schedule_enable = true;
    __asm_start_thread(pg_event_running->vm);
}

//----异常激活事件的同步队列---------------------------------------------------
//功能: 事件异常结束时，激活一个事件同步队列中的所有事件，返回
//      给同步中的事件的"执行结果"被置为0，被同步事件异常返回位被置位
//参数: para，参数队列的队列头，参数构成一个双向链表。
//返回: 无
//-----------------------------------------------------------------------------
void __djy_active_eventsync_exp(struct event_ECB *sync)
{
    struct event_ECB *event_temp;
    if(sync == NULL)
        return ;

    sync->multi_previous->multi_next = NULL;
    do{
        sync->last_status.all =sync->event_status.all;  //保存当前状态
//        sync->event_status.bit.event_sync = 0;          //取消"同步中"状态
//        sync->event_status.bit.event_sync_exit = 1;     //置位异常结束标志
        sync->event_result = 0;
        sync->sync_head = NULL;
        if(sync->event_status.bit.wait_overtime)        //是否在超时队列中
        {
            __djy_resume_delay(sync);                   //结束超时等待
//            sync->event_status.bit.wait_overtime =0;    //取消"超时等待中"状态
        }
        sync->event_status.all = 0;                     //取消"同步中"状态
        sync->event_status.bit.event_sync_exit = 1;     //置位异常结束标志
        event_temp = sync->multi_next;
        __djy_event_ready(sync);           //把事件加入到就绪队列中
        sync = event_temp;
    }while(sync != NULL);
    return;
}

//----异常激活事件参数的同步队列-----------------------------------------------
//功能: 事件异常结束时，激活一个事件所有参数的同步队列中的所有事件，并释放参数控
//      制块.返回给同步中的事件的"执行结果"被置为0，被同步事件异常返回位被置位
//参数: para，参数队列的队列头，参数构成一个双向链表。
//返回: 无
//-----------------------------------------------------------------------------
void __djy_active_parasync_exp(struct para_record *para)
{
    struct para_record *para_temp;
    struct event_ECB *pl_ecb,*event_temp;
    if(para == NULL)
        return ;

    para->previous->next = NULL;
    do{
        pl_ecb = para->sync;
        if(pl_ecb != NULL)
        {
            pl_ecb->multi_previous->multi_next = NULL;
            do{
                pl_ecb->last_status.all =pl_ecb->event_status.all;//保存当前状态
//                pl_ecb->event_status.bit.event_sync = 0;     //取消"同步中"状态
//                pl_ecb->event_status.bit.event_sync_exit = 1;//置位异常结束标志
                pl_ecb->event_result = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)   //是否在超时队列中
                {
                    __djy_resume_delay(pl_ecb);               //结束超时等待
//                    pl_ecb->event_status.bit.wait_overtime =0;//取消"超时等待中"状态
                }
                pl_ecb->event_status.all = 0;     //取消"同步中"状态
                pl_ecb->event_status.bit.event_sync_exit = 1;//置位异常结束标志
                event_temp = pl_ecb->multi_next;
                __djy_event_ready(pl_ecb);           //把事件加入到就绪队列中
                pl_ecb = event_temp;
            }while(pl_ecb != NULL);
        }

        if(para->dynamic_mem == true)
        {
            m_free(para->para);
        }
        para_temp = para;
        para = para->next;                  //取队列中下一个事件
        para_temp->sync = NULL;
        para_temp->next = pg_para_free;     //释放参数控制块
        pg_para_free = para_temp;
    }while(para != NULL);
    return;
}

#define cn_deliver_to   0   //虚拟机已经转交
#define cn_keep         1   //虚拟机保留不删除
#define cn_delete       2   //虚拟机应该被删除
#define cn_deleted      3   //虚拟机已经被删除

//----事件逸出-----------------------------------------------------------------
//功能: 事件处理函数自然返回,其行为类似于djy_event_completed,对于独立型事件，这
//      两个函数是一致的，但对于关联型，则有所不同。djy_event_completed不会把事
//      件从事件队列中删除，也不会把线程至于初始态，djy_event_exit则会删除事件，
//      并把线程重新初始化。
//参数: 无
//返回: 本函数不返回
//-----------------------------------------------------------------------------
void djy_event_exit(void)
{
    struct thread_vm *next_vm,*temp;
    struct event_ECB *pl_ecb;
    struct event_type   *pl_evtt;
    struct event_ECB *pl_ecb_temp;
    ucpu_t  vm_final = cn_delete;

    //此处不用int_save_asyn_signal函数，可以在应用程序有bug，没有成对调用
    //int_save_asyn_signal和int_restore_asyn_signal的情况下，确保错误到此为止。
    __int_reset_asyn_signal();  //直到__vm_engine函数才再次打开.

    //激活事件同步队列中的所有事件
    __djy_active_eventsync_exp(pg_event_running->sync);

    if(pg_event_running->para_current != NULL)
        __djy_active_eventsync_exp(pg_event_running->para_current->sync);
    //以下激活参数的同步队列中的事件,并释放参数队列中的全部参数
    __djy_active_parasync_exp(pg_event_running->para_high_prio);
    __djy_active_parasync_exp(pg_event_running->para_low_prio);

#if (cfg_dynamic_mem == 1)
    if(pg_event_running->local_memory != 0)
    {
        __m_cleanup(pg_event_running->event_id);    //
    }
#endif
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->previous
                    = (struct  event_ECB*)&pg_event_free;//表示本控制块空闲
    pg_event_running->next = pg_event_free;     //pg_event_free是单向非循环队列
    pg_event_free = pg_event_running;
    pg_event_free->event_id = pg_event_free - tg_ECB_table;   //容错用

    //下面处理完成同步队列，必须联系共享文档的相关章节才容易读懂，注释难于解释
    //那么清楚的。
    pl_evtt =&tg_evtt_table[pg_event_running->evtt_id];
    pl_evtt->done_sum++;
    pl_ecb = pl_evtt->done_sync;
    if(pl_ecb != NULL)
    {
        do
        {//链表中的事件都是要么没有指定超时，要么时限未到，其他情况不会在此链表
         //中留下痕迹，evtt_done_sync位也已经清除

            //同步条件达成的条件: 1、同步计数器已减至1。
            //2、同步计数器为0且本类型最后一条事件已经处理完
            if((pl_ecb->sync_counter == 1)
                    ||((pl_ecb->sync_counter == 0) 
                            &&(pl_evtt->pop_sum == pl_evtt->done_sum)))
            {
                pl_ecb->last_status.all = pl_ecb->event_status.all;
//                pl_ecb->event_status.bit.evtt_done_sync = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)  //指定的超时未到
                {
                    __djy_resume_delay(pl_ecb);       //从闹钟队列中移除事件
//                    pl_ecb->event_status.bit.wait_overtime = 0;
                }
                pl_ecb->event_status.all = 0;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //是最后一个事件
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next 
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous 
                                        = pl_ecb_temp->multi_previous;
                }
                __djy_event_ready(pl_ecb_temp);
            }else
            {
                if(pl_ecb->sync_counter != 0)
                    pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->done_sync);
    }

    //线程和事件控制块的处理方式
    //1、若线程数量小于等于vpus_res，则保留.
    //2、其他情况，销毁线程
    //3、事件控制块则直接受惠

    if(pl_evtt->property.correlative == 1)      //关联型事件
    {
        vm_final = cn_keep;
        pl_evtt->vpus =0;
    }else   //独立型事件
    {
        //有未得到虚拟机的事件，保留之
        if((pl_evtt->pop_sum - pl_evtt->done_sum) > pl_evtt->vpus)
        {
            vm_final = cn_keep;
        }else   //没有未得到虚拟机的事件，再看是否需保留
        {
            if(pl_evtt->vpus<=pl_evtt->vpus_res)//该类型事件拥有的线程数已经低于
                                                    //或等于最低保留量，保留之
            {
                vm_final = cn_keep;
            }else                                   //可能删除线程
            {
                if(pl_evtt->vpus == 1)   //这是最后一个事件
                {
                    pl_evtt->property.inuse = 0;
                    if((cn_run_mode!=cn_mode_mp)||(pl_evtt->default_prio<0x80))
                    {
                 //运行模式为si或dlsp，或该事件类型拥有紧急优先级时，须保留线程
                        vm_final = cn_keep;
                    }else
                    {
                        vm_final = cn_delete;
                    }
                }else
                {
                    vm_final = cn_delete;
                }
            }
        }
    }

//看事件类型控制块是否要删除，用户注销一个事件类型，如果该事件类型正在使用，则不
//会立即注销，只是不能弹出新事件，要等属于该事件类型的所有事件处理完才能注销。
    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //回收事件类型控制块，只需把registered属性清零。
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';   //清空事件类型名
        while(next_vm != NULL)          //释放该事件类型拥有的空闲线程
        {
            temp = next_vm;
            next_vm = next_vm->next;
            m_free((void *)temp);
        }
        vm_final = cn_deleted;          //线程已经被删除
    }

    __djy_select_event_to_run();
    if(vm_final == cn_delete)                   //删除虚拟机
    {
        m_free((void*)pg_event_running->vm);    //删除虚拟机
        pl_evtt->vpus--;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_turnto_context(pg_event_running->vm);
    }else if(vm_final == cn_keep)    //保留虚拟机
    {
        pg_event_running->vm->next = pl_evtt->my_free_vm;
        pl_evtt->my_free_vm = pg_event_running->vm;
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }else               //虚拟机已经转交给另一条事件,或已经被删除
    {
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }
}
//----完成事件------------------------------------------------------------------
//功能：向操作系统报告事件已经完成,操作系统接到报告后,完成清理工作.
//      1.如果事件同步队列非空，把同步事件放到ready表。
//      2.如果持有设备，释放之，并把该设备等待队列中的第一个事件放到ready表。
//      3.设置该事件类型控制块的参数。
//      4.把事件块各分量设置到初始态,并放入free表。
//      5.如果是非实时虚拟机,释放动态分配的内存(如果有)
//  如果是线程需保留，则:
//      6.复位虚拟机。
//      7.不保存上下文，直接切换到新的ready表头
//  如果线程无需保留
//      6.释放虚拟机内存。
//      7.不保存上下文，直接切换到新的ready表头
//参数：result，事件处理结果，这个结果将返回给弹出该事件的事件(如果设定了同步)
//返回：无
//备注: 调用本函数的必定是running事件,在running事件上下文中执行，不可以调用
//      __asm_reset_thread函数。
//-----------------------------------------------------------------------------
void djy_task_completed(ptu32_t result)
{
    struct thread_vm *next_vm,*temp;
    struct event_ECB *pl_ecb,*event_temp;
    struct  event_type   *pl_evtt;
    struct  event_ECB *pl_ecb_temp;
    struct para_record *cur_para;
    ucpu_t  vm_final = cn_delete;

    //此处不用int_save_asyn_signal函数，可以在应用程序有bug，没有成对调用
    //int_save_asyn_signal和int_restore_asyn_signal的情况下，确保错误到此为止。
    pl_evtt =&tg_evtt_table[pg_event_running->evtt_id];
    if(pl_evtt->property.correlative == enum_correlative)      //关联型事件
    {
        int_save_asyn_signal();
    }
    else
    {
        __int_reset_asyn_signal();  //直到__vm_engine函数才再次打开.
    }
    //下面处理同步队列，必须联系共享文档的相关章节才容易读懂，注释难于解释
    //那么清楚的。
    pl_ecb = pg_event_running->sync;     //取同步队列头
    if(pl_ecb != NULL)
    {
        pl_ecb->multi_previous->multi_next = NULL;
        do{
            pl_ecb->last_status.all = pl_ecb->event_status.all;  //保存当前状态
//            pl_ecb->event_status.bit.event_sync = 0;          //取消"同步中"状态
            pl_ecb->sync_head = NULL;
            pl_ecb->event_result = result;
            if(pl_ecb->event_status.bit.wait_overtime)        //是否在超时队列中
            {
                __djy_resume_delay(pl_ecb);                   //结束超时等待
//                pl_ecb->event_status.bit.wait_overtime =0;//取消"超时等待中"状态
            }
            pl_ecb->event_status.all = 0;
            event_temp = pl_ecb->multi_next;
            __djy_event_ready(pl_ecb);           //把事件加入到就绪队列中
            pl_ecb = event_temp;
        }while(pl_ecb != NULL);
    }
    cur_para = pg_event_running->para_current;
    if(cur_para != NULL)
    {
        pl_ecb = cur_para->sync;
        if(pl_ecb != NULL)
        {
            pl_ecb->multi_previous->multi_next = NULL;
            do{
                pl_ecb->last_status.all =pl_ecb->event_status.all;//保存当前状态
    //            pl_ecb->event_status.bit.event_sync = 0;     //取消"同步中"状态
                pl_ecb->sync_head = NULL;
                pl_ecb->event_result = result;
                if(pl_ecb->event_status.bit.wait_overtime)   //是否在超时队列中
                {
                    __djy_resume_delay(pl_ecb);               //结束超时等待
    //                pl_ecb->event_status.bit.wait_overtime =0;//取消"超时等待中"状态
                }
                pl_ecb->event_status.all =0;
                event_temp = pl_ecb->multi_next;
                __djy_event_ready(pl_ecb);           //把事件加入到就绪队列中
                pl_ecb = event_temp;
            }while(pl_ecb != NULL);
        }
        
        if(cur_para->dynamic_mem == true)
        {
            m_free(cur_para->para);
        }
        cur_para->next = pg_para_free;
        pg_para_free = cur_para;
        cur_para->sync = NULL;
        pg_event_running->para_current = NULL;
    }
    //下面处理完成同步队列，必须联系共享文档的相关章节才容易读懂，注释难于解释
    //那么清楚的。
    pl_evtt->done_sum++;
    pl_ecb = pl_evtt->done_sync;
    while(pl_ecb != NULL)
    {
        do
        {//链表中的事件都是要么没有指定超时，要么时限未到，其他情况不会在此链表
         //中留下痕迹，evtt_done_sync位也已经清除

            //同步条件达成的条件: 1、同步计数器已减至1。
            //2、同步计数器为0且本类型最后一条事件已经处理完
         if((pl_ecb->sync_counter == 1)
                 ||((pl_ecb->sync_counter == 0) 
                         &&(pl_evtt->pop_sum == pl_evtt->done_sum)))
            {
                pl_ecb->last_status.all = pl_ecb->event_status.all;
//                pl_ecb->event_status.bit.evtt_done_sync = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)  //指定的超时未到
                {
                    __djy_resume_delay(pl_ecb);       //从闹钟队列中移除事件
//                    pl_ecb->event_status.bit.wait_overtime = 0;
                }
                pl_ecb->event_status.all = 0;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //是最后一个事件
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next 
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous 
                                        = pl_ecb_temp->multi_previous;
                }
                __djy_event_ready(pl_ecb_temp);
            }else
            {
                if(pl_ecb->sync_counter != 0)
                    pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->done_sync);
    }

    //以下看事件的虚拟机如何处理。
    if(pl_evtt->property.correlative == enum_correlative)      //关联型事件
    {
        if(pl_evtt->vpus != 0)              //防应用程序反复调用本函数
            pl_evtt->vpus--;
        int_restore_asyn_signal();
        return;
    }else   //独立型事件
    {
#if (cfg_dynamic_mem == 1)
        if(pg_event_running->local_memory != 0)
        {
            __m_cleanup(pg_event_running->event_id);//强行清除事件运行中申请的内存
        }
#endif        
        __djy_cut_ready_event(pg_event_running);
        pg_event_running->previous
                        = (struct  event_ECB*)&pg_event_free;//表示本控制块空闲
        pg_event_running->next = pg_event_free;     //pg_event_free是单向非循环队列
        pg_event_free = pg_event_running;
        pg_event_free->event_id = pg_event_free - tg_ECB_table;   //容错用

        //有未得到虚拟机的事件，保留之
        if((pl_evtt->pop_sum - pl_evtt->done_sum) > pl_evtt->vpus)    
        {
            vm_final = cn_keep;
        }else   //没有未得到虚拟机的事件，再看是否需保留
        {
            if(pl_evtt->vpus == 1)   //这是最后一个事件
            {
                pl_evtt->property.inuse = 0;
                if((cn_run_mode!=cn_mode_mp)||(pl_evtt->default_prio<0x80))
                {//运行模式为si或dlsp，或者该事件类型拥有紧急优先级时，
                 //须保留线程虚拟机
                    vm_final = cn_keep;
                }else
                {
                    vm_final = cn_delete;
                }
            }else                       //这不是最后一个事件
            {
                vm_final = cn_delete;
            }
        }
    }

    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //回收事件类型控制块，只需把registered属性清零。
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';   //清空事件类型名
        while(next_vm != NULL)          //释放该事件类型拥有的空闲虚拟机
        {
            temp = next_vm;
            next_vm = next_vm->next;
            m_free((void *)temp);
        }
        vm_final = cn_deleted;
    }

    __djy_select_event_to_run();
    if(vm_final == cn_delete)      //删除虚拟机
    {
        m_free((void*)pg_event_running->vm);    //删除虚拟机
        pl_evtt->vpus--;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_turnto_context(pg_event_running->vm);
    }else if(vm_final == cn_keep)    //保留虚拟机
    {
        pg_event_running->vm->next = pl_evtt->my_free_vm;
        pl_evtt->my_free_vm = pg_event_running->vm;
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }else               //虚拟机已经转交给另一条事件,或已经被删除
    {
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }
}

//----查询唤醒原因-------------------------------------------------------------
//功能: 查询正在执行的事件被执行的原因.
//参数: 无
//返回: 唤醒原因,见源程序注释.
//-----------------------------------------------------------------------------
union event_status djy_wakeup_from(void)
{
    return pg_event_running->last_status;
}

//----查询事件id-------------------------------------------------------------
//功能: 根据提供的id号查询事件指针.
//参数: id,事件id
//返回: 事件id对应的事件指针.
//-----------------------------------------------------------------------------
struct  event_ECB *__djy_lookup_id(u16 id)
{
    return &tg_ECB_table[id];
}
//----虚拟机引擎---------------------------------------------------------------
//功能: 启动虚拟机,执行虚拟机入口函数,事件完成后执行清理工作
//参数: thread_routine 处理该事件类型的函数指针.
//返回: 无
//-----------------------------------------------------------------------------
void __djy_vm_engine(void (*thread_routine)(void))
{
//    int_restore_asyn_signal();   //对应在y_event_done函数中关闭的中断
    thread_routine();   //事件处理函数
    //如果thread_routine中未执行y_event_done而是直接返回，则在这里调用，
    //完成清理工作,然后直接切换到新的ready事件,本函数并不返回.
    djy_event_exit();
}

//----系统服务-----------------------------------------------------------------
//功能: 操作系统的系统服务功能,提供调试,统计等服务.
//参数: 无
//返回: 永不返回.
//惨痛教训:这是一个不允许阻塞的函数，深夜3点调出来的教训。
//-----------------------------------------------------------------------------
void __djy_service(void)
{
    u32 i,j;
    volatile ptu32_t level;
    u32 *stack;
    u32 pads;
    
    pads = ((u32)cfg_stack_fill<<24)
            |((u32)cfg_stack_fill<<16)
            |((u32)cfg_stack_fill<<8)
            |(u32)cfg_stack_fill;
    while(1)
    {
        for(i = 0; i<cn_events_limit; i++)
        {
            if(tg_ECB_table[i].vm == NULL)
                continue;
            level = tg_ECB_table[i].vm->stack_size>>6;
            if((tg_ECB_table[i].vm->stack - (u32*)(tg_ECB_table[i].vm)) < level)
                level = 0;
            stack = (u32*)(tg_ECB_table[i].vm);
            stack = (u32*)(&(tg_ECB_table[i].vm[1]));
            for(j = 0; j < level; j++)
            {
                if(stack[j] != pads)
                    level = 0;
            }
        }
    }
}

//----api启动函数--------------------------------------------------------------
//功能: 根据api号调用相应的api函数.
//参数: api_no,api号
//返回: mp模式才用，暂且放在这里
//-----------------------------------------------------------------------------
void djy_api_start(u32 api_no)
{
    switch(api_no)
    {
        default :break;
    }
    return;
}
//空函数,未初始化的函数指针指向.
void NULL_func(void)
{}

