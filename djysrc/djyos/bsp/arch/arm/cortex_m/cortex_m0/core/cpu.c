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
#include "cortexm0.h"
#include "exception.h"

void __start_systick(void);
struct systick_reg volatile * const pg_systick_reg
                        = (struct systick_reg *)0xE000E010;

struct scb_reg volatile * const pg_scb_reg
                        = (struct scb_reg *)0xe000ed00;

struct nvic_reg volatile * const pg_nvic_reg
                        = (struct nvic_reg *)0xe000e100;

extern u32  u32g_os_ticks;             //����ϵͳ����ticks��
volatile u32    u32g_delay_10uS = 40;
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

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
#if cfg_stack_fill != 0
    len = m_check_size(result);
    memset(result,cfg_stack_fill,len);
#endif
    result->stack_top = (u32*)((ptu32_t)result+len); //ջ����ַ����ֲ����
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
    u32 counter,u32_fors=10000,clksum;
    uint16_t u16_fors=10000;
    uint8_t  u8_fors1 = 100;
    uint8_t  u8_fors2 = 99;
    volatile u32 u32loops;
    volatile uint16_t u16loops;
    volatile uint8_t  u8loops1,u8loops2;

    clksum = cn_fclk/50;              //������,������100mS
    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u32loops=u32_fors;u32loops>0;u32loops--);       //ѭ��u32_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u32_fors������ʱ��
        if((pg_systick_reg->ctrl & bo_systick_ctrl_countflag) != 0)
        {
            u32_fors = u32_fors>>1;     //ѭ����������
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u32for = (uint64_t)counter * 10E8/cn_fclk/u32_fors;//���������64λ

    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u16loops=u16_fors;u16loops>0;u16loops--);       //ѭ��u16_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u16_fors������ʱ��
        if((pg_systick_reg->ctrl & bo_systick_ctrl_countflag) != 0)
        {
            u16_fors = u16_fors>>1;     //ѭ����������
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u16for = (uint64_t)counter * 10E8/cn_fclk/u16_fors;//���������64λ

    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //����������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u8loops1 = u8_fors1; u8loops1 > 0; u8loops1--)
            for(u8loops2=u8_fors2;u8loops2>0;u8loops2--); //ѭ��u8_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //����������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u8_fors������ʱ��
        if((pg_systick_reg->ctrl & bo_systick_ctrl_countflag) != 0)
        {
            u8_fors1 = u8_fors1>>1;     //ѭ����������
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u8for = (uint64_t)counter * 10E8/cn_fclk /(u8_fors1*(u8_fors2+1));

    u32g_delay_10uS = 10000/u32g_ns_of_u32for;
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
    u32 time;
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
//���ܣ���ȡ����ϵͳʱ��
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
    int_save_asyn_signal();
#endif

    time = u32g_os_ticks*cn_tick_us;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_restore_asyn_signal();
#endif
    return time;
}

//----��ȡ����ʱ��-----------------------------------------------------------
//���ܣ���ȡ����ʱ��,��λuS����4294.967296����Ƶ�0.
//��������
//���أ�ʱ��ֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
u32 djy_get_fine_time(void)
{
    u32 time1,time2;
    atom_low_t atom_low;
    atom_low = int_low_atom_start();
    
    time1 = pg_systick_reg->current;
    if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
        time2 = (u32g_os_ticks+1)*cn_tick_us;
    else
        time2 = u32g_os_ticks*cn_tick_us
                    + ((pg_systick_reg->reload-time1)*cn_fine_us >>16);
    
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


