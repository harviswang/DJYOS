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
// �ļ���     ��cpu_peri_port.h
// ģ������: �˿�ģ�����ã��˿��ж����ú�ʹ��
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 07/06.2014
// =============================================================================

#ifndef CPU_PERI_PORT_H_
#define CPU_PERI_PORT_H_

//�궨�壺port��
#define PORT_PORT_A     		0
#define PORT_PORT_B     		1
#define PORT_PORT_C     		2
#define PORT_PORT_D     		3
#define PORT_PORT_E     		4
#define PORT_PORT_F     		5
#define PORT_PORT_MAX     		5

//�궨�壺���ź�
#define PORT_PIN(x)				x
#define PORT_PIN_MAX			31

//�궨�壺���Ŷ๦������ѡ��
#define PORT_PINMUX_ALTDIS		0	//Analog
#define PORT_PINMUX_GPIO        1
#define PORT_PINMUX_ALT2        2	//chip specific
#define PORT_PINMUX_ALT3        3	//chip specific
#define PORT_PINMUX_ALT4        4	//chip specific
#define PORT_PINMUX_ALT5        5	//chip specific
#define PORT_PINMUX_ALT6        6	//chip specific
#define PORT_PINMUX_ALT7        7	//chip specific / JTAG / NMI

//�궨�壺�����жϴ�������
#define IQC_INTDMA_DISABLE      0	//��ֹ�ж�/DMA����
#define IQC_DMA_RISING_EDGE		1	//DMA���������ش���
#define IQC_DMA_FALING_EDGE     2	//DMA�����½��ش���
#define IQC_DMA_EIGHER_EDGE     3	//DMA�����κα��ش���
#define IQC_INT_LEVEL_LOW		8	//INTˮƽ�͵�ƽ����
#define IQC_INT_RISING_EDGE     9	//INT�����ش���
#define IQC_INT_FALING_EDGE     10	//INT�½��ش���
#define IQC_INT_EIGHER_EDGE     11	//INT�κα��ش���
#define IQC_INT_LEVEL_HIGH      12	//INTˮƽ�ߵ�ƽ����

//�궨�壺�ڲ�������
#define PORT_PULL_NONE          0
#define PORT_PULL_UP            1
#define PORT_PULL_DOWN          2

void PORT_MuxConfig(u32 port,u32 pin,u8 mux);
void PORT_IntConfig(u32 port,u32 pin,u8 trig,u8 pull);
void PORT_IntDisable(u32 port,u32 pin);
void PORT_IntFlagClear(u32 port, u32 pin);
u8   PORT_IntStatusGet(u32 port,u32 pin);


#endif /* CPU_PERI_PORT_H_ */
