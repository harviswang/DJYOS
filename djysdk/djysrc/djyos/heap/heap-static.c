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
//�ļ�����: �Ӷ�����׼��̬�ڴ����
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
#include "stdint.h"
#include "core-cfg.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "align.h"
#include "int.h"
#include "lock.h"
#include "systime.h"
#include "heap-in.h"
#include "heap.h"

extern void *pHeapList;             //�ڽű��ж���
struct tagHeapCB *tg_pHeapList=NULL;   //����ָ�룬ϵͳ�����еĶѱ�������һ��
struct tagHeapCB *tg_pSysHeap=NULL;   //����ָ�룬ϵͳ�����еĶѱ�������һ��

void *__M_StaticMallocHeap(ptu32_t size,struct tagHeapCB *Heap,u32 Timeout);
void *__M_StaticMalloc(ptu32_t size,u32 timeout);
void __M_StaticFreeHeap(void *pl_mem,struct tagHeapCB *Heap);
void __M_StaticFree(void *pl_mem);
void *__M_StaticRealloc(void *, ptu32_t NewSize,u32 timeout);
void *__M_StaticMallocStack(ptu32_t size);

ptu32_t Heap_StaticModuleInit(ptu32_t para);

ptu32_t __M_StaticFormatSizeHeap(ptu32_t size,struct tagHeapCB *Heap);
ptu32_t __M_StaticFormatSize(ptu32_t size);
ptu32_t __M_StaticGetMaxFreeBlockHeap(struct tagHeapCB *Heap);
ptu32_t __M_StaticGetMaxFreeBlock(void);
ptu32_t M_GetHeapSizeHeap(struct tagHeapCB *Heap);
ptu32_t M_GetHeapSize(void);
struct tagHeapCB *M_FindHeap(const char *HeapName);       //��heap-dynamic.c��
ptu32_t __M_StaticGetFreeMem(void);
ptu32_t __M_StaticGetFreeMemHeap(struct tagHeapCB *Heap);
ptu32_t __M_StaticCheckSize(void * mp);

void *  (*M_Malloc)(ptu32_t size,u32 timeout);
void *  (*M_Realloc) (void *, ptu32_t NewSize,u32 timeout);
void    (*free)(void * pl_mem);
void *  (*M_MallocHeap)(ptu32_t size,struct tagHeapCB *Heap,u32 timeout);
void *  (*M_MallocLc)(ptu32_t size,u32 timeout);
void *  (*M_MallocLcHeap)(ptu32_t size,struct tagHeapCB *Heap, u32 timeout);
void    (*M_FreeHeap)(void * pl_mem,struct tagHeapCB *Heap);
void *  (*__MallocStack)(ptu32_t size);
ptu32_t (*M_FormatSizeHeap)(ptu32_t size,struct tagHeapCB *Heap);
ptu32_t (*M_FormatSize)(ptu32_t size);
ptu32_t (*M_GetMaxFreeBlockHeap)(struct tagHeapCB *Heap);
ptu32_t (*M_GetMaxFreeBlock)(void);
ptu32_t (*M_GetFreeMem)(void);
ptu32_t (*M_GetFreeMemHeap)(struct tagHeapCB *Heap);
ptu32_t (*M_CheckSize)(void * mp);

u32 __memStrLen(char *addr)
{
	u32 result = 0;
	while(*addr != '\0')
	{
		result ++;
		addr++;
	}
	return result;
}

void __memHeapScan(void)
{
    struct tagHeapCB *HeapTemp;
    struct tagHeapCession *Cession;
    u32 CessionNum,Ctrlsize,NameLen,n;
    u8  *Offset;
    u32 *u32Offset;
    u32 HeapNo=0;
    u32 AlignSize;
    Offset = (u8*)&pHeapList;
    CessionNum = *(u32*)Offset;
    while(CessionNum != 0)     //��ѭ��ȡ������ѿ��ƿ�����
    {
        u32Offset = (u32*)Offset + 1;
        AlignSize = *u32Offset++;
        //���¼��㹫ʽ���ο�lds�ļ���ʽ
        Offset += sizeof(u32)*3*CessionNum + 2*sizeof(u32);
        NameLen = __memStrLen((char*)Offset);
        //����heap���ƿ��session���ƿ�����Ҫ���ڴ�ߴ硣
        Ctrlsize = NameLen + 1 + sizeof( struct tagHeapCB )
                    + CessionNum * sizeof(struct tagHeapCession);
        Ctrlsize = align_up_sys(Ctrlsize);
        Cession = NULL;
        //����forѭ������:
        //1��ɨ������session���ҵ���һ������Ctrlsize�����session��
        //2���ڸ�session�����������ڿ��ƽṹ���ڴ档
        //3������sizeС��Ctrlsize��session��
        //4����׼��̬�����Ҫ���ʼ��heap���ƿ��session���ƿ顣
        //5�������е�session���ӳ�����
        for(n = 0; n < CessionNum; n++)
        {
            if(Cession != NULL)
            {
                Cession->static_bottom = (void*)(*u32Offset);
                Cession->heap_bottom = (void*)(*u32Offset);
                Cession->heap_top = (void*)(*(u32Offset+1));
#if ((CN_CFG_DYNAMIC_MEM == 1))
                Cession->PageSize = *(u32Offset+2);
#endif
                if(n == CessionNum -1)
                    Cession->Next = NULL;
                else
                    Cession->Next = Cession+1;
                Cession++;
            }
            //�ҵ��˵�һ������Ҫ���session
            if((Cession == NULL) && ((*(u32Offset+1) - *u32Offset) >=Ctrlsize))
            {
                HeapTemp = (struct tagHeapCB *)(*(u32Offset+1) - Ctrlsize);
                HeapTemp = (struct tagHeapCB *)align_down_sys(HeapTemp);
                if(tg_pHeapList == NULL)
                {
                    tg_pHeapList = HeapTemp;
                    tg_pHeapList->NextHeap = HeapTemp;
                    tg_pHeapList->PreHeap = HeapTemp;
                    //��һ���ѿ���ϵͳ�ѡ�
                    if(HeapNo == 0)
                        tg_pSysHeap = tg_pHeapList;
                }
                else
                {
                    HeapTemp->NextHeap = tg_pHeapList;
                    HeapTemp->PreHeap = tg_pHeapList->PreHeap;
                    tg_pHeapList->PreHeap->NextHeap = HeapTemp;
                    tg_pHeapList->PreHeap = HeapTemp;
                }
                Cession = (struct tagHeapCession*)(HeapTemp + 1);
                HeapTemp->Cession = Cession;
                HeapTemp->CessionNum = CessionNum - n;
                if(AlignSize == 0)
                    HeapTemp->AlignSize = CN_ALIGN_SIZE;
                else
                    HeapTemp->AlignSize = AlignSize;
                HeapTemp->HeapName = (char *)Offset;
#if ((CN_CFG_DYNAMIC_MEM == 1))
                HeapTemp->mem_sync = NULL;
#endif
                Cession->static_bottom = (void*)(*u32Offset);
                Cession->heap_bottom = (void*)(*u32Offset);
                Cession->heap_top = (void*)HeapTemp;
#if ((CN_CFG_DYNAMIC_MEM == 1))
                Cession->PageSize = *(u32Offset+2);
#endif
                if(n == CessionNum -1)
                    Cession->Next = NULL;
                else
                    Cession->Next = Cession+1;
                Cession++;
            }
            Ctrlsize -= sizeof(struct tagHeapCession);
            u32Offset += 3;
        }
        Offset += NameLen + 1;      //+1�����������
        Offset = (u8*)align_up(sizeof(u32),Offset);
        CessionNum = *(u32*)Offset;
        HeapNo++;
    }
}
//----�ڴ�ṹ��ʼ��-----------------------------------------------------------
//����: ɨ��ȫ��heap������Heap���ƿ��Cession���ƿ�������ڴ棬����ʼ����
//����: ��
//����: ����1.
//��ע: 1.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//      2.������������ȵ��ñ�����,������̬���书��,�˺󻹲��߱���̬���书��.
//      3.�����ù���ȷ���Ѳ����������ﲻ����顣
//-----------------------------------------------------------------------------
ptu32_t Heap_StaticModuleInit(ptu32_t para)
{
    __memHeapScan();

    M_Malloc = __M_StaticMalloc;
    M_Realloc = __M_StaticRealloc;
    free = __M_StaticFree;
    M_MallocHeap = __M_StaticMallocHeap;
    M_MallocLc = __M_StaticMalloc;
    M_MallocLcHeap = __M_StaticMallocHeap;
    M_FreeHeap = __M_StaticFreeHeap;
    __MallocStack = __M_StaticMallocStack;
    M_FormatSizeHeap = __M_StaticFormatSizeHeap;
    M_FormatSize = __M_StaticFormatSize;
    M_GetMaxFreeBlockHeap = __M_StaticGetMaxFreeBlockHeap;
    M_GetMaxFreeBlock = __M_StaticGetMaxFreeBlock;
    M_GetFreeMem = __M_StaticGetFreeMem;
    M_GetFreeMemHeap = __M_StaticGetFreeMemHeap;
    M_CheckSize = __M_StaticCheckSize;

    return 1;
}

//----׼��̬�ڴ����-----------------------------------------------------------
//���ܣ�ִ��׼��̬����ķ���������һ���ڴ棬�÷���ʵ������ģ�������������Ϊ����
//      ���϶��롣
//������size,��������ڴ��ߴ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: 1.׼��̬�����뾲̬�ڴ��������,���ַ��䷽��ʹ��ԭ�Ӳ�����ȷ������һ�£�
//      ������������,�ڴ治����ֱ�ӷ��ء�
//      ��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ���������׼��̬����
//------------
//���¼�¼:
// 1.����: 2015/3/16
//   ˵��: ���ӶԲ���size����ֵ�ж�
//   ����: ������
//-----------------------------------------------------------------------------
void *__M_StaticMallocHeap(ptu32_t size,struct tagHeapCB *Heap,u32 Timeout)
{
    void *temp,*result = NULL;
    struct tagHeapCession *Cession;
    atom_low_t atom_m;

    if((Heap == NULL) || (0 == size))
        return NULL;
    Cession = Heap->Cession;
    atom_m = Int_LowAtomStart();
    while(Cession != NULL)
    {
        temp = Cession->heap_bottom;   //���浱ǰ�ѵ�
        //���±����ߴ�Ŀռ�����
        Cession->heap_bottom = (u8 *)align_up(Heap->AlignSize,
                            (ptu32_t)Cession->heap_bottom + sizeof(ptu32_t));
        if((ptu32_t)(Cession->heap_top-Cession->heap_bottom) >= size)
        {
            //�·�����ڴ�ߴ籣���ڵ�ǰ�ѵ�ǰ��
            temp = Cession->heap_bottom - sizeof(ptu32_t);
            *(ptu32_t *)temp = align_up(Heap->AlignSize,size); //����������ڴ��ߴ�
            result = Cession->heap_bottom;
            Cession->heap_bottom += align_up(Heap->AlignSize,size);
            break;
        }
        else
        {
            Cession->heap_bottom = temp;   //�ָ���ǰ�ѵ�
        }
        Cession = Cession->Next;
    }
    Int_LowAtomEnd(atom_m);
    return(result);
}
//----�����߳�ջ---------------------------------------------------------------
//���ܣ�׼��̬������, ��malloc��һ�µ�.
//������size,��������ڴ��ߴ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//-----------------------------------------------------------------------------
void *__M_StaticMallocStack(ptu32_t size)
{
    return __M_StaticMallocHeap(size,tg_pSysHeap,0);
}

void *__M_StaticMalloc(ptu32_t size,u32 timeout)
{
    return __M_StaticMallocHeap(size,tg_pSysHeap,0);
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
void *__M_StaticRealloc(void *p, ptu32_t NewSize,u32 Timeout)
{
    ptu32_t OldSize;
    void *NewP = NULL;

    if(NewSize == 0)
    {
        __M_StaticFree(p);
        return NULL;
    }

    if(p == NULL)
    {
        //��NewSize = 0,����NULL
        NewP = __M_StaticMalloc(NewSize,0);
        return NewP;
    }

    OldSize = __M_StaticCheckSize(p);
    if((NewSize > (OldSize - tg_pSysHeap->AlignSize)) && (NewSize <= OldSize))
        return p;
    else
    {
        NewP = __M_StaticMalloc(NewSize,CN_TIMEOUT_FOREVER);
        if(NewP)
        {
            memcpy(NewP,p,NewSize < OldSize ? NewSize:OldSize);
        }
    }
    return NewP;
}

//----�ͷ�׼��̬������ڴ��---------------------------------------------------
//���ܣ��ͷ�׼��̬�����һ���ڴ�飬��heap��cession�в��ң�������ͷŵ��ڴ��ǴӸ�
//      session��������һ���ڴ棬�ͷ�֮��
//������pl_mem�����ͷŵ��ڴ��ָ��
//      Heap��ָ������������ͷš�
//���أ�true = �ɹ��ͷţ�false =  �ͷ�ʧ��
//��ע: 1.׼��̬�����뾲̬�ڴ��������,û�б�����ʩ,��ȷ��Ҫ����Ա�Լ���֤.����
//      ���䷽��Ҳ������������,��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ����
//      ������׼��̬����
//      2.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//-----------------------------------------------------------------------------
void __M_StaticFreeHeap(void * pl_mem,struct tagHeapCB *Heap)
{
    atom_low_t atom_m;
    struct tagHeapCession *Cession;
    ptu32_t *psize;

    if(pl_mem == NULL)
        return ;
    if(pl_mem != (void *)align_up(Heap->AlignSize,pl_mem))//�����϶���Ҫ���ָ��
        return ;
    Cession = Heap->Cession;
    atom_m = Int_LowAtomStart();
    while(Cession != NULL)
    {
        psize = (ptu32_t *)pl_mem;
        psize--;
        if( (*psize + (u8*)pl_mem) == Cession->heap_bottom)
        {
            //pl_mem�ǴӸ�cession��������ڴ棬�����ͷ�
            Cession->heap_bottom = (void*)align_down(Heap->AlignSize,psize);
            break;
        }
        Cession = Cession->Next;
    }
    Int_LowAtomEnd(atom_m);

    return ;
}
void __M_StaticFree(void * pl_mem)
{
    __M_StaticFreeHeap(pl_mem,tg_pSysHeap);
}

//----��ѯ�ѿռ��С---------------------------------------------------------
//���ܣ����ضѿռ��С,�˺������־�̬��׼��̬,��һ��.
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  M_GetHeapSizeHeap(struct tagHeapCB *Heap)
{
    struct tagHeapCession *Cession;
    ptu32_t result=0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        result += Cession->heap_top - Cession->static_bottom;
        Cession = Cession->Next;
    }
    return result;
}

ptu32_t  M_GetHeapSize(void)
{
    if(tg_pSysHeap != NULL)
        return M_GetHeapSizeHeap(tg_pSysHeap);
    else
        return 0;
}

//----��ȡ��ʽ���ߴ�-----------------------------------------------------------
//����: ��ȻӦ�ó���������������ߴ��ڴ棬���ڴ������������û����ڴ�ȴ������
//      �û�����Ĺ�񻯳ߴ���ڴ�飬�����������㣬������������size�ֽ��ڴ棬
//      ϵͳ�����������ڴ���ʵ�ʳߴ磬��������ִ�з����ڴ�Ĳ�����Ҳ����֤��
//      ��ķ�������ܷ�ɹ���
//����: size��Ӧ�ó���������ĳߴ�
//����: ��񻯳ߴ�
//-----------------------------------------------------------------------------
ptu32_t __M_StaticFormatSizeHeap(ptu32_t size,struct tagHeapCB *Heap)
{
    return align_up(Heap->AlignSize,size);  //׼��̬���䣬���뼴��
}
ptu32_t __M_StaticFormatSize(ptu32_t size)
{
    return align_up_sys(size);  //׼��̬���䣬���뼴��
}

//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ������������ڴ��ĳߴ�
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t __M_StaticGetMaxFreeBlockHeap(struct tagHeapCB *Heap)
{
    struct tagHeapCession *Cession;
    ptu32_t result = 0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        if(result < (Cession->heap_top - Cession->heap_bottom))
            result = Cession->heap_top - Cession->heap_bottom;
        Cession = Cession->Next;
    }
    return result;
}
ptu32_t  __M_StaticGetMaxFreeBlock(void)
{
    if(tg_pSysHeap != NULL)
        return __M_StaticGetMaxFreeBlockHeap(tg_pSysHeap);
    else
        return 0;
}

//----��ѯ�ܿ����ڴ��С-------------------------------------------------------
//���ܣ����ؿ����ڴ��ֽ���
//��������
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __M_StaticGetFreeMem(void)
{
    if(tg_pSysHeap != NULL)
        return __M_StaticGetFreeMemHeap(tg_pSysHeap);
    else
        return 0;
}
ptu32_t  __M_StaticGetFreeMemHeap(struct tagHeapCB *Heap)
{
    struct tagHeapCession *Cession;
    ptu32_t result=0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        result += Cession->heap_top - Cession->heap_bottom;
        Cession = Cession->Next;
    }
    return result;
}

//----��ѯ�ڴ�ߴ�-------------------------------------------------------------
//����: ���ݸ�����ָ��,��ѯ��ָ�����ڵ��ڴ��ĳߴ�.
//����: mp,��̬������ڴ�ָ��.
//����: �ڴ��ߴ�,����0�м��ֺ���:1.�Ƿ�ָ��,2.mp����׼��̬�����ָ��.
//-----------------------------------------------------------------------------
ptu32_t __M_StaticCheckSize(void * mp)
{
    return *(ptu32_t*)(((ptu32_t*)mp) -1);
}

