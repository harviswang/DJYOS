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
// ģ������: ΪDJYOS׼��CPU���Ĺ��ܣ��̵߳��л���
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:zhangqf_cyg
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�����ļ��Ķ���ӿڶ�����ֲ���к���
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

#include "os.h"

#include "config-prj.h"
#include "E500v2RegBits.h"
extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024
void Exp_ConnectSystick(void (*tick)(void));
extern void __asm_init_Dtimer(u32 value_tick);
extern void __asm_get_tb_value(u32 *value_h32, u32 *value_l32);
extern void __asm_reset_the_tb(void);
extern ptu32_t __asm_reset_thread(ptu32_t (*thread_routine)(void),
                           struct  tagThreadVm  *vm);


// =============================================================================
// ��������:__CreateThread
//          �����߳�
// �������:evtt���̹߳�����evtt
// �������:stack_size��������VM��stack�Ĵ�С
// ����ֵ  :
// ˵��    :��evtt�е����ݻָ���������VM����
// =============================================================================
struct  tagThreadVm *__CreateThread(struct  tagEventType *evtt,u32 *stack_size)
{
    struct  tagThreadVm  *result;
    ptu32_t  len;

    //���������ջ:�߳�+��󵥸�api�����ջ
    len = evtt->stack_size+CN_KERNEL_STACK+sizeof(struct  tagThreadVm);
    //ջ����Ҫ���룬malloc�����ܱ�֤ջ���Ƕ���ģ����볤�ȿ���ʹջ������
    len = align_up_sys(len);
    result=(struct  tagThreadVm  *)(__MallocStack(len));
    *stack_size = len;
    if(result==NULL)
    {
        Djy_SaveLastError(EN_MEM_TRIED);   //�ڴ治�㣬���ش���
        return result;
    }
#if CN_CFG_STACK_FILL != 0
    memset(result,CN_CFG_STACK_FILL,len);
#endif
    result->stack_top = (u32*)((ptu32_t)result+len); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = len - sizeof(struct tagThreadVm); //����ջ���
    result->host_vm = NULL;
    //��λ������������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----��̬�����߳�-------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ����̣߳���ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������߳�ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct  tagThreadVm *__CreateStaticThread(struct  tagEventType *evtt,void *Stack,
                                    u32 StackSize)
{
    struct  tagThreadVm  *result;

    result = (struct  tagThreadVm  *)align_up_sys(Stack);

#if CN_CFG_STACK_FILL != 0
    memset(Stack,CN_CFG_STACK_FILL,StackSize);
#endif

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
//    len = M_CheckSize(result);
    result->stack_top = (u32 *)(align_down_sys((ptu32_t)Stack+StackSize)); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = (ptu32_t)(result->stack_top) - (ptu32_t)result
                            - sizeof(struct  tagThreadVm);       //����ջ���
    result->host_vm = NULL;
    //��λ�̲߳������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}
// =============================================================================
// ��������:__DjySetDelay
//          ����forѭ���ٶ�
// �������:��
// �������:��
// ����ֵ  :
// ˵��    :
// =============================================================================
void __DjySetDelay(void)
{
    uint32_t counter_h,counter_l;
    volatile uint32_t u32loops;
    atom_high_t high_atom;
    //maybe we need to close the int key
    high_atom = Int_HighAtomStart();//dont be interrupt by something
    __asm_reset_the_tb();

    for(u32loops=0;u32loops<10240;u32loops++);

    __asm_get_tb_value(&counter_h, &counter_l);

    Int_HighAtomEnd(high_atom);
    //actually, the counter_l is enough

    //counter is the time run during the u32_fors loops
    //use 64 bits to avoid overflow
    g_u32CycleSpeed = counter_l*cfg_core_tb_clk_time;//ns

    g_u32CycleSpeed = g_u32CycleSpeed/10000;//ÿ����Ҫ��������

    return;
}
// =============================================================================
// ��������:__DjyIsrTick
//          tick��isr
// �������:��
// �������:��
// ����ֵ  :
// ˵��    :�ú�����Ҫ���������delay���еļ���
// =============================================================================
void __DjyIsrTick(void)
{
    //todo: �Ż�ʱ����ʵ��ticks��
    Djy_IsrTick( 1 );
}

// =============================================================================
// ��������:__DjyInitTick
//          tick�ĳ�ʼ��
// �������:��
// �������:��
// ����ֵ  :
// ˵��    :tickʹ�õ������ǵĵݼ���������E500V2��
// =============================================================================
void __DjyInitTick(void)
{
    //here timer is not in the external interrupt, on the other hand it will
    //send an timer exception.
    //better to the asm to complete this
    //compute the value that used for the tick time
    u32 counter_value;

    //ʹ�õ���CORE implete clk, ccb/8
    counter_value =cfg_core_tb_clk/CN_CFG_TICK_HZ;//compute the count number

    Exp_ConnectSystick(__DjyIsrTick);//connect to the tick server

    __asm_init_Dtimer(counter_value);

    return;

    //at the same time we need to init the tick timer
}
//=============================================================================
//����:��ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//     g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//     ����,��Ҫʹ��ԭ�Ӳ�����
//     ��ʱ��ΪTBUTBL����ɵ�S64ʱ�䣬��ʱ����ϵ翪ʼ���㣬��TICK����ȷ
//����:��
//���:��
//����:64λʱ��
//˵��: ����һ��׮����,��rtc.c�ļ��� DjyGetTime ��������
// =============================================================================
s64 __DjyGetTime(void)
{
//PPC use the TB as the 64 bits time, so it could lasts many many years when
//it turns back
    u32 time_high;
    u32 time_low;
    u64 time_need;
    u32 time_div;
    
    __asm_get_tb_value(&time_high, &time_low);

    time_need = ((u64)time_high<<32)|(time_low);
    time_div = cfg_core_tb_clk/1000/1000;

    time_need = time_need/time_div;
    
    return time_need;
}


// =============================================================================
// ��������:LockSysCode
//          ϵͳ�����д����
// �������:��
// �������:��
// ����ֵ  :
// ˵��    :P1020��ǰ64MBΪ����Σ��ڴ���д����
// =============================================================================
void LockSysCode(void)
{
	u32 property;
	//protect the mem code from overwriting
	property = __AsmGetTlbMas3Pro(CN_DDR_CODEADDR);
	property = property &(~MAS3_SW);
	__AsmSetTlbMas3Pro(property, CN_DDR_CODEADDR);

	return;
}


