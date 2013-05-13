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
#include "os_inc.h"
static struct semaphore_LCB *pg_semaphore_rsc=NULL;
static struct mutex_LCB  *pg_mutex_rsc=NULL;
//说明： cn_locks_limit 是用户配置的，由于用户并不知道操作系统需要用多少信号量，
//      所以操作系统并不占用 cn_locks_limit 指标，用户使用的信号量从
//      tg_semp_pool定义的内存池中分配，操作系统使用的信号量自己定义，两不
//      相关，如果用户企图调用 semp_delete 删除操作系统定义的信号量，将引起造成
//      内存泄漏，而且相关操作系统资源也将失去信号量保护，后果不可预料
//定义信号量结构初始内存池
#if(cn_locks_limit != 0)
static union lock_MCB tg_lock_mem_block[cn_locks_limit];
#endif
static struct mem_cell_pool *pg_lock_pool;  //信号量结构内存池头指针

//这是在rsc模块中定义的，专供lock模块使用的一个函数
struct  rsc_node * __rsc_add_lock_tree(struct  rsc_node *node,
                                u32 size,char *name);

//----初始化锁模块模块step1----------------------------------------------------
//功能：初始化信号量模块的第一步，此后可以调用除semp_create和mutex_createe以外的
//      其他函数，包括__semp_create_knl和__mutex_createe_knl，该两函数被禁止是因
//      为module_init_memb还没有调用，又因mudule_init_memb需要使用信号量，故只能
//      在module_init_lock1后面调用。
//参数：无
//返回：1
//-----------------------------------------------------------------------------
ptu32_t module_init_lock1(ptu32_t para)
{
    static struct semaphore_LCB semp_root;
    static struct mutex_LCB mutex_root;
    pg_semaphore_rsc = (struct semaphore_LCB*)
        __rsc_add_lock_tree(&semp_root.node,sizeof(union lock_MCB),"semaphore");
    pg_mutex_rsc = (struct mutex_LCB*)
        __rsc_add_lock_tree(&mutex_root.node,sizeof(union lock_MCB),"mutex");
    return 1;
}

//----初始化锁模块模块step2----------------------------------------------------
//功能：初始化信号量模块的第二步，此后可以调用含semp_create和mutex_createe在内的
//      所有锁函数了
//参数：无
//返回：1
//-----------------------------------------------------------------------------
ptu32_t module_init_lock2(ptu32_t para)
{
    static struct mem_cell_pool lock_pool;
    //初始化锁控制块内存池
#if(cn_locks_limit != 0)
    pg_lock_pool = mb_create_pool_knl( &lock_pool,
                                    (void*)tg_lock_mem_block,
                                    cn_locks_limit,
                                    sizeof(union lock_MCB),
                                    0,0,
                                    "锁控制块池");
#else
    pg_lock_pool = mb_create_pool_knl( &lock_pool,
                                    NULL,
                                    cn_locks_limit,
                                    sizeof(union lock_MCB),
                                    0,0,
                                    "锁控制块池");
#endif
    return 1;
}

//----创建一个信号量-----------------------------------------------------------
//功能：建立一个信号量，以后就可以使用这个信号量了。
//参数：semp_limit,信号灯的总数，cn_limit_uint32=有无限多的信号灯。
//      init_lamp,初始信号灯数量
//      name，信号量的名字，所指向的字符串内存区不能是局部变量，可以是空
//返回：新建立的信号量指针
//-----------------------------------------------------------------------------
struct semaphore_LCB *semp_create(u32 lamps_limit,
                                 u32 init_lamp,char *name)
{
    struct semaphore_LCB *semp;
    if(init_lamp > lamps_limit)
        return NULL;
    semp = mb_malloc(pg_lock_pool,0);
    if(semp == NULL)
        return NULL;
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->lamp_used = 0;
    semp->semp_sync = NULL;
    //把新节点挂到信号量根节点下
    rsc_add_son(&pg_semaphore_rsc->node,&semp->node,
                        sizeof(struct semaphore_LCB),name);
    return semp;
}

//----创建一个内核信号量-------------------------------------------------------
//功能：创建一个内核信号量，与所有_knl为后缀的函数一样，cn_locks_limit是用户定义
//      的，为保持模块的独立性，内核不与用户争用cn_locks_limit指标，故内核使用的
//      锁(信号量和互斥量)用单独的函数创建和删除。争用的话，将导致内核和应用程序
//      互相耦合
//      与semp_create的区别是，本函数需要调用者提供semp实体，而不是从池中分配。
//参数：semp,目标信号量指针
//      semp_limit,信号灯的总数，cn_limit_uint32=有无限多的信号灯。
//      init_lamp,初始信号灯数量
//      name，信号量的名字，所指向的字符串内存区不能是局部变量，可以是空
//返回：新建立的信号量指针
//-----------------------------------------------------------------------------
void semp_create_knl( struct semaphore_LCB *semp,
                       u32 lamps_limit,u32 init_lamp,char *name)
{
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->lamp_used = 0;
    semp->semp_sync = NULL;
    //把新节点挂到信号量根节点下
    rsc_add_son(&pg_semaphore_rsc->node,&semp->node,
                        sizeof(struct semaphore_LCB),name);
}

//----点亮一个信号灯------------------------------------------------------------
//功能：释放一个信号量，可用信号量加1。
//参数：semp,信号量指针
//返回：无
//-----------------------------------------------------------------------------
void semp_post(struct semaphore_LCB *semp)
{
    struct  event_ECB *event;

    if(semp == NULL)
        return;
    int_save_asyn_signal();
    semp->lamp_used--;
    if(semp->semp_sync == NULL)     //等待队列空，点亮一盏信号灯
    {
        if(semp->lamps_limit == cn_limit_uint32)        //信号灯数量不限
        {
            if(semp->lamp_counter != cn_limit_uint32)   //信号灯数量未到上限
                semp->lamp_counter ++;                  //点亮一盏信号灯
        }else if(semp->lamp_counter < semp->lamps_limit )//信号灯数量未到上限
            semp->lamp_counter++;                       //点亮一盏信号灯
//        else
//            djy_error_login(enum_lock_range_error,semp->node.name);
    } else      //有事件在等待，把信号灯直接转交给队列中优先级最高的事件
    {
        event = semp->semp_sync;
        event->sync_head = NULL;
        if(event->multi_next == event)
        {//队列中只有一个事件
            semp->semp_sync = NULL;
        }else
        {//队列中有多个事件，取出队列头部的事件
            semp->semp_sync = event->multi_next;
            event->multi_next->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = event->multi_next;
            event->multi_next = NULL;
            event->multi_previous = NULL;
        }
        event->last_status.all = event->event_status.all;//备份事件状态
        if(event->event_status.bit.wait_overtime)
            __djy_resume_delay(event);    //如果事件在超时等待队列中，取出
        event->event_status.all = 0;
//        event->event_status.bit.wait_semp = 0; //清除等待状态
        __djy_event_ready(event);
    }
    int_restore_asyn_signal();
}

//----请求一盏信号灯-----------------------------------------------------------
//功能：请求然后熄灭一盏信号灯，表示可用资源数减1。
//参数：semp,信号量指针
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回：true=取得信号返回或资源不受信号灯保护(semp == NULL)，semp==NULL时返回
//      true是有意义的，如果你希望代码在不管是否受信号灯保护都保持一致，把semp
//      设为NULL是明智的选择，比如你要构建一个不受信号灯保护的设备，可把该设备
//      struct pan_device的semp成员设为NULL。
//      false=没有取得信号(超时返回或其他原因)
//-----------------------------------------------------------------------------
bool_t semp_pend(struct semaphore_LCB *semp,u32 timeout)
{
    struct  event_ECB *event;
    atom_low_t atom_low;
    bool_t lamp;

    if(semp == NULL)
        return true;
    //以下读取信号灯数量，必须保证原子操作，故关中断
    atom_low = int_low_atom_start(  );             //本函数对不能嵌套调用
    semp->lamp_used ++;
    if(semp->lamps_limit == cn_limit_uint32)   //本信号量有无限多信号灯
    {
        if(semp->lamp_counter != 0) //信号灯数量不为0
            semp->lamp_counter--;   //熄灭一盏信号灯
        lamp = true;
    }else                           //本信号量的信号灯数量有限
    {
        if(semp->lamp_counter > 0)  //有点亮的信号灯
        {
            lamp = true;            //标记资源可用
            semp->lamp_counter--;   //熄灭一盏信号灯
        }else                       //没有点亮的信号灯
            lamp = false;           //标记资源不可用
    }
    int_low_atom_end( atom_low );

    if(lamp == true)
        return true;    //取得信号灯返回
    else if(!djy_query_sch() || (timeout == 0))//禁止调度时,不能阻塞.或timeout为0
        return false;   //没有取得信号灯返回

    int_save_asyn_signal();
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->previous = NULL;
    pg_event_running->next = NULL;

    pg_event_running->sync_head = &semp->semp_sync;
    if(semp->semp_sync == NULL)
    {//同步队列空,running事件自成双向循环链表
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
        semp->semp_sync = pg_event_running;
    }else
    {
        event = semp->semp_sync;
        if(semp->sync_order == sort_prio)   //同步队列按优先级排序
        {
            do
            { //找到一个优先级低于新事件的事件.
                if(event->prio <= pg_event_running->prio)
                    event = event->multi_next;
                else
                    break;
            }while(event != semp->semp_sync);
            pg_event_running->multi_next = event;
            pg_event_running->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pg_event_running;
            event->multi_previous = pg_event_running;
            if(semp->semp_sync->prio > pg_event_running->prio)
                semp->semp_sync = semp->semp_sync->multi_previous;
        }else                               //按先后顺序，新事件直接排在队尾
        {
            pg_event_running->multi_next = event;
            pg_event_running->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pg_event_running;
            event->multi_previous = pg_event_running;
        }
    }
    pg_event_running->last_status.all =pg_event_running->event_status.all;
    pg_event_running->event_status.bit.wait_semp = 1;  //事件状态设为等待信号量
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    int_restore_asyn_signal();  //恢复中断，将触发上下文切换
    int_save_asyn_signal();     //此时，已经是重新获得信号量，上下文切回
    //检查从哪里返回，是超时还是同步事件完成。
    if(pg_event_running->event_status.bit.wait_semp)
    {//说明同步条件未到，从超时返回，应从目标事件的同步队列中取出事件。
     //此时，被同步的事件肯定还没有完成。
        pg_event_running->event_status.bit.wait_semp = 0;
        int_restore_asyn_signal();
        return false;
    }else
    {//说明是得到信号量返回
        int_restore_asyn_signal();
        return true;
    }
}

//----删除内核信号量-----------------------------------------------------------
//功能: 删除一个内核信号量，与所有_knl为后缀的函数一样，cn_locks_limit是用户定义
//      的，为保持模块的独立性，内核不与用户争用cn_locks_limit指标，故内核使用的
//      锁(信号量和互斥量)用单独的函数申请和删除。争用的话，将导致内核和应用程序
//      互相耦合
//      本函数和semp_delete的差别在于本函数不释放信号量控制块。
//参数：semp，被删除的信号量
//返回：无
//-----------------------------------------------------------------------------
bool_t semp_delete_knl(struct semaphore_LCB *semp)
{
    bool_t result;
    if(semp == NULL)    //参数错误
        return false;
    int_save_asyn_signal();
    if(semp->semp_sync == NULL)     //没有事件在等待信号灯
    {
        rsc_del_node(&semp->node);  //删除信号量结点
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}

//----删除一个信号量-----------------------------------------------------------
//功能：删除一个信号量
//参数：semp，被删除的信号量
//返回：无
//-----------------------------------------------------------------------------
bool_t semp_delete(struct semaphore_LCB *semp)
{
    bool_t result;
    if(semp == NULL)    //参数错误
        return false;
    int_save_asyn_signal();
    if(semp->semp_sync == NULL)     //没有事件在等待信号灯
    {
        rsc_del_node(&semp->node);  //删除信号量结点
        mb_free(pg_lock_pool,semp); //释放内存
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}

//----查询一个信号量-----------------------------------------------------------
//功能：查询信号灯以被请求的数量
//参数：semp，被查询的信号量
//返回：请求次数
//-----------------------------------------------------------------------------
u32 semp_query_used(struct semaphore_LCB *semp)
{
    u32 result;
    if(semp == NULL)            //参数错误
        return 0;
    else
    {
#if (32 > cn_cpu_bits)
        atom_low_t atom_low;
        atom_low = int_low_atom_start();
#endif
        result = semp->lamp_used;     //返回已使用的信号灯数量
#if (32 > cn_cpu_bits)
        int_low_atom_end();
#endif
    }
    return result;
}

//----查询信号量容量-----------------------------------------------------------
//功能：查询一个信号量包含信号灯的总数
//参数：semp，被查询的信号量
//返回：信号灯总数
//-----------------------------------------------------------------------------
u32 semp_query_capacital(struct semaphore_LCB *semp)
{
    u32 result;
    if(semp == NULL)            //参数错误
        return 0;
    else
    {
#if (32 > cn_cpu_bits)
        atom_low_t atom_low;
        atom_low = int_low_atom_start();
#endif
        result = semp->lamps_limit;     //返回已使用的信号灯数量
#if (32 > cn_cpu_bits)
        int_low_atom_end();
#endif
    }
    return result;
}
//----查询可用信号灯数量-------------------------------------------------------
//功能：查询一个信号量可用信号灯的数量
//参数：semp，被查询的信号量
//返回：信号灯总数
//-----------------------------------------------------------------------------
u32 semp_query_free(struct semaphore_LCB *semp)
{
    u32 result;
    if(semp == NULL)            //参数错误
        return 0;
    else
    {
#if (32 > cn_cpu_bits)
        atom_low_t atom_low;
        atom_low = int_low_atom_start();
#endif
        result = semp->lamp_counter;     //返回已使用的信号灯数量
#if (32 > cn_cpu_bits)
        int_low_atom_end();
#endif
    }
    return result;
}

//----设置同步队列排序方式-----------------------------------------------------
//功能: 设置等待信号量的事件的排序方式，可设为按发出请求的时间排序，或者按被阻塞
//      的事件的优先级排序。信号量初创建的时候，默认是按优先级排序的。
//      该设置只影响调用本函数后加入队列的事件，原来在队列中的事件不受影响。
//参数: semp: 被设置的信号量
//      order: sort_prio=优先级排队，sort_time=先后顺序排队
//返回: 无
//-----------------------------------------------------------------------------
void semp_set_sync_sort(struct semaphore_LCB *semp,enum semp_sync_sort order)
{
    if(NULL == semp)
        return;
    semp->sync_order = order;
}
//-----------------------下面是互斥量------------------------------------------

//----创建一个信号量-----------------------------------------------------------
//功能：建立一个互斥量。
//参数：name，互斥量的名字，所指向的字符串内存区不能是局部变量，可以是空
//返回：新建立的互斥量指针
//-----------------------------------------------------------------------------
struct mutex_LCB *mutex_create(char *name)
{
    struct mutex_LCB *mutex;
    mutex = mb_malloc(pg_lock_pool,0);
    if(mutex == NULL)
        return NULL;
    mutex->enable = true;   //互斥量的初始状态是可用的,否则必须指定onwer
    mutex->mutex_sync = NULL;
    mutex->prio_bak = cn_prio_invalid;
    mutex->owner = NULL;
    //把新节点挂到信号量根节点下
    rsc_add_son(&pg_mutex_rsc->node,&mutex->node,
                        sizeof(struct mutex_LCB),name);
    return mutex;
}

//----创建一个内核互斥量-------------------------------------------------------
//功能：创建一个内核互斥量，与所有_knl为后缀的函数一样，cn_locks_limit是用户定义
//      的，为保持模块的独立性，内核不与用户争用cn_locks_limit指标，故内核使用的
//      锁(信号量和互斥量)用单独的函数创建和删除。争用的话，将导致内核和应用程序
//      互相耦合。
//      与mutex_createe的区别是，本函数需要调用者提供mutex实体，而不从池中分配。
//参数：mutex,目标信号量指针
//      init_lamp,初始信号灯状态,true=资源可用，false=资源不可用
//      name，互斥量的名字，所指向的字符串内存区不能是局部变量，可以是空
//返回：新建立的信号量指针
//-----------------------------------------------------------------------------
void mutex_create_knl( struct mutex_LCB *mutex,char *name)
{
    mutex->enable = true;   //互斥量的初始状态是可用的,否则必须指定onwer
    mutex->mutex_sync = NULL;
    mutex->prio_bak = cn_prio_invalid;
    mutex->owner = NULL;
    //把新节点挂到信号量根节点下
    rsc_add_son(&pg_mutex_rsc->node,&mutex->node,
                        sizeof(struct mutex_LCB),name);
}

//----释放一个互斥量-----------------------------------------------------------
//功能：释放互斥量
//参数：mutex,互斥量指针
//返回：无
//-----------------------------------------------------------------------------
void mutex_post(struct mutex_LCB *mutex)
{
    struct  event_ECB *event;

    if(mutex == NULL)
        return;
    if(mutex->owner != pg_event_running)    //互斥量只能由拥有者释放
        return;
    int_save_asyn_signal();
    if(mutex->mutex_sync == NULL)     //等待队列空，设置互斥量为可用状态
    {
        mutex->enable = true;       //互斥量可用
    } else      //有事件在等待，把互斥量直接转交给队列中优先级最高的事件
    {
        event = mutex->mutex_sync;
        event->sync_head = NULL;
        if(event->multi_next == event)
        {//队列中只有一个事件
            mutex->mutex_sync = NULL;
        }else
        {//队列中有多个事件，取出队列头部的事件
            mutex->mutex_sync = event->multi_next;
            event->multi_next->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = event->multi_next;
            event->multi_next = NULL;
            event->multi_previous = NULL;
        }
        event->last_status.all = event->event_status.all;//备份事件状态
        if(event->event_status.bit.wait_overtime)
            __djy_resume_delay(event);    //如果事件在超时等待队列中，取出
        event->event_status.all = 0;
//        event->event_status.bit.wait_mutex = 0; //清除等待状态
        __djy_event_ready(event);
        if(mutex->prio_bak != cn_prio_invalid)  //该互斥量发生了优先级继承
        {
            __djy_cut_ready_event(pg_event_running);    //取出running事件
            pg_event_running->prio = mutex->prio_bak;   //恢复优先级
            mutex->prio_bak = cn_prio_invalid;
            __djy_event_ready(pg_event_running); //重新把running插入就绪队列(新位置)
        }
    }
    int_restore_asyn_signal();
}

//----请求互斥量-----------------------------------------------------------
//功能：请求互斥量，若成功则拥有该互斥量。由于有owner成员，互斥量不适合在事件之
//      间或事件与异步信号之间同步。只有存在owner成员，才能实现优先级继承。
//参数：mutex,互斥量指针
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回：true=取得信号返回或资源不受信号灯保护(mutex == NULL)，mutex==NULL时返回
//      true是有意义的，如果你希望代码在不管是否受信号灯保护都保持一致，把semp
//      设为NULL是明智的选择，比如你要构建一个不受信号灯保护的设备，可把该设备
//      struct pan_device的semp成员设为NULL。
//      false=没有取得信号(超时返回或其他原因)
//-----------------------------------------------------------------------------
bool_t mutex_pend(struct mutex_LCB *mutex,u32 timeout)
{
    struct  event_ECB *pl_ecb;
    atom_low_t atom_low;
    bool_t lamp;

    if(mutex == NULL)
        return true;
    //以下读取信号灯数量，必须保证原子操作，故关中断
    atom_low = int_low_atom_start(  );             //本函数对不能嵌套调用
    if(mutex->enable == true)   //信号灯可用
    {
        lamp = true;
        mutex->enable = false;
        mutex->prio_bak = cn_prio_invalid;
        mutex->owner = pg_event_running;
    }else                       //信号灯不可用
    {
        lamp = false;
    }
    int_low_atom_end( atom_low );

    if(lamp == true)
        return true;    //取得互斥量返回
    else if(!djy_query_sch() || (timeout == 0))//禁止调度时,不能阻塞.或timeout=0
        return false;   //没有取得互斥量返回

    int_save_asyn_signal();
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->previous = NULL;
    pg_event_running->next = NULL;

    pg_event_running->sync_head = &mutex->mutex_sync;
    if(mutex->mutex_sync == NULL)
    {//同步队列空,running事件自成双向循环链表
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
        mutex->mutex_sync = pg_event_running;
    }else
    {//同步队列非空,按优先级排序
        pl_ecb = mutex->mutex_sync;
        do
        { //找到一个优先级低于新事件的事件.
            if(pl_ecb->prio <= pg_event_running->prio)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != mutex->mutex_sync);
        pg_event_running->multi_next = pl_ecb;
        pg_event_running->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = pg_event_running;
        pl_ecb->multi_previous = pg_event_running;
        if(mutex->mutex_sync->prio > pg_event_running->prio)
            mutex->mutex_sync = mutex->mutex_sync->multi_previous;
    }
    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.wait_mutex = 1;  //事件状态设为等待互斥量
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    //下面看看是否要做优先级继承
    pl_ecb = mutex->owner;
    if(pl_ecb->prio > pg_event_running->prio) //需要继承优先级
    {//1、处于就绪态，2、处于某种阻塞态。
        if(pl_ecb->event_status.all == 0)//占用互斥量的事件处于就绪态
        {
            __djy_cut_ready_event(pl_ecb);
            //prio_bak保留事件的原始优先级，防止出现多次继承中被修改为中间优先级
            if(mutex->prio_bak == cn_prio_invalid)
                mutex->prio_bak = pl_ecb->prio;
            pl_ecb->prio = pg_event_running->prio;
            __djy_event_ready(pl_ecb);
        }else       //占用互斥量的事件处于某种阻塞态，暂不处理
        {
        }
    }
    int_restore_asyn_signal();  //恢复中断，将触发上下文切换
    int_save_asyn_signal();     //此时，已经是重新获得互斥量，上下文切回
    //检查从哪里返回，是超时还是同步事件完成。
    if(pg_event_running->event_status.bit.wait_mutex)
    {//说明同步条件未到，从超时返回，应从目标事件的同步队列中取出事件。
     //此时，被同步的事件肯定还没有完成。
        pg_event_running->event_status.bit.wait_mutex = 0;
        int_restore_asyn_signal();
        return false;
    }else
    {//说明是得到互斥量返回
        int_restore_asyn_signal();
        return true;
    }
}

//----删除内核互斥量-----------------------------------------------------------
//功能: 删除一个内核互斥量，与所有_knl为后缀的函数一样，cn_locks_limit是用户定义
//      的，为保持模块的独立性，内核不与用户争用cn_locks_limit指标，故内核使用的
//      锁(信号量和互斥量)用单独的函数创建和删除。争用的话，将导致内核和应用程序
//      互相耦合
//      本函数和semp_delete的差别在于本函数不释放互斥量控制块。
//参数：mutex，被删除的互斥量
//返回：true=成功删除，false=失败
//-----------------------------------------------------------------------------
bool_t mutex_delete_knl(struct mutex_LCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //参数错误
        return false;
    int_save_asyn_signal();
    if(mutex->mutex_sync == NULL)     //没有事件在等待互斥量
    {
        rsc_del_node(&mutex->node);  //删除互斥量结点
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}

//----删除一个互斥量-----------------------------------------------------------
//功能：删除一个互斥量
//参数：mutex，被删除的互斥量
//返回：无
//-----------------------------------------------------------------------------
bool_t mutex_delete(struct mutex_LCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //参数错误
        return false;
    int_save_asyn_signal();
    if(mutex->mutex_sync != NULL)     //没有事件在等待互斥量
    {
        rsc_del_node(&mutex->node);  //删除互斥量结点
        mb_free(pg_lock_pool,mutex); //释放内存
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}
//----查询一个互斥量-----------------------------------------------------------
//功能：查询该互斥量是否可用
//参数：mutex，被查询的互斥量
//返回：true = 可用，false = 不可用
//-----------------------------------------------------------------------------
bool_t mutex_query(struct mutex_LCB *mutex)
{
    return mutex->enable;
}

