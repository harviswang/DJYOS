/*
 * mmu.c
 *
 *  Created on: 2011-12-5
 *      Author: djyos
 */

#include "os_inc.h"

#if cfg_mmu_used == 1

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
u32 mmu_map_section(u32 vaddr, u32 paddr, int properties)
{
    mmu_page_table[vaddr>>20] = (paddr & 0xfff00000)|(properties&0xFFF);

    return 1;
}

// ---- ��ĳ��entry������TLB�� ----
// ��ĳЩ��ַ�任������TLB�У�����������Щ�����ַ�������ַ�ķ��ʾͻ��ø���Ѹ��
// ��Ҫ�Ӳ���
// ---- C10
void mmu_lock_entry()
{

}

// ---- ʹָ��TLB�е�һ����Ŀ��Ч ----
// ---- C8
void mmu_invalidate_tlb_instruction_entry(void)
{
}

// ---- ʹ����TLB�е�һ����Ŀ��Ч ----
// ---- C8
void mmu_invalidate_tlb_datas_entry(void)
{
}

// ---- ʹָ��TLB�е�������Ŀ��Ч ----
// ---- C8
void mmu_invalidate_tlb_instructions_all(void)
{
}

// ---- ʹ����TLB�е�������Ŀ��Ч ----
// ---- C8
void mmu_invalidate_tlb_datas_all(void)
{
}

// ---- ʹͳһTLB�е�������Ŀ��Ч ----
// ʹͳһTLBʧЧ����ָ�������ݵ�TLB��ʧЧ
// ---- C8
void mmu_invalidate_tlbs_all(void)
{
}

// ---- ����MMUһ��ҳ��Ļ�ַ ----
void mmu_set_sections_base(u32 base)
{
	__asm__ volatile (
			"mcr	p15, 0, r0, c2, c0, 0 @ҳ�����ַ \n\t"
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
void mmu_set_domain_access(u32 authorization)
{
	__asm__ volatile (
			"mcr 	p15, 0, r0, c3, c0, 0 @д��Ĵ��� \n\t"
			:
			:"r"(authorization)
			:"memory"
	);
}

// ---- ��ֹMMU ----
void mmu_disable(void)
{
	__asm__ volatile (
			"mrc	p15, 0, r0, c1, c0, 0 \n\t"
			"bic	r0, r0, #0x1 @��ֹmmu \n\t"
			"mcr	p15, 0, r0, c1, c0, 0 \n\t"
			:
			:
			:"memory"
	);
}

// ---- ʹ��MMU ----
void mmu_enable(void)
{
	__asm__ volatile (
			"mrc	p15, 0, r0, c1, c0, 0 \n\t"
			"orr	r0, r0, #0x1 @����mmu \n\t"
			"mcr	p15, 0, r0, c1, c0, 0 \n\t"
			:
			:
			:"memory"
	);
}

#endif
