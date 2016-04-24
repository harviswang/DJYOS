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
// �ļ���     ��cpu_peri_port.c
// ģ������: �˿�ģ�����ã��˿��ж����ú�ʹ��
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 07/06.2014
// =============================================================================

#include "cpu_peri.h"

static PORT_Type volatile * const PTR_PORT_REG[] = {(PORT_Type *)0x40049000u,
                                            (PORT_Type *)0x4004A000u,
                                            (PORT_Type *)0x4004B000u,
                                            (PORT_Type *)0x4004C000u,
                                            (PORT_Type *)0x4004D000u,
                                            (PORT_Type *)0x4004E000u};

// =============================================================================
// ���ܣ��˿��������ù������ã����ݲ�ͬ������Զ˿ڵ�����mux����Ϊ��ͬ��ֵ������ֵ
//       ��鿴�û��ֲ�ÿ��ģ��Զ˿ڵ�Ҫ��
// ������port,���õĶ˿ںţ�PORT_PORT_A~F
//       pin,���ţ�PORT_PIN(x),x��0~31
//       mux,�����ģ����أ�������ΪGPIO���ܣ���ΪPORT_PINMUX_GPIO
// ���أ�NONE
// =============================================================================
void PORT_MuxConfig(u32 port,u32 pin,u8 mux)
{
    u32 temp;
    if((port > PORT_PORT_MAX) || (pin > PORT_PIN_MAX))
        return;

    temp = PTR_PORT_REG[port]->PCR[pin];
    temp &= (u32)~0x700u;                           //clear specific bits
    PTR_PORT_REG[port]->PCR[pin] = temp | (mux << 8);//write mux to register
}

// =============================================================================
// ���ܣ��ж����ã�����������ʽ���ڲ������������õ�,���øú���ʵ�ʾ͵�ͬ��ʹ�����ж�
// ������port,���õĶ˿ںţ�PORT_PORT_A~F
//       pin,���ţ�PORT_PIN(x),x��0~31
//       trig,������ʽ����Ϊ���ش�����ˮƽ���������ط�Ϊ�ϡ��·�ʽ
//       pull,��������ʽ
// ���أ�0���жϱ�־����0���жϱ�־
// =============================================================================
void PORT_IntConfig(u32 port,u32 pin,u8 trig,u8 pull)
{
    u32 temp;
    if((port > PORT_PORT_MAX) || (pin > PORT_PIN_MAX))
        return;

    temp = PTR_PORT_REG[port]->PCR[pin];
    //configure int
    temp &= (u32)~0xF0000u;                             //clear specific bits
    PTR_PORT_REG[port]->PCR[pin] = temp | (trig << 16); //write trig to register

    //pull
    if(pull == PORT_PULL_NONE)
    {
        PTR_PORT_REG[port]->PCR[pin] &= (u32)~0x02u;
    }
    else if(pull == PORT_PULL_UP)
    {
        PTR_PORT_REG[port]->PCR[pin] |= 0x02u;
        PTR_PORT_REG[port]->PCR[pin] |= 0x01u;
    }
    else if(pull == PORT_PULL_DOWN)
    {
        PTR_PORT_REG[port]->PCR[pin] |= 0x02u;
        PTR_PORT_REG[port]->PCR[pin] &= (u32)~0x01u;
    }
    else
    {
        return;
    }
}

// =============================================================================
// ���ܣ���ָֹ���жϣ�ʹ�ж�����ʧ��
// ������port,���õĶ˿ںţ�PORT_PORT_A~F
//       pin,���ţ�PORT_PIN(x),x��0~31
// ���أ���
// =============================================================================
void PORT_IntDisable(u32 port, u32 pin)
{
    if((port > PORT_PORT_MAX) || (pin > PORT_PIN_MAX))
        return;

    PTR_PORT_REG[port]->PCR[pin] &= (u32)(~0xF0000u);
}

// =============================================================================
// ���ܣ����ָ���жϱ�־λ����ָ���Ĵ�����־λд1���ж��˳�ʱ�������־
// ������port,���õĶ˿ںţ�PORT_PORT_A~F
//       pin,���ţ�PORT_PIN(x),x��0~31
// ���أ���
// =============================================================================
void PORT_IntFlagClear(u32 port, u32 pin)
{
    if((port > PORT_PORT_MAX) || (pin > PORT_PIN_MAX))
        return;

    PTR_PORT_REG[port]->PCR[pin] |= (u32)(1<<24);
}

// =============================================================================
// ���ܣ����жϱ�־λ��ÿһ�����Ŷ���һ����Ӧ���жϱ�־λ���ɻ���ж��Ƿ���
// ������port,���õĶ˿ںţ�PORT_PORT_A~F
//       pin,���ţ�PORT_PIN(x),x��0~31
// ���أ�0���жϱ�־����0���жϱ�־
// =============================================================================
u8   PORT_IntStatusGet(u32 port,u32 pin)
{
    if((port > PORT_PORT_MAX) || (pin > PORT_PIN_MAX))
        return 0;

    if(PTR_PORT_REG[port]->ISFR & (1<<pin))
    {
        return 1;
    }else
    {
        return 0;
    }
}
