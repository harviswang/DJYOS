//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:����ģ��
//����:  ������.
//�汾��V1.1.0
//�ļ�����:�¼����͡��¼������Լ����¼�������صĴ���ȫ�������ˡ�
//����˵��:
//�޶���ʷ://5. ����: 2011-07-19
//   ����:  ������.
//   �°汾��: V1.3.0
//   �޸�˵��:���һ�����ܵ����ڴ�й©��bug����bug���������������ߴ����32�ֽڣ�
//   ���ڵ���djy_event_popʱҪ��̬������������ڴ�ռ䣬djy_task_completed��
//   djy_event_exit�����ͷŲ���ʱ�����ͷŸö�̬������ڴ档
//   �޸ķ�������struct  para_record�ṹ�����ӳ�Աdynamic_mem�����ڱ�ʾ�����Ƿ�
//   ��̬����õ��ģ����ǣ�����djy_task_completed��djy_event_exit�����ͷŲ���ʱ
//   ����m_free�ͷ�֮��
//4. ����: 2009-12-15
//   ����:  ������.
//   �°汾��: V1.3.0
//   �޸�˵��:
//   1��djy_evtt_regist���ж�ͬ���¼�����ʱ�����۸��¼����Ϳ��ƿ���Ч�񣬶�����
//      strcmp�����Ƚϣ�Ч�ʽϵͣ���Ϊ���ж��Ƿ�գ�Ȼ��űȽϡ�
//   2��ԭ����djy_evtt_regist�����ܴ�����������µǼ�ʧ��ʱ��û�д����¼�����
//      ���ƿ飬������Ȼ����registered=1��״̬��
//3. ����: 2009-10-05
//   ����:  ������.
//   �°汾��: V1.2.0
//   �޸�˵��:
//   1��������tg_int_global.nest_asyn_signal==0��Ϊ�Ƿ�������ȵ����ݣ���ᵼ��
//      �ж�ģ��͵���ģ�����������Ϲ�ϵ��
//      ��ȫ�ֱ���bg_schedule_enable��Ϊ�Ƿ�������ȵı�־
//   2���ĵ���һЩgcc���Լ�c99����֧�ֵ�����������Ӧ���㷺�ı�����
//   3��ǰ��İ汾����ʵ��djy_evtt_unregist��ȫ�����ܣ����汾��ȫ֮��
//   4��__djy_resume_delay��������һ�У�
//      pg_event_delay = pg_event_delay->previous;��Ϊ
//      pg_event_delay = pg_event_delay->next;
//      ����ᵼ������ͬ�����д���
//   5��ԭ�汾��,__turnto_context�������2ְ���ں˳�ʼ����ɺ��״ε��ñ�������
//      �������̵߳��ȵĹ��ܣ�����arm���ǿ��Եģ�����ȷ������cpuҲ���ԣ�����cm3
//      �ϾͲ��У��Ȱ�������ܷ������������һ������__asm_start_thread������arm
//      �汾���ú궨�壺#define __asm_start_thread __asm_turnto_context
//   6��int_restore_asyn_signal�����У���ִ��__int_contact_asyn_signal���жϣ�
//      Ȼ����ִ�У�
//            if(pg_event_running != pg_event_ready)
//                __djy_schedule();
//      �ڴ��ж���__djy_schedule�������¹ر��ж�֮�䣬������ж���ռ�Ļ�������
//      �������ش���int_restore_trunk����Ҳ��ͬ�������⡣
//      �°汾�У�__djy_schedule��������__asm_switch_contextʱ�Ŵ��жϣ��ž���
//      �������⣬__djy_schedule�������������ڹر��жϵ�������ִ�еġ�
//2. ����: 2009-03-18
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��:
//   1��������ΪСɮ�����������ֵ��������¼����͵����������ָ�������Ļ������
//      �û�����y_evtt_registʱʹ�õ��Ǿֲ����飬�����ڱ������ֵ��ڴ汻�ͷŵ�Σ
//      �ա���Ϊ����洢���31�ַ�����struct event_type�ṹ�а�evtt_name��ָ��
//      ��Ϊ���飬�޸��漰���к�����
//          __djy_init_sys��y_evtt_regist��y_evtt_unregist��y_get_evtt_id
//   2����y_evtt_unregist�������ͷ������������һ��bug�����ڷ��ʷǷ��ڴ��Σ�գ�
//      ��֮��
//   3��y_evtt_done_sync�����д���ͬ�����д���������bug�����ܵ��·��ʷǷ��ڴ棬
//      ��ʹͬ�����л��ң���֮��
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

struct process_vm *pg_my_process;
//Ϊcn_events_limit���¼����ƿ�����ڴ�
struct event_ECB tg_ECB_table[cn_events_limit];
static struct para_record  tg_event_para_table[cn_paras_limit];
//Ϊcn_evtts_limit���¼����Ϳ��ƿ�����ڴ�
struct event_type tg_evtt_table[cn_evtts_limit];
static struct  event_ECB  *pg_event_free; //��������ͷ,������
static struct  para_record  *pg_para_free; //��������ͷ,������
//��ת����ʱ��Ƭ��0��ʾ��ֹ��ת���ȣ�Ĭ��1��RRS = "round robin scheduling"��д��
static u32  u32g_RRS_slice = 1;

struct  event_ECB  *pg_event_ready;      //��������ͷ
struct  event_ECB  *pg_event_running;    //��ǰ����ִ�е��¼�
struct  event_ECB  *pg_event_delay;      //����ͬ�����б�ͷ
u32 u32g_running_start_time;        //��ǰ�������¼��Ŀ�ʼִ��ʱ��.
u32  u32g_os_ticks;            //����ϵͳ����ticks��
bool_t bg_schedule_enable;     //ϵͳ��ǰ����״̬�Ƿ��������

#if(cfg_debug_info == 1)

void debug_scan(void)
{
    u32 *para;
    u32 pl_ecb;
    u32 time1;
    while(1)
    {
        para = (u32*)djy_get_event_para(NULL); 
        if(para != NULL)
        {
            switch(*para)
            {
                case cn_db_info_event:
                {
                    printf_ng("�¼���  ��������  ʹ��   ���ȼ� CPU  �ڴ�\n\r");
                    for(pl_ecb = 0; pl_ecb < cn_events_limit; pl_ecb++)
                    {
                        
                        if(tg_ECB_table[pl_ecb].previous != 
                                        (struct  event_ECB*)&pg_event_free)
                        {
                            printf_ng("%05d   %05d     ",pl_ecb,tg_ECB_table[pl_ecb].evtt_id);
                            printf_ng("ʹ���� ");
                            printf_ng("%03d    ",tg_ECB_table[pl_ecb].prio);
                            time1 = tg_ECB_table[pl_ecb].consumed_time_second/10000;
                            printf_ng("%2d\%%  %8x",time1,tg_ECB_table[pl_ecb].vm->stack_size);
                        }
                        else
                        {
                            printf_ng("%5d             ",pl_ecb);
                            printf_ng("����");
                        }
                        printf_ng("\n\r");
                    }
                }break;
                case cn_db_info_evtt:
                {
                    printf_ng("���ͺ�  �Ǽ�   ���ȼ� ������    �ڴ�   ����\n\r");
                    for(pl_ecb = 0; pl_ecb < cn_evtts_limit; pl_ecb++)
                    {
                        
                        if(tg_evtt_table[pl_ecb].property.registered)
                        {
                            printf_ng("%05d   ",pl_ecb);
                            printf_ng("��ע�� ");
                            printf_ng("%03d    ",tg_evtt_table[pl_ecb].default_prio);
                            printf_ng("%8x  %8x ",
                                        tg_evtt_table[pl_ecb].thread_routine,
                                        tg_evtt_table[pl_ecb].stack_size);
                            if(tg_evtt_table[pl_ecb].evtt_name[0] != '\0')
                                printf_ng("%s",&tg_evtt_table[pl_ecb].evtt_name);
                            else
                                printf_ng("����");
                        }
                        else
                        {
                            printf_ng("%5d   ",pl_ecb);
                            printf_ng("����");
                        }
                        printf_ng("\n\r");
                    }
                    
                }break;
                default:break;
            }
        }
        djy_task_completed(0);
        for(pl_ecb = 0; pl_ecb < cn_events_limit; pl_ecb++)
        {
            tg_ECB_table[pl_ecb].consumed_time_second =
                              tg_ECB_table[pl_ecb].consumed_time
                            - tg_ECB_table[pl_ecb].consumed_time_record;
            tg_ECB_table[pl_ecb].consumed_time_record = 
                            tg_ECB_table[pl_ecb].consumed_time;
        }
        
        djy_wait_evtt_pop(djy_my_evtt_id(),1,1000*mS);
    }
}

//----��ʼ��������Ϣģ��-------------------------------------------------------
//����: ����������Ϣ�¼����Ͳ�����֮
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void init_debug_info(ptu32_t para)
{
    u16 evtt_debug;
    evtt_debug = djy_evtt_regist(enum_correlative,1,0,1,
                                 debug_scan,100,"debug_info");
    if(evtt_debug == cn_invalid_evtt_id)
        return;
    djy_event_pop(evtt_debug,enum_para_async,NULL,0,NULL,0,0,0);
}
#endif

//----ʵʱ������---------------------------------------------------------------
//����: strlen������ִ��ʱ�䲻�ɿأ�������ʵʱ������ʹ�á�
//����: s�������ַ���
//      over�����ⳤ��,over=0���޳��ȣ���strlen�������졣
//����: �����ȣ���over��Χ��û�м�⵽�������򷵻�-1
//-----------------------------------------------------------------------------
sint32_t strnlen(const char *s,s32 maxlen)
{
    u32 len;
    if(maxlen == 0)
        return strlen(s);
    else
    {
        for(len = 0; len < maxlen; len++)
        {
            if(s[len] =='\0')
            {
                return len;
            }
        }
        return cn_limit_uint32;
    }
}

//����Ϊ�����������

//----tick�ж�-----------------------------------------------------------------
//���ܣ�Ϊ����ϵͳ����ʱ��Դ�����Ƚ�����ͬ�������е��¼�����ʱ�䣬���������ʱ�䵽��
//      �¼����Ͱ����Ǽ���¼����������С����ʱ��Ƭ��ת���ȱ�������Ҫ���Ƿ�Ҫ
//      ִ����ת��ticks������Ϊ�첽�źš�
//������line��ticks�жϵ��ж��ߺţ�ʵ���ϲ��á�
//���أ���
//-----------------------------------------------------------------------------
void  djy_isr_tick(void)
{
    struct  event_ECB *pl_ecb,*pl_ecbp,*pl_ecbn;
    u32g_os_ticks++;    //ϵͳʱ��,4294967296�κ����,������

    if(pg_event_delay != NULL)
    {
        pl_ecb = pg_event_delay;
        while(1)
        {
            if(pl_ecb->delay_end_tick == u32g_os_ticks)
            {
                //�¼���ĳͬ�������У�Ӧ�ôӸö���ȡ��
                if(pl_ecb->sync_head != NULL)   
                {
                    if(*(pl_ecb->sync_head) == pl_ecb)//���¼�λ�ڶ���ͷ��
                    {
                        if(pl_ecb->multi_next == pl_ecb)//�ö�����ֻ��һ���¼�
                        {
                            *(pl_ecb->sync_head) = NULL;
                            
                        	pl_ecb->multi_next = NULL;
                            pl_ecb->multi_previous = NULL;
                        }else                   //��ͬ���������ж���¼�
                        {
                            //ͷָ��ָ����һ���¼�
                            *pl_ecb->sync_head = pl_ecb->multi_next;
                            pl_ecb->multi_previous->multi_next
                                                = pl_ecb->multi_next;
                            pl_ecb->multi_next->multi_previous
                                                = pl_ecb->multi_previous;
                        }
                    }else           //���¼����Ƕ���ͷ
                    {
                        pl_ecb->multi_previous->multi_next
                                            = pl_ecb->multi_next;
                        pl_ecb->multi_next->multi_previous
                                            = pl_ecb->multi_previous;
                    }
                    pl_ecb->sync_head = NULL;   //�¼�ͷָ���ÿ�
                }
                if(pl_ecb->next == pl_ecb)      //��������ͬ���������һ�����.
                {
                    pg_event_delay = NULL;
                    pl_ecb->last_status.all = pl_ecb->event_status.all;
                    pl_ecb->event_status.bit.event_delay = 0;
                    pl_ecb->event_status.bit.wait_overtime = 0;
                    __djy_event_ready(pl_ecb);
                    break;
                }else
                {
                    pg_event_delay = pl_ecb->next;
                    pl_ecb->next->previous = pl_ecb->previous;
                    pl_ecb->previous->next = pl_ecb->next;
                    pl_ecb->last_status.all = pl_ecb->event_status.all;
                    pl_ecb->event_status.bit.event_delay = 0;
                    pl_ecb->event_status.bit.wait_overtime = 0;
                    __djy_event_ready(pl_ecb);
                    pl_ecb = pg_event_delay;
                }
            }else
                break;
        }
    }
    
    //���洦��ʱ��Ƭ��ת����.
    //���ڿ��첽�ź�(�������)�ſ��ܽ���__djy_isr_tick����ʹ�������쵼�����¼���
    //�룬pg_event_runningҲ�ض������ȼ����������У������ܲ�����pg_event_ready
    if(u32g_RRS_slice != 0)      //������ת����
    {
        if( (pg_event_running->prio == pg_event_running->next->prio)
                    &&(pg_event_running != pg_event_running->next) )
        {//�����ȼ��ж���¼�������תʱ���Ƿ�
            if(u32g_os_ticks % u32g_RRS_slice == 0) //ʱ��Ƭ����
            {
                //�ȴ������ȼ��������У���pg_event_running�Ӷ�����ȡ������֮��
                //pg_event_running->next��
                pl_ecb = pg_event_running->next;
                pl_ecbn = pg_event_running->multi_next;
                pl_ecbp = pg_event_running->multi_previous;
                pl_ecbp->multi_next = pl_ecb;
                pl_ecb->multi_previous = pl_ecbp;
                pl_ecbn->multi_previous = pl_ecb;
                pl_ecb->multi_next = pl_ecbn;
                pg_event_running->multi_next = NULL;
                pg_event_running->multi_previous = NULL;

                //�ٴ����������,��pg_event_running�Ӿ����������ó�����Ȼ�����
                //�ŵ�ͬ���ȼ������

                //���û�и����ȼ��¼���ͬ��ʱ�䵽������ready���У�����������
                if(pg_event_ready == pg_event_running)
                    pg_event_ready = pl_ecb;
//                pl_ecbn = pg_event_ready->multi_next;
//                pl_ecbp = pg_event_running->previous;
                //�����а�pg_event_running�Ӷ�����ȡ��
                pg_event_running->previous->next = pl_ecb;
                pl_ecb->previous = pg_event_running->previous;
                //�����а�pg_event_running����Ŀ��λ��
                pg_event_running->previous = pl_ecbn->previous;
                pg_event_running->next = pl_ecbn;
                pl_ecbn->previous->next = pg_event_running;
                pl_ecbn->previous = pg_event_running;
            }
        }
    }
    return;
}

//----�ǼǴ�����Ϣ--------------------------------------------------------------
//���ܣ���ϵͳ������Ϣ�Ǽǵ�һ�����λ�������
//������error_type����������
//      text������˵��
//���أ���
//��������ı����Ĺ��ܣ�---db
//-----------------------------------------------------------------------------
void djy_error_login(u32 error_type,const char *text)
{
    pg_event_running->error_no = error_type;
}

//----��ȡ���һ��������Ϣ-----------------------------------------------------
//���ܣ�����
//������text�����ظô��������˵��
//���أ������
//��������ı�����ʱ��Ĺ��ܣ�ʱ��ʹ��ticks��---db
//-----------------------------------------------------------------------------
u32 djy_get_last_error(char *text)
{
    return pg_event_running->error_no;
}

//----��ready������ȡ��һ���¼�------------------------------------------------
//����: ��һ���¼���ready������ȡ������ʹready������������
//����: event����ȡ�����¼�ָ��
//����: ��
//ע: �������뱣֤���첽�ź�(����)����ֹ������µ��ñ�����
//-----------------------------------------------------------------------------
void __djy_cut_ready_event(struct event_ECB *event)
{
    struct event_ECB *pl_ecb;
    if(event != pg_event_ready)         //event����ready����ͷ
    {
        if(event->multi_next == NULL)   //�������ȼ�����������
        {
            event->next->previous = event->previous;
            event->previous->next = event->next;
        }else                           //�����ȼ�����������
        {
            pl_ecb = event->next;
            event->next->previous = event->previous;
            event->previous->next = event->next;
            if(pl_ecb->prio == event->prio)     //��Ӧ���ȼ���ֹһ���¼�
            {
                event->multi_previous->multi_next = pl_ecb;
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_next->multi_previous = pl_ecb;
                pl_ecb->multi_next = event->multi_next;
            }else                               //��Ӧ���ȼ�ֻ��һ���¼�
            {
                //pl_ecb��event->multi_next.
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = pl_ecb;
            }
        }
    }else                               //event��ready����ͷ
    {
        pg_event_ready = event->next;
        pl_ecb = event->next;
        event->next->previous = event->previous;
        event->previous->next = event->next;
        if(pl_ecb->prio == event->prio)     //��Ӧ���ȼ���ֹһ���¼�
        {
            event->multi_previous->multi_next = pl_ecb;
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_next->multi_previous = pl_ecb;
            pl_ecb->multi_next = event->multi_next;
        }else                               //��Ӧ���ȼ�ֻ��һ���¼�
        {
            //pl_ecb��event->multi_next.
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pl_ecb;
        }
    }
}

//----������ת����ʱ��Ƭ-------------------------------------------------------
//����: djyos�����в�����ת���ȵ��¼�ʱ��Ƭ������ͬ�ģ����������á������Ϊ0��
//      ��ʾ������ʱ��Ƭ��ת��
//����: slices���µ���ת����ʱ��Ƭ��΢��������������ȡ��Ϊ������ticksʱ��
//����: ��
//-----------------------------------------------------------------------------
void djy_set_RRS_slice(u32 slices)
{
#if (32 > cn_cpu_bits)
    atom_low_t atom_low;
    //���������ֳ���32λ,��Ҫ������ڲ��ܸ���u32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low = int_low_atom_start( );   //�������Բ���Ƕ�׵���
#endif

    u32g_RRS_slice =(slices + cn_tick_us -1)/cn_tick_us;

#if (32 > cn_cpu_bits)
    //���������ֳ���32λ,��Ҫ������ڲ��ܸ���u32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    int_low_atom_end( atom_low );
#endif
}

//----��ѯ��ת����ʱ��Ƭ-------------------------------------------------------
//����: djyos�����в�����ת���ȵ��¼�ʱ��Ƭ������ͬ�ģ����ñ�������ѯ��
//����: ��
//����: ��ǰʱ��Ƭ���ȣ�΢������
//-----------------------------------------------------------------------------
u32 djy_get_RRS_slice(void)
{
    u32 temp;
#if (32 > cn_cpu_bits)
    atom_low_t atom_low;
    //�������ֳ���32λ,��Ҫ������ڲ��ܶ�ȡu32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low = int_low_atom_start( );   //�������Բ���Ƕ�׵���
#endif

    temp = u32g_RRS_slice;

#if (32 > cn_cpu_bits)
    //�������ֳ���32λ,��Ҫ������ڲ��ܶ�ȡu32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    int_low_atom_end( atom_low );
#endif
    return temp * cn_tick_us;
}

//----���ҿ������߳�----------------------------------------------------------
//����: �� pg_event_ready�����л��һ���������е��߳�.
//      1.��pg_event_ready����ͷ��ʼ����,������¼���û�������߳�,����ͼ����֮.
//      2.������ܽ���,�϶������ڴ治�㣬��Ѹ��¼��ŵ��ڴ�ȴ�����,�������²���.
//      3.��˷���,ֱ���ҵ�һ���������е��߳�.Ȼ���pg_event_readyָ��ָ����¼�
//����: ��
//����: ��
//��ע: �������ɲ���ϵͳ����,����ǰ��֤���첽�źš�
//      ����ϵͳ�����¼�����ready,�����������ҵ��������е��߳�.
//----------------------------------------------------------------------------
void __djy_select_event_to_run(void)
{
    struct  event_ECB *pl_ecb;
    struct  event_type *pl_evtt;  //���������¼�������ָ��


    while(pg_event_ready->vm == NULL)
    {
        pl_evtt =& tg_evtt_table[pg_event_ready->evtt_id];
        if(pl_evtt->my_free_vm != NULL)     //���¼������п��е��߳�,ֱ��ʹ��
        {
            pg_event_ready->vm = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = pl_evtt->my_free_vm->next;
        }else       //���¼�����û�п��е��߳�,��ͼ����֮
        {
            pg_event_ready->vm = __create_thread(pl_evtt);  //�����߳�
            if(pg_event_ready->vm == NULL)                  //�����߳�ʧ��
            {
                pl_ecb = pg_event_ready;
                __djy_cut_ready_event(pl_ecb);
#if (cfg_dynamic_mem == 1)
                __m_wait_memory(pl_ecb);
#endif
            }else                                           //�ɹ������߳�
            {
                if(pl_evtt->property.correlative == enum_independence)
                    pl_evtt->vpus++;        //�����¼�vpus��ʾ�߳���
                                            //�����¼���vpus=��������
            }
        }
    }
}

//----�������������-----------------------------------------------------------
//����: ���������������
//����: ��
//����: ��
//��ע: ��ֻ��ռһ��λ���ѣ���mpģʽ����ʵ�ü�ֵ
//-----------------------------------------------------------------------------
void djy_create_process_vm(void)
{
    static struct process_vm my_process;
    pg_my_process = &my_process;
}


//----�¼�����-----------------------------------------------------------------
//���ܣ�����running�¼���cpu,��cpu����ready���еĵ�һ���¼�.���ready���еĵ�һ
//      ���¼���δӵ���߳�,����֮.�����߳�ʱ����ڴ治��,�������¼��ŵ�
//      �ڴ�ȴ�������,readyָ��ָ��ready���е���һ���¼�,������֮,ֱ���ҵ�һ��
//      �߳������ָ��ǿջ��߳ɹ������߳���������¼�.
//��������
//���أ�true = �ɹ��л���false = δ�л�ֱ�ӷ���
//��ע��1.�������ɲ���ϵͳ����
//      2.djyosϵͳ��Ϊ,�û���ֹ�жϾ���Ϊ���ܹ�����ִ��,���߱����ٽ���Դ.
//      djyos���첽�ź��൱�ڸ����ȼ��߳�,����ȫ�ֺ��첽�źŽ�ֹ״̬
//      ��,�ǲ������¼����ȵ�.
//      3.ʵʱ�ж��Ƿ��ֹ,������޹�.
//      4.����������ȼ���ϵͳ�����¼�����ready,��˱����������ܹ��ҵ�Ŀ���¼�
//-----------------------------------------------------------------------------
bool_t __djy_schedule(void)
{
    struct  event_ECB *event;
    u32 time;

    if(!djy_query_sch())
        return false;
    int_cut_asyn_signal();
    __djy_select_event_to_run();
    if(pg_event_ready != pg_event_running)
    {//��running�¼�����ready������,���ڴ治���Խ����������ʱ,���ܻ��������
     //������running���¼�ȫ�������ڴ�ȴ����еĿ���.��ʱִ��else�Ӿ�.
        event = pg_event_running;
#if(cfg_debug_info == 1)
        time = djy_get_fine_time();
        event->consumed_time += time - u32g_running_start_time;
        u32g_running_start_time = time;
#endif
        pg_event_running=pg_event_ready;
        __asm_switch_context(pg_event_ready->vm ,event->vm);
    }else
    {//���ȼ�����running���¼�ȫ�������ڴ�ȴ�����,��һ��Ҫ������¼�����
     //running�¼�,����ִ���κβ���
        return false;
    }
    return true;
}

//----�ж���ִ�е��¼�����------------------------------------------------------
//���ܣ�����running�¼���cpu,��cpu����ready���еĵ�һ���¼�.���ready���еĵ�һ
//      ���¼��в�ӵ���߳�,�򴴽�֮.�����߳�ʱ����ڴ治��,�������¼��ŵ�
//      �ڴ�ȴ�������,readyָ��ָ��ready���е���һ���¼�,������֮,ֱ���ҵ�һ��
//      �߳������ָ��ǿջ��߳ɹ������߳���������¼�.
//��������
//���أ���
//��ע���������ɲ���ϵͳ���첽�ź����淵��ǰ����
//      ����������ȼ���y_idle_service�¼�����ready,��������ܹ��ҵ����ȶ����.
//-----------------------------------------------------------------------------
void __djy_schedule_asyn_signal(void)
{
    struct  event_ECB *event;
    u32 time;

    __djy_select_event_to_run();
    if(pg_event_ready != pg_event_running)
    {//��running�¼�����ready������,���ڴ治���Խ����������ʱ,���ܻ��������
     //������running���¼�ȫ�������ڴ�ȴ����еĿ���.��ʱִ��else�Ӿ�.
         event=pg_event_running;
#if(cfg_debug_info == 1)
         time = djy_get_fine_time();
         event->consumed_time += time - u32g_running_start_time;
         u32g_running_start_time = time;
#endif
         pg_event_running=pg_event_ready;
         __asm_switch_context_int(pg_event_ready->vm,event->vm);
    }else
    {//���ȼ�����running���¼�ȫ�������ڴ�ȴ�����,��һ��Ҫ������¼�����
     //running�¼�,����ִ���κβ���
    }
    return;
}

//----�Ǽ��¼�����------------------------------------------------------------
//���ܣ��Ǽ�һ���¼����͵�ϵͳ��,�¼����;����ǼǺ�,�Ϳ���pop��,����,ϵͳ��
//      �ܾ�pop�������¼�
//������relation��ȡֵΪenum_independence��enum_correlative
//      default_prio�����¼����͵�Ĭ�����ȼ���
//      vpus_res��ϵͳ��æʱΪ�������¼������������������
//      vpus_limit��ͬһ�¼��������¼�����ӵ�е��߳����������ֵ
//      thread_routine���߳���ں���(�¼�������)
//      stack_size��ִ��thread_routine��Ҫ��ջ�ߴ磬������thread_routine��������
//          ���õ�ϵͳ����
//      evtt_name���¼�����������ͬģ��֮��Ҫ���浯���¼��Ļ������¼���������
//          ������������NULL����ֻҪ��NULL���Ͳ��������¼����ͱ������е���������
//          ���ֲ�����31�����ֽ��ַ�
//���أ����¼����͵����ͺ�
//------------------------------------------------------------------------------
u16 djy_evtt_regist(enum event_relation relation,
                       ufast_t default_prio,
                       u16 vpus_res,
                       u16 vpus_limit,
                       void (*thread_routine)(void),
                       u32 stack_size,
                       char *evtt_name)
{
    u16 i,evtt_id;
    if((default_prio >= cn_prio_sys_service) || (default_prio == 0))
    {
        djy_error_login(enum_knl_invalid_prio,"�¼��������ȼ��Ƿ�");
        return cn_invalid_evtt_id;
    }
    int_save_asyn_signal();      //��ֹ����Ҳ���ǽ�ֹ�첽�¼�
    for(evtt_id=0; evtt_id<cn_evtts_limit; evtt_id++)   //���ҿ��е��¼����ƿ�
        if( tg_evtt_table[evtt_id].property.registered == 0)
            break;
    if(evtt_id == cn_evtts_limit)     //û�п����¼����ƿ�
    {
        djy_error_login(enum_knl_etcb_exhausted,"û�п����¼����ƿ�");
        int_restore_asyn_signal();
        return cn_invalid_evtt_id;
    }else if(evtt_name != NULL) //�����������֣�������û������
    {
        for(i=0; i<cn_evtts_limit; i++)
        {
            if(tg_evtt_table[i].property.registered == 1)
            {
                if(strcmp(tg_evtt_table[i].evtt_name,evtt_name) == 0)
                {
                    djy_error_login(enum_knl_evtt_homonymy,"�¼���������");
                    int_restore_asyn_signal();
                    return cn_invalid_evtt_id;
                }
            }
        }
        if(strnlen(evtt_name,31) != cn_limit_uint32)
            strcpy(tg_evtt_table[evtt_id].evtt_name,evtt_name);
        else
        {
            memcpy(tg_evtt_table[evtt_id].evtt_name,evtt_name,31);
            tg_evtt_table[evtt_id].evtt_name[31] = '\0';
        }
    }else
    {
        tg_evtt_table[evtt_id].evtt_name[0] = '\0';   //����¼�������
    }
    
    tg_evtt_table[evtt_id].default_prio = default_prio;
    tg_evtt_table[evtt_id].pop_sum = 0;
    tg_evtt_table[evtt_id].done_sum = 0;
    tg_evtt_table[evtt_id].vpus_limit = vpus_limit;
    if(relation == enum_independence)
    {
        tg_evtt_table[evtt_id].vpus_res = vpus_res;
    }else
    {
        tg_evtt_table[evtt_id].vpus_res = 0;    //�������¼���vpus_res��Ч
    }
    tg_evtt_table[evtt_id].thread_routine = thread_routine;
    tg_evtt_table[evtt_id].stack_size = stack_size;
    tg_evtt_table[evtt_id].mark_event = NULL;
    tg_evtt_table[evtt_id].done_sync = NULL;
    tg_evtt_table[evtt_id].pop_sync = NULL;
    tg_evtt_table[evtt_id].vpus = 0;
    if((cn_run_mode!=cn_mode_mp)||(default_prio<0x80))
    {//����ģʽΪsi��dlsp������¼�����ӵ�н������ȼ�����Ԥ�ȴ���һ���߳������
        tg_evtt_table[evtt_id].my_free_vm =
                                __create_thread(&tg_evtt_table[evtt_id]);
        if(tg_evtt_table[evtt_id].my_free_vm == NULL)
        {//�ڴ治�㣬���ܴ��������
            djy_error_login(enum_mem_tried,"�Ǽ��¼�����ʱ�ڴ治��");
            int_restore_asyn_signal();
            return cn_invalid_evtt_id;
        }else
        {
            if(relation == enum_independence) //�������¼���vpus=��������
                tg_evtt_table[evtt_id].vpus = 1;
        }
    }else
    {
        tg_evtt_table[evtt_id].my_free_vm = NULL;
    }
    tg_evtt_table[evtt_id].property.correlative = relation;
    tg_evtt_table[evtt_id].property.registered = 1;
    tg_evtt_table[evtt_id].property.inuse = 0;
    tg_evtt_table[evtt_id].property.deleting = 0;
    int_restore_asyn_signal();
    return evtt_id;
}

//----ȡ�¼�����id-------------------------------------------------------------
//���ܣ������¼����͵����֣����ظ��¼����͵�id�ţ�������û���ֵ��¼����͡�
//������evtt_name���¼�������
//���أ��¼�����id�ţ����û���ҵ��򷵻�cn_invalid_id��
//��ע��ֻ���ҵ������ֵ��¼����ͣ�û���ֵ��¼������ֽ������¼����͡�
//----------------------------------------------------------------------------
u16 djy_get_evtt_id(char *evtt_name)
{
    u16 loop;
    if(evtt_name == NULL)
        return cn_invalid_evtt_id;
    for(loop = 0; loop < cn_evtts_limit; loop++)
    {
        if(strcmp(tg_evtt_table[loop].evtt_name,evtt_name) ==0)
            return loop;
    }
    return cn_invalid_evtt_id;
}

//----ɾ���¼�����-------------------------------------------------------------
//����: ɾ��һ���¼�����,��������л��и������¼�(inuse == 1),ֻ��Ǹ�����Ϊ��
//      ɾ��,������ɾ���������� done��������ɵ�.����������Ѿ�û�и������¼���,
//      ����ִ��������ɾ������.�����������,ϵͳ����ܾ����������͵����¼�.
//����: evtt_id,��ɾ�����¼����ͺ�
//����: true = �ɹ��������ɹ���ǣ�false = ʧ��
//-----------------------------------------------------------------------------
bool_t djy_evtt_unregist(u16 evtt_id)
{
    struct thread_vm *next_vm,*temp;
    struct event_type *evtt;
    struct event_ECB *pl_ecb,*pl_ecb_temp;
    bool_t result = true;
    if(evtt_id >= cn_evtts_limit)
        return false;
    int_save_asyn_signal();
    evtt = &tg_evtt_table[evtt_id];
    if(evtt->property.registered == false)
    {//���¼���������Ч�¼�����
        result = false;
    }
    if(evtt->done_sync != NULL)     //�����ͬ�����������¼���ȡ������������
    {
        pl_ecb = evtt->done_sync;     //ȡͬ������ͷ
        while(pl_ecb != NULL)
        {
            pl_ecb->last_status.all = pl_ecb->event_status.all;   //���浱ǰ״̬
//            pl_ecb->event_status.bit.evtt_done_sync = 0;     //ȡ��"ͬ����"״̬
            if(pl_ecb->event_status.bit.wait_overtime)   //�Ƿ��ڳ�ʱ������
            {
                __djy_resume_delay(pl_ecb);    //������ʱ�ȴ�
//                pl_ecb->event_status.bit.wait_overtime = 0;  //ȡ��"��ʱ�ȴ���"״̬
            }
            pl_ecb->event_status.all = 0;   
            pl_ecb_temp = pl_ecb;
            if(pl_ecb->multi_next == evtt->done_sync)   //�����һ���¼�
            {
                evtt->done_sync = NULL;
                pl_ecb = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;
            }
            __djy_event_ready(pl_ecb_temp);           //���¼����뵽����������
        }
    }
    if(evtt->pop_sync != NULL)     //������ͬ�����������¼���ȡ������������
    {
        pl_ecb = evtt->pop_sync;     //ȡͬ������ͷ
        while(pl_ecb != NULL)
        {
            pl_ecb->last_status.all = pl_ecb->event_status.all;   //���浱ǰ״̬
//            pl_ecb->event_status.bit.evtt_pop_sync = 0;     //ȡ��"ͬ����"״̬
            if(pl_ecb->event_status.bit.wait_overtime)   //�Ƿ��ڳ�ʱ������
            {
                __djy_resume_delay(pl_ecb);    //������ʱ�ȴ�
//                pl_ecb->event_status.bit.wait_overtime = 0;  //ȡ��"��ʱ�ȴ���"״̬
            }
            pl_ecb->event_status.all = 0;   
            pl_ecb_temp = pl_ecb;
            if(pl_ecb->multi_next == evtt->done_sync)  //�����һ���¼�
            {
                evtt->pop_sync = NULL;  //�ÿ��¼�ͬ������
                pl_ecb = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;      //ȡ��������һ���¼�
            }
            __djy_event_ready(pl_ecb_temp);       //���¼����뵽����������
        }
    }
    if(evtt->property.inuse)
    {
        //�¼���������ʹ�ã������ͬ���͵���ͬ�����зǿգ�ֻ���ɾ��
        evtt->property.deleting = 1;
    }else
    {
        //�����¼����Ϳ��ƿ飬ֻ���registered�������㡣
        evtt->property.registered = 0;
        evtt->evtt_name[0] = '\0';      //����¼�������
        next_vm = evtt->my_free_vm;
        while(next_vm != NULL)          //�ͷŸ��¼�����ӵ�еĿ��������
        {
            temp = next_vm;
            next_vm = next_vm->next;
            m_free((void *)temp);
        }
    }
    int_restore_asyn_signal();
    return result;
}
const struct event_ECB cn_sys_event = {
                        tg_ECB_table,tg_ECB_table,//next,previous
                        tg_ECB_table,tg_ECB_table,//multi_next,multi_previous
                        NULL,                          //vm
                        NULL,                       //para_high_prio
                        NULL,                       //para_low_prio
                        NULL,                       //para_current
                        NULL,                       //sync
                        NULL,                       //sync_head
#if(cfg_debug_info == 1)
                        0,                          //start_time
                        0,                          //consumed_time
                        0,                          //consumed_time_second
                        0,                          //consumed_time_record
#endif
                        0,                          //delay_start_tick
                        0,                          //delay_end_tick
                        enum_knl_no_error,          //error_no
                        0,                          //event_result
                        0,                          //wait_mem_size
                        {0},                        //last_status
                        {0},                        //event_status
                        cn_prio_sys_service,        //prio
                        0,                          //evtt_id
                        0,                          //sync_counter
                        0,                          //event_id
                        0                           //local_memory
                        };

//----�����¼�����-------------------------------------------------------------
//���ܣ�1.����ϵͳ�趨��ʼ������ϵͳ��������¼���ָ��
//      2.�� cn_events_limit ���¼����ƿ�ṹ���ӳ�һ�����ж���,���еĽṹ�μ�
//        �ĵ�����ָ��pg_free_event����,
//��������
//���أ���
//-----------------------------------------------------------------------------
void __djy_init_sys(void)
{
    u16 i;
    struct thread_vm *vm;
    pg_event_delay=NULL;    //��ʱ�¼������

    //���¼����ͱ�ȫ����Ϊû��ע��,0Ϊϵͳ��������
    for(i=1; i<cn_evtts_limit; i++)
    {
        tg_evtt_table[i].property.registered = 0;
    }
    for(i = 1; i < cn_events_limit; i++)    //i=0Ϊϵͳ�����¼�
    {
        if(i==(cn_events_limit-1))
            tg_ECB_table[i].next = NULL;
        else
            tg_ECB_table[i].next = &tg_ECB_table[i+1];
        //��ǰָ��ָ��pg_event_free�ĵ�ַ,˵�����¸������¼���.
        //û�б�ĺ���,ֻ����һ��Ψһ�Ҳ������ֵ,ȫ�ֱ�����ַ������������
        //�ǲ���仯��.
        tg_ECB_table[i].previous = (struct  event_ECB*)&pg_event_free;
        tg_ECB_table[i].event_id = i;    //��id���ڳ���������ά�ֲ���
    }
    pg_event_free = &tg_ECB_table[1];

    //��ʼ������������
    for(i = 0; i < cn_paras_limit; i++)
    {
        if(i==(cn_paras_limit-1))
            tg_event_para_table[i].next = NULL;
        else
            tg_event_para_table[i].next = &tg_event_para_table[i+1];
    }
    pg_para_free = tg_event_para_table;

    tg_evtt_table[0].property.correlative = 1;
//    tg_evtt_table[0].property.overlay = 0;
    tg_evtt_table[0].property.registered = 1;
    tg_evtt_table[0].property.inuse = 1;
    tg_evtt_table[0].property.deleting = 0;
    tg_evtt_table[0].my_free_vm = NULL;
    tg_evtt_table[0].evtt_name[0] = '\0';
    tg_evtt_table[0].default_prio = cn_prio_sys_service;
    tg_evtt_table[0].pop_sum = 0;
    tg_evtt_table[0].done_sum = 0;
    tg_evtt_table[0].vpus_res =0;
    tg_evtt_table[0].vpus_limit =5;
    tg_evtt_table[0].vpus = 1;
    tg_evtt_table[0].thread_routine = __djy_service;
    tg_evtt_table[0].stack_size = cn_syssvc_stack;
    tg_evtt_table[0].mark_event = tg_ECB_table;
    tg_evtt_table[0].done_sync = NULL;
    tg_evtt_table[0].pop_sync = NULL;

    vm = __create_thread(&tg_evtt_table[0]);
    if(vm == NULL)      //�ڴ治�㣬���ܴ�����פ�����
    {
        djy_error_login(enum_mem_tried,"���������ʱ�ڴ治��");
        tg_evtt_table[0].vpus = 0;
        return ;
    }
    //����ģ��popup����,����ϵͳ�����¼�.���¼�����Ϊ��ʱ,����popup�Ľ��
    //�ǲ���Ԥ֪��.����ϵͳ����ʱ,ϵͳ�����¼����ڶ�����,�����¼������ǲ����
    //��,����ģ��popup�¼�,������ֱ�ӵ���,����ʹpopup������ʡ��һ���ж϶����Ƿ�
    //�յĲ���.
    tg_ECB_table[0] = cn_sys_event;
    tg_ECB_table[0].vm = vm;
    tg_ECB_table[0].event_status.all = 0;
    pg_event_ready = tg_ECB_table;
    pg_event_running = pg_event_ready;
}

//--------����Ƿ��������------------------------------------------------------
//����: ����Ƿ��������,�����첽�ź����������߳�̬ʱ���������
//����: ��
//����: ������ȷ���true,���򷵻�false
//-----------------------------------------------------------------------------
bool_t djy_query_sch(void)
{
    return bg_schedule_enable;
}

//----���¼��������������----------------------------------------------------
//���ܣ����¼����뵽���������к��ʵ�λ��,���¼�ԭ�����ھ���������.������ֻ����
//      �¼����У�������event_status��ֵ
//������event_ready,��������¼�,���¼�ԭ�����ھ���������
//���أ���
//------------------------------------------------------------------------------
void __djy_event_ready(struct  event_ECB *event_ready)
{
    struct  event_ECB *event;
    atom_low_t atom_low;
    atom_low = int_low_atom_start(  );             //�������Բ���Ƕ�׵���
    event = pg_event_ready;
    do
    { //�ҵ�һ�����ȼ��������¼����¼�.����ϵͳ�����¼�prio=250����ready,����������ҵ�.
        if(event->prio <= event_ready->prio)
            event = event->multi_next;
        else
            break;
    }while(event != pg_event_ready);
    event_ready->next = event;
    event_ready->previous = event->previous;
    event->previous->next = event_ready;
    event->previous = event_ready;

    //�²�����¼���ͬ���ȼ�����󣬹������ܹ��ж����¼��Ƿ�����ȼ���Ψһ�¼���
    //���Ǹ����ȼ���Ψһ�¼�����Ҫ�������ȼ���������
    if(event_ready->prio != event_ready->previous->prio)
    {//��ʱevent == event_ready->next;
        event->multi_previous->multi_next = event_ready;
        event_ready->multi_previous = event->multi_previous;
        event->multi_previous = event_ready;
        event_ready->multi_next = event;
    }else       //����������ȼ���������
    {
        event_ready->multi_next = NULL;
        event_ready->multi_previous = NULL;
    }
    if(event_ready->prio < pg_event_ready->prio)
        pg_event_ready = event_ready;
    int_low_atom_end( atom_low );
}

//----�˳�����ͬ������---------------------------------------------------------
//����: ��һ���¼�������ͬ��������ȡ��,��������ʱ���Ƿ��Ѿ�����.���ڴ���ʱ��ͬ��
//      �����У���ͬ���������ڳ�ʱʱ�޵���ʱ��������ͬ������ȡ���¼���
//����: delay_event,��������¼�.
//����: ��
//��ע: 1.�����������Ÿ��û�,�����ǲ���ϵͳ�ڲ�ʹ��.����ϵͳ�����û��ṩ��ǰ
//      �˳���ʱ�Ĺ���,�������Է�ֹ�¼��以�����
//      2.������Ӧ���ڹرյ��������µ���,�����߱�֤,�����ڲ�������ж�״̬.
//      3.������ֻ���¼�������ͬ��������ȡ���������ŵ����������С�
//-----------------------------------------------------------------------------
void __djy_resume_delay(struct  event_ECB *delay_event)
{
    if(pg_event_delay->next == pg_event_delay)  //������ֻ��һ���¼�
        pg_event_delay = NULL;
    else
    {
        if(delay_event == pg_event_delay)
            pg_event_delay = pg_event_delay->next;
        delay_event->next->previous = delay_event->previous;
        delay_event->previous->next = delay_event->next;
    }
    delay_event->next = NULL;
    delay_event->previous = NULL;
    delay_event->delay_end_tick = djy_get_time_tick();
}

//----������ʱ����------------------------------------------------------------
//���ܣ�������ִ�е��¼�����,ֱ�Ӱ��Լ�������ʱ���У����������Ҳ������ready
//      ���У�һ����ͬ���������ã���timeout!=0ʱ���¼���������ͬ������ʵ��
//      timeout���ܣ����ض������¶�y_timer_sync�����ļ򻯡�
//������u32l_uS,�ӳ�ʱ��,��λ��΢�룬�������ϵ���Ϊcn_tick_us��������
//���أ���
//��ע��1������ϵͳ�ڲ�ʹ�õĺ����������ڹر��жϣ���ֹ���ȣ���������ʹ�á�
//      2�����ñ�����ǰrunning�¼��Ѿ��Ӿ�������ȡ�������������ı������
//      3���������ڲ�����һ�����ɵ��÷���֤���������ԣ���u32l_uS>0.
//-----------------------------------------------------------------------------
void __djy_addto_delay(u32 u32l_uS)
{
    struct  event_ECB * event;
    u32 start,delay;

    pg_event_running->delay_start_tick = djy_get_time_tick(); //�¼���ʱ��ʼʱ��
    delay = u32l_uS + cn_tick_us -(u32)1;
    //���u32l)uS�ӽ�0xffffffffʱ�����������������������������
    //delay_start_tick��delay_end_tick��ȵ������
    if(delay < cn_tick_us)
        delay = 0xffffffff;
    pg_event_running->delay_end_tick = pg_event_running->delay_start_tick
                    + delay/cn_tick_us;  //����ʱ��

    if(pg_event_delay==NULL)    //��ʱ���п�
    {
        pg_event_running->next = pg_event_running;
        pg_event_running->previous = pg_event_running;
        pg_event_delay=pg_event_running;
    }else
    {
        event = pg_event_delay;
        start=pg_event_running->delay_start_tick;
        delay = pg_event_running->delay_end_tick - start;
        do
        {//��ѭ���ҵ���һ��ʣ����ʱʱ���������ʱ�¼����¼�.
            //���1tick=1mS,��ô49.7��󽫻���,djyos�����ṩ����49.7�����ʱ
            //���ֱ�ӱȽ�delay_end,��һ���������ƶ���һ��������ʱ,�������,
            //�������ù̶�λ���޷���������󣬸�λ���ضϵ��ص㣬��delay_end
            //-start�����бȽϣ����Ա������ʱ��������
            if((event->delay_end_tick - start) <= delay)
                event = event->next;
            else
                break;
        }while(event != pg_event_delay);
        //���û���ҵ�ʣ����ʱʱ�������ʱ�¼������¼�,���¼��������β,
        //������β������pg_event_delay��ǰ��,eventǡ�õ���pg_event_delay
        //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
        //��������ǰ��ѭ���ҵ����,����ʹ�����д���
        pg_event_running->next = event;
        pg_event_running->previous = event->previous;
        event->previous->next = pg_event_running;
        event->previous = pg_event_running;
        if(pg_event_delay->delay_end_tick -start 
                    > pg_event_running->delay_end_tick-start)
            //���¼���ʱС��ԭ�����е���С��ʱ.
            pg_event_delay = pg_event_running;
    }
}

//----�����¼����ȼ�-----------------------------------------------------------
//����: �¼������У����Ե��ñ��������ı���������ȼ���������ȼ����ĵ��ˣ�����
//      �������ȣ��������¼����С�
//����: new_prio,���õ������ȼ�
//����: true = �ɹ����ã�false=ʧ�ܣ�һ�������ȼ����Ϸ�
//-----------------------------------------------------------------------------
bool_t djy_set_event_prio(ufast_t new_prio)
{
    if((new_prio >= cn_prio_sys_service) || (new_prio == 0))
        return false;
    int_save_asyn_signal();
    if(new_prio <= pg_event_running->prio)
        pg_event_running->prio = new_prio;
    else
    {
        __djy_cut_ready_event(pg_event_running);
        pg_event_running->next = NULL;
        pg_event_running->previous = NULL;
        pg_event_running->prio = new_prio;
        __djy_event_ready(pg_event_running);
    }
    int_restore_asyn_signal();
    return true;
}

//----����ͬ��-----------------------------------------------------------------
//���ܣ�������ִ�е��¼�����,ʹ�Լ���ͣu32l_uS΢����������.
//������u32l_uS,�ӳ�ʱ��,��λ��΢�룬0��������ת��������¼��ŵ�ͬ���ȼ���
//      ��󡣷�0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�ʵ����ʱʱ��(ticks)��
//��ע����ʱ����Ϊ˫��ѭ������
//-----------------------------------------------------------------------------
u32 djy_event_delay(u32 u32l_uS)
{
    struct  event_ECB * event;
    u32 start,delay;

    if( !djy_query_sch())
    {   //��ֹ���ȣ����ܽ�������ͬ��״̬��
        djy_error_login(enum_knl_cant_sched,NULL);
        return 0;
    }
    int_save_asyn_signal();
    //��ʱ��Ϊ0���㷨:������������ͬ���ȼ��ģ��ѱ��¼��ŵ���ת���һ����
    //ע��:���ﲻ����ת�����Ƿ�����
    if(u32l_uS == 0)
    {
        if((pg_event_running->prio == pg_event_running->next->prio)
                    && (pg_event_running != pg_event_running->next)   )
        {
            pg_event_running->delay_start_tick = djy_get_time_tick();//����ʱ��
            __djy_cut_ready_event(pg_event_running);      //��ͬ������ȡ��
            __djy_event_ready(pg_event_running);            //�Ż�ͬ������β��
        }else
        {
            int_restore_asyn_signal();
            return 0;   //��ʱ��Ϊ0���Ҳ�������ת����
        }
    }else
    {
        pg_event_running->delay_start_tick =djy_get_time_tick();//�趨�����ʱ��
        delay = u32l_uS + cn_tick_us -(u32)1;
        //���u32l)uS�ӽ�0xffffffffʱ�����������������������������
        //delay_start_tick��delay_end_tick��ȵ������
        if(delay < cn_tick_us)
            delay = 0xffffffff;
        pg_event_running->delay_end_tick = pg_event_running->delay_start_tick
                        + delay/cn_tick_us;  //����ʱ��

        __djy_cut_ready_event(pg_event_running);

        pg_event_running->last_status.all = pg_event_running->event_status.all;
        pg_event_running->event_status.bit.event_delay=1;
        if(pg_event_delay==NULL)    //����ͬ�����п�
        {
            pg_event_running->next = pg_event_running;
            pg_event_running->previous = pg_event_running;
            pg_event_delay=pg_event_running;
        }else
        {
            event = pg_event_delay;
            start = pg_event_running->delay_start_tick;
            delay = pg_event_running->delay_end_tick - start;
            do
            {//��ѭ���ҵ���һ������ʱ���������¼����¼�.
                //���1tick=1mS,��ô49.7��󽫻���,djyos�����ṩ����49.7�����ʱ
                //���ֱ�ӱȽ�delay_end,��һ���������ƶ���һ��������ʱ,�������,
                //�������ù̶�λ���޷���������󣬸�λ���ضϵ��ص㣬��delay_end
                //-start�����бȽϣ����Ա������ʱ��������
                if((event->delay_end_tick - start) <= delay)
                    event = event->next;
                else
                    break;
            }while(event != pg_event_delay);
            //��������¼�����ǰ���ҵ����¼�ǰ�棬��û���ҵ�����event������
            //pg_event_delay������˫��ѭ�����У�g_event_delayǰ��Ҳ�͸պ��Ƕ���β��
            pg_event_running->next = event;
            pg_event_running->previous = event->previous;
            event->previous->next = pg_event_running;
            event->previous = pg_event_running;
            if(pg_event_delay->delay_end_tick -start
                    >pg_event_running->delay_end_tick-start)
                //���¼���ʱС��ԭ�����е���С��ʱ.
                pg_event_delay = pg_event_running;
        }
    }
    int_restore_asyn_signal();
    return (djy_get_time_tick() -pg_event_running->delay_start_tick)*cn_tick_us;
}

//----ͬ���¼�----------------------------------------------------------------
//����: ���������е��¼����뵽ָ���¼���ͬ��������ȥ,Ȼ�����µ��ȡ���ָ���¼�
//      ������ɣ����߳�ʱʱ�䵽�������ѵ�ǰ�¼���
//����: event_id,Ŀ���¼�id��
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: cn_sync_success=ͬ�������������أ�
//      cn_sync_timeout=��ʱ���أ�
//      enum_knl_cant_sched=��ֹ����ʱ����ִ��ͬ������
//      enum_knl_event_free,event_id�ǿ���id
//----------------------------------------------------------------------------
u32 djy_wait_event_completed(u16 event_id,u32 timeout)
{
    struct  event_ECB * pl_ecb;
    pl_ecb = &tg_ECB_table[event_id];

    if(djy_query_sch() == false)  //�����ڽ�ֹ���ȵ������ִ��ͬ������
        return (u32)enum_knl_cant_sched;
    if(timeout == 0)
        return (u32)cn_sync_timeout;
    int_save_asyn_signal();
    if(pl_ecb->previous == (struct event_ECB *)&pg_event_free)
    {//Ŀ���¼����ƿ��ǿ����¼����ƿ�
        int_restore_asyn_signal();
        return (u32)enum_knl_event_free;
    }
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->next = NULL;
    pg_event_running->previous = NULL;

    //���°�pg_event_running���뵽Ŀ���¼���ͬ��������

    pg_event_running->sync_head = &pl_ecb->sync;
    if(pl_ecb->sync != NULL)
    {//��ͬ���¼���ͬ�����в��ǿյ�
        pg_event_running->multi_next = pl_ecb->sync;
        pg_event_running->multi_previous = pl_ecb->sync->multi_previous;
        pl_ecb->sync->multi_previous->multi_next = pg_event_running;
        pl_ecb->sync->multi_previous = pg_event_running;
    }else
    {//��ͬ���¼���ͬ�������ǿյ�
        pl_ecb->sync = pg_event_running;
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
    }

    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.event_sync = 1;
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    int_restore_asyn_signal();  //�ָ��жϻ��������µ���

    int_save_asyn_signal();
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(pg_event_running->event_status.bit.event_sync)
    {//˵��ͬ������δ�����ӳ�ʱ���ء�
        pg_event_running->event_status.bit.event_sync = 0;
        int_restore_asyn_signal();
        return (u32)cn_sync_timeout;
    }else if(pg_event_running->event_status.bit.event_sync_exit)
    {//��ͬ���¼����쳣��ֹ������û�б���ȷִ��
        pg_event_running->event_status.bit.event_sync_exit = 0;
        int_restore_asyn_signal();
        return (u32)enum_knl_event_sync_exit;
    }else
    {
        int_restore_asyn_signal();
        return (u32)cn_sync_success;
    }
}

//----�¼��������ͬ��----------------------------------------------------------
//����: ���������е��¼����뵽ָ���¼����͵�ǰͬ��������ȥ,Ȼ�����µ��ȡ����
//      ͬ����Ŀ���¼����͵���ɴ���Ϊͬ��������
//����: evtt_id,Ŀ���¼����ͺ�
//      done_times,��ɴ�����0��ʾ�����һ���������¼����.
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: cn_sync_success=ͬ�������������أ�
//      cn_sync_timeout=��ʱ���أ�
//      enum_knl_cant_sched=��ֹ����ʱ����ִ��ͬ������
//      enum_knl_evtt_free=evtt_id�ǿ���id
//      ������������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ�����ô����ġ�
//----------------------------------------------------------------------------
u32 djy_wait_evtt_completed(u16 evtt_id,u16 done_times,u32 timeout)
{
    struct  event_type *evtt;
    struct event_ECB *pl_ecb;
    evtt = &tg_evtt_table[evtt_id];
    if(djy_query_sch() == false)  //�����ڽ�ֹ���ȵ������ִ��ͬ������
        return (u32)enum_knl_cant_sched;
    if(timeout == 0)
        return (u32)cn_sync_timeout;
    int_save_asyn_signal();
    if((evtt->property.registered == false)     //�¼�����δע��
        ||(evtt->property.deleting == 1))       //�¼��������ڵȴ�ע��
    {
        int_restore_asyn_signal();
        return (u32)enum_knl_evtt_free;
    }
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->next = NULL;
    pg_event_running->previous = NULL;
    pg_event_running->sync_counter = done_times;

    //���°�pg_event_running���뵽Ŀ���¼���ͬ��������
    pg_event_running->sync_head = &evtt->done_sync;
    if(evtt->done_sync != NULL)
    {//��ͬ���¼����͵�ͬ�����в��ǿյ�
        pl_ecb = evtt->done_sync;
        do
        {//��ѭ���ҵ���һ��ʣ����ɴ�����С�����¼����¼�.
            if(pl_ecb->sync_counter < done_times)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != evtt->done_sync);
        //��������¼�(pg_event_running)���뵽������
        pg_event_running->multi_next = pl_ecb;
        pg_event_running->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = pg_event_running;
        pl_ecb->multi_previous = pg_event_running;
        if(evtt->done_sync->sync_counter > done_times)
            evtt->done_sync = pg_event_running;
    }else
    {//��ͬ���¼����͵�ͬ�������ǿյ�
        evtt->done_sync = pg_event_running;
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
    }

    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.evtt_done_sync = 1;
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    int_restore_asyn_signal();  //�ָ����Ȼ������¼��л�

    int_save_asyn_signal();
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(pg_event_running->event_status.bit.evtt_done_sync)
    {//˵��ͬ������δ�����ӳ�ʱ���أ���ʱ����ͬ�����¼��϶���û����ɡ�
        pg_event_running->event_status.bit.evtt_done_sync = 0;
        int_restore_asyn_signal();
        return (u32)cn_sync_timeout;
    }else if(pg_event_running->event_status.bit.event_sync_exit)
    {//��ͬ���¼����쳣��ֹ������û�б���ȷִ��
        pg_event_running->event_status.bit.event_sync_exit = 0;
        int_restore_asyn_signal();
        return (u32)enum_knl_event_sync_exit;
    }else
    {
        int_restore_asyn_signal();
        return (u32)cn_sync_success;
    }
}

//----�¼����͵���ͬ��---------------------------------------------------------
//����: ���������е��¼����뵽ָ���¼����͵ĵ���ͬ��������ȥ,Ȼ�����µ��ȡ�����
//      ͬ����ָ�Ը��¼����͵��¼��������ɴ�Ϊͬ��������
//����: evtt_id,Ŀ���¼����ͺ�
//      pop_times,����������������0.
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: cn_sync_success=ͬ�������������أ�
//      cn_sync_timeout=��ʱ���أ�
//      cn_sync_error=����
//      ������������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ�����ô����ġ�
//----------------------------------------------------------------------------
u32 djy_wait_evtt_pop(u16 evtt_id,u16 pop_times,u32 timeout)
{
    struct  event_type *evtt;
    struct event_ECB *pl_ecb;
    evtt = &tg_evtt_table[evtt_id];
    //�����ڽ�ֹ���ȵ������ִ��ͬ������,��������������Ϊ0
    if((djy_query_sch() == false) || (pop_times == 0))
        return cn_sync_error;
    if(timeout == 0)
        return cn_sync_timeout;
    int_save_asyn_signal();
    if((evtt->property.registered == false)  //�¼�����δע��
        ||(evtt->property.deleting == 1))   //�¼��������ڵȴ�ע��
    {
        int_restore_asyn_signal();
        return cn_sync_error;
    }
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->next = NULL;
    pg_event_running->previous = NULL;
    pg_event_running->sync_counter = pop_times;

    //���°�pg_event_running���뵽Ŀ���¼���ͬ��������
    pg_event_running->sync_head = &evtt->pop_sync;
    if(evtt->pop_sync != NULL)
    {//��ͬ���¼����͵�ͬ�����в��ǿյ�
        pl_ecb = evtt->pop_sync;
        do
        {//��ѭ���ҵ���һ��ʣ�൯��������С�����¼����¼�.
            if(pl_ecb->sync_counter < pop_times)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != evtt->pop_sync);
        //��������¼�(pg_event_running)���뵽������
        pg_event_running->multi_next = pl_ecb;
        pg_event_running->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = pg_event_running;
        pl_ecb->multi_previous = pg_event_running;
        if(evtt->pop_sync->sync_counter > pop_times)
            evtt->pop_sync = pg_event_running;
    }else
    {//��ͬ���¼���ͬ�������ǿյ�
        evtt->pop_sync = pg_event_running;
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
    }
    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.evtt_pop_sync = 1;
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
        
    int_restore_asyn_signal();  //�ָ��жϻ��������µ���

    int_save_asyn_signal();
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(pg_event_running->event_status.bit.evtt_pop_sync)
    {//˵��ͬ������δ�����ӳ�ʱ���أ���ʱ��ͬ�������϶���û�д�ɡ�
        pg_event_running->event_status.bit.evtt_pop_sync = 0;
        int_restore_asyn_signal();
        return cn_sync_timeout;
    }else
    {
        int_restore_asyn_signal();
        return cn_sync_success;
    }
}

//----�����¼�-----------------------------------------------------------------
//���ܣ������ϵͳ���淢���¼�,����ϵͳ�ӱ���,������������ʵ��̴߳����¼�.
//������evtt_id���¼�����id
//      para_sync��enum_para_sync=�ȴ��¼�������Ϻ󷵻أ�
//            enum_para_async=�����¼�����������.
//            ������������еĲ��������Ѿ��ﵽ�¼����Ϳ��ƿ��е�vpus_limitֵ����
//            �Զ���enum_para_sync����-db,δʵ�֣��ݰ�����vpus_limitֱ�ӷ��ش���
//      pop_result,������������˺Ϸ����¼�id����para_sync=enum_para_sync����
//              pop_result = cn_sync_success����ʾ�¼���������ɺ󷵻�
//              pop_result = cn_sync_timeout����ʾ�¼�δ��������ʱ����
//              ���sync=false����pop_result�����塣
//          �������������cn_invalid_event_id���򷵻ؾ���ĳ�����Ϣ
//      timeout,���sync=true�����峬ʱʱ�䣬���=0���򵯳��¼�����������ʱ
//             ���أ����¼�����õ�������λ��us
//      para,�¼�����ָ�룬�京�����¼��������������ߴ���ò�����32�ֽڡ�
//      para_size���������ȣ���ò�����32.
//      para_options, ����ѡ�����bit����:
//          bit0:1=���Ǵ����²������������У�0=�����Ӧ�������޲�������
//              �����²�������������ö��е����һ����������ֵ�������Ͳ���Ч
//          bit1:��������32bytesʱ�Ĵ�������
//              0=ֱ��ʹ�õ��÷��ṩ�Ļ���������ʱӦ�ó���Ӧ�����н���ڴ���ʳ�
//              ͻ�����⣬һ�����еķ����ǣ�ʹpara_sync=enum_para_sync��
//              timoout=cn_timeout_forever��
//              1=mallocһ���ڴ�顣
//      prio,�¼����ȼ�,0��ʾʹ��Ĭ��ֵ(�����¼����ͱ�pg_event_type_table��),
//           �������¼��������ȼ�ֻ����pg_event_type_table�е���ֵ��prio�����趨
//           ���Ǳ����¼���������ȼ���1=�ͣ�0=��
//���أ������ܻ�ȡ�¼����ƿ飬����-1�����򷵻��¼�id��
//ע: �������ܻ���¼����ƿ��������
//-----------------------------------------------------------------------------
u16 djy_event_pop(  u16  evtt_id,
                    enum pop_para_sync para_sync,
                    u32 *pop_result,
                    u32 timeout,    //������������峬ʱʱ�䣬
                                    //ֻ��para_sync=enum_para_sync����Ч
                    void *para,
                    u32 para_size,
                    ufast_t para_options, 
                    ufast_t prio)
{
    struct  event_ECB *pl_ecb;
    struct  event_type *pl_evtt;
    struct  event_ECB *pl_ecb_temp;
    struct  para_record *op_queue=NULL,**para_temp;
    u16 return_result;
    bool_t schbak;          //�Ƿ��������
    bool_t new_ecb = false; //�Ƿ�������µ��¼����ƿ飬���ڳ����˳�ʱ�ͷ���Դ
    bool_t new_para = false;//�Ƿ�������µĲ������ƿ飬���ڳ����˳�ʱ�ͷ���Դ
    bool_t creat_new_para;  //�μ�para_options�Ĳ���˵��
    bool_t malloc32;        //�μ�para_options�Ĳ���˵��
    if(evtt_id >= cn_evtts_limit)
    {
        djy_error_login(enum_knl_evttid_limit,NULL);
        if(pop_result != NULL)
            *pop_result = (ptu32_t)enum_knl_evttid_limit;
        return cn_invalid_event_id;
    }
    schbak = djy_query_sch( );
    if((para_sync == enum_para_sync) && (timeout != 0) && (!schbak))
    {   //Ҫ���¼�������ŷ��أ�����������ȶ��޷�ִ��
        djy_error_login(enum_knl_cant_sched,NULL);
        if(pop_result != NULL)
            *pop_result = (ptu32_t)enum_knl_cant_sched;
        return cn_invalid_event_id;
    }
    pl_evtt =&tg_evtt_table[evtt_id];
    if((pl_evtt->property.registered == 0)      //����δ�Ǽ�
        ||(pl_evtt->property.deleting == 1))    //�¼��������ڵȴ�ע��
    {
        djy_error_login(enum_knl_evtt_unregister,NULL);
        if(pop_result != NULL)
            *pop_result = (u32)enum_knl_evtt_unregister;
        return cn_invalid_event_id;
    }else if(prio >= cn_prio_sys_service)       //�Ƿ����ȼ�
    {
        djy_error_login(enum_knl_invalid_prio,NULL);
        if(pop_result != NULL)
            *pop_result = (u32)enum_knl_invalid_prio;
        return cn_invalid_event_id;
    }

    malloc32 = para_options && cn_malloc_over_32;
    creat_new_para = para_options && cn_creat_new_para;
    int_save_asyn_signal();                     //���첽�ź�(�ص���)
    pl_evtt->pop_sum++;
    //���洦���¼����͵���ͬ�����ο�djy_evtt_pop_sync��˵��
    pl_ecb = pl_evtt->pop_sync;
    if(pl_ecb != NULL)
    {
        do
        {//�����е��¼�����Ҫôû��ָ����ʱ��Ҫôʱ��δ����������������ڴ�����
         //�����ºۼ���evtt_done_syncλҲ�Ѿ����

            //ͬ��������ɵ�����: 1��ͬ���������Ѽ���1��
            if(pl_ecb->sync_counter == 1)
            {
                pl_ecb->last_status.all = pl_ecb->event_status.all;
//                pl_ecb->event_status.bit.evtt_pop_sync = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)  //ָ���ĳ�ʱδ��
                {
                    __djy_resume_delay(pl_ecb);     //�����Ӷ������Ƴ��¼�
//                    pl_ecb->event_status.bit.wait_overtime = 0;
                }
                pl_ecb->event_status.all = 0;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //�����һ���¼�
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next 
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous 
                                        = pl_ecb_temp->multi_previous;
                }
                __djy_event_ready(pl_ecb_temp);
            }else
            {
                pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->pop_sync);
    }

    //��Ҫ�������¼������Ϊ����֮һ:
    //1���¼����������޸������¼���
    //2���������¼�
    if((pl_evtt->property.inuse == 0) || (pl_evtt->property.correlative == 0))
    {
        if((pg_event_free==NULL)||(pg_para_free == NULL)) //û�п��е��¼����ƿ�
        {
            if(pg_event_free==NULL)
            {
                djy_error_login(enum_knl_ecb_exhausted,NULL);
                if(pop_result != NULL)
                    *pop_result = (u32)enum_knl_ecb_exhausted;
            }else
            {
                djy_error_login(enum_knl_pcb_exhausted,NULL);
                if(pop_result != NULL)
                    *pop_result = (u32)enum_knl_pcb_exhausted;
            }
            return_result = cn_invalid_event_id;
            goto end_pop;
        }else if((pl_evtt->property.correlative == 0)
                && ((pl_evtt->pop_sum-pl_evtt->done_sum)>= pl_evtt->vpus_limit))
        {
            djy_error_login(enum_knl_para_over,NULL);
            return_result = cn_invalid_event_id;
            if(pop_result != NULL)
                *pop_result = (u32)enum_knl_para_over;
            goto end_pop;
        }else                       //�п����¼����ƿ�
        {
            pl_ecb = pg_event_free;         //�ӿ�����������ȡһ���¼����ƿ�
            pg_event_free=pg_event_free->next;  //�����¼����ƿ�������1
            op_queue = pg_para_free;
            pg_para_free = pg_para_free->next;
            new_ecb = true;
            new_para = true;
            //�������¼����¼����ƿ�
            pl_ecb->next = NULL;
            pl_ecb->previous = NULL;
            pl_ecb->multi_next = NULL;
            pl_ecb->multi_previous = NULL;
            pl_ecb->vm = NULL;
            pl_ecb->para_high_prio = NULL;
            pl_ecb->para_low_prio = NULL;
            pl_ecb->sync = NULL;
            pl_ecb->sync_head = NULL;
#if(cfg_debug_info == 1)
            pl_ecb->start_time=djy_get_time_tick();   //�¼�����ʱ��
            pl_ecb->consumed_time = 0;
            pl_ecb->consumed_time_second = 0;
            pl_ecb->consumed_time_record = 0;
#endif
            pl_ecb->delay_start_tick = 0;
            pl_ecb->delay_end_tick = 0;
            pl_ecb->error_no = 0;
            pl_ecb->wait_mem_size = 0;
            pl_ecb->last_status.all = 0;
            pl_ecb->event_status.all = 0;
            pl_ecb->evtt_id=evtt_id;          //�����¼�����
            pl_ecb->sync_counter = 0;
    //      pl_ecb->event_id = xxx      //ע:event_id�����������ʼ����
            pl_ecb->local_memory = 0;
            pl_evtt->property.inuse = 1;
            pl_evtt->mark_event = pl_ecb;
            op_queue->start_time = pl_ecb->start_time;
            op_queue->dynamic_mem = false;
            
            if(para_size != 0)
            {
                if(para_size > 32)
                {
                    if(malloc32 == true)    //��̬�����ڴ棬Ȼ��copy����
                    {
                        //��������������������¼��õģ�����ȫ���ڴ�
                        op_queue->para = m_malloc_gbl(para_size,0);
                        if(op_queue->para == NULL)
                        {
                            djy_error_login(enum_knl_memory_over,NULL);
                            return_result = cn_invalid_event_id;
                            if(pop_result != NULL)
                                *pop_result = (u32)enum_knl_memory_over;
                            goto end_pop_save;
                        }else
                        {
                            memcpy(op_queue->para,para,para_size);
                            op_queue->dynamic_mem = true;
                        }
                    }else
                        op_queue->para = para;  //ֱ��ʹ�õ������ṩ�Ļ�������
                                                //����copy����
                }else   //�����ߴ�С��32��ֱ��ʹ�ò������ƿ��Դ���32�ֽڻ�����
                {
                    op_queue->para = op_queue->static_para;
                    memcpy(op_queue->para,para,para_size);
                }
            }
            if(pl_evtt->property.correlative == 0)
            {
                pl_ecb->para_current = op_queue;
                if(prio != 0)
                {
                    pl_ecb->prio = prio;                    //�����¼����ȼ�,
                }else
                    pl_ecb->prio =pl_evtt->default_prio;//���¼������м̳����ȼ�
            }else
            {
                pl_evtt->vpus = 1;          //��ʾ��������һ������
                pl_ecb->para_current = NULL;
                pl_ecb->prio = pl_evtt->default_prio;
                op_queue->next = op_queue;
                op_queue->previous = op_queue;
                if(prio == 0)
                {
                    pl_ecb->para_high_prio = op_queue;
                    pl_ecb->para_low_prio = NULL;
                }else
                {
                    pl_ecb->para_high_prio = NULL;
                    pl_ecb->para_low_prio = op_queue;
                }
            }
            __djy_event_ready(pl_ecb);
        }
        return_result = pl_ecb->event_id;
    }else       //�������¼����Ҷ������Ѿ��и������¼�
    {
        pl_ecb = pl_evtt->mark_event;
        if(prio == 0)
            para_temp = &pl_ecb->para_high_prio;   //�����������Ǹ����ȼ�����
        else
            para_temp = &pl_ecb->para_low_prio;  //�����������ǵ����ȼ�����
        if(creat_new_para == false)     //��ǿ�ƴ����²���
        {
            if(*para_temp == NULL)        //prio��Ӧ�Ķ��������޲���
            {
                //�޿��в������ƿ���߸��¼�������ʹ�õĲ������ƿ��Ѿ��ﵽ����
                if((pg_para_free==NULL) || (pl_evtt->vpus>=pl_evtt->vpus_limit))
                {
                    djy_error_login(enum_knl_pcb_exhausted,NULL);
                    return_result = cn_invalid_event_id;
                    if(pop_result != NULL)
                        *pop_result = (u32)enum_knl_pcb_exhausted;
                    goto end_pop_save;
                }else
                {
                    pl_evtt->vpus++;            //�����еĲ�����������
                    new_para = true;            //����������²���
                    op_queue = pg_para_free;    //�������з���������ƿ�
                    pg_para_free = pg_para_free->next;
                    op_queue->next = op_queue;  //�������ƿ��Գ�˫��ѭ������
                    op_queue->previous = op_queue;
                    op_queue->start_time = djy_get_time_tick();
                    op_queue->dynamic_mem = false;
                    *para_temp = op_queue;      //�²��������¼��Ĳ�������
                }
            }else
            {
                op_queue = (*para_temp)->previous;//�������Ĳ�����ԭ�����е����һ��
            }
        }else                   //�����²���
        {
            //�޿��в������ƿ���߸��¼�������ʹ�õĲ������ƿ��Ѿ��ﵽ����
            if((pg_para_free==NULL) || (pl_evtt->vpus>=pl_evtt->vpus_limit))
            {
                if(pg_para_free==NULL)
                {
                    djy_error_login(enum_knl_pcb_exhausted,NULL);
                    if(pop_result != NULL)
                        *pop_result = (u32)enum_knl_pcb_exhausted;
                }else
                {
                    djy_error_login(enum_knl_para_over,NULL);
                    if(pop_result != NULL)
                        *pop_result = (u32)enum_knl_para_over;
                }
                return_result = cn_invalid_event_id;
                goto end_pop_save;
            }else
            {
                pl_evtt->vpus++;                //�����еĲ�����������
                new_para = true;                //����������²���
                op_queue = pg_para_free;        //�������з���������ƿ�
                pg_para_free = pg_para_free->next;
                if(*para_temp == NULL)          //ԭ���п�
                {
                    op_queue->next = op_queue;
                    op_queue->previous = op_queue;
                    *para_temp = op_queue;
                }else                           //ԭ���зǿգ��²����ӵ����
                {
                    op_queue->next = *para_temp;
                    op_queue->previous = (*para_temp)->previous;
                    (*para_temp)->previous->next = op_queue;
                    (*para_temp)->previous = op_queue;
                }
            }
        }
        if(para_size != 0)
        {
            if(para_size > 32)
            {
                if(malloc32 == true)    //��̬�����ڴ棬Ȼ��copy����
                {
                    //��������������������¼��õģ�����ȫ���ڴ�
                    op_queue->para = m_malloc_gbl(para_size,0);
                    if(op_queue->para == NULL)
                    {
                        djy_error_login(enum_knl_memory_over,NULL);
                        return_result = cn_invalid_event_id;
                        if(pop_result != NULL)
                            *pop_result = (u32)enum_knl_memory_over;
                        goto end_pop_save;
                    }else
                    {
                        memcpy(op_queue->para,para,para_size);
                        op_queue->dynamic_mem = true;
                    }
                }else
                    op_queue->para = para;  //ֱ��ʹ�õ������ṩ�Ļ�������
                                            //����copy����
            }else   //�����ߴ�С��32��ֱ��ʹ�ò������ƿ��Դ���32�ֽڻ�����
            {
                op_queue->para = op_queue->static_para;
                memcpy(op_queue->para,para,para_size);
            }
        }
        return_result = pl_ecb->event_id;
    }

    if(para_sync == enum_para_sync)        //������������
    {
        if(timeout == 0)    //��ʱʱ��Ϊ0����������ʱ����
        {
            if(pop_result != NULL)
                *pop_result = (u32)cn_sync_timeout;
        }else               //�趨�˳�ʱʱ�䣬�����µ���
        {
            __djy_cut_ready_event(pg_event_running);
            pg_event_running->sync_counter = 1;
            
            //���°�pg_event_running���뵽Ŀ���¼���ͬ��������
            pg_event_running->sync_head = &(op_queue->sync);
            op_queue->sync = pg_event_running;
            pg_event_running->multi_next = pg_event_running;
            pg_event_running->multi_previous = pg_event_running;
            
            pg_event_running->last_status.all = 
                    pg_event_running->event_status.all;
            pg_event_running->event_status.bit.event_sync = 1;
            if(timeout != cn_timeout_forever)
            {
                pg_event_running->event_status.bit.wait_overtime =1;
                __djy_addto_delay(timeout);
            }
            int_restore_asyn_signal();  //�ָ��жϻ��������µ���

//ע:�¼���������djy_task_completed�����и�            
            int_save_asyn_signal();
            //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
            if(pg_event_running->event_status.bit.event_sync)
            {//˵��ͬ������δ�����ӳ�ʱ���ء�
                pg_event_running->event_status.bit.event_sync = 0;
                if(pop_result != NULL)
                    *pop_result = (u32)cn_sync_timeout;
            }else if(pg_event_running->event_status.bit.event_sync_exit)
            {//��ͬ���¼����쳣��ֹ������û�б���ȷִ��
                pg_event_running->event_status.bit.event_sync_exit = 0;
                if(pop_result != NULL)
                    *pop_result = (u32)enum_knl_event_sync_exit;
            }else
            {
                if(pop_result != NULL)
                    *pop_result = (u32)cn_sync_success;
            }
        }
    }
    goto end_pop;

end_pop_save:
    if(new_ecb == true)
    {
        pl_ecb->next = pg_event_free;//�ͷ�pl_ecb
        pg_event_free = pl_ecb;
        pl_ecb->previous = (struct  event_ECB*)&pg_event_free;
    }
    if(new_para == true)
    {
        op_queue->next = pg_para_free;//�ͷ�op_queue
        pg_para_free = op_queue;
        
    }
end_pop:
    int_restore_asyn_signal();  //�ָ��ж�״̬
    return return_result;
}

//----ȡ�¼�������-----------------------------------------------------------
//����: һ���¼��ڴ�������У�������������¼������ҵȴ��¼�������(����
//      djy_event_pop������sync=true)���������djy_event_popʱ�����˺Ϸ����¼�
//      id���Ҳ��ǳ�ʱ���أ�������ñ�������ȡ���¼��Ĵ�������ֻ��ȡ���һ��
//      �ɹ�������¼������
//����: ��
//����: �¼���������
//-----------------------------------------------------------------------------
ptu32_t djy_get_event_result(void)
{
    return pg_event_running->event_result;
}

//----��ȡ�¼�����-------------------------------------------------------------
//����: ��ȡ�������¼������¼��Ĳ���������˳��:1�������ȼ��Ķ��зǿգ�ȡ��һ��
//      �������Ӷ�����ɾ����2��pparaָ��ǿ�,ȡ��ֵ����ա�3�������ȼ����зǿգ�
//      ȡ��һ�����Ӷ���ɾ��
//����: time�������NULL���򷵻�����ȡ�Ĳ���������е�ʱ�䣬ticks��
//����: �¼�����ָ��
//ע: ������ֻ��ȡ���ڴ����е��¼��Լ��Ĳ���
//-----------------------------------------------------------------------------
void *djy_get_event_para(u32 *time)
{
    struct para_record *sub;
    void *result;
    atom_low_t  atom_bak;
    atom_bak = int_low_atom_start();
    if(pg_event_running->para_current != NULL)
    {
        result = (void*)pg_event_running->para_current->para;
    }else if(pg_event_running->para_high_prio != NULL) //�����ȼ����¼����зǿ�
    {
        sub = pg_event_running->para_high_prio;  //ȡ�����ȼ����¼�����
        if(sub->next == sub)                    //������ֻ��һ�����¼�
        {
            pg_event_running->para_high_prio = NULL;
        }else                                   //�����г���һ�����¼�
        {
            sub->next->previous = sub->previous;
            sub->previous->next = sub->next;
            pg_event_running->para_high_prio = sub->next;
        }
        pg_event_running->para_current = sub;
        result = (void*)sub->para;
    }else if(pg_event_running->para_low_prio != NULL)//�����ȼ����¼����зǿ�
    {
        sub = pg_event_running->para_low_prio;   //ȡ�����ȼ����¼�����
        if(sub->next == sub)                    //������ֻ��һ�����¼�
        {
            pg_event_running->para_low_prio = NULL;
        }else                                   //�����г���һ�����¼�
        {
            sub->next->previous = sub->previous;
            sub->previous->next = sub->next;
            pg_event_running->para_low_prio = sub->next;
        }
        pg_event_running->para_current = sub;
        result = (void*)sub->para;
    }else
    {
        result = NULL;
    }
    int_low_atom_end(atom_bak);
    if((result != NULL) && (time != NULL))
        *time = pg_event_running->para_current->start_time;
    return result;
}

//----ȡ������¼�����id-------------------------------------------------------
//����: ��Ӧ�ó�����ã�ȡ���ڴ�����¼����¼�����id
//����: ��
//����: �¼�����id
//-----------------------------------------------------------------------------
u16 djy_my_evtt_id(void)
{
    return pg_event_running->evtt_id;
}

//----��������ϵͳ--------------------------------------------------------------
//����: ��ʼ��ʱ����ણ���λ�첽�ź�ʹ��״̬��ѡ��������ȼ��¼�������
//      __asm_turnto_context���������л�������¼�
//����: ��
//����: ��
//_____________________________________________________________________________
void __djy_start_os(void)
{
    //����Ϊ�ݴ����ʣ��Է��û�ģ���ʼ��������û�гɶԵ����첽�ź�ʹ�����ֹ����
    __int_reset_asyn_signal();
    __djy_init_tick();
    __djy_select_event_to_run();
    pg_event_running = pg_event_ready;
    bg_schedule_enable = true;
    __asm_start_thread(pg_event_running->vm);
}

//----�쳣�����¼���ͬ������---------------------------------------------------
//����: �¼��쳣����ʱ������һ���¼�ͬ�������е������¼�������
//      ��ͬ���е��¼���"ִ�н��"����Ϊ0����ͬ���¼��쳣����λ����λ
//����: para���������еĶ���ͷ����������һ��˫������
//����: ��
//-----------------------------------------------------------------------------
void __djy_active_eventsync_exp(struct event_ECB *sync)
{
    struct event_ECB *event_temp;
    if(sync == NULL)
        return ;

    sync->multi_previous->multi_next = NULL;
    do{
        sync->last_status.all =sync->event_status.all;  //���浱ǰ״̬
//        sync->event_status.bit.event_sync = 0;          //ȡ��"ͬ����"״̬
//        sync->event_status.bit.event_sync_exit = 1;     //��λ�쳣������־
        sync->event_result = 0;
        sync->sync_head = NULL;
        if(sync->event_status.bit.wait_overtime)        //�Ƿ��ڳ�ʱ������
        {
            __djy_resume_delay(sync);                   //������ʱ�ȴ�
//            sync->event_status.bit.wait_overtime =0;    //ȡ��"��ʱ�ȴ���"״̬
        }
        sync->event_status.all = 0;                     //ȡ��"ͬ����"״̬
        sync->event_status.bit.event_sync_exit = 1;     //��λ�쳣������־
        event_temp = sync->multi_next;
        __djy_event_ready(sync);           //���¼����뵽����������
        sync = event_temp;
    }while(sync != NULL);
    return;
}

//----�쳣�����¼�������ͬ������-----------------------------------------------
//����: �¼��쳣����ʱ������һ���¼����в�����ͬ�������е������¼������ͷŲ�����
//      �ƿ�.���ظ�ͬ���е��¼���"ִ�н��"����Ϊ0����ͬ���¼��쳣����λ����λ
//����: para���������еĶ���ͷ����������һ��˫������
//����: ��
//-----------------------------------------------------------------------------
void __djy_active_parasync_exp(struct para_record *para)
{
    struct para_record *para_temp;
    struct event_ECB *pl_ecb,*event_temp;
    if(para == NULL)
        return ;

    para->previous->next = NULL;
    do{
        pl_ecb = para->sync;
        if(pl_ecb != NULL)
        {
            pl_ecb->multi_previous->multi_next = NULL;
            do{
                pl_ecb->last_status.all =pl_ecb->event_status.all;//���浱ǰ״̬
//                pl_ecb->event_status.bit.event_sync = 0;     //ȡ��"ͬ����"״̬
//                pl_ecb->event_status.bit.event_sync_exit = 1;//��λ�쳣������־
                pl_ecb->event_result = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)   //�Ƿ��ڳ�ʱ������
                {
                    __djy_resume_delay(pl_ecb);               //������ʱ�ȴ�
//                    pl_ecb->event_status.bit.wait_overtime =0;//ȡ��"��ʱ�ȴ���"״̬
                }
                pl_ecb->event_status.all = 0;     //ȡ��"ͬ����"״̬
                pl_ecb->event_status.bit.event_sync_exit = 1;//��λ�쳣������־
                event_temp = pl_ecb->multi_next;
                __djy_event_ready(pl_ecb);           //���¼����뵽����������
                pl_ecb = event_temp;
            }while(pl_ecb != NULL);
        }

        if(para->dynamic_mem == true)
        {
            m_free(para->para);
        }
        para_temp = para;
        para = para->next;                  //ȡ��������һ���¼�
        para_temp->sync = NULL;
        para_temp->next = pg_para_free;     //�ͷŲ������ƿ�
        pg_para_free = para_temp;
    }while(para != NULL);
    return;
}

#define cn_deliver_to   0   //������Ѿ�ת��
#define cn_keep         1   //�����������ɾ��
#define cn_delete       2   //�����Ӧ�ñ�ɾ��
#define cn_deleted      3   //������Ѿ���ɾ��

//----�¼��ݳ�-----------------------------------------------------------------
//����: �¼���������Ȼ����,����Ϊ������djy_event_completed,���ڶ������¼�����
//      ����������һ�µģ������ڹ����ͣ���������ͬ��djy_event_completed�������
//      �����¼�������ɾ����Ҳ������߳����ڳ�ʼ̬��djy_event_exit���ɾ���¼���
//      �����߳����³�ʼ����
//����: ��
//����: ������������
//-----------------------------------------------------------------------------
void djy_event_exit(void)
{
    struct thread_vm *next_vm,*temp;
    struct event_ECB *pl_ecb;
    struct event_type   *pl_evtt;
    struct event_ECB *pl_ecb_temp;
    ucpu_t  vm_final = cn_delete;

    //�˴�����int_save_asyn_signal������������Ӧ�ó�����bug��û�гɶԵ���
    //int_save_asyn_signal��int_restore_asyn_signal������£�ȷ�����󵽴�Ϊֹ��
    __int_reset_asyn_signal();  //ֱ��__vm_engine�������ٴδ�.

    //�����¼�ͬ�������е������¼�
    __djy_active_eventsync_exp(pg_event_running->sync);

    if(pg_event_running->para_current != NULL)
        __djy_active_eventsync_exp(pg_event_running->para_current->sync);
    //���¼��������ͬ�������е��¼�,���ͷŲ��������е�ȫ������
    __djy_active_parasync_exp(pg_event_running->para_high_prio);
    __djy_active_parasync_exp(pg_event_running->para_low_prio);

#if (cfg_dynamic_mem == 1)
    if(pg_event_running->local_memory != 0)
    {
        __m_cleanup(pg_event_running->event_id);    //
    }
#endif
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->previous
                    = (struct  event_ECB*)&pg_event_free;//��ʾ�����ƿ����
    pg_event_running->next = pg_event_free;     //pg_event_free�ǵ����ѭ������
    pg_event_free = pg_event_running;
    pg_event_free->event_id = pg_event_free - tg_ECB_table;   //�ݴ���

    //���洦�����ͬ�����У�������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ���
    //��ô����ġ�
    pl_evtt =&tg_evtt_table[pg_event_running->evtt_id];
    pl_evtt->done_sum++;
    pl_ecb = pl_evtt->done_sync;
    if(pl_ecb != NULL)
    {
        do
        {//�����е��¼�����Ҫôû��ָ����ʱ��Ҫôʱ��δ����������������ڴ�����
         //�����ºۼ���evtt_done_syncλҲ�Ѿ����

            //ͬ��������ɵ�����: 1��ͬ���������Ѽ���1��
            //2��ͬ��������Ϊ0�ұ��������һ���¼��Ѿ�������
            if((pl_ecb->sync_counter == 1)
                    ||((pl_ecb->sync_counter == 0) 
                            &&(pl_evtt->pop_sum == pl_evtt->done_sum)))
            {
                pl_ecb->last_status.all = pl_ecb->event_status.all;
//                pl_ecb->event_status.bit.evtt_done_sync = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)  //ָ���ĳ�ʱδ��
                {
                    __djy_resume_delay(pl_ecb);       //�����Ӷ������Ƴ��¼�
//                    pl_ecb->event_status.bit.wait_overtime = 0;
                }
                pl_ecb->event_status.all = 0;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //�����һ���¼�
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next 
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous 
                                        = pl_ecb_temp->multi_previous;
                }
                __djy_event_ready(pl_ecb_temp);
            }else
            {
                if(pl_ecb->sync_counter != 0)
                    pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->done_sync);
    }

    //�̺߳��¼����ƿ�Ĵ���ʽ
    //1�����߳�����С�ڵ���vpus_res������.
    //2����������������߳�
    //3���¼����ƿ���ֱ���ܻ�

    if(pl_evtt->property.correlative == 1)      //�������¼�
    {
        vm_final = cn_keep;
        pl_evtt->vpus =0;
    }else   //�������¼�
    {
        //��δ�õ���������¼�������֮
        if((pl_evtt->pop_sum - pl_evtt->done_sum) > pl_evtt->vpus)
        {
            vm_final = cn_keep;
        }else   //û��δ�õ���������¼����ٿ��Ƿ��豣��
        {
            if(pl_evtt->vpus<=pl_evtt->vpus_res)//�������¼�ӵ�е��߳����Ѿ�����
                                                    //�������ͱ�����������֮
            {
                vm_final = cn_keep;
            }else                                   //����ɾ���߳�
            {
                if(pl_evtt->vpus == 1)   //�������һ���¼�
                {
                    pl_evtt->property.inuse = 0;
                    if((cn_run_mode!=cn_mode_mp)||(pl_evtt->default_prio<0x80))
                    {
                 //����ģʽΪsi��dlsp������¼�����ӵ�н������ȼ�ʱ���뱣���߳�
                        vm_final = cn_keep;
                    }else
                    {
                        vm_final = cn_delete;
                    }
                }else
                {
                    vm_final = cn_delete;
                }
            }
        }
    }

//���¼����Ϳ��ƿ��Ƿ�Ҫɾ�����û�ע��һ���¼����ͣ�������¼���������ʹ�ã���
//������ע����ֻ�ǲ��ܵ������¼���Ҫ�����ڸ��¼����͵������¼����������ע����
    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //�����¼����Ϳ��ƿ飬ֻ���registered�������㡣
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';   //����¼�������
        while(next_vm != NULL)          //�ͷŸ��¼�����ӵ�еĿ����߳�
        {
            temp = next_vm;
            next_vm = next_vm->next;
            m_free((void *)temp);
        }
        vm_final = cn_deleted;          //�߳��Ѿ���ɾ��
    }

    __djy_select_event_to_run();
    if(vm_final == cn_delete)                   //ɾ�������
    {
        m_free((void*)pg_event_running->vm);    //ɾ�������
        pl_evtt->vpus--;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_turnto_context(pg_event_running->vm);
    }else if(vm_final == cn_keep)    //���������
    {
        pg_event_running->vm->next = pl_evtt->my_free_vm;
        pl_evtt->my_free_vm = pg_event_running->vm;
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }else               //������Ѿ�ת������һ���¼�,���Ѿ���ɾ��
    {
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }
}
//----����¼�------------------------------------------------------------------
//���ܣ������ϵͳ�����¼��Ѿ����,����ϵͳ�ӵ������,���������.
//      1.����¼�ͬ�����зǿգ���ͬ���¼��ŵ�ready��
//      2.��������豸���ͷ�֮�����Ѹ��豸�ȴ������еĵ�һ���¼��ŵ�ready��
//      3.���ø��¼����Ϳ��ƿ�Ĳ�����
//      4.���¼�����������õ���ʼ̬,������free��
//      5.����Ƿ�ʵʱ�����,�ͷŶ�̬������ڴ�(�����)
//  ������߳��豣������:
//      6.��λ�������
//      7.�����������ģ�ֱ���л����µ�ready��ͷ
//  ����߳����豣��
//      6.�ͷ�������ڴ档
//      7.�����������ģ�ֱ���л����µ�ready��ͷ
//������result���¼��������������������ظ��������¼����¼�(����趨��ͬ��)
//���أ���
//��ע: ���ñ������ıض���running�¼�,��running�¼���������ִ�У������Ե���
//      __asm_reset_thread������
//-----------------------------------------------------------------------------
void djy_task_completed(ptu32_t result)
{
    struct thread_vm *next_vm,*temp;
    struct event_ECB *pl_ecb,*event_temp;
    struct  event_type   *pl_evtt;
    struct  event_ECB *pl_ecb_temp;
    struct para_record *cur_para;
    ucpu_t  vm_final = cn_delete;

    //�˴�����int_save_asyn_signal������������Ӧ�ó�����bug��û�гɶԵ���
    //int_save_asyn_signal��int_restore_asyn_signal������£�ȷ�����󵽴�Ϊֹ��
    pl_evtt =&tg_evtt_table[pg_event_running->evtt_id];
    if(pl_evtt->property.correlative == enum_correlative)      //�������¼�
    {
        int_save_asyn_signal();
    }
    else
    {
        __int_reset_asyn_signal();  //ֱ��__vm_engine�������ٴδ�.
    }
    //���洦��ͬ�����У�������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ���
    //��ô����ġ�
    pl_ecb = pg_event_running->sync;     //ȡͬ������ͷ
    if(pl_ecb != NULL)
    {
        pl_ecb->multi_previous->multi_next = NULL;
        do{
            pl_ecb->last_status.all = pl_ecb->event_status.all;  //���浱ǰ״̬
//            pl_ecb->event_status.bit.event_sync = 0;          //ȡ��"ͬ����"״̬
            pl_ecb->sync_head = NULL;
            pl_ecb->event_result = result;
            if(pl_ecb->event_status.bit.wait_overtime)        //�Ƿ��ڳ�ʱ������
            {
                __djy_resume_delay(pl_ecb);                   //������ʱ�ȴ�
//                pl_ecb->event_status.bit.wait_overtime =0;//ȡ��"��ʱ�ȴ���"״̬
            }
            pl_ecb->event_status.all = 0;
            event_temp = pl_ecb->multi_next;
            __djy_event_ready(pl_ecb);           //���¼����뵽����������
            pl_ecb = event_temp;
        }while(pl_ecb != NULL);
    }
    cur_para = pg_event_running->para_current;
    if(cur_para != NULL)
    {
        pl_ecb = cur_para->sync;
        if(pl_ecb != NULL)
        {
            pl_ecb->multi_previous->multi_next = NULL;
            do{
                pl_ecb->last_status.all =pl_ecb->event_status.all;//���浱ǰ״̬
    //            pl_ecb->event_status.bit.event_sync = 0;     //ȡ��"ͬ����"״̬
                pl_ecb->sync_head = NULL;
                pl_ecb->event_result = result;
                if(pl_ecb->event_status.bit.wait_overtime)   //�Ƿ��ڳ�ʱ������
                {
                    __djy_resume_delay(pl_ecb);               //������ʱ�ȴ�
    //                pl_ecb->event_status.bit.wait_overtime =0;//ȡ��"��ʱ�ȴ���"״̬
                }
                pl_ecb->event_status.all =0;
                event_temp = pl_ecb->multi_next;
                __djy_event_ready(pl_ecb);           //���¼����뵽����������
                pl_ecb = event_temp;
            }while(pl_ecb != NULL);
        }
        
        if(cur_para->dynamic_mem == true)
        {
            m_free(cur_para->para);
        }
        cur_para->next = pg_para_free;
        pg_para_free = cur_para;
        cur_para->sync = NULL;
        pg_event_running->para_current = NULL;
    }
    //���洦�����ͬ�����У�������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ���
    //��ô����ġ�
    pl_evtt->done_sum++;
    pl_ecb = pl_evtt->done_sync;
    while(pl_ecb != NULL)
    {
        do
        {//�����е��¼�����Ҫôû��ָ����ʱ��Ҫôʱ��δ����������������ڴ�����
         //�����ºۼ���evtt_done_syncλҲ�Ѿ����

            //ͬ��������ɵ�����: 1��ͬ���������Ѽ���1��
            //2��ͬ��������Ϊ0�ұ��������һ���¼��Ѿ�������
         if((pl_ecb->sync_counter == 1)
                 ||((pl_ecb->sync_counter == 0) 
                         &&(pl_evtt->pop_sum == pl_evtt->done_sum)))
            {
                pl_ecb->last_status.all = pl_ecb->event_status.all;
//                pl_ecb->event_status.bit.evtt_done_sync = 0;
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status.bit.wait_overtime)  //ָ���ĳ�ʱδ��
                {
                    __djy_resume_delay(pl_ecb);       //�����Ӷ������Ƴ��¼�
//                    pl_ecb->event_status.bit.wait_overtime = 0;
                }
                pl_ecb->event_status.all = 0;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //�����һ���¼�
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next 
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous 
                                        = pl_ecb_temp->multi_previous;
                }
                __djy_event_ready(pl_ecb_temp);
            }else
            {
                if(pl_ecb->sync_counter != 0)
                    pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->done_sync);
    }

    //���¿��¼����������δ���
    if(pl_evtt->property.correlative == enum_correlative)      //�������¼�
    {
        if(pl_evtt->vpus != 0)              //��Ӧ�ó��򷴸����ñ�����
            pl_evtt->vpus--;
        int_restore_asyn_signal();
        return;
    }else   //�������¼�
    {
#if (cfg_dynamic_mem == 1)
        if(pg_event_running->local_memory != 0)
        {
            __m_cleanup(pg_event_running->event_id);//ǿ������¼�������������ڴ�
        }
#endif        
        __djy_cut_ready_event(pg_event_running);
        pg_event_running->previous
                        = (struct  event_ECB*)&pg_event_free;//��ʾ�����ƿ����
        pg_event_running->next = pg_event_free;     //pg_event_free�ǵ����ѭ������
        pg_event_free = pg_event_running;
        pg_event_free->event_id = pg_event_free - tg_ECB_table;   //�ݴ���

        //��δ�õ���������¼�������֮
        if((pl_evtt->pop_sum - pl_evtt->done_sum) > pl_evtt->vpus)    
        {
            vm_final = cn_keep;
        }else   //û��δ�õ���������¼����ٿ��Ƿ��豣��
        {
            if(pl_evtt->vpus == 1)   //�������һ���¼�
            {
                pl_evtt->property.inuse = 0;
                if((cn_run_mode!=cn_mode_mp)||(pl_evtt->default_prio<0x80))
                {//����ģʽΪsi��dlsp�����߸��¼�����ӵ�н������ȼ�ʱ��
                 //�뱣���߳������
                    vm_final = cn_keep;
                }else
                {
                    vm_final = cn_delete;
                }
            }else                       //�ⲻ�����һ���¼�
            {
                vm_final = cn_delete;
            }
        }
    }

    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //�����¼����Ϳ��ƿ飬ֻ���registered�������㡣
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';   //����¼�������
        while(next_vm != NULL)          //�ͷŸ��¼�����ӵ�еĿ��������
        {
            temp = next_vm;
            next_vm = next_vm->next;
            m_free((void *)temp);
        }
        vm_final = cn_deleted;
    }

    __djy_select_event_to_run();
    if(vm_final == cn_delete)      //ɾ�������
    {
        m_free((void*)pg_event_running->vm);    //ɾ�������
        pl_evtt->vpus--;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_turnto_context(pg_event_running->vm);
    }else if(vm_final == cn_keep)    //���������
    {
        pg_event_running->vm->next = pl_evtt->my_free_vm;
        pl_evtt->my_free_vm = pg_event_running->vm;
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }else               //������Ѿ�ת������һ���¼�,���Ѿ���ɾ��
    {
        pl_ecb = pg_event_running;
        pg_event_running = pg_event_ready;
        u32g_running_start_time = djy_get_fine_time();
        __asm_reset_switch(pl_evtt->thread_routine,
                        pg_event_running->vm,pl_ecb->vm);
    }
}

//----��ѯ����ԭ��-------------------------------------------------------------
//����: ��ѯ����ִ�е��¼���ִ�е�ԭ��.
//����: ��
//����: ����ԭ��,��Դ����ע��.
//-----------------------------------------------------------------------------
union event_status djy_wakeup_from(void)
{
    return pg_event_running->last_status;
}

//----��ѯ�¼�id-------------------------------------------------------------
//����: �����ṩ��id�Ų�ѯ�¼�ָ��.
//����: id,�¼�id
//����: �¼�id��Ӧ���¼�ָ��.
//-----------------------------------------------------------------------------
struct  event_ECB *__djy_lookup_id(u16 id)
{
    return &tg_ECB_table[id];
}
//----���������---------------------------------------------------------------
//����: ���������,ִ���������ں���,�¼���ɺ�ִ��������
//����: thread_routine ������¼����͵ĺ���ָ��.
//����: ��
//-----------------------------------------------------------------------------
void __djy_vm_engine(void (*thread_routine)(void))
{
//    int_restore_asyn_signal();   //��Ӧ��y_event_done�����йرյ��ж�
    thread_routine();   //�¼�������
    //���thread_routine��δִ��y_event_done����ֱ�ӷ��أ�����������ã�
    //���������,Ȼ��ֱ���л����µ�ready�¼�,��������������.
    djy_event_exit();
}

//----ϵͳ����-----------------------------------------------------------------
//����: ����ϵͳ��ϵͳ������,�ṩ����,ͳ�Ƶȷ���.
//����: ��
//����: ��������.
//��ʹ��ѵ:����һ�������������ĺ�������ҹ3��������Ľ�ѵ��
//-----------------------------------------------------------------------------
void __djy_service(void)
{
    u32 i,j;
    volatile ptu32_t level;
    u32 *stack;
    u32 pads;
    
    pads = ((u32)cfg_stack_fill<<24)
            |((u32)cfg_stack_fill<<16)
            |((u32)cfg_stack_fill<<8)
            |(u32)cfg_stack_fill;
    while(1)
    {
        for(i = 0; i<cn_events_limit; i++)
        {
            if(tg_ECB_table[i].vm == NULL)
                continue;
            level = tg_ECB_table[i].vm->stack_size>>6;
            if((tg_ECB_table[i].vm->stack - (u32*)(tg_ECB_table[i].vm)) < level)
                level = 0;
            stack = (u32*)(tg_ECB_table[i].vm);
            stack = (u32*)(&(tg_ECB_table[i].vm[1]));
            for(j = 0; j < level; j++)
            {
                if(stack[j] != pads)
                    level = 0;
            }
        }
    }
}

//----api��������--------------------------------------------------------------
//����: ����api�ŵ�����Ӧ��api����.
//����: api_no,api��
//����: mpģʽ���ã����ҷ�������
//-----------------------------------------------------------------------------
void djy_api_start(u32 api_no)
{
    switch(api_no)
    {
        default :break;
    }
    return;
}
//�պ���,δ��ʼ���ĺ���ָ��ָ��.
void NULL_func(void)
{}

