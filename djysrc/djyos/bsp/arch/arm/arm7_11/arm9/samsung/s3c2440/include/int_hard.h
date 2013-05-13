//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:中断模块
//作者：lst
//版本：V1.0.0
//文件描述: 与中断相关的代码，包含异步信号与实时中断
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __int_hard_h__
#define __int_hard_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef ucpu_t atom_high_t;
typedef ucpu_t atom_low_t;

//以下配置中断模块的常量，不但跟硬件有关，也跟软件策略有关。本实现异步信号和
//实时中断均使用irq，irq作为总中断开关，而异步信号则没有独立的使能开关，禁止异步
//信号是通过禁止所有被配置为异步信号的中断号来间接实现的，故没有独立的异步信号
//使能开关。如果移植者决定用fiq实现实时中断，用irq实现异步信号，则可以有独立的
//异步信号使能开关。
#define cfg_asyn_turnkey    0   //是否支持独立的异步信号使能开关
#define cfg_trunk_turnkey   1   //是否支持独立的总中断使能开关

#define cfg_asyn_reg_atom   1   //操作异步信号开关寄存器的原子性，即读或者写
                                //异步信号开关时，是否可以一条指令操作完。
                                //举例:arm9的s3c2410,INTMSK是一个32位寄存器，
                                //      在32位cpu上，一条指令可执行读或写，
                                //      可认为具有原子性，本常量设为1
                                //     arm9的s3c2416，INTMSK有64位，需两个
                                //      32位寄存器，至少两条指令才能读或写，
                                //      故不具原子性，本常量设为0
                                //     在cortex-m3上，直接读写BASEPRI寄存器，
                                //      也是一套指令操作完，具备原子性，设为1

struct int_reg
{
    u32 SRCPND   ;    //0x4a000000 有中断产生就置1
    u32 INTMOD   ;    //0x4a000004 Interrupt mode control
    u32 INTMSK   ;    //0x4a000008 Interrupt mask control
    u32 PRIORITY ;    //0x4a00000c IRQ priority control
    u32 INTPND   ;    //0x4a000010 SRCPND=1的位，若允许则置1
    u32 INTOFFSET;    //0x4a000014 Interruot request source offset
    u32 SUBSRCPND;    //0x4a000018 Sub source pending
    u32 INTSUBMSK;    //0x4a00001c Interrupt sub mask
};

#define cn_noirq    0x80
#define cn_nofiq    0x40
#define cn_noint    0xc0

atom_high_t int_high_atom_start(void);
void int_high_atom_end(atom_high_t high);
atom_low_t int_low_atom_start(void);
void int_low_atom_end(atom_low_t low);
void int_contact_asyn_signal(void);
void int_cut_asyn_signal(void);
void int_contact_trunk(void);
void int_cut_trunk(void);
bool_t int_contact_line(ufast_t ufl_line);
bool_t int_cut_line(ufast_t ufl_line);
bool_t int_echo_line(ufast_t ufl_line);
bool_t int_tap_line(ufast_t ufl_line);
void int_echo_all_line(void);
bool_t int_query_line(ufast_t ufl_line);
bool_t int_setto_asyn_signal(ufast_t ufl_line);
bool_t int_setto_real(ufast_t ufl_line);
bool_t int_enable_nest(ufast_t ufl_line);
bool_t int_disable_nest(ufast_t ufl_line);
bool_t int_set_prio(ufast_t ufl_line,u32 prio);
void __int_init_hard(void);
void int_init(void);
void __int_engine_real(ufast_t ufl_line);
void __int_engine_asyn_signal(ufast_t ufl_line);
void __int_engine_all(ufast_t ufl_line);

void int_enable_arm_int(void);
void int_disable_arm_int(void);
void __int_enable_irq(void);
void __int_disable_irq(void);
void __int_enable_fiq(void);
void __int_disable_fiq(void);

#ifdef __cplusplus
}
#endif

#endif //__int_hard_h__
