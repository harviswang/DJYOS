//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ѹ���ģ��
//����:  ������.
//�汾��V1.0.1
//�ļ�����:�ṩ�������ڴ�������
//����˵��:
//�޶���ʷ:
//3. ����:2009-10-31
//   ����:  ������.
//   �°汾�ţ�1.0.2
//   �޸�˵��: ������__m_check_memory��__m_wait_memory������һ�����󣬸�bug��
//   ����"�ٻ���"�ύ
//2. ����:2009-03-03
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������__m_check_memory������һ�����󣬸�bug������sniper�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

#if (cfg_dynamic_mem == 1)
//�ֽ�����ǰ��0������,256�ֽ�,���ڿ��ٲ�Ѱ�׸���0λ��λ��
u8 const cn_leading_zero[]=
{
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//�ֽ����ݺ�׺0������,256�ֽ�,���ڿ��ٲ�Ѱ�׸���0λ��λ��
//u8 const cn_suffix_zero[]=
//{
//    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
//};
const ucpu_t cn_low_xbit_msk_exp2 []=
{
    cn_low_1bit_1,
    cn_low_2bit_1,
    cn_low_4bit_1,
    cn_low_8bit_1,
    cn_low_16bit_1,
    cn_allbit_1,
};

const ucpu_t cn_low_xbit_msk []=
{
    0,
    cn_low_1bit_1,
    cn_low_2bit_1,
    cn_low_3bit_1,
    cn_low_4bit_1,
    cn_low_5bit_1,
    cn_low_6bit_1,
    cn_low_7bit_1,
    cn_low_8bit_1,
    cn_low_9bit_1,
    cn_low_10bit_1,
    cn_low_11bit_1,
    cn_low_12bit_1,
    cn_low_13bit_1,
    cn_low_14bit_1,
    cn_low_15bit_1,
    cn_low_16bit_1,
    cn_low_17bit_1,
    cn_low_18bit_1,
    cn_low_19bit_1,
    cn_low_20bit_1,
    cn_low_21bit_1,
    cn_low_22bit_1,
    cn_low_23bit_1,
    cn_low_24bit_1,
    cn_low_25bit_1,
    cn_low_26bit_1,
    cn_low_27bit_1,
    cn_low_28bit_1,
    cn_low_29bit_1,
    cn_low_30bit_1,
    cn_low_31bit_1,
    cn_allbit_1,
};
#else
#define m_malloc(size,timeout)          __m_static_malloc(size)
#define m_malloc_gbl(size,timeout)      __m_static_malloc(size)
#define m_free(p)                       __m_static_free(p)
#endif

static struct mem_global tg_MCB;        //ȫ���ڴ���ƿ�
static struct mutex_LCB tg_mem_mutex;   //����tg_mem_global������������

struct mem_global *djy_get_MCB(void)
{
    return &tg_MCB;
}
//----�ڴ�ṹ��ʼ��-----------------------------------------------------------
//����: ��ʼ���ڴ����ݽṹ,������ʼ����
//����: ��
//����: ��
//��ע: 1.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//      2.������������ȵ��ñ�����,������̬���书��,�������߱���̬���书��.
//-----------------------------------------------------------------------------
ptu32_t module_init_heap_static(ptu32_t para)
{
#if(align_up_sys(cn_page_size) != cn_page_size) //�����������assert����
cn_page_size ������ϵͳ����Ҫ��
#endif
    tg_MCB.dynamic = false;
    tg_MCB.static_bottom = (u8*)align_up_sys((ptu32_t)sys_heap_bottom);
    tg_MCB.heap_bottom = tg_MCB.static_bottom;
    tg_MCB.heap_top=(u8*)align_down_sys((ptu32_t)sys_heap_top);
    return 1;
}

//----׼��̬�ڴ����-----------------------------------------------------------
//���ܣ������ڴ�,����heap_bottomָ��
//������size,��������ڴ��ߴ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: 1.׼��̬�����뾲̬�ڴ��������,û�б�����ʩ,��ȷ��Ҫ����Ա�Լ���֤.����
//      ���䷽��Ҳ������������,��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ����
//      ������׼��̬����
//      2.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//-----------------------------------------------------------------------------
void *__m_static_malloc(ptu32_t size)
{
    u8 *temp;

    temp = tg_MCB.heap_bottom;   //���浱ǰ�ѵ�
    //���±����ߴ�Ŀռ�����
    tg_MCB.heap_bottom = (u8 *)align_up_sys
            ((ptu32_t)tg_MCB.heap_bottom + sizeof(ptu32_t));
    if((tg_MCB.heap_top-tg_MCB.heap_bottom) < size)
    {
        tg_MCB.heap_bottom = temp;   //�ָ���ǰ�ѵ�
        return NULL;        //�ڴ治�㣬����
    }
    //�·�����ڴ�ߴ籣���ڵ�ǰ�ѵ�ǰ��
    temp = tg_MCB.heap_bottom - sizeof(ptu32_t);
    *(ptu32_t *)temp = size;    //����������ڴ��ߴ�
    temp = tg_MCB.heap_bottom;   //���������û����ڴ���ַ
    tg_MCB.heap_bottom += size;   //�ƶ��ѵײ�ָ��
    tg_MCB.heap_bottom = (u8 *)align_up_sys
                ((ptu32_t)tg_MCB.heap_bottom);//����
    return(temp);
}

//----�ͷ�׼��̬������ڴ��---------------------------------------------------
//���ܣ��ͷ�׼��̬�����һ���ڴ��
//������pl_mem�����ͷŵ��ڴ��ָ��
//���أ�true = �ɹ��ͷţ�false =  �ͷ�ʧ��
//��ע: 1.׼��̬�����뾲̬�ڴ��������,û�б�����ʩ,��ȷ��Ҫ����Ա�Լ���֤.����
//      ���䷽��Ҳ������������,��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ����
//      ������׼��̬����
//      2.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//-----------------------------------------------------------------------------
bool_t __m_static_free(void * pl_mem)
{
    return true;
}

#if (cfg_dynamic_mem == 0)

//----��ѯ�ڴ�ߴ�-------------------------------------------------------------
//����: ���ݸ�����ָ��,��ѯ��ָ�����ڵ��ڴ��ĳߴ�.
//����: mp,��̬������ڴ�ָ��.
//����: �ڴ��ߴ�,����0�м��ֺ���:1.�Ƿ�ָ��,2.mp����׼��̬�����ָ��.
//-----------------------------------------------------------------------------
ptu32_t m_check_size(void * mp)
{
    return *(ptu32_t*)(((ptu32_t*)mp) -1);
}

#else

//----��ѯ�ڴ�ߴ�-------------------------------------------------------------
//����: ���ݸ�����ָ��,��ѯ��ָ�����ڵ��ڴ��ĳߴ�.
//����: mp,��̬������ڴ�ָ��.
//����: �ڴ��ߴ�,����0�м��ֺ���:1.�Ƿ�ָ��,2.mp����׼��̬�����ָ��.
//-----------------------------------------------------------------------------
ptu32_t m_check_size(void * mp)
{
    ptu32_t ua_pages_number;
    u8 *temp;
    uint16_t *pl_id;
    ufast_t uf_free_grade_th;
    if(((u8*)mp<tg_MCB.static_bottom)
                || ((u8*)mp>=tg_MCB.heap_top))
    {
        return 0;   //����,ָ�벻�ڶѷ�Χ��
    }

    if((u8*)mp < tg_MCB.heap_bottom)    //��ָ���ھ�̬������
    {
        temp = (u8*)((u8*)mp - sizeof(ptu32_t));
        return *(ptu32_t*)temp;
    }else                                           //��ָ���ڿ�������̬������
    {
        //�����ͷŵ��ڴ��ҳ��
        ua_pages_number=(ptu32_t)((ptu32_t)mp-(ptu32_t)tg_MCB.heap_bottom)
                            >>cn_page_size_suffix_zero;

        //�����ͷŵ��ڴ��Ľ׺�,��0���.ͨ���׺�Ҳ����ȷ���ڴ��Ĵ�С.
        //ȷ���ڴ��������,�ֲ�������Ҫ֪��ӵ�и��ڴ���¼�id��
        //ȫ�ַ�����������ڴ������ٱ�,����֪��ӵ�и��ڴ���¼�id
        pl_id = &tg_MCB.index_event_id[ua_pages_number];
        switch(pl_id[0])
        {
            case 0xffff :
            {//˫ҳ�ֲ�����,-1+id
                uf_free_grade_th = 1;
            }break;
            case 0xfffe :
            {//��ҳ�ֲ�����:-2+id+�׺�
                uf_free_grade_th = pl_id[2];
            }break;
            case 0xfffd :
            {//��ҳȫ���ڴ�:-3
                uf_free_grade_th = 0;
            }break;
            case 0xfffc :
            {//˫(��)ҳȫ���ڴ�:-4+�׺�.
                uf_free_grade_th = pl_id[1];
            }break;
            default :
            {
                if(pl_id[0] <= cn_event_id_limit)
                    uf_free_grade_th = 0;       //��ҳ�ֲ�����:id,
                else
                {
                    djy_error_login(enum_mem_error,NULL);   //ָ���д�,ֱ���˳�
                    return false;

                }
            }break;
        }
        return cn_page_size << uf_free_grade_th;
    }
}

//----������������λ1λ�ã�ǰ��0�ĸ�����----------------------------------------
//���ܣ������������ֽڵ����λ��ʼ����ǰ��0��������
//������data,��������ָ��
//      len,����Ĵ�С
//���أ�ǰ��0�ĸ���
//------------------------------------------------------------------------------
ucpu_t __m_leading_zero(u8 *data,u8 len)
{
    u8 u8_i;
    ucpu_t  uc_j=0;
    u8_i=len-1;
    do
    {
        if(cn_leading_zero[data[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[data[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i-- !=0);
    return uc_j;
}

//----������Ʊ�������ֽ���---------------------------------------------------
//���ܣ�������Ʊ�������ֽ���
//������u32_pages,�ڴ�ҳ��
//���أ��ֽ���
//ע��: ���ֺ������ܵĵ�һ��,��Ҫͬʱ������ƽṹ�����ֵ��ֽ���
//      ��ֲ����:����������Ļ����Ķ��뷽ʽ�й�,��ֲʱ������˽�����Ķ��뷽ʽ.
//      ��������������󲿷�cpu�Ķ���Ҫ��,�����ų�������.��������ֲʱ����ö���
//      ��������Ȼ�����Ƿ���Ҫ�޸�
//-----------------------------------------------------------------------------
u32 __m_calculate_expense(ptu32_t ua_pages)
{
    ptu32_t ua_bits,u32_return;
    ufast_t uf_grades,uf_grade_th;

    //�����������һ���ж�����ҳ�ߴ�,����:��ַ���ȼ�ȥua_pages��ǰ��0����������
    //����,����,ua_pages=5,���λλ����bit2,��29��ǰ��0��ҳ�ߴ���1,2,4ҳ3��,����=3
    uf_grades=cn_ptr_bits-__m_leading_zero((u8 *)&ua_pages,sizeof(ptu32_t));

    //ÿҳ����һ��16λ�ּ�¼ӵ�б�ҳ�ڴ���¼���ID��
    //������߽��ڴ��ʱ��ֻ��¼��ҳ��freeʱ������������ñ�����Ϊ�˷�ֹ��
    //�¼��佻�������ͷ��ڴ�ʱ�����ڳ��������������û�����ʹ�á�
    u32_return = ua_pages<<1;
    //��һ����������ufast_t���͵�,��Ҫ��ufast_t���Ͷ���.
    u32_return = (u32_return+sizeof(ufast_t)-1)&(~(sizeof(ufast_t)-1));
    //ÿ�׻�Ҫ����һ��ufast_t���͵�����,��¼���׵ļ���
    u32_return += uf_grades*(sizeof(ufast_t));
    //��һ����������ָ��,��Ҫ��ָ�����
    u32_return = (u32_return +sizeof(void*) -1)&(~(sizeof(void*)-1));
    //ÿ��һ��ָ��,ָ��λͼ������
    u32_return += uf_grades*sizeof(void*);

    for(uf_grade_th = 0; uf_grade_th < uf_grades; uf_grade_th++)
    {
        ua_bits = ua_pages>>uf_grade_th;   //���㱾����λ��
        do
        {
            ua_bits=(ua_bits+cn_cpu_bits-1)>>cn_cpu_bits_suffix_zero;
            u32_return +=ua_bits*sizeof(ucpu_t);    //�ü�λͼ����
            u32_return+=sizeof(void*);                //��Ҫһ��ָ��ָ���λͼ����
        }while(ua_bits>1); //ֱ�����м���������.
    }
    return u32_return;
}

//----�ӳ��ڴ��¼��------------------------------------------------------------
//����: ���ڴ��¼����ʱ(��pg_mem_record_free==NULL),���ñ��������ڴ���ȡ1��
//      �ṹ.���Ԥ�ȷ�����ڴ��Ѿ�����,�����__malloc_block����һҳ�µ��ڴ�.
//      ���û���㹻���ڴ���Է���,����NULL.�������ڲ�����,�����Ÿ��û�
//����: ��
//����: ����ɹ��ӳ�,���ر�ͷָ��,���򷵻�NULL
//��ע�����ñ�����ǰӦ�ñ�֤�жϱ��رա�ÿ��ֻȡһ���ṹ,�����ǰ��·������ҳ
//      �ڴ�����pg_mem_record_free����,��Ȼ�������ܵ�ִ��ʱ��,���������ʵʱ��.
//-----------------------------------------------------------------------------
/*
struct mem_record *__inc_record_link(void)
{
    static struct mem_record *record_free=NULL;
    struct mem_record *result;

    if(record_free==NULL)
        record_free=(struct mem_record*)__malloc_block(0);//����1ҳ�ڴ����ڼ�¼
    if(record_free==NULL)   //����û���ڴ���Է���
        return NULL;
    result = record_free;   //ȡ��һ���ṹ��
    result->next = NULL;    //��ʼ���½ڵ��nextָ��
    record_free = record_free+1;  //ָ����һ���ṹ��
    if((cn_page_size-sizeof(struct mem_record)*(result-record_free))
            < sizeof(struct mem_record))
    {//���η����ҳ�Ѿ�����.
        record_free=NULL;
    }
    return result;
}
*/

//----����ucpu_t������ǰ��0�ĸ���----------------------------------------------
//���ܣ���ucpu_t�����ݵ����λ��ʼ����ǰ��0��������
//������data,������
//���أ�ǰ��0�ĸ���
//-----------------------------------------------------------------------------
ucpu_t __m_leading_ucpu_zero(ucpu_t data)
{
    u8 u8_i;
    ucpu_t  uc_j=0;
    u8_i=sizeof(ucpu_t)-1;
    do
    {
        if(cn_leading_zero[((u8*)&data)[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[((u8*)&data)[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i-- !=0);
    return uc_j;
}

//----���ع�񻯽�-------------------------------------------------------------
//���ܣ����������ݹ��Ϊ�Ϸ����ڴ�ߴ硣
//������size,��������ڴ��ߴ�
//���أ����ڻ����size����С�������ߴ�Ľס�
//-----------------------------------------------------------------------------
ufast_t __m_get_grade(ptu32_t size)
{
    ucpu_t  uc_temp;
    if(size<=cn_page_size)
        return 0;
    uc_temp=__m_leading_zero((u8 *)&size,sizeof(ptu32_t));
    if((cn_high_1bit_1>>uc_temp) != size)
    //���size�����ǹ�񻯵���,���������������С������С
    //����ά��size�Ĵ�С,��������
        uc_temp--;

    //���ؽ���,��0���,����Сҳ�ߴ�ǰ��0����ߴ�ǰ��0.
    return (ufast_t)(cn_ptr_bits-1-cn_page_size_suffix_zero-uc_temp);
}

//----��¼������ڴ�-----------------------------------------------------------
//���ܣ�1.���ڴ������Ϣ���浽running�¼����ڴ������������,�Ա��¼���ɵ�ʱ��
//      ǿ���ջ��ڴ�.����������ȫ���ڴ�,�򲻼�¼,�¼����ʱҲ���ջ�.
//      2.���ڴ������Ϣ���浽index_event_id������.index_event_id��ĸ�ʽ���ṹ
//      mem_global_t�Ķ���.
//����: address,������ڴ��ַ.
//      uf_grade_th,��������ڴ�Ľ׺�
//��ע: �������ɲ���ϵͳ�ڲ�����,�ڴ����������һ������ѭ������,ÿ�¼�һ��.
//-----------------------------------------------------------------------------
/*
void __record_mem(u8 *address,ufast_t uf_grade_th)
{
    uint16_t *pl_id,id;
    ptu32_t  page;

    pg_mem_record_free->address=address;
    if(pg_event_running->held_memory==NULL)
    {   //���running�¼����������ڴ��¼,������ڴ����ӵ���¼��ͷ��.
        pg_event_running->held_memory=pg_mem_record_free;
        pg_mem_record_free = pg_mem_record_free->next;
        pg_event_running->held_memory->next = NULL;
    }else
    {   //���running�¼��Ѿ��������ڴ��¼,������б��¼�������ڴ����ӵ�
        //�ڴ��¼���������ڽ����.����pg_event_running->held_memory����
        struct mem_record *p;
        p=pg_mem_record_free;
        pg_mem_record_free = pg_mem_record_free->next;
        p->next=pg_event_running->held_memory;
        pg_event_running->held_memory=p;
    }

    //�Ķ���������������mem_global_t��index_event_id��Ա�����ע��.
    pl_id = tg_MCB.index_event_id;
    id = pg_event_running->event_id;
    page = (ptu32_t)(address-tg_MCB.heap_bottom)
            >>cn_page_size_suffix_zero;
    if(uf_grade_th==0)
    {//����1ҳ
        pl_id[page] = id;
    }else if(uf_grade_th==1)
    {//����2ҳ
        pl_id[page] = -1;
        pl_id[page+1] = id;
    }else
    {   //�����ҳ
        pl_id[page] = -2;
        pl_id[page+1] = id;
        pl_id[page+2] = uf_grade_th;
    }

    if(pg_mem_record_free==NULL)
        pg_mem_record_free=__inc_record_link();
}
*/

//----�鿴���ȴ������ڴ�----------------------------------------------------
//���ܣ��ȴ�������һ�����������˫��ѭ������,����������ڴ�������,С����ǰ,
//      ���ڴ����ʱ,����ʹ������С���¼��õ�����,��������ʹ�ڴ����㾡�����¼�,
//      ��ʹ�����ȼ�����,��Ϊʵʱ�¼��ǲ�����ʹ�ö�̬�����,���Բ���Ӱ��ʵʱ��.
//������size,��Ҫ������ڴ�ߴ�
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���
//      ��0ֵ�������ϵ���Ϊcn_tick_us��������
//����: true = �п����ڴ湩���䣬false = ���ڴ�ɷ���
//��ע: �������ɲ���ϵͳ����,�û����ܵ���.
//------------------------------------------------------------------------------
bool_t __m_check_memory(ptu32_t size,u32 timeout)
{
    struct  event_ECB *event;
    u32 u32l_start_time,u32l_rest_time = 0;
    bool_t   wait;
    u32l_start_time = djy_get_time();
    do
    {
        wait=false;
        if(tg_MCB.ua_free_block_max < size)
        {//���Ŀɷ����ߴ�С��������ڴ�,û���ڴ���Է���
            int_save_asyn_signal();
            wait=true;  //����Ǿ���������������ִ��,whileѭ����Ҫ�ٴ��ж�
                        //�Ƿ����㹻���ڴ����.
            __djy_cut_ready_event(pg_event_running);

            pg_event_running->next = NULL;
            pg_event_running->previous = NULL;

            pg_event_running->wait_mem_size = size; //���ȴ����ֽ���
            event = tg_MCB.mem_sync;       //��ȡ�ڴ�ȴ���ָ��

            //���°��¼������ڴ�ȴ�������
            pg_event_running->sync_head = &tg_MCB.mem_sync;
            if(event == NULL)    //�ȴ����п�
            {
                pg_event_running->multi_next = pg_event_running;
                pg_event_running->multi_previous = pg_event_running;
                tg_MCB.mem_sync = pg_event_running;
            }else
            {
                do
                {//��ѭ���ҵ���һ�������ڴ�������¼����¼�.
                    if(event->wait_mem_size < size)
                        event = event->multi_next;
                    else
                        break;
                }while(event != tg_MCB.mem_sync);
                //���û���ҵ������ڴ�����¼������¼�,���¼��������β,��
                //����β������event_wait��ǰ��,��ʱeventǡ�õ���event_wait
                //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
                //��������ǰ��ѭ���ҵ����,����ʹ�����д���
                pg_event_running->multi_next = event;
                pg_event_running->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = pg_event_running;
                event->multi_previous = pg_event_running;
            }

            pg_event_running->last_status.all
                            = pg_event_running->event_status.all;
            pg_event_running->event_status.bit.wait_memory = 1;
            if(timeout != cn_timeout_forever)
            {
                pg_event_running->event_status.bit.wait_overtime = 1;
                __djy_addto_delay(timeout-u32l_rest_time);
            }

            int_restore_asyn_signal();  //���жϲ���ʵ�ֵ���
            //�������ﷵ�أ��ǳ�ʱ�������¼��ͷ��ڴ�
            if(pg_event_running->event_status.bit.wait_memory)
            {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ���ڴ�ͬ��������ȡ���¼���
                pg_event_running->event_status.bit.wait_memory = 0;
                return false;   //��ʱ����
            }else   //ͬ������������ɣ�˵�����߳��ͷ��ڴ棬������һ��whileѭ��
            {       //����Ƿ����㹻���ڴ�,
                u32l_rest_time = djy_get_time() - u32l_start_time;
                //����������������ж��Ǳ�Ҫ�ģ������߳��ͷ��ڴ�ἤ��¼���
                //���ͷ��ڴ��ʱ������볬ʱʱ���ٽ�,�����������ȼ��Ĺ�ϵ���¼�
                //���������õ�ִ�У����¼�������ʼִ��ʱovertime����Ҳ�Ѿ�������
                //�����ʱ�ڴ���Ȼ���㣬��Ӧ�ð���ʱ����
                if((u32l_rest_time >= timeout)
                    &&(tg_MCB.ua_free_block_max < size))
                {
                    return false;
                }
            }
        }
    }while(wait==true);
    return true;    //return trueǰ�ǲ����жϵ�,�����ڷ����ڴ�֮ǰ�����ж�.
}

//----���¼�����ȴ�����-------------------------------------------------------
//����: ���¼�ֱ�ӷŽ��ȴ�����,������.�ȴ�������һ����������ڴ��С�������е�
//      ˫��ѭ������.���ڴ�ߴ���������������ȼ��Ŷӣ��������㿼��:1���������
//      �̶���������2����Ӧ�ó�������У�һ�㲻���ڷǳ����������ȼ��ǳ��ߵ�
//      �¼�������ʹ�ö�̬���䷽����
//����: event,������ȴ����е��¼�ָ��
//����: ��
//-----------------------------------------------------------------------------
void __m_wait_memory(struct  event_ECB *event)
{
    struct  event_ECB *pl_event;
    ptu32_t size;

    size = event->wait_mem_size;
    pl_event = tg_MCB.mem_sync;     //��ȡ�ڴ�ȴ���ָ��
    pl_event->sync_head = &tg_MCB.mem_sync;
    if(pl_event == NULL)            //�ȴ����п�
    {
    	event->next = NULL;
    	event->previous = NULL;
        
    	event->multi_next = event;
    	event->multi_previous = event;    
        
    	tg_MCB.mem_sync = event;
    }else
    {
        do
        {//��ѭ���ҵ���һ�������ڴ�������¼����¼�.
            if(pl_event->wait_mem_size < size)
                pl_event = pl_event->multi_next;
            else
                break;
        }while(pl_event != tg_MCB.mem_sync);
        //���û���ҵ������ڴ�����¼������¼�,���¼��������β,������
        //β������event_wait��ǰ��,��ʱeventǡ�õ���event_wait
        //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
        //��������ǰ��ѭ���ҵ����,����ʹ�����д���
        event->multi_next = pl_event;
        event->multi_previous = pl_event->multi_previous;
        pl_event->multi_previous->multi_next = event;
        pl_event->multi_previous = event;
    }
    return;
}

//----����running�¼�������ڴ�------------------------------------------------
//����: ���ڴ������¼�id���в�����event_id�¼�����ľֲ��ڴ棬����У���Ǽ�
//      enum_mem_leak���󣬲�ǿ���ͷ�֮��
//����: event_id��Ŀ���¼�id
//����: ��
//-----------------------------------------------------------------------------
void __m_cleanup(uint16_t event_id)
{
}

//----��ʼ���ڴ��-------------------------------------------------------------
//���ܣ�1.�Ӷ��з�����ʿ��ƽṹ������ڴ棬λ���ڶѵĶ�����ռ��������
//      2.��ʼ���ڴ������ƽṹ,��ʼ��ȫ���ڴ�ṹ struct mem_global
//      3.��ʼ���ڴ�����
//���أ�1=�ɹ���0=ʧ��
//��ע: 1.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//      2.���ñ�����ǰ,ֻ����ִ�о�̬���书��,����ִ�ж�̬���书��.
//      3.Ŀǰֻ�����ڲ���mmu���������mmu�ر���֧�ֶ���̵������û����á�
//-----------------------------------------------------------------------------
ptu32_t module_init_heap_dynamic(ptu32_t para)
{
    ptu32_t ua_temp,ua_temp1=0,ua_temp2,ua_temp3 = 0,ua_faction;
    ptu32_t ua_pages,ua_table_size=0;
    ufast_t uf_classes,uf_grades;
    ucpu_t  ***pppl_bitmap,**ppl_bitmap,*pl_bitmap;
    ufast_t *pl_classes;


//    for(ua_temp=0;ua_temp<cn_ptr_bits;ua_temp++)
//    {   //����ҳ�ߴ���1�ĸ���,
//        if(cn_page_size&(1<<ua_temp))
//            ua_temp1++;
//    }
//    if(ua_temp1!=1) //ҳ�ߴ���ֻ��һ��1ʱ˵��������2�������η�
//        return(enum_mem_page_error);
//ע�͵���ԭ��:cn_page_size�������ó�������,�϶���2�������η�.

    tg_MCB.dynamic = true;

#if(cn_virtual_memory == 0)
    ua_faction=(ptu32_t)(tg_MCB.heap_top-tg_MCB.heap_bottom);
    //����ѵ���ҳ����ȡ��ҳ����ȥ��������
    ua_pages= ua_faction>>cn_page_size_suffix_zero;
    //�����ҳ����������������������ڴ���ƽṹʹ�á�
    ua_faction=ua_faction-(ua_pages<<cn_page_size_suffix_zero);
    tg_MCB.heap_top -=ua_faction;
#else
    ua_faction = 0;     //����������ֹ����
//    ua_faction = tg_MCB.heap_top;
//    //���¶���ҳ�߽�
//    tg_MCB.heap_top = tg_MCB.heap_top & (~(cn_page_size-1));
//    //���϶���ҳ�߽�
//    tg_MCB.heap_bottom = (tg_MCB.heap_bottom + cn_page_size -1)
//                                & (~(cn_page_size-1));
//    ua_faction -= tg_MCB.heap_top;
#endif

    if(ua_pages < 2) //�������ҳ��С��2ҳ,���ƽṹ���ܻ�Ҫռ��1ҳ,����������?
    {
        return 0;
    }
    mutex_create_knl(&tg_mem_mutex,"dynamic memory allocation");

    ua_temp = __m_calculate_expense(ua_pages); //�������ƽṹ��Ҫ���ֽ���
    if(ua_faction < ua_temp)
    {
        //�������ƽṹ��ua_faction�⻹��Ҫ��ҳ��
        ua_temp = (ua_temp-ua_faction+cn_page_size-1)>>cn_page_size_suffix_zero;
        for(ua_temp1 = ua_temp; ua_temp1 > 0; ua_temp1--)
        {//������ҳ���϶����ڻ����ʵ����Ҫ��ҳ��,�������ڴ�ҳ���϶�ʱ.
         //����ֵ�ӳ�����ҳ����ʼ��һ��С,ֱ������Ϊֹ.
            ua_temp2 = __m_calculate_expense(ua_pages-ua_temp1);//�ع����ƽṹ�ֽ���
            ua_temp3 = (ua_temp2-ua_faction+cn_page_size-1)
                        >> cn_page_size_suffix_zero;//���¼�����ƽṹҳ��
            if(ua_temp3 >= ua_temp1)
            //������ҳ���պô��ڻ���ڹ���ֵʱ,ѭ����ֹ
                break;
        }
        ua_pages = ua_pages -ua_temp3;//ʵ�ʿɷ����ҳ��,u32_temp3Ϊ���ƽṹҳ��
    //    (tg_MCB.heap_top) -= 1;
        tg_MCB.heap_top
                        -= ua_temp3<<cn_page_size_suffix_zero;  //�����Ѷ���λ��
    }
    tg_MCB.ua_pages_num = ua_pages;
    tg_MCB.free_pages_num = ua_pages;
    //�ڴ���ƽṹ����
    memset(tg_MCB.heap_top,0,ua_faction+(ua_temp3<<cn_page_size_suffix_zero));

    //�����ܽ�������һ���ж�����ҳ�ߴ�,����:��ַ���ȼ�ȥua_pages��ǰ��0������
    //����,ua_pages=5,���λλ����bit2,��29��ǰ��0��ҳ�ߴ���1,2,4ҳ3��,����=3
    uf_grades=cn_ptr_bits-__m_leading_zero((u8 *)&ua_pages,sizeof(ptu32_t));

    tg_MCB.uf_grades = uf_grades;    //�ܽ���
    tg_MCB.ua_block_max=cn_page_size<<(uf_grades-1);//��߽׿�ߴ�
    tg_MCB.ua_free_block_max=tg_MCB.ua_block_max; //�����ÿ�ߴ�

    //����λͼָ��������ָ����
    ua_table_size=0;
    for(ua_temp=0;ua_temp<uf_grades;ua_temp++)
    {//��������
        ua_temp1 = ua_pages>>ua_temp;   //����ua_temp��0��λͼλ��
        do
        {
            //������һ��λͼ��λ��
            ua_temp1=(ua_temp1+cn_cpu_bits-1)>>cn_cpu_bits_suffix_zero;
            ua_table_size++;        //��Ҫһ��ָ��ָ��ü�λͼ�׵�ַ
        }while(ua_temp1>1); //��һ��λͼ����1λ(��������cn_cpu_bitsλ),����ѭ��
    }

    //�¼�ָ��id��,��ϸ˵����mem_global_t���Ͷ��岿��.
    tg_MCB.index_event_id=(uint16_t*)tg_MCB.heap_top;

    //���׵�λͼ������ָ�룬ÿ����һ��ufast_t�͵�����ʾ�ý׵�λͼ����
    ua_temp = (ptu32_t)(tg_MCB.index_event_id+ua_pages);
    ua_temp = (ua_temp+sizeof(ufast_t)-1)&(~(sizeof(ufast_t)-1));       //����
    pl_classes=(ufast_t *)ua_temp;
    tg_MCB.p_classes=pl_classes;     //���׿��н�������������ָ��

    //����Ǹ��������ָ���ָ����ַ
    ua_temp = (ptu32_t)(pl_classes+uf_grades);
    ua_temp = (ua_temp+sizeof(void *)-1)&(~(sizeof(void *)-1));         //����
    pppl_bitmap=(void*)ua_temp;
    tg_MCB.ppp_bitmap=pppl_bitmap; //���׿��н�����λͼָ����ָ������ָ��

    ppl_bitmap=(ucpu_t **)(pppl_bitmap+uf_grades);  //���н�����λͼָ����ָ��
    pl_bitmap=(ucpu_t *)(ppl_bitmap+ua_table_size); //���н�����λͼָ���

    //����������ͷ����
    for(ua_temp=0;ua_temp<uf_grades;ua_temp++)
    {//��������
        ua_temp1 = ua_pages>>ua_temp;       //ua_temp��λͼ��λ��
        uf_classes=0;                       //ua_temp�׵�λͼ����
        pppl_bitmap[ua_temp]=ppl_bitmap;    //ua_temp��λͼ���ָ��
        do
        {
            ucpu_t  msk=~0; //����λ��δ֪,�ô˷�����ȫ1
            ua_temp3=ua_temp1>>cn_cpu_bits_suffix_zero; //������Ĵ�С(��������)
            ua_temp1=ua_temp1%cn_cpu_bits;    //��·������Ĵ�С(��������)
            if(ua_temp1==0)
            //����û�ж����λ,���账��
                ua_temp1=ua_temp3;  //����λͼ��ռ��������һ����λ��
            else
            {//�����ж����λ,��1,ʹ�����λ��ͬ���ѷ����λ.
                msk=msk<<ua_temp1;              //ʹ�����λ��1
                *(pl_bitmap+ua_temp3) |=msk;    //�޸�λͼ����Ӧ��λ
                ua_temp1=ua_temp3+1;    //·��������,������������
            }
            *ppl_bitmap++ = pl_bitmap;  //��¼����λͼָ��
            pl_bitmap +=ua_temp1;       //λͼָ��ָ����һ��λͼ��ʼ��ַ
            uf_classes++;                //ua_temp�׵ļ�������
        }while(ua_temp1>1);             //ֱ��������������λͼ��
        pl_classes[ua_temp]=uf_classes; //�ü�·�������
    }
//    if(cn_mem_recycle)    //�ṩ�ڴ���չ���,����С�ڴ治Ҫʹ���ڴ����
//    {//��ʱ��ʼ��δ���,���迼���жϹرյ�����.
//        pg_mem_record_free=__inc_record_link();
//    }
    return 1;
}

//----���ڴ���з����ڴ�-------------------------------------------------------
//���ܣ�1.����ڴ�ߴ磬��������Ҫ�����С�ڴ�ߴ磬����ÿ��ڴ�ߴ�Ľ���
//      2.��ȡ�ü�����·����ȣ��ط���·���𼶲��ң�ֱ���ҵ������ڴ�Ϊֹ��
//      3.�����ڴ�����
//      4.���ڴ������Ϣ����������,�Ա������ڴ���.
//      5.����ڴ治�㣬����¼�����tg_mem_global.event_wait��,�������¼��л�.
//������size,��������ڴ��ߴ�
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: ����ڶ��¼���������ǰ���ñ���������¼ӵ����ʱȫ������ϵͳ�����¼��С�
//      ϵͳ�����¼������������ʵ�ͬ��ȫ�ַ��䡣
//-----------------------------------------------------------------------------
void *m_malloc(ptu32_t size,u32 timeout)
{
    u8 *ua_address;
    ufast_t  uf_grade_th;
    bool_t   en_scheduler;
    void *result;
    uint16_t *pl_id,id;
    u32 page;

    if(size == 0)
        return NULL;
    //�������¼����Ⱥ�dynamic��Ա�����ٷ����仯����ʹ��������Ҳ�ǰ�ȫ��
    if(tg_MCB.dynamic==false)
        return(__m_static_malloc(size));  //�ڴ���δ��ʼ��,ִ��׼��̬�ڴ����
    //�����ڴ�ֱ���ж�size�Ƿ�����,��Ϊȡ�û�����ǰ���ܷ����л����ж���Ч.
//    int_save_asyn_signal();
    if(mutex_pend(&tg_mem_mutex,timeout) == false)
        return NULL;
    en_scheduler = djy_query_sch();
    if((tg_MCB.ua_free_block_max < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( !__m_check_memory(size,timeout) )            //û�к��ʵĿ����ڴ��
        {
            result = NULL;
        }else    //�к��ʵĿ����ڴ��
        {
            uf_grade_th=__m_get_grade(size);          //ȡ�׺�
            ua_address=__malloc_block(uf_grade_th); //�����ڴ�
            pg_event_running->local_memory++;

//            __record_mem(ua_address,uf_grade_th);   //��¼�ڴ�
            //�Ķ���������������mem_global_t��index_event_id��Ա�����ע��.
            pl_id = tg_MCB.index_event_id;
            id = pg_event_running->event_id;
            page = (ptu32_t)(ua_address-tg_MCB.heap_bottom)
                    >>cn_page_size_suffix_zero;
            if(uf_grade_th==0)
            {//����1ҳ
                pl_id[page] = id;
            }else if(uf_grade_th==1)
            {//����2ҳ
                pl_id[page] = (u16)-1;
                pl_id[page+1] = id;
            }else
            {   //�����ҳ
                pl_id[page] = (u16)-2;
                pl_id[page+1] = id;
                pl_id[page+2] = uf_grade_th;
            }
            result = ua_address;
        }
    }
    mutex_post(&tg_mem_mutex);
//    int_restore_asyn_signal();
    return result;
}

//----����ȫ���ڴ�-------------------------------------------------------------
//���ܣ�1.����ڴ�ߴ磬��������Ҫ�����С�ڴ�ߴ磬����ÿ��ڴ����ڵڼ��ס�
//      2.��ȡ�ü�����·����ȣ��ط���·���𼶲��ң�ֱ���ҵ������ڴ�Ϊֹ��
//      3.�����ڴ�����
//      4.����ڴ治�㣬����¼�����tg_mem_global.event_wait��,�������¼��л�.
//������size,��������ڴ��ߴ�
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: �ô˺���������ڴ�,���������¼����ʱ���ջ�.
//-----------------------------------------------------------------------------
void *m_malloc_gbl(ptu32_t size,u32 timeout)
{
    u8 *ua_address;
    ufast_t  uf_grade_th;
    uint16_t    *pl_id;
    bool_t   en_scheduler;
    void *result;

    if(tg_MCB.dynamic==false)
        return(__m_static_malloc(size));  //�ڴ���δ��ʼ��,ִ��׼��̬�ڴ����
     //�����ڴ�ֱ���ж�size�Ƿ�����,��Ϊ��ȡ�û�����ǰ���ܷ����л����ж���Ч.
        if(mutex_pend(&tg_mem_mutex,timeout) == false)
            return NULL;
        en_scheduler = djy_query_sch();
    if((tg_MCB.ua_free_block_max < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( ! __m_check_memory(size,timeout))
        {
            result = NULL;
        }else
        {
            uf_grade_th=__m_get_grade(size);          //ȡ�׺�
            ua_address=__malloc_block(uf_grade_th); //�����ڴ�

            //������id���м�¼���η��������
            //�Ķ����δ�������mem_global_t��index_event_id��Ա�����ע��.
            pl_id = &tg_MCB.index_event_id
                    [(ptu32_t)(ua_address-tg_MCB.heap_bottom)
                            >>cn_page_size_suffix_zero];
            if(0==uf_grade_th)
                *pl_id = (u16)-3;
            else
            {
                *pl_id++ = (u16)-4;
                *pl_id = uf_grade_th;
            }
            result = ua_address;
        }
    }
    mutex_post(&tg_mem_mutex);
    return result;
}

//----����1���ڴ�--------------------------------------------------------------
//����: ���ڴ���з���һ���ڴ��,�ڲ�����,�����Ÿ��û�
//����: grade,������Ŀ�Ľ׺�,0=1��,1=2��,����֮
//����: ��õ��ڴ��ָ��,������ܷ����򷵻�NULL
//-----------------------------------------------------------------------------
void *__malloc_block(ufast_t grade)
{
    ptu32_t ua_temp1;
    ptu32_t ua_pages_number;
    ufast_t uf_word_shift,uf_grade_th,uf_classes;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;

    ptu32_t ua_word_offset,ua_bit_num,ua_loop;
    ucpu_t  uc_msk;               //����ƫ����

    pppl_bitmap=tg_MCB.ppp_bitmap; //���н�����λͼָ���ָ������ָ��
    ppl_bitmap=pppl_bitmap[grade];        //�ý׿��н�����λͼָ������ַ
    pl_classes=tg_MCB.p_classes;   //��ȡ���׿��н�����������ָ��.
    uf_classes=pl_classes[grade]-1;       //�ý׿��н��������һ��ָ����
                                          //ppl_bitmap�е�ƫ����
    tg_MCB.free_pages_num -= (1<<grade); //ʣ�����ҳ��
    ua_pages_number=0;
    do
    {//��ѭ�����ҵ�һ���ɷ����(��bit=0)��ƫ��λ��,�Ӹߵ��Ͳ���,����:
     //����,�ҳ��ý׿���λͼ��������߼�λͼ�е�һ��bit=0��λƫ����.
     //Ȼ��,�������ƫ��������θ߼���0λ����ƫ��,�������ֲ��ҳ�bit=0��λƫ��.
     //��˷��������ҵ����һ����һ��0λ��ƫ��,����Ǳ��η����Ŀ���.

        pl_bitmap=ppl_bitmap[uf_classes];   //��ȡ����λͼ��ָ��

        //ua_pages_number����һ����λƫ��,ua_temp1�Ǳ������һ�ֵ�����λƫ��
        ua_temp1=cn_cpu_bits-1-__m_leading_ucpu_zero(~pl_bitmap[ua_pages_number]);

        //��һ������ƫ�ơ��ֳ�+��������λƫ�Ƶõ�������λƫ��
        ua_pages_number =(ua_pages_number<<cn_cpu_bits_suffix_zero)+ua_temp1;
    }while(uf_classes-- >0);

    ua_pages_number <<= grade;  //Ŀ������ҳ��ҳ��

    //���濪ʼ��bitmap���Ǳ��η������
    for(uf_grade_th=0;uf_grade_th<=grade;uf_grade_th++)
    {//��0��ֱ����ǰ��(������),����׺ŵ��ڵ���������ڴ��λͼ.
        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset = ua_pages_number>>uf_grade_th;
        ppl_bitmap=pppl_bitmap[uf_grade_th];    //ȡ�øý׷����ָ������ַ
        ua_bit_num=1<<(grade-uf_grade_th);      //��������λ��
        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {//�ӵͼ����߼������ý׸���λͼ
            pl_bitmap=ppl_bitmap[ua_temp1]; //��Ӧ����Ӧ·���������ַ
            //��������λƫ��,������ʽ��ȡģ������
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //��ƫ��
            if(ua_bit_num>=cn_cpu_bits)   //ua_bit_num���������Ҫ��1��λ
            {//λ�����ڻ����1���ֿ�,��Ϊ����2��n�η��ֽ�,����Ҫ�����λ���϶�
             //����������,������������1�Ϳ�����.

                //��������,Ҳ����һ��λͼ������1��λ��
                ua_bit_num >>=cn_cpu_bits_suffix_zero;
                for(ua_loop =0; ua_loop < ua_bit_num; ua_loop++)
                    pl_bitmap[ua_word_offset+ua_loop]=cn_allbit_1;
            }else if(ua_bit_num>0)
            //λ��С��һ���ֿ�,�Ҵ���0
            {
                uc_msk=cn_low_xbit_msk[ua_bit_num]<<uf_word_shift;
                pl_bitmap[ua_word_offset] |=uc_msk;
                if(pl_bitmap[ua_word_offset] == cn_allbit_1)
                //����1��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                    ua_bit_num=0;
                else
                    break;
            }else
            {//ǰһ�β���ʹһ���ִӷ�ȫ1���ȫ1����λ���ֶ�Ӧ����һ��·����Ӧλ
                pl_bitmap[ua_word_offset] |= 1<<uf_word_shift;
                if(pl_bitmap[ua_word_offset] == cn_allbit_1)
                //����1��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                    ;
                else
                    break;
            }
        }
    }

    for(; uf_grade_th < tg_MCB.uf_grades; uf_grade_th++)
    {//��ǰ��(����)����߽�,����׺Ÿ��ڻ���ڵ���������ڴ��λͼ.ÿ��ֻ��1λ��1

        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset = ua_pages_number >> uf_grade_th;
        if(ua_word_offset >= tg_MCB.ua_pages_num>>uf_grade_th)
        //�ý��޶�Ӧ��,���ǿ��ܵ�,����һ����10ҳ���ڴ�,��9ҳ�͵�10ҳ���ڵ�3����û��
        //ӳ��.����ʽ�ұ��Ǳ����ܿ���,����Ǵ�0��Ƶ�ƫ����,���ֻ�����ܿ���-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //ȡ�øý׷����ָ������ַ

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //��������λƫ��,������ʽ��ȡģ������
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //��ƫ��
            pl_bitmap[ua_word_offset] |= 1<<uf_word_shift;
            if(pl_bitmap[ua_word_offset] == cn_allbit_1)
            //����ģ��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                ;
            else
                break;
        }
    }

    //ˢ���������ڴ��
    uf_grade_th=tg_MCB.uf_grades-1;     //��߽׺�
    do{
        uf_classes= pl_classes[uf_grade_th];        //��uf_grade_th���ܼ���
        ppl_bitmap   = pppl_bitmap[uf_grade_th];    //λͼ������ָ��
        pl_bitmap    = ppl_bitmap[uf_classes-1];    //��߼�λͼָ��
        if(*pl_bitmap != (~0))
        {//·������ֻҪ����ȫ1�ͱ�ʾ�ý׿϶��п��п�.
            //���ݽ׺ż��������п�ĳߴ�.
            tg_MCB.ua_free_block_max=cn_page_size<<uf_grade_th;
            break;
        }
    }while(uf_grade_th-- >0);   //����߽�(����)ɨ�赽0��

    return tg_MCB.heap_bottom
                    +(ua_pages_number<<cn_page_size_suffix_zero);
}

//----��ȡ��ʽ���ߴ�-----------------------------------------------------------
//����: ��ȻӦ�ó���������������ߴ��ڴ棬���ڴ������������û����ڴ�ȴ������
//      �û�����Ĺ�񻯳ߴ���ڴ�飬����������Ӧ�ó���ù�񻯳ߴ磬����ʵ��
//      ִ�з����ڴ�Ĳ�����Ҳ����֤���ķ�������ܷ�ɹ���
//����: size��Ӧ�ó���������ĳߴ�
//����: ��񻯳ߴ�
//-----------------------------------------------------------------------------
ptu32_t m_format_size(ptu32_t size)
{
    if(tg_MCB.dynamic==false)
        return align_up_sys(size);  //׼��̬���䣬���뼴��
    else
        return(cn_page_size << (__m_get_grade(size)+cn_page_size_suffix_zero));
}

//----�ͷŴӶѷ�����ڴ�-------------------------------------------------------
//����: �ͷŴӶ��з����һ���ڴ棬�����ʱ��̬���仹û�г�ʼ������ִ��׼��̬
//      �ڴ��ͷš��ͷ��ڴ�����¼��������п�ĳߴ磬������¼����ڴ�ͬ��
//      �����У���ѵȴ����ڴ�ߴ�С���µ������п�ߴ���¼����
//������pl_mem,���ͷŵ��ڴ�ָ��
//���أ����󷵻�flase,��ȷʱ����true
//-----------------------------------------------------------------------------
bool_t m_free(void * pl_mem)
{
    ptu32_t ua_temp1;
    ptu32_t ua_word_offset;
    ufast_t uf_word_shift;  //����ƫ����
    ptu32_t ua_pages_no;
    ufast_t uf_grade_th,uf_free_grade_th;
    ufast_t uf_classes;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;
    uint16_t *pl_id;    //id;
    struct  event_ECB  *event;
    ptu32_t ua_bit_num;
    ucpu_t  uc_msk;

    if(pl_mem == NULL)
        return false;
    //�����õ���tg_mem_global�е�dynamic��heap_bottom��heap_top��Ա����������
    //�¼����Ⱥ󣬾����ٱ仯���������ǰ�ȫ�ġ�
    if(tg_MCB.dynamic==false)
        return(__m_static_free(pl_mem));  //�ڴ���δ��ʼ��,ִ��׼��̬�ڴ����
    if(((u8*)pl_mem<tg_MCB.heap_bottom)
                || ((u8*)pl_mem>=tg_MCB.heap_top))
    {
        djy_error_login(enum_mem_error,NULL);  //���ͷŵ��ڴ治���ڴ����,ֱ���˳�.
        return false;
    }

    //�����ͷŵ��ڴ�����ҳҳ��
    ua_pages_no=(ptu32_t)((ptu32_t)pl_mem-(ptu32_t)tg_MCB.heap_bottom)
                        >>cn_page_size_suffix_zero;

    mutex_pend(&tg_mem_mutex,cn_timeout_forever);
    //�����ͷŵ��ڴ��Ľ׺�,��0���.ͨ���׺�Ҳ����ȷ���ڴ��Ĵ�С.
    //ȷ���ڴ�������,�ֲ��ڴ���Ҫ֪��ӵ�и��ڴ���¼�id��
    //ȫ���ڴ����������ڴ������ٱ�,����֪��ӵ�и��ڴ���¼�id
    pl_id = &tg_MCB.index_event_id[ua_pages_no];
    switch(pl_id[0])
    {
        case 0xffff :
        {   //˫ҳ�ֲ��ڴ�,-1+id
//            id = pl_id[1];
            uf_free_grade_th = 1;
        }break;
        case 0xfffe :
        {   //��ҳ�ֲ��ڴ�:-2+id+�׺�
//            id = pl_id[1];
            uf_free_grade_th = pl_id[2];
        }break;
        case 0xfffd :
        {   //��ҳȫ���ڴ�:-3
            uf_free_grade_th = 0;
        }break;
        case 0xfffc :
        {   //˫(��)ҳȫ���ڴ�:-4+�׺�.
            uf_free_grade_th = pl_id[1];
        }break;
        default :
        {
            if(pl_id[0] <= cn_event_id_limit)
            {   //��ҳ�ֲ��ڴ�:id,
//                id = pl_id[0];
                uf_free_grade_th = 0;
            }else
            {
                djy_error_login(enum_mem_error,NULL);   //ָ���д�,ֱ���˳�
                return false;

            }
        }break;
    }
/*
    if(0xffff != id)
    {   //���ͷŵ��Ǿֲ��ڴ�,��Ҫɾ���ڴ�����¼���.
        //������������,id ����running,��Ҳ����������,����ֱ����runningָ��.
        event = __djy_lookup_id(id);
        record = event->held_memory;
        if(record == NULL)
        {
            djy_error_login(enum_mem_error,NULL);  //�����,ֱ���˳�.
            return false;
        }
        if(record->address == pl_mem)
        {   //���һ����¼ƥ��
            event->held_memory = record->next;
            record->next = pg_mem_record_free;  //��ɾ���Ľ������free��ͷ��
            pg_mem_record_free = record;        //�ƶ�freeָ��
        } else
        {
            for(;record->next != NULL;record = record->next)
            //ѭ��ֱ���ҵ���ɾ���Ľ���ǰһ�����
                if(record->next->address == pl_mem)
                    break;
            if(record->next ==NULL)
            {
                djy_error_login(enum_mem_error,NULL);//������û����������¼,ֱ���˳�
                return false;
            }else
            {   //�ҵ���¼
                struct mem_record *p;
                p = record->next;               //ȡ�ô�ɾ���Ľ��
                record->next = p->next;         //ɾ�����
                p->next = pg_mem_record_free;   //��ɾ���Ľ������free��ͷ��
                pg_mem_record_free=p;           //�ƶ�freeָ��
            }
        }
    }
*/
    pppl_bitmap=tg_MCB.ppp_bitmap; //���н�����λͼָ���ָ������ָ��
//    ppl_bitmap=pppl_bitmap[uf_free_grade_th];//�ý׿��н�����λͼָ������ַ
//    pl_bitmap=ppl_bitmap[0];                 //���н�����λͼָ��
    pl_classes=tg_MCB.p_classes;    //��ȡ���׿��н�����������ָ��.
    uf_classes=pl_classes[uf_free_grade_th]-1;  //�ý׿��н��������һ��ָ����
                                                //ppl_bitmap�е�ƫ����

    //����λƫ��
//    ua_word_offset =ua_pages_no>>uf_free_grade_th;
    //��������λƫ��,������ʽ��ȡģ������
//    uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
//    ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //������ƫ��

    //����ҳ������
    tg_MCB.free_pages_num += 1<<uf_free_grade_th;

    //���濪ʼ��bitmap���Ǳ����ͷ����
    for(uf_grade_th = 0; uf_grade_th <= uf_free_grade_th; uf_grade_th++)
    {//��0��ֱ����ǰ��(������),����׺ŵ��ڵ���������ڴ��λͼ.
        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset =ua_pages_no>>uf_grade_th;
        ppl_bitmap=pppl_bitmap[uf_grade_th];        //ȡ�øü������ָ������ַ
        ua_bit_num=1<<(uf_free_grade_th-uf_grade_th);       //��������λ��
        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {//�ӵͼ����߼������ý׸���λͼ
            pl_bitmap = ppl_bitmap[ua_temp1]; //��Ӧ����Ӧ·���������ַ
            //��������λƫ��,������ʽ��ȡģ������
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset = ua_word_offset>>cn_cpu_bits_suffix_zero;  //��ƫ��
            if(ua_bit_num>=cn_cpu_bits)   //ua_bit_num���������Ҫ��0��λ
            {//λ�����ڻ����1���ֿ�,��Ϊ����2��n�η��ּ�,����Ҫ�����λ���϶�
             //����������,������������0�Ϳ�����.

                //��������,Ҳ����һ��λͼ������0��λ��
                ua_bit_num >>=cn_cpu_bits_suffix_zero;
                //uc_word_shift�����ﵱ��ʱ����ʹ��
                for(uf_word_shift =0; uf_word_shift < ua_bit_num; uf_word_shift++)
                    pl_bitmap[ua_word_offset+uf_word_shift]=0;
            }else if(ua_bit_num > 0)
            {//λ��С��һ���ֿ�,�Ҵ���0
                uc_msk=~(cn_low_xbit_msk[ua_bit_num]<<uf_word_shift);
                pl_bitmap[ua_word_offset] &=uc_msk;
                if(pl_bitmap[ua_word_offset] == uc_msk)
                //����0��λ���ڵ���ȫ1ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                    ua_bit_num=0;
                else
                    break;
            }else
            {//ǰһ�β���ʹһ���ִ�ȫ1->��0�����ֶ�Ӧ����һ��·����Ӧλ��0
                pl_bitmap[ua_word_offset] &= ~(1<<uf_word_shift);
                if(pl_bitmap[ua_word_offset] == ~(1<<uf_word_shift))
                //����0��λ���ڵ���ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                    ;
                else
                    break;
            }
        }
    }

    for(;uf_grade_th<tg_MCB.uf_grades;uf_grade_th++)
    {//��ǰ��(����)����߽�,����׺Ÿ��ڻ���ڵ���������ڴ��λͼ.ÿ��ֻ����1λ
        //ȡ��һ�׶�Ӧ���������нϵ͵�ַ��ƫ����
        ua_word_offset = (ua_pages_no >> uf_grade_th)<<1;
        //��������λƫ��,������ʽ��ȡģ������
        uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
        ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //��ƫ��
        ppl_bitmap=pppl_bitmap[uf_grade_th-1]; //ȡ�øü������ָ������ַ
        pl_bitmap = ppl_bitmap[0];
        if( (pl_bitmap[ua_word_offset] & (3<<uf_word_shift)))
            break;      //����λͼ�������㣬���ߵĽ�Ҳ��������
        
        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset=ua_pages_no>>uf_grade_th;
        if(ua_word_offset>=tg_MCB.ua_pages_num>>uf_grade_th)
        //�ý��޶�Ӧ��,���ǿ��ܵ�,����һ����10ҳ���ڴ�,��9ҳ�͵�10ҳ���ڵ�3����
        //��ӳ��.����ʽ�ұ��Ǳ����ܿ���,����Ǵ�0��Ƶ�ƫ����,���ֻ�����ܿ���-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //ȡ�øü������ָ������ַ

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //��������λƫ��,������ʽ��ȡģ������
            uf_word_shift=ua_word_offset&cn_low_xbit_msk[cn_cpu_bits_suffix_zero];
            ua_word_offset=ua_word_offset>>cn_cpu_bits_suffix_zero;  //��ƫ��
            pl_bitmap[ua_word_offset] &= ~(1<<uf_word_shift);
            if(pl_bitmap[ua_word_offset] == ~(1<<uf_word_shift))
            //����0��λ���ڵ���ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                ;
            else
                break;
        }
    }

    //ˢ���������ڴ��
    uf_grade_th=tg_MCB.uf_grades-1;     //ȡ��߽׵Ľ׺�
    do{
        uf_classes   = pl_classes[uf_grade_th];     //��uf_grade_th�׵��ܼ���
        ppl_bitmap   = pppl_bitmap[uf_grade_th];    //λͼ������ָ��
        pl_bitmap    = ppl_bitmap[uf_classes-1];    //��߼�λͼָ��
        if(*pl_bitmap != (~0))
        {//·������ֻҪ����ȫ1�ͱ�ʾ�ý׿϶��п��п�.
            //���ݽ׺ż��������п�ĳߴ�.
            tg_MCB.ua_free_block_max=cn_page_size<<uf_grade_th;
            break;
        }
    }while(uf_grade_th-- >0);   //����߽�(����)ɨ�赽0��

//    if(0xffff == id)
//    {//����������,�����������ʱ�������ڴ����
     //1.����ֲ��ڴ�,������-->��¼���η����-->����pg_mem_record_free��
     //     -->û���ڴ�����������.
     //2.�ͷ�ȫ���ڴ�,ʹ�������ڴ���Է���.
     //3.��ʱ����ֲ��ڴ�,��¼ʱ���ܳ���.
//        if(pg_mem_record_free==NULL)
//            pg_mem_record_free=__inc_record_link();
//    }
    mutex_post(&tg_mem_mutex);
    pg_event_running->local_memory--;

    //���ڴ�ȴ������������ڴ�֮��С�ڵ�ǰ��������ڴ�ļ����¼��ŵ�ready����
    //�ȴ�������˫��ѭ������
    int_save_asyn_signal();
    if(tg_MCB.mem_sync != NULL)
    {
        event = tg_MCB.mem_sync;   //ȡ�ȴ�����ͷ
        ua_temp1 = tg_MCB.ua_free_block_max;
        while(1)
        {
            //ua_word_offset�ڴ�Ȩ����ʱ����ʹ��,��񻯵�ǰ�¼���Ҫ���ڴ�.
            ua_word_offset = __m_get_grade(event->wait_mem_size)
                            << cn_page_size_suffix_zero;
            if(ua_word_offset <= ua_temp1)
            {//�¼��ȴ����ڴ���С���������ڴ��,���¼��ŵ���������
                //ʣ����óߴ���ٵ�ǰ������¼���������ڴ�,������ʵ��ִ������
                ua_temp1 -= __m_get_grade(event->wait_mem_size)
                            << cn_page_size_suffix_zero;
                event->last_status.all = event->event_status.all;
//                event->event_status.bit.wait_memory = 0;
                event->sync_head = NULL;
                if(event->event_status.bit.wait_overtime)
                    __djy_resume_delay(event);
                event->event_status.all = 0;
                if(event->multi_next == event->multi_previous)
                {//�����һ���¼�
                    tg_MCB.mem_sync  = NULL;
                    break;
                }else
                {
                    tg_MCB.mem_sync = event->multi_next;
                    event->multi_next->multi_previous = event->multi_next;
                    event->multi_previous->multi_next = event->multi_previous;
                }
                __djy_event_ready(event);
            }else
                break;
        }
    }
    int_restore_asyn_signal();  //�ָ��ж�״̬
    return(true);
}

//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ������������ڴ��ĳߴ�
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  m_get_max_free_mem(void)
{
    return tg_MCB.ua_free_block_max;
}

//----��ѯ�ѿռ��С---------------------------------------------------------
//���ܣ����ضѿռ��С
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  m_get_heap_size(void)
{
    return tg_MCB.ua_pages_num<<cn_page_size_suffix_zero;
}

//----��ѯ�ܿ����ڴ��С-------------------------------------------------------
//���ܣ����ؿ����ڴ��ֽ���
//��������
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  m_get_free_mem(void)
{
    return tg_MCB.free_pages_num<<cn_page_size_suffix_zero;
}

//----��ѯҳ�ߴ�---------------------------------------------------------------
//���ܣ�����ҳ�ֽ���
//��������
//���أ�ҳ�ֽ���
//-----------------------------------------------------------------------------
ptu32_t  m_get_page_size(void)
{
    return cn_page_size;
}
#endif
