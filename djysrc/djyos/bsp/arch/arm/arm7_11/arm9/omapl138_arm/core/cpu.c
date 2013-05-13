//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ������
//����:  ������.
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����:  ������.
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

extern uint32_t  u32g_os_ticks;             //����ϵͳ����ticks��
volatile uint32_t    u32g_delay_10uS = 40;
static u32 u32g_ns_of_u32for; //for(i=j;i>0;i--);��䣬i��j����u32�ͣ�ÿѭ��������
static u32 u32g_ns_of_u16for; //for(i=j;i>0;i--);��䣬i��j����u16�ͣ�ÿѭ��������
static u32 u32g_ns_of_u8for;  //for(i=j;i>0;i--);��䣬i��j����u8�ͣ�ÿѭ��������

//tickѡ��t64-0��t12

//----�����߳�-----------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ���α���������ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������̵߳������ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct  thread_vm *__create_thread(struct  event_type *evtt)
{
    struct  thread_vm  *result;
    ptu32_t  len;

    //���������ջ:�߳�+��󵥸�api�����ջ
    len = evtt->stack_size+cn_kernel_stack+sizeof(struct  thread_vm);
    //ջ����Ҫ���룬malloc�����ܱ�֤ջ���Ƕ���ģ����볤�ȿ���ʹջ������
    len = align_up_sys(len);
    result=(struct  thread_vm  *)m_malloc_gbl(len,0); //�����ڴ棬��ֲ����
    if(result==NULL)
    {
        djy_error_login(enum_mem_tried,NULL);   //�ڴ治�㣬���ش���
        return result;
    }

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
#if cfg_stack_fill != 0
    len = m_check_size(result);
    memset(result,cfg_stack_fill,len);
#endif
    result->stack_top = (uint32_t*)((ptu32_t)result+len); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = len - sizeof(struct thread_vm); //����ջ���
    result->host_vm = NULL;
    //��λ������������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----����ָ����ʱ����---------------------------------------------------------
//����: ����ָ����ʱ����,ʹy_delay_10us��������ʱ����=10uS�������ú��ֱ�������
//      �����Ż�ѡ��
//��������
//����: ��
//��ע: ��������ֲ����
//-----------------------------------------------------------------------------
void __djy_set_delay(void)
{
    uint32_t u32_fors;
    uint32_t u16_fors;
    uint32_t  u8_fors;
    volatile uint32_t u32loops;
    volatile uint16_t u16loops;
    volatile uint8_t  u8loops1,u8loops2;

    timer_stop(0,0);                    //ֹͣ��ʱ��0
    timer_clear(0,0);                   //�����ǰ����ֵ
    timer_set_clksrc(0,0,0);            //���þ���ʱ����Ϊ��ʱ��ʱ��
    timer_counter_len_set(0,1);         //����Ϊ˫32λ����
    timer_set_mode(0,0,2);              //��Ϊ�����Լ���
    timer_set_Period(0,0,0xffffffff);   //��ʱ����ֵ��Ϊ���
//    pg_timer0_reg->TCR |= (1<<10);      //����t64-0��t12Ϊ������ģʽ
    timer_start(0,0);
    for(u32loops=10000;u32loops>0;u32loops--); //��32λ����ִ��ѭ��
    u32_fors = timer_read(0,0);
    timer_stop(0,0);                    //ֹͣ��ʱ��0
    timer_clear(0,0);                   //�����ǰ����ֵ
    timer_start(0,0);
    for(u16loops=10000;u16loops>0;u16loops--); //��16λ����ִ��ѭ��
    u16_fors = timer_read(0,0);
    timer_stop(0,0);                    //ֹͣ��ʱ��0
    timer_clear(0,0);                   //�����ǰ����ֵ
    timer_start(0,0);
    for(u8loops1 = 100; u8loops1 > 0; u8loops1--)
        for(u8loops2=99;u8loops2>0;u8loops2--); //��8λ����ִ��ѭ��
    u8_fors = timer_read(0,0);
    timer_stop(0,0);                    //ֹͣ��ʱ��0

    u32g_ns_of_u32for = ((u64)cn_fine_us * u32_fors>>16) / 10;
    u32g_ns_of_u16for = ((u64)cn_fine_us * u16_fors>>16) / 10;
    u32g_ns_of_u8for  = ((u64)cn_fine_us * u8_fors>>16) / 10;

    u32g_delay_10uS = 10000/u32g_ns_of_u32for;
//    pg_timer0_reg->TCR &= ~((u32)1<<10);       //���t64-0��t12�Ķ�����ģʽ
}

uint32_t __djy_isr_tick(ufast_t line)
{
    djy_isr_tick();
    g_ptTimer0Reg->INTCTLSTAT |= 2;
    return 0;
}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __djy_init_tick(void)
{
    //�������ö�ʱ������������port_config.h��cn_tick_ms��cn_tick_hz��
    //cn_fine_us��cn_fine_hz�Ķ���һ��
    timer_stop(0,0);                    //ֹͣ��ʱ��0
    timer_clear(0,0);                   //�����ǰ����ֵ
    timer_set_clksrc(0,0,0);            //���þ���ʱ����Ϊ��ʱ��ʱ��
    timer_counter_len_set(0,1);         //����Ϊ˫32λ����
    timer_set_Period(0,0,cn_fine_hz/cn_tick_hz);   //t12����ֵ��Ϊtick����
    timer_int_ctrl(0,PRDINTEN12,true);  //ʹ�������ж�
    timer_int_ctrl(0,EVTINTEN12,false); //��ֹ�¼������ж�
    timer_set_mode(0,0,2);              //��Ϊ�����Լ���
    //����t64-0��t12��Ϊtick�ж�
    int_isr_connect(cn_int_line_t64p0_tint12,__djy_isr_tick);
    int_setto_asyn_signal(cn_int_line_t64p0_tint12);    //tick�жϱ���Ϊ�첽�ź�
    int_restore_asyn_line(cn_int_line_t64p0_tint12);    //����tick�ж�
    timer_start(0,0);
}

//----��ȡ��ǰticks-------------------------------------------------------------
//���ܣ���ȡ����ϵͳʱ��
//      u32g_os_ticks Ϊ32λ�޷��ű�����ARM��32λ��������ֱ�Ӷ�ȡ����32λϵͳ��
//      ��ȡ u32g_os_ticks ��Ҫ����1������,��˷���SysTimerʱ��Ҫ���жϡ�
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
uint32_t djy_get_time_tick(void)
{
    uint32_t time;
#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_save_asyn_line(cn_int_line_t64p0_tint12);
#endif

    time = u32g_os_ticks;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_restore_asyn_line(cn_int_line_t64p0_tint12);
#endif
    return time;
}

//----��ȡ��ǰticks-------------------------------------------------------------
//���ܣ���ȡ����ϵͳʱ��
//      u32g_os_ticks Ϊ32λ�޷��ű�����ARM��32λ��������ֱ�Ӷ�ȡ����32λϵͳ��
//      ��ȡ u32g_os_ticks ��Ҫ����1������,��˷���SysTimerʱ��Ҫ���жϡ�
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
uint32_t djy_get_time(void)
{
    uint32_t time;
#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_save_asyn_line(cn_int_line_t64p0_tint12);
#endif

    time = u32g_os_ticks*cn_tick_us;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_restore_asyn_line(cn_int_line_t64p0_tint12);
#endif
    return time;
}

//----��ȡ����ʱ��-----------------------------------------------------------
//���ܣ���ȡ����ʱ��,��λuS����4294.967296����Ƶ�0.
//��������
//���أ�ʱ��ֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
uint32_t djy_get_fine_time(void)
{
    u32 time1,time2;
    atom_low_t atom_low;
    atom_low = int_low_atom_start();
    
    time1 = g_ptTimer0Reg->TIM12;
    if(g_ptTimer0Reg->INTCTLSTAT & 0x02)
        time2 = (u32g_os_ticks+1)*cn_tick_us;
    else
        time2 = u32g_os_ticks*cn_tick_us + (time1*cn_fine_us >>16);
    
    int_low_atom_end(atom_low);
    return time2;
}

//----10΢�뼶��ʱ-------------------------------------------------------------
//���ܣ�����ѭ��ʵ�ֵ�΢��ֱ�����ʱ������__djy_set_delay���������ʹ�ñ�������
//      �����ڲ�ͬ�Ż�����Ͳ�ͬ��������,��ʱ����ͬ.
//������time����ʱʱ�䣬��λΪ10΢��,���655���롣
//���أ���
//ע�⣺������ʹ�ô˺�����̫����ʱ������ʱ��ʹ�ú��� djy_timer_sync,
//-----------------------------------------------------------------------------
void djy_delay_10us(volatile u16 time)
{
    volatile u32 i;
    for(; time > 0; time--)
        for(i = u32g_delay_10uS; i >0 ; i--);
}

//----ȡforѭ����ʱ����--------------------------------------------------------
//����: ������ʹ��for(i=j;i>0;i--);ѭ����ʵ�ֶ�ʱ����ʱʱ����ͨ����������ȡ����
//      ѭ����ִ��ʱ�䡣����ѭ������i�����ͣ�ʹ��:
//          i = djy_get_for_time(sizeof(i));
//      ����ʹ��i��ѭ��������forѭ����ִ��ʱ�䡣�����д���ʵ����ʱx����
//          i= x/i;
//          for( ; i > 0; i--);
//      ע��:��Щ��������ѿ�forѭ���������ô���ɾ������ѭ��������volatile���Ρ�
//����: var_len����������(�ֽ���)
//����: ����var_lenֵ������u32g_ns_of_u32/16/8for�е�һ����
u32 djy_get_for_time(ufast_t var_len)
{
    if(var_len == 1)
        return u32g_ns_of_u8for;
    else if(var_len == 2)
        return u32g_ns_of_u16for;
    else 
        return u32g_ns_of_u32for;
}


