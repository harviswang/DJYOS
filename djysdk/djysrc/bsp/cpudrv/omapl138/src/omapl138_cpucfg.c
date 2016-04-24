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
// omapl138��cpu����ģ�������ļ�
// �ļ��汾: v1.00
// ������Ա: lst
// ����ʱ��: 2011.06.01
// Copyright(c) 2011-2011  ��������������޹�˾
//========================================================
// �����޸ļ�¼(���µķ�����ǰ��):
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
//========================================================
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��

#include "stdint.h"
#include "cpu_peri.h"

#define SETBIT(dest,mask)     (dest |= mask)
#define CLRBIT(dest,mask)     (dest &= ~mask)
#define CHKBIT(dest,mask)     (dest & mask)

enum ModulePower Cpucfg_TestLPSC(u8 module)
{
    enum ModulePower result;
    u32 domain;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(module == cn_PSC0_DSP)
        domain = cn_RAM_Pseudo;
    else
        domain = cn_Always_ON;

    if(module <cn_psc1_base)
    {
        // spin until existing transitions are done.
        while (CHKBIT(g_ptLpsc0Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(g_ptLpsc0Reg->MDSTAT[module], MASK_STATE) == PSC_ENABLE)
        {
            result = PlscPowerOn;
        }else
        {
            result = PlscPowerOff;
        }
    }else
    {
        module -= cn_psc1_base;
        // spin until existing transitions are done.
        while (CHKBIT(g_ptLpsc1Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(g_ptLpsc1Reg->MDSTAT[module], MASK_STATE) == PSC_ENABLE)
        {
            result = PlscPowerOn;
        }else
        {
            result = PlscPowerOff;
        }
    }

    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
    return result;
}
//----ʹ��psc------------------------------------------------------------------
// ��������: ʹ������,
// ��������: module���������������ţ���omapl138_peri_cpucfg.h�ļ��ж���
// ����ֵ  : ��
//-----------------------------------------------------------------------------
void Cpucfg_EnableLPSC(u8 module)
{
    u32 domain;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(module == cn_PSC0_DSP)
        domain = cn_RAM_Pseudo;
    else
        domain = cn_Always_ON;

    if(module <cn_psc1_base)
    {
        // spin until existing transitions are done.
        while (CHKBIT(g_ptLpsc0Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(g_ptLpsc0Reg->MDSTAT[module], MASK_STATE) == PSC_ENABLE)
        {
            goto exit_completed;
        }

        // setup the transition...clear the bits before setting the next state.
        CLRBIT(g_ptLpsc0Reg->MDCTL[module], NEXT);
        SETBIT(g_ptLpsc0Reg->MDCTL[module], PSC_ENABLE);

        // kick off the transition.
        g_ptLpsc0Reg->PTCMD = domain;

        // spin until transition is done.
        while (CHKBIT(g_ptLpsc0Reg->PTSTAT, domain)) {}

        while (CHKBIT(g_ptLpsc0Reg->MDSTAT[module], MASK_STATE) != PSC_ENABLE) {}
    }else
    {
        module -= cn_psc1_base;
        // spin until existing transitions are done.
        while (CHKBIT(g_ptLpsc1Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(g_ptLpsc1Reg->MDSTAT[module], MASK_STATE) == PSC_ENABLE)
        {
            goto exit_completed;
        }

        // setup the transition...clear the bits before setting the next state.
        CLRBIT(g_ptLpsc1Reg->MDCTL[module], NEXT);
        SETBIT(g_ptLpsc1Reg->MDCTL[module], PSC_ENABLE);

        // kick off the transition.
        g_ptLpsc1Reg->PTCMD = domain;

        // spin until transition is done.
        while (CHKBIT(g_ptLpsc1Reg->PTSTAT, domain)) {}

        while (CHKBIT(g_ptLpsc1Reg->MDSTAT[module], MASK_STATE) !=PSC_ENABLE) {}
    }

exit_completed:
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}

//----��ֹpsc------------------------------------------------------------------
//����: ��ֹ������ʡ��
//����: module���������������ţ���cpu_peri_cpucfg.h�ļ��ж���
//����: ��
//-----------------------------------------------------------------------------
void Cpucfg_DisableLPSC(u8 module)
{
    u32 domain;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(module == cn_PSC0_DSP)
        domain = cn_RAM_Pseudo;
    else
        domain = cn_Always_ON;

    if(module <cn_psc1_base)
    {
        // spin until existing transitions are done.
        while (CHKBIT(g_ptLpsc0Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(g_ptLpsc0Reg->MDSTAT[module], MASK_STATE) == PSC_DISABLE)
        {
            goto exit_completed;
        }

        // setup the transition...clear the bits before setting the next state.
        CLRBIT(g_ptLpsc0Reg->MDCTL[module], NEXT);
        SETBIT(g_ptLpsc0Reg->MDCTL[module], PSC_DISABLE);

        // kick off the transition.
        g_ptLpsc0Reg->PTCMD = domain;

        // spin until transition is done.
        while (CHKBIT(g_ptLpsc0Reg->PTSTAT, domain)) {}

        while (CHKBIT(g_ptLpsc0Reg->MDSTAT[module], MASK_STATE) != PSC_DISABLE)
        {}
    }else
    {
        module -= cn_psc1_base;
        // spin until existing transitions are done.
        while (CHKBIT(g_ptLpsc1Reg->PTSTAT, domain)) {}

        // if we are already in the requested state...just return.
        if (CHKBIT(g_ptLpsc1Reg->MDSTAT[module], MASK_STATE) == PSC_DISABLE)
        {
            goto exit_completed;
        }

        // setup the transition...clear the bits before setting the next state.
        CLRBIT(g_ptLpsc1Reg->MDCTL[module], NEXT);
        SETBIT(g_ptLpsc1Reg->MDCTL[module], PSC_DISABLE);

        // kick off the transition.
        g_ptLpsc1Reg->PTCMD = domain;

        // spin until transition is done.
        while (CHKBIT(g_ptLpsc1Reg->PTSTAT, domain)) {}

        while (CHKBIT(g_ptLpsc1Reg->MDSTAT[module], MASK_STATE) !=PSC_DISABLE){}
    }

exit_completed:
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}

//-----------------------------------------------------------------------------
// ��������: ģ��ͬ����λ
// ��������: module���������������ţ���omapl138_peri_cpucfg.h�ļ��ж���
// ����ֵ  : ��
//-----------------------------------------------------------------------------
void Cpucfg_SyncResetLPSC(u8 module)
{
    u32 domain;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    if(module == cn_PSC0_DSP)
        domain = cn_RAM_Pseudo;
    else
        domain = cn_Always_ON;



    if(module <cn_psc1_base)
    {
        if( (g_ptLpsc0Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET )
        {
          g_ptLpsc0Reg->MDCTL[module] &= ~(u32)MASK_STATE;
          g_ptLpsc0Reg->MDCTL[module] |= PSC_SYNCRESET;
          g_ptLpsc0Reg->PTCMD = domain;

          /*Wait for power state transition to finish*/
          while (CHKBIT(g_ptLpsc0Reg->PTSTAT, domain)) {}

          while ((g_ptLpsc0Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET);
        }
    }else
    {
        module -= cn_psc1_base;
        if( (g_ptLpsc1Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET )
        {
          g_ptLpsc1Reg->MDCTL[module] &= ~(u32)MASK_STATE;
          g_ptLpsc1Reg->MDCTL[module] |= PSC_SYNCRESET;
          g_ptLpsc1Reg->PTCMD = domain;

          /*Wait for power state transition to finish*/
          while (CHKBIT(g_ptLpsc1Reg->PTSTAT, domain)) {}

          while ((g_ptLpsc1Reg->MDSTAT[module] & MASK_STATE) != PSC_SYNCRESET);
        }
    }

    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}
