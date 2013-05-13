//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ж�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ���ж���صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __int_h__
#define __int_h__

#include "cpu_peri_int_line.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  cn_asyn_signal     (0)
#define  cn_real            (1)

//��ʾ���ж���״̬��λͼռucpu_t���͵�����
#define cn_int_bits_words   ((cn_int_line_last+cn_cpu_bits)/cn_cpu_bits)

//�ж������ݽṹ��ÿ�ж�һ��
//��ֲ����
struct int_line
{
    u32 (*ISR)(ufast_t line);
    struct  event_ECB *sync_event;       //���ڵȴ����жϷ������¼�
    ucpu_t en_counter;          //��ֹ��������,����0ʱ��ʾ�����ж�
    ucpu_t int_type;            //1=ʵʱ�ж�,0=�첽�ź�
    bool_t enable_nest;         //true=���ж���Ӧ�ڼ�����Ƕ�ף���Ӳ������������
                                //�ԣ�Ҳ����������йء�����cortex-m3�汾���첽
                                //�źű�����Ϊ������ȼ����Ӷ������첽�źŶ���
                                //����Ƕ�ס�
                                //�ر�ע�⣬ʵʱ�ж��ܹ�������Ƕ���첽�źš�
                                //�ж���Ӧ�����ж��������enable_nest��ֵʹ��
                                //���ֹ�ж��������Ƿ�����Ƕ�ף��������Ӧ�ж�
                                //��Ӳ��û��������ֹ�жϣ�����һ��СС��"����"
                                //���ڸô����ڣ�������Ƕ�׵ġ�����cm3��ʵʱ�ж�

    uint16_t my_evtt_id;
    u32  prio;                  //���ȼ���������ʹ���߽���
};

//�ж��ܿ����ݽṹ.
struct int_master_ctrl
{
    //�ж�������λͼ��0=�첽�źţ�1=ʵʱ�ж�,�����λ���պÿ��������ж�����,��
    //�ж������ݽṹ��int_type��Ա������ͬ��
    ucpu_t  property_bitmap[cn_int_bits_words];
    ucpu_t nest_asyn_signal;   //�ж�Ƕ�����,������=0,��һ�ν����ж�=1,���εݼ�
    ucpu_t nest_real;   //�ж�Ƕ�����,������=0,��һ�ν����ж�=1,���εݼ�
    //�ж���ʹ��λͼ,1=ʹ��,0=��ֹ,��ӳ��Ӧ���ж��ߵĿ���״̬,
    //���ܿ���/�첽�źſ��ص�״̬�޹�.
    ucpu_t  enable_bitmap[cn_int_bits_words];
//    bool_t  en_trunk;           //1=���ж�ʹ��,  0=���жϽ�ֹ
//    bool_t  en_asyn_signal;         //1=�첽�ź�ʹ��,0=�첽�źŽ�ֹ
    ucpu_t en_trunk_counter;   //ȫ���жϽ�ֹ����,=0��ʾ����ȫ���ж�
    ucpu_t en_asyn_signal_counter; //�첽�źŽ�ֹ����,=0��ʾ�����첽�ź�
};

extern struct int_master_ctrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern struct int_line tg_int_table[];      //�ж��߿������ݽṹ
extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�

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
