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
//1. 日期: 2009-10-31
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __ARCH_H_
#define __ARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define arm920t

//存储器大小端的配置
#define cn_little_endian       0
#define cn_big_endian          1
#define cn_cpu_endian          cn_little_endian

//与CPU字长相关的配置
#define cn_cpu_bits     32  //处理器字长
#define cn_ptr_bits     32  //指针字长，移植敏感
#define cn_cpu_bits_suffix_zero 5
#define cn_byte_bits    8   //字节位宽



//以下是CPU可以用一条指令处理的数据类型，多用于需要保持数据完整性的操作，以及
//需要快速的操作。
//数据完整性和原子操作的异同:
//数据完整性是指在任何时候都能读到正确值的数据，比如下列语句:
//  uint32_t    b32;
//  b32++;  //b32的原值是0x0000ffff
//在32位risc机上，执行过程为：
//取b32地址到寄存器0-->取b32的数值到寄存器1-->寄存器1加1-->把寄存器1写入b32.
//上述过程无论在哪里被中断或者被高优先级线程打断，在中断或高优先级线程中读b32，
//要么得到0x0000ffff，要么得到0x00010000,这两都可以认为是正确数据.
//而在16位机上，执行过程是
//1、取0x0000到寄存器r0，取0xffff到寄存器r1
//2、执行加操作，使r0=0x0001,r1=0x0000
//3、把r0写入b32的高16位。
//4、把r1写入b32的低16位。
//如果在第3和第4步骤之间被中断打断，在中断里读b32，将得到错误的0x00001ffff。
//那么原子操作呢?就是整个执行过程不被任何中断或者高优先级线程打断，还看上面代码，
//如果b32被定义为原子变量，则无论在16位还是32位机上，b32都可以得到正确的值。目
//前，djyos并不支持原子变量，只能使用互斥量实现原子变量的效果。
    #define cn_limit_ucpu   cn_limit_uint32
    #define cn_min_ucpu     (0)
    #define cn_limit_scpu   cn_limit_sint32
    #define cn_min_scpu     cn_min_sint32
//以下是快速数据类型，一般来说，CPU类型是快速数据类型，但有些系统存储器宽度小于
//CPU字长，导致从存储器读一个cpu字长的数需要多个存储器周期，另外,16位和32位cpu
//中,字节操作也并不一定最快.对于有些对字长没有要求的数据类型来说，使用与存储器
//宽度等长的变量也许是最快速的。配置16位SDRAM的44b0x快速类型是16位的.
    #define cn_limit_ufast  cn_limit_uint16
    #define cn_min_ufast    (0)
    #define cn_limit_sfast  cn_limit_sint16
    #define cn_min_sfast    cn_min_sint16

//下面定义cpu系统的栈类型
#define cn_full_down_stack  0   //向下生长的满栈
#define cn_empty_down_stack 1   //向下生长的空栈
#define cn_full_up_stack    2   //向上生长的满栈
#define cn_empty_up_stack   3   //向上生长的空栈
#define cn_stack_type       cn_full_down_stack

//代理内核栈的深度计算方法:栈需求最深的那个系统服务
#define cn_kernel_stack     0x200   //代理内核栈，线程需要为系统服务额外增加的栈
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


