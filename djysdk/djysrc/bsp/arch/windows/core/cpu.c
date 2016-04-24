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
//3. ����:2013-2-7
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������__asm_reset_switch������һ�����󣬸�bug�������ύ
//1. ����: 2012-10-20
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
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <process.h>
#include <stdio.h>
//#pragma comment(lib,"Winmm.lib")
//#pragma comment (lib,"ntdll_32.lib")       // Copy From DDK  64λϵͳ��Ϊntdll_64.lib

struct WinHeap
{
    u32 CessionNum;
    u32 HeapAlign;
    u32 HeapBottom;
    u32 HeapTop;
    u32 PageSize;
    u8  name[5];
};
struct WinHeap pHeapList;

//struct WinHeap *pHeapList;             //�ڽű��ж���

extern ufast_t ufg_int_number;

extern u32  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed = 100; //for(i=j;i>0;i--);ÿѭ��������*1.024
s64 s64g_freq;
extern struct  tagEventECB  *g_ptEventReady;      //��������ͷ
extern struct  tagEventECB  *g_ptEventRunning;    //��ǰ����ִ�е��¼�
u8 DjyosHeap[0x1000000];   //16M�ڴ�
u32 WINAPI win_engine( LPVOID lpParameter );

u32 WINAPI win32_switch( LPVOID lpParameter );

HINSTANCE hInstance = NULL;

extern HANDLE win32_int_event;
extern HANDLE win32_int_engine;
HANDLE old_id,new_id;

u32 switch_context(ufast_t line)
{
    SuspendThread(old_id);
    ResumeThread(new_id);
    Int_ContactAsynSignal();
    return 0;
}

void main(int argc, char *argv[])
{
    MSG msg;
    LARGE_INTEGER litmp;

    SetProcessAffinityMask(GetCurrentProcess(), 1);
    hInstance = GetModuleHandle(NULL);

    pHeapList.CessionNum = 1;
    pHeapList.HeapAlign = 0;
    pHeapList.HeapBottom = DjyosHeap;
    pHeapList.HeapTop = pHeapList.HeapBottom + sizeof(DjyosHeap);
    pHeapList.PageSize = 128;
    strcpy(pHeapList.name,"sys");
    pHeapList.name[4] = '\0';
//  a = CreateDirectory(_TEXT("djyfs"),NULL);
//  a = CreateDirectory(_TEXT("djyfs\\sys"),NULL);
    Heap_StaticModuleInit(0);
    Int_Init();
    QueryPerformanceFrequency(&litmp);
    s64g_freq = litmp.QuadPart;
    Int_IsrConnect(cn_int_line_switch_context,switch_context);
    Int_SettoAsynSignal(cn_int_line_switch_context); //tick�жϱ���Ϊ�첽�ź�
    Int_RestoreAsynLine(cn_int_line_switch_context);
//    printf("VC �������汾��%d\n\r",_MSC_VER);
    Sys_Start();
// ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return ;
}


ATOM RegisterWinClass(HINSTANCE hInstance,WNDPROC *func,TCHAR *title)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)func;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = title;
    wcex.hIconSm        = NULL;

    return RegisterClassEx(&wcex);


}

//----�����߳�-----------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ���α���������ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������̵߳������ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct  tagThreadVm *__CreateThread(struct  tagEventType *evtt,u32 *stack_size)
{
    struct  tagThreadVm  *result;
    //stack_size��cn_kernel_stack��windows�汾�в�û���õ���windows���߳�ջ���Լ�ά��
    //�ģ�����д�ȽϽӽ�ʵ��cpuƽ̨��
    *stack_size = evtt->stack_size+sizeof(struct  tagThreadVm);
    result=(struct  tagThreadVm  *)__MallocStack(*stack_size);
    if(NULL == result)
    {
        return result;
    }
    else
    {
        memset(result,0,sizeof(struct tagThreadVm));
    }
    result->stack_top = CreateThread( NULL, 0, win_engine, evtt->thread_routine,
                                        CREATE_SUSPENDED, NULL );
//    SetThreadAffinityMask( result->stack_top, 1 );

    result->stack = 0;
    result->next = NULL;
    result->stack_size = 0; //����ջ���
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
    ptu32_t  len;

    result = (struct  tagThreadVm  *)align_up_sys(Stack);

#if CN_CFG_STACK_FILL != 0
    memset(Stack,CN_CFG_STACK_FILL,StackSize);
#endif

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
//    len = M_CheckSize(result);

    result->stack_top = CreateThread( NULL, 0, win_engine, evtt->thread_routine,
                                        CREATE_SUSPENDED, NULL );
    result->stack = 0;
    result->next = NULL;
    result->stack_size = 0; //����ջ���
    result->host_vm = NULL;
    //��λ�̲߳������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----�����߳�-----------------------------------------------------------------
//����:  ��ʼ���̵߳�ջ���������߳�ִ�к������´����߳�ʱ������ñ�����
//����:  thread_routine�������õ��̺߳���
//      vm���߳������ָ��
//����:  ��ʼ��������ĵ�ǰջָ��
//����ԭ��:void * __asm_reset_thread(void (*thread_routine)(void),
//                                   struct  tagThreadVm  *vm);
//-----------------------------------------------------------------------------
void * __asm_reset_thread(ptu32_t (*thread_routine)(void),struct  tagThreadVm  *vm)
{
    return NULL;
}

//----��λ���̣߳��л������߳�-------------------------------------------------
//����:  ��old_vm�����������³�ʼ�����´�����״̬��Ȼ���л������̵߳�
//      �������м���ִ�С�
//��һ���ڳ�פ�ڴ��������д�����¼���ɺ󣬲���ɾ��������������븴λ
//      ���������Ȼ���л������������̣߳���ʱ�����Ҫ���ñ���������˱�������
//      ����λ���߳�����������ִ�С�
//����:  thread_routine�������õ��̺߳���
//      new_vm��Ŀ���߳�
//      old_vm������λ�߳�
//����:  ��
//����ԭ��:void __asm_reset_switch(void (*thread_routine)(void),
//                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm);
//-----------------------------------------------------------------------------
void __asm_reset_switch(ptu32_t (*thread_routine)(void),
                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)
{
    Int_HalfEnableAsynSignal();
    old_vm->stack_top = CreateThread( NULL, 0, win_engine, thread_routine,
                                        CREATE_SUSPENDED, NULL );
    old_vm->stack = 0;
    old_vm->next = NULL;
    old_vm->stack_size = 0; //����ջ���
    old_vm->host_vm = NULL;

    ResumeThread((HANDLE )new_vm->stack_top);
    Int_ContactAsynSignal();
    _endthreadex(0);
}


void __asm_start_thread(struct  tagThreadVm  *new_vm)
{
    ResumeThread(win32_int_engine);
    __asm_turnto_context(new_vm);
}

//----����������---------------------------------------------------------------
//����:  ������ԭ�����ģ�ֱ�������µ�������ִ��
//����:  new_sp���������ĵ�ջָ��
//����:  ��
//����ԭ��: void __asm_turnto_context(struct  tagThreadVm  *new_vm);
//˵��:  ���¼����,��û�б�Ҫ������¼���������,ֱ���л������¼�����.
//-----------------------------------------------------------------------------
void __asm_turnto_context(struct  tagThreadVm  *new_vm)
{
    Int_HalfEnableAsynSignal();
    ResumeThread((HANDLE )new_vm->stack_top);
    Int_ContactAsynSignal();
}

//----�������л�---------------------------------------------------------------
//����:  ���浱ǰ�̵߳������ģ��л������̵߳��������м���ִ�С�
//����:  new_sp���������ĵ�ջָ��
//����:  old_sp���������ĵ�ջָ���ָ�룬��&vm->stack�������ṩ��������ջָ�룬
//               sp�Ĵ����ĵ�ǰֵ����
//����:  ��
//����ԭ��: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm);
//-----------------------------------------------------------------------------
void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)
{
    old_id = (HANDLE )old_vm->stack_top;
    new_id = (HANDLE )new_vm->stack_top;
    ufg_int_number = cn_int_line_switch_context;
    SetEvent(win32_int_event);

}

//----�ж��е��������л�-------------------------------------------------------
//����:  ���汻�ж��̵߳������ģ��л������̵߳��������м���ִ�С���������Ȼ����
//      �Ϸ�����(���û���int_isr_connect�������ӵ��жϷ�����)�У�����ARM
//      �У�ȴ������svc̬
//����:  new_sp���л�Ŀ�������
//����:  old_sp�����ж��߳������
//����:  ��
//����ԭ��: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm);
//-----------------------------------------------------------------------------
void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)
{
    SuspendThread((HANDLE )old_vm->stack_top);
    ResumeThread((HANDLE )new_vm->stack_top);
}

u32 WINAPI win_engine( LPVOID lpParameter )
{
    void (*thread_routine)(void);

    thread_routine = (void (*)(void)) lpParameter;
    __Djy_VmEngine(thread_routine);
    return 0;
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
//x86 ��QueryPerformanceFrequency������QueryPerformanceCounter��������10000��for
//ѭ��ʱ�䣬Ȼ����㡣����������ʹ�÷�����
#if 0
�ڽ��ж�ʱ֮ǰ���ȵ���QueryPerformanceFrequency()������û����ڲ���ʱ����ʱ��Ƶ�ʣ�
Ȼ������Ҫ�ϸ�ʱ���¼�����֮ǰ�ͷ���֮��ֱ����QueryPerformanceCounter()������
�������λ�õļ���֮�ʱ��Ƶ�ʣ�������¼��� ���ľ�ȷʱ�䡣���д���ʵ��1ms��
��ȷ��ʱ��
#endif

}

void CALLBACK TimerCallBack(UINT uTimerID, UINT uMsg, DWORD dwUser,
                            DWORD dw1, DWORD dw2)
{
    Int_TapLine(cn_int_line_timer_event);
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
    Int_IsrConnect(cn_int_line_timer_event,__DjyIsrTick);
    Int_SettoAsynSignal(cn_int_line_timer_event); //tick�жϱ���Ϊ�첽�ź�
    Int_RestoreAsynLine(cn_int_line_timer_event);
    timeSetEvent(CN_CFG_TICK_US/mS,CN_CFG_TICK_US/mS,&TimerCallBack,0,TIME_PERIODIC);
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
    LARGE_INTEGER litmp;
    s64 cnt;
    s64 time;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();

    QueryPerformanceCounter(&litmp);
    cnt = litmp.QuadPart;
    time = (u32)(cnt*1000000/s64g_freq);
    Int_LowAtomEnd(atom_low);
    return time;// (((u32)cn_tick_us*1000/cn_fine_ns)-pg_timer_reg->TCNTO3) *cn_fine_ns;
}

void __asm_reset_cpu(void)
{
}
void __asm_reset_cpu_hot(void)
{
}
void restart_system(void)
{
}
void reboot(void)
{
}
void reset(void)
{
}
