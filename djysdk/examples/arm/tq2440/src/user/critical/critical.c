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
//����ģ��:��ȫ����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:��ȫ�����Ǳ����ڿ����󾡿��ܿ��ִ�еĲ��ִ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "int.h"
#include "cpu_peri.h"

//�˱����ù������õ����ж�Դ,ÿ��ʹ��һ���ж�Դ,�ں���Ҫ��ռһ��struct tagIntLine
//�ṹ(32�ֽ�)���ڴ�.�����ڴ����ϵͳ,�����ڰ�cpu_peri_int_line.h�ļ�����
//�����ж�Դȫ��copy������,�ڴ��ѷ���ϵͳ��ֻ���õ����ж�Դ�г�����
const ufast_t tg_IntUsed[] =
{
	cn_int_line_eint0,
	cn_int_line_eint1,
	cn_int_line_eint2,
	cn_int_line_eint3,
	cn_int_line_eint4_7,
	cn_int_line_eint8_23,
	cn_int_line_resv6,
	cn_int_line_bat_flt,
	cn_int_line_tick,
	cn_int_line_wdt,
	cn_int_line_timer0,
	cn_int_line_timer1,
	cn_int_line_timer2,
	cn_int_line_timer3,
	cn_int_line_timer4,
	cn_int_line_uart2,
	cn_int_line_lcd,
	cn_int_line_dma0,
	cn_int_line_dma1,
	cn_int_line_dma2,
	cn_int_line_dma3,
	cn_int_line_sdi,
	cn_int_line_spi0,
	cn_int_line_uart1,
	cn_int_line_resv24,
	cn_int_line_usbd,
	cn_int_line_usbh,
	cn_int_line_iic,
	cn_int_line_uart0,
	cn_int_line_spi1,
	cn_int_line_rtc,
	cn_int_line_adc,

};

const ufast_t tg_IntUsedNum = sizeof(tg_IntUsed)/sizeof(ufast_t);

//----����ȫ��IO��--------------------------------------------------------------
//���ܣ���������⣬ȫ�����óɳ�ʼ̬�������ܿ�����Ӧ�Ĺ���ģ�����ж��塣
//��������
//���أ���
//------------------------------------------------------------------------------
void gpio_init(void)
{
}

extern void Timer_ModuleInit(void);
extern bool_t WWDG_STM32Init(u32 setcycle);
bool_t WdtHal_BootStart(u32 bootfeedtimes);
void critical(void)
{
    //�����Ҫ������������ι��,������ڳ�ʼ�����Ź�֮ǰ,��ʼ����ʱ��Ӳ��
//    Timer_ModuleInit();     //todo:������������
    //��ʼ��Ӳ�����Ź�,��Ҫ����������ι���Ļ�,�����ڴ˳�ʼ��.
//    WWDG_STM32Init(1000);
    // ����ʵʩ�ж�+��ʱ��ι������,���������������,Sys_ModuleInit�����е���
    // WdtHal_BootEnd�Ǳ����,�������ע�͵�.
//    WdtHal_BootStart(20);
}
