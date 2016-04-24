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

#ifndef __MMU_H_
#define __MMU_H_

#ifdef __cplusplus
extern "C" {
#endif

extern u32 mmu_page_table[];

// cp15Э�������еļĴ�������
// c0 - ID���루ֻ����

// c1 - ���ƼĴ���
#define CN_BM_CP15_C1_MMU_EN 0x00000001 //ʹ��MMU����PU
#define CN_CP15_C1_MMU_EN 0
#define CN_CP15_C1_MEM_ALIGN_CHECK_EN 0x00000002 //ʹ���ڴ����ʱ�ĵ�ַ������
#define CN_BO_CP15_C1_MEM_ALIGN_CHECK_EN 1
#define CN_BM_CP15_C1_CACHE_DATA_EN 0x00000004 //ʹ������Cache�����ָ�������Cacheͳһʱ���������λ��ͬʱ��ָֹ�������Cache��
#define CN_BO_CP15_C1_CACHE_DATA_EN 2
#define CN_BM_CP15_C1_WRITE_BUFFER_EN 0x00000008 //ʹ��д����
#define CN_BO_CP15_C1_WRITE_BUFFER_EN 3
#define CN_BM_CP15_C1_MEM_BIG_ENDIAN 0x00000080 //ѡ��ʹ��big-endian�ڴ�ģʽ�����������ֶ�ģʽ��֧�ֵ�ARM����Ч��
#define CN_BO_CP15_C1_MEM_BIG_ENDIAN 7
#define CN_BM_CP15_C1_SYS_PROTECTION 0x00000100 //�ڻ���MMU�Ĵ洢ϵͳ�У�������λ����ϵͳ����
#define CN_BO_CP15_C1_SYS_PROTECTION 8
#define CN_BM_CP15_C1_ROM_PROTECTION 0x00000200 //�ڻ���MMU�Ĵ洢ϵͳ�У�������λ����ROM����
#define CN_BO_CP15_C1_ROM_PROTECTION 9
#define CN_BM_CP15_C1_BR_PREDICTION_EN 0x00000800 //ʹ����תԤ�⹦�ܣ����֧�֣�
#define CN_BO_CP15_C1_BR_PREDICTION_E
#define CN_BM_CP15_C1_CACHE_INSTRUCTION_EN 0x00001000 //ʹ��ָ��Cache
#define CN_BO_CP15_C1_CACHE_INSTRUCTION_EN 12
#define CN_BM_CP15_C1_EXCEPTIONS_LOCATION 0x00002000 //�쳣�ж��������λ��ѡ��0:0x0-0x1c;1:0xFFFF0000-0xFFFF001C��
#define CN_BO_CP15_C1_EXCEPTIONS_LOCATION 13
#define CN_BM_CP15_C1_RR_STRATEGY 0x00004000 //ϵͳ��Cache����̭�㷨��0:�����̭��1:Round-robin��̭�㷨��
#define CN_BO_CP15_C1_RR_STRATEGY 14

// һ���������и���λ�Ķ��壨�ζ��壩
#define CN_BM_FIRST_LEVEL_DES_AP 0x00000C00 //AP����λ
#define CN_BO_FIRST_LEVEL_DES_AP 10
#define CN_BM_FIRST_LEVEL_DES_DOMAIN 0x000001E0 //��
#define CN_BO_FIRST_LEVEL_DES_DOMAIN 5
#define CN_BM_FIRST_LEVEL_DES_CACHE 0x00000008 //Cλ
#define CN_BO_FIRST_LEVEL_DES_CACHE 3 //Cλ
#define CN_BM_FIRST_LEVEL_DES_BUFFER 0x00000004 //Bλ
#define CN_BO_FIRST_LEVEL_DES_BUFFER 2 //Bλ

u32 MMU_MapSection(u32 vaddr, u32 paddr, int properties);
void MMU_LockEntry();
void MMU_InvalidateTlbInstructionEntry(void);
void MMU_InvalidateTlbDatasEntry(void);
void MMU_InvalidateTlbInstructionsAll(void);
void MMU_InvalidateTlbDatasAll(void);
void MMU_InvalidateTlbsAll(void);
void MMU_SetSectionsBase(u32 *base);
void MMU_SetDomainAccess(u32 authorization);
void MMU_Disable(void);
void MMU_Enable(void); 

#ifdef __cplusplus
}
#endif

#endif /* __MMU_H_ */
