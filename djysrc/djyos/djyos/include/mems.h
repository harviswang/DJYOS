//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:堆管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供块相联内存分配策略
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __mems_H__
#define __mems_H__

#ifdef __cplusplus
extern "C" {
#endif

struct  event_ECB;

struct mem_global
{
    u8     *static_bottom; //堆底指针，从准静态分配算起
    u8     *heap_bottom;   //堆底指针，从块相联动态分配算起
    u8     *heap_top;      //堆顶指针,
    bool_t      dynamic;        //false =执行准静态分配
#if (cfg_dynamic_mem == 1)
    struct  event_ECB *mem_sync; //等待分配内存的事件链表,双向循环链表
    ptu32_t     ua_pages_num;   //总页数
    ptu32_t     free_pages_num;  //空闲内存页数
    ptu32_t     ua_block_max;    //最大内存块尺寸(字节数)
    ptu32_t     ua_free_block_max; //空闲的最大内存块大小(字节数);
    ufast_t     uf_grades;      //总阶数
                                //true = 执行动态内存分配

    //以下3部分在初始化时从堆中划出。
    //第一部分，每页分配一个16位字记录拥有本页内存的事件的ID。不允许跨事件分配和
    //释放局部内存，这是一个容易造成混乱的操作。
    //本表还用于标示该次分配的页数,使free函数可以不指定释放的字节数.
    //如果y_event_done函数发现事件仍有未释放的局部内存块，依据此表强制释放之
    //格式如下:
    //1.单页局部内存:id,
    //2.双页局部内存:0xffff+id
    //3.多页局部内存:0xfffe+id+阶号
    //4.单页全局内存:0xfffd,全局内存没有所属事件id,只需要知道内存大小
    //5.双(多)页全局内存:0xfffc+阶号.
    uint16_t    *index_event_id;
    ufast_t     *p_classes;     //第二部分,各阶块空闲金字塔级数表
    ucpu_t      ***ppp_bitmap;  //第三部分,指向"空闲金字塔位图指针的指针",
                                //每级一个,指向该阶空闲金字塔的第一个指针
#endif
} ;

//用于把事件申请的局部内存块串起来，调用y_event_done函数时，据此回收内存。
struct mem_record
{
    struct mem_record   *next;
    struct event_ECB *owner;
    u8 *address;
};

#if (cfg_dynamic_mem == 0)
#define m_malloc(size,timeout)          __m_static_malloc(size)
#define m_malloc_gbl(size,timeout)      __m_static_malloc(size)
#define m_free(p)                       __m_static_free(p)
#else
void *m_malloc(ptu32_t size,u32 timeout);
void *m_malloc_gbl(ptu32_t size,u32 timeout);
bool_t m_free(void * pl_mem);
#endif

ucpu_t __m_leading_zero(u8 *data,u8 len);
u32 __m_calculate_expense(ptu32_t ua_pages);
struct mem_record *__inc_record_link(void);
ucpu_t __m_leading_ucpu_zero(ucpu_t data);
ufast_t __m_get_grade(ptu32_t size);
void __record_mem(u8 *address,ufast_t uf_grade_th);
bool_t __m_check_memory(ptu32_t size,u32 timeout);
void __m_wait_memory(struct  event_ECB *event);
void __m_cleanup(uint16_t event_id);
ptu32_t module_init_heap_static(ptu32_t para);
ptu32_t module_init_heap_dynamic(ptu32_t para);
void *__m_static_malloc(ptu32_t size);
bool_t __m_static_free(void * pl_mem);
void *__malloc_block(ufast_t grade);
ptu32_t m_check_size(void * mp);
ptu32_t  m_get_max_free_mem(void);
ptu32_t  m_get_heap_size(void);
ptu32_t  m_get_free_mem(void);
ptu32_t  m_get_page_size(void);

#ifdef __cplusplus
}
#endif

#endif //__mems_H__

