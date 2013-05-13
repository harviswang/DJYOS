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
#include "cpu_peri.h"

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
#if cfg_stack_fill != 0
    len = m_check_size(result);
    memset(result,cfg_stack_fill,len);
#endif

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
//    len = m_check_size(result);
    result->stack_top = (u32*)((ptu32_t)result+len); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = len - sizeof(struct thread_vm); //����ջ���
    result->host_vm = NULL;
    //��λ������������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

static volatile ufast_t uf_delay_counter;
u32 set_delay_isr(ufast_t timer_int_line)
{
    uf_delay_counter++;
    return 0;
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
    u32 counter,u32_fors=10000,temp,phase_ns;
    uint16_t u16_fors=10000,clksum;
    u8  u8_fors1 = 100;
    u8  u8_fors2 = 99;
    volatile u32 u32loops;
    volatile uint16_t u16loops;
    volatile u8  u8loops1,u8loops2;

    timer_set_clk_source(3,0);         //pclkԤ��Ƶ����1/2��Ƶ
    timer_set_precale(1,7);            //ʱ������Ƶ��pclk��8��Ƶ
    timer_set_type(3,1);                //�Զ�������������
    phase_ns = 10E8/cn_timer_clk * 16;  //ÿ�����ns��
    clksum = (uint16_t)(cn_timer_clk/16000);     //1000uS��������
    int_isr_connect(cn_int_line_timer3,set_delay_isr);
    int_setto_real(cn_int_line_timer3);          //timer3�жϱ���Ϊʵʱ�ж�
    int_enable_real_line(cn_int_line_timer3);
    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        timer_set_counter(3,clksum);
        uf_delay_counter = 0;
        timer_reload(3);
        timer_start(3);
        for(u32loops=u32_fors;u32loops>0;u32loops--); //ѭ��u32_fors��
        timer_stop(3);
        counter = timer_read(3);                  //��ȡѭ��u32_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65535;
                u32_fors = u32_fors / (temp / 65535 +1);
            }
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u32for = (uint64_t)counter * phase_ns /u32_fors;//���������64λ

    while(1)  //����16λ����ѭ��ʱ��(nS)
    {
        timer_set_counter(3,clksum);
        uf_delay_counter = 0;
        timer_reload(3);
        timer_start(3);
        for(u16loops=u16_fors;u16loops>0;u16loops--); //ѭ��u16_fors��
        timer_stop(3);
        counter = timer_read(3);                  //��ȡѭ��u32_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65535;
                u16_fors = u16_fors / (temp / 65535 +1);
            }
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    u32g_ns_of_u16for = (uint64_t)counter * phase_ns /u16_fors;//���������64λ

    while(1)  //����8λ����ѭ��ʱ��(nS)
    {
        timer_set_counter(3,clksum);
        uf_delay_counter = 0;
        timer_reload(3);
        timer_start(3);
        for(u8loops1 = u8_fors1; u8loops1 > 0; u8loops1--)
            for(u8loops2=u8_fors2;u8loops2>0;u8loops2--); //ѭ��u8_fors��
        timer_stop(3);
        counter = timer_read(3);                  //��ȡѭ��u8_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65535;
                u8_fors1 = u8_fors1 / (temp / 65535 +1);
            }
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
    counter = clksum - counter;    //ȡʵ����������
    //���������64λ
    u32g_ns_of_u8for = (uint64_t)counter * phase_ns /(u8_fors1*(u8_fors2+1));

    u32g_delay_10uS = 10000/u32g_ns_of_u32for;
}

u32 __djy_isr_tick(ufast_t line)
{
    djy_isr_tick();
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
	int_cut_line(cn_int_line_timer3);
    //����timer3��Ϊtick�ж�
    int_isr_connect(cn_int_line_timer3,__djy_isr_tick);
    int_setto_asyn_signal(cn_int_line_timer3);          //tick�жϱ���Ϊ�첽�ź�
    //�������ö�ʱ������������port_config.h��cn_tick_us��cn_tick_hz��
    //cn_fine_ns��cn_fine_hz�Ķ���һ��
    timer_set_clk_source(3,0);          //pclkԤ��Ƶ����1/2��Ƶ
    timer_set_precale(1,cn_timer_clk/cn_fine_hz/2 -1);
    //���ϰѶ�ʱ������ʱ�ӵ�Ƶ����Ϊcn_fine_hz
    //timer_set_counter(3,(uint16_t)((u32)cn_tick_us*1000/cn_fine_ns));
    timer_set_counter(3, cn_fine_hz/cn_tick_hz);
    timer_set_type(3,1);                //����tick��ʱ����������
    timer_reload(3);                    //���ض�ʱֵ
    timer_start(3);                     //������ʱ��
    int_restore_asyn_line(cn_int_line_timer3);//����tick�ж�S
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
    int_save_asyn_line(cn_int_line_timer3);
#endif

    time = u32g_os_ticks;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_restore_asyn_line(cn_int_line_timer3);
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
    int_save_asyn_line(cn_int_line_timer3);
#endif

    time = u32g_os_ticks*cn_tick_us;

#if (32 > cn_cpu_bits)
    //���������ֳ�����32λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    int_restore_asyn_line(cn_int_line_timer3);
#endif
    return time;
}

//----��ȡ����ʱ��-----------------------------------------------------------
//���ܣ���ȡ����ϵͳ����ʱ��,ʱ�䵥λ�������ļ�������,��Ӳ����ͬ����ͬ,һ����Ϊ
//      tick�İٷ�֮һ����ǧ��֮һ,44b0x����Ϊǧ��֮һ.����Ǵ��ϴ�ticks�ı䵽
//      ���ڵ�ʱ����/
//��������
//���أ�ʱ��ֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
u32 djy_get_fine_time(void)
{
    //return (((u32)cn_tick_us*1000/cn_fine_ns)-pg_timer_reg->TCNTO3) *cn_fine_ns;

    u32 time1,time2;
    atom_low_t atom_low;
    atom_low = int_low_atom_start();

    time1 = pg_timer_reg->TCNTO3;
//    if(pg_timer_reg->INTCTLSTAT & 0x02)
//        time2 = (u32g_os_ticks+1)*cn_tick_us;
//    else
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
