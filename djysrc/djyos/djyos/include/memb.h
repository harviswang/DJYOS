//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ڴ�ع���ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ�̶���������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __memb_H__
#define __memb_H__

#ifdef __cplusplus
extern "C" {
#endif

struct mem_cell_pool
{
    struct rsc_node memb_node;    //��Դ���,�����ڴ�ؽ���Ϊһ����Դ���
    void  *continue_pool;    //�����ڴ���׵�ַ��ʹ������������ʵʱ�ԡ�
    struct mem_cell_free  *free_list;        //δ���������,����,NULL��β
    struct semaphore_LCB memb_semp;
    ptu32_t pool_offset;    //�������е�ƫ����(��ǰ��ַ)
    u32 cell_size;          //���С,��ʼ��ʱ����ϵͳҪ������ߴ硣
    u32 cell_increment;     //���ڴ�غľ�ʱ�����Զ��Ӷ��������ڴ�سߴ磬��λΪ
                            //�ڴ����������Ϊ0��ʾ��������ע�⣬�ڴ�سߴ���ֻ
                            //�������ģ���һ���Ӷ����������ڴ棬�ǲ��ͷŵġ�
    u32 cell_limit;         //���increment !=0������Ա�޶����ڴ�ص����ߴ磬
                            //��λΪ�ڴ������
    struct  event_ECB *cell_sync;   //ͬ�����С�
};

//�����ڴ�鹹��˫��ѭ����������ʼ���ڴ��ʱ�������ڴ���������������ͷ�ʱ
//�ż��롣
//��������ڵ㣬��free_list����˫�������������ӳ���Ľ�׳�ԣ�����ǵ�������
//��Ȼ����ʵ�ֹ��ܣ����ǣ�����ظ��ͷ�һ���Ѿ��ͷŵ��ڴ�飬����ܵ����ƻ�����
//��˫��������������жϸý���Ƿ��Ѿ���free_list�У����ǣ����ظ�ִ���ͷŲ�
//��������������Ȼ�����ñ����ķ����ж��Ƿ���free_list����ִ��ʱ�䲻ȷ����������
//ʵʱϵͳ��Ҫ��
//��˫������ĺ���ǣ������ڴ���������ָ�볤�ȣ�ͨ��Ϊ8�ֽڡ�
struct mem_cell_free
{
    struct mem_cell_free *previous;
    struct mem_cell_free *next;
};
ptu32_t module_init_memb(ptu32_t para);
struct mem_cell_pool *mb_create_pool(void *pool_original,u32 capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name);
struct mem_cell_pool *mb_create_pool_knl(struct mem_cell_pool *pool,
                                void *pool_original,u32 capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name);
bool_t mb_delete_pool(struct mem_cell_pool *pool);
bool_t mb_delete_pool_knl(struct mem_cell_pool *pool);
void *mb_malloc(struct mem_cell_pool *pool,u32 timeout);
void mb_free(struct mem_cell_pool *pool,void *block);
u32 mb_query_free(struct mem_cell_pool *pool);
u32 mb_query_capacital(struct mem_cell_pool *pool);

#ifdef __cplusplus
}
#endif

#endif //__memb_H__

