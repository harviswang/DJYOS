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
typedef struct
{
    ucpu_t bitmsk;
    ucpu_t en_systick;
}atom_low_t;

#define cn_real_prio_default    0x80    //Ĭ�ϵ�ʵʱ�ж����ȼ�������ֲ�汾��֧��
                                        //ʵʱ�ж�Ƕ�ף���������Ч
                                        
// ���������ж�ģ��ĳ�����������Ӳ���йأ�Ҳ����������йء�m0�У�PRIMASK��Ϊ��
// �жϿ��أ����첽�ź���û�ж������أ�����m3�����Ե�����m3����BASEPRI�Ĵ�����
// Ϊ�������첽�źſ���
#define cfg_asyn_turnkey    0   //�Ƿ�֧�ֶ������첽�ź�ʹ�ܿ���
#define cfg_trunk_turnkey   1   //�Ƿ�֧�ֶ��������ж�ʹ�ܿ���

#define cfg_asyn_reg_atom   0   //�����첽�źſ��ؼĴ�����ԭ���ԣ���������д
                                //�첽�źſ���ʱ���Ƿ����һ��ָ������ꡣ
                                //����:arm9��s3c2410,INTMSK��һ��32λ�Ĵ�����
                                //      ��32λcpu�ϣ�һ��ָ���ִ�ж���д��
                                //      ����Ϊ����ԭ���ԣ���������Ϊ1
                                //     arm9��s3c2416��INTMSK��64λ��������
                                //      32λ�Ĵ�������������ָ����ܶ���д��
                                //      �ʲ���ԭ���ԣ���������Ϊ0
                                //     ��cortex-m3�ϣ�ֱ�Ӷ�дBASEPRI�Ĵ�����
                                //      Ҳ��һ��ָ������꣬�߱�ԭ���ԣ���Ϊ1
                                //      m0�ϣ���Ҫ��ȡsystick��ʹ��λ������һ
                                //      ����ɡ�

struct nvic_reg      //at 0xe000e100,
{
    vu32 setena;    //д1ʹ����Ӧ�жϣ���1=�жϱ�ʹ��
    vu32 res0[31];
    vu32 clrena;    //д1��ֹ��Ӧ�жϣ���1=�жϱ�ʹ��
    vu32 res1[31];
    vu32 setpend;   //д1�����жϣ���1=�жϱ�����
    vu32 res2[31];
    vu32 clrpend;   //д1����жϹ���״̬����1=�жϱ�����
    vu32 res3[31];
    vu32 res4[64];
    vu32 pri[8];     //���ȼ���32���жϣ�ÿ��ռ2λ
};

extern struct nvic_reg volatile * const pg_nvic_reg;

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
bool_t int_set_prio(ufast_t ufl_line,u32 prio);
void __int_init_hard(void);
void int_init(void);
void __int_engine_real(ufast_t ufl_line);
void __int_engine_asyn_signal(ufast_t ufl_line);

#ifdef __cplusplus
}
#endif

#endif //__int_hard_h__
