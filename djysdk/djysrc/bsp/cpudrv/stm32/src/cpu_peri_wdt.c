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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_wdt.c
// ģ������: WDTģ�飬ʹ����WDT��������Ź�
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 28/08.2014
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "wdt_hal.h"
#include "cpu_peri.h"
#include "stm32f10x.h"

#define CN_WDT_WDTCYCLE  5000*mS//200*1000      //��λΪ΢��
#define CN_WWDG_WDTCYCLE  50*mS                 //WWDG���Ϊ58mS
#define CN_WDT_DOGNAME   "STM32IWDG"
#define CN_WWDG_DOGNAME   "STM32WWDG"

#define CN_IWDG_BASE  0x40003000
#define CN_WWDG_BASE  0x40002C00
#define STM_IWDG      ((tagIWDGReg*)CN_IWDG_BASE)
#define STM_WWDG      ((tagWWDGReg*)CN_WWDG_BASE)

#define CN_PRE_VALUE   5
static u16 s_prevalue[]={4,8,16,32,64,128,256,256};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ------------------------ STM32 �������Ź� -----------------------------------
// ----------------------------------------------------------------------------
// =============================================================================
// ��������:ʹ�ܿ��Ź�
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32Enable(void)
{
    STM_IWDG->IWDG_KR = 0xCCCC;
    return true;
}

// =============================================================================
// ��������:IWDG_Stm32WdtFeed
//          IWDG_Stm32WdtFeed��ι������
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32WdtFeed(void)
{
    STM_IWDG->IWDG_KR = 0xAAAA;
    return true;
}

// =============================================================================
// ��������:IWDG_PreSet,IWDGʱ��ԴΪ�ڲ�RC��Ϊ40KHz
//          IWDG_PreSet����ʱ�ӷֲ�ϵ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32PreSet(u8 Value)
{
    //����IWDGģʽ
    if(!(STM_IWDG->IWDG_SR & IWDG_SR_PVU_MASK))
    {
        STM_IWDG->IWDG_KR = 0x5555;
        STM_IWDG->IWDG_PR = IWDG_PR_PR & Value;
        //wait finish
        while((STM_IWDG->IWDG_SR & IWDG_SR_PVU_MASK));
        return true;
    }
    else
        return false;
}

// =============================================================================
// ��������:IWDG_Stm32ReloadSet,������װ��ֵ
//          IWDG_Stm32ReloadSetӲ�����Ź���ʼ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32ReloadSet(u16 value)
{
    if(!(STM_IWDG->IWDG_SR & IWDG_SR_RVU_MASK))
    {
        STM_IWDG->IWDG_KR = 0x5555;
        STM_IWDG->IWDG_RLR = IWDG_RLR_RL & value;
        //wait finish
        while((STM_IWDG->IWDG_SR & IWDG_SR_RVU_MASK));
        return true;
    }
    else
        return false;
}

// =============================================================================
// ��������:IWDG_Stm32Initial
//          IWDG_Stm32Initial���Ź�ע��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵����IWDG�����ã�������PR��RL��STM_IWDG->IWDG_SR��Ӧλ����Ϊ1
// =============================================================================
bool_t IWDG_Stm32Initial(u32 setcycle)
{
    bool_t result;
    pg_rcc_reg->CSR |= (1<<0);//turn on LSI
    while(!(pg_rcc_reg->CSR & (1<<1)));

    IWDG_Stm32PreSet(CN_PRE_VALUE);     //����ʱ��Ԥ��Ƶ��ʱ��ԴΪ40KHZ
    //������װ��ֵ����ι����װ��ֵ,����Ϊ5��Ƶ
    IWDG_Stm32ReloadSet((40/s_prevalue[CN_PRE_VALUE]) * (CN_WDT_WDTCYCLE/mS));

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CN_WDT_WDTCYCLE,\
                                    IWDG_Stm32WdtFeed,NULL,NULL);

    IWDG_Stm32Enable();
    return result;
}

bool_t WWDG_STM32WdtFeed(void)
{
    STM_WWDG->WWDG_CR |= 0x7F;
    return true;
}

//PCLK1=36Mʱ�ӣ�CK��ʱ��ʱ��(PCLK1����4096)����8��Ϊ1.09����WWDG������ÿ
//1.09mS����һ�Σ�WWDG���Ź����ι��ʱ��Ϊ58mS
bool_t WWDG_STM32Init(u32 setcycle)
{
    bool_t result;
    pg_rcc_reg->APB1ENR |= (1<<11);//enable wwdg
    STM_WWDG->WWDG_CR &= ~(1<<7);//WDGA
    STM_WWDG->WWDG_CFR |= (3<<7);//WDGTB0,1
    WWDG_STM32WdtFeed();

    result = WdtHal_RegisterWdtChip(CN_WWDG_DOGNAME,CN_WWDG_WDTCYCLE,\
                            WWDG_STM32WdtFeed,NULL,NULL);
    STM_WWDG->WWDG_CR |= (1<<7);//ʹ�ܿ��Ź�
    return result;
}
