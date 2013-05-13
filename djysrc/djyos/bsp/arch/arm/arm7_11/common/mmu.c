/*
 * mmu.c
 *
 *  Created on: 2011-12-5
 *      Author: djyos
 */

#include "os_inc.h"

#if cfg_mmu_used == 1

// ---- mmu段映射--------------------------------------------------------------
//功能: 将虚拟地址vaddr开始的一个段(1M)映射到物理地址paddr中，同时设置该段的
//      cache及写缓冲属性。映射后，访问vaddr将被mmu重定向到paddr中。
//页表条目C(bit3),B(bit2)位含义定义:
// C B   写通型cache         写回型cache         可选型cache
// 0 0   禁cache，禁写缓冲   禁cache，禁写缓冲   禁cache，禁写缓冲
// 0 1   禁cache，开写缓冲   禁cache，开写缓冲   禁cache，开写缓冲
// 1 0   开cache，禁写缓冲   不可预测            写通开cache，开写缓冲
// 1 1   开cache，开写缓冲   开cache，开写缓冲   写回开cache，开写缓冲
//返回：无
//-----------------------------------------------------------------------------
u32 mmu_map_section(u32 vaddr, u32 paddr, int properties)
{
    mmu_page_table[vaddr>>20] = (paddr & 0xfff00000)|(properties&0xFFF);

    return 1;
}

// ---- 将某个entry锁定在TLB中 ----
// 将某些地址变换锁定在TLB中，这样对于这些虚拟地址到物理地址的访问就会变得更加迅速
// 需要加参数
// ---- C10
void mmu_lock_entry()
{

}

// ---- 使指令TLB中的一个条目无效 ----
// ---- C8
void mmu_invalidate_tlb_instruction_entry(void)
{
}

// ---- 使数据TLB中的一个条目无效 ----
// ---- C8
void mmu_invalidate_tlb_datas_entry(void)
{
}

// ---- 使指令TLB中的所有条目无效 ----
// ---- C8
void mmu_invalidate_tlb_instructions_all(void)
{
}

// ---- 使数据TLB中的所有条目无效 ----
// ---- C8
void mmu_invalidate_tlb_datas_all(void)
{
}

// ---- 使统一TLB中的所有条目无效 ----
// 使统一TLB失效，即指令与数据的TLB都失效
// ---- C8
void mmu_invalidate_tlbs_all(void)
{
}

// ---- 设置MMU一级页表的基址 ----
void mmu_set_sections_base(u32 base)
{
	__asm__ volatile (
			"mcr	p15, 0, r0, c2, c0, 0 @页表基地址 \n\t"
			:
			:"r"(base)
			:"memory"
	);
}

// ---- 设置MMU域访问权限 -----------------------------------------------------
//功能: 设置mmu的域控制，arm可以设置16个域，每个域可以定义访问权限。在页表中，
//      可以指定该段的访问权限受哪个域控制。
//参数: authorization,访问权限控制字，每个域占两位
//返回: 无
//-----------------------------------------------------------------------------
void mmu_set_domain_access(u32 authorization)
{
	__asm__ volatile (
			"mcr 	p15, 0, r0, c3, c0, 0 @写域寄存器 \n\t"
			:
			:"r"(authorization)
			:"memory"
	);
}

// ---- 禁止MMU ----
void mmu_disable(void)
{
	__asm__ volatile (
			"mrc	p15, 0, r0, c1, c0, 0 \n\t"
			"bic	r0, r0, #0x1 @禁止mmu \n\t"
			"mcr	p15, 0, r0, c1, c0, 0 \n\t"
			:
			:
			:"memory"
	);
}

// ---- 使能MMU ----
void mmu_enable(void)
{
	__asm__ volatile (
			"mrc	p15, 0, r0, c1, c0, 0 \n\t"
			"orr	r0, r0, #0x1 @允许mmu \n\t"
			"mcr	p15, 0, r0, c1, c0, 0 \n\t"
			:
			:
			:"memory"
	);
}

#endif
