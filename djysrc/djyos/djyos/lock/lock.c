//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ��ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �����ź����ͻ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
static struct semaphore_LCB *pg_semaphore_rsc=NULL;
static struct mutex_LCB  *pg_mutex_rsc=NULL;
//˵���� cn_locks_limit ���û����õģ������û�����֪������ϵͳ��Ҫ�ö����ź�����
//      ���Բ���ϵͳ����ռ�� cn_locks_limit ָ�꣬�û�ʹ�õ��ź�����
//      tg_semp_pool������ڴ���з��䣬����ϵͳʹ�õ��ź����Լ����壬����
//      ��أ�����û���ͼ���� semp_delete ɾ������ϵͳ������ź��������������
//      �ڴ�й©��������ز���ϵͳ��ԴҲ��ʧȥ�ź����������������Ԥ��
//�����ź����ṹ��ʼ�ڴ��
#if(cn_locks_limit != 0)
static union lock_MCB tg_lock_mem_block[cn_locks_limit];
#endif
static struct mem_cell_pool *pg_lock_pool;  //�ź����ṹ�ڴ��ͷָ��

//������rscģ���ж���ģ�ר��lockģ��ʹ�õ�һ������
struct  rsc_node * __rsc_add_lock_tree(struct  rsc_node *node,
                                u32 size,char *name);

//----��ʼ����ģ��ģ��step1----------------------------------------------------
//���ܣ���ʼ���ź���ģ��ĵ�һ�����˺���Ե��ó�semp_create��mutex_createe�����
//      ��������������__semp_create_knl��__mutex_createe_knl��������������ֹ����
//      Ϊmodule_init_memb��û�е��ã�����mudule_init_memb��Ҫʹ���ź�������ֻ��
//      ��module_init_lock1������á�
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t module_init_lock1(ptu32_t para)
{
    static struct semaphore_LCB semp_root;
    static struct mutex_LCB mutex_root;
    pg_semaphore_rsc = (struct semaphore_LCB*)
        __rsc_add_lock_tree(&semp_root.node,sizeof(union lock_MCB),"semaphore");
    pg_mutex_rsc = (struct mutex_LCB*)
        __rsc_add_lock_tree(&mutex_root.node,sizeof(union lock_MCB),"mutex");
    return 1;
}

//----��ʼ����ģ��ģ��step2----------------------------------------------------
//���ܣ���ʼ���ź���ģ��ĵڶ������˺���Ե��ú�semp_create��mutex_createe���ڵ�
//      ������������
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t module_init_lock2(ptu32_t para)
{
    static struct mem_cell_pool lock_pool;
    //��ʼ�������ƿ��ڴ��
#if(cn_locks_limit != 0)
    pg_lock_pool = mb_create_pool_knl( &lock_pool,
                                    (void*)tg_lock_mem_block,
                                    cn_locks_limit,
                                    sizeof(union lock_MCB),
                                    0,0,
                                    "�����ƿ��");
#else
    pg_lock_pool = mb_create_pool_knl( &lock_pool,
                                    NULL,
                                    cn_locks_limit,
                                    sizeof(union lock_MCB),
                                    0,0,
                                    "�����ƿ��");
#endif
    return 1;
}

//----����һ���ź���-----------------------------------------------------------
//���ܣ�����һ���ź������Ժ�Ϳ���ʹ������ź����ˡ�
//������semp_limit,�źŵƵ�������cn_limit_uint32=�����޶���źŵơ�
//      init_lamp,��ʼ�źŵ�����
//      name���ź��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct semaphore_LCB *semp_create(u32 lamps_limit,
                                 u32 init_lamp,char *name)
{
    struct semaphore_LCB *semp;
    if(init_lamp > lamps_limit)
        return NULL;
    semp = mb_malloc(pg_lock_pool,0);
    if(semp == NULL)
        return NULL;
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->lamp_used = 0;
    semp->semp_sync = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    rsc_add_son(&pg_semaphore_rsc->node,&semp->node,
                        sizeof(struct semaphore_LCB),name);
    return semp;
}

//----����һ���ں��ź���-------------------------------------------------------
//���ܣ�����һ���ں��ź�����������_knlΪ��׺�ĺ���һ����cn_locks_limit���û�����
//      �ģ�Ϊ����ģ��Ķ����ԣ��ں˲����û�����cn_locks_limitָ�꣬���ں�ʹ�õ�
//      ��(�ź����ͻ�����)�õ����ĺ���������ɾ�������õĻ����������ں˺�Ӧ�ó���
//      �������
//      ��semp_create�������ǣ���������Ҫ�������ṩsempʵ�壬�����Ǵӳ��з��䡣
//������semp,Ŀ���ź���ָ��
//      semp_limit,�źŵƵ�������cn_limit_uint32=�����޶���źŵơ�
//      init_lamp,��ʼ�źŵ�����
//      name���ź��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
void semp_create_knl( struct semaphore_LCB *semp,
                       u32 lamps_limit,u32 init_lamp,char *name)
{
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->lamp_used = 0;
    semp->semp_sync = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    rsc_add_son(&pg_semaphore_rsc->node,&semp->node,
                        sizeof(struct semaphore_LCB),name);
}

//----����һ���źŵ�------------------------------------------------------------
//���ܣ��ͷ�һ���ź����������ź�����1��
//������semp,�ź���ָ��
//���أ���
//-----------------------------------------------------------------------------
void semp_post(struct semaphore_LCB *semp)
{
    struct  event_ECB *event;

    if(semp == NULL)
        return;
    int_save_asyn_signal();
    semp->lamp_used--;
    if(semp->semp_sync == NULL)     //�ȴ����пգ�����һյ�źŵ�
    {
        if(semp->lamps_limit == cn_limit_uint32)        //�źŵ���������
        {
            if(semp->lamp_counter != cn_limit_uint32)   //�źŵ�����δ������
                semp->lamp_counter ++;                  //����һյ�źŵ�
        }else if(semp->lamp_counter < semp->lamps_limit )//�źŵ�����δ������
            semp->lamp_counter++;                       //����һյ�źŵ�
//        else
//            djy_error_login(enum_lock_range_error,semp->node.name);
    } else      //���¼��ڵȴ������źŵ�ֱ��ת�������������ȼ���ߵ��¼�
    {
        event = semp->semp_sync;
        event->sync_head = NULL;
        if(event->multi_next == event)
        {//������ֻ��һ���¼�
            semp->semp_sync = NULL;
        }else
        {//�������ж���¼���ȡ������ͷ�����¼�
            semp->semp_sync = event->multi_next;
            event->multi_next->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = event->multi_next;
            event->multi_next = NULL;
            event->multi_previous = NULL;
        }
        event->last_status.all = event->event_status.all;//�����¼�״̬
        if(event->event_status.bit.wait_overtime)
            __djy_resume_delay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
        event->event_status.all = 0;
//        event->event_status.bit.wait_semp = 0; //����ȴ�״̬
        __djy_event_ready(event);
    }
    int_restore_asyn_signal();
}

//----����һյ�źŵ�-----------------------------------------------------------
//���ܣ�����Ȼ��Ϩ��һյ�źŵƣ���ʾ������Դ����1��
//������semp,�ź���ָ��
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�true=ȡ���źŷ��ػ���Դ�����źŵƱ���(semp == NULL)��semp==NULLʱ����
//      true��������ģ������ϣ�������ڲ����Ƿ����źŵƱ���������һ�£���semp
//      ��ΪNULL�����ǵ�ѡ�񣬱�����Ҫ����һ�������źŵƱ������豸���ɰѸ��豸
//      struct pan_device��semp��Ա��ΪNULL��
//      false=û��ȡ���ź�(��ʱ���ػ�����ԭ��)
//-----------------------------------------------------------------------------
bool_t semp_pend(struct semaphore_LCB *semp,u32 timeout)
{
    struct  event_ECB *event;
    atom_low_t atom_low;
    bool_t lamp;

    if(semp == NULL)
        return true;
    //���¶�ȡ�źŵ����������뱣֤ԭ�Ӳ������ʹ��ж�
    atom_low = int_low_atom_start(  );             //�������Բ���Ƕ�׵���
    semp->lamp_used ++;
    if(semp->lamps_limit == cn_limit_uint32)   //���ź��������޶��źŵ�
    {
        if(semp->lamp_counter != 0) //�źŵ�������Ϊ0
            semp->lamp_counter--;   //Ϩ��һյ�źŵ�
        lamp = true;
    }else                           //���ź������źŵ���������
    {
        if(semp->lamp_counter > 0)  //�е������źŵ�
        {
            lamp = true;            //�����Դ����
            semp->lamp_counter--;   //Ϩ��һյ�źŵ�
        }else                       //û�е������źŵ�
            lamp = false;           //�����Դ������
    }
    int_low_atom_end( atom_low );

    if(lamp == true)
        return true;    //ȡ���źŵƷ���
    else if(!djy_query_sch() || (timeout == 0))//��ֹ����ʱ,��������.��timeoutΪ0
        return false;   //û��ȡ���źŵƷ���

    int_save_asyn_signal();
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->previous = NULL;
    pg_event_running->next = NULL;

    pg_event_running->sync_head = &semp->semp_sync;
    if(semp->semp_sync == NULL)
    {//ͬ�����п�,running�¼��Գ�˫��ѭ������
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
        semp->semp_sync = pg_event_running;
    }else
    {
        event = semp->semp_sync;
        if(semp->sync_order == sort_prio)   //ͬ�����а����ȼ�����
        {
            do
            { //�ҵ�һ�����ȼ��������¼����¼�.
                if(event->prio <= pg_event_running->prio)
                    event = event->multi_next;
                else
                    break;
            }while(event != semp->semp_sync);
            pg_event_running->multi_next = event;
            pg_event_running->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pg_event_running;
            event->multi_previous = pg_event_running;
            if(semp->semp_sync->prio > pg_event_running->prio)
                semp->semp_sync = semp->semp_sync->multi_previous;
        }else                               //���Ⱥ�˳�����¼�ֱ�����ڶ�β
        {
            pg_event_running->multi_next = event;
            pg_event_running->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pg_event_running;
            event->multi_previous = pg_event_running;
        }
    }
    pg_event_running->last_status.all =pg_event_running->event_status.all;
    pg_event_running->event_status.bit.wait_semp = 1;  //�¼�״̬��Ϊ�ȴ��ź���
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    int_restore_asyn_signal();  //�ָ��жϣ��������������л�
    int_save_asyn_signal();     //��ʱ���Ѿ������»���ź������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(pg_event_running->event_status.bit.wait_semp)
    {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ��Ŀ���¼���ͬ��������ȡ���¼���
     //��ʱ����ͬ�����¼��϶���û����ɡ�
        pg_event_running->event_status.bit.wait_semp = 0;
        int_restore_asyn_signal();
        return false;
    }else
    {//˵���ǵõ��ź�������
        int_restore_asyn_signal();
        return true;
    }
}

//----ɾ���ں��ź���-----------------------------------------------------------
//����: ɾ��һ���ں��ź�����������_knlΪ��׺�ĺ���һ����cn_locks_limit���û�����
//      �ģ�Ϊ����ģ��Ķ����ԣ��ں˲����û�����cn_locks_limitָ�꣬���ں�ʹ�õ�
//      ��(�ź����ͻ�����)�õ����ĺ��������ɾ�������õĻ����������ں˺�Ӧ�ó���
//      �������
//      ��������semp_delete�Ĳ�����ڱ��������ͷ��ź������ƿ顣
//������semp����ɾ�����ź���
//���أ���
//-----------------------------------------------------------------------------
bool_t semp_delete_knl(struct semaphore_LCB *semp)
{
    bool_t result;
    if(semp == NULL)    //��������
        return false;
    int_save_asyn_signal();
    if(semp->semp_sync == NULL)     //û���¼��ڵȴ��źŵ�
    {
        rsc_del_node(&semp->node);  //ɾ���ź������
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}

//----ɾ��һ���ź���-----------------------------------------------------------
//���ܣ�ɾ��һ���ź���
//������semp����ɾ�����ź���
//���أ���
//-----------------------------------------------------------------------------
bool_t semp_delete(struct semaphore_LCB *semp)
{
    bool_t result;
    if(semp == NULL)    //��������
        return false;
    int_save_asyn_signal();
    if(semp->semp_sync == NULL)     //û���¼��ڵȴ��źŵ�
    {
        rsc_del_node(&semp->node);  //ɾ���ź������
        mb_free(pg_lock_pool,semp); //�ͷ��ڴ�
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}

//----��ѯһ���ź���-----------------------------------------------------------
//���ܣ���ѯ�źŵ��Ա����������
//������semp������ѯ���ź���
//���أ��������
//-----------------------------------------------------------------------------
u32 semp_query_used(struct semaphore_LCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > cn_cpu_bits)
        atom_low_t atom_low;
        atom_low = int_low_atom_start();
#endif
        result = semp->lamp_used;     //������ʹ�õ��źŵ�����
#if (32 > cn_cpu_bits)
        int_low_atom_end();
#endif
    }
    return result;
}

//----��ѯ�ź�������-----------------------------------------------------------
//���ܣ���ѯһ���ź��������źŵƵ�����
//������semp������ѯ���ź���
//���أ��źŵ�����
//-----------------------------------------------------------------------------
u32 semp_query_capacital(struct semaphore_LCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > cn_cpu_bits)
        atom_low_t atom_low;
        atom_low = int_low_atom_start();
#endif
        result = semp->lamps_limit;     //������ʹ�õ��źŵ�����
#if (32 > cn_cpu_bits)
        int_low_atom_end();
#endif
    }
    return result;
}
//----��ѯ�����źŵ�����-------------------------------------------------------
//���ܣ���ѯһ���ź��������źŵƵ�����
//������semp������ѯ���ź���
//���أ��źŵ�����
//-----------------------------------------------------------------------------
u32 semp_query_free(struct semaphore_LCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > cn_cpu_bits)
        atom_low_t atom_low;
        atom_low = int_low_atom_start();
#endif
        result = semp->lamp_counter;     //������ʹ�õ��źŵ�����
#if (32 > cn_cpu_bits)
        int_low_atom_end();
#endif
    }
    return result;
}

//----����ͬ����������ʽ-----------------------------------------------------
//����: ���õȴ��ź������¼�������ʽ������Ϊ�����������ʱ�����򣬻��߰�������
//      ���¼������ȼ������ź�����������ʱ��Ĭ���ǰ����ȼ�����ġ�
//      ������ֻӰ����ñ������������е��¼���ԭ���ڶ����е��¼�����Ӱ�졣
//����: semp: �����õ��ź���
//      order: sort_prio=���ȼ��Ŷӣ�sort_time=�Ⱥ�˳���Ŷ�
//����: ��
//-----------------------------------------------------------------------------
void semp_set_sync_sort(struct semaphore_LCB *semp,enum semp_sync_sort order)
{
    if(NULL == semp)
        return;
    semp->sync_order = order;
}
//-----------------------�����ǻ�����------------------------------------------

//----����һ���ź���-----------------------------------------------------------
//���ܣ�����һ����������
//������name�������������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½����Ļ�����ָ��
//-----------------------------------------------------------------------------
struct mutex_LCB *mutex_create(char *name)
{
    struct mutex_LCB *mutex;
    mutex = mb_malloc(pg_lock_pool,0);
    if(mutex == NULL)
        return NULL;
    mutex->enable = true;   //�������ĳ�ʼ״̬�ǿ��õ�,�������ָ��onwer
    mutex->mutex_sync = NULL;
    mutex->prio_bak = cn_prio_invalid;
    mutex->owner = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    rsc_add_son(&pg_mutex_rsc->node,&mutex->node,
                        sizeof(struct mutex_LCB),name);
    return mutex;
}

//----����һ���ں˻�����-------------------------------------------------------
//���ܣ�����һ���ں˻�������������_knlΪ��׺�ĺ���һ����cn_locks_limit���û�����
//      �ģ�Ϊ����ģ��Ķ����ԣ��ں˲����û�����cn_locks_limitָ�꣬���ں�ʹ�õ�
//      ��(�ź����ͻ�����)�õ����ĺ���������ɾ�������õĻ����������ں˺�Ӧ�ó���
//      ������ϡ�
//      ��mutex_createe�������ǣ���������Ҫ�������ṩmutexʵ�壬�����ӳ��з��䡣
//������mutex,Ŀ���ź���ָ��
//      init_lamp,��ʼ�źŵ�״̬,true=��Դ���ã�false=��Դ������
//      name�������������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
void mutex_create_knl( struct mutex_LCB *mutex,char *name)
{
    mutex->enable = true;   //�������ĳ�ʼ״̬�ǿ��õ�,�������ָ��onwer
    mutex->mutex_sync = NULL;
    mutex->prio_bak = cn_prio_invalid;
    mutex->owner = NULL;
    //���½ڵ�ҵ��ź������ڵ���
    rsc_add_son(&pg_mutex_rsc->node,&mutex->node,
                        sizeof(struct mutex_LCB),name);
}

//----�ͷ�һ��������-----------------------------------------------------------
//���ܣ��ͷŻ�����
//������mutex,������ָ��
//���أ���
//-----------------------------------------------------------------------------
void mutex_post(struct mutex_LCB *mutex)
{
    struct  event_ECB *event;

    if(mutex == NULL)
        return;
    if(mutex->owner != pg_event_running)    //������ֻ����ӵ�����ͷ�
        return;
    int_save_asyn_signal();
    if(mutex->mutex_sync == NULL)     //�ȴ����пգ����û�����Ϊ����״̬
    {
        mutex->enable = true;       //����������
    } else      //���¼��ڵȴ����ѻ�����ֱ��ת�������������ȼ���ߵ��¼�
    {
        event = mutex->mutex_sync;
        event->sync_head = NULL;
        if(event->multi_next == event)
        {//������ֻ��һ���¼�
            mutex->mutex_sync = NULL;
        }else
        {//�������ж���¼���ȡ������ͷ�����¼�
            mutex->mutex_sync = event->multi_next;
            event->multi_next->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = event->multi_next;
            event->multi_next = NULL;
            event->multi_previous = NULL;
        }
        event->last_status.all = event->event_status.all;//�����¼�״̬
        if(event->event_status.bit.wait_overtime)
            __djy_resume_delay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
        event->event_status.all = 0;
//        event->event_status.bit.wait_mutex = 0; //����ȴ�״̬
        __djy_event_ready(event);
        if(mutex->prio_bak != cn_prio_invalid)  //�û��������������ȼ��̳�
        {
            __djy_cut_ready_event(pg_event_running);    //ȡ��running�¼�
            pg_event_running->prio = mutex->prio_bak;   //�ָ����ȼ�
            mutex->prio_bak = cn_prio_invalid;
            __djy_event_ready(pg_event_running); //���°�running�����������(��λ��)
        }
    }
    int_restore_asyn_signal();
}

//----���󻥳���-----------------------------------------------------------
//���ܣ����󻥳��������ɹ���ӵ�иû�������������owner��Ա�����������ʺ����¼�֮
//      ����¼����첽�ź�֮��ͬ����ֻ�д���owner��Ա������ʵ�����ȼ��̳С�
//������mutex,������ָ��
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�true=ȡ���źŷ��ػ���Դ�����źŵƱ���(mutex == NULL)��mutex==NULLʱ����
//      true��������ģ������ϣ�������ڲ����Ƿ����źŵƱ���������һ�£���semp
//      ��ΪNULL�����ǵ�ѡ�񣬱�����Ҫ����һ�������źŵƱ������豸���ɰѸ��豸
//      struct pan_device��semp��Ա��ΪNULL��
//      false=û��ȡ���ź�(��ʱ���ػ�����ԭ��)
//-----------------------------------------------------------------------------
bool_t mutex_pend(struct mutex_LCB *mutex,u32 timeout)
{
    struct  event_ECB *pl_ecb;
    atom_low_t atom_low;
    bool_t lamp;

    if(mutex == NULL)
        return true;
    //���¶�ȡ�źŵ����������뱣֤ԭ�Ӳ������ʹ��ж�
    atom_low = int_low_atom_start(  );             //�������Բ���Ƕ�׵���
    if(mutex->enable == true)   //�źŵƿ���
    {
        lamp = true;
        mutex->enable = false;
        mutex->prio_bak = cn_prio_invalid;
        mutex->owner = pg_event_running;
    }else                       //�źŵƲ�����
    {
        lamp = false;
    }
    int_low_atom_end( atom_low );

    if(lamp == true)
        return true;    //ȡ�û���������
    else if(!djy_query_sch() || (timeout == 0))//��ֹ����ʱ,��������.��timeout=0
        return false;   //û��ȡ�û���������

    int_save_asyn_signal();
    __djy_cut_ready_event(pg_event_running);
    pg_event_running->previous = NULL;
    pg_event_running->next = NULL;

    pg_event_running->sync_head = &mutex->mutex_sync;
    if(mutex->mutex_sync == NULL)
    {//ͬ�����п�,running�¼��Գ�˫��ѭ������
        pg_event_running->multi_next = pg_event_running;
        pg_event_running->multi_previous = pg_event_running;
        mutex->mutex_sync = pg_event_running;
    }else
    {//ͬ�����зǿ�,�����ȼ�����
        pl_ecb = mutex->mutex_sync;
        do
        { //�ҵ�һ�����ȼ��������¼����¼�.
            if(pl_ecb->prio <= pg_event_running->prio)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != mutex->mutex_sync);
        pg_event_running->multi_next = pl_ecb;
        pg_event_running->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = pg_event_running;
        pl_ecb->multi_previous = pg_event_running;
        if(mutex->mutex_sync->prio > pg_event_running->prio)
            mutex->mutex_sync = mutex->mutex_sync->multi_previous;
    }
    pg_event_running->last_status.all = pg_event_running->event_status.all;
    pg_event_running->event_status.bit.wait_mutex = 1;  //�¼�״̬��Ϊ�ȴ�������
    if(timeout != cn_timeout_forever)
    {
        pg_event_running->event_status.bit.wait_overtime = 1;
        __djy_addto_delay(timeout);
    }
    //���濴���Ƿ�Ҫ�����ȼ��̳�
    pl_ecb = mutex->owner;
    if(pl_ecb->prio > pg_event_running->prio) //��Ҫ�̳����ȼ�
    {//1�����ھ���̬��2������ĳ������̬��
        if(pl_ecb->event_status.all == 0)//ռ�û��������¼����ھ���̬
        {
            __djy_cut_ready_event(pl_ecb);
            //prio_bak�����¼���ԭʼ���ȼ�����ֹ���ֶ�μ̳��б��޸�Ϊ�м����ȼ�
            if(mutex->prio_bak == cn_prio_invalid)
                mutex->prio_bak = pl_ecb->prio;
            pl_ecb->prio = pg_event_running->prio;
            __djy_event_ready(pl_ecb);
        }else       //ռ�û��������¼�����ĳ������̬���ݲ�����
        {
        }
    }
    int_restore_asyn_signal();  //�ָ��жϣ��������������л�
    int_save_asyn_signal();     //��ʱ���Ѿ������»�û��������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(pg_event_running->event_status.bit.wait_mutex)
    {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ��Ŀ���¼���ͬ��������ȡ���¼���
     //��ʱ����ͬ�����¼��϶���û����ɡ�
        pg_event_running->event_status.bit.wait_mutex = 0;
        int_restore_asyn_signal();
        return false;
    }else
    {//˵���ǵõ�����������
        int_restore_asyn_signal();
        return true;
    }
}

//----ɾ���ں˻�����-----------------------------------------------------------
//����: ɾ��һ���ں˻�������������_knlΪ��׺�ĺ���һ����cn_locks_limit���û�����
//      �ģ�Ϊ����ģ��Ķ����ԣ��ں˲����û�����cn_locks_limitָ�꣬���ں�ʹ�õ�
//      ��(�ź����ͻ�����)�õ����ĺ���������ɾ�������õĻ����������ں˺�Ӧ�ó���
//      �������
//      ��������semp_delete�Ĳ�����ڱ��������ͷŻ��������ƿ顣
//������mutex����ɾ���Ļ�����
//���أ�true=�ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t mutex_delete_knl(struct mutex_LCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //��������
        return false;
    int_save_asyn_signal();
    if(mutex->mutex_sync == NULL)     //û���¼��ڵȴ�������
    {
        rsc_del_node(&mutex->node);  //ɾ�����������
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}

//----ɾ��һ��������-----------------------------------------------------------
//���ܣ�ɾ��һ��������
//������mutex����ɾ���Ļ�����
//���أ���
//-----------------------------------------------------------------------------
bool_t mutex_delete(struct mutex_LCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //��������
        return false;
    int_save_asyn_signal();
    if(mutex->mutex_sync != NULL)     //û���¼��ڵȴ�������
    {
        rsc_del_node(&mutex->node);  //ɾ�����������
        mb_free(pg_lock_pool,mutex); //�ͷ��ڴ�
        result = true;
    }else
        result = false;
    int_restore_asyn_signal();
    return result;
}
//----��ѯһ��������-----------------------------------------------------------
//���ܣ���ѯ�û������Ƿ����
//������mutex������ѯ�Ļ�����
//���أ�true = ���ã�false = ������
//-----------------------------------------------------------------------------
bool_t mutex_query(struct mutex_LCB *mutex)
{
    return mutex->enable;
}

