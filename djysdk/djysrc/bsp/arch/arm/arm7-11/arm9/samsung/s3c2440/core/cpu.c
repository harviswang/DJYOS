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
#include "djyos.h"
#include "cpu.h"
#include "cpu_peri.h"

void __asm_reset_thread(void (*thread_routine)(void),struct tagThreadVm *vm);

extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024

//tickѡ��t64-0��t12

//----�����߳�-----------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ���α���������ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������̵߳������ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct  tagThreadVm *__CreateThread(struct  tagEventType *evtt,u32 *stack_size)
{
    struct  tagThreadVm  *result;
    ptu32_t  len;

    //���������ջ:�߳�+��󵥸�api�����ջ
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
    result->stack_top = align_down_sys((ptu32_t)Stack+StackSize); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = (ptu32_t)(result->stack_top) - (ptu32_t)result
                            - sizeof(struct  tagThreadVm);       //����ջ���
    result->host_vm = NULL;
    //��λ�̲߳������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

static volatile ufast_t uf_delay_counter;
u32 SetDelayIsr(ufast_t timer_int_line)
{
    uf_delay_counter++;
    return 0;
}

//----����ָ����ʱ����---------------------------------------------------------
//����: ����ָ����ʱ����,ʹy_delay_10us��������ʱ����=10uS�������ú��ֱ�������
//      �����Ż�ѡ��
//��������
//����: ��
//��ע: ��������ֲ����
//-----------------------------------------------------------------------------
void __DjySetDelay(void)
{
    u32 counter,u32_fors=10000,temp,phase_ns;
    uint16_t clksum;
    volatile u32 u32loops;

    Timer_SetClkSource(3,0);         //pclkԤ��Ƶ����1/2��Ƶ
    Timer_SetPrecale(1,7);            //ʱ������Ƶ��pclk��8��Ƶ
    Timer_SetType(3,1);                //�Զ�������������
    phase_ns = 10E8/CN_CFG_TIMER_CLK * 16;  //ÿ�����ns��
    clksum = (uint16_t)(CN_CFG_TIMER_CLK/2000);     //1000uS��������

    Int_IsrConnect(cn_int_line_timer3,SetDelayIsr);
    Int_SettoReal(cn_int_line_timer3);          //timer3�жϱ���Ϊʵʱ�ж�
    Int_EnableRealLine(cn_int_line_timer3);
    while(1)  //����32λ����ѭ��ʱ��(nS)
    {
        Timer_SetCounter(3,clksum);
        uf_delay_counter = 0;
        Timer_Reload(3);
        Timer_Start(3);
        for(u32loops=u32_fors;u32loops>0;u32loops--); //ѭ��u32_fors��
        Timer_Stop(3);
        counter = Timer_Read(3);                  //��ȡѭ��u32_fors������ʱ��
        if(uf_delay_counter != 0)
        {
            temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (uint16_t)temp;
            }else
            {
                clksum = 65536;
                u32_fors = u32_fors / (temp / 65536 +1);
            }
        }
        else
        {
            break;      //ѭ��������û�з����жϣ�����Ҫ��
        }
    };
    counter = clksum - counter;    //ȡʵ����������
    g_u32CycleSpeed = (uint64_t)counter * phase_ns /u32_fors;//���������64λ

    g_u32CycleSpeed = (g_u32CycleSpeed << 10) / 1000;     //����1.024��
}

u32 __DjyIsrTick(ufast_t line)
{
    Djy_IsrTick(1);
    return 0;
}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __DjyInitTick(void)
{
    Int_CutLine(cn_int_line_timer3);
    //����timer3��Ϊtick�ж�
    Int_IsrConnect(cn_int_line_timer3,__DjyIsrTick);
    Int_SettoAsynSignal(cn_int_line_timer3);          //tick�жϱ���Ϊ�첽�ź�
    //�������ö�ʱ������������port_config.h��cn_tick_us��cn_tick_hz��
    //cn_fine_ns��cn_fine_hz�Ķ���һ��
    Timer_SetClkSource(3,0);          //pclkԤ��Ƶ����1/2��Ƶ
    Timer_SetPrecale(1,CN_CFG_TIMER_CLK/CN_CFG_FINE_HZ/2 -1);
    //���ϰѶ�ʱ������ʱ�ӵ�Ƶ����Ϊcn_fine_hz
    //Timer_SetCounter(3,(uint16_t)((u32)cn_tick_us*1000/cn_fine_ns));
    Timer_SetCounter(3, CN_CFG_FINE_HZ/CN_CFG_TICK_HZ);
    Timer_SetType(3,1);                //����tick��ʱ����������
    Timer_Reload(3);                    //���ض�ʱֵ
    Timer_Start(3);                     //������ʱ��
    Int_RestoreAsynLine(cn_int_line_timer3);//����tick�ж�S
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
    s64 time1,time2;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();

    time1 = CN_CFG_FINE_HZ/CN_CFG_TICK_HZ - pg_timer_reg->TCNTO3;
//    if(pg_timer_reg->INTCTLSTAT & 0x02)
//        time2 = (u32g_os_ticks+1)*cn_tick_us;
//    else
        time2 = g_s64OsTicks*CN_CFG_TICK_US + (time1*CN_CFG_FINE_US >>16);

    Int_LowAtomEnd(atom_low);
    return time2;
}

void reset(void)
{
    //wait for adding
}
extern void reboot(void)
{

}
extern void restart_system(void)
{

}

