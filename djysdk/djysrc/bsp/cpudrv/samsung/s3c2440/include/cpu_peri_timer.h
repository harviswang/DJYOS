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
//����ģ��: Ӳ����ʱ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: Ӳ����ʱ������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_TIMER_H__
#define __CPU_PERI_TIMER_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tagTimerReg
{
    vu32 TCFG0;     //0x51000000
    vu32 TCFG1;     //0x51000004
    vu32 TCON;      //0x51000008
    vu32 TCNTB0;    //0x5100000C
    vu32 TCMPB0;    //0x51000010
    vu32 TCNTO0;    //0x51000014
    vu32 TCNTB1;    //0x51000018
    vu32 TCMPB1;    //0x5100001C
    vu32 TCNTO1;    //0x51000020
    vu32 TCNTB2;    //0x51000024
    vu32 TCMPB2;    //0x51000028
    vu32 TCNTO2;    //0x5100002C
    vu32 TCNTB3;    //0x51000030
    vu32 TCMPB3;    //0x51000034
    vu32 TCNTO3;    //0x51000038
    vu32 TCNTB4;    //0x5100003C
    vu32 TCNTO4;    //0x51000040
};

extern struct tagTimerReg volatile * const pg_timer_reg;

void Timer_SetClkSource(ufast_t timer,ufast_t value);
void Timer_SetPrecale(ufast_t group,uint16_t value);
void Timer_SetCounter(ufast_t timer,uint16_t value);
void Timer_SetCompare(ufast_t timer,uint16_t value);
void Timer_SetType(ufast_t timer,ufast_t type);
void Timer_Reload(ufast_t timer);
void Timer_Start(ufast_t timer);
void Timer_Stop(ufast_t timer);
uint16_t Timer_Read(ufast_t timer);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_TIMER_H__
