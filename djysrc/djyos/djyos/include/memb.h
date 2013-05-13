//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:内存池管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供固定块分配策略
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __memb_H__
#define __memb_H__

#ifdef __cplusplus
extern "C" {
#endif

struct mem_cell_pool
{
    struct rsc_node memb_node;    //资源结点,整个内存池将作为一个资源结点
    void  *continue_pool;    //连续内存池首地址，使用它可以增加实时性。
    struct mem_cell_free  *free_list;        //未分配块链表,单向,NULL结尾
    struct semaphore_LCB memb_semp;
    ptu32_t pool_offset;    //连续池中的偏移量(当前地址)
    u32 cell_size;          //块大小,初始化时将按系统要求调整尺寸。
    u32 cell_increment;     //当内存池耗尽时，将自动从堆中增加内存池尺寸，单位为
                            //内存块数量，设为0表示不增量，注意，内存池尺寸是只
                            //增不减的，即一旦从堆中申请了内存，是不释放的。
    u32 cell_limit;         //如果increment !=0，本成员限定该内存池的最大尺寸，
                            //单位为内存块数。
    struct  event_ECB *cell_sync;   //同步队列。
};

//空闲内存块构成双向循环链表。但初始化内存池时并不把内存加入链表，而是在释放时
//才加入。
//空闲链表节点，把free_list做成双向链表，可以增加程序的健壮性，如果是单向链表，
//虽然可以实现功能，但是，如果重复释放一个已经释放的内存块，则可能导致破坏链表。
//用双向链表，则很容易判断该结点是否已经在free_list中，如是，则不重复执行释放操
//作，单向链表虽然可以用遍历的方法判断是否在free_list，但执行时间不确定，不符合
//实时系统的要求。
//用双向链表的后果是，单块内存至少两个指针长度，通常为8字节。
struct mem_cell_free
{
    struct mem_cell_free *previous;
    struct mem_cell_free *next;
};
ptu32_t module_init_memb(ptu32_t para);
struct mem_cell_pool *mb_create_pool(void *pool_original,u32 capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name);
struct mem_cell_pool *mb_create_pool_knl(struct mem_cell_pool *pool,
                                void *pool_original,u32 capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name);
bool_t mb_delete_pool(struct mem_cell_pool *pool);
bool_t mb_delete_pool_knl(struct mem_cell_pool *pool);
void *mb_malloc(struct mem_cell_pool *pool,u32 timeout);
void mb_free(struct mem_cell_pool *pool,void *block);
u32 mb_query_free(struct mem_cell_pool *pool);
u32 mb_query_capacital(struct mem_cell_pool *pool);

#ifdef __cplusplus
}
#endif

#endif //__memb_H__

