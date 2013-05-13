//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ж�ģ��
//����:  ������.
//�汾��V2.0.1
//�ļ�����: s3c2410���ж���صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//3. ����: 2010-01-12
//   ����:  ������.
//   �°汾��: V2.0.1
//   �޸�˵��: 
//   1���鵽��һ���ǳ����ε����⣬��__int_engine_real�����ڣ�������
//      if(tg_int_global.en_trunk_counter == 0)
//          tg_int_global.en_trunk = true;      //ԭ��д��false��
//   2����int_save_trunk,int_restore_trunk,int_save_asyn_signal,
//      int_restore_asyn_signal�������������Ӷ�bg_schedule_enable�Ĳ���
//2. ����: 2010-01-04
//   ����:  ������.
//   �°汾��: V2.0.0
//   �޸�˵��: ��ԭ�汾��ȥ��Ӳ����ز��ֶ��ɣ����λ��Ҳ��arch�ĵ�djyos
//1. ����: 2009-12-10
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ��44b0�汾��ֲ����
//------------------------------------------------------

//ʵʱ�жϵ�����:
// 1������������κ�ϵͳ���á�
// 2��������ı��κ��ж��ߵ��첽�źŻ���ʵʱ�ж����ԡ�
// 3��������ʹ�ܻ��ֹ�첽�źţ������ź��ߺ����첽�źſ��ء�
// 4������ʹ�ܻ��ֹ����Ϊʵʱ�жϵ��ж���

#include "os_inc.h"

ufast_t tg_int_lookup_table[cn_int_line_last+1];
struct int_master_ctrl  tg_int_global;

//----�������ж�״̬����ֹ���ж�--------------------------------------------
//���ܣ���������int_restore_trunk()����ú��������ñ�����ʹ��ֹ������1������һ
//      ��int_restore_trunkʹ��ֹ������1��
//      ����ǰ����Ϊ0������Ϊ1����ֹ���жϣ���Ϊ0ʱ�򵥵���1
//��������
//���أ���
//------------------------------------------------------------------------------
void int_save_trunk(void)
{
    if(tg_int_global.nest_real != 0)
        return;
    int_cut_trunk();
    if(tg_int_global.en_trunk_counter != cn_limit_ucpu)//�����޺��ټӻ���Ƶ�0
        tg_int_global.en_trunk_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_trunk_counter != 0��״̬�������
    //ʹ�жϹرգ���ʹ�û��������õ�int_save_trunk�𲻵�����
    bg_schedule_enable = false;
    return;
}

//----�ָ���������ж�״̬---------------------------------------------------
//���ܣ���������int_save_trunk()����ú��������ñ�����ʹ��ֹ������1������
//      һ��int_save_trunkʹ��ֹ������1��
//      ����������0ʱ�������ж�,����򵥼�1
//��������
//���أ���
//------------------------------------------------------------------------------
void int_restore_trunk(void)
{
    if(tg_int_global.nest_real != 0)
        return;
    if(tg_int_global.en_trunk_counter != 0)
        tg_int_global.en_trunk_counter--;
    if(tg_int_global.en_trunk_counter == 0)
    {
        int_contact_trunk();
        if(tg_int_global.en_asyn_signal_counter == 0)
        {
            bg_schedule_enable = true;
            if(pg_event_running != pg_event_ready)
                __djy_schedule();
        }
    }else
    {
        int_cut_trunk();
    }
    return;
}

//----�鿴���ж��Ƿ�����-----------------------------------------------------
//���ܣ�
//��������
//���أ�������true,��ֹ����false
//-----------------------------------------------------------------------------
bool_t int_check_trunk(void)
{
    if(tg_int_global.en_trunk_counter == 0)
        return true;
    else
        return false;
}

//----���浱ǰ״̬����ֹ�첽�ź�------------------------------------------------
//���ܣ���������int_restore_asyn_signal()����ú��������ñ�����ʹ��ֹ�������ӣ�
//      ����һ��int_restore_asyn_signal()ʹ��ֹ�������١�
//      ����ǰ����Ϊ0������Ϊ1����ֹ�첽�źţ���Ϊ0ʱ�򵥵���1
//��������
//���أ���
//------------------------------------------------------------------------------
void int_save_asyn_signal(void)
{
    if(tg_int_global.nest_asyn_signal != 0)
        return;
    //�����޺��ټӻ���Ƶ�0
    int_cut_asyn_signal();
    if(tg_int_global.en_asyn_signal_counter != cn_limit_ucpu)
        tg_int_global.en_asyn_signal_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_asyn_signal_counter != 0��״̬��
    //�����ʹ�жϹرգ���ʹ�û��������õ�en_asyn_signal_counter�𲻵�����
    bg_schedule_enable = false;
    return;
}

//----��λ�첽�źſ���------------------------------------------------
//���ܣ����첽�źſ��ػָ�����ʼ״̬����en_asyn_signal_counter=1��״̬����ʼ��
//      �ж�ϵͳ�󣬻�Ҫ��������ģ���ʼ�����������������¼����ȣ����������¼�
//      ����ǰ���ñ�������λ�첽�ź�״̬�����Է�ֹģ���ʼ�������bug�����޸�
//      ���첽�ź�ʹ��״̬��
//��������
//���أ���
//-----------------------------------------------------------------------------
void __int_reset_asyn_signal(void)
{
    int_cut_asyn_signal();
    tg_int_global.en_asyn_signal_counter =1;
    return;
}

//----�ָ�������첽�ź�״̬----------------------------------------------------
//���ܣ���������int_save_asyn_signal()����ú��������ñ�����ʹ��ֹ�������٣�����
//      һ��int_save_asyn_signal()�ǽ�ֹ�������ӡ�
//      ����������0ʱ�����첽�ź�,����򵥼�1
//��������
//���أ���
//------------------------------------------------------------------------------
void int_restore_asyn_signal(void)
{
    if(tg_int_global.nest_asyn_signal != 0)
        return;
    if(tg_int_global.en_asyn_signal_counter != 0)
        tg_int_global.en_asyn_signal_counter--;
    if(tg_int_global.en_asyn_signal_counter==0)
    {
//        tg_int_global.en_asyn_signal = true;   //�첽�ź���Ϊʹ��
        if(tg_int_global.en_trunk_counter == 0) 
        {
            bg_schedule_enable = true;
            if(pg_event_running != pg_event_ready)
                __djy_schedule();
            else
                int_contact_asyn_signal();
        }else
            int_contact_asyn_signal();
    }else
    {
        int_cut_asyn_signal();    //��ֹcounter>0�ڼ�����(bug)��
    }
    return;
}

//----���첽�ź�ǰ�벿---------------------------------------------------------
//����: ����һ��ר��Ϊ�������л�׼���ĺ���/
//      �����ݽṹ���첽�ź�����Ϊ����״̬������������һ��:�����첽�źſ��ء�
//      �����������л������У��������ú�״̬��ʹ���������л������һ�������´�
//      �ж�ͬʱ��ɡ�������̱�ȻҪ���жϣ���Щϵͳû���ܹ������pcͬʱ��ɵġ�
//      �������첽�źſ��أ���������Ʊػ��ʵʱ�ж�Ҳ�ص�����˰��������ݽṹ��
//      ������������Լ��ٹر�ʵʱ�жϵ�ʱ�䡣��ʹ�ò���cpu����ʵ��"ʵʱ�ж�����
//      �ر�"��Ҫ�󣬵��ǹر�ʱ���Ƿǳ��̵ġ�
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void int_half_enable_asyn_signal(void)
{
    tg_int_global.en_asyn_signal_counter = 0;
}

//----�鿴�첽�ź��Ƿ�����-----------------------------------------------------
//���ܣ�
//��������
//���أ�������true,��ֹ����false
//-----------------------------------------------------------------------------
bool_t int_check_asyn_signal(void)
{
    if( tg_int_global.en_asyn_signal_counter == 0)
        return true;
    else
        return false;
}

//----���浱ǰ״̬����ֹ�첽�ź��ж���-----------------------------------------
//���ܣ���������int_restore_asyn_line()����ú��������ñ�����ʹ��ֹ�������ӣ���
//      ��һ��int_restore_asyn_line�ǽ�ֹ�������١�
//      ����ǰ����Ϊ0������Ϊ1����ֹ�ж��ߣ���Ϊ0ʱ�򵥵���1
//������ufl_line
//���أ���
//-----------------------------------------------------------------------------
bool_t int_save_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    int_cut_line(ufl_line);
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter!=cn_limit_ucpu)//�����޺��ټӻ���Ƶ�0
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_counter != 0��״̬��
    //�����ʹ�жϹرգ���ʹ�û��������õ�en_counter�𲻵�����
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----���浱ǰ״̬����ֹ�첽�ź��ж���-----------------------------------------
//���ܣ���������int_restore_real_line()����ú��������ñ�����ʹ��ֹ�������ӣ���
//      ��һ��int_restore_real_line�ǽ�ֹ�������١�
//      ����ǰ����Ϊ0������Ϊ1����ֹ�ж��ߣ���Ϊ0ʱ�򵥵���1
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t int_save_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;
    int_cut_line(ufl_line);
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter!=cn_limit_ucpu)//�����޺��ټӻ���Ƶ�0
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter++;
    //ԭ�㷨�Ǵ�0->1�Ĺ����вŽ��룬�������en_counter != 0��״̬��
    //�����ʹ�жϹرգ���ʹ�û��������õ�en_counter�𲻵�����
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----�ָ�������첽�ź��ж���״̬---------------------------------------------
//���ܣ���������int_save_line��������ú��������ñ�����ʹ��ֹ�������٣���
//      ��һ��int_save_line�ǽ�ֹ�������ӡ�
//      ����������0ʱ�����ж���,����򵥼�1
//������ufl_line
//���أ���
//ע: ��������������ʵʱ�ж�ISR�е��ã�������ֱ�ӷ���false��
//-----------------------------------------------------------------------------
bool_t int_restore_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter != 0)
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter--;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter==0)
    {
        tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
        int_contact_line(ufl_line);
    }
    return true;
}

//----�ָ������ʵʱ�ж���״̬-------------------------------------------------
//���ܣ���������int_save_line��������ú��������ñ�����ʹ��ֹ�������٣���
//      ��һ��int_save_line�ǽ�ֹ�������ӡ�
//      ����������0ʱ�����ж���,����򵥼�1
//������ufl_line
//���أ���
//ע: ��int_restore_asyn_line��һ�������������첽�źź�ʵʱ�ж�ISR�ж��������
//-----------------------------------------------------------------------------
bool_t int_restore_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter != 0)
        tg_int_table[tg_int_lookup_table[ufl_line]].en_counter--;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter==0)
    {
        tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
        int_contact_line(ufl_line);
    }
    return true;
}

//----ֱ�ӽ�ֹ�첽�ź��ж���---------------------------------------------------
//���ܣ���������int_enable_asyn_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ�����
//      ��λ���������ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t int_disable_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    int_cut_line(ufl_line);
    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 1;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----ֱ�ӽ�ֹʵʱ�ж���-------------------------------------------------------
//���ܣ���������int_enable_real_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ�����
//      ��λ���������ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t int_disable_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;
    int_cut_line(ufl_line);
    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 1;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                &= ~(1<<(ufl_line % cn_cpu_bits));
    return true;
}

//----ֱ�������첽�ź��ж���---------------------------------------------------
//���ܣ���������int_disable_asyn_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ���
//      �����㣬����ͨ�ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t int_enable_asyn_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast)
            || (tg_int_global.nest_real != 0) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
        return false;
    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 0;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
    int_contact_line(ufl_line);
    return true;
}

//----ֱ������ʵʱ�ж���-------------------------------------------------------
//���ܣ���������int_disable_real_line()����ú��������ñ�����ʹ�ж��ߵ�ʹ�ܼ���
//      �����㣬����ͨ�ж���
//������ufl_line
//���أ���
//------------------------------------------------------------------------------
bool_t int_enable_real_line(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if(tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_asyn_signal)
        return false;

    tg_int_table[tg_int_lookup_table[ufl_line]].en_counter = 0;
    tg_int_global.enable_bitmap[ufl_line/cn_cpu_bits]
                |= 1<<(ufl_line % cn_cpu_bits);
    int_contact_line(ufl_line);
    return true;
}

//----��ѯ�ж���ʹ��״̬-------------------------------------------------------
//���ܣ���ѯ�ж����Ƿ�����
//������ufl_line������ѯ���ж���
//���أ�true = ʹ�ܣ�false = ��ֹ��
//-----------------------------------------------------------------------------
bool_t int_check_line(ufast_t ufl_line)
{
    if(tg_int_table[tg_int_lookup_table[ufl_line]].en_counter == 0)
        return true;
    else
        return false;
}

//----�����ж�����ISR----------------------------------------------------------
//���ܣ�Ϊָ���ж���ָ���ж���Ӧ�������ú���Ϊ��ͨ������
//������ufl_line,��Ҫ���õ��ж��ߺ�
//      isr���ж���Ӧ���������û��ṩ��ԭ�ͣ�void isr(ufast_t)
//���أ���
//-----------------------------------------------------------------------------
void int_isr_connect(ufast_t ufl_line, u32 (*isr)(ufast_t))
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].ISR = isr;
    return;
}

//----�����ж������¼�����-----------------------------------------------------
//���ܣ�Ϊָ�����ж���ָ��һ���¼�����id��������첽�źţ����ڷ���������ǰ����
//      �¼�����Ϊ��id���¼��������ʵʱ�жϣ��򲻵����¼���
//������ufl_line,��Ҫ���õ��ж��ߺ�
//      my_evtt_id���¼�����id
//���أ���
//-----------------------------------------------------------------------------
void int_evtt_connect(ufast_t ufl_line,uint16_t my_evtt_id)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].my_evtt_id = my_evtt_id;
    return;
}

//----�Ͽ��ж������ж���Ӧ�����Ĺ���-------------------------------------------
//���ܣ��Ͽ�ָ���ж���ָ���ж���Ӧ�����Ĺ�������֮�Կպ���
//������ufl_line,��Ҫ���õ��ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void int_isr_disconnect(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].ISR = NULL;
    return;
}

//----�Ͽ��ж������¼����͵Ĺ���-----------------------------------------------
//���ܣ��Ͽ�ָ���ж���ָ���¼����͵Ĺ�������֮��cn_invalid_evtt_id
//������ufl_line,��Ҫ���õ��ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void int_evtt_disconnect(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return;
    tg_int_table[tg_int_lookup_table[ufl_line]].my_evtt_id = cn_invalid_evtt_id;
    return;
}

//----�趨�ж�ͬ��-------------------------------------------------------------
//����: �������ڴ�����¼����̣߳�ֱ��ָ�����ж��ߵ��жϷ�������Ӧ�����أ�Ȼ���
//      �����̡߳������ж���ԭ��״̬��Σ����ñ������������ж��߱�ʹ��(��ֱ��ʹ
//      �ܣ����ǵ���int_save_asyn_line)�����ڷ��غ�ָ���ֹ״̬��
//����: ufl_line,�ȴ���Ŀ���ж���
//����: false = ���ж��Ѿ��������̵߳ȴ���ֱ�ӷ��ء�
//      true = �ɹ�ͬ�������ڸ��жϷ����󷵻ء�
//��ע: 1.�ж���һ���ٽ���Դ���������жϺ�����̫������飬���ж�ͬ���Ĺ��ܱȽϼ�
//      ����ÿ���ж���ͬһʱ��ֻ����һ���̵߳ȴ���Ҳ���������ó�ʱ�ȴ�
//      2.����djyosһ�����ж�ͬ������ֻ�ܰ��Լ�����ȴ�״̬�������ܿ��Ʊ��
//      �̣߳��ʺ���ԭ�Ͳ����� bool_t int_sync(ufast_t line,uint16_t event_id)
//      3.ʵʱ�ж����õȴ���Ч�����ñ�����ʱ�����line�Ѿ�������Ϊʵʱ�жϣ���
//      ֱ�ӷ���false��������ñ�������line���ܱ�����Ϊʵʱ�жϡ�
//      4.Ϊ��ȷʵ�ֹ��ܣ����ڵ���ǰȷ�����ж����Ǳ���ֹ�ġ�
//-----------------------------------------------------------------------------
bool_t int_asyn_signal_sync(ufast_t ufl_line)
{
    if( (ufl_line > cn_int_line_last) 
            || (tg_int_lookup_table[ufl_line] == cn_limit_ufast) )
        return false;
    if( !djy_query_sch())
    {   //��ֹ���ȣ����ܽ����첽�ź�ͬ��״̬��
        djy_error_login(enum_knl_cant_sched,NULL);
        return false;
    }
    int_save_asyn_signal();   //�ڲ������������ڼ䲻�ܷ����ж�
    //ʵʱ�жϲ�������ͬ����һ���ж�ֻ����һ��ͬ���¼�
    if((tg_int_table[tg_int_lookup_table[ufl_line]].int_type == cn_real)
            || (tg_int_table[tg_int_lookup_table[ufl_line]].sync_event != NULL))
    {
        int_restore_asyn_signal();
        return false; //ʵʱ�жϻ��Ѿ���ͬ���¼�
    }else
    {
        if(int_query_line(ufl_line) == true)    //�ж��Ѿ�������ͬ�������ﵽ
        {
            int_echo_line(ufl_line);
            int_restore_asyn_signal();
            return true;
        }
        //�������дӾ���������ȡ��running�¼�
        __djy_cut_ready_event(pg_event_running);
        pg_event_running->next = NULL;
        pg_event_running->previous = NULL;
        pg_event_running->last_status.all
                        = pg_event_running->event_status.all;
        pg_event_running->event_status.bit.wait_asyn_signal = 1;
        tg_int_table[tg_int_lookup_table[ufl_line]].sync_event = pg_event_running;
    }
    int_enable_asyn_line(ufl_line);
    int_restore_asyn_signal();      //���ñ������������߳��л������ڴ�����¼���
                                    //����
    int_disable_asyn_line(ufl_line);
    return true;
}
//��ע: ���ṩ�������ж�ͬ�����ܣ���Ϊdjyos���ṩ���������߻��߹���Ĺ��ܣ�����
//      ����ʱ���ж�Ϊ����һ��ʱ���жϰ��̴߳�������ready�󣬵��´�ʱ���жϵ���
//      ֮ǰ�����߳�Ҫô����ready̬(���ܱ������߳���ռ)����ʱ����Ҫ�ٴδ�����
//      Ҫô�ڵȴ�������������������ȴ��ڴ���䡢�ȴ���ʱ�����ȴ��ź����ȣ���ʱ
//      ����ʱ���жϴ��������ƻ�������Ľṹ���������ж�ͬ�����ö�ε��õ���ͬ��
//      �ķ�����ɣ���ÿ�δ������߳�����˱�Ҫ�Ĺ����Ժ������ٴε��õ���ͬ��
//      �����������������ÿһ��������ȷ��Ŀ�꣬��������Ŀ�ĵ����߻����


