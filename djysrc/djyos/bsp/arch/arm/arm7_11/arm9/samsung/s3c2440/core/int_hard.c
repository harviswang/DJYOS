//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �ж�ģ����Ӳ����صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ԭint.c�а�Ӳ���޹ش����������
//------------------------------------------------------
#include "os_inc.h"
#include "exception.h"

struct int_reg volatile * const pg_int_reg
                        = (struct int_reg *)0x4a000000;
//�����ж�������Σ�һ����ԣ��쳣��������쳣������ת���͸ö�һͬ�����׵�ַ
u32 u32g_vect_table[cn_int_line_last+1] __attribute__((section(".table.vectors")));

//----����߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ�����ǰ���ж�״̬��Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ��
//      ��ϵĲ�����
//      int_high_atom_start--int_high_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
//      Ƕ�׵ĵط�������ʹ��int_save_trunk���ر��ж�(����)��
//      ��������������"int_restore_trunk--int_save_trunk"�Ŀ��ٰ汾����������
//      Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ���������int_restore_trunk��
//      int_save_trunk�е��κ�һ����
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_high_t int_high_atom_start(void)
{
    volatile atom_high_t ret;
    __asm__ volatile(
    		".equ	cn_noint, 0xc0 ;"
    		"mrs	%0, cpsr \n\t"
    		"orr	r0, %0, #cn_noint \n\t"
    		"msr	cpsr_c, r0 \n\t"
    		"mov	r0, %0 \n\t"
    		: "=r" (ret)
    		:
			);
    return ret;
}

//----�뿪�߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ�����ǰ���ж�״̬��������Ӧ��int_high_atom_start��������
//      int_high_atom_start--int_high_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
//      Ƕ�׵ĵط�������ʹ��int_save_trunk���ر��ж�(����)��
//      ��������������"int_restore_trunk--int_save_trunk"�Ŀ��ٰ汾����������
//      Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ���������int_restore_trunk��
//      int_save_trunk�е��κ�һ����
//������high��int_high_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void int_high_atom_end(atom_high_t high)
{
	__asm__(
			"msr cpsr_c,%0 \n\t" : :"r"(high)
	);
}

//----����ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ���ȡ��ǰ�첽�źſ���״̬��Ȼ���ֹ�첽�źš��ͼ���ԭ�Ӳ�����������ֹ�ٽ�
//      ���ݵĲ������ʣ������ܻᱻʵʱ�жϴ�ϡ�
//      int_low_atom_start---int_low_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
//      Ƕ�׵ĵط�������ʹ��int_save_asyn_signal���ر��ж�(����)��
//      ��������������"int_restore_asyn_signal--int_save_asyn_signal"�Ŀ��ٰ汾��
//      ��������Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ���������
//      int_restore_asyn_signal��int_save_asyn_signal�е��κ�һ����
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_low_t int_low_atom_start(void)
{
    register ucpu_t msk_lsb;
	atom_low_t low;
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    low = pg_int_reg->INTMSK;
    msk_lsb = (~tg_int_global.property_bitmap[0]) & cn_int_msk_all_line;
    pg_int_reg->INTMSK |= msk_lsb;
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
	return low;
}
                    
//----�뿪�ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ�����ǰ�첽�ź����״̬��������Ӧ��int_low_atom_start�������ס�
//      int_low_atom_start---int_low_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
//      Ƕ�׵ĵط�������ʹ��int_save_asyn_signal���ر��ж�(����)��
//      ��������������"int_restore_asyn_signal--int_save_asyn_signal"�Ŀ��ٰ汾��
//      ��������Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ���������
//      int_restore_asyn_signal��int_save_asyn_signal�е��κ�һ����
//������low��int_low_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void int_low_atom_end(atom_low_t low)
{
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    pg_int_reg->INTMSK &= ~((~tg_int_global.property_bitmap[0]) & (~low));
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
}

//----��ͨ�첽�źſ���---------------------------------------------------------
//���ܣ���ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�ѷ����������ж�����rINTMSK��Ӧλ��0.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void int_contact_asyn_signal(void)
{
    //INTMSK���첽�ź����ж��߱�ʹ�ܵ�λ����0
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    pg_int_reg->INTMSK &= ~((~tg_int_global.property_bitmap[0])
                         & tg_int_global.enable_bitmap[0]);
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//���ܣ��Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�������첽�ź�����rINTMSK��Ӧλ��1.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void int_cut_asyn_signal(void)
{
    register ucpu_t msk;
#if(cfg_asyn_reg_atom == 0)
    atom_high_t high;
    high = int_high_atom_start();
#endif      //for #if(cfg_asyn_reg_atom == 0)
    msk = (~tg_int_global.property_bitmap[0]) & cn_int_msk_all_line;
    pg_int_reg->INTMSK |= msk;
#if(cfg_asyn_reg_atom == 0)
    int_high_atom_end(high);
#endif      //for #if(cfg_asyn_reg_atom == 0)
}

//----��ͨ���жϿ���-----------------------------------------------------------
//���ܣ���ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void int_contact_trunk(void)
{
    __int_enable_irq();
}

//----�Ͽ����жϿ���---------------------------------------------------------
//���ܣ��Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void int_cut_trunk(void)
{
    __int_disable_irq();
}

//----��ͨ�����ж��߿���-------------------------------------------------------
//���ܣ���ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t int_contact_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
#if cfg_asyn_turnkey == 0
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
    {//������ж��������첽�ź�,���첽�źſ�������,������ж���
        if(tg_int_global.en_asyn_signal_counter == 0)
            pg_int_reg->INTMSK &= ~(1<<ufl_line);
    }else  //������ж�����ʵʱ�ж�,�ж����ж�������,ֱ��������ж���
        pg_int_reg->INTMSK &= ~(1<<ufl_line);
#else
        pg_int_reg->INTMSK &= ~(1<<ufl_line);
#endif
    return  true;
}

//----�Ͽ������ж��߿���-------------------------------------------------------
//���ܣ��Ͽ������ж��߿��أ��������жϺ��첽�źſ���״̬��Σ����ж��߱���ֹ
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t int_cut_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    pg_int_reg->INTMSK |=1<<ufl_line;
    return true;
}

//----Ӧ���жϣ������Ӧ�ж��ߵ��жϹ���״̬-----------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ��������ǰ��������Ӧͬһ�ж���
//      �ĺ����жϣ�����󣬲ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//      �����첽�ź��޹�
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t int_echo_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    pg_int_reg->SRCPND = 1<<ufl_line;
    pg_int_reg->INTPND = 1<<ufl_line;
    return true;
}

//----�����ж�-----------------------------------------------------------------
//����: ����һ���ж�.����жϱ�������,��������Ӱ��.����������ж��߱�����Ϊʵʱ
//      �жϻ����첽�ź��޹�
//������ufast ufl_line�����������ж��ߺ�
//���أ������Ӧ���ж���Ӳ�����ṩ����������жϹ���,���� false,���򷵻� true
//��ע: ������ʵ��������Ӳ��,��ЩӲ��ϵͳ��֧�ִ˹���.
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t int_tap_line(ufast_t ufl_line)
{
    return false;    //2440��֧���������Ӳ���жϵ�����.
}

//----Ӧ��ȫ���жϣ����ȫ���ж��ߵ��жϹ���״̬-------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ���
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
void __int_echo_all_line(void)
{
    pg_int_reg->SRCPND = cn_int_msk_all_line;
    pg_int_reg->INTPND = cn_int_msk_all_line;;
}

//----��ѯ�ж�������״̬-------------------------------------------------------
//���ܣ���ѯ�������Ӧ�ж���״̬�������ڲ�ѯʽ�жϳ���
//������ufl_line������ѯ���ж���
//���أ����жϹ��𣬷���true�����򷵻�false
//��ע: ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
//      ����������ֲ�ؼ�����
//-----------------------------------------------------------------------------
bool_t int_query_line(ufast_t ufl_line)
{
    ucpu_t  ucl_msk;
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    ucl_msk=1<<ufl_line;
    if(pg_int_reg->SRCPND & ucl_msk)
    {
        pg_int_reg->SRCPND = ucl_msk;
        pg_int_reg->INTPND = ucl_msk;
        return true;
    }else
        return false;
}

//----��ָ���ж�������Ϊ�첽�ź�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊ�첽�ź�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t int_setto_asyn_signal(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    tg_int_table[tg_int_lookup_table[ufl_line]].int_type = cn_asyn_signal;   //�ж�������

    tg_int_global.property_bitmap[ufl_line/cn_cpu_bits]
            		&= ~(1<<(ufl_line % cn_cpu_bits));   //����λͼ
    return true;
}

//----��ָ���ж�������Ϊʵʱ�ж�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊʵʱ�ж�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t int_setto_real(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].sync_event != NULL)
        return false;     //���߳��ڵȴ�����жϣ�������Ϊʵʱ�ж�
    tg_int_table[tg_int_lookup_table[ufl_line]].int_type = cn_real;    //�ж�������
    tg_int_table[tg_int_lookup_table[ufl_line]].enable_nest = false;   //��ʵ�ֲ�֧��ʵʱ�ж�Ƕ��
    tg_int_global.property_bitmap[ufl_line/cn_cpu_bits]
            |= 1<<(ufl_line % cn_cpu_bits);   //����λͼ
    return true;
}

//----ʹ���ж�Ƕ��-------------------------------------------------------------
//����: ʹ��һ���ж�Դ����ռ��ʹ�ܺ���Ӧ���жϷ����ڼ䣬���ܻᱻ����ж���ռ��
//      ����������ʵʱ�жϻ����첽�źţ�������ʵ���йأ���ֲ��Ӧ�ø���Ӳ������
//      ��Ӧ������ȷ���Ƿ�����Ƕ�ס�
//      Ƕ�׷�����ʵʱ�ж�֮������첽�ź�֮�䡣ʵʱ�ж���Զ���Դ���첽�źţ���
//      ����Ƕ�ס�
//      ��������ϵ�е�ARMоƬ�жϹ��������ص㣬����ʵʱ�жϵĴ��������ַ�ʽ:
//      1����fiqʵ��ʵʱ�жϣ�INTOFFSET1��INTOFFSET2�Ĵ����ṩ�˵�ǰ���ڷ����
//         �жϺţ����������Ĵ�����fiq����Ч�����ʵʱ�ж���fiqʵ�֣�����Ҫ��λ
//         ��鱻��Ϊʵʱ�жϵ��жϺ���INTPND1��INTPND1�Ĵ����е�ֵ������ȷ����
//         ������жϺţ�������̿��ܷǳ���������������ܹ�ʵ��ʵʱ�ж�Ƕ�ף���
//         ȴ��ȫʧȥ��ʵʱ�жϵ���Ƴ��ԣ�djyos for s3c2416����ȡ���ַ�ʽ��
//      2����һ����������ǣ�ϵͳֻ��Ҫһ��ʵʱ�жϣ�����ȫ����Ϊ�첽�źţ�����
//         �������fiqʵ��ʵʱ�ж�����Ϊ����ģ���Ȼֻ��һ���жϣ�Ƕ��Ҳ���޴�
//         ̸���ˡ�
//      3��ʵʱ�жϺ��첽�źŶ�����irq��ʽʵ�֣�cpsr��Iλ����trunk���أ��첽�ź�
//         û�ж������أ���ͨ�������е������첽�źŵ��жϺŵĿ���ȫ���ص���ģ��
//         �첽�źſ��صġ���������ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�Ϊʲô��?arm�����
//         Ϊ��ֻҪIλΪ0���µ��жϾͿ�����ռ���ڷ�����жϣ�Ҳ����ζ�ţ��첽��
//         ����ռʵʱ�жϣ���Ӳ����������ġ�ʵʱ�ж�Ҫʵ��Ƕ�ף����ȹص��첽��
//         �ţ�������I=0�����ʵʱ�ж���Ӧ�����첽�źű��ص�֮ǰ�����������첽
//         �źŷ����Ļ�����irq����ͻᷢ�������ص��첽�ź�Ҳû�ã����ж�һ����
//         �������첽�źžͻ���ռʵʱ�жϡ�������ռ��������"����"���ң�������
//         ���������⣬����ʹϵͳ�����������ռʵʱ�жϵ��첽�źţ������ڼ䷢��
//         ���߳��л�����fiq�����ջ������pg_event_running��ջ����ˣ�djyos for
//         2416�汾��֧��ʵʱ�ж�Ƕ�ס�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t int_enable_nest(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_global.property_bitmap[ufl_line/cn_cpu_bits]
                & (1<<(ufl_line % cn_cpu_bits)))
        return false;       //��ʵ�ֲ�֧��ʵʱ�ж�Ƕ��
    else
        tg_int_table[tg_int_lookup_table[ufl_line]].enable_nest = true;
    return true;
}

//----��ֹ�ж�Ƕ��-------------------------------------------------------------
//����: ���ĳ�жϺŵ��ж�Ƕ��ʹ�ܡ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t int_disable_nest(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    tg_int_table[tg_int_lookup_table[ufl_line]].enable_nest = false;
	return true;
}

//----�趨Ƕ�����ȼ�-----------------------------------------------------------
//����: �趨ָ���ж��ߵ�Ƕ�����ȼ�����������������Ӳ�����ܡ����Ӳ����֧�֣���
//      ���ֿպ�����
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ���
//ע: ��������ֲ����
//-----------------------------------------------------------------------------
bool_t int_set_prio(ufast_t ufl_line,u32 prio)
{
    return false;
}

//----��ʼ���ж�Ӳ����ز���---------------------------------------------------
//����: �����
//����: ��
//����: ��
//ע: ��ֲ���У���Ӳ����أ�Ҳ����������й�
//-----------------------------------------------------------------------------
void __int_init_hard(void)
{
    int_disable_arm_int();     //��ֹ�ж�

    pg_int_reg->INTMSK = cn_int_msk_all_line;
    //�жϹ�������Fλ���ǽ�ֹ,2440���жϹ�������ȱ�ݣ�����ֻ����һ
    //���ж�����Ϊfiq������fiq����ж��ߵĹ���ʹ��fiqʵ�ʱ�irq�����ʲ�ʹ��fiq��
    //�����첽�źŻ���ʵʩ�жϣ���ʹ��irqʵ�֡�
    pg_int_reg->INTMOD=0;    //���������ж���ΪIRQ�ж�
    engine_irq = __int_engine_all;
    // irq_bottom = __int_engine_all;
}
//----��ʼ���ж�---------------------------------------------------------------
//���ܣ���ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//      2.�첽�źű��ֽ�ֹ,��������������������д�.
//      3.���ж�����
//      �û���ʼ������Ӧ���������¹���:
//      1.ϵͳ��ʼʱ���Ѿ���ֹ�����첽�ź�,�û���ʼ��ʱ���뵣���첽�źŷ���.
//      2.��ʼ�������������Ҫ�������ж�/ʵʱ�ж�/�첽�ź�,Ӧ�óɶ�ʹ��.��ֹʹ
//        �첽�ź�ʵ�ʴ�������״̬(���첽�����жϿ���ͬʱ����).
//      3.���Բ����ж���,�������ӡ�������ֹ��,��Ӧ�óɶ�ʹ��.
//      4.����ʹ��save/restore������,��Ҫʹ��enable/disable������.
//��������
//���أ���
//-----------------------------------------------------------------------------
void int_init(void)
{
    ufast_t ufl_line;
    __int_init_hard();
    __int_echo_all_line();
    for(ufl_line=0;ufl_line <= cn_int_line_last;ufl_line++)
    {
        tg_int_lookup_table[ufl_line] = cn_limit_ufast;
    }
    for(ufl_line = 0; ufl_line < ufg_int_used_num; ufl_line++)
    {
        tg_int_lookup_table[tg_int_used[ufl_line]] = ufl_line;
        tg_int_table[ufl_line].en_counter = 1;               //��ֹ�ж�,����Ϊ1
        tg_int_table[ufl_line].int_type = cn_asyn_signal;        //��Ϊ�첽�ź�
        //�����жϺ���ָ��ָ��պ���
        tg_int_table[ufl_line].ISR = NULL_func;
        tg_int_table[ufl_line].sync_event = NULL;                //ͬ���¼���
        tg_int_table[ufl_line].my_evtt_id = cn_invalid_evtt_id;  //�������¼�
    }

    for(ufl_line=0; ufl_line < cn_int_bits_words; ufl_line++)
    {
        //����λͼ����,ȫ����Ϊ�첽�źŷ�ʽ
        tg_int_global.property_bitmap[ufl_line] = 0;
        //�ж�ʹ��λͼ��0,ȫ�����ڽ�ֹ״̬
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }
    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.nest_real = 0;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;   //�첽�źż���
    int_cut_asyn_signal();
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;   //���жϼ���
    int_contact_trunk();                //��ͨ���жϿ���								  
}

//----���ж�����---------------------------------------------------------------
//���ܣ���Щϵͳ�����ж�����������������ʵʱ�жϻ����첽�źŵģ����߲�ϣ���ڻ�
//      ��׶�ʹ�ù��ڸ��ӵĴ���ģ�����2416��2440�ȣ����ڻ��׶λ�ȡ�жϺź�
//      ֱ�ӵ��ñ��������ɱ���������������첽�ź������ʵʱ�ж����档
//      ��cortex-m3��omapl138���������ж��������־Ϳ�������ʵʱ�жϻ����첽��
//      �ţ��������ṩ�������������ڻ�ಿ��ֱ�ӵ�����Ӧ�����档
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------

void __int_engine_all(ufast_t ufl_line)
{
    if(tg_int_table[ufl_line].int_type == cn_real)
        __int_engine_real(ufl_line);                //��ʵʱ�ж�
    else
        __int_engine_asyn_signal(ufl_line);         //���첽�ź�
}

//----ʵʱ�ж�����-------------------------------------------------------------
//���ܣ���Ӧʵʱ�жϣ������жϺŵ����û�ISR
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __int_engine_real(ufast_t ufl_line)
{
//    tg_int_global.nest_real++;
//    tg_int_global.en_trunk = false;
//    tg_int_global.en_trunk_counter++;

    //��if�������ֲ���У���cpu���жϹ������ļ��������й�:
    //1���첽�ź��Ƿ��ж����Ŀ��أ�����cortex-m3��omapl138�����еģ�2440��2416��
    //   2410����û�еġ����û�ж������أ����ڴ����ж�ǰ��ϵ��첽�ź��߿���
    //2���첽�źź�ʵʱ�ж϶����ڿ���״̬������£��첽�ź��Ƿ������ռʵʱ�жϡ�
	//   �������ʵ�����ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
	//3��ʵʱ�ж���Ӧ���Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ��֧��ʵʱ�ж�Ƕ�ף�������ע�͵�
//    if(tg_int_table[ufl_line].enable_nest == true)
//    {
//        real_save_asyn = int_low_atom_start();
//        int_contact_trunk();
//    }
    int_echo_line(ufl_line);        //�ж�Ӧ��
    if(tg_int_table[ufl_line].ISR != NULL)
        tg_int_table[ufl_line].ISR(ufl_line);  //�����û��жϺ���


    //�����ʵ��֧��ʵʱ�ж�Ƕ�ף���������4��
//    tg_int_global.nest_real--;
//    tg_int_global.en_trunk_counter--;
//    if(tg_int_global.en_trunk_counter == 0)
//        tg_int_global.en_trunk = true;

}

//----�첽�¼��ж�����---------------------------------------------------------
//���ܣ���Ӧ�첽�źţ������жϺŵ����û�ISR����󵯳��ж��߿��ƿ��my_evtt_id
//      ��Աָ�����¼����ͣ�����ڷ���ǰ�鿴�Ƿ���Ҫ���������л�������Ҫ���л�
//      ֮��
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __int_engine_asyn_signal(ufast_t ufl_line)
{
    struct event_ECB *event;
    u32 isr_result;
    tg_int_global.en_asyn_signal_counter = 1;
    bg_schedule_enable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.nest_asyn_signal++;
    int_disable_asyn_line(ufl_line);	//����ֹ�����ܻ��γ�����ѭ��
    if(tg_int_table[ufl_line].enable_nest == true)  //����Ƕ�׵����
        int_contact_asyn_signal();
    else
	    int_cut_asyn_signal();
    int_echo_line(ufl_line);        //�ж�Ӧ��,
    int_contact_trunk();
    if(tg_int_table[ufl_line].ISR != NULL)
        isr_result = tg_int_table[ufl_line].ISR(ufl_line);
    if(tg_int_table[ufl_line].enable_nest == true)  //֧��Ƕ�׵����
        int_cut_asyn_signal();

    event = tg_int_table[ufl_line].sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
    {
        event->last_status.all = event->event_status.all;
        event->event_status.bit.wait_asyn_signal = 0;
        event->event_result = isr_result;
        __djy_event_ready(event);   //�Ѹ��¼��ŵ�ready����
        tg_int_table[ufl_line].sync_event = NULL;   //���ͬ��
    }
    if(tg_int_table[ufl_line].my_evtt_id != cn_invalid_evtt_id)
        djy_event_pop(tg_int_table[ufl_line].my_evtt_id,enum_para_async,NULL,0,
                                    &ufl_line,sizeof(ufl_line),0,0);
    tg_int_global.nest_asyn_signal--;
    if(tg_int_global.nest_asyn_signal == 0)
    {
//        tg_int_global.en_asyn_signal = true;
        tg_int_global.en_asyn_signal_counter = 0;
        if(pg_event_ready != pg_event_running)
            __djy_schedule_asyn_signal();       //ִ���ж��ڵ���
        bg_schedule_enable = true;
    }
    int_cut_trunk();
    int_enable_asyn_line(ufl_line);
    int_contact_asyn_signal();  //ǰ��ر��ˣ����ڴ�
}

