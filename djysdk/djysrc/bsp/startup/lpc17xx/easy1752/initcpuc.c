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
#include "stdint.h"
#include "cpu_peri.h"

struct scb_reg volatile * const pg_scb_reg  = (struct scb_reg *)0xe000ed00;

#define cn_fcco             (CN_CFG_MCLK* 3)    //  PLLƵ��(275Mhz~550Mhz)
#define cn_pll_n            1               //  1~256
#define cn_pll_m            ((cn_fcco / CN_CFG_EXTCLK) * (cn_pll_n) / 2)   //6~512

void Init_Cpu(void)
{
    extern   uint32_t   msp_top[ ];
    //flash��Ϊ����
    pg_sysctrl_reg->flashcfg = ((0x05ul << 12) & (~(0x003f))) | 0x003a;

    __set_PSP(msp_top);
    __set_MSP(msp_top);
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


//����ʱ��ѡ���ڴ�ȫ����Ϊ��ͬ����ֵ����ֵ��config-prj.h�ж���
#if (CN_CFG_MCLK/4 == CN_CFG_PCLK)          //����ʱ��=1/4��ʱ��
    pg_sysctrl_reg->pclksel0 = 0x00000000;
    pg_sysctrl_reg->pclksel1 = 0x00000000;
#endif

#if (CN_CFG_MCLK/2 == CN_CFG_PCLK)          //����ʱ��=1/2��ʱ��

    pg_sysctrl_reg->pclksel0 = 0xAAAAAAAA;
    pg_sysctrl_reg->pclksel1 = 0xAAAAAAAA;
#endif

#if (CN_CFG_MCLK == CN_CFG_PCLK)          //����ʱ��=��ʱ��

    pg_sysctrl_reg->pclksel0 = 0x55555555;
    pg_sysctrl_reg->pclksel1 = 0x55555555;
#endif
    pg_sysctrl_reg->clkoutcfg = 0;

    //�Ͽ�PLL��������ٴ���
    if ((pg_sysctrl_reg->pll0stat >> 24) == 1)
    {
        pg_sysctrl_reg->pll0con = 1;        //pllʹ�ܣ����Ͽ�����
        pg_sysctrl_reg->pll0feed = 0xAA;
        pg_sysctrl_reg->pll0feed = 0x55;
    }
    pg_sysctrl_reg->pll0con = 0;            //pll��ֹ���ҶϿ�����
    pg_sysctrl_reg->pll0feed = 0xAA;
    pg_sysctrl_reg->pll0feed = 0x55;
    while(pg_sysctrl_reg->pll0stat & (3 << 24)) {    }

     //������Ҫʱ��
     //  Enable mainOSC,1MHz~20MHz
    pg_sysctrl_reg->scs = (pg_sysctrl_reg->scs & 0x04) | 0x20;

    while ((pg_sysctrl_reg->scs & ((u32)1 << 6)) == 0) {    }

    pg_sysctrl_reg->clksrcsel = 0x01;  //select main OSC as the PLL clock source
    pg_sysctrl_reg->pll0cfg   = (((cn_pll_n - 1) << 16) | (cn_pll_m - 1));
    pg_sysctrl_reg->pll0feed  = 0xAA;
    pg_sysctrl_reg->pll0feed  = 0x55;

    pg_sysctrl_reg->pll0con   = 1;        //pllʹ�ܣ����Ͽ�����
    pg_sysctrl_reg->pll0feed  = 0xAA;
    pg_sysctrl_reg->pll0feed  = 0x55;

    while ((pg_sysctrl_reg->pll0stat & ((u32)1 << 24)) == 0) {    }

    pg_sysctrl_reg->cclkcfg = (cn_fcco / CN_CFG_MCLK) - 1;

    //  Check lock bit status
    while (((pg_sysctrl_reg->pll0stat & (1ul << 26)) == 0)) {   }

    while (((pg_sysctrl_reg->pll0stat & 0x00007FFF) != (cn_pll_m - 1)) &&
           (((pg_sysctrl_reg->pll0stat & 0x00FF0000) >> 16) != (cn_pll_n - 1)))
    {
    }

    pg_sysctrl_reg->pll0con  = 3;               //ʹ�ܲ�����pll
    pg_sysctrl_reg->pll0feed = 0xAA;
    pg_sysctrl_reg->pll0feed = 0x55;
    //�ȴ�ֱ��ʱ�����Ӳ�����
    while ((pg_sysctrl_reg->pll0stat & ((u32)1 << 25)) == 0) {   }

// ���ô洢������ģ��
#if CN_CFG_MCLK <= 20000000                         //ʹ��1��CPUʱ��
    pg_sysctrl_reg->flashcfg = (((u32)0x01 << 12) & (~(0x003f))) | 0x003a;
#endif

#if CN_CFG_MCLK > 20000000 && CN_CFG_MCLK <= 40000000   //ʹ��2��CPUʱ��
    pg_sysctrl_reg->flashcfg = (((u32)0x02 << 12) & (~(0x003f))) | 0x003a;
#endif

#if CN_CFG_MCLK > 40000000 && CN_CFG_MCLK <= 60000000   //ʹ��3��CPUʱ��
    pg_sysctrl_reg->flashcfg = (((u32)0x03 << 12) & (~(0x003f))) | 0x003a;
#endif

#if CN_CFG_MCLK > 60000000 && CN_CFG_MCLK <= 80000000   //ʹ��4��CPUʱ��
    pg_sysctrl_reg->flashcfg = (((u32)0x04 << 12) & (~(0x003f))) | 0x003a;
#endif

#if CN_CFG_MCLK > 80000000 && CN_CFG_MCLK <= 100000000  //ʹ��5��CPUʱ��
    pg_sysctrl_reg->flashcfg = (((u32)0x05 << 12) & (~(0x003f))) | 0x003a;
#endif
    Load_Preload();
    return;
}


