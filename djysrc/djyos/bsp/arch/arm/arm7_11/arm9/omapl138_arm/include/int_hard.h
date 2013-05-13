/*========================================================*
 * 文件模块说明:                                                 *
 * omapl138的中断硬件相关文件，移植自都江堰操作系统作者拥有版权
 * 的S3C2440中断硬件相关文件，针对omapl138做了一些修改
 * 文件版本: v1.00                                               *
 * 开发人员: lst                              *
 * 创建时间: 2011.06.01                                          *
 * Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             *
 *========================================================*
 * 程序修改记录(最新的放在最前面):							     *
 *  <版本号> <修改日期>, <修改人员>: <修改功能概述>				 *
 *========================================================*
* 例如：
*  V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核
*
*/
//以下是原作者版权声明
//----------------------------------------------------
//Copyright (C), 2004-2009,  都江堰操作系统开发团队
//版权所有 (C), 2004-2009,   都江堰操作系统开发团队
//所属模块:中断模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 与中断相关的代码，包含异步信号与实时中断
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __int_hard_h__
#define __int_hard_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef u32 atom_high_t;
typedef u32 atom_low_t;

//以下配置中断模块的常量，不但跟硬件有关，也跟软件策略有关。例如在cm3中，异步信
//号不能嵌套，就是软件策略选择的结果。一般来说，软件策略应该尽可能让实时中断能够
//支持嵌套。
#define cfg_asyn_turnkey    1   //是否支持独立的异步信号使能开关
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


#define reg_hier_fiq_bit    0x01    //hier寄存器的bit0=1使能fiq，=0禁止fiq
#define reg_hier_irq_bit    0x02    //hier寄存器的bit1=1使能fiq，=0禁止fiq
//注:"引用"的意思是按中断号对个别中断执行一定的操作
//   "掩码"的意思是用位掩码的方式同时对多个中断执行操作
//注1:中断响应后，因不冻结GPIR和GPVR，存在读取其中一个时，另一个发生变化的可能，
//    在现实中很难应用
struct st_int_reg  //基址 0xfffee000 偏移
{
    vu32 REVID;      //0000 硬件版本寄存器
    vu32 CR;         //0004 控制寄存器
    vu8 RSVD0[8];
    vu32 GER;        //0010 全局使能寄存器
    vu8 RSVD1[8];
    vu32 GNLR;       //001c 全局嵌套级别寄存器
    vu32 SISR;       //0020 中断状态引用设置寄存器
    vu32 SICR;       //0024 中断状态引用清除寄存器
    vu32 EISR;       //0028 中断状态引用使能寄存器
    vu32 EICR;       //002c 中断状态引用除能寄存器
    vu8 RSVD2[4];
    vu32 HIEISR;     //0034 写0使能fiq，写1使能irq
    vu32 HIDISR;     //0038 写0除能fiq，写1除能irq
    vu8 RSVD3[20];
    vu32 VBR;        //0050 向量表基址寄存器
    vu32 VSR;        //0054 向量表中每项尺寸
    vu32 VNR;        //0058 空向量寄存器--------指向空函数即可
    vu8 RSVD4[36];
    vu32 GPIR;       //0080 正悬挂的最高优先级中断----无意义，见注1
    vu32 GPVR;       //0084 正悬挂的最高优先级中断的向量
    vu8 RSVD5[376];
    vu32 SRSR[4];    //0200 悬挂状态掩码寄存器，写入相应位可引起中断
    vu8 RSVD6[112];
    vu32 SECR[4];    //0280 位掩码，写1清除中断标志，(读无效，ti文档有误)
    vu8 RSVD7[112];
    vu32 ESR[4];     //0300 位掩码，只写，写1使能相应中断，写0无效
    vu8 RSVD8[112];
    vu32 ECR[4];     //0380 位掩码，只写，写1除能相应中断，写0无效
    vu8 RSVD9[112];
    vu32 CMR[26];    //0400 通道设置，设置中断号与中断通道的对应关系，
                             //     共26个寄存器，每个中断占用8位，通道号对应
                             //     的是优先级，通道号低优先级高。同一个通道
                             //     中，中断号低的优先级高。
                             //     实时中断的通道号可任意设置为0或1.
                             //     异步信号的通道号可任意设置为2~31
    vu8 RSVD10[1176];
    vu32 HIPIR1;     //0900 悬挂的fiq中断中最高优先级中断号
    vu32 HIPIR2;     //0904 悬挂的irq中断中最高优先级中断号
    vu8 RSVD11[2040];
    vu32 HINLR1;     //1100 fiq中断嵌套级别寄存器，一般不用
    vu32 HINLR2;     //1104 irq中断嵌套级别寄存器，一般不用
    vu8 RSVD14[1016];
    vu32 HIER;       //1500 bit0写1使能fiq，写0除能，bit1操作irq。
    vu8 RSVD15[252];
    vu32 HIPVR1;     //1600 正在响应的fiq中断向量地址
    vu32 HIPVR2;     //1604 正在响应的irq中断向量地址
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
