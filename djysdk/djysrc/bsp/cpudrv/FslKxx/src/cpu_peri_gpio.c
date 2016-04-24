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
// �ļ���     ��cpu_peri_gpio.c
// ģ������: GPIOģ��ĳ�ʼ���Ĳ������������������ȡ��������õ�
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 07/06.2014
// =============================================================================

#include "cpu_peri.h"

static GPIO_Type volatile * const PTR_GPIO_REG[] = {(GPIO_Type *)PTA_BASE,
                                            (GPIO_Type *)PTB_BASE,
                                            (GPIO_Type *)PTC_BASE,
                                            (GPIO_Type *)PTD_BASE,
                                            (GPIO_Type *)PTE_BASE,
                                            (GPIO_Type *)PTF_BASE};

// =============================================================================
// ���ܣ�����GPIO�˿����ŵĹ���Ϊ���������������øù���ǰ����Ӧ��port�ڶ�Ӧ������Ӧ
//       �Ѿ�����ΪGPIO���ܣ����ù�����portģ��ʵ��
// ������port,���õĶ˿ںţ�PORTA~PORTF�ֱ��Ӧ�궨��GPIO_PORT_A~GPIO_PORT_F
//       msk,��Ӧ�˿��϶�Ӧ�����ţ��ɺ�GPIO_PIN(X)ʵ�֣�xΪ���ź�
//       func_no,�����������ܣ���GPIO_FUNC_INPUT/GPIO_FUNC_OUTPUT
// ���أ�NONE
// =============================================================================
void GPIO_PinConfig(u32 port,u32 msk,u32 func_no)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    //������Ӧ��port�ڶ�Ӧ������ΪGPIO����---todo

    if(func_no == GPIO_FUNC_OUTPUT)
    {
        PTR_GPIO_REG[port]->PDDR |= (1<<msk);
    }
    else if(func_no == GPIO_FUNC_INPUT)
    {
        PTR_GPIO_REG[port]->PDDR &= ~(1<<msk);
    }
    else
    {
        return;
    }
}

// =============================================================================
// ���ܣ���port����Ϊ����ʱ����ͨ����API��ȡ�����ϵ���ֵ������port����������ֵ
// ������port,���õĶ˿ںţ�PORTA~PORTF�ֱ��Ӧ�궨��GPIO_PORT_A~GPIO_PORT_F
// ���أ�port�˿��ϵ���ֵ
// =============================================================================
u32  GPIO_DataGet(u32 port)
{
    if(port > GPIO_PORT_MAX)
        return 0;
    return PTR_GPIO_REG[port]->PDIR;
}

// =============================================================================
// ���ܣ��˿�����Ϊ���ʱ������ֵ������˿���
// ������port,���õĶ˿ںţ�PORTA~PORTF�ֱ��Ӧ�궨��GPIO_PORT_A~GPIO_PORT_F
//       data,���뵽�˿��ϵ�ֵ
// ���أ���
// =============================================================================
void GPIO_DataOut(u32 port,u32 data)
{
    if(port > GPIO_PORT_MAX)
        return;

    PTR_GPIO_REG[port]->PDOR = data;
}

// =============================================================================
// ���ܣ������������Ϊ1
// ������port,���õĶ˿ں�
//       msk,��Ӧ�˿��϶�Ӧ������
// ���أ�ʵ�����õ�MCG���Ƶ��
// =============================================================================
void GPIO_PinSet(u32 port,u32 msk)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    PTR_GPIO_REG[port]->PSOR |= (1<<msk);
}

// =============================================================================
// ���ܣ�����������Ϊ0
// ������port,���õĶ˿ں�
//       msk,��Ӧ�˿��϶�Ӧ������
// ���أ���
// =============================================================================
void GPIO_PinClear(u32 port,u32 msk)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    PTR_GPIO_REG[port]->PCOR |= (1<<msk);
}

// =============================================================================
// ���ܣ���ת������ŵĵ�ƽ
// ������port,���õĶ˿ں�
//       msk,��Ӧ�˿��϶�Ӧ������
// ���أ���
// =============================================================================
void GPIO_PinToggle(u32 port,u32 msk)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    PTR_GPIO_REG[port]->PTOR |= (1<<msk);
}

// =============================================================================
// ���ܣ�����ʱ���������Ӧ�Ķ˿���
// ������port,���õĶ˿ں�
// ���أ���
// =============================================================================
void GPIO_PowerOn(u32 port)
{
//  if(port > GPIO_PORT_MAX)
//      return;
//  switch(port)
//  {
//  case GPIO_PORT_A:
//      SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK ;
//      break;
//  case GPIO_PORT_B:
//      SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;
//      break;
//  case GPIO_PORT_C:
//      SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK ;
//      break;
//  case GPIO_PORT_D:
//      SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK ;
//      break;
//  case GPIO_PORT_E:
//      SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;
//      break;
//  case GPIO_PORT_F:
//      SIM_SCGC5 |= SIM_SCGC5_PORTF_MASK ;
//      break;
//  default:
//      break;
//  }
}

// =============================================================================
// ���ܣ�����ʱ�Ӳ��������Ӧ�Ķ˿��ϣ����ڵ͹������
// ������port,���õĶ˿ں�
// ���أ���
// =============================================================================
void GPIO_PowerOff(u32 port)
{
//  if(port > GPIO_PORT_MAX)
//      return;
//  switch(port)
//  {
//  case GPIO_PORT_A:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTA_MASK ;
//      break;
//  case GPIO_PORT_B:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTB_MASK ;
//      break;
//  case GPIO_PORT_C:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTC_MASK ;
//      break;
//  case GPIO_PORT_D:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTD_MASK ;
//      break;
//  case GPIO_PORT_E:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTE_MASK ;
//      break;
//  case GPIO_PORT_F:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTF_MASK ;
//      break;
//  default:
//      break;
//  }
}
