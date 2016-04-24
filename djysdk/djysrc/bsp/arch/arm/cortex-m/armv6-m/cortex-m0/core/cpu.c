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
#include "stdint.h"
#include "core-cfg.h"
#include "stdlib.h"
#include "cortexm0.h"
#include "exception.h"

void __start_systick(void);
struct tagSystickReg volatile * const pg_systick_reg
                        = (struct tagSystickReg *)0xE000E010;

struct scb_reg volatile * const pg_scb_reg
                        = (struct scb_reg *)0xe000ed00;

struct nvic_reg volatile * const pg_nvic_reg
                        = (struct nvic_reg *)0xe000e100;

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
        Djy_SaveLastError(EN_MEM_TRIED);   //�ڴ治�㣬���ش���
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

//----����ָ��ָ����ʱ����-----------------------------------------------------
//����: ����ָ����ʱ����,ʹ�����ú��ֱ������ͱ����Ż�ѡ��y_delay_us����׼ȷ��ʱ��
//��������
//����: ��
//��ע: ��������ֲ����
//-----------------------------------------------------------------------------
void __DjySetDelay(void)
{
    u32 counter,u32_fors=10000,clksum;
    volatile u32 u32loops;


    clksum = CN_CFG_FCLK/50;              //������,������100mS
    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        pg_systick_reg->reload = clksum;
        pg_systick_reg->current = 0;
        pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        for(u32loops=u32_fors;u32loops>0;u32loops--);       //ѭ��u32_fors��
        //ֹͣ��ʱ��
        pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                                |(0<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
        counter = pg_systick_reg->current;      //��ȡѭ��u32_fors������ʱ��
        if((pg_systick_reg->ctrl & bo_systick_ctrl_countflag) != 0)
        {
            u32_fors = u32_fors>>1;     //ѭ����������
        }else
            break;      //ѭ��������û�з����жϣ�����Ҫ��
    };
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
    u32 time1;
    s64 time2;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();

    time1 = pg_systick_reg->current;
    if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
        time2 = (g_s64OsTicks+1)*CN_CFG_TICK_US;
    else
        time2 = g_s64OsTicks*CN_CFG_TICK_US
                    + ((pg_systick_reg->reload-time1)*CN_CFG_FINE_US >>16);

    Int_LowAtomEnd(atom_low);
    return time2;
}

