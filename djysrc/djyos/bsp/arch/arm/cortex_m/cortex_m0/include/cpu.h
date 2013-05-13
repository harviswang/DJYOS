//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:调度器
//作者：lst
//版本：V1.0.0
//文件描述:调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------


#ifndef __CPU_H__
#define __CPU_H__

#ifdef __cplusplus
extern "C" {
#endif

extern struct systick_reg volatile * const pg_systick_reg;
extern struct scb_reg volatile * const pg_scb_reg;
extern struct nvic_reg volatile * const pg_nvic_reg;

u32 djy_get_time_tick(void);
u32 djy_get_time(void);
struct  thread_vm *__create_thread(struct  event_type *evtt);
void *__asm_reset_thread(void (*thread_routine)(void),
                           struct  thread_vm  *vm);
void __asm_reset_switch(void (*thread_routine)(void),
                           struct  thread_vm *new_vm,struct  thread_vm *old_vm);
void __asm_start_thread(struct  thread_vm  *new_vm);
void __asm_turnto_context(struct  thread_vm  *new_vm);
void __asm_switch_context(struct  thread_vm *new_vm,struct  thread_vm *old_vm);
void __asm_switch_context_int(struct  thread_vm *new_vm,struct  thread_vm *old_vm);
u32 djy_get_fine_time(void);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_H__*/

