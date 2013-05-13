//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:堆管理模块
//作者:  罗侍田.
//版本：V1.0.1
//文件描述:提供块相联内存分配策略
//其他说明:
//修订历史:
//3. 日期:2009-10-31
//   作者:  罗侍田.
//   新版本号：1.0.2
//   修改说明: 修正了__m_check_memory和__m_wait_memory函数的一处错误，该bug由
//   网友"再回首"提交
//2. 日期:2009-03-03
//   作者:  罗侍田.
//   新版本号：1.0.1
//   修改说明: 修正了__m_check_memory函数的一处错误，该bug由网友sniper提交
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

#if (cfg_dynamic_mem == 1)
//字节数据前导0个数表,256字节,用于快速查寻首个非0位的位置
u8 const cn_leading_zero[]=
{
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//字节数据后缀0个数表,256字节,用于快速查寻首个非0位的位置
//u8 const cn_suffix_zero[]=
//{
//    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//};
const ucpu_t cn_low_xbit_msk_exp2 []=
{
    cn_low_1bit_1,
    cn_low_2bit_1,
    cn_low_4bit_1,
    cn_low_8bit_1,
    cn_low_16bit_1,
    cn_allbit_1,
};

const ucpu_t cn_low_xbit_msk []=
{
    0,
    cn_low_1bit_1,
    cn_low_2bit_1,
    cn_low_3bit_1,
    cn_low_4bit_1,
    cn_low_5bit_1,
    cn_low_6bit_1,
    cn_low_7bit_1,
    cn_low_8bit_1,
    cn_low_9bit_1,
    cn_low_10bit_1,
    cn_low_11bit_1,
    cn_low_12bit_1,
    cn_low_13bit_1,
    cn_low_14bit_1,
    cn_low_15bit_1,
    cn_low_16bit_1,
    cn_low_17bit_1,
    cn_low_18bit_1,
    cn_low_19bit_1,
    cn_low_20bit_1,
    cn_low_21bit_1,
    cn_low_22bit_1,
    cn_low_23bit_1,
    cn_low_24bit_1,
    cn_low_25bit_1,
    cn_low_26bit_1,
    cn_low_27bit_1,
    cn_low_28bit_1,
    cn_low_29bit_1,
    cn_low_30bit_1,
    cn_low_31bit_1,
    cn_allbit_1,
};
#else
#define m_malloc(size,timeout)          __m_static_malloc(size)
#define m_malloc_gbl(size,timeout)      __m_static_malloc(size)
#define m_free(p)                       __m_static_free(p)
#endif

static struct mem_global tg_MCB;        //全局内存控制块
static struct mutex_LCB tg_mem_mutex;   //保护tg_mem_global不被并发访问

struct mem_global *djy_get_MCB(void)
{
    return &tg_MCB;
}
//----内存结构初始化-----------------------------------------------------------
//功能: 初始化内存数据结构,但不初始化堆
//参数: 无
//返回: 无
//备注: 1.本函数在初始化完成之前调用,中断尚未开启,无需考虑关闭中断的问题.
//      2.软件启动后首先调用本函数,启动静态分配功能,但还不具备动态分配功能.
//-----------------------------------------------------------------------------
ptu32_t module_init_heap_static(ptu32_t para)
{
#if(align_up_sys(cn_page_size) != cn_page_size) //本行起编译期assert作用
cn_page_size 不符合系统对齐要求。
#endif
    tg_MCB.dynamic = false;
    tg_MCB.static_bottom = (u8*)align_up_sys((ptu32_t)sys_heap_bottom);
    tg_MCB.heap_bottom = tg_MCB.static_bottom;
    tg_MCB.heap_top=(u8*)align_down_sys((ptu32_t)sys_heap_top);
    return 1;
}

//----准静态内存分配-----------------------------------------------------------
//功能：分配内存,调整heap_bottom指针
//参数：size,欲分配的内存块尺寸
//返回：分配的内存指针，NULL表示没有内存可以分配
//备注: 1.准静态分配与静态内存分配类似,没有保护措施,正确性要程序员自己保证.这种
//      分配方法也不会引起阻塞,在执行module_init_heap_dynamic之前,所有的内存分配
//      均采用准静态分配
//      2.本函数在初始化完成之前调用,中断尚未开启,无需考虑关闭中断的问题.
//-----------------------------------------------------------------------------
void *__m_static_malloc(ptu32_t size)
{
    u8 *temp;

    temp = tg_MCB.heap_bottom;   //保存当前堆底
    //留下保存块尺寸的空间后对齐
    tg_MCB.heap_bottom = (u8 *)align_up_sys
            ((ptu32_t)tg_MCB.heap_bottom + sizeof(ptu32_t));
    if((tg_MCB.heap_top-tg_MCB.heap_bottom) < size)
    {
        tg_MCB.heap_bottom = temp;   //恢复当前堆底
        return NULL;        //内存不足，返回
    }
    //新分配的内存尺寸保存在当前堆底前面
    temp = tg_MCB.heap_bottom - sizeof(ptu32_t);
    *(ptu32_t *)temp = size;    //保存申请的内存块尺寸
    temp = tg_MCB.heap_bottom;   //保存分配给用户的内存块地址
    tg_MCB.heap_bottom += size;   //移动堆底部指针
    tg_MCB.heap_bottom = (u8 *)align_up_sys
                ((ptu32_t)tg_MCB.heap_bottom);//对齐
    return(temp);
}

//----释放准静态分配的内存块---------------------------------------------------
//功能：释放准静态分配的一个内存块
//参数：pl_mem，待释放的内存块指针
//返回：true = 成功释放，false =  释放失败
//备注: 1.准静态分配与静态内存分配类似,没有保护措施,正确性要程序员自己保证.这种
//      分配方法也不会引起阻塞,在执行module_init_heap_dynamic之前,所有的内存分配
//      均采用准静态分配
//      2.本函数在初始化完成之前调用,中断尚未开启,无需考虑关闭中断的问题.
//-----------------------------------------------------------------------------
bool_t __m_static_free(void * pl_mem)
{
    return true;
}

#if (cfg_dynamic_mem == 0)

//----查询内存尺寸-------------------------------------------------------------
//功能: 根据给定的指针,查询该指针所在的内存块的尺寸.
//参数: mp,动态分配的内存指针.
//返回: 内存块尺寸,返回0有几种含义:1.非法指针,2.mp是由准静态分配的指针.
//-----------------------------------------------------------------------------
ptu32_t m_check_size(void * mp)
{
    return *(ptu32_t*)(((ptu32_t*)mp) -1);
}

#else

//----查询内存尺寸-------------------------------------------------------------
//功能: 根据给定的指针,查询该指针所在的内存块的尺寸.
//参数: mp,动态分配的内存指针.
//返回: 内存块尺寸,返回0有几种含义:1.非法指针,2.mp是由准静态分配的指针.
//-----------------------------------------------------------------------------
ptu32_t m_check_size(void * mp)
{
    ptu32_t ua_pages_number;
    u8 *temp;
    uint16_t *pl_id;
    ufast_t uf_free_grade_th;
    if(((u8*)mp<tg_MCB.static_bottom)
                || ((u8*)mp>=tg_MCB.heap_top))
    {
        return 0;   //错误,指针不在堆范围内
    }

    if((u8*)mp < tg_MCB.heap_bottom)    //该指针在静态分配区
    {
        temp = (u8*)((u8*)mp - sizeof(ptu32_t));
        return *(ptu32_t*)temp;
    }else                                           //该指针在块相联动态分配区
    {
        //计算释放的内存的页号
        ua_pages_number=(ptu32_t)((ptu32_t)mp-(ptu32_t)tg_MCB.heap_bottom)
                            >>cn_page_size_suffix_zero;

        //查找释放的内存块的阶号,从0起计.通过阶号也可以确定内存块的大小.
        //确定内存分配类型,局部分配需要知道拥有该内存的事件id，
        //全局分配无需清除内存分配跟踪表,无需知道拥有该内存的事件id
        pl_id = &tg_MCB.index_event_id[ua_pages_number];
        switch(pl_id[0])
        {
            case 0xffff :
            {//双页局部分配,-1+id
                uf_free_grade_th = 1;
            }break;
            case 0xfffe :
            {//多页局部分配:-2+id+阶号
                uf_free_grade_th = pl_id[2];
            }break;
            case 0xfffd :
            {//单页全局内存:-3
                uf_free_grade_th = 0;
            }break;
            case 0xfffc :
            {//双(多)页全局内存:-4+阶号.
                uf_free_grade_th = pl_id[1];
            }break;
            default :
            {
                if(pl_id[0] <= cn_event_id_limit)
                    uf_free_grade_th = 0;       //单页局部分配:id,
                else
                {
                    djy_error_login(enum_mem_error,NULL);   //指针有错,直接退出
                    return false;

                }
            }break;
        }
        return cn_page_size << uf_free_grade_th;
    }
}

//----返回数组中首位1位置（前导0的个数）----------------------------------------
//功能：从数组的最高字节的最高位开始计算前导0的数量。
//参数：data,输入数组指针
//      len,数组的大小
//返回：前导0的个数
//------------------------------------------------------------------------------
ucpu_t __m_leading_zero(u8 *data,u8 len)
{
    u8 u8_i;
    ucpu_t  uc_j=0;
    u8_i=len-1;
    do
    {
        if(cn_leading_zero[data[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[data[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i-- !=0);
    return uc_j;
}

//----计算控制表所需的字节数---------------------------------------------------
//功能：计算控制表所需的字节数
//参数：u32_pages,内存页数
//返回：字节数
//注意: 保持函数功能的单一性,不要同时计算控制结构各部分的字节数
//      移植敏感:本函数与你的机器的对齐方式有关,移植时你必须了解机器的对齐方式.
//      本函数能满足绝大部分cpu的对齐要求,但不排除有例外.所以在移植时你最好读懂
//      本函数，然后考虑是否需要修改
//-----------------------------------------------------------------------------
u32 __m_calculate_expense(ptu32_t ua_pages)
{
    ptu32_t ua_bits,u32_return;
    ufast_t uf_grades,uf_grade_th;

    //计算阶数，即一共有多少种页尺寸,方法:地址长度减去ua_pages中前导0的数量就是
    //阶数,例如,ua_pages=5,最高位位置是bit2,有29个前导0，页尺寸有1,2,4页3种,阶数=3
    uf_grades=cn_ptr_bits-__m_leading_zero((u8 *)&ua_pages,sizeof(ptu32_t));

    //每页分配一个16位字记录拥有本页内存的事件的ID。
    //当分配高阶内存块时，只记录首页，free时并不清除。设置本字是为了防止在
    //事件间交叉分配和释放内存时不至于出错，但并不建议用户这样使用。
    u32_return = ua_pages<<1;
    //下一个数据域是ufast_t类型的,需要按ufast_t类型对齐.
    u32_return = (u32_return+sizeof(ufast_t)-1)&(~(sizeof(ufast_t)-1));
    //每阶还要分配一个ufast_t类型的数据,纪录本阶的级数
    u32_return += uf_grades*(sizeof(ufast_t));
    //下一个数据域是指针,需要按指针对齐
    u32_return = (u32_return +sizeof(void*) -1)&(~(sizeof(void*)-1));
    //每阶一个指针,指向位图所引表
    u32_return += uf_grades*sizeof(void*);

    for(uf_grade_th = 0; uf_grade_th < uf_grades; uf_grade_th++)
    {
        ua_bits = ua_pages>>uf_grade_th;   //计算本阶总位数
        do
        {
            ua_bits=(ua_bits+cn_cpu_bits-1)>>cn_cpu_bits_suffix_zero;
            u32_return +=ua_bits*sizeof(ucpu_t);    //该级位图字数
            u32_return+=sizeof(void*);                //需要一个指针指向该位图首字
        }while(ua_bits>1); //直到所有级都分配完.
    }
    return u32_return;
}

//----延长内存记录表------------------------------------------------------------
//功能: 当内存记录表不够时(即pg_mem_record_free==NULL),调用本函数从内存中取1个
//      结构.如果预先分配的内存已经用完,则调用__malloc_block分配一页新的内存.
//      如果没有足够的内存可以分配,返回NULL.本函数内部调用,不开放给用户
//参数: 无
//返回: 如果成功延长,返回表头指针,否则返回NULL
//备注：调用本函数前应该保证中断被关闭。每次只取一个结构,而不是把新分配的整页
//      内存连到pg_mem_record_free表中,虽然增加了总的执行时间,但是提高了实时性.
//-----------------------------------------------------------------------------
/*
struct mem_record *__inc_record_link(void)
{
    static struct mem_record *record_free=NULL;
    struct mem_record *result;

    if(record_free==NULL)
        record_free=(struct mem_record*)__malloc_block(0);//分配1页内存用于记录
    if(record_free==NULL)   //表明没有内存可以分配
        return NULL;
    result = record_free;   //取得一个结构体
    result->next = NULL;    //初始化新节点的next指针
    record_free = record_free+1;  //指向下一个结构体
    if((cn_page_size-sizeof(struct mem_record)*(result-record_free))
            < sizeof(struct mem_record))
    {//本次分配的页已经用完.
        record_free=NULL;
    }
    return result;
}
*/

//----返回ucpu_t类数据前导0的个数----------------------------------------------
//功能：从ucpu_t类数据的最高位开始计算前导0的数量。
//参数：data,输入数
//返回：前导0的个数
//-----------------------------------------------------------------------------
ucpu_t __m_leading_ucpu_zero(ucpu_t data)
{
    u8 u8_i;
    ucpu_t  uc_j=0;
    u8_i=sizeof(ucpu_t)-1;
    do
    {
        if(cn_leading_zero[((u8*)&data)[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[((u8*)&data)[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i-- !=0);
    return uc_j;
}

//----返回规格化阶-------------------------------------------------------------
//功能：把任意数据规格化为合法的内存尺寸。
//参数：size,欲分配的内存块尺寸
//返回：大于或等于size的最小的允许块尺寸的阶。
//-----------------------------------------------------------------------------
ufast_t __m_get_grade(ptu32_t size)
{
    ucpu_t  uc_temp;
    if(size<=cn_page_size)
        return 0;
    uc_temp=__m_leading_zero((u8 *)&size,sizeof(ptu32_t));
    if((cn_high_1bit_1>>uc_temp) != size)
    //如果size本身不是规格化的数,调整到比它大的最小允许块大小
    //否则维持size的大小,不做调整
        uc_temp--;

    //返回阶数,从0起计,即最小页尺寸前导0减块尺寸前导0.
    return (ufast_t)(cn_ptr_bits-1-cn_page_size_suffix_zero-uc_temp);
}

//----记录分配的内存-----------------------------------------------------------
//功能：1.把内存分配信息保存到running事件的内存分配链表里面,以备事件完成的时候
//      强制收回内存.如果分配的是全局内存,则不记录,事件完成时也不收回.
//      2.把内存分配信息保存到index_event_id表里面.index_event_id表的格式见结构
//      mem_global_t的定义.
//参数: address,分配的内存地址.
//      uf_grade_th,被分配的内存的阶号
//备注: 本函数由操作系统内部调用,内存分配链表是一个单向不循环链表,每事件一个.
//-----------------------------------------------------------------------------
/*
void __record_mem(u8 *address,ufast_t uf_grade_th)
{
    uint16_t *pl_id,id;
    ptu32_t  page;

    pg_mem_record_free->address=address;
    if(pg_event_running->held_memory==NULL)
    {   //如果running事件尚无申请内存记录,则把新内存连接到记录的头部.
        pg_event_running->held_memory=pg_mem_record_free;
        pg_mem_record_free = pg_mem_record_free->next;
        pg_event_running->held_memory->next = NULL;
    }else
    {   //如果running事件已经有申请内存记录,则把所有本事件申请的内存连接到
        //内存记录链表中相邻结点上.可由pg_event_running->held_memory访问
        struct mem_record *p;
        p=pg_mem_record_free;
        pg_mem_record_free = pg_mem_record_free->next;
        p->next=pg_event_running->held_memory;
        pg_event_running->held_memory=p;
    }

    //阅读以下条件句请结合mem_global_t中index_event_id成员定义的注释.
    pl_id = tg_MCB.index_event_id;
    id = pg_event_running->event_id;
    page = (ptu32_t)(address-tg_MCB.heap_bottom)
            >>cn_page_size_suffix_zero;
    if(uf_grade_th==0)
    {//分配1页
        pl_id[page] = id;
    }else if(uf_grade_th==1)
    {//分配2页
        pl_id[page] = -1;
        pl_id[page+1] = id;
    }else
    {   //分配多页
        pl_id[page] = -2;
        pl_id[page+1] = id;
        pl_id[page+2] = uf_grade_th;
    }

    if(pg_mem_record_free==NULL)
        pg_mem_record_free=__inc_record_link();
}
*/

//----查看并等待空闲内存----------------------------------------------------
//功能：等待队列是一个经过排序的双向循环链表,按照申请的内存量排序,小的在前,
//      当内存可用时,首先使申请量小的事件得到满足,这样可以使内存满足尽量多事件,
//      不使用优先级排序,因为实时事件是不允许使用动态分配的,所以不会影响实时性.
//参数：size,需要申请的内存尺寸
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，
//      非0值将被向上调整为cn_tick_us的整数倍
//返回: true = 有空闲内存供分配，false = 无内存可分配
//备注: 本函数由操作系统调用,用户不能调用.
//------------------------------------------------------------------------------
bool_t __m_check_memory(ptu32_t size,u32 timeout)
{
    struct  event_ECB *event;
    u32 u32l_start_time,u32l_rest_time = 0;
    bool_t   wait;
    u32l_start_time = djy_get_time();
    do
    {
        wait=false;
        if(tg_MCB.ua_free_block_max < size)
        {//最大的可分配块尺寸小于请求的内存,没有内存可以分配
            int_save_asyn_signal();
            wait=true;  //如果是经过了阻塞后重新执行,while循环需要再次判断
                        //是否有足够的内存分配.
            __djy_cut_ready_event(pg_event_running);

            pg_event_running->next = NULL;
            pg_event_running->previous = NULL;

            pg_event_running->wait_mem_size = size; //填充等待的字节数
            event = tg_MCB.mem_sync;       //获取内存等待表指针

            //以下把事件置入内存等待队列中
            pg_event_running->sync_head = &tg_MCB.mem_sync;
            if(event == NULL)    //等待队列空
            {
                pg_event_running->multi_next = pg_event_running;
                pg_event_running->multi_previous = pg_event_running;
                tg_MCB.mem_sync = pg_event_running;
            }else
            {
                do
                {//本循环找到第一个请求内存大于新事件的事件.
                    if(event->wait_mem_size < size)
                        event = event->multi_next;
                    else
                        break;
                }while(event != tg_MCB.mem_sync);
                //如果没有找到申请内存比新事件长的事件,新事件插入队列尾,而
                //队列尾部就是event_wait的前面,此时event恰好等于event_wait
                //如果找到剩余延时时间长于新事件的事件,新事件插入该事件前面.
                //所以无论前面循环找到与否,均可使用下列代码
                pg_event_running->multi_next = event;
                pg_event_running->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = pg_event_running;
                event->multi_previous = pg_event_running;
            }

            pg_event_running->last_status.all
                            = pg_event_running->event_status.all;
            pg_event_running->event_status.bit.wait_memory = 1;
            if(timeout != cn_timeout_forever)
            {
                pg_event_running->event_status.bit.wait_overtime = 1;
                __djy_addto_delay(timeout-u32l_rest_time);
            }

            int_restore_asyn_signal();  //打开中断才能实现调度
            //检查从哪里返回，是超时还是有事件释放内存
            if(pg_event_running->event_status.bit.wait_memory)
            {//说明同步条件未到，从超时返回，应从内存同步队列中取出事件。
                pg_event_running->event_status.bit.wait_memory = 0;
                return false;   //超时返回
            }else   //同步返回条件达成，说明有线程释放内存，进入下一次while循环
            {       //检查是否有足够的内存,
                u32l_rest_time = djy_get_time() - u32l_start_time;
                //在这里做这个条件判断是必要的，其他线程释放内存会激活本事件，
                //但释放内存的时间可能与超时时间临界,或者由于优先级的关系本事件
                //不能立即得到执行，本事件真正开始执行时overtime条件也已经成立。
                //如果此时内存仍然不足，就应该按超时返回
                if((u32l_rest_time >= timeout)
                    &&(tg_MCB.ua_free_block_max < size))
                {
                    return false;
                }
            }
        }
    }while(wait==true);
    return true;    //return true前是不开中断的,以免在分配内存之前发生中断.
}

//----把事件放入等待队列-------------------------------------------------------
//功能: 把事件直接放进等待队列,不调度.等待队列是一个按请求的内存从小到大排列的
//      双向循环链表.用内存尺寸排序而不是用优先级排队，基于两点考量:1、可以最大
//      程度满足需求，2、在应用程序设计中，一般不会在非常紧急、优先级非常高的
//      事件处理中使用动态分配方法。
//参数: event,待进入等待队列的事件指针
//返回: 无
//-----------------------------------------------------------------------------
void __m_wait_memory(struct  event_ECB *event)
{
    struct  event_ECB *pl_event;
    ptu32_t size;

    size = event->wait_mem_size;
    pl_event = tg_MCB.mem_sync;     //获取内存等待表指针
    pl_event->sync_head = &tg_MCB.mem_sync;
    if(pl_event == NULL)            //等待队列空
    {
    	event->next = NULL;
    	event->previous = NULL;
        
    	event->multi_next = event;
    	event->multi_previous = event;    
        
    	tg_MCB.mem_sync = event;
    }else
    {
        do
        {//本循环找到第一个请求内存大于新事件的事件.
            if(pl_event->wait_mem_size < size)
                pl_event = pl_event->multi_next;
            else
                break;
        }while(pl_event != tg_MCB.mem_sync);
        //如果没有找到申请内存比新事件长的事件,新事件插入队列尾,而队列
        //尾部就是event_wait的前面,此时event恰好等于event_wait
        //如果找到剩余延时时间长于新事件的事件,新事件插入该事件前面.
        //所以无论前面循环找到与否,均可使用下列代码
        event->multi_next = pl_event;
        event->multi_previous = pl_event->multi_previous;
        pl_event->multi_previous->multi_next = event;
        pl_event->multi_previous = event;
    }
    return;
}

//----回收running事件申请的内存------------------------------------------------
//功能: 在内存管理的事件id表中查找由event_id事件申请的局部内存，如果有，则登记
//      enum_mem_leak错误，并强行释放之。
//参数: event_id，目标事件id
//返回: 无
//-----------------------------------------------------------------------------
void __m_cleanup(uint16_t event_id)
{
}

//----初始化内存堆-------------------------------------------------------------
//功能：1.从堆中分配访问控制结构所需的内存，位置在堆的顶部，占用整数块
//      2.初始化内存分配控制结构,初始化全局内存结构 struct mem_global
//      3.初始化内存分配表
//返回：1=成功，0=失败
//备注: 1.本函数在初始化完成之前调用,中断尚未开启,无需考虑关闭中断的问题.
//      2.调用本函数前,只能能执行静态分配功能,不能执行动态分配功能.
//      3.目前只适用于不带mmu的情况，带mmu特别是支持多进程的情况还没有想好。
//-----------------------------------------------------------------------------
ptu32_t module_init_heap_dynamic(ptu32_t para)
{
    ptu32_t ua_temp,ua_temp1=0,ua_temp2,ua_temp3 = 0,ua_faction;
    ptu32_t ua_pages,ua_table_size=0;
    ufast_t uf_classes,uf_grades;
    ucpu_t  ***pppl_bitmap,**ppl_bitmap,*pl_bitmap;
    ufast_t *pl_classes;


//    for(ua_temp=0;ua_temp<cn_ptr_bits;ua_temp++)
//    {   //计算页尺寸中1的个数,
//        if(cn_page_size&(1<<ua_temp))
//            ua_temp1++;
//    }
//    if(ua_temp1!=1) //页尺寸中只有一个1时说明正好是2的整数次方
//        return(enum_mem_page_error);
//注释掉的原因:cn_page_size将由配置程序生成,肯定是2的整数次方.

    tg_MCB.dynamic = true;

#if(cn_virtual_memory == 0)
    ua_faction=(ptu32_t)(tg_MCB.heap_top-tg_MCB.heap_bottom);
    //计算堆的总页数，取整页，舍去余数部分
    ua_pages= ua_faction>>cn_page_size_suffix_zero;
    //计算堆页数调整后的余数，可用于内存控制结构使用。
    ua_faction=ua_faction-(ua_pages<<cn_page_size_suffix_zero);
    tg_MCB.heap_top -=ua_faction;
#else
    ua_faction = 0;     //本行用于阻止警告
//    ua_faction = tg_MCB.heap_top;
//    //向下对其页边界
//    tg_MCB.heap_top = tg_MCB.heap_top & (~(cn_page_size-1));
//    //向上对其页边界
//    tg_MCB.heap_bottom = (tg_MCB.heap_bottom + cn_page_size -1)
//                                & (~(cn_page_size-1));
//    ua_faction -= tg_MCB.heap_top;
#endif

    if(ua_pages < 2) //如果堆中页数小于2页,控制结构可能还要占用1页,还有意义吗?
    {
        return 0;
    }
    mutex_create_knl(&tg_mem_mutex,"dynamic memory allocation");

    ua_temp = __m_calculate_expense(ua_pages); //初估控制结构需要的字节数
    if(ua_faction < ua_temp)
    {
        //初估控制结构除ua_faction外还需要的页数
        ua_temp = (ua_temp-ua_faction+cn_page_size-1)>>cn_page_size_suffix_zero;
        for(ua_temp1 = ua_temp; ua_temp1 > 0; ua_temp1--)
        {//初估的页数肯定大于或等于实际需要的页数,尤其是内存页数较多时.
         //估计值从初估的页数开始逐一减小,直到合适为止.
            ua_temp2 = __m_calculate_expense(ua_pages-ua_temp1);//重估控制结构字节数
            ua_temp3 = (ua_temp2-ua_faction+cn_page_size-1)
                        >> cn_page_size_suffix_zero;//重新计算控制结构页数
            if(ua_temp3 >= ua_temp1)
            //当所需页数刚好大于或等于估计值时,循环终止
                break;
        }
        ua_pages = ua_pages -ua_temp3;//实际可分配的页数,u32_temp3为控制结构页数
    //    (tg_MCB.heap_top) -= 1;
        tg_MCB.heap_top
                        -= ua_temp3<<cn_page_size_suffix_zero;  //调整堆顶部位置
    }
    tg_MCB.ua_pages_num = ua_pages;
    tg_MCB.free_pages_num = ua_pages;
    //内存控制结构清零
    memset(tg_MCB.heap_top,0,ua_faction+(ua_temp3<<cn_page_size_suffix_zero));

    //计算总阶数，即一共有多少种页尺寸,方法:地址长度减去ua_pages中前导0的数量
    //例如,ua_pages=5,最高位位置是bit2,有29个前导0，页尺寸有1,2,4页3种,阶数=3
    uf_grades=cn_ptr_bits-__m_leading_zero((u8 *)&ua_pages,sizeof(ptu32_t));

    tg_MCB.uf_grades = uf_grades;    //总阶数
    tg_MCB.ua_block_max=cn_page_size<<(uf_grades-1);//最高阶块尺寸
    tg_MCB.ua_free_block_max=tg_MCB.ua_block_max; //最大可用块尺寸

    //计算位图指针表所需的指针数
    ua_table_size=0;
    for(ua_temp=0;ua_temp<uf_grades;ua_temp++)
    {//遍历各阶
        ua_temp1 = ua_pages>>ua_temp;   //计算ua_temp阶0级位图位数
        do
        {
            //计算下一级位图的位数
            ua_temp1=(ua_temp1+cn_cpu_bits-1)>>cn_cpu_bits_suffix_zero;
            ua_table_size++;        //需要一个指针指向该级位图首地址
        }while(ua_temp1>1); //下一级位图超过1位(本级超过cn_cpu_bits位),继续循环
    }

    //事件指针id表,详细说明见mem_global_t类型定义部分.
    tg_MCB.index_event_id=(uint16_t*)tg_MCB.heap_top;

    //各阶的位图级数表指针，每阶用一个ufast_t型的数表示该阶的位图级数
    ua_temp = (ptu32_t)(tg_MCB.index_event_id+ua_pages);
    ua_temp = (ua_temp+sizeof(ufast_t)-1)&(~(sizeof(ufast_t)-1));       //对齐
    pl_classes=(ufast_t *)ua_temp;
    tg_MCB.p_classes=pl_classes;     //各阶空闲金字塔级数表首指针

    //随后是各级分配表指针的指针首址
    ua_temp = (ptu32_t)(pl_classes+uf_grades);
    ua_temp = (ua_temp+sizeof(void *)-1)&(~(sizeof(void *)-1));         //对齐
    pppl_bitmap=(void*)ua_temp;
    tg_MCB.ppp_bitmap=pppl_bitmap; //各阶空闲金字塔位图指针表的指针表的首指针

    ppl_bitmap=(ucpu_t **)(pppl_bitmap+uf_grades);  //空闲金字塔位图指针表的指针
    pl_bitmap=(ucpu_t *)(ppl_bitmap+ua_table_size); //空闲金字塔位图指针表

    //填充各索引表和分配表
    for(ua_temp=0;ua_temp<uf_grades;ua_temp++)
    {//遍历各阶
        ua_temp1 = ua_pages>>ua_temp;       //ua_temp阶位图总位数
        uf_classes=0;                       //ua_temp阶的位图级数
        pppl_bitmap[ua_temp]=ppl_bitmap;    //ua_temp阶位图表的指针
        do
        {
            ucpu_t  msk=~0; //类型位宽未知,用此方法置全1
            ua_temp3=ua_temp1>>cn_cpu_bits_suffix_zero; //本级表的大小(整数部分)
            ua_temp1=ua_temp1%cn_cpu_bits;    //本路径级表的大小(余数部分)
            if(ua_temp1==0)
            //表中没有多余的位,无需处理
                ua_temp1=ua_temp3;  //本级位图所占字数是下一级的位数
            else
            {//表中有多余的位,置1,使多余的位等同于已分配的位.
                msk=msk<<ua_temp1;              //使多余的位置1
                *(pl_bitmap+ua_temp3) |=msk;    //修改位图中相应的位
                ua_temp1=ua_temp3+1;    //路径表修正,加上余数部分
            }
            *ppl_bitmap++ = pl_bitmap;  //记录本级位图指针
            pl_bitmap +=ua_temp1;       //位图指针指向下一级位图起始地址
            uf_classes++;                //ua_temp阶的级数增量
        }while(ua_temp1>1);             //直到遍历本阶所有位图级
        pl_classes[ua_temp]=uf_classes; //该级路径表深度
    }
//    if(cn_mem_recycle)    //提供内存回收功能,建议小内存不要使用内存回收
//    {//此时初始化未完成,无需考虑中断关闭的问题.
//        pg_mem_record_free=__inc_record_link();
//    }
    return 1;
}

//----从内存堆中分配内存-------------------------------------------------------
//功能：1.规格化内存尺寸，计算满足要求的最小内存尺寸，计算该块内存尺寸的阶数
//      2.读取该级访问路径深度，沿访问路径逐级查找，直到找到空闲内存为止。
//      3.重置内存分配表
//      4.把内存分配信息置入链表中,以备清理内存用.
//      5.如果内存不足，则把事件挂在tg_mem_global.event_wait下,并引发事件切换.
//参数：size,欲分配的内存块尺寸
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回：分配的内存指针，NULL表示没有内存可以分配
//备注: 如果在多事件调度启动前调用本函数，记录拥有者时全部算在系统服务事件中。
//      系统服务事件永不结束，故等同于全局分配。
//-----------------------------------------------------------------------------
void *m_malloc(ptu32_t size,u32 timeout)
{
    u8 *ua_address;
    ufast_t  uf_grade_th;
    bool_t   en_scheduler;
    void *result;
    uint16_t *pl_id,id;
    u32 page;

    if(size == 0)
        return NULL;
    //启动多事件调度后，dynamic成员不会再发生变化，即使并发访问也是安全的
    if(tg_MCB.dynamic==false)
        return(__m_static_malloc(size));  //内存尚未初始化,执行准静态内存分配
    //不能在此直接判断size是否满足,因为取得互斥量前可能发生切换而判断无效.
//    int_save_asyn_signal();
    if(mutex_pend(&tg_mem_mutex,timeout) == false)
        return NULL;
    en_scheduler = djy_query_sch();
    if((tg_MCB.ua_free_block_max < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( !__m_check_memory(size,timeout) )            //没有合适的空闲内存块
        {
            result = NULL;
        }else    //有合适的空闲内存块
        {
            uf_grade_th=__m_get_grade(size);          //取阶号
            ua_address=__malloc_block(uf_grade_th); //申请内存
            pg_event_running->local_memory++;

//            __record_mem(ua_address,uf_grade_th);   //记录内存
            //阅读以下条件句请结合mem_global_t中index_event_id成员定义的注释.
            pl_id = tg_MCB.index_event_id;
            id = pg_event_running->event_id;
            page = (ptu32_t)(ua_address-tg_MCB.heap_bottom)
                    >>cn_page_size_suffix_zero;
            if(uf_grade_th==0)
            {//分配1页
                pl_id[page] = id;
            }else if(uf_grade_th==1)
            {//分配2页
                pl_id[page] = (u16)-1;
                pl_id[page+1] = id;
            }else
            {   //分配多页
                pl_id[page] = (u16)-2;
                pl_id[page+1] = id;
                pl_id[page+2] = uf_grade_th;
            }
            result = ua_address;
        }
    }
    mutex_post(&tg_mem_mutex);
//    int_restore_asyn_signal();
    return result;
}

//----分配全局内存-------------------------------------------------------------
//功能：1.规格化内存尺寸，计算满足要求的最小内存尺寸，计算该块内存属于第几阶。
//      2.读取该级访问路径深度，沿访问路径逐级查找，直到找到空闲内存为止。
//      3.重置内存分配表
//      4.如果内存不足，则把事件挂在tg_mem_global.event_wait下,并引发事件切换.
//参数：size,欲分配的内存块尺寸
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回：分配的内存指针，NULL表示没有内存可以分配
//备注: 用此函数分配的内存,并不会在事件完成时被收回.
//-----------------------------------------------------------------------------
void *m_malloc_gbl(ptu32_t size,u32 timeout)
{
    u8 *ua_address;
    ufast_t  uf_grade_th;
    uint16_t    *pl_id;
    bool_t   en_scheduler;
    void *result;

    if(tg_MCB.dynamic==false)
        return(__m_static_malloc(size));  //内存尚未初始化,执行准静态内存分配
     //不能在此直接判断size是否满足,因为关取得互斥量前可能发生切换而判断无效.
        if(mutex_pend(&tg_mem_mutex,timeout) == false)
            return NULL;
        en_scheduler = djy_query_sch();
    if((tg_MCB.ua_free_block_max < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( ! __m_check_memory(size,timeout))
        {
            result = NULL;
        }else
        {
            uf_grade_th=__m_get_grade(size);          //取阶号
            ua_address=__malloc_block(uf_grade_th); //申请内存

            //以下在id表中记录本次分配的性质
            //阅读本段代码请结合mem_global_t中index_event_id成员定义的注释.
            pl_id = &tg_MCB.index_event_id
                    [(ptu32_t)(ua_address-tg_MCB.heap_bottom)
                            >>cn_page_size_suffix_zero];
            if(0==uf_grade_th)
                *pl_id = (u16)-3;
            else
            {
                *pl_id++ = (u16)-4;
                *pl_id = uf_grade_th;
            }
            result = ua_address;
        }
    }
    mutex_post(&tg_mem_mutex);
    return result;
}

//----分配1块内存--------------------------------------------------------------
//功能: 从内存堆中分配一个内存块,内部调用,不开放给用户
//参数: grade,欲分配的块的阶号,0=1阶,1=2阶,类推之
//返回: 获得的内存块指针,如果不能分配则返回NULL
//-----------------------------------------------------------------------------
void *__malloc_block(ufast_t grade)
{
    ptu32_t ua_temp1;
    ptu32_t ua_pages_number;
    ufast_t uf_word_shift,uf_grade_th,uf_classes;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;

    ptu32_t ua_word_offset,ua_bit_num,ua_loop;
    ucpu_t  uc_msk;               //字内偏移量

    pppl_bitmap=tg_MCB.ppp_bitmap; //空闲金字塔位图指针的指针表的首指针
    ppl_bitmap=pppl_bitmap[grade];        //该阶空闲金字塔位图指针表的首址
    pl_classes=tg_MCB.p_classes;   //读取各阶空闲金字塔级数表指针.
    uf_classes=pl_classes[grade]-1;       //该阶空闲金字塔最高一级指针在
                                          //ppl_bitmap中的偏移量
    tg_MCB.free_pages_num -= (1<<grade); //剩余可用页数
    ua_pages_number=0;
    do
    {//本循环查找第一个可分配块(即bit=0)的偏移位数,从高到低查找,方法:
     //首先,找出该阶空闲位图金字塔最高级位图中第一个bit=0的位偏移量.
     //然后,根据这个偏移量计算次高级含0位的字偏移,读出该字并找出bit=0的位偏移.
     //如此反复可以找到最后一级第一个0位的偏移,这就是本次分配的目标块.

        pl_bitmap=ppl_bitmap[uf_classes];   //读取各级位图的指针

        //ua_pages_number是上一级的位偏移,ua_temp1是本级最后一字的字内位偏移
        ua_temp1=cn_cpu_bits-1-__m_leading_ucpu_zero(~pl_bitmap[ua_pages_number]);

        //上一级的字偏移×字长+本级字内位偏移得到本级总位偏移
        ua_pages_number =(ua_pages_number<<cn_cpu_bits_suffix_zero)+ua_temp1;
    }while(uf_classes-- >0);

    ua_pages_number <<= grade;  //目标块的首页的页号

    //下面开始在bitmap表标记本次分配情况
    for(uf_grade_th=0;uf_grade_th<=grade;uf_grade_th++)
    {//从0阶直到当前阶(不包含),处理阶号低于当次申请的内存的位图.
        //块的起始页号右移阶号即得到在该阶0级位图中的偏移
        ua_word_offset = ua_pages_number>>uf_grade_th;
        ppl_bitmap=pppl_bitmap[uf_grade_th];    //取得该阶分配表指针表的首址
        ua_bit_num=1<<(grade-uf_grade_th);      //计算掩码位数
        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {//从低级到高级遍历该阶各级位图
            pl_bitmap=ppl_bitmap[ua_temp1]; //相应级相应路径分配表首址
            //计算字内位偏移,下面算式起取模的作用
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //字偏移
            if(ua_bit_num>=cn_cpu_bits)   //ua_bit_num保存的是需要置1的位
            {//位数大于或等于1个字宽,因为采用2的n次方分阶,所以要处理的位数肯定
             //是整数个字,按整数个字置1就可以了.

                //计算字数,也是下一级位图中需置1的位数
                ua_bit_num >>=cn_cpu_bits_suffix_zero;
                for(ua_loop =0; ua_loop < ua_bit_num; ua_loop++)
                    pl_bitmap[ua_word_offset+ua_loop]=cn_allbit_1;
            }else if(ua_bit_num>0)
            //位数小于一个字宽,且大于0
            {
                uc_msk=cn_low_xbit_msk[ua_bit_num]<<uf_word_shift;
                pl_bitmap[ua_word_offset] |=uc_msk;
                if(pl_bitmap[ua_word_offset] == cn_allbit_1)
                //被置1的位所在的字变为全1时，该字对应的下一级位图相应的位需置1
                    ua_bit_num=0;
                else
                    break;
            }else
            {//前一次操作使一个字从非全1变成全1，置位该字对应的下一级路径相应位
                pl_bitmap[ua_word_offset] |= 1<<uf_word_shift;
                if(pl_bitmap[ua_word_offset] == cn_allbit_1)
                //被置1的位所在的字变为全1时，该字对应的下一级位图相应的位需置1
                    ;
                else
                    break;
            }
        }
    }

    for(; uf_grade_th < tg_MCB.uf_grades; uf_grade_th++)
    {//当前阶(包含)到最高阶,处理阶号高于或等于当次申请的内存的位图.每次只需1位置1

        //块的起始页号右移阶号即得到在该阶0级位图中的偏移
        ua_word_offset = ua_pages_number >> uf_grade_th;
        if(ua_word_offset >= tg_MCB.ua_pages_num>>uf_grade_th)
        //该阶无对应块,这是可能的,比如一个共10页的内存,第9页和第10页都在第3阶中没有
        //映射.条件式右边是本阶总块数,左端是从0起计的偏移量,最大只能是总块数-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //取得该阶分配表指针表的首址

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //计算字内位偏移,下面算式起取模的作用
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //字偏移
            pl_bitmap[ua_word_offset] |= 1<<uf_word_shift;
            if(pl_bitmap[ua_word_offset] == cn_allbit_1)
            //被掩模的位所在的字变为全1时，该字对应的下一级位图相应的位需置1
                ;
            else
                break;
        }
    }

    //刷新最大空闲内存块
    uf_grade_th=tg_MCB.uf_grades-1;     //最高阶号
    do{
        uf_classes= pl_classes[uf_grade_th];        //第uf_grade_th阶总级数
        ppl_bitmap   = pppl_bitmap[uf_grade_th];    //位图索引表指针
        pl_bitmap    = ppl_bitmap[uf_classes-1];    //最高级位图指针
        if(*pl_bitmap != (~0))
        {//路径顶端只要不是全1就表示该阶肯定有空闲块.
            //根据阶号计算最大空闲块的尺寸.
            tg_MCB.ua_free_block_max=cn_page_size<<uf_grade_th;
            break;
        }
    }while(uf_grade_th-- >0);   //从最高阶(最大块)扫描到0阶

    return tg_MCB.heap_bottom
                    +(ua_pages_number<<cn_page_size_suffix_zero);
}

//----获取格式化尺寸-----------------------------------------------------------
//功能: 虽然应用程序允许申请任意尺寸内存，但内存管理器分配给用户的内存却是满足
//      用户需求的规格化尺寸的内存块，本函数告诉应用程序该规格化尺寸，但不实际
//      执行分配内存的操作，也不保证随后的分配操作能否成功。
//参数: size，应用程序欲分配的尺寸
//返回: 规格化尺寸
//-----------------------------------------------------------------------------
ptu32_t m_format_size(ptu32_t size)
{
    if(tg_MCB.dynamic==false)
        return align_up_sys(size);  //准静态分配，对齐即可
    else
        return(cn_page_size << (__m_get_grade(size)+cn_page_size_suffix_zero));
}

//----释放从堆分配的内存-------------------------------------------------------
//功能: 释放从堆中分配的一块内存，如果此时动态分配还没有初始化，则执行准静态
//      内存释放。释放内存后，重新计算最大空闲块的尺寸，如果有事件在内存同步
//      队列中，则把等待的内存尺寸小于新的最大空闲块尺寸的事件激活。
//参数：pl_mem,待释放的内存指针
//返回：错误返回flase,正确时返回true
//-----------------------------------------------------------------------------
bool_t m_free(void * pl_mem)
{
    ptu32_t ua_temp1;
    ptu32_t ua_word_offset;
    ufast_t uf_word_shift;  //字内偏移量
    ptu32_t ua_pages_no;
    ufast_t uf_grade_th,uf_free_grade_th;
    ufast_t uf_classes;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;
    uint16_t *pl_id;    //id;
    struct  event_ECB  *event;
    ptu32_t ua_bit_num;
    ucpu_t  uc_msk;

    if(pl_mem == NULL)
        return false;
    //下面用到的tg_mem_global中的dynamic、heap_bottom和heap_top成员，在启动多
    //事件调度后，均不再变化，并发读是安全的。
    if(tg_MCB.dynamic==false)
        return(__m_static_free(pl_mem));  //内存尚未初始化,执行准静态内存分配
    if(((u8*)pl_mem<tg_MCB.heap_bottom)
                || ((u8*)pl_mem>=tg_MCB.heap_top))
    {
        djy_error_login(enum_mem_error,NULL);  //欲释放的内存不在内存堆中,直接退出.
        return false;
    }

    //计算释放的内存块的首页页号
    ua_pages_no=(ptu32_t)((ptu32_t)pl_mem-(ptu32_t)tg_MCB.heap_bottom)
                        >>cn_page_size_suffix_zero;

    mutex_pend(&tg_mem_mutex,cn_timeout_forever);
    //查找释放的内存块的阶号,从0起计.通过阶号也可以确定内存块的大小.
    //确定内存块的类型,局部内存需要知道拥有该内存的事件id，
    //全局内存无需清理内存分配跟踪表,无需知道拥有该内存的事件id
    pl_id = &tg_MCB.index_event_id[ua_pages_no];
    switch(pl_id[0])
    {
        case 0xffff :
        {   //双页局部内存,-1+id
//            id = pl_id[1];
            uf_free_grade_th = 1;
        }break;
        case 0xfffe :
        {   //多页局部内存:-2+id+阶号
//            id = pl_id[1];
            uf_free_grade_th = pl_id[2];
        }break;
        case 0xfffd :
        {   //单页全局内存:-3
            uf_free_grade_th = 0;
        }break;
        case 0xfffc :
        {   //双(多)页全局内存:-4+阶号.
            uf_free_grade_th = pl_id[1];
        }break;
        default :
        {
            if(pl_id[0] <= cn_event_id_limit)
            {   //单页局部内存:id,
//                id = pl_id[0];
                uf_free_grade_th = 0;
            }else
            {
                djy_error_login(enum_mem_error,NULL);   //指针有错,直接退出
                return false;

            }
        }break;
    }
/*
    if(0xffff != id)
    {   //待释放的是局部内存,需要删除内存分配记录结点.
        //绝大多数情况下,id 等于running,但也可能有例外,不能直接用running指针.
        event = __djy_lookup_id(id);
        record = event->held_memory;
        if(record == NULL)
        {
            djy_error_login(enum_mem_error,NULL);  //链表空,直接退出.
            return false;
        }
        if(record->address == pl_mem)
        {   //与第一个记录匹配
            event->held_memory = record->next;
            record->next = pg_mem_record_free;  //被删除的结点连到free表头部
            pg_mem_record_free = record;        //移动free指针
        } else
        {
            for(;record->next != NULL;record = record->next)
            //循环直到找到待删除的结点的前一个结点
                if(record->next->address == pl_mem)
                    break;
            if(record->next ==NULL)
            {
                djy_error_login(enum_mem_error,NULL);//链表中没有搜索到记录,直接退出
                return false;
            }else
            {   //找到记录
                struct mem_record *p;
                p = record->next;               //取得待删除的结点
                record->next = p->next;         //删除结点
                p->next = pg_mem_record_free;   //被删除的结点连到free表头部
                pg_mem_record_free=p;           //移动free指针
            }
        }
    }
*/
    pppl_bitmap=tg_MCB.ppp_bitmap; //空闲金字塔位图指针的指针表的首指针
//    ppl_bitmap=pppl_bitmap[uf_free_grade_th];//该阶空闲金字塔位图指针表的首址
//    pl_bitmap=ppl_bitmap[0];                 //空闲金字塔位图指针
    pl_classes=tg_MCB.p_classes;    //读取各阶空闲金字塔级数表指针.
    uf_classes=pl_classes[uf_free_grade_th]-1;  //该阶空闲金字塔最高一级指针在
                                                //ppl_bitmap中的偏移量

    //计算位偏移
//    ua_word_offset =ua_pages_no>>uf_free_grade_th;
    //计算字内位偏移,下面算式起取模的作用
//    uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
//    ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //计算字偏移

    //空闲页数增加
    tg_MCB.free_pages_num += 1<<uf_free_grade_th;

    //下面开始在bitmap表标记本次释放情况
    for(uf_grade_th = 0; uf_grade_th <= uf_free_grade_th; uf_grade_th++)
    {//从0阶直到当前阶(不包含),处理阶号低于当次申请的内存的位图.
        //块的起始页号右移阶号即得到在该阶0级位图中的偏移
        ua_word_offset =ua_pages_no>>uf_grade_th;
        ppl_bitmap=pppl_bitmap[uf_grade_th];        //取得该级分配表指针表的首址
        ua_bit_num=1<<(uf_free_grade_th-uf_grade_th);       //计算掩码位数
        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {//从低级到高级遍历该阶各级位图
            pl_bitmap = ppl_bitmap[ua_temp1]; //相应级相应路径分配表首址
            //计算字内位偏移,下面算式起取模的作用
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset = ua_word_offset>>cn_cpu_bits_suffix_zero;  //字偏移
            if(ua_bit_num>=cn_cpu_bits)   //ua_bit_num保存的是需要清0的位
            {//位数大于或等于1个字宽,因为采用2的n次方分级,所以要处理的位数肯定
             //是整数个字,按整数个字清0就可以了.

                //计算字数,也是下一级位图中需清0的位数
                ua_bit_num >>=cn_cpu_bits_suffix_zero;
                //uc_word_shift在这里当临时变量使用
                for(uf_word_shift =0; uf_word_shift < ua_bit_num; uf_word_shift++)
                    pl_bitmap[ua_word_offset+uf_word_shift]=0;
            }else if(ua_bit_num > 0)
            {//位数小于一个字宽,且大于0
                uc_msk=~(cn_low_xbit_msk[ua_bit_num]<<uf_word_shift);
                pl_bitmap[ua_word_offset] &=uc_msk;
                if(pl_bitmap[ua_word_offset] == uc_msk)
                //被清0的位所在的字全1全1->有0，该字对应的下一级位图相应位清0
                    ua_bit_num=0;
                else
                    break;
            }else
            {//前一次操作使一个字从全1->有0，该字对应的下一级路径相应位清0
                pl_bitmap[ua_word_offset] &= ~(1<<uf_word_shift);
                if(pl_bitmap[ua_word_offset] == ~(1<<uf_word_shift))
                //被清0的位所在的字全1->有0，该字对应的下一级位图相应位清0
                    ;
                else
                    break;
            }
        }
    }

    for(;uf_grade_th<tg_MCB.uf_grades;uf_grade_th++)
    {//当前阶(包含)到最高阶,处理阶号高于或等于当次申请的内存的位图.每次只需清1位
        //取低一阶对应本阶两块中较低地址的偏移量
        ua_word_offset = (ua_pages_no >> uf_grade_th)<<1;
        //计算字内位偏移,下面算式起取模的作用
        uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
        ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //字偏移
        ppl_bitmap=pppl_bitmap[uf_grade_th-1]; //取得该级分配表指针表的首址
        pl_bitmap = ppl_bitmap[0];
        if( (pl_bitmap[ua_word_offset] & (3<<uf_word_shift)))
            break;      //本阶位图无须清零，更高的阶也无须清零
        
        //块的起始页号右移阶号即得到在该阶0级位图中的偏移
        ua_word_offset=ua_pages_no>>uf_grade_th;
        if(ua_word_offset>=tg_MCB.ua_pages_num>>uf_grade_th)
        //该阶无对应块,这是可能的,比如一个共10页的内存,第9页和第10页都在第3阶中
        //无映射.条件式右边是本阶总块数,左端是从0起计的偏移量,最大只能是总块数-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //取得该级分配表指针表的首址

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //计算字内位偏移,下面算式起取模的作用
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //字偏移
            pl_bitmap[ua_word_offset] &= ~(1<<uf_word_shift);
            if(pl_bitmap[ua_word_offset] == ~(1<<uf_word_shift))
            //被清0的位所在的字全1->有0，该字对应的下一级位图相应位清0
                ;
            else
                break;
        }
    }

    //刷新最大空闲内存块
    uf_grade_th=tg_MCB.uf_grades-1;     //取最高阶的阶号
    do{
        uf_classes   = pl_classes[uf_grade_th];     //第uf_grade_th阶的总级数
        ppl_bitmap   = pppl_bitmap[uf_grade_th];    //位图索引表指针
        pl_bitmap    = ppl_bitmap[uf_classes-1];    //最高级位图指针
        if(*pl_bitmap != (~0))
        {//路径顶端只要不是全1就表示该阶肯定有空闲块.
            //根据阶号计算最大空闲块的尺寸.
            tg_MCB.ua_free_block_max=cn_page_size<<uf_grade_th;
            break;
        }
    }while(uf_grade_th-- >0);   //从最高阶(最大块)扫描到0阶

//    if(0xffff == id)
//    {//如果不加这句,出现下述情况时将发生内存错误
     //1.申请局部内存,堆用完-->记录本次分配后-->发现pg_mem_record_free空
     //     -->没有内存增加链表长度.
     //2.释放全局内存,使得又有内存可以分配.
     //3.此时申请局部内存,记录时可能出错.
//        if(pg_mem_record_free==NULL)
//            pg_mem_record_free=__inc_record_link();
//    }
    mutex_post(&tg_mem_mutex);
    pg_event_running->local_memory--;

    //把内存等待队列中申请内存之和小于当前可用最大内存的几个事件放到ready队列
    //等待队列是双向循环链表
    int_save_asyn_signal();
    if(tg_MCB.mem_sync != NULL)
    {
        event = tg_MCB.mem_sync;   //取等待队列头
        ua_temp1 = tg_MCB.ua_free_block_max;
        while(1)
        {
            //ua_word_offset在此权当临时变量使用,规格化当前事件需要的内存.
            ua_word_offset = __m_get_grade(event->wait_mem_size)
                            << cn_page_size_suffix_zero;
            if(ua_word_offset <= ua_temp1)
            {//事件等待的内存量小于最大空闲内存块,把事件放到就绪队列
                //剩余可用尺寸减少当前激活的事件所申请的内存,但并不实际执行申请
                ua_temp1 -= __m_get_grade(event->wait_mem_size)
                            << cn_page_size_suffix_zero;
                event->last_status.all = event->event_status.all;
//                event->event_status.bit.wait_memory = 0;
                event->sync_head = NULL;
                if(event->event_status.bit.wait_overtime)
                    __djy_resume_delay(event);
                event->event_status.all = 0;
                if(event->multi_next == event->multi_previous)
                {//是最后一个事件
                    tg_MCB.mem_sync  = NULL;
                    break;
                }else
                {
                    tg_MCB.mem_sync = event->multi_next;
                    event->multi_next->multi_previous = event->multi_next;
                    event->multi_previous->multi_next = event->multi_previous;
                }
                __djy_event_ready(event);
            }else
                break;
        }
    }
    int_restore_asyn_signal();  //恢复中断状态
    return(true);
}

//----查询最大可用内存块-------------------------------------------------------
//功能：返回最大可用内存块的尺寸
//参数：无
//返回：内存块字节数
//-----------------------------------------------------------------------------
ptu32_t  m_get_max_free_mem(void)
{
    return tg_MCB.ua_free_block_max;
}

//----查询堆空间大小---------------------------------------------------------
//功能：返回堆空间大小
//参数：无
//返回：内存堆字节数
//-----------------------------------------------------------------------------
ptu32_t  m_get_heap_size(void)
{
    return tg_MCB.ua_pages_num<<cn_page_size_suffix_zero;
}

//----查询总空闲内存大小-------------------------------------------------------
//功能：返回空闲内存字节数
//参数：无
//返回：空闲内存字节数
//-----------------------------------------------------------------------------
ptu32_t  m_get_free_mem(void)
{
    return tg_MCB.free_pages_num<<cn_page_size_suffix_zero;
}

//----查询页尺寸---------------------------------------------------------------
//功能：返回页字节数
//参数：无
//返回：页字节数
//-----------------------------------------------------------------------------
ptu32_t  m_get_page_size(void)
{
    return cn_page_size;
}
#endif
