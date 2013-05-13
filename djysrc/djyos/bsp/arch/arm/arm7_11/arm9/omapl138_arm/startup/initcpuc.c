//========================================================  
// 文件模块说明:                                            
// 这是从omapl138_cpucfg.c和omapl138_initcpu.c中copy 过来的代码，改了函数名，为解
// 决启动代码必须在flash中运行的问题。                          
// 文件版本: v1.00                                          
// 开发人员: lst                                            
// 创建时间: 2011.06.01                                     
// Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             
//========================================================  
// 程序修改记录(最新的放在最前面):							
//  <版本号> <修改日期>, <修改人员>: <修改功能概述>			
//========================================================  
//例如：
// V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核

#include "os_inc.h"
#include "cpu_peri.h"
#define SETBIT(dest,mask)     (dest |= mask)
#define CLRBIT(dest,mask)     (dest &= ~mask)
#define CHKBIT(dest,mask)     (dest & mask)
void config_pll1(u32 pllm, u32 postdiv, u32 plldiv1, 
                    u32 plldiv2, u32 plldiv3);
void config_pll0(u32 clkmode, u32 prediv, u32 pllm, u32 postdiv, 
                    u32 plldiv1, u32 plldiv3, u32 plldiv7);

// ---- 初始化MMU -------------------------------------------------------------
//功能: 初始化mmu映射控制
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void init_mmu(void)
{
	register vu32 i;

    // 0~0xbFFFFFFF段的，禁止Cache、开启Buffer
    for(i=0; i<0xc00; i ++) 
    {
    	mmu_map_section(i<<20, i<<20, 0xc16);   // 平板式的映射
    	
    }

    // 0xc0000000~0xc7FFFFFF段的，128M ddr2内存，允许Cache、开启Buffer
    for(i=0xc00; i<0xc80; i ++)
    {
    	mmu_map_section(i<<20, i<<20, 0xc1e);   // 平板式的映射
    	
    }
    // 0xc0000000~0xc7FFFFFF段的，128M ddr2内存，允许Cache、开启Buffer
    for(i=0xc00; i<0xc1f; i ++)
    {
    	mmu_map_section(i<<20, i<<20, 0xc1e);   // 平板式的映射
    	
    }
    
    mmu_map_section((u32)0xc1f<<20, (u32)0xc1f<<20, 0xc16);   // 平板式的映射

    // 0xc8000000~0xFFFFFFFF段的，禁止Cache、开启Buffer
    for(i=0xc20; i<0x1000; i ++) 
    {
    	mmu_map_section(i<<20, i<<20, 0xc16);   // 平板式的映射
    	
    }
    mmu_set_sections_base(mmu_page_table);
    mmu_set_domain_access(0xffffffff);          //所有域具有管理者权限
    cache_enable_all();
    cache_enable_write_buf();
    mmu_enable();
}

// =========================================================================
// 函数功能: 使能外设,
// 函数参数: module，待操作的外设编号，在omapl138_peri_cpucfg.h文件中定义
// 返回值  : 无
// =========================================================================
void flash_EnableLPSC(u8 module)
{
    u32 domain;
    tagLpscReg volatile * Lpsc0Reg = (tagLpscReg *)0x01C10000;
    tagLpscReg volatile * Lpsc1Reg = (tagLpscReg *)0x01E27000;
    tagSysconfig0Reg volatile *Sysconfig0Reg = (tagSysconfig0Reg *)0x01c14000;
    Sysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(module == cn_PSC0_DSP)
        domain = cn_RAM_Pseudo;
    else
        domain = cn_Always_ON;

    if(module <cn_psc1_base)
    {
        // spin until existing transitions are done.
        while (CHKBIT(Lpsc0Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(Lpsc0Reg->MDSTAT[module], MASK_STATE) == PSC_ENABLE)
        {
            goto exit_completed;
        }

        // setup the transition...clear the bits before setting the next state.
        CLRBIT(Lpsc0Reg->MDCTL[module], NEXT);
        SETBIT(Lpsc0Reg->MDCTL[module], PSC_ENABLE);

        // kick off the transition.
        Lpsc0Reg->PTCMD = domain;

        // spin until transition is done.
        while (CHKBIT(Lpsc0Reg->PTSTAT, domain)) {}

        while (CHKBIT(Lpsc0Reg->MDSTAT[module], MASK_STATE) != PSC_ENABLE) {}
    }else
    {
        module -= cn_psc1_base;
        // spin until existing transitions are done.
        while (CHKBIT(Lpsc1Reg->PTSTAT, domain)) {}
        
        // if we are already in the requested state...just return.
        if (CHKBIT(Lpsc1Reg->MDSTAT[module], MASK_STATE) == PSC_ENABLE)
        {
            goto exit_completed;
        }
        
        // setup the transition...clear the bits before setting the next state.
        CLRBIT(Lpsc1Reg->MDCTL[module], NEXT);
        SETBIT(Lpsc1Reg->MDCTL[module], PSC_ENABLE);
        
        // kick off the transition.
        Lpsc1Reg->PTCMD = domain;
        
        // spin until transition is done.
        while (CHKBIT(Lpsc1Reg->PTSTAT, domain)) {}
        
        while (CHKBIT(Lpsc1Reg->MDSTAT[module], MASK_STATE) !=PSC_ENABLE) {}
    }

exit_completed:
    Sysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}

// =========================================================================
// 函数功能: 模块同步复位
// 函数参数: module，待操作的外设编号，在omapl138_peri_cpucfg.h文件中定义
// 返回值  : 无
// =========================================================================
void flash_SyncResetLPSC(u8 module)
{
    u32 domain;
    tagLpscReg volatile * const Lpsc0Reg = (tagLpscReg *)0x01C10000;
    tagLpscReg volatile * const Lpsc1Reg = (tagLpscReg *)0x01E27000;
    tagSysconfig0Reg volatile *Sysconfig0Reg = (tagSysconfig0Reg *)0x01c14000;
    Sysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(module == cn_PSC0_DSP)
        domain = cn_RAM_Pseudo;
    else
        domain = cn_Always_ON;



    if(module <cn_psc1_base)
    {
        if( (Lpsc0Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET ) 
        {
          Lpsc0Reg->MDCTL[module] &= ~(u32)MASK_STATE;
          Lpsc0Reg->MDCTL[module] |= PSC_SYNCRESET;
          Lpsc0Reg->PTCMD = domain;
        
          /*Wait for power state transition to finish*/
          while (CHKBIT(Lpsc0Reg->PTSTAT, domain)) {}
        
          while ((Lpsc0Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET);
        }
    }else
    {
        module -= cn_psc1_base;
        if( (Lpsc1Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET ) 
        {
          Lpsc1Reg->MDCTL[module] &= ~(u32)MASK_STATE;
          Lpsc1Reg->MDCTL[module] |= PSC_SYNCRESET;
          Lpsc1Reg->PTCMD = domain;
        
          /*Wait for power state transition to finish*/
          while (CHKBIT(Lpsc1Reg->PTSTAT, domain)) {}
        
          while ((Lpsc1Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET);
        }
    }

    Sysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}

//-----------------------------------------------------------------------------
// helper function to initialize cpu, system, and pheripheral clocks.
// configure arm and dsp to 300 MHz and emif to 133MHz.
//-----------------------------------------------------------------------------
void config_clocks(void)
{
    config_pll0(1,0,15,0,DIV_ENABLE|0,DIV_ENABLE|2,DIV_ENABLE|7);
//    config_pll0(0,0,23,0,DIV_ENABLE|1,DIV_ENABLE|5,DIV_ENABLE|11);
    //pll0的各时钟域的定义:
    //pll0输出时钟=400mhz
    //主时钟=400mhz
    //sysclk1=400mhz,sysclk2=sysclk1/2,sysclk4=sysclk1/4,sysclk6=sysclk1
    //sysclk3=100mhz,emifa时钟，不知外设能否支持得了。
    //sysclk7=50mhz
    config_pll1(23,1,DIV_ENABLE|0,DIV_ENABLE|3,31);
    //pll1输出时钟=600mhz
    //pll1_sysclk1=200mhz
    //pll1_sysckk2=200mhz
    //pll1_sysclk3=disable
    // enable 4.5 divider PLL and set it as the EMIFA clock source.
//    SETBIT(SYSCONFIG->CFGCHIP[3], DIV4P5ENA | EMA_CLKSRC);
   
}

void config_ddr(void)
{
    tagDdriiReg volatile * DdriiReg = (tagDdriiReg*)0xb0000000;
    tagSysconfig0Reg volatile *Sysconfig0Reg = (tagSysconfig0Reg *)0x01c14000;
    tagSysconfig1Reg volatile *Sysconfig1Reg = (tagSysconfig1Reg *)0x01e2c000;
    flash_EnableLPSC(cn_PSC1_DDR2_MDDR);
    // unlock the system config registers.
    Sysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(Sysconfig1Reg->VTPIO_CTL & 0x00000040)
    {
        // Set IOPWRDN bit to enable input buffer powerdown enable mode
        Sysconfig1Reg->VTPIO_CTL |= (0x00004000);
        // Clear POWERDN bit (enable VTP)
        Sysconfig1Reg->VTPIO_CTL &= ~(0x00000040);
        
        // Pulse (low) CLRZ to initiate VTP IO Calibration
        // Set CLRZ bit
        Sysconfig1Reg->VTPIO_CTL |= (0x00002000);
        // Clear CLRZ bit (CLRZ should be low for at least 2ns)
        Sysconfig1Reg->VTPIO_CTL &= ~(0x00002000);
        Sysconfig1Reg->VTPIO_CTL |= 0x00002000;       // Set CLRZ bit
        
        // Polling READY bit to see when VTP calibration is done
        while(!((Sysconfig1Reg->VTPIO_CTL & 0x00008000)>>15)) {}
        
        // Set LOCK bit for static mode
        Sysconfig1Reg->VTPIO_CTL |= 0x00000080;
        // Set PWRSAVE bit to save power
        Sysconfig1Reg->VTPIO_CTL |= 0x00000100;
    }

    DdriiReg->SDCR =(0x1               << 23)  |  // BOOTUNLOCK
                  (0x0               << 15);    // TIMUNLOCK

    // Setting based on 512Mb DDRII MT47H32M16LFBF-6 on EVM
    // Config DDR timings
    //经反复验证，CL=5，RL=6是最稳定的。
    DdriiReg->DRPYC1R      = (0x0               << 8)   |  // Reserved
                  (0x1               << 7)   |  // EXT_STRBEN
                  (0x1               << 6)   |  // PWRDNEN
                  (0x0               << 3)   |  // Reserved
                  (0x6               << 0);     // RL  CAS latency is 5, so RL=CAS+1=6!
    DdriiReg->SDCR = (DdriiReg->SDCR & 0xF0000000) |  // Reserved
                  (0x0               << 27)  |  // DDR2TERM1
                  (0x0               << 26)  |  // IBANK_POS
                  (0x0               << 25)  |  // MSDRAMEN, Mobile DDR disable;
                  (0x0               << 24)  |  // DDRDRIVE1
                  (0x0               << 23)  |  // BOOTUNLOCK
                  (0x0               << 22)  |  // DDR2DDQS, single end DQS;
                  (0x0               << 21)  |  // DDR2TERM0
                  (0x1               << 20)  |  // DDR2EN  Tang;
                  (0x0               << 19)  |  // DDRDLL_DIS
                  (0x1               << 18)  |  // DDRDRIVE0
                  (0x1               << 17)  |  // DDREN
                  (0x1               << 16)  |  // SDRAMEN
                  (0x1               << 15)  |  // TIMUNLOCK
                  (0x1               << 14)  |  // NM
                  (0x0               << 12)  |  // Reserved
                  (0x5               << 9)   |  // CL  CAS latency = 5.
                  (0x0               << 7)   |  // Reserved
                  (0x3               << 4)   |  // IBANK, 2=4bank,3=8bank
                  (0x0               << 3)   |  // Reserved
                  (0x2               << 0);     // PAGESIZE, 1024byte page size.

#define cn_trfc_128M        127.5
#define cn_trfc_64M         105
#define cn_trp_128M         15
#define cn_trp_64M          12.5
#define cn_trcd_128M        15
#define cn_trcd_64M         12.5
#define cn_twr              15
#define cn_tras             45
#define cn_trc_128M         60
#define cn_trc_64M          57.5
#define cn_trrd             10
#define cn_twtr             7.5

#if 0
    DdriiReg->SDTIMR1 =   (((u32) ((cn_trfc_128M * cn_ddrii_freq / 1000) )) << 25) |
                     (((u32) ((cn_trp_128M * cn_ddrii_freq / 1000) )) << 22) |
                     (((u32) ((cn_trcd_128M * cn_ddrii_freq / 1000) )) << 19) |
                     (((u32) ((cn_twr * cn_ddrii_freq / 1000) )) << 16) |
                     (((u32) ((cn_tras * cn_ddrii_freq / 1000) )) << 11) |
                     (((u32) ((cn_trc_128M * cn_ddrii_freq / 1000) )) << 6)  |
                     (((u32) ((cn_trrd  * cn_ddrii_freq / 1000))) << 3)  |  // tRRD = 10ns // Increase this if memory is still unstable - tlee
//                                   (2<<3) |
                     (DdriiReg->SDTIMR1 & 0x4)                                |  // Reserved
//                     (((u32) ((cn_twtr * cn_ddrii_freq / 1000) ))  << 0);    // tWTR=7.5ns       
                     1; // Max out tWTR - tlee (decrease for better performance until no longer stable)

    // Subtracting 0.5 instead of 1 so that the int is rounded up after truncating a real value
    // tRASMAX is rounded down so subtracting 1
    // CAS/CL = 5

#define cn_trtp             7.5
#define cn_txsrd            200
#define cn_tcke             3
//Modified many by Tony here;    
     DdriiReg->SDTIMR2 = (DdriiReg->SDTIMR2 & 0x80000000) |  // Reserved
                     (((u32) ((70000 / 7800.0) )) << 27)  |   // tRASMAX=70000ns
                     (0x2 << 25) | // tXP=2, due to Txp(2)<Tcke(3), so T_XP=Tcke-1=2
                     (0x0 << 23)  |  // tODT (Not supported)
                     (((u32) (((cn_trfc_128M+10) * cn_ddrii_freq / 1000) )) << 16) |  // tXSNR=Trfc+10=138.5ns
                     ((cn_txsrd - 1)  << 8)   |  // tXSRD (200 Cycles)
                     (((u32) ((cn_trtp * cn_ddrii_freq / 1000)))  << 5)   |  // tRTP (1 Cycle); 7.5ns
                     ((cn_tcke-1) << 0);     // tCKE
#endif
    DdriiReg->SDTIMR1 = 0x1e492189;
    DdriiReg->SDTIMR2 = 0x3c11c702;
    DdriiReg->SDCR    &= ~0x00008000; // Clear TIMUNLOCK, Active modification!

    DdriiReg->SDCR2   = 0x00000000; // IBANK_POS set to 0 so this register does not apply,only apply to DDRII(Tony)
    DdriiReg->SDRCR   = (0x1 << 31)  |  
                     (0x1 << 30)  |  
                     (0x0 << 24)  |  // Reserved
                     (0x0 << 23)  |  // SR_PD
                     (0x0 << 16)  |  // Reserved
                     (((u32) ((7.8 * cn_ddrii_freq))) << 0);     // RR, 7.8us.
    DdriiReg->PBBPR = 0x20;
    /*SyncReset the Clock to EMIF3A SDRAM*/
    flash_SyncResetLPSC(cn_PSC1_DDR2_MDDR);

    /*Enable the Clock to EMIF3A SDRAM*/

    DdriiReg->SDRCR &= ~0xC0000000;  // disable self-refresh
    Sysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_LOCK;
    flash_EnableLPSC(cn_PSC1_DDR2_MDDR);

}

//----pll0配置-----------------------------------------------------------------
//功能: pll0及相关时钟域配置。各时钟的上限频率较复杂，跟电源电压有关，
//      请参考datasheet
//      pll0时钟:晶振(20M)*(pllm+1)/(prediv+1)/(postdiv+1)
//      sysclk1: pll0时钟/(plldiv1+1),是dsp时钟
//      sysclk3: pll0时钟/(plldiv3+1)，是emifa的时钟
//      sysclk7: pll0时钟/(plldiv7+1)，EMAC RMII,应设为50M
//      sysclk2: =sysclk1/2,用途较多，且有很多可选择，参阅omapl138的reference
//      sysclk4: =sysclk1/4，用途较多，参阅omapl138的refe rence
//      sysclk6: =sysclk1,ARM的时钟
//参数: clkmode，0=晶体，1=晶振
//      prediv,pll参数，范围0~31
//      pllm,pll参数，范围0~63,手册有误
//      postdiv，pll参数，范围0~31
//      plldiv1，plldiv3、plldiv7:各时钟域的除数，范围均是0~31
//-----------------------------------------------------------------------------
void config_pll0(u32 clkmode, u32 prediv, u32 pllm, u32 postdiv, 
                 u32 plldiv1, u32 plldiv3, u32 plldiv7)
{
    vu32 i;
    tagSysconfig0Reg volatile *Sysconfig0Reg = (tagSysconfig0Reg *)0x01c14000;
    tagPllReg volatile * Pll0Reg = (tagPllReg*)0x01C11000;
    
    // unlock the system config registers.
    Sysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    
    // unlock pll regs.
    CLRBIT(Sysconfig0Reg->CFGCHIP[0], PLL0_MASTER_LOCK);
    
    // prepare to enable pll (PLLENSRC must be clear for PLLEN to have effect).
    CLRBIT(Pll0Reg->PLLCTL, PLLENSRC);
    
    // disable external clock source.
    CLRBIT(Pll0Reg->PLLCTL, EXTCLKSRC);
    
    // switch to bypass mode...wait 4 cycles to ensure it switches properly.
    CLRBIT(Pll0Reg->PLLCTL, PLLEN);
    for (i = 0; i < 4; i++) {}       //必须改为vu32，否则可能被优化掉
    
    // select clock mode (on-chip oscillator or external).
    CLRBIT(Pll0Reg->PLLCTL, CLKMODE);
    SETBIT(Pll0Reg->PLLCTL, (clkmode << CLKMODE_SHIFT));
   
    // reset the pll.
    CLRBIT(Pll0Reg->PLLCTL, PLLRST);
    
    // disable the pll...set disable bit.
    //lst注:这个位在datasheet中的说明是:Reserved. Write the default value when 
    //modifying this register.默认值是1
    SETBIT(Pll0Reg->PLLCTL, PLLDIS);
    
    // PLL initialization sequence
    //----------------------------
    // power up the pll...clear power down bit.
    CLRBIT(Pll0Reg->PLLCTL, PLLPWRDN);   
    
    // enable the pll...clear disable bit.
    //lst注:这个位在datasheet中的说明是:Reserved. Write the default value when 
    //modifying this register.默认值是1
    CLRBIT(Pll0Reg->PLLCTL, PLLDIS);     
    
    //PLL stabilisation time- take out this step ,
    //not required here when PLL in bypassmode
//    for(i = 0; i < PLL_STABILIZATION_TIME; i++) {;}
    
    // program the required multiplier value.
    Pll0Reg->PLLM = pllm;
    
    // program postdiv ratio.
    Pll0Reg->POSTDIV = DIV_ENABLE | postdiv;
    
    // spin until all transitions are complete.
    while (CHKBIT(Pll0Reg->PLLSTAT, GOSTAT)) {}
    
    // program the divisors.
//    pg_pll0_reg->PREDIV =  DIV_ENABLE | prediv;
    Pll0Reg->PLLDIV1 = DIV_ENABLE | plldiv1;
    Pll0Reg->PLLDIV2 = DIV_ENABLE | (((plldiv1 + 1) * 2) - 1);
    Pll0Reg->PLLDIV3 = DIV_ENABLE | plldiv3;
    Pll0Reg->PLLDIV4 = DIV_ENABLE | (((plldiv1 + 1) * 4) - 1);
    Pll0Reg->PLLDIV6 = DIV_ENABLE | plldiv1;
    Pll0Reg->PLLDIV7 = DIV_ENABLE | plldiv7;
    
    // kick off the transitions and spin until they are complete.
    SETBIT(Pll0Reg->PLLCMD, GOSET);
    while (CHKBIT(Pll0Reg->PLLSTAT, GOSTAT)) {}
    
    //Wait for PLL to reset properly. See PLL spec for PLL reset time
    //This step is not required here -step11
    for(i = 0; i < PLL_RESET_TIME_CNT; i++) {;}   /*128 MXI Cycles*/
    
    // bring pll out of reset and wait for pll to lock.
    SETBIT(Pll0Reg->PLLCTL, PLLRST);
    for (i = 0; i < 100; i++) {}
    
    // exit bypass mode.
    SETBIT(Pll0Reg->PLLCTL, PLLEN);
    
    // lock pll regs.
    SETBIT(Sysconfig0Reg->CFGCHIP[0], PLL0_MASTER_LOCK);
   
}

//----pll1配置-----------------------------------------------------------------
//功能: pll1及相关时钟域配置。各时钟的上限频率较复杂，跟电源电压有关，
//      请参考datasheet
//      pll1时钟:晶振(20M)*(pllm+1)/(postdiv+1)
//      sysclk1: pll1时钟/(plldiv1+1),DDRII时钟
//      sysclk2: pll1时钟/(plldiv2+1)，用于较多外设，参看手册
//      sysclk3: pll1时钟/(plldiv3+1)，可选为pll0的输入时钟，但djyos没有用
//参数: pllm,pll参数，范围0~63,手册有误
//      postdiv，pll参数，范围0~31
//      plldiv1，plldiv2、plldiv3:各时钟域的除数，范围均是0~31
//-----------------------------------------------------------------------------
void config_pll1(u32 pllm, u32 postdiv, u32 plldiv1, u32 plldiv2, u32 plldiv3)
{
    vu32 i;
    tagSysconfig0Reg volatile *Sysconfig0Reg = (tagSysconfig0Reg *)0x01c14000;
    tagPllReg volatile * Pll1Reg = (tagPllReg*)0x01E1A000;
   
    // unlock the system config registers.
    Sysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
   
    // unlock pll regs.
    CLRBIT(Sysconfig0Reg->CFGCHIP[3], PLL1_MASTER_LOCK);
    
    // prepare to enable pll (PLLENSRC must be clear for PLLEN to have effect).
    CLRBIT(Pll1Reg->PLLCTL, PLLENSRC);
    CLRBIT(Pll1Reg->PLLCTL, EXTCLKSRC);    //pll1无此控制位----lst
   
    // switch to bypass mode...wait 4 cycles to ensure it switches properly.
    CLRBIT(Pll1Reg->PLLCTL, PLLEN);
    for (i = 0; i < 4; i++) {}
    
    // reset the pll.
    CLRBIT(Pll1Reg->PLLCTL, PLLRST);
    
    // disable the pll...set disable bit.
    SETBIT(Pll1Reg->PLLCTL, PLLDIS);
    
    // PLL initialization sequence
    //----------------------------
    // power up the pll...clear power down bit.
    CLRBIT(Pll1Reg->PLLCTL, PLLPWRDN);
    
    // enable the pll...clear disable bit.
    CLRBIT(Pll1Reg->PLLCTL, PLLDIS);
    
    //PLL stabilisation time- take out this step ,
    //not required here when PLL in bypassmode
//    for(i = 0; i < PLL_STABILIZATION_TIME; i++) {;}
    
    // program the required multiplier value.
    Pll1Reg->PLLM = pllm;
    
    // program postdiv ratio.
    Pll1Reg->POSTDIV = DIV_ENABLE | postdiv;
    
    // spin until all transitions are complete.
    while (CHKBIT(Pll1Reg->PLLSTAT, GOSTAT)) {}
    
    // program the divisors.
    Pll1Reg->PLLDIV1 = DIV_ENABLE | plldiv1;
    Pll1Reg->PLLDIV2 = DIV_ENABLE | plldiv2;
    Pll1Reg->PLLDIV3 = DIV_ENABLE | plldiv3;
    
    // kick off the transitions and spin until they are complete.
    SETBIT(Pll1Reg->PLLCMD, GOSET);
    while (CHKBIT(Pll1Reg->PLLSTAT, GOSTAT)) {}
    
    //Wait for PLL to reset properly. See PLL spec for PLL reset time
    //This step is not required here -step11
    for(i = 0; i < PLL_RESET_TIME_CNT; i++) {;}
    
    // bring pll out of reset and wait for pll to lock.
    SETBIT(Pll1Reg->PLLCTL, PLLRST);
    for (i = 0; i < 100; i++) {}
    
    // exit bypass mode.
    SETBIT(Pll1Reg->PLLCTL, PLLEN);
    
    // lock pll regs.
    SETBIT(Sysconfig0Reg->CFGCHIP[3], PLL1_MASTER_LOCK);
}

