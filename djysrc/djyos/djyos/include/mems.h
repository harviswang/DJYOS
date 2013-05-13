//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ѹ���ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ�������ڴ�������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __mems_H__
#define __mems_H__

#ifdef __cplusplus
extern "C" {
#endif

struct  event_ECB;

struct mem_global
{
    u8     *static_bottom; //�ѵ�ָ�룬��׼��̬��������
    u8     *heap_bottom;   //�ѵ�ָ�룬�ӿ�������̬��������
    u8     *heap_top;      //�Ѷ�ָ��,
    bool_t      dynamic;        //false =ִ��׼��̬����
#if (cfg_dynamic_mem == 1)
    struct  event_ECB *mem_sync; //�ȴ������ڴ���¼�����,˫��ѭ������
    ptu32_t     ua_pages_num;   //��ҳ��
    ptu32_t     free_pages_num;  //�����ڴ�ҳ��
    ptu32_t     ua_block_max;    //����ڴ��ߴ�(�ֽ���)
    ptu32_t     ua_free_block_max; //���е�����ڴ���С(�ֽ���);
    ufast_t     uf_grades;      //�ܽ���
                                //true = ִ�ж�̬�ڴ����

    //����3�����ڳ�ʼ��ʱ�Ӷ��л�����
    //��һ���֣�ÿҳ����һ��16λ�ּ�¼ӵ�б�ҳ�ڴ���¼���ID����������¼������
    //�ͷžֲ��ڴ棬����һ��������ɻ��ҵĲ�����
    //�������ڱ�ʾ�ôη����ҳ��,ʹfree�������Բ�ָ���ͷŵ��ֽ���.
    //���y_event_done���������¼�����δ�ͷŵľֲ��ڴ�飬���ݴ˱�ǿ���ͷ�֮
    //��ʽ����:
    //1.��ҳ�ֲ��ڴ�:id,
    //2.˫ҳ�ֲ��ڴ�:0xffff+id
    //3.��ҳ�ֲ��ڴ�:0xfffe+id+�׺�
    //4.��ҳȫ���ڴ�:0xfffd,ȫ���ڴ�û�������¼�id,ֻ��Ҫ֪���ڴ��С
    //5.˫(��)ҳȫ���ڴ�:0xfffc+�׺�.
    uint16_t    *index_event_id;
    ufast_t     *p_classes;     //�ڶ�����,���׿���н�����������
    ucpu_t      ***ppp_bitmap;  //��������,ָ��"���н�����λͼָ���ָ��",
                                //ÿ��һ��,ָ��ý׿��н������ĵ�һ��ָ��
#endif
} ;

//���ڰ��¼�����ľֲ��ڴ�鴮����������y_event_done����ʱ���ݴ˻����ڴ档
struct mem_record
{
    struct mem_record   *next;
    struct event_ECB *owner;
    u8 *address;
};

#if (cfg_dynamic_mem == 0)
#define m_malloc(size,timeout)          __m_static_malloc(size)
#define m_malloc_gbl(size,timeout)      __m_static_malloc(size)
#define m_free(p)                       __m_static_free(p)
#else
void *m_malloc(ptu32_t size,u32 timeout);
void *m_malloc_gbl(ptu32_t size,u32 timeout);
bool_t m_free(void * pl_mem);
#endif

ucpu_t __m_leading_zero(u8 *data,u8 len);
u32 __m_calculate_expense(ptu32_t ua_pages);
struct mem_record *__inc_record_link(void);
ucpu_t __m_leading_ucpu_zero(ucpu_t data);
ufast_t __m_get_grade(ptu32_t size);
void __record_mem(u8 *address,ufast_t uf_grade_th);
bool_t __m_check_memory(ptu32_t size,u32 timeout);
void __m_wait_memory(struct  event_ECB *event);
void __m_cleanup(uint16_t event_id);
ptu32_t module_init_heap_static(ptu32_t para);
ptu32_t module_init_heap_dynamic(ptu32_t para);
void *__m_static_malloc(ptu32_t size);
bool_t __m_static_free(void * pl_mem);
void *__malloc_block(ufast_t grade);
ptu32_t m_check_size(void * mp);
ptu32_t  m_get_max_free_mem(void);
ptu32_t  m_get_heap_size(void);
ptu32_t  m_get_free_mem(void);
ptu32_t  m_get_page_size(void);

#ifdef __cplusplus
}
#endif

#endif //__mems_H__

