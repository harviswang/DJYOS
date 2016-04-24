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
//------------------------------------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//��Ȩ���� (C), 2005-2009,   lst.
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------------------------------
#ifndef __CPU_PERI_GPIO_REG__
#define __CPU_PERI_GPIO_REG__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
// ����IOģʽ
#define CN_GPIO_MODE_OD_PULLUP            0x00    //��©������
#define CN_GPIO_MODE_OD_REPEATER          0x01    //��©���м�(����Ӧ)
#define CN_GPIO_MODE_OD_NOPULL            0x02    //��©����������
#define CN_GPIO_MODE_OD_PULLDOWN          0x03    //��©������
#define CN_GPIO_MODE_NOOD_PULLUP          0x04    //�޿�©������
#define CN_GPIO_MODE_NOOD_REPEATER        0x05    //�޿�©���м�(����Ӧ)
#define CN_GPIO_MODE_NOOD_NOPULL          0x06    //�޿�©����������
#define CN_GPIO_MODE_NOOD_PULLDOWN        0x07    //�޿�©������

// GPIO ģ���API����
void GPIO_CfgPinFunc(u32 port,u32 pinnum,u32 func_no);
void GPIO_CfgPinMode(u32 port,u32 pinnum,u32 mode);
void GPIO_SetToIn(u32 port,u32 msk);
void GPIO_SetToOut(u32 port,u32 msk);
u32  GPIO_DataGet(u32 port);
void GPIO_DataOut(u32 port,u32 data);
void GPIO_SetToHigh(u32 port,u32 msk);
void GPIO_SetToLow(u32 port,u32 msk);
void GPIO_PowerOn(u32 port);
void GPIO_PowerOff(u32 port);

// GPIO�ⲿ�ж�����
void GPIO_CfgIntMode(u32 port,u32 pinnum,u32 mode);
u32 GPIO_GetIntStatus(u32 port,u32 msk,u32 mode);
void GPIO_ClearIntStatus(u32 port,u32 msk);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_GPIO_REG__*/
