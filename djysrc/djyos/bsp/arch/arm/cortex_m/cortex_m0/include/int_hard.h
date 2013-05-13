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
typedef struct
{
    ucpu_t bitmsk;
    ucpu_t en_systick;
}atom_low_t;

#define cn_real_prio_default    0x80    //默认的实时中断优先级，本移植版本不支持
                                        //实时中断嵌套，本设置无效
                                        
// 以下配置中断模块的常量，不但跟硬件有关，也跟软件策略有关。m0中，PRIMASK作为总
// 中断开关，而异步信号则没有独立开关，这与m3有明显的区别，m3中用BASEPRI寄存器作
// 为独立的异步信号开关
#define cfg_asyn_turnkey    0   //是否支持独立的异步信号使能开关
#define cfg_trunk_turnkey   1   //是否支持独立的总中断使能开关

#define cfg_asyn_reg_atom   0   //操作异步信号开关寄存器的原子性，即读或者写
                                //异步信号开关时，是否可以一条指令操作完。
                                //举例:arm9的s3c2410,INTMSK是一个32位寄存器，
                                //      在32位cpu上，一条指令可执行读或写，
                                //      可认为具有原子性，本常量设为1
                                //     arm9的s3c2416，INTMSK有64位，需两个
                                //      32位寄存器，至少两条指令才能读或写，
                                //      故不具原子性，本常量设为0
                                //     在cortex-m3上，直接读写BASEPRI寄存器，
                                //      也是一套指令操作完，具备原子性，设为1
                                //      m0上，因还要读取systick的使能位，不能一
                                //      次完成。

struct nvic_reg      //at 0xe000e100,
{
    vu32 setena;    //写1使能相应中断，读1=中断被使能
    vu32 res0[31];
    vu32 clrena;    //写1禁止相应中断，读1=中断被使能
    vu32 res1[31];
    vu32 setpend;   //写1悬挂中断，读1=中断被挂起
    vu32 res2[31];
    vu32 clrpend;   //写1清除中断挂起状态，读1=中断被挂起
    vu32 res3[31];
    vu32 res4[64];
    vu32 pri[8];     //优先级，32个中断，每个占2位
};

extern struct nvic_reg volatile * const pg_nvic_reg;

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
bool_t int_set_prio(ufast_t ufl_line,u32 prio);
void __int_init_hard(void);
void int_init(void);
void __int_engine_real(ufast_t ufl_line);
void __int_engine_asyn_signal(ufast_t ufl_line);

#ifdef __cplusplus
}
#endif

#endif //__int_hard_h__
