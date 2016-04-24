//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
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
#include "config-prj.h"
#include "align.h"
#include "stdint.h"
#include "rsc.h"
#include "int.h"
#include "lock.h"
#include "stdlib.h"
#include "pool.h"

static struct tagMemCellPool *s_ptPooCtrl;  //�����ڴ�ؿ��ƿ鱾����ڴ��

//----��ʼ���̶������ģ��------------------------------------------------------
//����: ��ʼ������ϵͳ�Ĺ̶����ڴ����ģ��.�ڴ�ؿ��ƿ鱾��Ҳ�ǰ����ڴ�صķ�ʽ
//      �����,���Ǵ�ʱ�ڴ�������û�г�ʼ�����,����Ҫ�ֶ����������ڴ�ؿ��ƿ�
//      ���ڴ��.
//����: ��.
//����: 1.
//----------------------------------------------------------------------------
ptu32_t Mb_ModuleInit(ptu32_t para)
{
    static struct tagMemCellPool cell_pool;
    cell_pool.continue_pool = M_Malloc(gc_u32CfgMemPoolLimit * sizeof(struct tagMemCellPool),0);
    if(cell_pool.continue_pool == NULL)
        return 0;
    cell_pool.cell_size = sizeof(struct tagMemCellPool);
    cell_pool.free_list = NULL;
    cell_pool.pool_offset = (ptu32_t)cell_pool.continue_pool;
    cell_pool.pool_offset += gc_u32CfgMemPoolLimit * sizeof(struct tagMemCellPool);
    s_ptPooCtrl = &cell_pool;
    Rsc_AddTree(&cell_pool.memb_node,sizeof(struct tagMemCellPool),RSC_MEMPOOL,
                    "�̶�������");
    Lock_SempCreate_s(&cell_pool.memb_semp,gc_u32CfgMemPoolLimit,
                      gc_u32CfgMemPoolLimit,CN_SEMP_BLOCK_FIFO,"�̶�������");
    return 1;
}

//----����һ���ڴ��-------------------------------------------------------
//����: ��ʼ��һ���ڴ�أ�ԭʼ�ڴ�ص��ڴ����û��ṩ�����ϵͳ�ж���Ҫ������ʼ
//      ��ַ���ٰ���ָ�����Ͷ��룬cell_sizeҲӦ����ָ�볤�ȵ����������������
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
//            ����.
//      increment,���������ڴ���е��ڴ浥Ԫ�ľ�ʱ�����Ӷ������ӵ��ڴ������
//          ��Ϊ0�����ӡ�ע�⣬�ڴ������󣬼�ʹ�û�����mb_free�ͷ����ڴ棬��
//          �����ͷ����ڴ���е�ȫ���ڴ棬�����������ڴ治�ᱻ�ջء�
//      limit,���increment !=0��limit�����ڴ�ص����������Է������Ƶ����ӣ���
//          ���ڴ�ľ���
//      name,���ڴ��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �ڴ��ָ��.
//----------------------------------------------------------------------------
struct tagMemCellPool *Mb_CreatePool(void *pool_original,u32 init_capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name)
{
    struct tagMemCellPool *pool;

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
#if(CN_MEM_ALIGN == 1)
    if((cell_size % sizeof(void*) != 0)
            || (((ptu32_t)pool_original) % sizeof(void*) != 0))
        return NULL;
#endif

    //����һ���ڴ�ؿ���ͷ
    pool = (struct tagMemCellPool *)Mb_Malloc(s_ptPooCtrl,0);
    if(pool == NULL)
        return NULL;    //�ڴ�ؿ���ͷ���䲻�ɹ�

    pool->continue_pool = (void*)pool_original;     //�������׵�ַ
    pool->free_list = NULL;                 //��ʼʱ���������ǿյ�
    //����������ƫ�Ƶ�ַ
    pool->pool_offset = (ptu32_t)pool_original + init_capacital*cell_size;
    pool->cell_size = cell_size;
    pool->cell_increment = increment;
    pool->cell_limit = limit;
    pool->next_inc_pool = NULL;
    Rsc_AddSon(&s_ptPooCtrl->memb_node,
                &pool->memb_node,
                sizeof(struct tagMemCellPool),
                RSC_MEMPOOL,name);
    //init_capacital�п�����0
    Lock_SempCreate_s(&pool->memb_semp,init_capacital,init_capacital,
                        CN_SEMP_BLOCK_FIFO,name);
    return pool;
}

//----����һ���ں��ڴ��-------------------------------------------------------
//����: �����ڴ�أ����������ṩ�ڴ�ؿ��ƿ飬�߿ɿ��Ե�Ӧ���У���Ӧ��ʹ�ö�̬��
//      ��ķ�ʽ����̬������ɿ���Ȼ���ָ�봫�ݹ������ں���ʹ�õ��ڴ�أ�����ʹ
//      �þ�̬����ġ�Ӧ�ó�������cfg_mem_poolsʱ��ֻ��Ҫ�����Լ�������Ϳ����ˡ�
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
struct tagMemCellPool *Mb_CreatePool_s(struct tagMemCellPool *pool,
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
    pool->next_inc_pool = NULL;
    Rsc_AddSon(&s_ptPooCtrl->memb_node,
                &pool->memb_node,
                sizeof(struct tagMemCellPool),
                RSC_MEMPOOL,name);
    Lock_SempCreate_s(&pool->memb_semp,init_capacital,init_capacital,
                            CN_SEMP_BLOCK_FIFO,name);
    return pool;
}

//----ɾ��һ���ڴ��-----------------------------------------------------------
//����: ɾ��һ���ڴ�أ���ĳ�ڴ�ز�����Ҫʱ���ɵ��ñ�����.������ֻ�������ڴ��
//      ���ź�������Դ��㣬�ڴ�ػ������ǵ������ṩ�ģ�Ӧ�ɵ���������
//����: pool���ڴ��ָ��
//����: true= �ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t Mb_DeletePool(struct tagMemCellPool *pool)
{
    void *inc_memory,*temp;
    if(!Lock_SempDelete_s(&pool->memb_semp))
        return false;
    inc_memory = pool->next_inc_pool;
    //����ڴ����ʹ�ù��������˶�̬���ݲ����������ͷ����ӵ��ڴ�
    while(inc_memory != NULL)
    {
        temp = *(void**)inc_memory;
        free(inc_memory);
        inc_memory = temp;
    }
    if(!Rsc_DelNode(&pool->memb_node))
        return false;
    Mb_Free(s_ptPooCtrl,pool);
    return true;
}

//----ɾ��һ���ں��ڴ��-------------------------------------------------------
//����: ɾ��һ���ڴ�أ���mb_create_pool_s��Ӧ
//����: pool���ڴ��ָ��
//����: ��
//-----------------------------------------------------------------------------
bool_t Mb_DeletePool_r(struct tagMemCellPool *pool)
{
    void *inc_memory,*temp;
    if(!Lock_SempDelete_s(&pool->memb_semp))
        return false;
    inc_memory = pool->next_inc_pool;
    //����ڴ����ʹ�ù��������˶�̬���ݲ����������ͷ����ӵ��ڴ�
    while(inc_memory != NULL)
    {
        temp = *(void**)inc_memory;
        free(inc_memory);
        inc_memory = temp;
    }
    if(!Rsc_DelNode(&pool->memb_node))
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
void *Mb_Malloc(struct tagMemCellPool *pool,u32 timeout)
{
    void *result,*inc;
    u32 limit,inc_size,inc_cell,capacity,frees;
    char *name;
    atom_low_t atom_low;
    if(pool == NULL)
        return NULL;
    //û��ȡ���ź����������ڴ�ؿ�,����ź����Ǳ����ڴ�صģ�ȷ����������ڴ��
    //�������ڴ�ص�����
    if(Lock_SempPend(&pool->memb_semp,0) == false)  //���ź����������ڴ���Ѿ�����
    {
        //ע��:��һ�к���һ��֮����ܷ����߳��л�
        Int_SaveAsynSignal();
        //�ڴ������δ�ﵽ���ޣ����Լ�������
        if(Lock_SempQueryCapacital(&pool->memb_semp) < pool->cell_limit)
        {
            //��ʱtimeoutֻ��Ϊ0����Ϊ�ڽ�ֹ�첽�źŵ�����£���Ϊ0Ҳû��
            //+ align_up_sys(1)��Ϊ�˶���
            inc = M_Malloc(pool->cell_increment*pool->cell_size
                                + align_up_sys(1),timeout);
            if(inc != NULL)
            {
                //���ʵ�ʷ��䵽���ڴ���
                inc_size = M_CheckSize(inc) - align_up_sys(1);
                inc_cell = inc_size/pool->cell_size;
                capacity = Lock_SempQueryCapacital(&pool->memb_semp);
                frees = Lock_SempQueryFree(&pool->memb_semp);
                limit = capacity + inc_cell;
                name = ((struct tagRscNode*)&pool->memb_semp)->name;
                Lock_SempDelete_s(&pool->memb_semp);      //ɾ��ԭ�ź���
                //�����ݺ���������������ź�����
                Lock_SempCreate_s(&pool->memb_semp,limit,frees+inc_cell,
                                    CN_SEMP_BLOCK_FIFO,name);
                pool->continue_pool = (void*)((ptu32_t)inc + align_up_sys(1));
                pool->pool_offset = (ptu32_t)inc + inc_cell*pool->cell_size;
                //���³�ʼ���������ñ����ڱ�Ƕ�̬���ӵ��ڴ�飬����ɾ���ڴ��
                //ʱ�ͷ��ڴ棬�����ڴ涪ʧ��
                *(void **)inc = pool->next_inc_pool;
                pool->next_inc_pool = inc;
                Int_RestoreAsynSignal();
            }
            else
            {
                Int_RestoreAsynSignal();
                return NULL;
            }
        }
        else
        {
            Int_RestoreAsynSignal();
            return NULL;
        }
    }

    //ע:��semp_pend��int_low_atom_start֮�䷢����ռ������ģ���Ϊ�ź����Ѿ�
    //ȡ�ã������¼������ܰ��ڴ�����⡣
    atom_low = Int_LowAtomStart( );
    if(pool->free_list != NULL)     //���ж��������ڴ��
    {
        result = pool->free_list;   //ȡ�ն��б�ͷ�����ڴ��
        if(((struct tagMemCellFree*)result)->next==(struct tagMemCellFree*)result)
            pool->free_list = NULL; //���ǿ��ж����е����һ���ˣ������ܲ����ڴ�
                                    //���е����һ�飬��Ϊ�������п��ܻ���
        else
        {
            pool->free_list = pool->free_list->next;  //���ж�������һ��.
            pool->free_list->previous=((struct tagMemCellFree*)result)->previous;
            ((struct tagMemCellFree*)result)->previous->next = pool->free_list;
        }
    }else                   //���ж��������ڴ�飬����������ȡ
    {
        pool->pool_offset -= pool->cell_size;  //ƫ�Ƶ�ַ����
        //����ƫ�Ƶ�ַ�����ڴ��
        result = (void*)pool->pool_offset;
    }
    Int_LowAtomEnd(atom_low);
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
void Mb_Free(struct tagMemCellPool *pool,void *block)
{
    atom_low_t atom_low;
    if(pool == NULL)
        return;
//    pl_continue = (void*)pool->pool_offset;
//    block_sum = Lock_SempQueryCapacital(&pool->memb_semp);
    //20110613��ע:�������ݹ��ܺ����м�鲻����Ч��ע�͵���
    //���ͷſ�Ϸ��Լ��:1����ַ���ܵ����ڴ���׵�ַ
    //                   2����ַ���ܸ����ڴ��ĩ��ַ
    //                   3����ַ�����ǿ��׵�ַ����ƫ�Ʊ����ǿ�ߴ��������
//    if((block < (void*)pool->pool_offset)
//            ||((ptu32_t)block>=((ptu32_t)pl_continue+pool->cell_size*block_sum))
//            ||(((ptu32_t)block-(ptu32_t)pl_continue) % pool->cell_size != 0))
//        return ;
    atom_low = Int_LowAtomStart( );
    //�鿴���ͷŵ��ڴ���Ƿ��Ѿ���free_list�����С�
    if(pool->free_list == NULL)
    {
        pool->free_list = (struct tagMemCellFree*)block;
        pool->free_list->next = (struct tagMemCellFree*)block;
        pool->free_list->previous = (struct tagMemCellFree*)block;

     }
    else
    {
        //if(((struct tagMemCellFree*)block)->previous->next
         //                                   != (struct tagMemCellFree*)block)
       // {   //����4�аѱ��ͷŵ��ڴ����뵽����β��
            ((struct tagMemCellFree*)block)->next = pool->free_list;
            ((struct tagMemCellFree*)block)->previous = pool->free_list->previous;
            pool->free_list->previous->next = (struct tagMemCellFree*)block;
            pool->free_list->previous = (struct tagMemCellFree*)block;
       // }
    }
    Int_LowAtomEnd(atom_low);
    Lock_SempPost(&pool->memb_semp);
}


//----��ѯʣ������-------------------------------------------------------------
//����: ��ѯ�ڴ�ػ��ж��ٿ����ڴ��
//����: pool,Ŀ���ڴ��.
//����: �ڴ����
//-----------------------------------------------------------------------------
u32 Mb_QueryFree(struct tagMemCellPool *pool)
{
    if(pool == NULL)
        return 0;
    return Lock_SempQueryFree(&pool->memb_semp);
}

//----��ѯ�ڴ������-----------------------------------------------------------
//����: ��ѯ�ڴ���ܹ��ж����ڴ��
//����: pool,Ŀ���ڴ��.
//����: �ڴ����
//-----------------------------------------------------------------------------
u32 Mb_QueryCapacital(struct tagMemCellPool *pool)
{
    if(pool == NULL)
        return 0;
    return Lock_SempQueryCapacital(&pool->memb_semp);
}

