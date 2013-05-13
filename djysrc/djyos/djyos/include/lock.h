//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 锁模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 包含信号量和互斥量
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __semp_h__
#define __semp_h__

#ifdef __cplusplus
extern "C" {
#endif


struct  event_ECB;

// 当事件请求信号量不得，将进入该信号量的同步队列，等待持有信号量的事件释放信号
// 量，等待队列有两种排序方式，即优先级顺序和时间顺序方式。
// 只有信号量可以用先后顺序，互斥量只能用优先级排序方式。
enum semp_sync_sort
{
    sort_prio = 0,  //按请求信号量的事件的优先级排队
    sort_time,      //按请求信号量的先后顺序排队
};
//---------不限量信号的意义--------------
//1、不限量信号可提供受保护的资源的使用计数
//2、为可限量也可不限量的资源提供一致的代码，尤其是提供在运行时配置的可能性
struct semaphore_LCB
{
    struct  rsc_node node;
    enum semp_sync_sort sync_order;
    u32 lamps_limit;   //信号灯数量上限，cn_limit_uint32表示不限数量
    u32 lamp_counter;  //可用信号灯数量。
    u32 lamp_used;     //该信号灯被请求的数量，次数溢出时将环绕到0
    struct  event_ECB *semp_sync;    //等候信号的事件队列，优先级排队
};

struct mutex_LCB
{
    struct  rsc_node node;
    bool_t  enable;
    ufast_t  prio_bak;          //优先级继承中备份原优先级
    struct  event_ECB *owner;        //持有互斥量的事件
    struct  event_ECB *mutex_sync;    //等候信号的事件队列，优先级排队
};

//用于信号量和互斥量共享内存池
union lock_MCB
{
    struct semaphore_LCB sem;
    struct mutex_LCB  mut;
};

//出错信息定义
enum lock_error_code
{
    enum_lock_no_error = cn_lock_no_error,      //没有错误
    enum_lock_range_error,            //信号量范围超过限制
};

ptu32_t module_init_lock1(ptu32_t para);
ptu32_t module_init_lock2(ptu32_t para);
struct semaphore_LCB *semp_create(u32 lamps_limit,u32 init_lamp,char *name);
void semp_create_knl( struct semaphore_LCB *semp,
                       u32 lamps_limit,u32 init_lamp,char *name);
void semp_post(struct semaphore_LCB *semp);
bool_t semp_pend(struct semaphore_LCB *semp,u32 timeout);
bool_t semp_delete_knl(struct semaphore_LCB *semp);
bool_t semp_delete(struct semaphore_LCB *semp);
u32 semp_query_used(struct semaphore_LCB *semp);
u32 semp_query_capacital(struct semaphore_LCB *semp);
u32 semp_query_free(struct semaphore_LCB *semp);
void semp_set_sync_sort(struct semaphore_LCB *semp,enum semp_sync_sort order);
struct mutex_LCB *mutex_create(char *name);
void mutex_create_knl( struct mutex_LCB *mutex,char *name);
void mutex_post(struct mutex_LCB *mutex);
bool_t mutex_pend(struct mutex_LCB *mutex,u32 timeout);
bool_t mutex_delete(struct mutex_LCB *mutex);
bool_t mutex_delete_knl(struct mutex_LCB *mutex);
bool_t mutex_query(struct mutex_LCB *mutex);

#ifdef __cplusplus
}
#endif

#endif //__semp_h__

