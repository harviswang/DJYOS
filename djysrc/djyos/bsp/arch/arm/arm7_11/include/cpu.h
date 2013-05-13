//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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

