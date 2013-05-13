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
#ifndef __semp_h__
#define __semp_h__

#ifdef __cplusplus
extern "C" {
#endif


struct  event_ECB;

// ���¼������ź������ã���������ź�����ͬ�����У��ȴ������ź������¼��ͷ��ź�
// �����ȴ���������������ʽ�������ȼ�˳���ʱ��˳��ʽ��
// ֻ���ź����������Ⱥ�˳�򣬻�����ֻ�������ȼ�����ʽ��
enum semp_sync_sort
{
    sort_prio = 0,  //�������ź������¼������ȼ��Ŷ�
    sort_time,      //�������ź������Ⱥ�˳���Ŷ�
};
//---------�������źŵ�����--------------
//1���������źſ��ṩ�ܱ�������Դ��ʹ�ü���
//2��Ϊ������Ҳ�ɲ���������Դ�ṩһ�µĴ��룬�������ṩ������ʱ���õĿ�����
struct semaphore_LCB
{
    struct  rsc_node node;
    enum semp_sync_sort sync_order;
    u32 lamps_limit;   //�źŵ��������ޣ�cn_limit_uint32��ʾ��������
    u32 lamp_counter;  //�����źŵ�������
    u32 lamp_used;     //���źŵƱ�������������������ʱ�����Ƶ�0
    struct  event_ECB *semp_sync;    //�Ⱥ��źŵ��¼����У����ȼ��Ŷ�
};

struct mutex_LCB
{
    struct  rsc_node node;
    bool_t  enable;
    ufast_t  prio_bak;          //���ȼ��̳��б���ԭ���ȼ�
    struct  event_ECB *owner;        //���л��������¼�
    struct  event_ECB *mutex_sync;    //�Ⱥ��źŵ��¼����У����ȼ��Ŷ�
};

//�����ź����ͻ����������ڴ��
union lock_MCB
{
    struct semaphore_LCB sem;
    struct mutex_LCB  mut;
};

//������Ϣ����
enum lock_error_code
{
    enum_lock_no_error = cn_lock_no_error,      //û�д���
    enum_lock_range_error,            //�ź�����Χ��������
};

ptu32_t module_init_lock1(ptu32_t para);
ptu32_t module_init_lock2(ptu32_t para);
struct semaphore_LCB *semp_create(u32 lamps_limit,u32 init_lamp,char *name);
void semp_create_knl( struct semaphore_LCB *semp,
                       u32 lamps_limit,u32 init_lamp,char *name);
void semp_post(struct semaphore_LCB *semp);
bool_t semp_pend(struct semaphore_LCB *semp,u32 timeout);
bool_t semp_delete_knl(struct semaphore_LCB *semp);
bool_t semp_delete(struct semaphore_LCB *semp);
u32 semp_query_used(struct semaphore_LCB *semp);
u32 semp_query_capacital(struct semaphore_LCB *semp);
u32 semp_query_free(struct semaphore_LCB *semp);
void semp_set_sync_sort(struct semaphore_LCB *semp,enum semp_sync_sort order);
struct mutex_LCB *mutex_create(char *name);
void mutex_create_knl( struct mutex_LCB *mutex,char *name);
void mutex_post(struct mutex_LCB *mutex);
bool_t mutex_pend(struct mutex_LCB *mutex,u32 timeout);
bool_t mutex_delete(struct mutex_LCB *mutex);
bool_t mutex_delete_knl(struct mutex_LCB *mutex);
bool_t mutex_query(struct mutex_LCB *mutex);

#ifdef __cplusplus
}
#endif

#endif //__semp_h__

