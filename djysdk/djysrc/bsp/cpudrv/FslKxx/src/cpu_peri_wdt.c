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

#include "stdint.h"
#include "cpu_peri.h"
#include "wdt_hal.h"

#define CN_WDT_WDTCYCLE  5*1000*1000      //��λΪ΢��
#define CN_WDT_DOGNAME   "FreescaleKxxWDT"

/********************************************************************/
/*
 * Watchdog timer unlock routine. Writing 0xC520 followed by 0xD928
 * will unlock the write once registers in the WDOG so they are writable
 * within the WCT period.
 *
 * Parameters:
 * none
 ********************************************************************/
void WDT_Unlock(void)
{
    atom_high_t atom;
    //UNLOCKʱ����뱣֤�������
    atom = Int_HighAtomStart();

    /* Write 0xC520 to the unlock register */
    WDOG->UNLOCK = 0xC520;
    /* Followed by 0xD928 to complete the unlock */
    WDOG->UNLOCK = 0xD928;

    Int_HighAtomEnd(atom);
}
/********************************************************************/
/*
 * Watchdog timer disable routine
 *
 * Parameters:
 * none
 *******************************************************************/
void WDT_Disable(void)
{
    /* First unlock the watchdog so that we can write to registers */
    WDT_Unlock();

    /* Clear the WDOGEN bit to disable the watchdog */
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

/********************************************************************/
/*
 * Watchdog timer enable routine
 *
 * Parameters:
 * none
 *******************************************************************/
void WDT_Enable(void)
{
    WDT_Unlock();

    WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;
}

// =============================================================================
// ��������:�趨WDT��ι�����ڣ���������û��ι�����򹷽�
// �������:cycle,���ڣ�΢��
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
void WDT_SetCycle(u32 cycle)
{
    u8 temp,wdt_clk;
    temp = (WDOG->PRESC & 0x700)>>8;
    wdt_clk = CN_BUS_CLK/((temp + 1)*Mhz);

    //WDT��ʱ����Դ��BUSʱ�ӣ���ˣ���ʱ��Ϊ�ں�ʱ�ӵ�һ��
    WDT_Unlock();
    WDOG->TOVALH = (u16)((wdt_clk * cycle) & 0xFFFF0000) >> 16;
    WDOG->STCTRLL = (u16)((wdt_clk * cycle) & 0xFFFF);
}

// =============================================================================
// ��������:WDT_FreescaleWdtFeed
//          Freescaleι������
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_WdtFeed(void)
{
    atom_high_t atom;

    WDT_Unlock();
    //UNLOCKʱ����뱣֤�������
    atom = Int_HighAtomStart();
    WDOG->REFRESH = 0xA602;
    WDOG->REFRESH = 0xB480;

    Int_HighAtomEnd(atom);

    //just for test
    GPIO_PinToggle(GPIO_PORT_A,GPIO_PIN(29));
    return true;
}

// =============================================================================
// ��������:���жϱ�־����λ��WDT���жϷ���������WCTʱ���ϵͳ�ᱻRESET
// �������:��
// ����ֵ  :
// =============================================================================
u32 WDT_ISR(ufast_t IntLine)
{
    WDT_Unlock();

    WDOG->STCTRLL |= (1<<15);   //���ж�

    return 0;
}

// =============================================================================
// ��������:WDT_IntInit��ע�ῴ�Ź��жϣ�FREESCALE�Ŀ��Ź���ʱ���ȷ����жϣ���RESET
// �������:��
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
void WDT_IntInit(void)
{
    u8 IntLine = CN_INT_LINE_WDT;
    Int_IsrConnect(IntLine,WDT_ISR);
    Int_SettoReal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreRealLine(IntLine);
}

// =============================================================================
// ��������:WDT_FreescaleInit
//          FREESCALE���Ź�ע��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_FslInit(u32 setcycle)
{
    bool_t result;

    //��ʼ��WDTģ��
    WDT_SetCycle(CN_WDT_WDTCYCLE);
//  WDT_IntInit();

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CN_WDT_WDTCYCLE,\
                                    WDT_WdtFeed,NULL,NULL);

    WDT_Enable();
    return result;
}

