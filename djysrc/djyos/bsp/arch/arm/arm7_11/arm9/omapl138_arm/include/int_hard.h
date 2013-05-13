/*========================================================*
 * �ļ�ģ��˵��:                                                 *
 * omapl138���ж�Ӳ������ļ�����ֲ�Զ����߲���ϵͳ����ӵ�а�Ȩ
 * ��S3C2440�ж�Ӳ������ļ������omapl138����һЩ�޸�
 * �ļ��汾: v1.00                                               *
 * ������Ա: lst                              *
 * ����ʱ��: 2011.06.01                                          *
 * Copyright(c) 2011-2011  ��������������޹�˾             *
 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):							     *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>				 *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/
//������ԭ���߰�Ȩ����
//----------------------------------------------------
//Copyright (C), 2004-2009,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�����Ŷ�
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
#ifndef __int_hard_h__
#define __int_hard_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef u32 atom_high_t;
typedef u32 atom_low_t;

//���������ж�ģ��ĳ�����������Ӳ���йأ�Ҳ����������йء�������cm3�У��첽��
//�Ų���Ƕ�ף������������ѡ��Ľ����һ����˵���������Ӧ�þ�������ʵʱ�ж��ܹ�
//֧��Ƕ�ס�
#define cfg_asyn_turnkey    1   //�Ƿ�֧�ֶ������첽�ź�ʹ�ܿ���
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


#define reg_hier_fiq_bit    0x01    //hier�Ĵ�����bit0=1ʹ��fiq��=0��ֹfiq
#define reg_hier_irq_bit    0x02    //hier�Ĵ�����bit1=1ʹ��fiq��=0��ֹfiq
//ע:"����"����˼�ǰ��жϺŶԸ����ж�ִ��һ���Ĳ���
//   "����"����˼����λ����ķ�ʽͬʱ�Զ���ж�ִ�в���
//ע1:�ж���Ӧ���򲻶���GPIR��GPVR�����ڶ�ȡ����һ��ʱ����һ�������仯�Ŀ��ܣ�
//    ����ʵ�к���Ӧ��
struct st_int_reg  //��ַ 0xfffee000 ƫ��
{
    vu32 REVID;      //0000 Ӳ���汾�Ĵ���
    vu32 CR;         //0004 ���ƼĴ���
    vu8 RSVD0[8];
    vu32 GER;        //0010 ȫ��ʹ�ܼĴ���
    vu8 RSVD1[8];
    vu32 GNLR;       //001c ȫ��Ƕ�׼���Ĵ���
    vu32 SISR;       //0020 �ж�״̬�������üĴ���
    vu32 SICR;       //0024 �ж�״̬��������Ĵ���
    vu32 EISR;       //0028 �ж�״̬����ʹ�ܼĴ���
    vu32 EICR;       //002c �ж�״̬���ó��ܼĴ���
    vu8 RSVD2[4];
    vu32 HIEISR;     //0034 д0ʹ��fiq��д1ʹ��irq
    vu32 HIDISR;     //0038 д0����fiq��д1����irq
    vu8 RSVD3[20];
    vu32 VBR;        //0050 �������ַ�Ĵ���
    vu32 VSR;        //0054 ��������ÿ��ߴ�
    vu32 VNR;        //0058 �������Ĵ���--------ָ��պ�������
    vu8 RSVD4[36];
    vu32 GPIR;       //0080 �����ҵ�������ȼ��ж�----�����壬��ע1
    vu32 GPVR;       //0084 �����ҵ�������ȼ��жϵ�����
    vu8 RSVD5[376];
    vu32 SRSR[4];    //0200 ����״̬����Ĵ�����д����Ӧλ�������ж�
    vu8 RSVD6[112];
    vu32 SECR[4];    //0280 λ���룬д1����жϱ�־��(����Ч��ti�ĵ�����)
    vu8 RSVD7[112];
    vu32 ESR[4];     //0300 λ���룬ֻд��д1ʹ����Ӧ�жϣ�д0��Ч
    vu8 RSVD8[112];
    vu32 ECR[4];     //0380 λ���룬ֻд��д1������Ӧ�жϣ�д0��Ч
    vu8 RSVD9[112];
    vu32 CMR[26];    //0400 ͨ�����ã������жϺ����ж�ͨ���Ķ�Ӧ��ϵ��
                             //     ��26���Ĵ�����ÿ���ж�ռ��8λ��ͨ���Ŷ�Ӧ
                             //     �������ȼ���ͨ���ŵ����ȼ��ߡ�ͬһ��ͨ��
                             //     �У��жϺŵ͵����ȼ��ߡ�
                             //     ʵʱ�жϵ�ͨ���ſ���������Ϊ0��1.
                             //     �첽�źŵ�ͨ���ſ���������Ϊ2~31
    vu8 RSVD10[1176];
    vu32 HIPIR1;     //0900 ���ҵ�fiq�ж���������ȼ��жϺ�
    vu32 HIPIR2;     //0904 ���ҵ�irq�ж���������ȼ��жϺ�
    vu8 RSVD11[2040];
    vu32 HINLR1;     //1100 fiq�ж�Ƕ�׼���Ĵ�����һ�㲻��
    vu32 HINLR2;     //1104 irq�ж�Ƕ�׼���Ĵ�����һ�㲻��
    vu8 RSVD14[1016];
    vu32 HIER;       //1500 bit0д1ʹ��fiq��д0���ܣ�bit1����irq��
    vu8 RSVD15[252];
    vu32 HIPVR1;     //1600 ������Ӧ��fiq�ж�������ַ
    vu32 HIPVR2;     //1604 ������Ӧ��irq�ж�������ַ
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
