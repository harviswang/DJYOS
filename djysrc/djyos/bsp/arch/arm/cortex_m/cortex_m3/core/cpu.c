//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include "cortexm3.h"
#include "exception.h"
#include <string.h>

// void __start_systick(void);
struct systick_reg volatile * const pg_systick_reg
                        = (struct systick_reg *)0xE000E010;

extern uint32_t  u32g_os_ticks;             //����ϵͳ����ticks��
volatile uint32_t    u32g_delay_10uS = 40;
static u32 u32g_ns_of_u32for; //for(i=j;i>0;i--);��䣬i��j����u32�ͣ�ÿѭ��������
static u32 u32g_ns_of_u16for; //for(i=j;i>0;i--);��䣬i��j����u16�ͣ�ÿѭ��������
static u32 u32g_ns_of_u8for;  //for(i=j;i>0;i--);��䣬i��j����u8�ͣ�ÿѭ��������

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
#if cfg_stack_fill != 0
    len = m_check_size(result);
    memset(result,cfg_stack_fill,len);
#endif

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
//    len = m_check_size(result);
    result->stack_top = (uint32_t*)((ptu32_t)result+len); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = len - sizeof(struct thread_vm); //����ջ���
    result->host_vm = NULL;
    //��λ������������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

static volatile ufast_t uf_delay_counter;
void set_delay_isr(void)
{
    uf_delay_counter++;
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
    uint32_t counter,u32_fors=10000,temp,clksum;
    uint16_t u16_fors=10000;
    uint8_t  u8_fors1 = 100;
    uint8_t  u8_fors2 = 99;
    volatile uint32_t u32loops;
    volatile uint16_t u16loops;
    volatile uint8_t  u8loops1,u8loops2;
    u32 backup;

    clksum = cn_fclk/1000;              //1000uS��������
    backup = u32g_exp_table[cn_sysvect_systick];
    u32g_exp_table[cn_sysvect_systick] = (u32)set_delay_isr;
    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        uf_delay_counter = 0;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u32loops=u32_fors;u32loops>0;u32loops--);       //ѭ��u32_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u32_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (uint32_t)(uf_delay_counter+1) * clksum;
            if(temp < 0xffffff)
            {
                clksum = temp;
            }else
            {
                clksum = 0xffffff;
                u32_fors = u32_fors / (temp / 0xffffff +1);
            }
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u32for = (uint64_t)counter * 10E8/cn_fclk/u32_fors;//���������64λ

    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        uf_delay_counter = 0;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u16loops=u16_fors;u16loops>0;u16loops--);       //ѭ��u16_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u16_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (uint32_t)(uf_delay_counter+1) * clksum;
            if(temp < 0xffffff)
            {
                clksum = temp;
            }else
            {
                clksum = 0xffffff;
                u16_fors = u16_fors / (temp / 0xffffff +1);
            }
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u16for = (uint64_t)counter * 10E8/cn_fclk/u16_fors;//���������64λ

    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        uf_delay_counter = 0;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u8loops1 = u8_fors1; u8loops1 > 0; u8loops1--)
            for(u8loops2=u8_fors2;u8loops2>0;u8loops2--); //ѭ��u8_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u8_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (uint32_t)(uf_delay_counter+1) * clksum;
            if(temp < 0xffffff)
            {
                clksum = temp;
            }else
            {
                clksum = 0xffffff;
                u8_fors1 = u8_fors1 / (temp / 0xffffff +1);
            }
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u8for = (uint64_t)counter * 10E8/cn_fclk /(u8_fors1*(u8_fors2+1));

    u32g_delay_10uS = 10000/u32g_ns_of_u32for;
    u32g_exp_table[cn_sysvect_systick] = backup;
}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __djy_init_tick(void)
{
    exp_connect_systick(djy_isr_tick);
    pg_systick_reg->reload = cn_fclk/cn_tick_hz;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                            |(1<<bo_systick_ctrl_tickint)   //��������ж�
                            |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
}

//----��ȡ��ǰticks-------------------------------------------------------------
//���ܣ���ȡ����ϵͳʱ��
//      u32g_os_ticks Ϊ32λ�޷��ű�����ARM��32λ��������ֱ�Ӷ�ȡ����32λϵͳ��
//      ��ȡ u32g_os_ticks ��Ҫ����1������,��˷���SysTimerʱ��Ҫ���жϡ�
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
u32 djy_get_time_tick(void)
{
    uint32_t time;
#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_save_asyn_signal();
#endif

    time = u32g_os_ticks;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_restore_asyn_signal();
#endif
    return time;
}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����32λ��������ת��0
//      u32g_os_ticks Ϊ32λ�޷��ű�����ARM��32λ��������ֱ�Ӷ�ȡ����32λϵͳ��
//      ��ȡ u32g_os_ticks ��Ҫ����1������,��˷���SysTimerʱ��Ҫ���жϡ�
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
u32 djy_get_time(void)
{
    u32 time;
#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low_t atom_low;
    atom_low = int_low_atom_start();
#endif

    time = u32g_os_ticks*cn_tick_us;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_low_atom_end(atom_low);
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
    u32 time;
    atom_low_t atom_low;
    atom_low = int_low_atom_start();
    
    time = (pg_systick_reg->reload - pg_systick_reg->current);
    time = u32g_os_ticks*cn_tick_us + (time*cn_fine_us >>16);
    
    int_low_atom_end(atom_low);
    return time;
}
//----10΢�뼶��ʱ-------------------------------------------------------------
//���ܣ�����ѭ��ʵ�ֵ�΢��ֱ�����ʱ������__djy_set_delay���������ʹ�ñ�������
//      �����ڲ�ͬ�Ż�����Ͳ�ͬ��������,��ʱ����ͬ.
//������time����ʱʱ�䣬��λΪ10΢��,���655���롣
//���أ���
//ע�⣺������ʹ�ô˺�����̫����ʱ������ʱ��ʹ�ú��� djy_event_delay,
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


