// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��initcpuc.c
// ģ������: ϵͳ����ģ�飬��Ҫ���ں�������صĳ�ʼ��������ʱ�ӣ����ߵ�
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 11/08.2014
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "exception.h"
#include "arch_feature.h"

// �ⲿ�����ͺ�������
// =============================================================================
extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);
extern void Load_Preload(void);
// =============================================================================

struct scb_reg volatile * const pg_scb_reg  = (struct scb_reg *)0xe000ed00;

// ϵͳʱ��ģ���ʼ���궨��
#define CLOCK_SETUP           1
#define SCS_Val               0x00000020
#define CLKSRCSEL_Val         0x00000001
#define PLL0_SETUP            1
#define PLL0CFG_Val           0x00050063
#define PLL1_SETUP            1
#define PLL1CFG_Val           0x00000023
#define CCLKCFG_Val           0x00000003
#define USBCLKCFG_Val         0x00000000
#define PCLKSEL0_Val          0x00000000
#define PCLKSEL1_Val          0x00000000
#define PCONP_Val             0x042887DE
#define CLKOUTCFG_Val         0x00000000

#define FLASH_SETUP           1
#define FLASHCFG_Val          0x00005000

// =============================================================================
// ���ܣ�ϵͳʱ�ӳ�ʼ��������OSC��ϵͳ��ʱ�ӣ�PLLʱ�ӷ�Ƶ��ʹ�ṩϵͳ����ģ��ʱ��
//       PLL0���ʱ�Ӽ��㹫ʽ��FCCO0 = 2*M*OSC_IN/N = 2*100*12M/6 = 400M
//       CPU��ʱ�Ӽ��㹫ʽ��PLL0����CCLK = FCCO0/DIV = 400M/4 = 100M
//       USB��ʱ�������USBCLK = M * F_osc or F_usb = F_cco1 / (2 * P) = 48M
//                     F_cco1 = F_osc * M * 2 * P (156 MHz to 320 MHz)
// ��������
// ���أ���
// =============================================================================
void SystemInit (void)
{
#if (CLOCK_SETUP)                       // Clock Setup
    LPC_SC->SCS       = SCS_Val;
    if (LPC_SC->SCS & (1 << 5))
    {                                   // If Main Oscillator is enabled
        while ((LPC_SC->SCS & (1<<6)) == 0)
            ;                           // Wait for Oscillator to be ready
    }

    LPC_SC->CCLKCFG   = CCLKCFG_Val;      // Setup Clock Divider
    /* Periphral clock must be selected before PLL0 enabling and connecting
     * - according errata.lpc1768-16.March.2010 -
     */
    LPC_SC->PCLKSEL0  = PCLKSEL0_Val;     // Peripheral Clock Selection */
    LPC_SC->PCLKSEL1  = PCLKSEL1_Val;

    LPC_SC->CLKSRCSEL = CLKSRCSEL_Val;    // Select Clock Source sysclk / PLL0

#if (PLL0_SETUP)
    LPC_SC->PLL0CFG   = PLL0CFG_Val;      // configure PLL0
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;

    LPC_SC->PLL0CON   = 0x01;             // PLL0 Enable
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;
    while (!(LPC_SC->PLL0STAT & (1<<26)));// Wait for PLOCK0

    LPC_SC->PLL0CON   = 0x03;             // PLL0 Enable & Connect
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;
    // Wait for PLLC0_STAT & PLLE0_STAT
    while ((LPC_SC->PLL0STAT & ((1<<25) | (1<<24))) != ((1<<25) | (1<<24)));
#endif

#if (PLL1_SETUP)
    LPC_SC->PLL1CFG   = PLL1CFG_Val;
    LPC_SC->PLL1FEED  = 0xAA;
    LPC_SC->PLL1FEED  = 0x55;

    LPC_SC->PLL1CON   = 0x01;             // PLL1 Enable
    LPC_SC->PLL1FEED  = 0xAA;
    LPC_SC->PLL1FEED  = 0x55;
    while (!(LPC_SC->PLL1STAT & (1<<10)));// Wait for PLOCK1

    LPC_SC->PLL1CON   = 0x03;             // PLL1 Enable & Connect
    LPC_SC->PLL1FEED  = 0xAA;
    LPC_SC->PLL1FEED  = 0x55;
     // Wait for PLLC1_STAT & PLLE1_STAT
    while ((LPC_SC->PLL1STAT & ((1<< 9) | (1<< 8))) != ((1<< 9) | (1<< 8)));
#else
    LPC_SC->USBCLKCFG = USBCLKCFG_Val;    //Setup USB Clock Divider
#endif

    LPC_SC->PCONP     = PCONP_Val;        // Power Control for Peripherals

    LPC_SC->CLKOUTCFG = CLKOUTCFG_Val;    // Clock Output Configuration
#endif

#if (FLASH_SETUP == 1)                      // Flash Accelerator Setup
    LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | FLASHCFG_Val;
#endif
}

// =============================================================================
// ���ܣ�CORTEX M3�ں�����������ϵͳ��λ�����󣬴ӻ����������ת��C�ĵ�һ������������
//       ���ʼ������ջ���û���ջ�������ں˼Ĵ�����Ȼ���ʼ��ʱ��ģ�顣
// ��������
// ���أ���
// =============================================================================
void Init_Cpu(void)
{
    __set_PSP((uint32_t)msp_top);
    __set_PRIMASK(1);
    __set_FAULTMASK(1);
    __set_CONTROL(0);
    switch(pg_scb_reg->CPUID)
    {
        case cn_revision_r0p0:
            break;                              //�г�û�а汾0��оƬ
        case cn_revision_r1p0:
            pg_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case cn_revision_r1p1:
            pg_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case cn_revision_r2p0:break;            //����ûʲôҪ����
    }
    SystemInit();

    Load_Preload();
    return;
}


