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

#include "os_inc.h"
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
