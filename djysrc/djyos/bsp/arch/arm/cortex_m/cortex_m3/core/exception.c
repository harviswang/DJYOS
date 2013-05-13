//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ����: 2010-01-20
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: �޸� exp_esr_tick������������tick�ж��е����첽�źŵ�ʹ�ܺͽ�ֹ
//      ����ʱ������en_asyn_signal_counter�Ȳ���δ��ȷ���ã�������⡣
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
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
    pg_scb_reg->systick_pri15 = 0xff;   //������ȼ�,systickӦ�������첽�źŹض�
    pg_scb_reg->svc_pri11 = 0x0;       //svc�����ȼ����첽�ź���ͬ��
    pg_scb_reg->SHCSR |=0x70000;     //���� ���ߡ��÷����洢�� fault
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
        __djy_schedule_asyn_signal();       //ִ���ж��ڵ���
    bg_schedule_enable = true;
}
