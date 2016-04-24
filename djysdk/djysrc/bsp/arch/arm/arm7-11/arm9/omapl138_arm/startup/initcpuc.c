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
//========================================================
// �ļ�ģ��˵��:
// ���Ǵ�omapl138_cpucfg.c��omapl138_initcpu.c��copy �����Ĵ��룬���˺�������Ϊ��
// ���������������flash�����е����⡣
// �ļ��汾: v1.00
// ������Ա: lst
// ����ʱ��: 2011.06.01
// Copyright(c) 2011-2011    ��������������޹�˾
//========================================================
// �����޸ļ�¼(���µķ�����ǰ��):
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
//========================================================
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��

#include "stdint.h"
#include "cache.h"
#include "mmu.h"
#include "cpu_peri.h"
#define SETBIT(dest,mask)     (dest |= mask)
#define CLRBIT(dest,mask)     (dest &= ~mask)
#define CHKBIT(dest,mask)     (dest & mask)
void config_pll1(u32 pllm, u32 postdiv, u32 plldiv1,
                    u32 plldiv2, u32 plldiv3);
void config_pll0(u32 clkmode, u32 prediv, u32 pllm, u32 postdiv,
                    u32 plldiv1, u32 plldiv3, u32 plldiv7);

#define MT47H64M16HR    3

#if(MT47H64M16HR == 3)
#define DDR_SDRCR   0x249
#define DDR_SDTIMR1 0x26923209
//#define DDR_SDTIMR2 0x8414c722
#define DDR_SDTIMR2 0x7c14c722
#else
#define DDR_SDRCR   0x492
#define DDR_SDTIMR1 0x264A3209
#define DDR_SDTIMR2 0x3C14C722
#endif

// ---- ��ʼ��MMU -------------------------------------------------------------
//����: ��ʼ��mmuӳ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void MMU_Init(void)
{
    register vu32 i;

    // 0~0xbFFFFFFF�εģ���ֹCache������Buffer
    for(i=0; i<0xc00; i ++)
    {
        MMU_MapSection(i<<20, i<<20, 0xc16);   // ƽ��ʽ��ӳ��

    }

    // 0xc0000000~0xc7FFFFFF�εģ�128M ddr2�ڴ棬����Cache������Buffer
    for(i=0xc00; i<0xc80; i ++)
    {
        MMU_MapSection(i<<20, i<<20, 0xc1e);   // ƽ��ʽ��ӳ��

    }
    // 0xc0000000~0xc7FFFFFF�εģ�128M ddr2�ڴ棬����Cache������Buffer
    for(i=0xc00; i<0xc1f; i ++)
    {
        MMU_MapSection(i<<20, i<<20, 0xc1e);   // ƽ��ʽ��ӳ��

    }

    MMU_MapSection((u32)0xc1f<<20, (u32)0xc1f<<20, 0xc16);   // ƽ��ʽ��ӳ��

    // 0xc8000000~0xFFFFFFFF�εģ���ֹCache������Buffer
    for(i=0xc20; i<0x1000; i ++)
    {
        MMU_MapSection(i<<20, i<<20, 0xc16);   // ƽ��ʽ��ӳ��

    }
    MMU_SetSectionsBase(mmu_page_table);
    MMU_SetDomainAccess(0xffffffff);          //��������й�����Ȩ��
    Cache_EnableAll();
    Cache_EnableWriteBuf();
    MMU_Enable();
}

// =========================================================================
// ��������: ʹ������,
// ��������: module���������������ţ���omapl138_peri_cpucfg.h�ļ��ж���
// ����ֵ  : ��
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
// ��������: ģ��ͬ����λ
// ��������: module���������������ţ���omapl138_peri_cpucfg.h�ļ��ж���
// ����ֵ  : ��
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
    config_pll0(1,0,15,0,DIV_ENABLE|0,DIV_ENABLE|3,DIV_ENABLE|7);   //@400M
//  config_pll0(1,0,14,0,DIV_ENABLE|0,DIV_ENABLE|3,DIV_ENABLE|7); //@375M
//    config_pll0(0,0,23,0,DIV_ENABLE|1,DIV_ENABLE|5,DIV_ENABLE|11);
    //pll0�ĸ�ʱ����Ķ���:
    //pll0���ʱ��=400mhz
    //��ʱ��=400mhz
    //sysclk1=400mhz,sysclk2=sysclk1/2,sysclk4=sysclk1/4,sysclk6=sysclk1
    //sysclk3=100mhz,emifaʱ�ӣ���֪�����ܷ�֧�ֵ��ˡ�
    //sysclk7=50mhz
    config_pll1(23,1,DIV_ENABLE|0,DIV_ENABLE|3,31); //DDR @150M
    //config_pll1(19,1,DIV_ENABLE|0,DIV_ENABLE|3,31);   //DDR @125M
    //config_pll1(15,1,DIV_ENABLE|0,DIV_ENABLE|3,31);   //DDR @100M
    //config_pll1(11,1,DIV_ENABLE|0,DIV_ENABLE|3,31);   //DDR @75M
    //pll1���ʱ��=600mhz
    //pll1_sysclk1=200mhz
    //pll1_sysckk2=200mhz
    //pll1_sysclk3=disable
    // enable 4.5 divider PLL and set it as the EMIFA clock source.
//    SETBIT(SYSCONFIG->CFGCHIP[3], DIV4P5ENA | EMA_CLKSRC);

}

void config_ddr(void)
{
    unsigned int k;
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
                    (0x1               << 15);    // TIMUNLOCK

    // Setting based on 512Mb DDRII MT47H32M16LFBF-6 on EVM
    // Config DDR timings
    DdriiReg->DRPYC1R      = (0x0               << 8)   |  // Reserved
                  (0x1               << 7)   |  // EXT_STRBEN
                  (0x1               << 6)   |  // PWRDNEN
                  (0x0               << 3)   |  // Reserved
                  (0x4               << 0);     // RL  CAS latency is 5, so RL=CAS+1=6!
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
                  (0x1               << 18)  |  // DDRDRIVE0        //1
                  //(0x0               << 18)  |  // DDRDRIVE0        //1
                  (0x1               << 17)  |  // DDREN
                  (0x1               << 16)  |  // SDRAMEN
                  (0x1               << 15)  |  // TIMUNLOCK
                  (0x1               << 14)  |  // NM
                  (0x0               << 12)  |  // Reserved
               //   (0x5               << 9)   |  // CL  CAS latency = 5. ��������
                  (0x3               << 9)   |  // CL  CAS latency = 3. it's enough for DDR_CLK=150MHz.
                  (0x0               << 7)   |  // Reserved
                  (0x3               << 4)   |  // IBANK, 2=4bank,3=8bank
                  (0x0               << 3)   |  // Reserved
                  (0x2               << 0);     // PAGESIZE, 1024byte page size.

#define cn_trfc_128M        127.5
#define cn_trfc_64M         105.0
#define cn_trp_128M         12.5    //15
#define cn_trp_64M          12.5
#define cn_trcd_128M        12.5    //15
#define cn_trcd_64M         12.5
#define cn_twr              15.0
#define cn_tras             45.0
#define cn_trc_128M         57.5    //60
#define cn_trc_64M          57.5
#define cn_trrd             10.0
#define cn_twtr             7.5

#if 0
    DdriiReg->SDTIMR1 =   (((u32) ((cn_trfc_128M * CN_CFG_DDRII_CLK / 1000-0.5) )) << 25) |
                     (((u32) ((cn_trp_128M * CN_CFG_DDRII_CLK / 1000-0.5) )) << 22) |
                     (((u32) ((cn_trcd_128M * CN_CFG_DDRII_CLK / 1000) )) << 19) |
                     (((u32) ((cn_twr * CN_CFG_DDRII_CLK / 1000-0.5) )) << 16) |
                     (((u32) ((cn_tras * CN_CFG_DDRII_CLK / 1000-0.5) )) << 11) |
                     (((u32) ((cn_trc_128M * CN_CFG_DDRII_CLK / 1000-0.5) )) << 6)  |
                     (((u32) ((cn_trrd  * CN_CFG_DDRII_CLK / 1000-0.5))) << 3)  |  // tRRD = 10ns // Increase this if memory is still unstable - tlee
//                                   (2<<3) |
                     (DdriiReg->SDTIMR1 & 0x4)                                |  // Reserved
//                     (((u32) ((cn_twtr * CN_CFG_DDRII_CLK / 1000) ))  << 0);    // tWTR=7.5ns
                     1; // Max out tWTR - tlee (decrease for better performance until no longer stable)

    // Subtracting 0.5 instead of 1 so that the int is rounded up after truncating a real value
    // tRASMAX is rounded down so subtracting 1
    // CAS/CL = 5

#define cn_tras_max         70000.0
#define cn_trefi            7800.0        //ˢ���ʣ�ns
#define cn_trtp             7.5
#define cn_txsrd            200.0
#define cn_tcke             3
//Modified many by Tony here;
     DdriiReg->SDTIMR2 = (DdriiReg->SDTIMR2 & 0x80000000) |  // Reserved
                     (((u32) ((cn_tras_max / cn_trefi-0.5) )) << 27)  |   // tRASMAX=70000ns
                     ((cn_tcke-1) << 25) | // tXP=2, due to Txp(2)<Tcke(3), so T_XP=Tcke-1=2
                     (0x0 << 23)  |  // tODT (Not supported)
                     (((u32) (((cn_trfc_128M+10) * CN_CFG_DDRII_CLK / 1000-0.5) )) << 16) |  // tXSNR=Trfc+10=138.5ns
                     ((u32)(cn_txsrd - 1)  << 8)   |  // tXSRD (200 Cycles)
                     (((u32) ((cn_trtp * CN_CFG_DDRII_CLK / 1000-0.5)))  << 5)   |  // tRTP (1 Cycle); 7.5ns
                     ((cn_tcke-1) << 0);     // tCKE
#else

    //DdriiReg->SDTIMR1 = 0x2C924209;   //test
    //DdriiReg->SDTIMR2 = 0x8817C724; //test

   //DdriiReg->SDTIMR1 = 0x26922a09;//@150M new
   //DdriiReg->SDTIMR1 = 0x264a3209;//@150M old
   //DdriiReg->SDTIMR2 = 0x3c14c722;//@150M
   // DdriiReg->SDTIMR1 = 0x18492149;//@100M
   // DdriiReg->SDTIMR2 = 0x3C0DC702;//@100M
    //DdriiReg->SDTIMR1 = 0x1e4929c9;//@125M
    //DdriiReg->SDTIMR2 = 0x3c11c702;//@125M
   // DdriiReg->SDTIMR1 = 0x204929c9;//@132M
   // DdriiReg->SDTIMR2 = 0x3c12c702;//@132M
    //DdriiReg->SDTIMR1 = 0x12011909;//@75
    //DdriiReg->SDTIMR2 = 0x3C0AC702;//@75
    //DdriiReg->SDTIMR1 = 0x26923249;//��������
    //DdriiReg->SDTIMR2 = 0x4414c722;//��������

    DdriiReg->SDTIMR1 = DDR_SDTIMR1;    //test
    DdriiReg->SDTIMR2 = DDR_SDTIMR2; //test

#endif
    DdriiReg->SDCR    &= ~0x00008000; // Clear TIMUNLOCK, Active modification!

    DdriiReg->SDCR2   = 0x00000000; // IBANK_POS set to 0 so this register does not apply,only apply to DDRII(Tony)
    DdriiReg->SDRCR   = (0x1 << 31)  |  //LPMODEEN
                        (0x1 << 30)  | //MCLKSTOP_EN
                        (0x0 << 24)  |  // Reserved
                        (0x0 << 23)  |  // SR_PD
                        (0x0 << 16)  |  // Reserved
                        DDR_SDRCR;
                        //0x492;//@150
                      //0x1d4c;//for test 50us refresh
                       //0x2ee; //5us
                        //0x3cf;    //  @125M
                        //0x30c;    //  @100M
                        //0x249;    //  @75M
                        //0X406;//@TEST
                        //0x405;//@132M
                        //(((u32) ((7.8 * CN_CFG_DDRII_CLK))) << 0);     // RR, 7.8us.
    DdriiReg->PBBPR = 0x20;
    /*SyncReset the Clock to EMIF3A SDRAM*/
    flash_SyncResetLPSC(cn_PSC1_DDR2_MDDR);

    /*Enable the Clock to EMIF3A SDRAM*/

    flash_EnableLPSC(cn_PSC1_DDR2_MDDR);
    Sysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_LOCK;

    Sysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    DdriiReg->SDRCR &= ~0xC0000000;  // disable self-refresh
    DdriiReg->PBBPR = 0x20;
    Sysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    Sysconfig0Reg->KICKR[1] = KICK1R_LOCK;

    k = *(volatile unsigned int *)0xc0000000;   //add 2013-1-12
    DdriiReg->DRPYC1R |= 0x00002000;            //add 2013-1-12

}

//----pll0����-----------------------------------------------------------------
//����: pll0�����ʱ�������á���ʱ�ӵ�����Ƶ�ʽϸ��ӣ�����Դ��ѹ�йأ�
//      ��ο�datasheet
//      pll0ʱ��:����(20M)*(pllm+1)/(prediv+1)/(postdiv+1)
//      sysclk1: pll0ʱ��/(plldiv1+1),��dspʱ��
//      sysclk3: pll0ʱ��/(plldiv3+1)����emifa��ʱ��
//      sysclk7: pll0ʱ��/(plldiv7+1)��EMAC RMII,Ӧ��Ϊ50M
//      sysclk2: =sysclk1/2,��;�϶࣬���кܶ��ѡ�񣬲���omapl138��reference
//      sysclk4: =sysclk1/4����;�϶࣬����omapl138��refe rence
//      sysclk6: =sysclk1,ARM��ʱ��
//����: clkmode��0=���壬1=����
//      prediv,pll��������Χ0~31
//      pllm,pll��������Χ0~63,�ֲ�����
//      postdiv��pll��������Χ0~31
//      plldiv1��plldiv3��plldiv7:��ʱ����ĳ�������Χ����0~31
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
    for (i = 0; i < 4; i++) {}       //�����Ϊvu32��������ܱ��Ż���

    // select clock mode (on-chip oscillator or external).
    CLRBIT(Pll0Reg->PLLCTL, CLKMODE);
    SETBIT(Pll0Reg->PLLCTL, (clkmode << CLKMODE_SHIFT));

    // reset the pll.
    CLRBIT(Pll0Reg->PLLCTL, PLLRST);

    // disable the pll...set disable bit.
    //lstע:���λ��datasheet�е�˵����:Reserved. Write the default value when
    //modifying this register.Ĭ��ֵ��1
    SETBIT(Pll0Reg->PLLCTL, PLLDIS);

    // PLL initialization sequence
    //----------------------------
    // power up the pll...clear power down bit.
    CLRBIT(Pll0Reg->PLLCTL, PLLPWRDN);

    // enable the pll...clear disable bit.
    //lstע:���λ��datasheet�е�˵����:Reserved. Write the default value when
    //modifying this register.Ĭ��ֵ��1
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

//----pll1����-----------------------------------------------------------------
//����: pll1�����ʱ�������á���ʱ�ӵ�����Ƶ�ʽϸ��ӣ�����Դ��ѹ�йأ�
//      ��ο�datasheet
//      pll1ʱ��:����(20M)*(pllm+1)/(postdiv+1)
//      sysclk1: pll1ʱ��/(plldiv1+1),DDRIIʱ��
//      sysclk2: pll1ʱ��/(plldiv2+1)�����ڽ϶����裬�ο��ֲ�
//      sysclk3: pll1ʱ��/(plldiv3+1)����ѡΪpll0������ʱ�ӣ���djyosû����
//����: pllm,pll��������Χ0~63,�ֲ�����
//      postdiv��pll��������Χ0~31
//      plldiv1��plldiv2��plldiv3:��ʱ����ĳ�������Χ����0~31
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
    CLRBIT(Pll1Reg->PLLCTL, EXTCLKSRC);    //pll1�޴˿���λ----lst

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

