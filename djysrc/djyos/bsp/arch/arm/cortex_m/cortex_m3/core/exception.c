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
#include "cortexm3.h"
#include "exception.h"
u32 u32g_esr_table[16];
void (*user_systick)(void) = NULL_func;
void exp_systick_handler(void);
void exp_svc_handler(void);

u32 u32g_exp_table[16] __attribute__((section(".table.exceptions")));

void exp_init(void)
{
	extern   uint32_t   msp_top[ ];
	extern   void cpu_init(void);
	extern   void hardfault_handler(void);

    u32g_exp_table[0]  = (uint32_t)msp_top;
    u32g_exp_table[cn_sysvect_reset]  = (uint32_t) cpu_init;
    u32g_exp_table[cn_sysvect_nmi]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_hard_fault]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_memmanage_fault]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_bus_fault]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_usage_fault]  = (uint32_t) hardfault_handler;
    u32g_exp_table[7]  = (uint32_t) hardfault_handler;
    u32g_exp_table[8]  = (uint32_t) hardfault_handler;
    u32g_exp_table[9]  = (uint32_t) hardfault_handler;
    u32g_exp_table[10]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_svc] = (u32)exp_svc_handler;
    u32g_exp_table[cn_sysvect_debug]  = (uint32_t) hardfault_handler;
    u32g_exp_table[13]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_pendsv]  = (uint32_t) hardfault_handler;
    u32g_exp_table[cn_sysvect_systick] = (u32)exp_systick_handler;

 	pg_scb_reg->VTOR = (u32)u32g_exp_table;
    pg_scb_reg->systick_pri15 = 0xff;   //最低优先级,systick应该能随异步信号关断
    pg_scb_reg->svc_pri11 = 0x0;       //svc的优先级和异步信号相同。
    pg_scb_reg->SHCSR |=0x70000;     //允许 总线、用法、存储器 fault
}
void exp_connect_nmi(void (*esr)(void))
{
    u32g_exp_table[cn_sysvect_nmi] = (u32)esr;
}

void exp_connect_systick(void (*tick)(void))
{
    user_systick = tick;
}

void exp_esr_tick(void)
{
    bg_schedule_enable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;

    user_systick();

    tg_int_global.nest_asyn_signal = 0;
//    tg_int_global.en_asyn_signal = true;
    tg_int_global.en_asyn_signal_counter = 0;
    if(pg_event_ready != pg_event_running)
        __djy_schedule_asyn_signal();       //执行中断内调度
    bg_schedule_enable = true;
}
