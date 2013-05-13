//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//版权所有 (C), 2005-2009,   lst.
//所属模块:  CPU初始化
//作者：     lst
//版本：      V1.0.0
//初始版本完成日期：2009-08-05
//文件描述:  cpu时钟初始化
//其他说明:  无
//修订历史:
//2. ...
//1. 日期: 2009-08-05
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "sysctl.h"

// ---- 初始化MMU ----
// 映射控制，将SDRAM的向量表映射到0x00000000的地址上去（注意映射的主语与宾语）
// S3C2440的指令和数据的Cache是分开的。
// 在进行映射控制时，还需要控制C及B位。
void init_mmu(void)
{
	register vu32 i;

    // 0~0x2FFFFFFF段的初始化，这里很有可能放置的是ROM，开启Cache、Buffer
    for(i=0x00000000; i<0x30000000; ) {
    	mmu_map_section(i, i, 0xc1e);	// 平板式的映射
    	i += 0x100000;
    }

    // 0x30000000~0x3FFFFFFF段映射
    mmu_map_section(0x30000000, 0x30000000, 0xc12);	//非cache区
    // 其中0x30000000~0x33FFFFFF的63MB是SDRAM，开启Cache、Buffer
    for(i=0x30100000; i<0x40000000; ) {
    	mmu_map_section(i, i, 0xc1e);	// 平板式的映射
    	i += 0x100000;
    }

    // 0x40000000~0x5FFFFFFF段映射
    // 这里是外设寄存器段的地址，由于操作IO口，因而这里不能使能Cache，但可以开启Buffer
    for(i=0x40000000; i<0x60000000; ) {
    	mmu_map_section(i, i, 0xc12);	// 平板式的映射
    	i += 0x100000;
    }

//    // 其余地址都不可用
//    for(i=0x60000000; i != 0; i += 0x100000) {
//    	mmu_map(i, i, 0x016);	// 平板式的映射
//    }

#ifdef debug
    mmu_map_section(0x00000000, 0x30100000, 0xc1e);
#endif
    mmu_set_sections_base(mmu_page_table);
    mmu_set_domain_access(0xffffffff);          //所有域具有管理者权限
    cache_enable_all();
    cache_enable_write_buf();
    mmu_enable();
}
//此时内存未初始化好，不能使用局部变量
void init_cpu_clk(void)
{
	register struct clk_ctrl_reg volatile * clk_ctrl
	                            = (struct clk_ctrl_reg*)0x4c000000;
	clk_ctrl->CAMDIVN = 0;
	clk_ctrl->CLKDIVN = (1<<bo_clkdivn_udivn)    //uclk = upll输出/2
                              +(2<<bo_clkdivn_hdivn)    //hclk = fclk/4=100M
                              +(1<<bo_clkdivn_pdivn);   //pclk = hckk/2=50M
    
    __asm__       //总线尚未初始化，不用用for循环，因为变量要使用内存
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
    //dmg将上面的时钟分频进行修改，原因是上面没有使用官方推荐的表进行设置
    //以下时钟寄存器设置可得到405MHz的主频输出（原来设置为399.65Mhz）
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
    xr0 &= 0x00fffff0;       //清掉bank0,bank6/7
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

