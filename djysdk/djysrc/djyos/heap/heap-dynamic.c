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
//����ģ��:�ѹ���ģ��
//����:  ������.
//�汾��V1.0.1
//�ļ�����:�ṩ�������ڴ�������
//����˵��:
//�޶���ʷ:
//4. ����:2014-9-1
//   ����:  ������.
//   �°汾�ţ�2.0.0
//   �޸�˵��: ֧�ֶ���ѣ�ÿ�����ɶ��Ƭ�����
//      1������ɾ���������¼ÿ��ʱ��������ڴ�Ĵ��롣
//      2��������Ҫ��config_prj.h������heap��ص���Ŀ������lds�ṩ��
//      3��������ʾheap������Ϣ�ĺ���
//      4��ʵ�����׺�����һ�״�Ĭ�϶��з��䣬һ�״�ָ�����з��䡣
//      5������ʱ�Ȳ�������ʵ�Ƭ��
//4. ����:2013-1-18
//   ����:  ������.
//   �°汾�ţ�1.0.3
//   �޸�˵��: ������m_free������һ�����󣬸�bug������"��ë��"�ύ
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
#include "config-prj.h"
#include "stdint.h"
#include "core-cfg.h"
#include "stdlib.h"
#include "string.h"
#include "align.h"
#include "int.h"
#include "heap-in.h"
#include "heap.h"
#include "lock.h"
#include "djyos.h"
#include "stdio.h"

//���ó�������
//��nλΪ1�������λΪ0�ĳ���
#define CN_HIGH_1BIT_1      (0x80000000)
#define CN_HIGH_2BIT_1      (0xc0000000)
#define CN_HIGH_3BIT_1      (0xe0000000)
#define CN_HIGH_4BIT_1      (0xf0000000)
#define CN_HIGH_5BIT_1      (0xf8000000)
#define CN_HIGH_6BIT_1      (0xfc000000)
#define CN_HIGH_7BIT_1      (0xfe000000)
#define CN_HIGH_8BIT_1      (0xff000000)
#define CN_HIGH_9BIT_1      (0xff800000)
#define CN_HIGH_10BIT_1     (0xffc00000)
#define CN_HIGH_11BIT_1     (0xffe00000)
#define CN_HIGH_12BIT_1     (0xfff00000)
#define CN_HIGH_13BIT_1     (0xfff80000)
#define CN_HIGH_14BIT_1     (0xfffc0000)
#define CN_HIGH_15BIT_1     (0xfffe0000)
#define CN_HIGH_16BIT_1     (0xffff0000)
#define CN_HIGH_17BIT_1     (0xffff8000)
#define CN_HIGH_18BIT_1     (0xffffc000)
#define CN_HIGH_19BIT_1     (0xffffe000)
#define CN_HIGH_20BIT_1     (0xfffff000)
#define CN_HIGH_21BIT_1     (0xfffff800)
#define CN_HIGH_22BIT_1     (0xfffffc00)
#define CN_HIGH_23BIT_1     (0xfffffe00)
#define CN_HIGH_24BIT_1     (0xffffff00)
#define CN_HIGH_25BIT_1     (0xffffff80)
#define CN_HIGH_26BIT_1     (0xffffffc0)
#define CN_HIGH_27BIT_1     (0xffffffe0)
#define CN_HIGH_28BIT_1     (0xfffffff0)
#define CN_HIGH_29BIT_1     (0xfffffff8)
#define CN_HIGH_30BIT_1     (0xfffffffc)
#define CN_HIGH_31BIT_1     (0xfffffffe)
#define CN_ALLBIT_1         (0xffffffff)

//��nλΪ1������λΪ0�ĳ���
#define CN_LOW_1BIT_1      (0x00000001)
#define CN_LOW_2BIT_1      (0x00000003)
#define CN_LOW_3BIT_1      (0x00000007)
#define CN_LOW_4BIT_1      (0x0000000f)
#define CN_LOW_5BIT_1      (0x0000001f)
#define CN_LOW_6BIT_1      (0x0000003f)
#define CN_LOW_7BIT_1      (0x0000007f)
#define CN_LOW_8BIT_1      (0x000000ff)
#define CN_LOW_9BIT_1      (0x000001ff)
#define CN_LOW_10BIT_1     (0x000003ff)
#define CN_LOW_11BIT_1     (0x000007ff)
#define CN_LOW_12BIT_1     (0x00000fff)
#define CN_LOW_13BIT_1     (0x00001fff)
#define CN_LOW_14BIT_1     (0x00003fff)
#define CN_LOW_15BIT_1     (0x00007fff)
#define CN_LOW_16BIT_1     (0x0000ffff)
#define CN_LOW_17BIT_1     (0x0001ffff)
#define CN_LOW_18BIT_1     (0x0003ffff)
#define CN_LOW_19BIT_1     (0x0007ffff)
#define CN_LOW_20BIT_1     (0x000fffff)
#define CN_LOW_21BIT_1     (0x001fffff)
#define CN_LOW_22BIT_1     (0x003fffff)
#define CN_LOW_23BIT_1     (0x007fffff)
#define CN_LOW_24BIT_1     (0x00ffffff)
#define CN_LOW_25BIT_1     (0x01ffffff)
#define CN_LOW_26BIT_1     (0x03ffffff)
#define CN_LOW_27BIT_1     (0x07ffffff)
#define CN_LOW_28BIT_1     (0x0fffffff)
#define CN_LOW_29BIT_1     (0x1fffffff)
#define CN_LOW_30BIT_1     (0x3fffffff)
#define CN_LOW_31BIT_1     (0x7fffffff)

#define CN_MEM_DOUBLE_PAGE_LOCAL    0xffff
#define CN_MEM_MANY_PAGE_LOCAL      0xfffe
#define CN_MEM_SINGLE_PAGE_GLOBAL   0xfffd
#define CN_MEM_MANY_PAGE_GLOBAL     0xfffc
#define CN_MEM_FREE_PAGE            0xfffb

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

const ucpu_t cn_low_xbit_msk_exp2 []=
{
    CN_LOW_1BIT_1,
    CN_LOW_2BIT_1,
    CN_LOW_4BIT_1,
    CN_LOW_8BIT_1,
    CN_LOW_16BIT_1,
    CN_ALLBIT_1,
};

const ucpu_t cn_low_xbit_msk []=
{
    0,
    CN_LOW_1BIT_1,
    CN_LOW_2BIT_1,
    CN_LOW_3BIT_1,
    CN_LOW_4BIT_1,
    CN_LOW_5BIT_1,
    CN_LOW_6BIT_1,
    CN_LOW_7BIT_1,
    CN_LOW_8BIT_1,
    CN_LOW_9BIT_1,
    CN_LOW_10BIT_1,
    CN_LOW_11BIT_1,
    CN_LOW_12BIT_1,
    CN_LOW_13BIT_1,
    CN_LOW_14BIT_1,
    CN_LOW_15BIT_1,
    CN_LOW_16BIT_1,
    CN_LOW_17BIT_1,
    CN_LOW_18BIT_1,
    CN_LOW_19BIT_1,
    CN_LOW_20BIT_1,
    CN_LOW_21BIT_1,
    CN_LOW_22BIT_1,
    CN_LOW_23BIT_1,
    CN_LOW_24BIT_1,
    CN_LOW_25BIT_1,
    CN_LOW_26BIT_1,
    CN_LOW_27BIT_1,
    CN_LOW_28BIT_1,
    CN_LOW_29BIT_1,
    CN_LOW_30BIT_1,
    CN_LOW_31BIT_1,
    CN_ALLBIT_1,
};

struct  tagEventECB;
#define CN_MEM_DOUBLE_PAGE_LOCAL    0xffff
#define CN_MEM_MANY_PAGE_LOCAL      0xfffe
#define CN_MEM_SINGLE_PAGE_GLOBAL   0xfffd
#define CN_MEM_MANY_PAGE_GLOBAL     0xfffc
#define CN_MEM_FREE_PAGE            0xfffb

extern void *pHeapList;             //�ڽű��ж���
extern struct tagHeapCB *tg_pHeapList;   //����ָ�룬ϵͳ�����еĶѱ�������һ��
extern struct tagHeapCB *tg_pSysHeap;   //����ָ�룬ϵͳ�����еĶѱ�������һ��

void *__M_Malloc(ptu32_t size,u32 timeout);
void  __M_Free(void * pl_mem);
void *__M_Realloc(void *, ptu32_t NewSize,u32 timeout);
void *__M_MallocHeap(ptu32_t size,struct tagHeapCB *Heap,u32 timeout);
void *__M_MallocLc(ptu32_t size,u32 timeout);
void *__M_MallocLcHeap(ptu32_t size,struct tagHeapCB *Heap, u32 timeout);
void  __M_FreeHeap(void * pl_mem,struct tagHeapCB *Heap);
void *__M_MallocStack(ptu32_t size);

ptu32_t Heap_DynamicModuleInit(ptu32_t para);
void __M_ShowHeap(void);
void __M_ShowHeapSpy(void);

ptu32_t __M_FormatSizeHeap(ptu32_t size,struct tagHeapCB *Heap);
ptu32_t __M_FormatSize(ptu32_t size);
ptu32_t __M_GetMaxFreeBlockHeap(struct tagHeapCB *Heap);
ptu32_t __M_GetMaxFreeBlock(void);
ptu32_t M_GetHeapSizeHeap(struct tagHeapCB *Heap);  //��heap-static.c��
ptu32_t M_GetHeapSize(void);  //��heap-static.c��
struct tagHeapCB *M_FindHeap(const char *HeapName);
ptu32_t __M_GetFreeMem(void);
ptu32_t __M_GetFreeMemHeap(struct tagHeapCB *Heap);
ptu32_t __M_CheckSize(void * mp);

extern void *  (*M_Malloc)(ptu32_t size,u32 timeout);
extern void *  (*M_Realloc) (void *, ptu32_t NewSize,u32 timeout);
extern void  (*free)(void * pl_mem);
extern void *  (*M_MallocHeap)(ptu32_t size,struct tagHeapCB *Heap,u32 timeout);
extern void *  (*M_MallocLc)(ptu32_t size,u32 timeout);
extern void *  (*M_MallocLcHeap)(ptu32_t size,struct tagHeapCB *Heap, u32 timeout);
extern void    (*M_FreeHeap)(void * pl_mem,struct tagHeapCB *Heap);
extern void *  (*__MallocStack)(ptu32_t size);
extern ptu32_t (*M_FormatSizeHeap)(ptu32_t size,struct tagHeapCB *Heap);
extern ptu32_t (*M_FormatSize)(ptu32_t size);
extern ptu32_t (*M_GetMaxFreeBlockHeap)(struct tagHeapCB *Heap);
extern ptu32_t (*M_GetMaxFreeBlock)(void);
extern ptu32_t (*M_GetFreeMem)(void);
extern ptu32_t (*M_GetFreeMemHeap)(struct tagHeapCB *Heap);
extern ptu32_t (*M_CheckSize)(void * mp);


//�ڲ���������
void *__M_MallocBlock(ufast_t grade,struct tagHeapCession *Cession);

extern void __Djy_EventReady(struct  tagEventECB *event_ready);
extern void __Djy_CutReadyEvent(struct tagEventECB *event);
extern void __Djy_ResumeDelay(struct  tagEventECB *delay_event);
extern void ___Djy_AddToDelay(u32 u32l_uS);
extern void __Djy_CutEventFromEvttMarked(struct tagEventECB *event);

//----���Ҷ�-------------------------------------------------------------------
//����: ͨ�����֣����Ҷѿ��ƿ�ָ��
//����: HeapName���ѵ�����
//����: ��ָ�룬�Ҳ���������ԭ�򷵻�NULL
//-----------------------------------------------------------------------------
struct tagHeapCB *M_FindHeap(const char *HeapName)
{
    struct tagHeapCB *Heap = NULL,*Work;
    if(tg_pHeapList == NULL)
        return NULL;
    Work = tg_pHeapList;
    do
    {
        if( ! strcmp(HeapName,Work->HeapName))
        {
            Heap = Work;
            break;
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);
    return Heap;
}

void __M_ShowHeap(void)
{
    struct tagHeapCB *Heap;
    struct tagHeapCession *Cession;
    struct tagEventECB *ECB;
    u32 n,m,k;

    if(tg_pHeapList == NULL)
        return;
    Heap = tg_pHeapList;
    printf("�г�ϵͳ�����е�Heap\r\n\r\n");
    do
    {
        printf("Heap name: %s���� %d �� Cessions\r\n",
                    Heap->HeapName,Heap->CessionNum);
#if ((CN_CFG_DYNAMIC_MEM == 1))
        ECB = Heap->mem_sync;
        n = 0;
        if(ECB != NULL)
        {
            do
            {
                n++;
                ECB = ECB->multi_next;
            }while(ECB != Heap->mem_sync);
        }
#endif
        printf("�� %d ���¼��������ȴ��� Heap �����ڴ�\r\n\r\n",n);
        Cession = Heap->Cession;
        n = 0;
        m = 0;
        k = 0;
        while(Cession != NULL)
        {
            printf("Cession %d:\r\n",n+1);

            printf("׼��̬����ռ�:  0x%08x\r\n",
                    Cession->heap_bottom - Cession->static_bottom);
#if ((CN_CFG_DYNAMIC_MEM == 1))
            printf("��̬����ռ�:    0x%08x\r\n",
                        Cession->ua_pages_num * Cession->PageSize);
#endif
            printf("��̬��ʼ��ַ:    0x%08x,    ��̬��ʼ��ַ:  0x%08x\r\n",
                        Cession->static_bottom,
                        Cession->heap_bottom);
#if ((CN_CFG_DYNAMIC_MEM == 1))
            printf("��ҳ��:          %10d��   ҳ�ߴ�:        0x%08x\n\r",
                        Cession->ua_pages_num,Cession->PageSize);
            printf("����ҳ��:        %10d��   ���пռ�:      0x%08x\r\n",
                        Cession->free_pages_num,
                        Cession->free_pages_num * Cession->PageSize);
            printf("����ߴ�:      0x%08x��   �����п�ߴ�:0x%08x\r\n\r\n",
                        Cession->ua_block_max,
                        Cession->ua_free_block_max);
#endif
            n++;
#if ((CN_CFG_DYNAMIC_MEM == 1))
            m += Cession->ua_pages_num * Cession->PageSize;
            k += Cession->free_pages_num * Cession->PageSize;
#endif
            Cession = Cession->Next;
        }
        if(n > 1)
        {
            printf("%d��Cession�ܶ�̬����ռ� %d bytes\r\n",n,m);
            printf("%d��Cession�ܿ����ڴ�     %d bytes\r\n",n,k);
            printf("\r\n");
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);
}

void __M_ShowHeapSpy(void)
{
    struct tagHeapCB *Heap;
    struct tagHeapCession *Cession;
    u16 *pl_eid;
    u32 n,m,k;

    if(tg_pHeapList == NULL)
        return;
    Heap = tg_pHeapList;
    printf("�г�����Heap�ķ������\r\n");
    do
    {
        printf("\r\nHeap name: %s���� %d �� Cessions\r\n",
                    Heap->HeapName,Heap->CessionNum);
        Cession = Heap->Cession;
        n = 0;
        k = 0;
        while(Cession != NULL)
        {
            printf("\r\nCession %d:\r\n",n+1);
            printf("         ҳ��            ����    �¼�\r\n",n+1);
            m = 0;
#if ((CN_CFG_DYNAMIC_MEM == 1))
            pl_eid = Cession->index_event_id;
            while(m < Cession->ua_pages_num)
            {
                switch(pl_eid[m])
                {
                    case CN_MEM_DOUBLE_PAGE_LOCAL:
                    {//˫ҳ�ֲ�����,-1+id
                        printf("%10d = %-10d  �ֲ�    %05d\n\r",m,m+1,pl_eid[m+1]);
                        m += 2;
                    }break;
                    case CN_MEM_MANY_PAGE_LOCAL:
                    {//��ҳ�ֲ�����:-2+id+�׺�
                        printf("%10d = %-10d  �ֲ�    %05d\n\r",
                                    m,m+(1<<pl_eid[m+2])-1,pl_eid[m+1]);
                        m += 1<<pl_eid[m+2];
                    }break;
                    case CN_MEM_SINGLE_PAGE_GLOBAL:
                    {//��ҳȫ���ڴ�:-3
                        printf("%10d - %-10d  ȫ��\n\r",m,m);
                        m++;
                    }break;
                    case CN_MEM_MANY_PAGE_GLOBAL:
                    {//˫(��)ҳȫ���ڴ�:-4+�׺�.
                        printf("%10d - %-10d  ȫ��\n\r",m,m+(1<<pl_eid[m+1])-1);
                        m += 1<<pl_eid[m+1];
                    }break;
                    case CN_MEM_FREE_PAGE:
                    {//�����ڴ�
                        for(k = m+1; k < Cession->ua_pages_num; k++)
                        {
                            if(pl_eid[k] != CN_MEM_FREE_PAGE)
                                break;
                        }
                        printf("%10d ~ %-10d  ����\n\r",m,k-1);
                        m = k;
                    }break;
                    default :
                    {
                        printf("%10d = %-10d  �ֲ�    %05d\n\r",m,m,pl_eid[m]);
                        m++;
                    }break;
                }
            }
#endif
            n++;
            Cession = Cession->Next;
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);
}

//----��ѯ�ڴ�ߴ�-------------------------------------------------------------
//����: ���ݸ�����ָ��,��ѯ��ָ�����ڵ��ڴ��ĳߴ�.
//����: mp,��̬������ڴ�ָ��.
//����: �ڴ��ߴ�,����0�м��ֺ���:1.�Ƿ�ָ��,2.mp����׼��̬�����ָ��.
//-----------------------------------------------------------------------------
ptu32_t __M_CheckSize(void * mp)
{
    struct tagHeapCB *Heap;
    struct tagHeapCession *Cession;
    ptu32_t ua_pages_number;
    ptu32_t *temp;
    uint16_t *pl_id;
    ufast_t uf_free_grade_th;

    if(tg_pHeapList == NULL)
        return 0;
    Heap = tg_pHeapList;
    do
    {
        Cession = Heap->Cession;

        while(Cession != NULL)
        {

            if(((u8*)mp >= Cession->static_bottom)
                        && ((u8*)mp < Cession->heap_top))
            {
                if((u8*)mp < Cession->heap_bottom)    //��ָ���ھ�̬������
                {
                    temp = mp;
                    temp--;
                    return *temp;
                }else                   //��ָ���ڿ�������̬������
                {
                #if ((CN_CFG_DYNAMIC_MEM == 1))
                    //�����ͷŵ��ڴ��ҳ��
                    ua_pages_number=(ptu32_t)((u8*)mp - Cession->heap_bottom)
                                                /Cession->PageSize;

                    //�����ͷŵ��ڴ��Ľ׺�,��0���.ͨ���׺�Ҳ����ȷ���ڴ���
                    //��С.ȷ���ڴ��������,�ֲ�������Ҫ֪��ӵ�и��ڴ���¼�id��
                    //ȫ�ַ�����������ڴ������ٱ�,����֪��ӵ�и��ڴ���¼�id
                    pl_id = &Cession->index_event_id[ua_pages_number];
                    switch(pl_id[0])
                    {
                        case CN_MEM_DOUBLE_PAGE_LOCAL :
                        {//˫ҳ�ֲ�����,-1+id
                            uf_free_grade_th = 1;
                        }break;
                        case CN_MEM_MANY_PAGE_LOCAL :
                        {//��ҳ�ֲ�����:-2+id+�׺�
                            uf_free_grade_th = (ufast_t)pl_id[2];
                        }break;
                        case CN_MEM_SINGLE_PAGE_GLOBAL :
                        {//��ҳȫ���ڴ�:-3
                            uf_free_grade_th = 0;
                        }break;
                        case CN_MEM_MANY_PAGE_GLOBAL :
                        {//˫(��)ҳȫ���ڴ�:-4+�׺�.
                            uf_free_grade_th = (ufast_t)pl_id[1];
                        }break;
                        case CN_MEM_FREE_PAGE :
                        {//˫(��)ҳȫ���ڴ�:-4+�׺�.
                            uf_free_grade_th = CN_LIMIT_UFAST;
                        }break;
                        default :
                        {
                            if(pl_id[0] <= CN_EVENT_ID_LIMIT)
                                uf_free_grade_th = 0;       //��ҳ�ֲ�����:id,
                            else
                            {
                                Djy_SaveLastError(EN_MEM_ERROR);
                                return 0;
                            }
                        }break;
                    }
                    if(uf_free_grade_th == CN_LIMIT_UFAST)
                        return 0;
                    else
                        return Cession->PageSize << uf_free_grade_th;
                #endif
                }
            }
            Cession = Cession->Next;
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);

    //mpָ�벻���κ�heap�ķ�Χ֮�ڣ������ߵ����
    return 0;
}

//----������������λ1λ�ã�ǰ��0�ĸ�����----------------------------------------
//���ܣ������������ֽڵ����λ��ʼ����ǰ��0��������
//������data,��������ָ��
//      len,����Ĵ�С
//���أ�ǰ��0�ĸ���
//------------------------------------------------------------------------------
ucpu_t __M_LeadingZero(u8 *data,u8 len)
{
    u8 u8_i;
    ucpu_t  uc_j=0;
#if CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN
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
#else
    u8_i=0;
    do
    {
        if(cn_leading_zero[data[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[data[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i++ != len-1);
#endif
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
u32 __M_CalculateExpense(ptu32_t ua_pages)
{
    ptu32_t ua_bits,u32_return;
    ufast_t uf_grades,uf_grade_th;

    //�����������һ���ж�����ҳ�ߴ�,����:��ַ���ȼ�ȥua_pages��ǰ��0����������
    //����,����,ua_pages=5,���λλ����bit2,��29��ǰ��0��ҳ�ߴ���1,2,4ҳ3��,����=3
    uf_grades=CN_PTR_BITS-__M_LeadingZero((u8 *)&ua_pages,sizeof(ptu32_t));

    //ÿҳ����һ��16λ�ּ�¼ӵ�б�ҳ�ڴ���¼���ID��
    //������߽��ڴ��ʱ��ֻ��¼��ҳ��freeʱ������������ñ�����Ϊ�˷�ֹ��
    //�¼��佻�������ͷ��ڴ�ʱ�����ڳ��������������û�����ʹ�á�
    u32_return = ua_pages * sizeof(u16);

    //��һ����������ufast_t���͵�,��Ҫ��ufast_t���Ͷ���.
    u32_return = align_up(sizeof(ufast_t),u32_return);

    //ÿ�׻�Ҫ����һ��ufast_t���͵�����,��¼���׵ļ���
    u32_return += uf_grades*(sizeof(ufast_t));

    //��һ����������ָ��,��Ҫ��ָ�����
    u32_return = align_up(sizeof(void*),u32_return);

    //ÿ��һ��ָ��,ָ��λͼ������
    u32_return += uf_grades*sizeof(void*);

    for(uf_grade_th = 0; uf_grade_th < uf_grades; uf_grade_th++)
    {
        ua_bits = ua_pages>>uf_grade_th;   //���㱾����λ��
        do
        {
            ua_bits=(ua_bits+CN_CPU_BITS-1)>>CN_CPU_BITS_SUFFIX_ZERO;
            u32_return += ua_bits*sizeof(ucpu_t);   //�ü�λͼ����
            u32_return += sizeof(void*);            //��Ҫһ��ָ��ָ���λͼ����
        }while(ua_bits>1); //ֱ�����м���������.
    }
    return u32_return;
}

//----����ucpu_t������ǰ��0�ĸ���----------------------------------------------
//���ܣ���ucpu_t�����ݵ����λ��ʼ����ǰ��0��������
//������data,������
//���أ�ǰ��0�ĸ���
//-----------------------------------------------------------------------------
ucpu_t __M_LeadingUcpuZero(ucpu_t data)
{
    return(__M_LeadingZero((u8*)&data,sizeof(ucpu_t)));
}

//----���ع�񻯽�-------------------------------------------------------------
//���ܣ����������ݹ��Ϊ��񻯳ߴ硣����ǰ����֪sizeС��Cession������ߴ�
//������size,��������ڴ��ߴ�
//      Heap��Ŀ��ѡ�
//���أ��׺š�
//-----------------------------------------------------------------------------
ufast_t __M_GetGrade(ptu32_t size,struct tagHeapCession *Cession)
{
    ucpu_t  grade;
    u32 pages;

    pages = (size + Cession->PageSize -1)/Cession->PageSize;
    grade = __M_LeadingZero((u8 *)&pages,sizeof(u32));
    if((CN_HIGH_1BIT_1>>grade) != pages)
    //���grade���ǹ�񻯵���,���������������С������С
    //����ά��size�Ĵ�С,��������
        grade--;
    grade = CN_PTR_BITS-1-grade;
    //���ؽ���,��0���,����Сҳ�ߴ�ǰ��0����ߴ�ǰ��0.
    return grade;
}


//----���ؿ��еĹ�񻯽�-------------------------------------------------------
//���ܣ����������ݹ��Ϊ���п����ڴ�Ĺ�񻯡��ý׺ź�cession��ʾ��ͬ���Ľ�
//      �ţ���ͬcession����ߴ��ǲ�һ���ġ�
//      ���ñ�����ǰ���Ѿ�ȷ��heap�����㹻�Ŀ����ڴ档
//������size,��������ڴ��ߴ�
//      Heap��Ŀ��ѡ�
//      result���������ʵĿ����ڴ���Cession��
//���أ��׺š�
//-----------------------------------------------------------------------------
ufast_t __M_GetFreeGrade(ptu32_t size,struct tagHeapCB *Heap,
                         struct tagHeapCession **result)
{
    ucpu_t  grade,result_grade = 0;
    struct tagHeapCession *Cession;
    ptu32_t tsize,fsize = CN_LIMIT_PTU32;
    u32 pages;

    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        if(Cession->ua_free_block_max >= size)
        {
            pages = (size + Cession->PageSize -1)/Cession->PageSize;
            grade = __M_LeadingZero((u8 *)&pages,sizeof(u32));
            if((CN_HIGH_1BIT_1>>grade) != pages)
            //���grade���ǹ�񻯵���,���������������С������С
            //����ά��size�Ĵ�С,��������
                grade--;
            grade = CN_PTR_BITS-1-grade;
            tsize = Cession->PageSize << grade;
            if(tsize < fsize)
            {
                *result = Cession;
                fsize = tsize;
                result_grade = grade;
             }
        }
        Cession = Cession->Next;
    }
    //���ؽ���,��0���,����Сҳ�ߴ�ǰ��0����ߴ�ǰ��0.
    return result_grade;
}
//----�鿴���ȴ������ڴ�----------------------------------------------------
//���ܣ��ȴ�������һ�����������˫��ѭ������,����������ڴ�������,С����ǰ,
//      ���ڴ����ʱ,����ʹ������С���¼��õ�����,��������ʹ�ڴ����㾡�����¼�,
//      ��ʹ�����ȼ�����,��Ϊʵʱ�¼��ǲ�����ʹ�ö�̬�����,���Բ���Ӱ��ʵʱ��.
//������size����Ҫ������ڴ�ߴ�
//      Heap��Ŀ��heap
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���
//      ��0ֵ�������ϵ���Ϊcn_tick_us��������
//����: true = �п����ڴ湩���䣬false = ���ڴ�ɷ���
//��ע: �������ɲ���ϵͳ����,�û����ܵ���.
//------------------------------------------------------------------------------
bool_t __M_CheckMemory(ptu32_t size,struct tagHeapCB *Heap,u32 timeout)
{
    struct  tagEventECB *event;
    u32 u32l_start_time,u32l_rest_time = 0;
    bool_t   wait;

    u32l_start_time = DjyGetTime();
    do
    {
        wait=false;

        if(__M_GetMaxFreeBlockHeap(Heap) < size)
        {//���Ŀɷ����ߴ�С��������ڴ�,û���ڴ���Է���
            Int_SaveAsynSignal();
            wait=true;  //����Ǿ���������������ִ��,whileѭ����Ҫ�ٴ��ж�
                        //�Ƿ����㹻���ڴ����.
            __Djy_CutReadyEvent(g_ptEventRunning);

            g_ptEventRunning->next = NULL;
            g_ptEventRunning->previous = NULL;

            g_ptEventRunning->wait_mem_size = size; //���ȴ����ֽ���
            event = Heap->mem_sync;       //��ȡ�ڴ�ȴ���ָ��

            //���°��¼������ڴ�ȴ�������
            g_ptEventRunning->sync_head = &Heap->mem_sync;
            if(event == NULL)    //�ȴ����п�
            {
                g_ptEventRunning->multi_next = g_ptEventRunning;
                g_ptEventRunning->multi_previous = g_ptEventRunning;
                Heap->mem_sync = g_ptEventRunning;
            }else
            {
                do
                {//��ѭ���ҵ���һ�������ڴ�������¼����¼�.
                    if(event->wait_mem_size < size)
                        event = event->multi_next;
                    else
                        break;
                }while(event != Heap->mem_sync);
                //���û���ҵ������ڴ�����¼������¼�,���¼��������β,��
                //����β������event_wait��ǰ��,��ʱeventǡ�õ���event_wait
                //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
                //��������ǰ��ѭ���ҵ����,����ʹ�����д���
                g_ptEventRunning->multi_next = event;
                g_ptEventRunning->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = g_ptEventRunning;
                event->multi_previous = g_ptEventRunning;
            }

            if(timeout != CN_TIMEOUT_FOREVER)
            {
                g_ptEventRunning->event_status= CN_STS_WAIT_MEMORY
                                                + CN_STS_SYNC_TIMEOUT;
                u32l_rest_time = timeout-((u32)DjyGetTime() - u32l_start_time);
                ___Djy_AddToDelay(timeout-u32l_rest_time);
            }else
            {
                g_ptEventRunning->event_status = CN_STS_WAIT_MEMORY;
            }

            Int_RestoreAsynSignal();  //���жϲ���ʵ�ֵ���
            //�������ﷵ�أ��ǳ�ʱ�������¼��ͷ��ڴ�
            if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
            {//����ӳ�ʱ���أ���Ӧ�÷���false��˵��û���ڴ����
             //����ǳ�ʱ���أ�˵�����߳��ͷ��ڴ棬��������һ��whileѭ��
             //����Ƿ����㹻���ڴ�,
                return false;   //��ʱ����
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
//      ������ֻ�ṩ��djyos.c�д����߳�ʱʹ�ã�ֱ�Ӷ�ϵͳ�Ѳ���
//����: event,������ȴ����е��¼�ָ��
//����: ��
//-----------------------------------------------------------------------------
void __M_WaitMemory(struct  tagEventECB *event)
{
    struct  tagEventECB *pl_event;
    ptu32_t size;

    size = event->wait_mem_size;
    pl_event = tg_pSysHeap->mem_sync;     //��ȡ�ڴ�ȴ���ָ��
    event->sync_head = &tg_pSysHeap->mem_sync;
    if(pl_event == NULL)            //�ȴ����п�
    {
        event->next = NULL;
        event->previous = NULL;

        event->multi_next = event;
        event->multi_previous = event;

        tg_pSysHeap->mem_sync = event;
    }else
    {
        do
        {//��ѭ���ҵ���һ�������ڴ�������¼����¼�.
            if(pl_event->wait_mem_size < size)
                pl_event = pl_event->multi_next;
            else
                break;
        }while(pl_event != tg_pSysHeap->mem_sync);
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
void __M_CleanUp(uint16_t event_id)
{
    struct tagHeapCB *Heap;
    struct tagHeapCession *Cession;
    uint16_t *pl_id;
    u32 n;

    if(M_Malloc != __M_Malloc)
        return;             //��̬�ڴ����δ��ʼ����
    Heap = tg_pHeapList;
    do
    {
        Cession = Heap->Cession;

        while(Cession != NULL)
        {
            pl_id = Cession->index_event_id;
            n = 0;
            while(n < Cession->ua_pages_num)
            {
            //�Ķ���������������struct tagHeapCession��index_event_id��ע��.
                if(pl_id[n] == event_id)
                {
                    __M_Free(Cession->PageSize * n + Cession->heap_bottom);
                    n++;
                }
                else if( (pl_id[n] == CN_MEM_DOUBLE_PAGE_LOCAL)
                          && (pl_id[n+1] == event_id))
                {
                    __M_Free(Cession->PageSize * n + Cession->heap_bottom);
                    n += 2;
                }
                else if( (pl_id[n] == CN_MEM_MANY_PAGE_LOCAL)
                          && (pl_id[n+1] == event_id))
                {
                    __M_Free(Cession->PageSize * n + Cession->heap_bottom);
                    n += 1<<pl_id[n+1];
                }
            }
            Cession = Cession->Next;
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);

    //mpָ�벻���κ�heap�ķ�Χ֮�ڣ������ߵ����
    return ;
}

//----��ʼ���ڴ��-------------------------------------------------------------
//���ܣ�1.�Ӷ��з�����ʿ��ƽṹ������ڴ棬λ���ڶѵĶ�����ռ��������
//      2.��ʼ���ڴ������ƽṹ,��ʼ��ȫ���ڴ�ṹ struct tagMemGlobal
//      3.��ʼ���ڴ�����
//���أ�1=�ɹ���0=ʧ��
//��ע: 1.�������ڳ�ʼ�����֮ǰ����,�첽�ź���δ����,���迼�ǹر��жϵ�����.
//      2.���ñ�����ǰ,ֻ����ִ�о�̬���书��,����ִ�ж�̬���书��.
//      3.Ŀǰֻ�����ڲ���mmu���������mmu�ر���֧�ֶ���̵������û����á�
//-----------------------------------------------------------------------------
ptu32_t Heap_DynamicModuleInit(ptu32_t para)
{
    ptu32_t ua_temp,ua_temp1=0,ua_temp2,ua_temp3 = 0,ua_faction;
    ptu32_t ua_pages,ua_table_size=0;
    ufast_t uf_classes,uf_grades;
    ucpu_t  ***pppl_bitmap,**ppl_bitmap,*pl_bitmap;
    ufast_t *pl_classes;

    struct tagHeapCB *Heap;
    struct tagHeapCession *Cession;

    if(tg_pHeapList == NULL)
        return 0;
    Heap = tg_pHeapList;
    do
    {
        Cession = Heap->Cession;
        Lock_MutexCreate_s(&(Heap->HeapMutex),Heap->HeapName);

        while(Cession != NULL)
        {
            ua_faction=(ptu32_t)(Cession->heap_top-Cession->heap_bottom);
            //����ѵ���ҳ����ȡ��ҳ����ȥ��������
            ua_pages = ua_faction / Cession->PageSize;
            ua_faction = ua_faction % Cession->PageSize;
            //�����ҳ����������������������ڴ���ƽṹʹ�á�
            Cession->heap_top -=ua_faction;

            if(ua_pages < 2) //�������ҳ��С��2ҳ,���ƽṹ���ܻ�Ҫռ��1ҳ,����������?
            {
                Cession->ua_pages_num = 0;
                Cession->free_pages_num = 0;
                Cession->ua_block_max = 0;
                Cession->ua_free_block_max = 0;
                Cession->uf_grades = 0;
                break;
            }

            ua_temp = __M_CalculateExpense(ua_pages); //�������ƽṹ��Ҫ���ֽ���
            if(ua_faction < ua_temp)
            {
                //�������ƽṹ��ua_faction�⻹��Ҫ��ҳ��
                ua_temp = (ua_temp-ua_faction+Cession->PageSize-1)
                            /Cession->PageSize;
                for(ua_temp1 = ua_temp; ua_temp1 > 0; ua_temp1--)
                {//������ҳ���϶����ڻ����ʵ����Ҫ��ҳ��,�������ڴ�ҳ���϶�ʱ.
                 //����ֵ�ӳ�����ҳ����ʼ��һ��С,ֱ������Ϊֹ.
                    //�ع����ƽṹ�ֽ���
                    ua_temp2 = __M_CalculateExpense(ua_pages-ua_temp1);
                    ua_temp3 = (ua_temp2-ua_faction+Cession->PageSize-1)
                            /Cession->PageSize;         //���¼�����ƽṹҳ��
                    if(ua_temp3 >= ua_temp1)
                    //������ҳ���պô��ڻ���ڹ���ֵʱ,ѭ����ֹ
                        break;
                }
                //ʵ�ʿɷ����ҳ��,u32_temp3Ϊ���ƽṹҳ��
                ua_pages = ua_pages -ua_temp3;
                Cession->heap_top -= ua_temp3 * Cession->PageSize;  //�����Ѷ�
            }
            Cession->ua_pages_num = ua_pages;
            Cession->free_pages_num = ua_pages;
            //�ڴ���ƽṹ����
            memset(Cession->heap_top,0,ua_faction+(ua_temp3 *Cession->PageSize));

            //�����ܽ�������һ���ж�����ҳ�ߴ�,����:��ַ���ȼ�ȥua_pages��ǰ��0
            //����������,ua_pages=5,���λλ����bit2,��29��ǰ��0��ҳ�ߴ���1,2,4
            //ҳ3��,����=3
            uf_grades=CN_PTR_BITS-__M_LeadingZero((u8 *)&ua_pages,sizeof(ptu32_t));

            Cession->uf_grades = uf_grades;    //�ܽ���
            Cession->ua_block_max=Cession->PageSize<<(uf_grades-1);//��߽׿�ߴ�
            Cession->ua_free_block_max=Cession->ua_block_max; //�����ÿ�ߴ�

            //����λͼָ��������ָ����
            ua_table_size=0;
            for(ua_temp=0;ua_temp<uf_grades;ua_temp++)
            {//��������
                ua_temp1 = ua_pages>>ua_temp;   //����ua_temp��0��λͼλ��
                do
                {
                    //������һ��λͼ��λ��
                    ua_temp1=(ua_temp1+CN_CPU_BITS-1)>>CN_CPU_BITS_SUFFIX_ZERO;
                    ua_table_size++;        //��Ҫһ��ָ��ָ��ü�λͼ�׵�ַ
                //��һ��λͼ����1λ(��������cn_cpu_bitsλ),����ѭ��
                }while(ua_temp1>1);
            }

            //�¼�ָ��id��,��ϸ˵����mem_global_t���Ͷ��岿��.
            Cession->index_event_id=(uint16_t*)Cession->heap_top;
            for(ua_temp = 0; ua_temp<ua_pages; ua_temp++)
                Cession->index_event_id[ua_temp] = CN_MEM_FREE_PAGE;
            //���׵�λͼ������ָ�룬ÿ����һ��ufast_t�͵�����ʾ�ý׵�λͼ����
            ua_temp = (ptu32_t)(Cession->index_event_id+ua_pages);
            ua_temp = (ua_temp+sizeof(ufast_t)-1)&(~(sizeof(ufast_t)-1)); //����
            pl_classes=(ufast_t *)ua_temp;
            Cession->p_classes=pl_classes;     //���׿��н�������������ָ��

            //����Ǹ��������ָ���ָ����ַ
            ua_temp = (ptu32_t)(pl_classes+uf_grades);
            ua_temp = (ua_temp+sizeof(void *)-1)&(~(sizeof(void *)-1));  //����
            pppl_bitmap=(void*)ua_temp;
            //���׽�����λͼָ����ָ������ָ��
            Cession->ppp_bitmap=pppl_bitmap;

            //������λͼָ����ָ��
            ppl_bitmap=(ucpu_t **)(pppl_bitmap+uf_grades);
            pl_bitmap=(ucpu_t *)(ppl_bitmap+ua_table_size); //������λͼָ���

            //����������ͷ����
            for(ua_temp=0;ua_temp<uf_grades;ua_temp++)  //��������
            {
                ua_temp1 = ua_pages>>ua_temp;       //ua_temp��λͼ��λ��
                uf_classes=0;                       //ua_temp�׵�λͼ����
                pppl_bitmap[ua_temp]=ppl_bitmap;    //ua_temp��λͼ���ָ��
                do
                {
                    ucpu_t  msk=~0;     //����λ��δ֪,�ô˷�����ȫ1
                    ua_temp3=ua_temp1>>CN_CPU_BITS_SUFFIX_ZERO; //������Ĵ�С(��������)
                    ua_temp1=ua_temp1%CN_CPU_BITS;    //��·������Ĵ�С(��������)
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
            Cession = Cession->Next;
        }       //for while(Cession != NULL)
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);

    M_Malloc = __M_Malloc;
    M_Realloc = __M_Realloc;
    free = __M_Free;
    M_MallocHeap = __M_MallocHeap;
    M_MallocLc = __M_MallocLc;
    M_MallocLcHeap = __M_MallocLcHeap;
    M_FreeHeap = __M_FreeHeap;
    __MallocStack = __M_MallocStack;
    M_FormatSizeHeap = __M_FormatSizeHeap;
    M_FormatSize = __M_FormatSize;
    M_GetMaxFreeBlockHeap = __M_GetMaxFreeBlockHeap;
    M_GetMaxFreeBlock = __M_GetMaxFreeBlock;
    M_GetFreeMem = __M_GetFreeMem;
    M_GetFreeMemHeap = __M_GetFreeMemHeap;
    M_CheckSize = __M_CheckSize;

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
void *__M_MallocLcHeap(ptu32_t size,struct tagHeapCB *Heap,u32 timeout)
{
    struct tagHeapCession *Cession;
    void *ua_address;
    ufast_t  uf_grade_th;
    bool_t   en_scheduler;
    void *result;
    uint16_t *pl_id,id;
    u32 page;

    if( (size == 0) || (Heap == NULL) )
        return NULL;
    //�����ڴ�ֱ���ж�size�Ƿ�����,��Ϊȡ�û�����ǰ���ܷ����л����ж���Ч.
    if(Lock_MutexPend(&Heap->HeapMutex,timeout) == false)
        return NULL;
    en_scheduler = Djy_QuerySch();
    //��ֹ���������£������ǰû���㹻�Ŀ����ڴ棬�ԹԵ��߰ɡ�
    if((__M_GetMaxFreeBlockHeap(Heap) < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( !__M_CheckMemory(size,Heap,timeout) )        //û�к��ʵĿ����ڴ��
        {
            result = NULL;
        }else    //�к��ʵĿ����ڴ��
        {
            uf_grade_th=__M_GetFreeGrade(size,Heap,&Cession);    //ȡ�׺�
            ua_address=__M_MallocBlock(uf_grade_th,Cession);     //�����ڴ�
            g_ptEventRunning->local_memory++;

            //�Ķ���������������struct tagHeapCession��index_event_id��ע��.
            pl_id = Cession->index_event_id;
            id = g_ptEventRunning->event_id;
            page = (ptu32_t)((u8*)ua_address-Cession->heap_bottom)/Cession->PageSize;
            if(uf_grade_th==0)
            {//����1ҳ
                pl_id[page] = id;
            }else if(uf_grade_th==1)
            {//����2ҳ
                pl_id[page] = CN_MEM_DOUBLE_PAGE_LOCAL;
                pl_id[page+1] = id;
            }else
            {   //�����ҳ
                pl_id[page] = CN_MEM_MANY_PAGE_LOCAL;
                pl_id[page+1] = id;
                pl_id[page+2] = uf_grade_th;
            }
            result = ua_address;
        }
    }
    Lock_MutexPost(&Heap->HeapMutex);
    return result;
}
void *__M_MallocLc(ptu32_t size,u32 timeout)
{
    return ( __M_MallocLcHeap(size,tg_pSysHeap,timeout) );
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
void *__M_MallocHeap(ptu32_t size,struct tagHeapCB *Heap, u32 timeout)
{
    struct tagHeapCession *Cession;
    void *ua_address;
    ufast_t  uf_grade_th;
    uint16_t    *pl_id;
    bool_t   en_scheduler;
    void *result;

    if( (size == 0) || (Heap == NULL) )
        return NULL;
    //�����ڴ�ֱ���ж�size�Ƿ�����,��Ϊ��ȡ�û�����ǰ���ܷ����л����ж���Ч.
    if(Lock_MutexPend(&Heap->HeapMutex,timeout) == false)
        return NULL;
    en_scheduler = Djy_QuerySch();
    //��ֹ���������£������ǰû���㹻�Ŀ����ڴ棬�ԹԵ��߰ɡ�
    if((__M_GetMaxFreeBlockHeap(Heap) < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( ! __M_CheckMemory(size,Heap,timeout))
        {
            result = NULL;
        }else
        {
            uf_grade_th=__M_GetFreeGrade(size,Heap,&Cession);    //ȡ�׺�
            ua_address=__M_MallocBlock(uf_grade_th,Cession);     //�����ڴ�

            //������id���м�¼���η��������
            //�Ķ����δ�������mem_global_t��index_event_id��Ա�����ע��.
            pl_id = &Cession->index_event_id
                    [(ptu32_t)((u8*)ua_address-Cession->heap_bottom)
                            /Cession->PageSize];
            if(0==uf_grade_th)
                *pl_id = CN_MEM_SINGLE_PAGE_GLOBAL;
            else
            {
                *pl_id++ = CN_MEM_MANY_PAGE_GLOBAL;
                *pl_id = uf_grade_th;
            }
            result = ua_address;
        }
    }
    Lock_MutexPost(&Heap->HeapMutex);
    return result;
}
void *__M_Malloc(ptu32_t size,u32 timeout)
{
    return (__M_MallocHeap(size,tg_pSysHeap,timeout));
}

//----�����߳�ջ---------------------------------------------------------------
//���ܣ���m_malloc_gbl���ƣ���Ҫ���ڹ��첽�źŵ������µ��ã����������ṩ������
//      �̺߳���__CreateThreadʹ��
//������size,��������ڴ��ߴ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//-----------------------------------------------------------------------------
void *__M_MallocStack(ptu32_t size)
{
    struct tagHeapCession *Cession;
    void *ua_address;
    ufast_t  uf_grade_th;
    u16    *pl_id;
    void *result;

    if( (size == 0) || (tg_pSysHeap == NULL) )
        return NULL;
    if(__M_GetMaxFreeBlockHeap(tg_pSysHeap) < size)
    {
        result = NULL;
    }else
    {
        uf_grade_th=__M_GetFreeGrade(size,tg_pSysHeap,&Cession);    //ȡ�׺�
        ua_address=__M_MallocBlock(uf_grade_th,Cession);     //�����ڴ�

            //������id���м�¼���η��������
            //�Ķ����δ�������mem_global_t��index_event_id��Ա�����ע��.
            pl_id = &Cession->index_event_id
                    [(ptu32_t)((u8*)ua_address-Cession->heap_bottom)
                              /Cession->PageSize];
            if(0==uf_grade_th)
                *pl_id = CN_MEM_SINGLE_PAGE_GLOBAL;
            else
            {
                *pl_id++ = CN_MEM_MANY_PAGE_GLOBAL;
                *pl_id = uf_grade_th;
            }
            result = ua_address;
    }
    return result;
}

//----����1���ڴ�--------------------------------------------------------------
//����: ���ڴ���з���һ���ڴ��,�ڲ�����,�����Ÿ��û�
//����: grade,������Ŀ�Ľ׺�,0=1��,1=2��,����֮
//      Cession����Cession���з���
//����: ��õ��ڴ��ָ��,������ܷ����򷵻�NULL
//-----------------------------------------------------------------------------
void *__M_MallocBlock(ufast_t grade,struct tagHeapCession *Cession)
{
    ptu32_t ua_temp1;
    ptu32_t ua_pages_number;
    ufast_t uf_grade_th,uf_classes;
    u32 u32WordShift;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;

    ptu32_t ua_word_offset,ua_bit_num,ua_loop;
    ucpu_t  uc_msk;               //����ƫ����

    pppl_bitmap=Cession->ppp_bitmap; //���н�����λͼָ���ָ������ָ��
    ppl_bitmap=pppl_bitmap[grade];        //�ý׿��н�����λͼָ������ַ
    pl_classes=Cession->p_classes;   //��ȡ���׿��н�����������ָ��.
    uf_classes=pl_classes[grade]-1;       //�ý׿��н��������һ��ָ����
                                          //ppl_bitmap�е�ƫ����
    Cession->free_pages_num -= (1<<grade); //ʣ�����ҳ��
    ua_pages_number=0;
    do
    {//��ѭ�����ҵ�һ���ɷ����(��bit=0)��ƫ��λ��,�Ӹߵ��Ͳ���,����:
     //����,�ҳ��ý׿���λͼ��������߼�λͼ�е�һ��bit=0��λƫ����.
     //Ȼ��,�������ƫ��������θ߼���0λ����ƫ��,�������ֲ��ҳ�bit=0��λƫ��.
     //��˷��������ҵ����һ����һ��0λ��ƫ��,����Ǳ��η����Ŀ���.

        pl_bitmap=ppl_bitmap[uf_classes];   //��ȡ����λͼ��ָ��

        //ua_pages_number����һ����λƫ��,ua_temp1�Ǳ������һ�ֵ�����λƫ��
        ua_temp1=CN_CPU_BITS-1-__M_LeadingUcpuZero(~pl_bitmap[ua_pages_number]);

        //��һ������ƫ�ơ��ֳ�+��������λƫ�Ƶõ�������λƫ��
        ua_pages_number =(ua_pages_number<<CN_CPU_BITS_SUFFIX_ZERO)+ua_temp1;
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
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            if(ua_bit_num>=CN_CPU_BITS)   //ua_bit_num���������Ҫ��1��λ
            {//λ�����ڻ����1���ֿ�,��Ϊ����2��n�η��ֽ�,����Ҫ�����λ���϶�
             //����������,������������1�Ϳ�����.

                //��������,Ҳ����һ��λͼ������1��λ��
                ua_bit_num >>=CN_CPU_BITS_SUFFIX_ZERO;
                for(ua_loop =0; ua_loop < ua_bit_num; ua_loop++)
                    pl_bitmap[ua_word_offset+ua_loop]=CN_ALLBIT_1;
            }else if(ua_bit_num>0)
            //λ��С��һ���ֿ�,�Ҵ���0
            {
                uc_msk=cn_low_xbit_msk[ua_bit_num]<<u32WordShift;
                pl_bitmap[ua_word_offset] |=uc_msk;
                if(pl_bitmap[ua_word_offset] == CN_ALLBIT_1)
                //����1��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                    ua_bit_num=0;
                else
                    break;
            }else
            {//ǰһ�β���ʹһ���ִӷ�ȫ1���ȫ1����λ���ֶ�Ӧ����һ��·����Ӧλ
                pl_bitmap[ua_word_offset] |= 1<<u32WordShift;
                if(pl_bitmap[ua_word_offset] == CN_ALLBIT_1)
                {
                //����1��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                }
                else
                    break;
            }
        }
    }

    for(; uf_grade_th < Cession->uf_grades; uf_grade_th++)
    {//��ǰ��(����)����߽�,����׺Ÿ��ڻ���ڵ���������ڴ��λͼ.ÿ��ֻ��1λ��1

        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset = ua_pages_number >> uf_grade_th;
        if(ua_word_offset >= Cession->ua_pages_num>>uf_grade_th)
        //�ý��޶�Ӧ��,���ǿ��ܵ�,����һ����10ҳ���ڴ�,��9ҳ�͵�10ҳ���ڵ�3����û��
        //ӳ��.����ʽ�ұ��Ǳ����ܿ���,����Ǵ�0��Ƶ�ƫ����,���ֻ�����ܿ���-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //ȡ�øý׷����ָ������ַ

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //��������λƫ��,������ʽ��ȡģ������
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            pl_bitmap[ua_word_offset] |= 1<<u32WordShift;
            if(pl_bitmap[ua_word_offset] == CN_ALLBIT_1)
            {
            //����ģ��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
            }
            else
                break;
        }
    }

    //ˢ���������ڴ��
    if(Cession->free_pages_num != 0)
    {
        uf_grade_th=Cession->uf_grades-1;     //��߽׺�
        do{
            uf_classes= pl_classes[uf_grade_th];        //��uf_grade_th���ܼ���
            ppl_bitmap   = pppl_bitmap[uf_grade_th];    //λͼ������ָ��
            pl_bitmap    = ppl_bitmap[uf_classes-1];    //��߼�λͼָ��
            if(*pl_bitmap != (~0))
            {//·������ֻҪ����ȫ1�ͱ�ʾ�ý׿϶��п��п�.
                //���ݽ׺ż��������п�ĳߴ�.
                Cession->ua_free_block_max=Cession->PageSize<<uf_grade_th;
                break;
            }
        }while(uf_grade_th-- >0);   //����߽�(����)ɨ�赽0��
    }
    else
        Cession->ua_free_block_max = 0;
    return Cession->heap_bottom
                    +(ua_pages_number * Cession->PageSize);
}

//----���·���һ���ڴ�---------------------------------------------------------
//����: ���µĳߴ�,��pָ�����·���һ���ڴ�.
//      �������,���ԭ������ȫ��copy���·�����ڴ���.
//      �����С,���³ߴ�copyԭ������,���������ݽ�����.
//      ���p=NULL,���൱��__M_Malloc
//      ���NewSize = 0,���൱�� __M_Free
//      ������ڴ����ʧ��,ԭ�����ڴ�Ҳ�����ͷ�,����NULL
//      ������������ڴ�,��ʹ��free�����ͷ�.
//����: p, ���޸ĳߴ��ָ��
//      NewSize,�µ��ڴ�ߴ�
//����: �³ߴ���ڴ�ָ��,ע�������ԭָ��.
//------------
//���¼�¼:
// 1.����: 2015/3/16
//   ˵��: ���ȶԲ���NewSize���ж��߼�
//   ����: ������
//-----------------------------------------------------------------------------
void *__M_Realloc(void *p, ptu32_t NewSize,u32 Timeout)
{
    ptu32_t OldSize;
    void *NewP = NULL;

    if(NewSize == 0)
    {
        __M_Free(p);
        return NULL;
    }

    if(p == NULL)
    {
        //��NewSize = 0,����NULL
        NewP = __M_Malloc(NewSize,CN_TIMEOUT_FOREVER);
        return NewP;
    }

    OldSize = __M_CheckSize(p);
    if((NewSize > (OldSize>>1)) && (NewSize <= OldSize))
        return p;
    else
    {
        NewP = __M_Malloc(NewSize,CN_TIMEOUT_FOREVER);
        if(NewP)
        {
            memcpy(NewP,p,NewSize < OldSize ? NewSize:OldSize);
        }
        __M_Free(p);
    }
    return NewP;
}

//----��ȡ��ʽ���ߴ�-----------------------------------------------------------
//����: ��ȻӦ�ó���������������ߴ��ڴ棬���ڴ������������û����ڴ�ȴ������
//      �û�����Ĺ�񻯳ߴ���ڴ�飬�����������㣬������������size�ֽ��ڴ棬
//      ϵͳ�����������ڴ���ʵ�ʳߴ磬��������ִ�з����ڴ�Ĳ�����Ҳ����֤��
//      ��ķ�������ܷ�ɹ���
//����: size��Ӧ�ó���������ĳߴ�
//����: ��񻯳ߴ�
//------------
//���¼�¼:
// 1.����: 2015/3/16
//   ˵��: ���ӶԲ���size����ֵ�ж�
//   ����: ������
//-----------------------------------------------------------------------------
ptu32_t __M_FormatSizeHeap(ptu32_t size,struct tagHeapCB *Heap)
{
    ucpu_t  grade;
    struct tagHeapCession *Cession;
    ptu32_t tsize,fsize = CN_LIMIT_PTU32;
    u32 pages;

    if(0 == size)
        return (0);
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        pages = (size + Cession->PageSize -1)/Cession->PageSize;
        grade = __M_LeadingZero((u8 *)&pages,sizeof(u32));
        if((CN_HIGH_1BIT_1>>grade) != pages)
        //���grade���ǹ�񻯵���,���������������С������С
        //����ά��size�Ĵ�С,��������
            grade--;
        grade = CN_PTR_BITS-1-grade;
        tsize = Cession->PageSize << grade;
        if( (tsize < fsize) && (tsize <= Cession->ua_block_max) )
        {
            fsize = tsize;
        }
        Cession = Cession->Next;
    }
    return fsize;
}
ptu32_t __M_FormatSize(ptu32_t size)
{
    if(tg_pSysHeap != NULL)
        return __M_FormatSizeHeap(size,tg_pSysHeap);
    else
        return 0;
}

//----�ͷŴӶѷ�����ڴ�-------------------------------------------------------
//����: �ͷŴӶ��з����һ���ڴ棬�����ʱ��̬���仹û�г�ʼ������ִ��׼��̬
//      �ڴ��ͷš��ͷ��ڴ�����¼��������п�ĳߴ磬������¼����ڴ�ͬ��
//      �����У���ѵȴ����ڴ�ߴ�С���µ������п�ߴ���¼����
//������pl_mem,���ͷŵ��ڴ�ָ��
//���أ����󷵻�flase,��ȷʱ����true
//-----------------------------------------------------------------------------
void __M_FreeHeap(void * pl_mem,struct tagHeapCB *Heap)
{
    struct tagHeapCession *Cession;
    ptu32_t ua_temp1;
    ptu32_t ua_word_offset;
    u32 u32WordShift;          //����ƫ����
    ptu32_t ua_pages_no;
    ufast_t uf_grade_th,uf_free_grade_th;
    ufast_t uf_classes;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;
    uint16_t *pl_id;                //id;
    struct  tagEventECB  *event;
    ptu32_t ua_bit_num;
    ucpu_t  uc_msk;

    u8    event_mem_stage = 0;

    if( (pl_mem == NULL) || (Heap == NULL) )
        return;

    Cession = Heap->Cession;
    //����ѭ���ҳ����ͷŵ��ڴ����ĸ�Cession��
    while(Cession != NULL)
    {
        if(((u8*)pl_mem < Cession->heap_top) && ((u8*)pl_mem >= Cession->heap_bottom) )
            break ;
        Cession = Cession->Next;
    }

    if(Cession == NULL)
    {
        Djy_SaveLastError(EN_MEM_ERROR);  //���ͷŵ��ڴ治���ڴ����,ֱ���˳�.
        return;
    }

    //�����ͷŵ��ڴ�����ҳҳ��
    ua_pages_no=(ptu32_t)((ptu32_t)pl_mem-(ptu32_t)Cession->heap_bottom)
                                            / Cession->PageSize;

    Lock_MutexPend(&(Heap->HeapMutex),CN_TIMEOUT_FOREVER);
    //�����ͷŵ��ڴ��Ľ׺�,��0���.ͨ���׺�Ҳ����ȷ���ڴ��Ĵ�С.
    //ȷ���ڴ�������,�ֲ��ڴ���Ҫ֪��ӵ�и��ڴ���¼�id��
    //ȫ���ڴ����������ڴ������ٱ�,����֪��ӵ�и��ڴ���¼�id
    pl_id = &Cession->index_event_id[ua_pages_no];
    switch(pl_id[0])
    {
        case CN_MEM_DOUBLE_PAGE_LOCAL :
        {   //˫ҳ�ֲ��ڴ�,-1+id
            pl_id[1] = CN_MEM_FREE_PAGE;
            uf_free_grade_th = 1;
        }break;
        case CN_MEM_MANY_PAGE_LOCAL :
        {   //��ҳ�ֲ��ڴ�:-2+id+�׺�
            uf_free_grade_th = (ufast_t)pl_id[2];
            pl_id[1] = CN_MEM_FREE_PAGE;
            pl_id[2] = CN_MEM_FREE_PAGE;
        }break;
        case CN_MEM_SINGLE_PAGE_GLOBAL :
        {   //��ҳȫ���ڴ�:-3
            uf_free_grade_th = 0;
        }break;
        case CN_MEM_MANY_PAGE_GLOBAL :
        {   //˫(��)ҳȫ���ڴ�:-4+�׺�.
            uf_free_grade_th = (ufast_t)pl_id[1];
            pl_id[1] = CN_MEM_FREE_PAGE;
        }break;
        default :
        {
            if(pl_id[0] <= CN_EVENT_ID_LIMIT)
            {   //��ҳ�ֲ��ڴ�:id,
//                id = pl_id[0];
                pl_id[0] = CN_MEM_FREE_PAGE;
                uf_free_grade_th = 0;
            }else
            {
                Djy_SaveLastError(EN_MEM_ERROR);   //ָ���д�,ֱ���˳�
                Lock_MutexPost(&(Heap->HeapMutex) );
                pl_id[0] = CN_MEM_FREE_PAGE;
                return;

            }
        }break;
    }
    pl_id[0] = CN_MEM_FREE_PAGE;
    pppl_bitmap=Cession->ppp_bitmap; //���н�����λͼָ���ָ������ָ��
//    ppl_bitmap=pppl_bitmap[uf_free_grade_th];//�ý׿��н�����λͼָ������ַ
//    pl_bitmap=ppl_bitmap[0];                 //���н�����λͼָ��
    pl_classes=Cession->p_classes;    //��ȡ���׿��н�����������ָ��.
    uf_classes=pl_classes[uf_free_grade_th]-1;  //�ý׿��н��������һ��ָ����
                                                //ppl_bitmap�е�ƫ����

    //����λƫ��
//    ua_word_offset =ua_pages_no>>uf_free_grade_th;
    //��������λƫ��,������ʽ��ȡģ������
//    u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
//    ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //������ƫ��

    //����ҳ������
    Cession->free_pages_num += 1<<uf_free_grade_th;

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
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset = ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            if(ua_bit_num>=CN_CPU_BITS)   //ua_bit_num���������Ҫ��0��λ
            {//λ�����ڻ����1���ֿ�,��Ϊ����2��n�η��ּ�,����Ҫ�����λ���϶�
             //����������,������������0�Ϳ�����.

                //��������,Ҳ����һ��λͼ������0��λ��
                ua_bit_num >>=CN_CPU_BITS_SUFFIX_ZERO;
                //uc_word_shift�����ﵱ��ʱ����ʹ��
                for(u32WordShift =0; u32WordShift < ua_bit_num; u32WordShift++)
                    pl_bitmap[ua_word_offset+u32WordShift]=0;
            }else if(ua_bit_num > 0)
            {//λ��С��һ���ֿ�,�Ҵ���0
                uc_msk=~(cn_low_xbit_msk[ua_bit_num]<<u32WordShift);
                pl_bitmap[ua_word_offset] &=uc_msk;
                if(pl_bitmap[ua_word_offset] == uc_msk)
                //����0��λ���ڵ���ȫ1ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                    ua_bit_num=0;
                else
                    break;
            }else
            {//ǰһ�β���ʹһ���ִ�ȫ1->��0�����ֶ�Ӧ����һ��·����Ӧλ��0
                pl_bitmap[ua_word_offset] &= ~(1<<u32WordShift);
                if(pl_bitmap[ua_word_offset] == ~(1<<u32WordShift))
                {
                //����0��λ���ڵ���ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                }
                else
                    break;
            }
        }
    }

    for(;uf_grade_th<Cession->uf_grades;uf_grade_th++)
    {//��ǰ��(����)����߽�,����׺Ÿ��ڻ���ڵ���������ڴ��λͼ.ÿ��ֻ����1λ
        //ȡ��һ�׶�Ӧ���������нϵ͵�ַ��ƫ����
        ua_word_offset = (ua_pages_no >> uf_grade_th)<<1;
        //��������λƫ��,������ʽ��ȡģ������
        u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
        ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
        ppl_bitmap=pppl_bitmap[uf_grade_th-1]; //ȡ�øü������ָ������ַ
        pl_bitmap = ppl_bitmap[0];
        if( (pl_bitmap[ua_word_offset] & (3<<u32WordShift)))
            break;      //����λͼ�������㣬���ߵĽ�Ҳ��������

        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset=ua_pages_no>>uf_grade_th;
        if(ua_word_offset>=Cession->ua_pages_num>>uf_grade_th)
        //�ý��޶�Ӧ��,���ǿ��ܵ�,����һ����10ҳ���ڴ�,��9ҳ�͵�10ҳ���ڵ�3����
        //��ӳ��.����ʽ�ұ��Ǳ����ܿ���,����Ǵ�0��Ƶ�ƫ����,���ֻ�����ܿ���-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //ȡ�øü������ָ������ַ

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //��������λƫ��,������ʽ��ȡģ������
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            pl_bitmap[ua_word_offset] &= ~(1<<u32WordShift);
            if(pl_bitmap[ua_word_offset] == ~(1<<u32WordShift))
            {
            //����0��λ���ڵ���ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
            }
            else
                break;
        }
    }

    //ˢ���������ڴ��
    uf_grade_th=Cession->uf_grades-1;     //ȡ��߽׵Ľ׺�
    do{
        uf_classes   = pl_classes[uf_grade_th];     //��uf_grade_th�׵��ܼ���
        ppl_bitmap   = pppl_bitmap[uf_grade_th];    //λͼ������ָ��
        pl_bitmap    = ppl_bitmap[uf_classes-1];    //��߼�λͼָ��
        if(*pl_bitmap != (~0))
        {//·������ֻҪ����ȫ1�ͱ�ʾ�ý׿϶��п��п�.
            //���ݽ׺ż��������п�ĳߴ�.
            Cession->ua_free_block_max=Cession->PageSize<<uf_grade_th;
            break;
        }
    }while(uf_grade_th-- >0);   //����߽�(����)ɨ�赽0��

    Lock_MutexPost( &(Heap->HeapMutex) );
    g_ptEventRunning->local_memory--;

    //���ڴ�ȴ������������ڴ�֮��С�ڵ�ǰ��������ڴ�ļ����¼��ŵ�ready����
    //�ȴ�������˫��ѭ������
    Int_SaveAsynSignal();
    if(Heap->mem_sync != NULL)
    {
        event = Heap->mem_sync;   //ȡ�ȴ�����ͷ
        ua_temp1 = Cession->ua_free_block_max;
        while(1)
        {
            //��ϸ�����ﲢû�и���event����������Ϊevent�Ƴ��������д������
            //���Ӧ������������һ�䣺
            event = Heap->mem_sync;   //ȡ�ȴ�����ͷ
            //ua_word_offset�ڴ�Ȩ����ʱ����ʹ��,��񻯵�ǰ�¼���Ҫ���ڴ�.
            event_mem_stage = __M_GetGrade(event->wait_mem_size,Cession);
            ua_word_offset = Cession->PageSize<<(event_mem_stage);
            if(ua_word_offset <= ua_temp1)
            {//�¼��ȴ����ڴ���С���������ڴ��,���¼��ŵ���������
                //ʣ����óߴ���ٵ�ǰ������¼���������ڴ�,������ʵ��ִ������
                ua_temp1 -= ua_word_offset;
                event->sync_head = NULL;
                if(event->event_status & CN_STS_SYNC_TIMEOUT)
                    __Djy_ResumeDelay(event);
                event->event_status = CN_STS_EVENT_READY;
                event->wakeup_from = CN_STS_WAIT_MEMORY;
//                if(event->multi_next == event->multi_previous)
//                {//�����һ���¼�
//                    tg_MCB.mem_sync  = NULL;
//                    break;
//                }else   //bug
                if(event->multi_next == event)
                {//�����һ���¼�
                    Heap->mem_sync  = NULL;
                    break;
                }else

                {
                    Heap->mem_sync = event->multi_next;
                    event->multi_next->multi_previous = event->multi_previous;
                    event->multi_previous->multi_next = event->multi_next;
                }
                //������    ECB�ǲ�����VM�����û��������Ϊ����VM���ã��ŷ��ڴˣ�
                //�����Ҫ��maked�������Ƴ���
                if(NULL == event->vm)//����VM���ã���marked����ɾ��
                {
                    __Djy_CutEventFromEvttMarked(event);
                }
                else//������������ʱ�����ڴ�����õ��µ�
                {
                    //Ŀǰ����ʲô��������
                }
                __Djy_EventReady(event);
            }else
                break;
        }
    }
    Int_RestoreAsynSignal();  //�ָ��ж�״̬
    return;
}

void __M_Free(void * pl_mem)
{
    if( tg_pSysHeap == NULL)
        return;
    __M_FreeHeap(pl_mem,tg_pSysHeap);
    return;
}

//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ������������ڴ��ĳߴ�
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t __M_GetMaxFreeBlockHeap(struct tagHeapCB *Heap)
{
    struct tagHeapCession *Cession;
    ptu32_t result = 0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        if(result < Cession->ua_free_block_max)
            result = Cession->ua_free_block_max ;
        Cession = Cession->Next;
    }
    return result;
}
ptu32_t  __M_GetMaxFreeBlock(void)
{
    if(tg_pSysHeap != NULL)
        return __M_GetMaxFreeBlockHeap(tg_pSysHeap);
    else
        return 0;
}

//----��ѯ�ܿ����ڴ��С-------------------------------------------------------
//���ܣ����ؿ����ڴ��ֽ���
//��������
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __M_GetFreeMem(void)
{
    if(tg_pSysHeap != NULL)
        return __M_GetFreeMemHeap(tg_pSysHeap);
    else
        return 0;
}
ptu32_t  __M_GetFreeMemHeap(struct tagHeapCB *Heap)
{
    struct tagHeapCession *Cession;
    ptu32_t result=0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        result += Cession->free_pages_num * Cession->PageSize;
        Cession = Cession->Next;
    }
    return result;
}

