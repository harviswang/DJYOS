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
// �ļ���     ��sysinit.c
// ģ������: ϵͳ��ʼ�����֣���Ҫ���ں�������صĳ�ʼ������ʱ�ӣ����ߵ�
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 06/06.2014
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "board-config.h"

#define CLK0_FREQ_HZ        50000000    //��ʱ�Ӿ���
#define PLL0_PRDIV          5           //PLL��Ƶ
#define PLL0_VDIV           24          //PLL��Ƶ

// =============================================================================
// ���ܣ�PLL��ʼ�����֣���Ҫ��PLL�;����ʼ������ʱ����ڲ�ʱ���л����ⲿʱ�ӵ���Ҫ��
//       �֣��˴�ʹ��PLL0��OSC0��Ϊ�ڲ�ʱ��Դ������Ƶ�ͱ�Ƶ��ʹ�ں�ʱ�Ӵﵽ150M
// ������crystal_fre_hz,�ⲿ����Ƶ��
//       prdiv,PLL��Ƶ����
//       vdiv,PLL��Ƶ����
// ���أ�ʵ�����õ�MCG���Ƶ��
// =============================================================================
u32 PLL_Init(u32 crystal_fre_hz, u8 prdiv, u8 vdiv)
{
    //reset��ϵͳ����FEIģʽ��ʱ�����ù���Ϊ��
    //FEI--->FBE--->PBE--->PEE(�ֲ�640ҳtable25-22)

    //Ĭ��ʹ������0����������0
    // ���ÿ��ƼĴ���MCG_C2
    // ����bitλ�����ø��پ���high-gain operation���ⲿ����
    MCG->C2 &= ~(MCG_C2_RANGE0_MASK | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK);
    MCG->C2 |= (MCG_C2_RANGE0(1)
                | (0 << MCG_C2_HGO0_SHIFT)
                | (0 << MCG_C2_EREFS0_SHIFT));

    // ���ÿ��ƼĴ���MCG_C1
    //����bitλ��CLKΪ�ⲿʱ�ӣ�FRDIVΪ5ʱ�����÷�ƵΪ1024��������FLL����
    //31.25-39.0625 kHz ֮��
    MCG->C1 &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK);
    MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(5);

    while((MCG->S & MCG_S_IREFST_MASK));    //�ȴ�FLLʱ��תΪ�ⲿԴ
    while(((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2);

    // Now in FBE
    MCG->C6 |= MCG_C6_CME0_MASK;
    // start Configure PLL0
    MCG->C5 &= ~MCG_C5_PLLREFSEL0_MASK;         // ensure OSC0
    MCG->C11 &= ~MCG_C11_PLLCS_MASK;            // select PLL0

    // Configure MCG_C5
    MCG->C5 &= ~MCG_C5_PRDIV0_MASK;             // clear settings
    MCG->C5 |= MCG_C5_PRDIV0(prdiv - 1);            //set PLL ref divider

    // Configure MCG_C6
    MCG->C6 &= ~MCG_C6_VDIV0_MASK;                          // clear settings
    MCG->C6 |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(vdiv - 16); // write new VDIV

    while(!(MCG->S & MCG_S_PLLST_MASK));        // wait for PLLST status bit to set
    while(!(MCG->S & MCG_S_LOCK0_MASK));        // Wait for LOCK bit to set

    // Use actual PLL settings to calculate PLL frequency
    prdiv = ((MCG->C5 & MCG_C5_PRDIV0_MASK) + 1);
    vdiv = ((MCG->C6 & MCG_C6_VDIV0_MASK) + 16);

    // now in PBE
    MCG->C1 &= ~MCG_C1_CLKS_MASK; // ��CLKS��ѡ��MCG_OUTԴΪPLL
    while(((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3);

    // start Configure PLL1 if needed, for DDR,so, CLK = 100MHz
#if (CN_CFG_DDR_USED == 1)
    // Configure MCG_C12
    MCG->C11 &= ~MCG_C11_PLLREFSEL1_MASK;       // Clear select bit to choose OSC0
    MCG->C11 &= ~MCG_C11_PRDIV1_MASK;
    MCG->C11 |= MCG_C11_PRDIV1(5 - 1);
    MCG->C12 &= ~MCG_C12_VDIV1_MASK;            // clear VDIV settings
    MCG->C12 |=  MCG_C12_VDIV1(24 - 16); // write new VDIV and enable PLL
    // Now enable the PLL
    MCG->C11 |= MCG_C11_PLLCLKEN1_MASK; // Set PLLCLKEN2 to enable PLL1

    while(!(MCG->S2 & MCG_S2_LOCK1_MASK));  // Wait for PLL1 locked
#endif

    //MCGOUT equals PLL output frequency/2
    return (((crystal_fre_hz / prdiv) * vdiv) / 2);
}

// =============================================================================
// ���ܣ�ϵͳʱ�ӳ�ʼ������ϵͳ�ϵ�ʹ��FEI���л���ʹ���ⲿ��50Mʱ�Ӻ�Ϊʱ��Դ��������
//       PLL��Ƶ�ͱ�Ƶ��ʹ�ں�ʱ��Ϊ120M��busʱ��Ϊ2��Ƶ��FlexBus3��Ƶ��Flash6��Ƶ��
//       ����ģ�飨��portA~E����ʱ�Ӹ���ʹ��ʱ���ã��˴�ֻ����ϵͳʱ��
// ��������
// ���أ���
// =============================================================================
void SysClockInit(void)
{
    //��ʼ���˿�ʱ�ӣ���ϵͳ��ʼ��ʱȫ����
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK
                  | SIM_SCGC5_PORTF_MASK );

    //��ʼ��PLLǰ���ȳ�ʼ��ϵͳʱ�ӷֲ������ֱ���������
    //core/systemʱ�ӡ�busʱ�ӡ�FlexBusʱ�ӡ�Flashʱ��
    SIM->CLKDIV1 = ( 0
                    | SIM_CLKDIV1_OUTDIV1(0)
                    | SIM_CLKDIV1_OUTDIV2(1)
                    | SIM_CLKDIV1_OUTDIV3(2)
                    | SIM_CLKDIV1_OUTDIV4(5) );

    //ϵͳ��PLLʱ�ӷ�Ƶ���ã���������MCGOUTCLK = 120M
    //����50M��crystal����ƵΪ5��PLL��ƵΪ24������ʱ�Ӽ��㹫ʽΪ��
    //MCGOUTCLK = ((50M/5) * 24)/2 = 120M
    PLL_Init(CLK0_FREQ_HZ,PLL0_PRDIV,PLL0_VDIV);
}

// =============================================================================
// ���ܣ�DDR2��ʼ�����ײ�Ӳ���ĳ�ʼ�����뱣֤��ȷ������һֱ�ȴ�
// ��������
// ���أ���
// =============================================================================
void SDRAM_Init(void)
{
#if (CN_CFG_DDR_USED == 1)
    /* Enable DDR controller clock */
    SIM->SCGC3 |= SIM_SCGC3_DDR_MASK;

    /* Enable DDR pads and set slew rate */
    SIM->MCR |= 0xC4;   // bits were left out of the manual so there isn't a macro right now

    DDR->RCR |= DDR_RCR_RST_MASK;

    * (vu32 *)(0x400Ae1ac) = 0x01030203;

    DDR->CR00 = 0x00000400;    // DDRCLS = 4 is reserved??
    DDR->CR02 = 0x02000031;
    DDR->CR03 = 0x02020506;
    DDR->CR04 = 0x06090202;
    DDR->CR05 = 0x02020302;
    DDR->CR06 = 0x02904002;
    DDR->CR07 = 0x01000303;
    DDR->CR08 = 0x05030201;
    DDR->CR09 = 0x020000c8;
    DDR->CR10 = 0x03003207;
    DDR->CR11 = 0x01000000;
    DDR->CR12 = 0x04920031;
    DDR->CR13 = 0x00000005;
    DDR->CR14 = 0x00C80002;
    DDR->CR15 = 0x00000032; //  | DDR->CR15_SREF_MASK ;
    DDR->CR16 = 0x00000001;
    DDR->CR20 = 0x00030300;
    DDR->CR21 = 0x00040232;
    DDR->CR22 = 0x00000000;
    DDR->CR23 = 0x00040302;
    DDR->CR25 = 0x0A010201;
    DDR->CR26 = 0x0101FFFF;
    DDR->CR27 = 0x01010101;
    DDR->CR28 = 0x00000003;
    DDR->CR29 = 0x00000000;
    DDR->CR30 = 0x00000001;
    DDR->CR34 = 0x02020101;
    DDR->CR36 = 0x01010201;
    DDR->CR37 = 0x00000200;
    DDR->CR38 = 0x00200000;
    DDR->CR39 = 0x01010020;
    DDR->CR40 = 0x00002000;
    DDR->CR41 = 0x01010020;
    DDR->CR42 = 0x00002000;
    DDR->CR43 = 0x01010020;
    DDR->CR44 = 0x00000000;
    DDR->CR45 = 0x03030303;
    DDR->CR46 = 0x02006401;
    DDR->CR47 = 0x01020202;
    DDR->CR48 = 0x01010064;
    DDR->CR49 = 0x00020101;
    DDR->CR50 = 0x00000064;
    DDR->CR52 = 0x02000602;
    DDR->CR53 = 0x03c80000;
    DDR->CR54 = 0x03c803c8;
    DDR->CR55 = 0x03c803c8;
    DDR->CR56 = 0x020303c8;
    DDR->CR57 = 0x01010002;

    asm("NOP");

    DDR->CR00 |= 0x00000001;

    while ((DDR->CR30 & 0x400) != 0x400);

    MCM->CR |= MCM_CR_DDRSIZE(1);
#endif
}

// =============================================================================
// ���ܣ�CACHE��ʼ������ʹ��buffer ��cache��K70��cache��Ϊ16��region���ֱ�����˲�
//       �ĵ�ַ��Χ�����ֲ�K70 Sub-Family Reference Manual page 126.�����������
//       ����ΪĬ�ϵ�cache���ԣ�Write-through��Write-back��non-cacheable��
// ��������
// ���أ���
// =============================================================================
void Cache_Init(void)
{
#if (CN_CFG_DDR_USED == 1)
    // Code Cache Init
    /* Cache Set Command: set command bits in CCR */
    /* set invalidate way 1 and invalidate way 0 bits */
    LMEM->PCCCR = LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK;
    /* set ccr[go] bit to initiate command to invalidate cache */
    LMEM->PCCCR |= LMEM_PCCCR_GO_MASK;

//    LMEM->PCCRMR |= LMEM_PCCRMR_R0(0x03) | LMEM_PCCRMR_R8(0x03);
    /* wait until the ccr[go] bit clears to indicate command complete */
    while(((LMEM->PCCCR & LMEM_PCCCR_GO_MASK)>>LMEM_PCCCR_GO_SHIFT)== 0x1)
    {};
    /* enable write buffer */
    LMEM->PCCCR |= LMEM_PCCCR_ENWRBUF_MASK;
    /* enable cache */
    LMEM->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;

    // System Cache Init
    /* Cache Set Command: set command bits in CCR */
    /* set invalidate way 1 and invalidate way 0 bits */
    LMEM->PSCCR = LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK;
    /* set ccr[go] bit to initiate command to invalidate cache */
    LMEM->PSCCR |= LMEM_PSCCR_GO_MASK;

//    LMEM->PSCRMR |= LMEM_PSCRMR_R0(0x03) | LMEM_PSCRMR_R8(0x03);
    /* wait until the ccr[go] bit clears to indicate command complete */
    while(((LMEM->PSCCR & LMEM_PSCCR_GO_MASK)>>LMEM_PSCCR_GO_SHIFT)== 0x1)
    {};
    /* enable write buffer */
    LMEM->PSCCR |= LMEM_PSCCR_ENWRBUF_MASK;
    /* enable cache */
    LMEM->PSCCR |= LMEM_PSCCR_ENCACHE_MASK;

#endif
}


