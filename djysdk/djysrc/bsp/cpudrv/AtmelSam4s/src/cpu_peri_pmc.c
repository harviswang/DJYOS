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
//�汾��     V1.0.0
//��ʼ�汾������ڣ�2014-1-20
//�ļ�����:  cpuʱ�ӳ�ʼ��
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2014-1-20
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os.h"
#include "cpu_peri_pmc.h"

//todo  ---- #if MCK = 100MHz
#define BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8))
#define BOARD_PLLBR     (CKGR_PLLBR_MULB(24) \
                       | CKGR_PLLBR_PLLBCOUNT(0x1) \
                       | CKGR_PLLBR_DIVB(3))
#define BOARD_MCKR      (PMC_MCKR_PRES_CLK | PMC_MCKR_CSS_PLLB_CLK)

/* Define clock timeout */
#define CLOCK_TIMEOUT    0xFFFFFFFF

#define MASK_STATUS0 0xFFFFFFFC
#define MASK_STATUS1 0xFFFFFFFF

struct atmel_pmc_reg volatile * const SAM_PMC_REG =
                                    (struct atmel_pmc_reg *)0x400E0400;
//------CPUʱ�ӳ�ʼ��-----------------------------------------------------
//����: ��ʼ��CPUʱ�ӣ��ϵ�Ĭ��ʹ�õ����ڲ��ľ���ʱ�ӣ��ú���������Դ�л���
 //     �ⲿ����ʱ�ӣ����潫main clockʱ���л���PLLB��Ƶ���ʱ��
//����: ��
//����: ��
//-----------------------------------------------------------------------
void cpu_clk_init(void)
{
    uint32_t timeout = 0;

    /* Initialize main oscillator */
    if ( !(SAM_PMC_REG->CKGR_MOR & CKGR_MOR_MOSCSEL) )
    {
    	SAM_PMC_REG->CKGR_MOR = CKGR_MOR_KEY(0x37) | BOARD_OSCOUNT |
                                CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
        timeout = 0;
        while (!(SAM_PMC_REG->PMC_SR & PMC_SR_MOSCXTS) &&
                                    (timeout++ < CLOCK_TIMEOUT));
    }

    /* Switch to 3-20MHz Xtal oscillator */
    SAM_PMC_REG->CKGR_MOR = CKGR_MOR_KEY(0x37) | BOARD_OSCOUNT |
                CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
    timeout = 0;
    while (!(SAM_PMC_REG->PMC_SR & PMC_SR_MOSCSELS) && (timeout++ < CLOCK_TIMEOUT));
    SAM_PMC_REG->PMC_MCKR = (SAM_PMC_REG->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) |
                                PMC_MCKR_CSS_MAIN_CLK;
    for ( timeout = 0; !(SAM_PMC_REG->PMC_SR & PMC_SR_MCKRDY) &&
                            (timeout++ < CLOCK_TIMEOUT) ; );

    /* Initialize PLLB */
    SAM_PMC_REG->CKGR_PLLBR = BOARD_PLLBR;
    timeout = 0;
    while (!(SAM_PMC_REG->PMC_SR & PMC_SR_LOCKB) && (timeout++ < CLOCK_TIMEOUT));

    /* Switch to main clock */
    SAM_PMC_REG->PMC_MCKR = (BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    for ( timeout = 0; !(SAM_PMC_REG->PMC_SR & PMC_SR_MCKRDY) &&
                                    (timeout++ < CLOCK_TIMEOUT) ; );

    SAM_PMC_REG->PMC_MCKR = BOARD_MCKR ;
    for ( timeout = 0; !(SAM_PMC_REG->PMC_SR & PMC_SR_MCKRDY) &&
                                    (timeout++ < CLOCK_TIMEOUT) ; );
}

//------ʹ������ʱ��-----------------------------------------------------------
//����: ����ʱ�ӿɵ���ʹ�ܺͳ��ܣ��Խ�Լ�ܺ�
//����: dwId������ID������ID�ɲο��ֲ�39ҳ
//����: ��
//-----------------------------------------------------------------------------
void PMC_EnablePeripheral( uint32_t dwId )
{
    if(dwId > 34)
    	return;

    if ( dwId < 32 )
    {
        if ( (SAM_PMC_REG->PMC_PCSR0 & ((uint32_t)1 << dwId)) == ((uint32_t)1 << dwId) )
        {
//            djy_printf( "PMC_EnablePeripheral: clock of peripheral"  "
//                        %u is already enabled\n\r", (unsigned int)dwId ) ;
        }
        else
        {
            SAM_PMC_REG->PMC_PCER0 = 1 << dwId ;
        }
    }
    else
    {
        dwId -= 32;
        if ((SAM_PMC_REG->PMC_PCSR1 & ((uint32_t)1 << dwId)) == ((uint32_t)1 << dwId))
        {
//            djy_printf( "PMC_EnablePeripheral: clock of peripheral"  "
//                    %u is already enabled\n\r", (unsigned int)(dwId + 32) ) ;
        }
        else
        {
            SAM_PMC_REG->PMC_PCER1 = 1 << dwId ;
        }
    }
}

//------��������ʱ��-----------------------------------------------------------
//����: ����ʱ�ӿɵ���ʹ�ܺͳ��ܣ��Խ�Լ�ܺ�
//����: dwId������ID������ID�ɲο��ֲ�39ҳ
//����: ��
//-----------------------------------------------------------------------------
void PMC_DisablePeripheral( uint32_t dwId )
{
    if(dwId > 34)
    	return;

    if ( dwId < 32 )
    {
        if ( (SAM_PMC_REG->PMC_PCSR0 & ((uint32_t)1 << dwId)) != ((uint32_t)1 << dwId) )
        {
//            djy_printf("PMC_DisablePeripheral: clock of peripheral" "
//                            %u is not enabled\n\r", (unsigned int)dwId ) ;
        }
        else
        {
            SAM_PMC_REG->PMC_PCDR0 = 1 << dwId ;
        }
    }
    else
    {
        dwId -= 32 ;
        if ( (SAM_PMC_REG->PMC_PCSR1 & ((uint32_t)1 << dwId)) != ((uint32_t)1 << dwId) )
        {
//            djy_printf( "PMC_DisablePeripheral: clock of peripheral" "
//                        %u is not enabled\n\r", (unsigned int)(dwId + 32) ) ;
        }
        else
        {
            SAM_PMC_REG->PMC_PCDR1 = 1 << dwId ;
        }
    }
}

//------ʹ����������ʱ��-------------------------------------------------------
//����: ����ʱ�ӿɵ���ʹ�ܺͳ��ܣ��Խ�Լ�ܺ�
//����: dwId������ID������ID�ɲο��ֲ�39ҳ
//����: ��
//-----------------------------------------------------------------------------
void PMC_EnableAllPeripherals( void )
{
    SAM_PMC_REG->PMC_PCER0 = MASK_STATUS0 ;
    while ( (SAM_PMC_REG->PMC_PCSR0 & MASK_STATUS0) != MASK_STATUS0 ) ;

    SAM_PMC_REG->PMC_PCER1 = MASK_STATUS1 ;
    while ( (SAM_PMC_REG->PMC_PCSR1 & MASK_STATUS1) != MASK_STATUS1 ) ;

//    djy_printf( "Enable all periph clocks\n\r" ) ;
}

//------ʹ������ʱ��-----------------------------------------------------------
//����: ����ʱ�ӿɵ���ʹ�ܺͳ��ܣ��Խ�Լ�ܺ�
//����: dwId������ID������ID�ɲο��ֲ�39ҳ
//����: ��
//-----------------------------------------------------------------------------
void PMC_DisableAllPeripherals( void )
{
    SAM_PMC_REG->PMC_PCDR0 = MASK_STATUS0 ;
    while ( (SAM_PMC_REG->PMC_PCSR0 & MASK_STATUS0) != 0 ) ;

    SAM_PMC_REG->PMC_PCDR1 = MASK_STATUS1 ;
    while ( (SAM_PMC_REG->PMC_PCSR1 & MASK_STATUS1) != 0 ) ;

//    djy_printf( "Disable all periph clocks\n\r" ) ;
}

//------�ж�����ʱ���Ƿ�ʹ��---------------------------------------------------
//����: �ж�ָ��������ID�ŵ�����ʱ���Ƿ�ʹ��
//����: dwId������ID������ID�ɲο��ֲ�39ҳ
//����: 0,����   1,ʹ��
//-----------------------------------------------------------------------------
uint32_t PMC_IsPeriphEnabled( uint32_t dwId )
{
    if(dwId > 34)
    	return 0xFF;

    if ( dwId < 32 )
    {
        return ( SAM_PMC_REG->PMC_PCSR0 & (1 << dwId) ) ;
    }
    else {
        return ( SAM_PMC_REG->PMC_PCSR1 & (1 << (dwId - 32)) ) ;
    }
}

