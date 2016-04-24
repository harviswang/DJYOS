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
#include "arch_feature.h"
#include "stdint.h"
#include "cpu_peri.h"
#include "exception.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);

extern void SRAM_Init(void);
extern void Load_Preload(void);

struct scb_reg volatile * const pg_scb_reg  = (struct scb_reg *)0xe000ed00;

void Init_Cpu(void)
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
    pg_inflash_fpec_reg->ACR |= (CN_CFG_MCLK-1)/24000000;   //���õȴ����ڡ�
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

    Load_Preload();
}

