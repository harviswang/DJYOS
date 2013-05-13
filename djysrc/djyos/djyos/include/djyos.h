//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:核心模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:事件类型、事件管理以及多事件调度相关的代码全在这里了。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __djyos_H__
#define __djyos_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
//结构类型声明,来自其他文件定义的结构
struct semaphore_LCB;
//结构类型声明,本文件定义的结构
struct  thread_vm;
struct  event_ECB;
struct  event_type;

#define mS      1000    //系统时间单位是微秒，想用ms就得*1000

#define cn_invalid_event_id cn_limit_uint16
#define cn_invalid_evtt_id  cn_limit_uint16    //非法事件类型号
#define cn_event_id_limit   32767       //最大事件id号
#define cn_evtt_id_limit    32767       //最大事件类型id号
#define cn_timeout_forever  0xffffffff  //无限延时

#define cn_sync_success     0       //同步条件达成
#define cn_sync_timeout     1       //同步条件未达成，超时返回，或timeout设为0
#define cn_sync_unregister  2       //同步条件未达成，因被同步事件类型被注销。
                                    //用于事件类型相关的同步。
#define cn_sync_error       cn_limit_uint32    //出错

//出错信息定义
enum knl_error_code
{
    enum_knl_no_error = cn_knl_no_error,  //没有错误
    enum_knl_etcb_error,            //事件类型控制块错误
    enum_knl_ecb_exhausted,         //事件控制块耗尽
    enum_knl_etcb_exhausted,        //事件类型控制块耗尽
    enum_knl_pcb_exhausted,         //参数控制块耗尽
    enum_knl_cant_sched,            //企图在禁止调度时执行可能引起事件调度的操作
    enum_knl_invalid_prio,          //非法优先级
    enum_knl_vpu_over,              //事件类型的虚拟机超过限制数
    enum_knl_para_over,             //事件参数数量超过限制数
    enum_knl_evtt_homonymy,         //事件类型重名
    enum_knl_event_free,            //企图使用空闲事件控制块
    enum_knl_evtt_free,             //企图使用空闲事件类型控制块
    enum_knl_evttid_limit,          //事件类型id越限
    enum_knl_evtt_unregister,       //事件类型未登记
    enum_knl_event_sync_exit,       //事件同步中，被同步的事件异常结束
    enum_knl_memory_over,           //内存不足
};

//事件优先级名称定义
#define cn_prio_critical    (100)
#define cn_prio_real        (130)
#define cn_prio_RRS         (200)
#define cn_prio_wdt         (1)
#define cn_prio_sys_service (250)
#define cn_prio_invalid     (255)   //非法优先级

//container_of宏的定义,这是来自linux的代码,但被修改成更广泛支持的c90格式
//container_of - 根据结构成员指针计算容器结构地址
//ptr:      结构成员指针
//type:     容器结构的类型
//member:   成员名称
#define container_of(container,ptr, type, member) do{         \
        const void *__mptr = (void*)(ptr);    \
        container = (type *)( (char *)__mptr - offsetof(type,member) );}while(0)

struct process_vm       //进程虚拟机
{
    u8  res;
};

#define cn_save_context_int     1
#define cn_save_context_noint   0
//有mmu的机器上,地址分配:0~1G操作系统,1~2G共享虚拟机,2~4G虚拟机的其他空间.
//特别注意:本结构要被汇编访问，其成员顺序不能改变，也不能随意增加成员
struct  thread_vm        //虚拟机数据结构
{
    //虚拟机栈指针,在虚拟机被抢占时保存sp,运行时并不动态跟踪sp变化
    u32    *stack;
    u32    *stack_top;     //虚拟机的栈顶指针
    struct thread_vm *next;     //用于把evtt的所有空闲虚拟机连成一个单向开口链表
                                //该链表由evtt的my_free_vm指针索引
    u32    stack_size;     //栈深度
    struct process_vm *host_vm;  //宿主进程虚拟机，在si和dlsp模式中为NULL
};

//事件进入运行态的原因.
//事件应该是内闭的,djyos不提供查询事件当前状态的功能.如果提供查询功能,那么,执行
//查询的必定是当前运行的服务,其自身的当前状态必定是运行态,查询自身是多此一举.
//而查询别人的状态,有干涉内政之嫌.
//使用方法:当事件进入运行态时,根据进入前状态设置事件的last_status.
//查询函数根据last_status值判断事件从何种状态进入运行态.
//因优先级低而等待的不予考虑,例如,某事件延时结束后因为优先级低而等待一定时间后
//才运行,查询结果为"闹铃响"
struct event_status_bit             //就绪态的事件所有位均＝0
{
    uint16_t event_delay:1;         //闹钟同步
    uint16_t wait_overtime:1;       //超时等待
    uint16_t event_sync:1;          //事件同步
    uint16_t event_sync_exit:1;     //事件同步中被同步的目标事件异常退出
    uint16_t evtt_pop_sync:1;       //事件类型弹出同步
    uint16_t evtt_done_sync:1;      //事件类型完成同步
    uint16_t wait_memory:1;         //从系统堆分配内存同步
    uint16_t wait_semp:1;           //信号量同步
    uint16_t wait_mutex :1;         //互斥量同步
    uint16_t wait_asyn_signal:1;    //异步信号同步
    uint16_t wait_cpipe:1;          //定长pipe
    uint16_t wait_vpipe:1;          //动态长度pipe
};

union event_status
{
    uint16_t all;
    struct event_status_bit bit;
};

#define cn_db_info_event       0
#define cn_db_info_evtt        1

#define cn_creat_new_para   0x01    //总是为单线程事件创建新参数
#define cn_malloc_over_32   0x02    //参数超过32bytes时，使用malloc分配空间
//说明:
//1.弹出事件时，可
struct  para_record 
{
    struct  para_record *next,*previous;
    struct event_ECB *sync;     //等待本参数所要求的任务处理完成的事件队列，
                                //若为NULL,则不等待处理，直接返回
    u32 start_time;             //参数产生的时间，操作系统的ticks数，非uS
    bool_t dynamic_mem;         //true=保存参数的内存是动态申请的
    void *para;
    u8 static_para[32];         //事件参数，格式由弹出和处理事件的双方协商确定.
                                //若参数尺寸少于32字节，直接存在这里
};

enum pop_para_sync
{
    enum_para_async=0,          //弹出事件不要求同步
    enum_para_sync,             //弹出事件要求同步，即新事件处理完成再返回
};
struct event_ECB
{
    //事件链指针,用于构成下列链表
    //pg_event_free: 特殊，参见文档
    //pg_event_ready:特殊链表，参见文档
    //pg_event_delay:双向循环
    struct  event_ECB *next,*previous;
    //多功能链表指针，用于连接以下链表:
    //1、各同步队列，比如事件同步，事件类型弹出同步等
    //2、就绪队列中优先级单调队列，双向循环，用以实现轮询和y_event_ready中
    //   的O(1)算法。
    struct  event_ECB *multi_next,*multi_previous;      //条件同步队列
    struct  thread_vm  *vm;             //处理本事件的线程虚拟机指针,
    struct  para_record *para_high_prio;//高优先级子事件产生的任务
    struct  para_record *para_low_prio; //低优先级子事件产生的任务
    struct  para_record *para_current;  //当前将要或正在处理的事件。
    struct  event_ECB *sync;            //同步于本事件的队列，当本事件完成后，
                                        //激活队列中的事件
                                        //与参数同步不一样，参数同步是弹出时用的
    struct  event_ECB **sync_head;      //记住自己在哪一个同步队列中，以便超时
                                        //返回时从该同步队列取出事件
#if(cfg_debug_info == 1)
    u32    start_time;     //事件发生时间，操作系统的ticks数，非uS
    u32    consumed_time;  //事件消耗的总时间
    u32    consumed_time_second;    //最近1秒消耗的时间
    u32    consumed_time_record;    //消耗时间的整秒记录
#endif
    u32    delay_start_tick;    //设定闹铃时间
    u32    delay_end_tick;      //闹铃响时间
    u32    error_no;       //本事件执行产生的最后一个错误号
    ptu32_t     event_result;   //如果本事件处理时弹出了事件，并且等待处理结果
                                //(即调用pop函数时，sync=true)，且正常返回，这里
                                //保存该事件处理结果。
    u32    wait_mem_size;  //等待分配的内存数量.
    union event_status last_status;    //最后状态,用于查询事件进入运行态前的状态
    union event_status event_status;   //当前状态,本变量由操作系统内部使用,
    ufast_t     prio;       //事件优先级，0~255,事件一经弹出，就不能再改变优先级
//    ufast_t     prio_bak;   //优先级备份，用于优先级继承中备份被提升优先级
                            //的事件的优先级。

    uint16_t    evtt_id;                //事件类型id，0~32767
    uint16_t    sync_counter;           //同步计数
    //事件id范围:0~32767(cn_event_id_limit)
    uint16_t    event_id;   //事件序列编号,等同于事件在事件块数组中的偏移位置
    u32    local_memory;       //本事件处理过程中申请的局部内存块数
                                    //不收回将导致内存泄漏.
};

//事件属性定义表
struct evtt_status
{
    u16 correlative:1;  //0=表示独立事件,事件队列中可能同时存在多条该类事件
                        //1=同一类型的多次事件间是有关联的，必须协作处理
                        //故事件队列中只允许存在一条该事件
    u16 registered:1;   //0=该事件类型还没有注册,系统将拒绝pop该类型录
                        //1=该事件类型已经注册,可以pop该类型事件
    u16 inuse:1;        //0=所有队列中都没有该类型的事件
                        //1=队列中(包括等待中)至少有一条该类型的事件
    u16 deleting:1;     //0=正常状态，1=等待注销状态。
};

enum event_relation
{
    enum_independence=0,//表示独立事件,事件队列中可能同时存在多条该类事件
    enum_correlative,   //关联事件，同一类型的多次事件间是有关联的，必须协作处理
                        //故事件队列中只允许存在一条该事件
};
struct event_type
{
    //同一类型的事件可以有多条正在执行或等待调度,但这些事件有相同的属性.
    struct evtt_status    property;
    //空闲线程指针,分配线程给事件时，优先从这里分配
    struct  thread_vm  *my_free_vm;
    char evtt_name[32]; //事件类型允许没有名字，但只要有名字，就不允许同名
                        //如果一个类型不希望别的模块弹出事件，可以不用名字。
                        //如模块间需要交叉弹出事件，用名字访问。
    //优先级小于0x80为紧急优先级,它影响虚拟机的构建.类型优先级在初始化时设定,
    ufast_t     default_prio;       //事件类型优先级.不同于事件优先级,1~255,0非法.
    uint16_t    pop_sum,done_sum;   //已弹出事件总数，已完成事件总数
    uint16_t    vpus_res;   //系统繁忙时为本类型事件保留的虚拟机数量，关联型无效
    uint16_t    vpus_limit; //独立型:本类型事件允许同时建立的线程虚拟机个数
                            //关联型:参数队列中可同时容纳的参数个数。
    uint16_t    vpus;       //独立型:本类型事件已经拥有的线程虚拟机个数
                            //关联型:本类型事件参数队列中的参数个数
    void (*thread_routine)(void);//函数指针,可能是死循环.
    u32 stack_size;              //thread_routine所需的栈大小

    struct event_ECB *mark_event; //关联型事件才有效，指向事件队列中的本类型事件
    //这两队列都是以剩余次数排队的双向循环链表
    struct  event_ECB *done_sync,*pop_sync;//弹出同步和完成同步队列头指针,
};

//就绪队列(优先级队列),始终执行队列头部的事件,若有多个优先级相同,轮流执行
extern struct  event_ECB  *pg_event_ready;
extern struct  event_ECB  *pg_event_running;   //当前正在执行的事件
extern bool_t bg_schedule_enable;

void __djy_set_delay(void);
sint32_t strnlen(const char *s,s32 maxlen);
void djy_isr_tick(void);
void djy_error_login(u32 error_type,const char *text);
void __djy_cut_ready_event(struct event_ECB *event);
void djy_set_RRS_slice(u32 slices);
u32 djy_get_RRS_slice(void);
u32 djy_get_last_error(char *text);
u32 djy_get_RRS_slice(void);
void __djy_select_event_to_run(void);
void djy_create_process_vm(void);
bool_t __djy_schedule(void);
void __djy_schedule_asyn_signal(void);
uint16_t djy_evtt_regist(enum event_relation relation,
                        ufast_t default_prio,
                        u16 vpu_res,
                        uint16_t vpus_limit,
                        void (*thread_routine)(void),
                        u32 stack_size,
                        char *evtt_name);
uint16_t djy_get_evtt_id(char *evtt_name);
bool_t djy_evtt_unregist(uint16_t evtt_id);
void __djy_init_sys(void);
bool_t djy_query_sch(void);
void __djy_event_ready(struct  event_ECB *event_ready);
void __djy_resume_delay(struct  event_ECB *delay_event);
void __djy_addto_delay(u32 u32l_uS);
bool_t djy_set_event_prio(ufast_t new_prio);
u32 djy_event_delay(u32 u32l_uS);
u32 djy_wait_event_completed(uint16_t event_id,u32 timeout);
u32 djy_wait_evtt_completed(uint16_t evtt_id,uint16_t done_times,u32 timeout);
u32 djy_wait_evtt_pop(uint16_t evtt_id,uint16_t pop_times,u32 timeout);
u16 djy_event_pop(  u16  evtt_id,
                    enum pop_para_sync para_sync,
                    u32 *pop_result,
                    u32 timeout,
                    void *para,
                    u32 para_size,
                    ufast_t para_options,
                    ufast_t prio);
ptu32_t djy_get_event_result(void);
void *djy_get_event_para(u32 *time);
void __djy_init_tick(void);
void __djy_start_os(void);
void djy_event_exit(void);
void djy_task_completed(ptu32_t result);
union event_status djy_wakeup_from(void);
struct  event_ECB *__djy_lookup_id(uint16_t id);
void __djy_vm_engine(void (*thread_routine)(void));
void __djy_service(void);
u16 djy_my_evtt_id(void);
void djy_api_start(u32 api_no);
void NULL_func(void);

#ifdef __cplusplus
}
#endif
#endif //__djyos_H__
