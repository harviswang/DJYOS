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
#include "cortexm0.h"
#include "exception.h"
void (*user_systick)(void) = NULL_func;
void exp_systick_handler(void);
void exp_svc_handler(void);


void exp_init(void)
{
    pg_scb_reg->pri12_15 |=0xff000000;    //systick��Ϊ������ȼ�,=�첽�ź�
    pg_scb_reg->pri8_11 |= 0xff000000;    //svc�����ȼ����첽�ź���ͬ��
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
        __djy_schedule_asyn_signal();       //ִ���ж��ڵ���
    bg_schedule_enable = true;
}
