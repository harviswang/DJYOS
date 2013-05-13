//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//��Ȩ���� (C), 2005-2009,   lst.
//����ģ��:  CPU��ʼ��
//���ߣ�     lst
//�汾��      V1.0.0
//��ʼ�汾������ڣ�2009-08-05
//�ļ�����:  cpuʱ�ӳ�ʼ��
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-05
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include "sysctl.h"

// ---- ��ʼ��MMU ----
// ӳ����ƣ���SDRAM��������ӳ�䵽0x00000000�ĵ�ַ��ȥ��ע��ӳ�����������
// S3C2440��ָ������ݵ�Cache�Ƿֿ��ġ�
// �ڽ���ӳ�����ʱ������Ҫ����C��Bλ��
void init_mmu(void)
{
	register vu32 i;

    // 0~0x2FFFFFFF�εĳ�ʼ����������п��ܷ��õ���ROM������Cache��Buffer
    for(i=0x00000000; i<0x30000000; ) {
    	mmu_map_section(i, i, 0xc1e);	// ƽ��ʽ��ӳ��
    	i += 0x100000;
    }

    // 0x30000000~0x3FFFFFFF��ӳ��
    mmu_map_section(0x30000000, 0x30000000, 0xc12);	//��cache��
    // ����0x30000000~0x33FFFFFF��63MB��SDRAM������Cache��Buffer
    for(i=0x30100000; i<0x40000000; ) {
    	mmu_map_section(i, i, 0xc1e);	// ƽ��ʽ��ӳ��
    	i += 0x100000;
    }

    // 0x40000000~0x5FFFFFFF��ӳ��
    // ����������Ĵ����εĵ�ַ�����ڲ���IO�ڣ�������ﲻ��ʹ��Cache�������Կ���Buffer
    for(i=0x40000000; i<0x60000000; ) {
    	mmu_map_section(i, i, 0xc12);	// ƽ��ʽ��ӳ��
    	i += 0x100000;
    }

//    // �����ַ��������
//    for(i=0x60000000; i != 0; i += 0x100000) {
//    	mmu_map(i, i, 0x016);	// ƽ��ʽ��ӳ��
//    }

#ifdef debug
    mmu_map_section(0x00000000, 0x30100000, 0xc1e);
#endif
    mmu_set_sections_base(mmu_page_table);
    mmu_set_domain_access(0xffffffff);          //��������й�����Ȩ��
    cache_enable_all();
    cache_enable_write_buf();
    mmu_enable();
}
//��ʱ�ڴ�δ��ʼ���ã�����ʹ�þֲ�����
void init_cpu_clk(void)
{
	register struct clk_ctrl_reg volatile * clk_ctrl
	                            = (struct clk_ctrl_reg*)0x4c000000;
	clk_ctrl->CAMDIVN = 0;
	clk_ctrl->CLKDIVN = (1<<bo_clkdivn_udivn)    //uclk = upll���/2
                              +(2<<bo_clkdivn_hdivn)    //hclk = fclk/4=100M
                              +(1<<bo_clkdivn_pdivn);   //pclk = hckk/2=50M
    
    __asm__       //������δ��ʼ����������forѭ������Ϊ����Ҫʹ���ڴ�
    (
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
    );
    
//    pg_clk_ctrl_reg->MPLLCON = (cn_mpll_mdiv<<bo_mpll_mdiv)
//                              +(cn_mpll_pdiv<<bo_mpll_pdiv)
//                              +(cn_mpll_sdiv<<bo_mpll_sdiv);
    //dmg�������ʱ�ӷ�Ƶ�����޸ģ�ԭ��������û��ʹ�ùٷ��Ƽ��ı��������
    //����ʱ�ӼĴ������ÿɵõ�405MHz����Ƶ�����ԭ������Ϊ399.65Mhz��
    clk_ctrl->MPLLCON = (0x7F<<bo_mpll_mdiv)
                              +(0x02<<bo_mpll_pdiv)
                              +(0x01<<bo_mpll_sdiv);
}
void init_mem_bus(void)
{
	register struct memory_ctrl_reg volatile * const memory_ctrl
	                            = (struct memory_ctrl_reg*)0x48000000;
    register u32 xr0;
    xr0 = memory_ctrl->BWSCON;
    xr0 &= 0x00fffff0;       //���bank0,bank6/7
    xr0  |= (0<<bo_bus_ublb_bank7) +(0<<bo_bus_wait_bank7)
                                   +(2<<bo_bus_width_bank7)
                                   +(0<<bo_bus_ublb_bank6)
                                   +(0<<bo_bus_wait_bank6)
                                   +(2<<bo_bus_width_bank6)
                                   +(0<<bo_bus_ublb_bank4)
                                   +(0<<bo_bus_wait_bank4)
                                   +(1<<bo_bus_width_bank4);
    memory_ctrl->BWSCON = xr0;
    memory_ctrl->BANKCON0 = (3<<bo_bankcon_tacs)
                                  +(3<<bo_bankcon_tcos)
                                  +(6<<bo_bankcon_tacc)
                                  +(3<<bo_bankcon_tcoh)
                                  +(3<<bo_bankcon_tcah)
                                  +(1<<bo_bankcon_tacp)
                                  +(0<<bo_bankcon_pmc);
    memory_ctrl->BANKCON4 = (3<<bo_bankcon_tacs)
                                  +(3<<bo_bankcon_tcos)
                                  +(6<<bo_bankcon_tacc)
                                  +(3<<bo_bankcon_tcoh)
                                  +(3<<bo_bankcon_tcah)
                                  +(1<<bo_bankcon_tacp)
                                  +(0<<bo_bankcon_pmc);
    memory_ctrl->BANKCON6 = (3<<bo_b67_mt)
                                  +(0<<bo_b67_trcd)
                                  +(1<<bo_b67_scan);
    memory_ctrl->BANKCON7 = (3<<bo_b67_mt)
                                  +(0<<bo_b67_trcd)
                                  +(1<<bo_b67_scan);
    memory_ctrl->REFRESH = (1<<bo_refresh_refen)
                                 +(0<<bo_refresh_trefmd)
                                 +(0<<bo_refresh_trp)
                                 +(1<<bo_refresh_trc)
                                 +(0<<bo_refresh_tchr)
                                 +(SDRAM_refresh_time<<bo_refresh_refcnt);
    memory_ctrl->BANKSIZE = (0<<bo_banksize_bursten)
                                  +(1<<bo_banksize_sckeen)
                                  +(1<<bo_banksize_sclken)
                                  +(1<<bo_banksize_size);
    memory_ctrl->MRSRB6 = 3<<bo_mrsrb_cl;
    memory_ctrl->MRSRB7 = 3<<bo_mrsrb_cl;
}

