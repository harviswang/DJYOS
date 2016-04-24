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
//����ģ��: �ں�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ���ز���ϵͳ
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����:  ������.
//   �°汾�ţ�V1.1.0
//   �޸�˵��: ԭ�汾�У���ϵͳ��ʼ��Ҳ���������ˣ����汾����֮
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "int_hard.h"
#include "copy_table.h"

#if CN_CFG_CACHE_USED == 1
#include "cache.h"
#endif

extern struct copy_table sysload_copy_table;

//----����ϵͳ�ں˼��س���-----------------------------------------------------
//���ܣ��������в���ϵͳ�ں˴��룬�Լ���siģʽ��ȫ��Ӧ�ó�����롣
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
void loader(void)
{
   volatile  u32 *src,*des;
   volatile  u32 i, j;

    for(i=0; i<sysload_copy_table.record_cnt; i++) {
        src = (u32*) sysload_copy_table.record[i].load_start_address;
        des = (u32*) sysload_copy_table.record[i].run_start_address;
        if(sysload_copy_table.record[i].type == 1) {    //copy
            if(src != des) {
                for(j=0; j<sysload_copy_table.record[i].size; src++,des++) {
                    *des=*src;
                    j+=4;
                }
            }
        } else if(sysload_copy_table.record[i].type == 0) {    //zero init
            for(j=0; j<sysload_copy_table.record[i].size; des++) {
                *des=0;
                j+=4;
            }
        }
    }

#if CN_CFG_CACHE_USED == 1
 //   Cache_CleanData();
 //   Cache_InvalidInst();
#endif
}

ptu32_t Heap_StaticModuleInit(ptu32_t para);
void Int_Init(void);
void critical(void);
void Sys_Start(void);
//----����ϵͳ�ں˼��س���-----------------------------------------------------
//���ܣ��������в���ϵͳ�ں˴��룬�Լ���siģʽ��ȫ��Ӧ�ó�����롣
//����: �ޡ�
//����: �ޡ�
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
//----------------------------------------------------------------------------
void Pre_Start(void)
{
    extern bool_t Exp_Init(void);
    //�Դ˿��Ե���malloc�庯������׼��̬�ѷ����㷨����Ҫ�ͷ��ڴ棬Ҫ���ϸ����
    //����malloc-free���������߲�����
    Heap_StaticModuleInit(0);
    Exp_Init();
    Int_Init();
    critical();
    loader();

    Sys_Start();        //��ʼ����ϵͳ
}
