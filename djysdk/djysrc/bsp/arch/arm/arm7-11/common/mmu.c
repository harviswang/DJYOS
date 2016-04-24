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

#include "stdint.h"

extern u32 mmu_page_table[];

// ---- mmu��ӳ��--------------------------------------------------------------
//����: �������ַvaddr��ʼ��һ����(1M)ӳ�䵽�����ַpaddr�У�ͬʱ���øöε�
//      cache��д�������ԡ�ӳ��󣬷���vaddr����mmu�ض���paddr�С�
//ҳ����ĿC(bit3),B(bit2)λ���嶨��:
// C B   дͨ��cache         д����cache         ��ѡ��cache
// 0 0   ��cache����д����   ��cache����д����   ��cache����д����
// 0 1   ��cache����д����   ��cache����д����   ��cache����д����
// 1 0   ��cache����д����   ����Ԥ��            дͨ��cache����д����
// 1 1   ��cache����д����   ��cache����д����   д�ؿ�cache����д����
//���أ���
//-----------------------------------------------------------------------------
u32 MMU_MapSection(u32 vaddr, u32 paddr, int properties)
{
    mmu_page_table[vaddr>>20] = (paddr & 0xfff00000)|(properties&0xFFF);

    return 1;
}

// ---- ��ĳ��entry������TLB�� ----
// ��ĳЩ��ַ�任������TLB�У�����������Щ�����ַ�������ַ�ķ��ʾͻ��ø���Ѹ��
// ��Ҫ�Ӳ���
// ---- C10
void MMU_LockEntry()
{

}

// ---- ʹָ��TLB�е�һ����Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbInstructionEntry(void)
{
}

// ---- ʹ����TLB�е�һ����Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbDatasEntry(void)
{
}

// ---- ʹָ��TLB�е�������Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbInstructionsAll(void)
{
}

// ---- ʹ����TLB�е�������Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbDatasAll(void)
{
}

// ---- ʹͳһTLB�е�������Ŀ��Ч ----
// ʹͳһTLBʧЧ����ָ�������ݵ�TLB��ʧЧ
// ---- C8
void MMU_InvalidateTlbsAll(void)
{
}

// ---- ����MMUһ��ҳ��Ļ�ַ ----
void MMU_SetSectionsBase(u32 *base)
{
    __asm__ volatile (
            "mcr    p15, 0, r0, c2, c0, 0 @ҳ�����ַ \n\t"
            :
            :"r"(base)
            :"memory"
    );
}

// ---- ����MMU�����Ȩ�� -----------------------------------------------------
//����: ����mmu������ƣ�arm��������16����ÿ������Զ������Ȩ�ޡ���ҳ���У�
//      ����ָ���öεķ���Ȩ�����ĸ�����ơ�
//����: authorization,����Ȩ�޿����֣�ÿ����ռ��λ
//����: ��
//-----------------------------------------------------------------------------
void MMU_SetDomainAccess(u32 authorization)
{
    __asm__ volatile (
            "mcr    p15, 0, r0, c3, c0, 0 @д��Ĵ��� \n\t"
            :
            :"r"(authorization)
            :"memory"
    );
}

// ---- ��ֹMMU ----
void MMU_Disable(void)
{
    __asm__ volatile (
            "mrc    p15, 0, r0, c1, c0, 0 \n\t"
            "bic    r0, r0, #0x1 @��ֹmmu \n\t"
            "mcr    p15, 0, r0, c1, c0, 0 \n\t"
            :
            :
            :"memory"
    );
}

// ---- ʹ��MMU ----
void MMU_Enable(void)
{
    __asm__ volatile (
            "mrc    p15, 0, r0, c1, c0, 0 \n\t"
            "orr    r0, r0, #0x1 @����mmu \n\t"
            "mcr    p15, 0, r0, c1, c0, 0 \n\t"
            :
            :
            :"memory"
    );
}

