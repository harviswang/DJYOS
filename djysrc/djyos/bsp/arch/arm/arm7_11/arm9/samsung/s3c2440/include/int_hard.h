//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ���ж���صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __int_hard_h__
#define __int_hard_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef ucpu_t atom_high_t;
typedef ucpu_t atom_low_t;

//���������ж�ģ��ĳ�����������Ӳ���йأ�Ҳ����������йء���ʵ���첽�źź�
//ʵʱ�жϾ�ʹ��irq��irq��Ϊ���жϿ��أ����첽�ź���û�ж�����ʹ�ܿ��أ���ֹ�첽
//�ź���ͨ����ֹ���б�����Ϊ�첽�źŵ��жϺ������ʵ�ֵģ���û�ж������첽�ź�
//ʹ�ܿ��ء������ֲ�߾�����fiqʵ��ʵʱ�жϣ���irqʵ���첽�źţ�������ж�����
//�첽�ź�ʹ�ܿ��ء�
#define cfg_asyn_turnkey    0   //�Ƿ�֧�ֶ������첽�ź�ʹ�ܿ���
#define cfg_trunk_turnkey   1   //�Ƿ�֧�ֶ��������ж�ʹ�ܿ���

#define cfg_asyn_reg_atom   1   //�����첽�źſ��ؼĴ�����ԭ���ԣ���������д
                                //�첽�źſ���ʱ���Ƿ����һ��ָ������ꡣ
                                //����:arm9��s3c2410,INTMSK��һ��32λ�Ĵ�����
                                //      ��32λcpu�ϣ�һ��ָ���ִ�ж���д��
                                //      ����Ϊ����ԭ���ԣ���������Ϊ1
                                //     arm9��s3c2416��INTMSK��64λ��������
                                //      32λ�Ĵ�������������ָ����ܶ���д��
                                //      �ʲ���ԭ���ԣ���������Ϊ0
                                //     ��cortex-m3�ϣ�ֱ�Ӷ�дBASEPRI�Ĵ�����
                                //      Ҳ��һ��ָ������꣬�߱�ԭ���ԣ���Ϊ1

struct int_reg
{
    u32 SRCPND   ;    //0x4a000000 ���жϲ�������1
    u32 INTMOD   ;    //0x4a000004 Interrupt mode control
    u32 INTMSK   ;    //0x4a000008 Interrupt mask control
    u32 PRIORITY ;    //0x4a00000c IRQ priority control
    u32 INTPND   ;    //0x4a000010 SRCPND=1��λ������������1
    u32 INTOFFSET;    //0x4a000014 Interruot request source offset
    u32 SUBSRCPND;    //0x4a000018 Sub source pending
    u32 INTSUBMSK;    //0x4a00001c Interrupt sub mask
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
