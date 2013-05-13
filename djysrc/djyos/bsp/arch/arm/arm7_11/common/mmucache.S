
.section	.mmutable, "ax", %note
.align	14
.global mmu_page_table
mmu_page_table:
	.space	0x4000


@页表条目C(bit3),B(bit2)位含义定义:
@ C B   写通型cache         写回型cache         可选型cache
@ 0 0   禁cache，禁写缓冲   禁cache，禁写缓冲   禁cache，禁写缓冲
@ 0 1   禁cache，开写缓冲   禁cache，开写缓冲   禁cache，开写缓冲
@ 1 0   开cache，禁写缓冲   不可预测            写通开cache，开写缓冲
@ 1 1   开cache，开写缓冲   开cache，开写缓冲   写回开cache，开写缓冲
//初始化页表
.section .text, "ax", %progbits
.align 2
.global init_pages_table
init_pages_table:
	STMFD	R13!, {R14}	
	@以下初始化L1页表，平板式全映射
    ldr     r0, =mmu_page_table  @L1页表地址
    mov     r1, #0
    ldr     r3, =0xc16   @不开cache,开写缓冲
	mov     r4,#0x1000
loopnommu:
    mov     r2, r1, lsl #20   @目标段编号写入L1条目的高12位
    add     r2, r2, r3
    str     r2, [r0], #4
    add     r1, r1, #1
    cmp     r1, r4
    bne     loopnommu

	
    ldr     r0, =mmu_page_table
    mcr     p15, #0, r0, c2, c0, #0   @页表基地址
    ldr     r0, =0xffffffff          @全部域具有管理者权限
    mcr     p15, #0, r0, c3, c0, #0   @写域寄存器

	LDMFD	R13!, {PC}	



