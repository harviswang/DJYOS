//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:调度器
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------


#ifndef __CPU_H__
#define __CPU_H__

#ifdef __cplusplus
extern "C" {
#endif

#define __asm_start_thread __asm_turnto_context

struct  thread_vm *__create_thread(struct  event_type *evtt);
void __asm_reset_thread(void (*thread_routine)(void),
                           struct  thread_vm  *vm);
void __asm_reset_switch(void (*thread_routine)(void),
                           struct  thread_vm *new_vm,struct  thread_vm *old_vm);
void __asm_turnto_context(struct  thread_vm  *new_vm);
void __asm_switch_context(struct  thread_vm *new_vm,struct  thread_vm *old_vm);
void __asm_switch_context_int(struct  thread_vm *new_vm,struct  thread_vm *old_vm);
u32 djy_get_time(void);
void djy_delay_10us(volatile uint16_t time);
u32 djy_get_time_tick(void);
u32 djy_get_fine_time(void);
u32 djy_get_for_time(ufast_t var_len);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_H__*/

