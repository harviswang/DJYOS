//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:内存池管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供固定块分配策略，内存块至少2个指针长度(通常为8字节)。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

//每个内存池都有一个mem_cell_pool类型的结构,这也是用固定块分配法进行分配的.
static struct mem_cell_pool tg_pool_of_cell_pool[cn_mem_pools];
static struct mem_cell_pool *pg_pool_of_cell_pool;

//----初始化固定块分配模块------------------------------------------------------
//功能: 初始化操作系统的固定块内存分配模块.
//参数: 无.
//返回: 1.
//----------------------------------------------------------------------------
ptu32_t module_init_memb(ptu32_t para)
{
    static struct mem_cell_pool cell_pool;
    cell_pool.cell_size = sizeof(struct mem_cell_pool);
    cell_pool.continue_pool = tg_pool_of_cell_pool;
    cell_pool.free_list = NULL;
    cell_pool.pool_offset = (ptu32_t)&tg_pool_of_cell_pool[cn_mem_pools];
    pg_pool_of_cell_pool = &cell_pool;
    rsc_add_tree_root(&cell_pool.memb_node,sizeof(struct mem_cell_pool),
                    "固定块分配池");
    semp_create_knl(&cell_pool.memb_semp,
                      cn_mem_pools,cn_mem_pools,"固定块分配池");
    return 1;
}

//----创建一个内存池-------------------------------------------------------
//功能: 初始化一个内存池，原始内存池的内存由用户提供。分配规则：
//      1.开始时,free_list指针空,continue_pool指向一个连续内存池。
//      2.如果free_list非空，新内存从free_list队列头部取。
//      3.如果free_list空，新内存从continue_pool中取。
//      4.释放内存时，把内存块链接到free_list队列,而不是返回continue_pool数组.
//      内存池是一个临界资源,操作时需要关闭中断,如果新申请的内存块全部链接到
//      free_list,将会连续很长时间关闭中断,而现在采用的方法每次只操作一个内存块,
//      连续关中断的时间被降低到最小.
//参数: pool_original,最初由用户提供的内存池.
//      init_capacital,原始内存池的尺寸,以块为单位。
//      cell_size,内存块尺寸，若系统有对齐要求，必须为指针长度的整数倍，最小为两倍指针
//			长度.
//      increment,增量，本内存池中的内存单元耗尽时，将从堆中增加的内存块数，
//          设为0则不增加。注意，内存池扩大后，即使用户调用mb_free释放了内存，但
//          除非释放了内存池中的全部内存，否则新增的内存不会被收回。
//      limit,如果increment !=0，limit限制内存池的最大块数，以防无限制地增加，导
//          致内存耗尽。
//      name,给内存池起个名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 内存池指针.
//----------------------------------------------------------------------------
struct mem_cell_pool *mb_create_pool(void *pool_original,u32 init_capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name)
{
    struct mem_cell_pool *pool;

    //创建内存池时，可以不提供内存块，此时init_capacital也必须是0.
    //此功能结合内存池动态增量功能，可帮助应用程序优化内存配置，例如:
    //某串口，可能有若干种通信协议，取决于现场应用。我们可以同时初始化好多种通信
    //协议的驱动程序，为每种通信协议初始化一个空的内存池，就可以实现只为实际发生
    //的通信协议分配内存。
    if(( pool_original == NULL) && (init_capacital != 0))
        return NULL;
//对齐参数检查，1、如果系统要求对齐访问，则块尺寸必须是指针尺寸的整数倍，内存池
//起始地址也是指针尺寸的整数倍。2、如果系统不要求对齐访问，则块尺寸大于指针尺寸
//就可以了。
    if(cell_size < 2*sizeof(void*))
        return NULL;
#if(mem_align == 1)
    if((cell_size % sizeof(void*) != 0)
            || (((ptu32_t)pool_original) % sizeof(void*) != 0))
        return NULL;
#endif

    //分配一个内存池控制头
    pool = (struct mem_cell_pool *)mb_malloc(pg_pool_of_cell_pool,0);
    if(pool == NULL)
        return NULL;    //内存池控制头分配不成功

    pool->continue_pool = (void*)pool_original;     //连续池首地址
    pool->free_list = NULL;                 //开始时空闲链表是空的
    //设置连续池偏移地址
    pool->pool_offset = (ptu32_t)pool_original + init_capacital*cell_size;
    pool->cell_size = cell_size;
    pool->cell_increment = increment;
    pool->cell_limit = limit;
    rsc_add_son(&pg_pool_of_cell_pool->memb_node,
                &pool->memb_node,
                sizeof(struct mem_cell_pool),
                name);
    //init_capacital有可能是0
    semp_create_knl(&pool->memb_semp,init_capacital,init_capacital,name);
    return pool;
}

//----创建一个内核内存池-------------------------------------------------------
//功能: 创建一个内核内存池，与所有_knl为后缀的函数一样，cn_mem_pools是用户定义
//      的，为保持模块的独立性，内核不与用户争用cn_mem_pools指标，故内核使用的
//      内存池控制块用单独的函数创建和删除。争用的话，将导致内核和应用程序
//      互相耦合
//      与mb_create_pool的区别是，本函数需要调用者提供内存池控制块实体。
//参数: pool,用户提供的内存池控制块
//      pool_original,最初由用户提供的内存池.
//      init_capacital,原始内存池的尺寸,以字节为单位,不能以块为单位,因为块尺寸将按
//          系统对齐要求调整,0表示直接从系统内存中申请.
//      increment,增量，本内存池中的内存单元耗尽时，将从堆中增加的内存单元数量，
//          设为0则不增加。注意，内存池扩大后，即使用户调用mb_free释放了内存，但
//          除非释放了内存池中的全部内存，否则新增的内存不会被收回。
//      limit,如果increment !=0，limit限制内存池的最大尺寸，以防无限制地增加，导
//          致内存耗尽。
//      cell_size,块尺寸,将按对齐要求调整,ARM中调整为8的整数倍.
//      name,给内存池起个名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 内存池指针.
//注: 本函数是专门为内核准备的，故不作参数检查。
//----------------------------------------------------------------------------
struct mem_cell_pool *mb_create_pool_knl(struct mem_cell_pool *pool,
                                void *pool_original,u32 init_capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name)
{
    pool->continue_pool = (void*)pool_original;     //连续池首地址
    pool->free_list = NULL;                 //开始时空闲链表是空的
    //设置连续池偏移地址
    pool->pool_offset = (ptu32_t)pool_original + init_capacital*cell_size;
    pool->cell_size = cell_size;
    pool->cell_increment = increment;
    pool->cell_limit = limit;
    rsc_add_son(&pg_pool_of_cell_pool->memb_node,
                &pool->memb_node,
                sizeof(struct mem_cell_pool),
                name);
    semp_create_knl(&pool->memb_semp,init_capacital,init_capacital,name);
    return pool;
}

//----删除一个内存池-----------------------------------------------------------
//功能: 删除一个内存池，当某内存池不再需要时，可调用本函数.本函数只清理了内存池
//      的信号量和资源结点，内存池缓冲区是调用者提供的，应由调用者清理。
//参数: pool，内存池指针
//返回: true= 成功删除，false=失败
//-----------------------------------------------------------------------------
bool_t mb_delete_pool(struct mem_cell_pool *pool)
{
    if(!semp_delete_knl(&pool->memb_semp))
        return false;
    if(!rsc_del_node(&pool->memb_node))
        return false;
    mb_free(pg_pool_of_cell_pool,pool);
    return true;
}

//----删除一个内核内存池-------------------------------------------------------
//功能: 删除一个内存池，当某内存池不再需要时，可调用本函数.本函数只清理了内存池
//      的信号量和资源结点，内存池缓冲区是调用者提供的，应由调用者清理。
//      与所有_knl为后缀的函数一样，cn_mem_pools是用户定义
//      的，为保持模块的独立性，内核不与用户争用cn_mem_pools指标，故内核使用的
//      内存池控制块用单独的函数创建和删除。争用的话，将导致内核和应用程序
//      互相耦合
//      本函数与mb_delete_pool的区别是，本函数不释放内存池控制块
//参数: pool，内存池指针
//返回: 无
//-----------------------------------------------------------------------------
bool_t mb_delete_pool_knl(struct mem_cell_pool *pool)
{
    if(!semp_delete_knl(&pool->memb_semp))
        return false;
    if(!rsc_del_node(&pool->memb_node))
        return false;
    return true;
}

//----分配一块内存-------------------------------------------------------------
//功能: 从制定内存池中分配一块内存,只能分配一块,而且不能保证连续两次分配的内存
//      地址是连续的.
//参数: pool，内存池指针
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 申请成功返回内存地址,否则返回NULL.
//-----------------------------------------------------------------------------
void *mb_malloc(struct mem_cell_pool *pool,u32 timeout)
{
    void *result,*inc;
    u32 limit,inc_size,inc_cell,capacity,frees;
    char *name;
    atom_low_t atom_low;
    if(pool == NULL)
        return NULL;
    //没有取得信号量，表明内存池空,这个信号量是保护内存池的，确保被分配的内存块
    //不超过内存池的容量
    if(semp_pend(&pool->memb_semp,0) == false)  //无信号量，表明内存块已经用完
    {
        //注意:上一行和下一行之间可能发生线程切换
        int_save_asyn_signal();
        //内存池容量未达到上限，可以继续增加
        if(semp_query_capacital(&pool->memb_semp) < pool->cell_limit)
        {
            //此时timeout只能为0，因为在禁止异步信号的情况下，不为0也没用
            inc = m_malloc_gbl(pool->cell_increment*pool->cell_size,0);
            if(inc != NULL)
            {
                inc_size = m_check_size(inc);   //检查实际分配到的内存量
                inc_cell = inc_size/pool->cell_size;
                capacity = semp_query_capacital(&pool->memb_semp);
                frees = semp_query_free(&pool->memb_semp);
                limit = capacity + inc_cell;
                name = ((struct rsc_node*)&pool->memb_semp)->name;
                semp_delete_knl(&pool->memb_semp);      //删除原信号量
                //按扩容后的容量重新申请信号量。
                semp_create_knl(&pool->memb_semp,limit,frees+inc_cell,name);
                pool->continue_pool = inc;
                pool->pool_offset = (ptu32_t)inc + inc_cell*pool->cell_size;
            }
        }
        int_restore_asyn_signal();
        if(semp_pend(&pool->memb_semp,timeout) == false)
            return NULL;
    }

    //注:从semp_pend到int_low_atom_start之间发生抢占是允许的，因为信号量已经
    //取得，其他事件不可能把内存块分配光。
    atom_low = int_low_atom_start( );
    if(pool->free_list != NULL)     //空闲队列中有内存块
    {
        result = pool->free_list;   //取空队列表头部的内存块
        if(((struct mem_cell_free*)result)->next==(struct mem_cell_free*)result)
            pool->free_list = NULL; //这是空闲队列中的最后一块了，但可能不是内存
                                    //池中的最后一块，因为连续池中可能还有
        else
        {
            pool->free_list = pool->free_list->next;  //空闲队列下移一格.
            pool->free_list->previous=((struct mem_cell_free*)result)->previous;
            ((struct mem_cell_free*)result)->previous->next = pool->free_list;
        }
    }else                   //空闲队列中无内存块，从连续池中取
    {
        pool->pool_offset -= pool->cell_size;  //偏移地址调整
        //分配偏移地址处的内存块
        result = (void*)pool->pool_offset;
    }
    int_low_atom_end(atom_low);
    return result;
}
//----释放内存--------------------------------------------------------------
//功能: 释放内存,把使用完毕的内存块放回指定的内存池,内存池和内存块必须匹配,否则
//      会发生灾难性的错误.新释放的块链接到 free_list 队列中,而不是放回连续池
//      中,也不重新返回系统堆.内存池从系统堆分配内存是单向的,一经分配永不释放.
//参数: block,待释放的内存块指针
//      pool,目标内存池.
//返回: 无
//-----------------------------------------------------------------------------
void mb_free(struct mem_cell_pool *pool,void *block)
{
    atom_low_t atom_low;
    if(pool == NULL)
        return;
//    pl_continue = (void*)pool->pool_offset;
//    block_sum = semp_query_capacital(&pool->memb_semp);
    //20110613日注:增加扩容功能后，下列检查不再有效，注释掉。
    //被释放块合法性检查:1、地址不能低于内存池首地址
    //                   2、地址不能高于内存池末地址
    //                   3、地址必须是块首地址，即偏移必须是块尺寸的整数倍
//    if((block < (void*)pool->pool_offset)
//            ||((ptu32_t)block>=((ptu32_t)pl_continue+pool->cell_size*block_sum))
//            ||(((ptu32_t)block-(ptu32_t)pl_continue) % pool->cell_size != 0))
//        return ;
    atom_low = int_low_atom_start( );
    //查看待释放的内存块是否已经在free_list队列中。
    if(pool->free_list == NULL)
    {
        pool->free_list = (struct mem_cell_free*)block;
        pool->free_list->next = (struct mem_cell_free*)block;
        pool->free_list->previous = (struct mem_cell_free*)block;
        
     }
    else
    {
        //if(((struct mem_cell_free*)block)->previous->next 
         //                                   != (struct mem_cell_free*)block)
       // {   //以下4行把被释放的内存块插入到队列尾部
            ((struct mem_cell_free*)block)->next = pool->free_list;
            ((struct mem_cell_free*)block)->previous = pool->free_list->previous;
            pool->free_list->previous->next = (struct mem_cell_free*)block;
            pool->free_list->previous = (struct mem_cell_free*)block;
       // }
    }
    int_low_atom_end(atom_low);
    semp_post(&pool->memb_semp);
}


//----查询剩余容量-------------------------------------------------------------
//功能: 查询内存池还有多少空闲内存块
//参数: pool,目标内存池.
//返回: 内存块数
//-----------------------------------------------------------------------------
u32 mb_query_free(struct mem_cell_pool *pool)
{
    if(pool == NULL)
        return 0;
    return semp_query_free(&pool->memb_semp);
}

//----查询内存池容量-----------------------------------------------------------
//功能: 查询内存池总共有多少内存块
//参数: pool,目标内存池.
//返回: 内存块数
//-----------------------------------------------------------------------------
u32 mb_query_capacital(struct mem_cell_pool *pool)
{
    if(pool == NULL)
        return 0;
    return semp_query_capacital(&pool->memb_semp);
}

