//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:中断模块
//作者：lst
//版本：V1.1.0
//文件描述: cm3异常处理模块
//其他说明:
//修订历史:
//2. 日期: 2010-01-20
//   作者: lst
//   新版本号: V1.1.0
//   修改说明: 修改 exp_esr_tick函数，否则在tick中断中调用异步信号的使能和禁止
//      函数时，由于en_asyn_signal_counter等参数未正确设置，会出问题。
//1. 日期: 2009-08-30
//   作者: lst
//   新版本号: V1.0.0
//   修改说明:
//------------------------------------------------------
#include "os_inc.h"
#include "cortexm0.h"
#include "exception.h"
void (*user_systick)(void) = NULL_func;
void exp_systick_handler(void);
void exp_svc_handler(void);


void exp_init(void)
{
    pg_scb_reg->pri12_15 |=0xff000000;    //systick设为最低优先级,=异步信号
    pg_scb_reg->pri8_11 |= 0xff000000;    //svc的优先级和异步信号相同。
}
void exp_connect_nmi(void (*esr)(void))
{
}

void exp_connect_systick(void (*tick)(void))
{
    user_systick = tick;
}

void exp_esr_tick(void)
{
    if((pg_systick_reg->ctrl & bm_systick_ctrl_tickint) == 0)
	    return;
    bg_schedule_enable = false;
//    tg_int_global.en_asyn_signal = false;
//    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal++;
    user_systick();

    tg_int_global.nest_asyn_signal--;
//    tg_int_global.en_asyn_signal = true;
//    tg_int_global.en_asyn_signal_counter = 0;
    if(pg_event_ready != pg_event_running)
        __djy_schedule_asyn_signal();       //执行中断内调度
    bg_schedule_enable = true;
}
