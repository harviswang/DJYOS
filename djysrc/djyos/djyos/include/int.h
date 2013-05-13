//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
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
#ifndef __int_h__
#define __int_h__

#include "cpu_peri_int_line.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  cn_asyn_signal     (0)
#define  cn_real            (1)

//表示各中断线状态的位图占ucpu_t类型的字数
#define cn_int_bits_words   ((cn_int_line_last+cn_cpu_bits)/cn_cpu_bits)

//中断线数据结构，每中断一个
//移植敏感
struct int_line
{
    u32 (*ISR)(ufast_t line);
    struct  event_ECB *sync_event;       //正在等待本中断发生的事件
    ucpu_t en_counter;          //禁止次数计数,等于0时表示允许中断
    ucpu_t int_type;            //1=实时中断,0=异步信号
    bool_t enable_nest;         //true=本中断响应期间允许嵌套，对硬件能力有依赖
                                //性，也与软件设置有关。例如cortex-m3版本，异步
                                //信号被设置为最低优先级，从而所有异步信号都不
                                //允许嵌套。
                                //特别注意，实时中断能够无条件嵌套异步信号。
                                //中断响应后，由中断引擎根据enable_nest的值使能
                                //或禁止中断来控制是否允许嵌套，如果在响应中断
                                //后，硬件没有立即禁止中断，将有一个小小的"窗口"
                                //，在该窗口内，是允许嵌套的。例如cm3的实时中断

    uint16_t my_evtt_id;
    u32  prio;                  //优先级，含义由使用者解析
};

//中断总控数据结构.
struct int_master_ctrl
{
    //中断线属性位图，0=异步信号，1=实时中断,数组的位数刚好可以容纳中断数量,与
    //中断线数据结构的int_type成员含义相同。
    ucpu_t  property_bitmap[cn_int_bits_words];
    ucpu_t nest_asyn_signal;   //中断嵌套深度,主程序=0,第一次进入中断=1,依次递加
    ucpu_t nest_real;   //中断嵌套深度,主程序=0,第一次进入中断=1,依次递加
    //中断线使能位图,1=使能,0=禁止,反映相应的中断线的控制状态,
    //与总开关/异步信号开关的状态无关.
    ucpu_t  enable_bitmap[cn_int_bits_words];
//    bool_t  en_trunk;           //1=总中断使能,  0=总中断禁止
//    bool_t  en_asyn_signal;         //1=异步信号使能,0=异步信号禁止
    ucpu_t en_trunk_counter;   //全局中断禁止计数,=0表示允许全局中断
    ucpu_t en_asyn_signal_counter; //异步信号禁止计数,=0表示允许异步信号
};

extern struct int_master_ctrl  tg_int_global;          //定义并初始化总中断控制结构
extern struct int_line tg_int_table[];      //中断线控制数据结构
extern ufast_t tg_int_lookup_table[];       //中断线查找表

void int_save_trunk(void);
void int_restore_trunk(void);
bool_t int_check_trunk(void);
void int_save_asyn_signal(void);
void __int_reset_asyn_signal(void);
void int_restore_asyn_signal(void);
void int_half_enable_asyn_signal(void);
bool_t int_check_asyn_signal(void);
bool_t int_save_asyn_line(ufast_t ufl_line);
bool_t int_save_real_line(ufast_t ufl_line);
bool_t int_restore_asyn_line(ufast_t ufl_line);
bool_t int_restore_real_line(ufast_t ufl_line);
bool_t int_disable_asyn_line(ufast_t ufl_line);
bool_t int_disable_real_line(ufast_t ufl_line);
bool_t int_enable_asyn_line(ufast_t ufl_line);
bool_t int_enable_real_line(ufast_t ufl_line);
bool_t int_check_line(ufast_t ufl_line);
void int_isr_connect(ufast_t ufl_line, u32 (*isr)(ufast_t));
void int_evtt_connect(ufast_t ufl_line,uint16_t my_evtt_id);
void int_isr_disconnect(ufast_t ufl_line);
void int_evtt_disconnect(ufast_t ufl_line);
void int_reset_asyn_sync(ufast_t ufl_line);
bool_t int_asyn_signal_sync(ufast_t ufl_line);

#ifdef __cplusplus
}
#endif

#endif //__int_h__
