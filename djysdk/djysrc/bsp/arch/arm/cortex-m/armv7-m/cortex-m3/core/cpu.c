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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "config-prj.h"
#include "align.h"
#include "stdint.h"
#include "core-cfg.h"
#include "stdlib.h"
#include "int.h"
#include "exception.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"


// void __start_systick(void);
struct tagSystickReg volatile * const pg_systick_reg
                        = (struct tagSystickReg *)0xE000E010;

extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024

//----�����߳�-----------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ����̣߳���ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������߳�ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct  tagThreadVm *__CreateThread(struct  tagEventType *evtt,u32 *stack_size)
{
    struct  tagThreadVm  *result;
    ptu32_t  len;

    //�����߳�ջ:�߳�+��󵥸�api�����ջ
    len = evtt->stack_size+CN_KERNEL_STACK+sizeof(struct  tagThreadVm);
    //ջ����Ҫ���룬malloc�����ܱ�֤ջ���Ƕ���ģ����볤�ȿ���ʹջ������
    len = align_up_sys(len);
    result=(struct  tagThreadVm  *)__MallocStack(len);
    *stack_size = len;
    if(result==NULL)
    {
//        Djy_SaveLastError(EN_MEM_TRIED);   //�ڴ治�㣬���ش���
        return result;
    }
#if CN_CFG_STACK_FILL != 0
    len = M_CheckSize(result);
    memset(result,CN_CFG_STACK_FILL,len);
#endif

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
//    len = M_CheckSize(result);
    result->stack_top = (u32*)((ptu32_t)result+len); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = len - sizeof(struct tagThreadVm); //����ջ���
    result->host_vm = NULL;
    //��λ�̲߳������߳�
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
    result->stack_top = (u32*)align_down_sys((ptu32_t)Stack+StackSize); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = (ptu32_t)(result->stack_top) - (ptu32_t)result
                            - sizeof(struct  tagThreadVm);       //����ջ���
    result->host_vm = NULL;
    //��λ�̲߳������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

static volatile ufast_t uf_delay_counter;
void SetDelayIsr(void)
{
    uf_delay_counter++;
}

//----����ָ��ָ����ʱ����-----------------------------------------------------
//����: ����ָ����ʱ����,ʹ�����ú��ֱ������ͱ����Ż�ѡ��,djy_delay_us����׼ȷ��ʱ��
//��������
//����: ��
//��ע: ��������ֲ����
//-----------------------------------------------------------------------------
void __DjySetDelay(void)
{
    uint32_t counter,u32_fors=10000,temp,clksum;
    volatile uint32_t u32loops;
    u32 backup;

    clksum = 0xFFFFFF;	//�˴�����ȡ���������ֵ��M3��M4�ں�SYSTICK��24λ������

	pg_systick_reg->reload = clksum;
	uf_delay_counter = 0;
	pg_systick_reg->current = 0;
	pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
							|(1<<bo_systick_ctrl_tickint)   //��������ж�
							|(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
	for(u32loops=u32_fors;u32loops>0;u32loops--);       //ѭ��u32_fors��

	//�˴����Խ׶���ȷ�������ݼ�����������0ʱ��˵��һ�������������޷��������ѭ��
	//���Сu32_forsֵ�������systick�жϷ����������ж��ٸ���������
	if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
	{
		while(1);
	}
	//ֹͣ��ʱ��
	pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
							|(1<<bo_systick_ctrl_tickint)   //��������ж�
							|(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
	counter = pg_systick_reg->current;      //��ȡѭ��u32_fors������ʱ��

    counter = clksum - counter;    //ȡʵ����������
    g_u32CycleSpeed = (uint64_t)counter * 10E8/CN_CFG_FCLK/u32_fors;//���������64λ
    g_u32CycleSpeed = (g_u32CycleSpeed << 10) / 1000;     //����1.024��
}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __DjyInitTick(void)
{
    Exp_ConnectSystick(Djy_IsrTick);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                            |(1<<bo_systick_ctrl_tickint)   //��������ж�
                            |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��rtc.c�ļ��� DjyGetTime ��������
//-----------------------------------------------------------------------------
s64 __DjyGetTime(void)
{
    s64 time;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();

    time = (pg_systick_reg->reload - pg_systick_reg->current);
    time = g_s64OsTicks*CN_CFG_TICK_US + (time*CN_CFG_FINE_US >>16);

    Int_LowAtomEnd(atom_low);
    return time;
}

void reset(void)
{
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
}
extern void reboot(void)
{
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
}
extern void restart_system(void)
{
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
}

