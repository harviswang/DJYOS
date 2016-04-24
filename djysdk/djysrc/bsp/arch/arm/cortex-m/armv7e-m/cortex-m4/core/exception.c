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
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: cm4�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ����: 2010-01-20
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: �޸� exp_esr_tick������������tick�ж��е����첽�źŵ�ʹ�ܺͽ�ֹ
//      ����ʱ������en_asyn_signal_counter�Ȳ���δ��ȷ���ã�������⡣
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#include "stdint.h"
#include "exception.h"
#include "exp_api.h"
#include "stdio.h"
#include "endian.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"

extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�
extern struct tagIntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);

//�쳣��������ⲿ����
extern void exp_systick_handler(void);
extern void exp_svc_handler(void);
extern void exp_memfault_handler(void);
extern void exp_usagefault_handler(void);
extern void exp_busfault_handler(void);

extern   uint32_t   msp_top[ ];
extern   void Init_Cpu(void);
extern   void hardfault_handler(void);


//ϵͳtick����������
void (*user_systick)(u32 inc_ticks);

u32 g_u32ExpTable[16] __attribute__((section(".table.exceptions")));


// =============================================================================
// ����:�쳣�������ʼ����������������ָ���ĵ�ַ����,��ʹ�ܸ����쳣
// ����:��
// ����:��
// =============================================================================
static void __Exp_TableSet(void)
{
    g_u32ExpTable[0]                            = (uint32_t)msp_top;
    g_u32ExpTable[CN_SYSVECT_RESET]             = (uint32_t) Init_Cpu;
    g_u32ExpTable[CN_SYSVECT_NMI]               = (uint32_t) hardfault_handler;
    g_u32ExpTable[CN_SYSVECT_HARD_FAULT]        = (uint32_t) hardfault_handler;
    g_u32ExpTable[CN_SYSVECT_MEMMANAGE_FAULT]  = (uint32_t) exp_memfault_handler;
    g_u32ExpTable[CN_SYSVECT_BUS_FAULT]         = (uint32_t) exp_busfault_handler;
    g_u32ExpTable[CN_SYSVECT_USAGE_FAULT]   = (uint32_t) exp_usagefault_handler;
    g_u32ExpTable[7]                            = (uint32_t) hardfault_handler;
    g_u32ExpTable[8]                            = (uint32_t) hardfault_handler;
    g_u32ExpTable[9]                            = (uint32_t) hardfault_handler;
    g_u32ExpTable[10]                       = (uint32_t) hardfault_handler;
    g_u32ExpTable[CN_SYSVECT_SVC]               = (u32)exp_svc_handler;
    g_u32ExpTable[CN_SYSVECT_DEBUG]             = (uint32_t) hardfault_handler;
    g_u32ExpTable[13]                       = (uint32_t) hardfault_handler;
    g_u32ExpTable[CN_SYSVECT_PENDSV]            = (uint32_t) hardfault_handler;
    g_u32ExpTable[CN_SYSVECT_SYSTICK]           = (u32)exp_systick_handler;

    pg_scb_reg->VTOR = (u32)g_u32ExpTable;
    pg_scb_reg->systick_pri15 = 0xff;   //������ȼ�,systickӦ�������첽�źŹض�
    pg_scb_reg->svc_pri11 = 0x0;       //svc�����ȼ����첽�ź���ͬ��
    pg_scb_reg->SHCSR |=0x70000;     //���� ���ߡ��÷����洢�� fault

    pg_scb_reg->CCR |= 0x610;       //ǿ��SP�Զ����룬��0
}

// =============================================================================
// �������ܣ�__SwapExpCpuInfoByEndian
//          ת��Ӳ��CPU�쳣��Ϣ�ֽ���
// ���������cpuinfo, CPU�쳣ͨ����Ϣ
// ���������cpuinfo, CPU�쳣ͨ����Ϣ��ת����
// ����ֵ  ����
// ˵��    ���ڲ�����
// =============================================================================
void __SwapExpCpuInfoByEndian(struct SysExceptionInfo *cpuinfo)
{
    u32 *temp;
    u32 i,len;
    temp =(u32 *)cpuinfo;
    len = (sizeof(struct SysExceptionInfo))/4;
    for(i = 0; i<len; i++)
    {
        *temp = swapl(*temp);
        temp++;
    }
}

// =============================================================================
// ����:SYSTICK����������������ָ����tick������ʼ������Exp_SystickTickHandler������
// ����:tick,SYSTICK���û�������
// ����:��
// =============================================================================
void Exp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

// =============================================================================
// ����:��Cortex Mϵͳ�ں��У��ں�ϵͳ��ʱ��SYSTICI��ʱ�ж������쳣���ú����Ǵ��쳣
//      ������Ļ�����������C�ķ���������ʵ��ϵͳTICK����
// ����:��
// ����:��
// =============================================================================
void Exp_SystickTickHandler(void)
{
    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;

    user_systick(1);

    tg_int_global.nest_asyn_signal = 0;
//    tg_int_global.en_asyn_signal = true;
    tg_int_global.en_asyn_signal_counter = 0;
    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
}

// =============================================================================
// ����:ϵͳ�ڴ�����쳣����������Ҫ������memory manage �쳣��ص��쳣
// ����:core_info���쳣�����ֳ�
// ����: _SYSEXP_RESULT_TYPE
// =============================================================================
enum _EN_EXP_DEAL_TYPE_ Exp_MemManageFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct tagExpThrowPara  head;
    u32 ExpPC;
    u32 result = EN_EXP_DEAL_IGNORE;
    bool_t   shedule_bak,MMA_Flag=false;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    ExpRecord.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    if(pg_scb_reg->MFSR & CN_EXP_MFSR_MMARVALID)
    {
        ExpPC = pg_scb_reg->MMFAR;
        MMA_Flag = true;
    }
    //handler the exp
    if(pg_scb_reg->MFSR & CN_EXP_MFSR_IACCVIOL)
    {
        if(MMA_Flag == true)
        {
            printk("MEM Fault: Instruction Access Err At 0x%08x\n\r",ExpPC);
        }
        else
        {
            printk("MEM Fault: Instruction Access Err\n\r");
        }
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_DACCVIOL)
    {
        if(MMA_Flag == true)
        {
            printk("MEM Fault: Data Access Err At 0x%08x\n\r",ExpPC);
        }
        else
        {
            printk("MEM Fault: Data Access Err\n\r");
        }
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_MUNSTKERR)
    {
        printk("MEM Fault: Unstack Err\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_MSTKERR)
    {
        printk("MEM Fault: Stack Err\n\r");
        result = EN_EXP_DEAL_RESET;
        result = EN_EXP_DEAL_RESET;
    }
    else
    {
        printk("MEM Fault: %s\n\r","Unknown MEM Fault");
        result = EN_EXP_DEAL_RESET;
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


   head.dealresult = result;
   head.para  = (u8 *)(&ExpRecord);
   head.para_len = sizeof(struct SysExceptionInfo);
   head.name = CN_HARDEXP_DECODERNAME;
   head.validflag = true;
   Exp_Throw(&head,&result);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return result;
}

// =============================================================================
// ����:ϵͳӲ�����󣬿��ܻ�������������fault�ϷµĽ��������ǣ���Ӧ�ý�һ����ѯԭ��
// ����:core_info���쳣�����ֳ�
// ����: _SYSEXP_RESULT_TYPE
// =============================================================================
enum _EN_EXP_DEAL_TYPE_ Exp_HardFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct tagExpThrowPara  head;
    u32 result = EN_EXP_DEAL_IGNORE;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    ExpRecord.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->HFSR & CN_EXP_HFSR_VECTBL)
    {
        printk("Hard Fault: VECTBL Access Err\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->UFSR & CN_EXP_HFSR_FORCED)
    {
        printk("Hard Fault: Forced By Other Fault\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->HFSR & CN_EXP_HFSR_DEBUGEVT)
    {
        printk("Hard Fault: Because Of Debug\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else
    {
        printk("Hard Fault: %s\n\r","Unknown Hard Fault");
        result = EN_EXP_DEAL_RESET;
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


   head.dealresult = result;
   head.para  = (u8 *)(&ExpRecord);
   head.para_len = sizeof(struct SysExceptionInfo);
   head.name = CN_HARDEXP_DECODERNAME;
   head.validflag = true;
   Exp_Throw(&head,&result);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return result;
}

// =============================================================================
// ����:ϵͳ���ڵ���������쳣������������ֻ�������
// ����:core_info���쳣�����ֳ�
// ����: _SYSEXP_RESULT_TYPE
// =============================================================================
enum _EN_EXP_DEAL_TYPE_ Exp_DebugFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct tagExpThrowPara  head;
    u32 result = EN_EXP_DEAL_IGNORE;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    ExpRecord.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    printk("Debug Fault: %s\n\r","This is a debug fault");
    result = EN_EXP_DEAL_RESET;

    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


   head.dealresult = result;
   head.para  = (u8 *)(&ExpRecord);
   head.para_len = sizeof(struct SysExceptionInfo);
   head.name = CN_HARDEXP_DECODERNAME;
   head.validflag = true;
   Exp_Throw(&head,&result);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return result;
}

// =============================================================================
// ����:ϵͳ�ڴ�����쳣����������Ҫ������memory manage �쳣��ص��쳣
// ����:core_info���쳣�����ֳ�
// ����: _SYSEXP_RESULT_TYPE
// =============================================================================
enum _EN_EXP_DEAL_TYPE_ Exp_BusFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct tagExpThrowPara  head;
    u32 ExpPC;
    u32 result = EN_EXP_DEAL_IGNORE;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    ExpRecord.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;

    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->BFSR & CN_EXP_BFSR_IBUSERR)
    {
        printk("Bus Fault: Access Instruction Err\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_PRECISERR)
    {
        //�����쳣ʱ�ĵ�ַ
        if(pg_scb_reg->BFSR & CN_EXP_BFSR_BFARVALID)
        {
            ExpPC = ((struct SysExpCoreInfo *)core_info)->spr_pc;
            printk("Bus Fault: Data Access Err At 0x%08x\n\r",ExpPC);
        }
        else
        {
            printk("Bus Fault: Data Access Err Unknown PC\n\r");
        }
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_IMPRECISERR)
    {
        printk("Bus Fault: Data Access Err\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_UNSTKERR)
    {
        printk("Bus Fault: Exception Unstack Err\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_STKERR)
    {
        printk("Bus Fault: Exception Stack Err\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else
    {
        printk("Bus Fault: %s\n\r","Unknown Fault");
        result = EN_EXP_DEAL_RESET;
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


   head.dealresult = result;
   head.para  = (u8 *)(&ExpRecord);
   head.para_len = sizeof(struct SysExceptionInfo);
   head.name = CN_HARDEXP_DECODERNAME;
   head.validflag = true;
   Exp_Throw(&head,&result);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return result;
}

// =============================================================================
// ���ܣ��˲�����Ҫ��������������Usage�쳣��Ϣ
// ������core_info,�쳣�����ֳ�
// ���� �� _SYSEXP_RESULT_TYPE
// =============================================================================
enum _EN_EXP_DEAL_TYPE_ Exp_UsageFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct tagExpThrowPara  head;
    u32 ExpPC;
    u32 result = EN_EXP_DEAL_IGNORE;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    ExpRecord.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;

    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->UFSR & CN_EXP_UFSR_UNDEFINSTR)
    {
        printk("Usage Fault: Undefined Instruction\n\r");
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_INVSTATE)
    {
        ExpPC = ((struct SysExpCoreInfo *)core_info)->spr_pc;
        printk("Usage Fault: Attempt To Enter ARM State At 0x%08x\n\r",ExpPC);
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_INVPC)
    {
        ExpPC = ((struct SysExpCoreInfo *)core_info)->spr_pc;
        printk("Usage Fault: Invalid PC 0x%08x\n\r",ExpPC);
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_NOCP)
    {
        ExpPC = ((struct SysExpCoreInfo *)core_info)->spr_pc;
        printk("Usage Fault: Attempt to Coprocessor At 0x%08x\n\r",ExpPC);
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_UNALIGNED)
    {
        ExpPC = ((struct SysExpCoreInfo *)core_info)->spr_pc;
        printk("Usage Fault: Unaligned At 0x%08x\n\r",ExpPC);
        result = EN_EXP_DEAL_RESET;
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_DIVBYZERO)
    {
        ExpPC = ((struct SysExpCoreInfo *)core_info)->spr_pc;
        printk("Usage Fault: Divide By Zero At 0x%08x\n\r",ExpPC);
        result = EN_EXP_DEAL_RESET;
    }
    else
    {
        printk("Usage Fault: %s\n\r","Unknown Fault");
        result = EN_EXP_DEAL_RESET;
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


   head.dealresult = result;
   head.para  = (u8 *)(&ExpRecord);
   head.para_len = sizeof(struct SysExceptionInfo);
   head.name = CN_HARDEXP_DECODERNAME;
   head.validflag = true;
   Exp_Throw(&head,&result);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return result;
}

// =============================================================================
// �������ܣ��˲�����Ҫ��������������ͨ���쳣��Ϣ��NVIC�Ĳ��ִ����쳣��Ϣ�ļĴ���
// ���������layer,�׳����
//           parahead���׳�ʱ�Ĳ���
//           endian, ��Ϣ�Ĵ洢��ʽ
// �����������
// ����ֵ  : true�ɹ�, falseʧ��
// =============================================================================
bool_t  Exp_HardDecoder(struct tagExpThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if((parahead == NULL) || (false == parahead->validflag))
    {
        //�Ǳ�����Ϣ���������
        printk("CPU����Ӳ��û���쳣\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->para_len)//�������Ƿ�����
        {
            //��������������������������
            mycpuinfo = (struct SysExceptionInfo *)(parahead->para);
            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//��ǰ�汾�����Խ���
            {
                printk("CPUINFO LIST:\r\n");
                printk("CPUINFO:len = 0x%08x\n\r",parahead->para_len);
                printk("CPUINFO:deal result = 0x%08x\n\r", parahead->dealresult);

                //EXP FAULT ��Ϣ����
                printk("SysExpInfo:Cortex M3/M4 General Registers:\n\r");
                printk("R0:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r0);
                printk("R1:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r1);
                printk("R2:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r2);
                printk("R3:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r3);
                printk("R4:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r4);
                printk("R5:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r5);
                printk("R6:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r6);
                printk("R7:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r7);
                printk("R8:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r8);
                printk("R9:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r9);
                printk("R10:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r10);
                printk("R11:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r11);
                printk("R12:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r12);

                printk("SysExpInfo:Cortex M3/M4 Special Registers:\n\r");
                printk("SP:0x%08x\n\r",mycpuinfo->CoreInfo.spr_sp);
                printk("LR:0x%08x\n\r",mycpuinfo->CoreInfo.spr_lr);
                printk("PC:0x%08x\n\r",mycpuinfo->CoreInfo.spr_pc);
                printk("XPSR:0x%08x\n\r",mycpuinfo->CoreInfo.spr_xpsr);

                printk("SysExpInfo:NVIC Exception Registers:\n\r");
                printk("MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.mfsr);
                printk("BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfsr);
                printk("UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);
                printk("HFSR:0x%08x\n\r",mycpuinfo->NvicInfo.hfsr);
                printk("DFSR:0x%04x\n\r",mycpuinfo->NvicInfo.dfsr);

                result = true;
            }
            else
            {
                printk("CPU Info Decode:ivalid info flag!\n\r");
                result = false; //���ܲ���һ����CPU���߰汾���޷�����
            }
        }
        else
        {
            printk("CPU Info Decode:incomplete info frame!\n\r");
            result = false;
        }
    }

    return result;
}
// =============================================================================
// ��������?�쳣���ݽ�������?�쳣Ӳ�����뺯����ע��,��ʼ���쳣ģ������
// �������?��
// �������?��
// ����ֵ     :true�ɹ�, falseʧ��
// =============================================================================
bool_t Exp_InfoDecoderInit(void)
{
    bool_t result;
    result = Exp_RegisterThrowinfoDecoder(Exp_HardDecoder,
                CN_HARDEXP_DECODERNAME);

    return result;
}

// =============================================================================
// �������ܣ��쳣��ʼ��,�����쳣������ĳ�ʼ��
// �����������ʱ�ޣ�����������
// �����������
// ����ֵ     :true�ɹ�, falseʧ��
// =============================================================================
void Exp_Init(void)
{
    __Exp_TableSet();
}
