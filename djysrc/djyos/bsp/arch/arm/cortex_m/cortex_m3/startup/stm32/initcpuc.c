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
#include "cortexm3.h"
#include "cpu_peri.h"
#include "exception.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);

extern void SRAM_Init(void);
extern void load_preload(void);

struct scb_reg volatile * const pg_scb_reg  = (struct scb_reg *)0xe000ed00;

void cpu_init(void)
{

	__set_PSP((uint32_t)msp_top);
	__set_PRIMASK(1);
	__set_FAULTMASK(1);
	__set_CONTROL(0);
	switch(pg_scb_reg->CPUID)
    {
        case cn_revision_r0p0:
        	break;    //�г�û�а汾0��оƬ
        case cn_revision_r1p0:
            pg_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case cn_revision_r1p1:
            pg_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case cn_revision_r2p0:break;    //����ûʲôҪ����
    }

    pg_inflash_fpec_reg->ACR &= ~(u32)0x1f;
    pg_inflash_fpec_reg->ACR |= (cn_mclk-1)/24000000;   //���õȴ����ڡ�
    pg_inflash_fpec_reg->ACR |= 0x10;       //����Ԥȡ


    if(((pg_rcc_reg->CR & cn_cr_check_mask) != cn_cr_check)
                || ((pg_rcc_reg->CFGR & cn_cfgr_check_mask) != cn_cfgr_check))
    {
        //��ʼ��ʼ��ʱ��
        //step1:��λʱ�ӿ��ƼĴ���
        pg_rcc_reg->CR |= (uint32_t)0x00000001;
        // ��λ SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] MCO[2:0] λ
        pg_rcc_reg->CFGR &= (uint32_t)0xF8FF0000;
        // ��λ HSEON, CSSON and PLLON λ
        pg_rcc_reg->CR &= (uint32_t)0xFEF6FFFF;
        // ��λ HSEBYP λ
        pg_rcc_reg->CR &= (uint32_t)0xFFFBFFFF;
        // ��λ PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE λ
        pg_rcc_reg->CFGR &= (uint32_t)0xFF80FFFF;
        // ��ֹ�����ж�
        pg_rcc_reg->CIR = 0x00000000;

        //step2:���ø�ʱ�ӿ���λ�Լ���Ƶ����Ƶֵ
        pg_rcc_reg->CFGR = cn_cfgr_set+(7<<24);   // set clock configuration register
        pg_rcc_reg->CR   = cn_cr_set;     // set clock control register

        while(bb_rcc_cr_hserdy ==0);
        while(bb_rcc_cr_pllrdy ==0);
    }
    SRAM_Init();

    load_preload();
}

