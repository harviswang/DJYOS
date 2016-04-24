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
//�ļ�����:�ṩ�̶���������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __POOL_H__
#define __POOL_H__
#include "stdint.h"
#include "lock.h"
#ifdef __cplusplus
extern "C" {
#endif

struct tagMemCellPool
{
    struct tagRscNode memb_node;    //��Դ���,�����ڴ�ؽ���Ϊһ����Դ���
    void  *continue_pool;    //�����ڴ���׵�ַ��ʹ������������ʵʱ�ԡ�
    struct tagMemCellFree  *free_list;        //δ���������,����,NULL��β
    struct tagSemaphoreLCB memb_semp;
    ptu32_t pool_offset;    //�������е�ƫ����(��ǰ��ַ)
    u32 cell_size;          //���С,��ʼ��ʱ����ϵͳҪ������ߴ硣
    u32 cell_increment;     //���ڴ�غľ�ʱ�����Զ��Ӷ��������ڴ�سߴ磬��λΪ
                            //�ڴ����������Ϊ0��ʾ��������ע�⣬�ڴ�سߴ���ֻ
                            //�������ģ���һ���Ӷ����������ڴ棬�ǲ��ͷŵġ�
    u32 cell_limit;         //���increment !=0������Ա�޶����ڴ�ص����ߴ磬
                            //��λΪ�ڴ������
    struct  tagEventECB *cell_sync;   //ͬ�����С�
    void * next_inc_pool;   //������ַ�����ڼ�¼���ڴ�����й����е�������������
                            //ɾ���ڴ��ʱ�ͷ�֮��
};

//�����ڴ�鹹��˫��ѭ����������ʼ���ڴ��ʱ�������ڴ���������������ͷ�ʱ
//�ż��롣
//��������ڵ㣬��free_list����˫�������������ӳ���Ľ�׳�ԣ�����ǵ�������
//��Ȼ����ʵ�ֹ��ܣ����ǣ�����ظ��ͷ�һ���Ѿ��ͷŵ��ڴ�飬����ܵ����ƻ�����
//��˫��������������жϸý���Ƿ��Ѿ���free_list�У����ǣ����ظ�ִ���ͷŲ�
//��������������Ȼ�����ñ����ķ����ж��Ƿ���free_list����ִ��ʱ�䲻ȷ����������
//ʵʱϵͳ��Ҫ��
//��˫������ĺ���ǣ������ڴ���������ָ�볤�ȣ�ͨ��Ϊ8�ֽڡ�
struct tagMemCellFree
{
    struct tagMemCellFree *previous;
    struct tagMemCellFree *next;
};
ptu32_t Mb_ModuleInit(ptu32_t para);
struct tagMemCellPool *Mb_CreatePool(void *pool_original,u32 capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name);
struct tagMemCellPool *Mb_CreatePool_s(struct tagMemCellPool *pool,
                                void *pool_original,u32 capacital,
                                u32 cell_size,u32 increment,
                                u32 limit,char *name);
bool_t Mb_DeletePool(struct tagMemCellPool *pool);
bool_t Mb_DeletePool_s(struct tagMemCellPool *pool);
void *Mb_Malloc(struct tagMemCellPool *pool,u32 timeout);
void Mb_Free(struct tagMemCellPool *pool,void *block);
u32 Mb_QueryFree(struct tagMemCellPool *pool);
u32 Mb_QueryCapacital(struct tagMemCellPool *pool);

#ifdef __cplusplus
}
#endif

#endif //__POOL_H__

