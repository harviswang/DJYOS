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
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: Ϊϵͳ�ṩʱ������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-5-1
//   ����:  ������
//   �°汾��: V1.1.0
//   �޸�˵��: ������޸�
// 1. ����: 2011-10-28
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "int.h"
#include "systime.h"

s64 __DjyGetTime(void);
static sysrunningtime  fnSysRunningTime = NULL;
extern s64  g_s64OsTicks;             //����ϵͳ����ticks��

// =============================================================================
// �������ܣ�SysTimerConnect
//        Ϊϵͳ�ṩ����ʱ��
// ���������sysrunningtime
// ���������
// ����ֵ  ��
// ˵�����ú��������ϵͳ��ʼ����ʱ����ã���ز�����ϵͳ�������ٵ��ã�������͵�ǰϵͳ������ʱ������ͬ��
// =============================================================================
bool_t SysTimerConnect(sysrunningtime runtime)
{
    if(NULL != runtime)
    {
        fnSysRunningTime = runtime;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:DjyGetTime
//       ��ȡϵͳ����ʱ��
// �����������
// ���������
// ����ֵ  ����ȡ��ϵͳ����ʱ��
// ˵��������ע��ʱ��ϵͳʱ������ʱ��ϵͳ�ģ�����ʹ��Ĭ�ϵ�ʱ��ϵͳ��BSP��ֲ��ʱ��ʹ�õģ�
// =============================================================================
s64 DjyGetTime(void)
{
    if(NULL != fnSysRunningTime)
    {
        return fnSysRunningTime();
    }
    else
    {
        return __DjyGetTime();
    }
}

//----��ȡ��ǰticks-------------------------------------------------------------
//���ܣ���ȡ����ϵͳʱ��ticks��
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
s64 DjyGetTimeTick(void)
{
    s64 time;
#if (64 > CN_CPU_BITS)
    //���������ֳ�����64λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
#endif

    time = g_s64OsTicks;

#if (64 > CN_CPU_BITS)
    //���������ֳ�����64λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    Int_LowAtomEnd(atom_low);
#endif
    return time;
}

