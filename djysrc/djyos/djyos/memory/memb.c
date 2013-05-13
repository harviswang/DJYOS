//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ڴ�ع���ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ�̶��������ԣ��ڴ������2��ָ�볤��(ͨ��Ϊ8�ֽ�)��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

//ÿ���ڴ�ض���һ��mem_cell_pool���͵Ľṹ,��Ҳ���ù̶�����䷨���з����.
static struct mem_cell_pool tg_pool_of_cell_pool[cn_mem_pools];
static struct mem_cell_pool *pg_pool_of_cell_pool;

//----��ʼ���̶������ģ��------------------------------------------------------
//����: ��ʼ������ϵͳ�Ĺ̶����ڴ����ģ��.
//����: ��.
//����: 1.
//----------------------------------------------------------------------------
ptu32_t module_init_memb(ptu32_t para)
{
    static struct mem_cell_pool cell_pool;
    cell_pool.cell_size = sizeof(struct mem_cell_pool);
    cell_pool.continue_pool = tg_pool_of_cell_pool;
    cell_pool.free_list = NULL;
    cell_pool.pool_offset = (ptu32_t)&tg_pool_of_cell_pool[cn_mem_pools];
    pg_pool_of_cell_pool = &cell_pool;
    rsc_add_tree_root(&cell_pool.memb_node,sizeof(struct mem_cell_pool),
                    "�̶�������");
    semp_create_knl(&cell_pool.memb_semp,
                      cn_mem_pools,cn_mem_pools,"�̶�������");
    return 1;
}

//----����һ���ڴ��-------------------------------------------------------
//����: ��ʼ��һ���ڴ�أ�ԭʼ�ڴ�ص��ڴ����û��ṩ���������
//      1.��ʼʱ,free_listָ���,continue_poolָ��һ�������ڴ�ء�
//      2.���free_list�ǿգ����ڴ��free_list����ͷ��ȡ��
//      3.���free_list�գ����ڴ��continue_pool��ȡ��
//      4.�ͷ��ڴ�ʱ�����ڴ�����ӵ�free_list����,�����Ƿ���continue_pool����.
//      �ڴ����һ���ٽ���Դ,����ʱ��Ҫ�ر��ж�,�����������ڴ��ȫ�����ӵ�
//      free_list,���������ܳ�ʱ��ر��ж�,�����ڲ��õķ���ÿ��ֻ����һ���ڴ��,
//      �������жϵ�ʱ�䱻���͵���С.
//����: pool_original,������û��ṩ���ڴ��.
//      init_capacital,ԭʼ�ڴ�صĳߴ�,�Կ�Ϊ��λ��
//      cell_size,�ڴ��ߴ磬��ϵͳ�ж���Ҫ�󣬱���Ϊָ�볤�ȵ�����������СΪ����ָ��
//			����.
//      increment,���������ڴ���е��ڴ浥Ԫ�ľ�ʱ�����Ӷ������ӵ��ڴ������
//          ��Ϊ0�����ӡ�ע�⣬�ڴ������󣬼�ʹ�û�����mb_free�ͷ����ڴ棬��
//          �����ͷ����ڴ���е�ȫ���ڴ棬�����������ڴ治�ᱻ�ջء�
//      limit,���increment !=0��limit�����ڴ�ص����������Է������Ƶ����ӣ���
//          ���ڴ�ľ���
//      name,���ڴ��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �ڴ��ָ��.
//----------------------------------------------------------------------------
struct mem_cell_pool *mb_create_pool(void *pool_original,u32 init_capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name)
{
    struct mem_cell_pool *pool;

    //�����ڴ��ʱ�����Բ��ṩ�ڴ�飬��ʱinit_capacitalҲ������0.
    //�˹��ܽ���ڴ�ض�̬�������ܣ��ɰ���Ӧ�ó����Ż��ڴ����ã�����:
    //ĳ���ڣ�������������ͨ��Э�飬ȡ�����ֳ�Ӧ�á����ǿ���ͬʱ��ʼ���ö���ͨ��
    //Э�����������Ϊÿ��ͨ��Э���ʼ��һ���յ��ڴ�أ��Ϳ���ʵ��ֻΪʵ�ʷ���
    //��ͨ��Э������ڴ档
    if(( pool_original == NULL) && (init_capacital != 0))
        return NULL;
//���������飬1�����ϵͳҪ�������ʣ����ߴ������ָ��ߴ�����������ڴ��
//��ʼ��ַҲ��ָ��ߴ����������2�����ϵͳ��Ҫ�������ʣ����ߴ����ָ��ߴ�
//�Ϳ����ˡ�
    if(cell_size < 2*sizeof(void*))
        return NULL;
#if(mem_align == 1)
    if((cell_size % sizeof(void*) != 0)
            || (((ptu32_t)pool_original) % sizeof(void*) != 0))
        return NULL;
#endif

    //����һ���ڴ�ؿ���ͷ
    pool = (struct mem_cell_pool *)mb_malloc(pg_pool_of_cell_pool,0);
    if(pool == NULL)
        return NULL;    //�ڴ�ؿ���ͷ���䲻�ɹ�

    pool->continue_pool = (void*)pool_original;     //�������׵�ַ
    pool->free_list = NULL;                 //��ʼʱ���������ǿյ�
    //����������ƫ�Ƶ�ַ
    pool->pool_offset = (ptu32_t)pool_original + init_capacital*cell_size;
    pool->cell_size = cell_size;
    pool->cell_increment = increment;
    pool->cell_limit = limit;
    rsc_add_son(&pg_pool_of_cell_pool->memb_node,
                &pool->memb_node,
                sizeof(struct mem_cell_pool),
                name);
    //init_capacital�п�����0
    semp_create_knl(&pool->memb_semp,init_capacital,init_capacital,name);
    return pool;
}

//----����һ���ں��ڴ��-------------------------------------------------------
//����: ����һ���ں��ڴ�أ�������_knlΪ��׺�ĺ���һ����cn_mem_pools���û�����
//      �ģ�Ϊ����ģ��Ķ����ԣ��ں˲����û�����cn_mem_poolsָ�꣬���ں�ʹ�õ�
//      �ڴ�ؿ��ƿ��õ����ĺ���������ɾ�������õĻ����������ں˺�Ӧ�ó���
//      �������
//      ��mb_create_pool�������ǣ���������Ҫ�������ṩ�ڴ�ؿ��ƿ�ʵ�塣
//����: pool,�û��ṩ���ڴ�ؿ��ƿ�
//      pool_original,������û��ṩ���ڴ��.
//      init_capacital,ԭʼ�ڴ�صĳߴ�,���ֽ�Ϊ��λ,�����Կ�Ϊ��λ,��Ϊ��ߴ罫��
//          ϵͳ����Ҫ�����,0��ʾֱ�Ӵ�ϵͳ�ڴ�������.
//      increment,���������ڴ���е��ڴ浥Ԫ�ľ�ʱ�����Ӷ������ӵ��ڴ浥Ԫ������
//          ��Ϊ0�����ӡ�ע�⣬�ڴ������󣬼�ʹ�û�����mb_free�ͷ����ڴ棬��
//          �����ͷ����ڴ���е�ȫ���ڴ棬�����������ڴ治�ᱻ�ջء�
//      limit,���increment !=0��limit�����ڴ�ص����ߴ磬�Է������Ƶ����ӣ���
//          ���ڴ�ľ���
//      cell_size,��ߴ�,��������Ҫ�����,ARM�е���Ϊ8��������.
//      name,���ڴ��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �ڴ��ָ��.
//ע: ��������ר��Ϊ�ں�׼���ģ��ʲ���������顣
//----------------------------------------------------------------------------
struct mem_cell_pool *mb_create_pool_knl(struct mem_cell_pool *pool,
                                void *pool_original,u32 init_capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name)
{
    pool->continue_pool = (void*)pool_original;     //�������׵�ַ
    pool->free_list = NULL;                 //��ʼʱ���������ǿյ�
    //����������ƫ�Ƶ�ַ
    pool->pool_offset = (ptu32_t)pool_original + init_capacital*cell_size;
    pool->cell_size = cell_size;
    pool->cell_increment = increment;
    pool->cell_limit = limit;
    rsc_add_son(&pg_pool_of_cell_pool->memb_node,
                &pool->memb_node,
                sizeof(struct mem_cell_pool),
                name);
    semp_create_knl(&pool->memb_semp,init_capacital,init_capacital,name);
    return pool;
}

//----ɾ��һ���ڴ��-----------------------------------------------------------
//����: ɾ��һ���ڴ�أ���ĳ�ڴ�ز�����Ҫʱ���ɵ��ñ�����.������ֻ�������ڴ��
//      ���ź�������Դ��㣬�ڴ�ػ������ǵ������ṩ�ģ�Ӧ�ɵ���������
//����: pool���ڴ��ָ��
//����: true= �ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t mb_delete_pool(struct mem_cell_pool *pool)
{
    if(!semp_delete_knl(&pool->memb_semp))
        return false;
    if(!rsc_del_node(&pool->memb_node))
        return false;
    mb_free(pg_pool_of_cell_pool,pool);
    return true;
}

//----ɾ��һ���ں��ڴ��-------------------------------------------------------
//����: ɾ��һ���ڴ�أ���ĳ�ڴ�ز�����Ҫʱ���ɵ��ñ�����.������ֻ�������ڴ��
//      ���ź�������Դ��㣬�ڴ�ػ������ǵ������ṩ�ģ�Ӧ�ɵ���������
//      ������_knlΪ��׺�ĺ���һ����cn_mem_pools���û�����
//      �ģ�Ϊ����ģ��Ķ����ԣ��ں˲����û�����cn_mem_poolsָ�꣬���ں�ʹ�õ�
//      �ڴ�ؿ��ƿ��õ����ĺ���������ɾ�������õĻ����������ں˺�Ӧ�ó���
//      �������
//      ��������mb_delete_pool�������ǣ����������ͷ��ڴ�ؿ��ƿ�
//����: pool���ڴ��ָ��
//����: ��
//-----------------------------------------------------------------------------
bool_t mb_delete_pool_knl(struct mem_cell_pool *pool)
{
    if(!semp_delete_knl(&pool->memb_semp))
        return false;
    if(!rsc_del_node(&pool->memb_node))
        return false;
    return true;
}

//----����һ���ڴ�-------------------------------------------------------------
//����: ���ƶ��ڴ���з���һ���ڴ�,ֻ�ܷ���һ��,���Ҳ��ܱ�֤�������η�����ڴ�
//      ��ַ��������.
//����: pool���ڴ��ָ��
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: ����ɹ������ڴ��ַ,���򷵻�NULL.
//-----------------------------------------------------------------------------
void *mb_malloc(struct mem_cell_pool *pool,u32 timeout)
{
    void *result,*inc;
    u32 limit,inc_size,inc_cell,capacity,frees;
    char *name;
    atom_low_t atom_low;
    if(pool == NULL)
        return NULL;
    //û��ȡ���ź����������ڴ�ؿ�,����ź����Ǳ����ڴ�صģ�ȷ����������ڴ��
    //�������ڴ�ص�����
    if(semp_pend(&pool->memb_semp,0) == false)  //���ź����������ڴ���Ѿ�����
    {
        //ע��:��һ�к���һ��֮����ܷ����߳��л�
        int_save_asyn_signal();
        //�ڴ������δ�ﵽ���ޣ����Լ�������
        if(semp_query_capacital(&pool->memb_semp) < pool->cell_limit)
        {
            //��ʱtimeoutֻ��Ϊ0����Ϊ�ڽ�ֹ�첽�źŵ�����£���Ϊ0Ҳû��
            inc = m_malloc_gbl(pool->cell_increment*pool->cell_size,0);
            if(inc != NULL)
            {
                inc_size = m_check_size(inc);   //���ʵ�ʷ��䵽���ڴ���
                inc_cell = inc_size/pool->cell_size;
                capacity = semp_query_capacital(&pool->memb_semp);
                frees = semp_query_free(&pool->memb_semp);
                limit = capacity + inc_cell;
                name = ((struct rsc_node*)&pool->memb_semp)->name;
                semp_delete_knl(&pool->memb_semp);      //ɾ��ԭ�ź���
                //�����ݺ���������������ź�����
                semp_create_knl(&pool->memb_semp,limit,frees+inc_cell,name);
                pool->continue_pool = inc;
                pool->pool_offset = (ptu32_t)inc + inc_cell*pool->cell_size;
            }
        }
        int_restore_asyn_signal();
        if(semp_pend(&pool->memb_semp,timeout) == false)
            return NULL;
    }

    //ע:��semp_pend��int_low_atom_start֮�䷢����ռ������ģ���Ϊ�ź����Ѿ�
    //ȡ�ã������¼������ܰ��ڴ�����⡣
    atom_low = int_low_atom_start( );
    if(pool->free_list != NULL)     //���ж��������ڴ��
    {
        result = pool->free_list;   //ȡ�ն��б�ͷ�����ڴ��
        if(((struct mem_cell_free*)result)->next==(struct mem_cell_free*)result)
            pool->free_list = NULL; //���ǿ��ж����е����һ���ˣ������ܲ����ڴ�
                                    //���е����һ�飬��Ϊ�������п��ܻ���
        else
        {
            pool->free_list = pool->free_list->next;  //���ж�������һ��.
            pool->free_list->previous=((struct mem_cell_free*)result)->previous;
            ((struct mem_cell_free*)result)->previous->next = pool->free_list;
        }
    }else                   //���ж��������ڴ�飬����������ȡ
    {
        pool->pool_offset -= pool->cell_size;  //ƫ�Ƶ�ַ����
        //����ƫ�Ƶ�ַ�����ڴ��
        result = (void*)pool->pool_offset;
    }
    int_low_atom_end(atom_low);
    return result;
}
//----�ͷ��ڴ�--------------------------------------------------------------
//����: �ͷ��ڴ�,��ʹ����ϵ��ڴ��Ż�ָ�����ڴ��,�ڴ�غ��ڴ�����ƥ��,����
//      �ᷢ�������ԵĴ���.���ͷŵĿ����ӵ� free_list ������,�����ǷŻ�������
//      ��,Ҳ�����·���ϵͳ��.�ڴ�ش�ϵͳ�ѷ����ڴ��ǵ����,һ�����������ͷ�.
//����: block,���ͷŵ��ڴ��ָ��
//      pool,Ŀ���ڴ��.
//����: ��
//-----------------------------------------------------------------------------
void mb_free(struct mem_cell_pool *pool,void *block)
{
    atom_low_t atom_low;
    if(pool == NULL)
        return;
//    pl_continue = (void*)pool->pool_offset;
//    block_sum = semp_query_capacital(&pool->memb_semp);
    //20110613��ע:�������ݹ��ܺ����м�鲻����Ч��ע�͵���
    //���ͷſ�Ϸ��Լ��:1����ַ���ܵ����ڴ���׵�ַ
    //                   2����ַ���ܸ����ڴ��ĩ��ַ
    //                   3����ַ�����ǿ��׵�ַ����ƫ�Ʊ����ǿ�ߴ��������
//    if((block < (void*)pool->pool_offset)
//            ||((ptu32_t)block>=((ptu32_t)pl_continue+pool->cell_size*block_sum))
//            ||(((ptu32_t)block-(ptu32_t)pl_continue) % pool->cell_size != 0))
//        return ;
    atom_low = int_low_atom_start( );
    //�鿴���ͷŵ��ڴ���Ƿ��Ѿ���free_list�����С�
    if(pool->free_list == NULL)
    {
        pool->free_list = (struct mem_cell_free*)block;
        pool->free_list->next = (struct mem_cell_free*)block;
        pool->free_list->previous = (struct mem_cell_free*)block;
        
     }
    else
    {
        //if(((struct mem_cell_free*)block)->previous->next 
         //                                   != (struct mem_cell_free*)block)
       // {   //����4�аѱ��ͷŵ��ڴ����뵽����β��
            ((struct mem_cell_free*)block)->next = pool->free_list;
            ((struct mem_cell_free*)block)->previous = pool->free_list->previous;
            pool->free_list->previous->next = (struct mem_cell_free*)block;
            pool->free_list->previous = (struct mem_cell_free*)block;
       // }
    }
    int_low_atom_end(atom_low);
    semp_post(&pool->memb_semp);
}


//----��ѯʣ������-------------------------------------------------------------
//����: ��ѯ�ڴ�ػ��ж��ٿ����ڴ��
//����: pool,Ŀ���ڴ��.
//����: �ڴ����
//-----------------------------------------------------------------------------
u32 mb_query_free(struct mem_cell_pool *pool)
{
    if(pool == NULL)
        return 0;
    return semp_query_free(&pool->memb_semp);
}

//----��ѯ�ڴ������-----------------------------------------------------------
//����: ��ѯ�ڴ���ܹ��ж����ڴ��
//����: pool,Ŀ���ڴ��.
//����: �ڴ����
//-----------------------------------------------------------------------------
u32 mb_query_capacital(struct mem_cell_pool *pool)
{
    if(pool == NULL)
        return 0;
    return semp_query_capacital(&pool->memb_semp);
}

