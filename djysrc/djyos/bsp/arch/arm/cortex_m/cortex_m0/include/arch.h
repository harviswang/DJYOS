//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:堆管理模块
//作者：lst
//版本：V1.0.0
//文件描述: 内核配置头文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-10-16
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __ARCH_H_
#define __ARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

//存储器大小端的配置
#define cn_little_endian       0
#define cn_big_endian          1
#define cn_cpu_endian          cn_little_endian

//与CPU字长相关的配置
#define cn_cpu_bits     32  //处理器字长
#define cn_ptr_bits     32  //CPU地址位数，移植敏感
#define cn_cpu_bits_suffix_zero 5
#define cn_byte_bits    8   //字节位宽

    #define cn_limit_ucpu   cn_limit_uint32
    #define cn_min_ucpu     (0)

    #define cn_limit_scpu   cn_limit_sint32
    #define cn_min_scpu     cn_min_sint32

    #define cn_limit_ufast  cn_limit_uint8
    #define cn_min_ufast    (0)

    #define cn_limit_sfast  cn_limit_sint8
    #define cn_min_sfast    cn_min_sint8

//下面定义cpu系统的栈类型
#define cn_full_down_stack  0   //向下生长的满栈
#define cn_empty_down_stack 1   //向下生长的空栈
#define cn_full_up_stack    2   //向上生长的满栈
#define cn_empty_up_stack   3   //向上生长的空栈
#define cn_stack_type       cn_full_down_stack

//定义是否支持cache，loader根据本常量决定是否做cache同步
#define cn_cache_used       0
#define cn_mmu_used         0   //定义是否支持mmu
#define cortex_m0

//代理内核栈的深度计算方法:栈需求最深的那个系统服务
#define cn_kernel_stack     300     //代理内核栈，线程需要为系统服务额外增加的栈
                                    //目前最大api函数dev_add_root_device 需256bytes
                                    //----2009-10-11

//定义一些与对齐相关的宏
#define mem_align           1           //如果目标系统没有对齐要求，改为0
#define align_size          8           //arm(含cm3)要求8字节对齐

#define align_down(bound,x) ((x)&(~((bound)-1)))    //把x按bound边界向下对齐
#define align_up(bound,x)   (((x)+((bound)-1))&(~((bound)-1)))  //向上对齐,

//以下声明本系统的数据对齐方式，移植敏感
#define align_down_sys(x)   align_down(8,x)   //arm要求8字节对齐
#define align_up_sys(x)     align_up(8,x)     //arm要求8字节对齐

#define define_align_buf(name,size)     \
                align_type name[align_up_sys(size)/sizeof(align_size)]

#ifdef __cplusplus
}
#endif
#endif // __ARCH_H_


