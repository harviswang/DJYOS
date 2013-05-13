//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�¼����͡��¼������Լ����¼�������صĴ���ȫ�������ˡ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __djyos_H__
#define __djyos_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
//�ṹ��������,���������ļ�����Ľṹ
struct semaphore_LCB;
//�ṹ��������,���ļ�����Ľṹ
struct  thread_vm;
struct  event_ECB;
struct  event_type;

#define mS      1000    //ϵͳʱ�䵥λ��΢�룬����ms�͵�*1000

#define cn_invalid_event_id cn_limit_uint16
#define cn_invalid_evtt_id  cn_limit_uint16    //�Ƿ��¼����ͺ�
#define cn_event_id_limit   32767       //����¼�id��
#define cn_evtt_id_limit    32767       //����¼�����id��
#define cn_timeout_forever  0xffffffff  //������ʱ

#define cn_sync_success     0       //ͬ���������
#define cn_sync_timeout     1       //ͬ������δ��ɣ���ʱ���أ���timeout��Ϊ0
#define cn_sync_unregister  2       //ͬ������δ��ɣ���ͬ���¼����ͱ�ע����
                                    //�����¼�������ص�ͬ����
#define cn_sync_error       cn_limit_uint32    //����

//������Ϣ����
enum knl_error_code
{
    enum_knl_no_error = cn_knl_no_error,  //û�д���
    enum_knl_etcb_error,            //�¼����Ϳ��ƿ����
    enum_knl_ecb_exhausted,         //�¼����ƿ�ľ�
    enum_knl_etcb_exhausted,        //�¼����Ϳ��ƿ�ľ�
    enum_knl_pcb_exhausted,         //�������ƿ�ľ�
    enum_knl_cant_sched,            //��ͼ�ڽ�ֹ����ʱִ�п��������¼����ȵĲ���
    enum_knl_invalid_prio,          //�Ƿ����ȼ�
    enum_knl_vpu_over,              //�¼����͵����������������
    enum_knl_para_over,             //�¼�������������������
    enum_knl_evtt_homonymy,         //�¼���������
    enum_knl_event_free,            //��ͼʹ�ÿ����¼����ƿ�
    enum_knl_evtt_free,             //��ͼʹ�ÿ����¼����Ϳ��ƿ�
    enum_knl_evttid_limit,          //�¼�����idԽ��
    enum_knl_evtt_unregister,       //�¼�����δ�Ǽ�
    enum_knl_event_sync_exit,       //�¼�ͬ���У���ͬ�����¼��쳣����
    enum_knl_memory_over,           //�ڴ治��
};

//�¼����ȼ����ƶ���
#define cn_prio_critical    (100)
#define cn_prio_real        (130)
#define cn_prio_RRS         (200)
#define cn_prio_wdt         (1)
#define cn_prio_sys_service (250)
#define cn_prio_invalid     (255)   //�Ƿ����ȼ�

//container_of��Ķ���,��������linux�Ĵ���,�����޸ĳɸ��㷺֧�ֵ�c90��ʽ
//container_of - ���ݽṹ��Աָ����������ṹ��ַ
//ptr:      �ṹ��Աָ��
//type:     �����ṹ������
//member:   ��Ա����
#define container_of(container,ptr, type, member) do{         \
        const void *__mptr = (void*)(ptr);    \
        container = (type *)( (char *)__mptr - offsetof(type,member) );}while(0)

struct process_vm       //���������
{
    u8  res;
};

#define cn_save_context_int     1
#define cn_save_context_noint   0
//��mmu�Ļ�����,��ַ����:0~1G����ϵͳ,1~2G���������,2~4G������������ռ�.
//�ر�ע��:���ṹҪ�������ʣ����Ա˳���ܸı䣬Ҳ�����������ӳ�Ա
struct  thread_vm        //��������ݽṹ
{
    //�����ջָ��,�����������ռʱ����sp,����ʱ������̬����sp�仯
    u32    *stack;
    u32    *stack_top;     //�������ջ��ָ��
    struct thread_vm *next;     //���ڰ�evtt�����п������������һ�����򿪿�����
                                //��������evtt��my_free_vmָ������
    u32    stack_size;     //ջ���
    struct process_vm *host_vm;  //�����������������si��dlspģʽ��ΪNULL
};

//�¼���������̬��ԭ��.
//�¼�Ӧ�����ڱյ�,djyos���ṩ��ѯ�¼���ǰ״̬�Ĺ���.����ṩ��ѯ����,��ô,ִ��
//��ѯ�ıض��ǵ�ǰ���еķ���,������ĵ�ǰ״̬�ض�������̬,��ѯ�����Ƕ��һ��.
//����ѯ���˵�״̬,�и�������֮��.
//ʹ�÷���:���¼���������̬ʱ,���ݽ���ǰ״̬�����¼���last_status.
//��ѯ��������last_statusֵ�ж��¼��Ӻ���״̬��������̬.
//�����ȼ��Ͷ��ȴ��Ĳ��迼��,����,ĳ�¼���ʱ��������Ϊ���ȼ��Ͷ��ȴ�һ��ʱ���
//������,��ѯ���Ϊ"������"
struct event_status_bit             //����̬���¼�����λ����0
{
    uint16_t event_delay:1;         //����ͬ��
    uint16_t wait_overtime:1;       //��ʱ�ȴ�
    uint16_t event_sync:1;          //�¼�ͬ��
    uint16_t event_sync_exit:1;     //�¼�ͬ���б�ͬ����Ŀ���¼��쳣�˳�
    uint16_t evtt_pop_sync:1;       //�¼����͵���ͬ��
    uint16_t evtt_done_sync:1;      //�¼��������ͬ��
    uint16_t wait_memory:1;         //��ϵͳ�ѷ����ڴ�ͬ��
    uint16_t wait_semp:1;           //�ź���ͬ��
    uint16_t wait_mutex :1;         //������ͬ��
    uint16_t wait_asyn_signal:1;    //�첽�ź�ͬ��
    uint16_t wait_cpipe:1;          //����pipe
    uint16_t wait_vpipe:1;          //��̬����pipe
};

union event_status
{
    uint16_t all;
    struct event_status_bit bit;
};

#define cn_db_info_event       0
#define cn_db_info_evtt        1

#define cn_creat_new_para   0x01    //����Ϊ���߳��¼������²���
#define cn_malloc_over_32   0x02    //��������32bytesʱ��ʹ��malloc����ռ�
//˵��:
//1.�����¼�ʱ����
struct  para_record 
{
    struct  para_record *next,*previous;
    struct event_ECB *sync;     //�ȴ���������Ҫ�����������ɵ��¼����У�
                                //��ΪNULL,�򲻵ȴ�����ֱ�ӷ���
    u32 start_time;             //����������ʱ�䣬����ϵͳ��ticks������uS
    bool_t dynamic_mem;         //true=����������ڴ��Ƕ�̬�����
    void *para;
    u8 static_para[32];         //�¼���������ʽ�ɵ����ʹ����¼���˫��Э��ȷ��.
                                //�������ߴ�����32�ֽڣ�ֱ�Ӵ�������
};

enum pop_para_sync
{
    enum_para_async=0,          //�����¼���Ҫ��ͬ��
    enum_para_sync,             //�����¼�Ҫ��ͬ���������¼���������ٷ���
};
struct event_ECB
{
    //�¼���ָ��,���ڹ�����������
    //pg_event_free: ���⣬�μ��ĵ�
    //pg_event_ready:���������μ��ĵ�
    //pg_event_delay:˫��ѭ��
    struct  event_ECB *next,*previous;
    //�๦������ָ�룬����������������:
    //1����ͬ�����У������¼�ͬ�����¼����͵���ͬ����
    //2���������������ȼ��������У�˫��ѭ��������ʵ����ѯ��y_event_ready��
    //   ��O(1)�㷨��
    struct  event_ECB *multi_next,*multi_previous;      //����ͬ������
    struct  thread_vm  *vm;             //�����¼����߳������ָ��,
    struct  para_record *para_high_prio;//�����ȼ����¼�����������
    struct  para_record *para_low_prio; //�����ȼ����¼�����������
    struct  para_record *para_current;  //��ǰ��Ҫ�����ڴ�����¼���
    struct  event_ECB *sync;            //ͬ���ڱ��¼��Ķ��У������¼���ɺ�
                                        //��������е��¼�
                                        //�����ͬ����һ��������ͬ���ǵ���ʱ�õ�
    struct  event_ECB **sync_head;      //��ס�Լ�����һ��ͬ�������У��Ա㳬ʱ
                                        //����ʱ�Ӹ�ͬ������ȡ���¼�
#if(cfg_debug_info == 1)
    u32    start_time;     //�¼�����ʱ�䣬����ϵͳ��ticks������uS
    u32    consumed_time;  //�¼����ĵ���ʱ��
    u32    consumed_time_second;    //���1�����ĵ�ʱ��
    u32    consumed_time_record;    //����ʱ��������¼
#endif
    u32    delay_start_tick;    //�趨����ʱ��
    u32    delay_end_tick;      //������ʱ��
    u32    error_no;       //���¼�ִ�в��������һ�������
    ptu32_t     event_result;   //������¼�����ʱ�������¼������ҵȴ�������
                                //(������pop����ʱ��sync=true)�����������أ�����
                                //������¼���������
    u32    wait_mem_size;  //�ȴ�������ڴ�����.
    union event_status last_status;    //���״̬,���ڲ�ѯ�¼���������̬ǰ��״̬
    union event_status event_status;   //��ǰ״̬,�������ɲ���ϵͳ�ڲ�ʹ��,
    ufast_t     prio;       //�¼����ȼ���0~255,�¼�һ���������Ͳ����ٸı����ȼ�
//    ufast_t     prio_bak;   //���ȼ����ݣ��������ȼ��̳��б��ݱ��������ȼ�
                            //���¼������ȼ���

    uint16_t    evtt_id;                //�¼�����id��0~32767
    uint16_t    sync_counter;           //ͬ������
    //�¼�id��Χ:0~32767(cn_event_id_limit)
    uint16_t    event_id;   //�¼����б��,��ͬ���¼����¼��������е�ƫ��λ��
    u32    local_memory;       //���¼��������������ľֲ��ڴ����
                                    //���ջؽ������ڴ�й©.
};

//�¼����Զ����
struct evtt_status
{
    u16 correlative:1;  //0=��ʾ�����¼�,�¼������п���ͬʱ���ڶ��������¼�
                        //1=ͬһ���͵Ķ���¼������й����ģ�����Э������
                        //���¼�������ֻ�������һ�����¼�
    u16 registered:1;   //0=���¼����ͻ�û��ע��,ϵͳ���ܾ�pop������¼
                        //1=���¼������Ѿ�ע��,����pop�������¼�
    u16 inuse:1;        //0=���ж����ж�û�и����͵��¼�
                        //1=������(�����ȴ���)������һ�������͵��¼�
    u16 deleting:1;     //0=����״̬��1=�ȴ�ע��״̬��
};

enum event_relation
{
    enum_independence=0,//��ʾ�����¼�,�¼������п���ͬʱ���ڶ��������¼�
    enum_correlative,   //�����¼���ͬһ���͵Ķ���¼������й����ģ�����Э������
                        //���¼�������ֻ�������һ�����¼�
};
struct event_type
{
    //ͬһ���͵��¼������ж�������ִ�л�ȴ�����,����Щ�¼�����ͬ������.
    struct evtt_status    property;
    //�����߳�ָ��,�����̸߳��¼�ʱ�����ȴ��������
    struct  thread_vm  *my_free_vm;
    char evtt_name[32]; //�¼���������û�����֣���ֻҪ�����֣��Ͳ�����ͬ��
                        //���һ�����Ͳ�ϣ�����ģ�鵯���¼������Բ������֡�
                        //��ģ�����Ҫ���浯���¼��������ַ��ʡ�
    //���ȼ�С��0x80Ϊ�������ȼ�,��Ӱ��������Ĺ���.�������ȼ��ڳ�ʼ��ʱ�趨,
    ufast_t     default_prio;       //�¼��������ȼ�.��ͬ���¼����ȼ�,1~255,0�Ƿ�.
    uint16_t    pop_sum,done_sum;   //�ѵ����¼�������������¼�����
    uint16_t    vpus_res;   //ϵͳ��æʱΪ�������¼��������������������������Ч
    uint16_t    vpus_limit; //������:�������¼�����ͬʱ�������߳����������
                            //������:���������п�ͬʱ���ɵĲ���������
    uint16_t    vpus;       //������:�������¼��Ѿ�ӵ�е��߳����������
                            //������:�������¼����������еĲ�������
    void (*thread_routine)(void);//����ָ��,��������ѭ��.
    u32 stack_size;              //thread_routine�����ջ��С

    struct event_ECB *mark_event; //�������¼�����Ч��ָ���¼������еı������¼�
    //�������ж�����ʣ������Ŷӵ�˫��ѭ������
    struct  event_ECB *done_sync,*pop_sync;//����ͬ�������ͬ������ͷָ��,
};

//��������(���ȼ�����),ʼ��ִ�ж���ͷ�����¼�,���ж�����ȼ���ͬ,����ִ��
extern struct  event_ECB  *pg_event_ready;
extern struct  event_ECB  *pg_event_running;   //��ǰ����ִ�е��¼�
extern bool_t bg_schedule_enable;

void __djy_set_delay(void);
sint32_t strnlen(const char *s,s32 maxlen);
void djy_isr_tick(void);
void djy_error_login(u32 error_type,const char *text);
void __djy_cut_ready_event(struct event_ECB *event);
void djy_set_RRS_slice(u32 slices);
u32 djy_get_RRS_slice(void);
u32 djy_get_last_error(char *text);
u32 djy_get_RRS_slice(void);
void __djy_select_event_to_run(void);
void djy_create_process_vm(void);
bool_t __djy_schedule(void);
void __djy_schedule_asyn_signal(void);
uint16_t djy_evtt_regist(enum event_relation relation,
                        ufast_t default_prio,
                        u16 vpu_res,
                        uint16_t vpus_limit,
                        void (*thread_routine)(void),
                        u32 stack_size,
                        char *evtt_name);
uint16_t djy_get_evtt_id(char *evtt_name);
bool_t djy_evtt_unregist(uint16_t evtt_id);
void __djy_init_sys(void);
bool_t djy_query_sch(void);
void __djy_event_ready(struct  event_ECB *event_ready);
void __djy_resume_delay(struct  event_ECB *delay_event);
void __djy_addto_delay(u32 u32l_uS);
bool_t djy_set_event_prio(ufast_t new_prio);
u32 djy_event_delay(u32 u32l_uS);
u32 djy_wait_event_completed(uint16_t event_id,u32 timeout);
u32 djy_wait_evtt_completed(uint16_t evtt_id,uint16_t done_times,u32 timeout);
u32 djy_wait_evtt_pop(uint16_t evtt_id,uint16_t pop_times,u32 timeout);
u16 djy_event_pop(  u16  evtt_id,
                    enum pop_para_sync para_sync,
                    u32 *pop_result,
                    u32 timeout,
                    void *para,
                    u32 para_size,
                    ufast_t para_options,
                    ufast_t prio);
ptu32_t djy_get_event_result(void);
void *djy_get_event_para(u32 *time);
void __djy_init_tick(void);
void __djy_start_os(void);
void djy_event_exit(void);
void djy_task_completed(ptu32_t result);
union event_status djy_wakeup_from(void);
struct  event_ECB *__djy_lookup_id(uint16_t id);
void __djy_vm_engine(void (*thread_routine)(void));
void __djy_service(void);
u16 djy_my_evtt_id(void);
void djy_api_start(u32 api_no);
void NULL_func(void);

#ifdef __cplusplus
}
#endif
#endif //__djyos_H__
