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
//------------------------------------------------------
#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

//����IOģʽ
//����ģʽ���������������뻹��gpio����
//��ΪƬ���������ʱ������������ӵ�һ�������ϵģ�ֻ��ʹ��һ�����裬������
//remap���ܰ�����ӳ�䵽��������ϡ�
#define CN_GPIO_MODE_IN_ANALOG          0x00    //ģ������
#define CN_GPIO_MODE_IN_FLOATING        0x04    //��������
#define CN_GPIO_MODE_IN_PULLDOWN        0x08    //��������
#define CN_GPIO_MODE_IN_PULLUP          0x18    //��������

#define CN_GPIO_MODE_GPIO_OUT_OD_10Mhz  0x05    //gpio��©������ٶ�10Mhz
#define CN_GPIO_MODE_GPIO_OUT_OD_2Mhz   0x06    //gpio��©������ٶ�2Mhz
#define CN_GPIO_MODE_GPIO_OUT_OD_50Mhz  0x07    //gpio��©������ٶ�50Mhz
#define CN_GPIO_MODE_GPIO_OUT_PP_10Mhz  0x01    //gpio����������ٶ�10Mhz
#define CN_GPIO_MODE_GPIO_OUT_PP_2Mhz   0x02    //gpio����������ٶ�2Mhz
#define CN_GPIO_MODE_GPIO_OUT_PP_50Mhz  0x03    //gpio����������ٶ�50Mhz

#define CN_GPIO_MODE_PERI_OUT_OD_10Mhz  0x0d    //Ƭ�����迪©������ٶ�10Mhz
#define CN_GPIO_MODE_PERI_OUT_OD_2Mhz   0x0e    //Ƭ�����迪©������ٶ�2Mhz
#define CN_GPIO_MODE_PERI_OUT_OD_50Mhz  0x0f    //Ƭ�����迪©������ٶ�50Mhz
#define CN_GPIO_MODE_PERI_OUT_PP_10Mhz  0x09    //Ƭ����������������ٶ�10Mhz
#define CN_GPIO_MODE_PERI_OUT_PP_2Mhz   0x0a    //Ƭ����������������ٶ�2Mhz
#define CN_GPIO_MODE_PERI_OUT_PP_50Mhz  0x0b    //Ƭ����������������ٶ�50Mhz

#define CN_GPIO_A   0
#define CN_GPIO_B   1
#define CN_GPIO_C   2
#define CN_GPIO_D   3
#define CN_GPIO_E   4
#define CN_GPIO_F   5
#define CN_GPIO_G   6

//gpioa at 0x40010800;
//gpiob at 0x40010c00;
//gpioc at 0x40011000;
//gpiod at 0x40011400;
//gpioe at 0x40011800;
//gpiof at 0x40011c00;
//gpiog at 0x40012000;

struct tagGPIO_StReg
{
  vu32 CRL;
  vu32 CRH;
  vu32 IDR;
  vu32 ODR;
  vu32 BSRR;
  vu32 BRR;
  vu32 LCKR;
};

//afioλ���壬bo_Ϊλƫ�ƣ�1λ����λ����ַ��bb_ǰ׺����λ�����룬bm_ǰ׺
#define bb_afio_mapr_base           (0x42000000 + 0x10000*32)        //λ����ַ
#define bb_afio_mapr_spi1           (*(vu32*)(bb_afio_mapr_base+4*0))
#define bb_afio_mapr_i2c1           (*(vu32*)(bb_afio_mapr_base+4*1))
#define bb_afio_mapr_usart1         (*(vu32*)(bb_afio_mapr_base+4*2))
#define bb_afio_mapr_usart2         (*(vu32*)(bb_afio_mapr_base+4*3))
#define bm_afio_mapr_usart3         0x00000030
#define bm_afio_mapr_tim1           0x000000c0
#define bm_afio_mapr_tim2           0x00000300
#define bm_afio_mapr_tim3           0x00000c00
#define bb_afio_mapr_tim4           (*(vu32*)(bb_afio_mapr_base+4*12))
#define bm_afio_mapr_can            0x00006000
#define bb_afio_mapr_pd01           (*(vu32*)(bb_afio_mapr_base+4*15))
#define bb_afio_mapr_tim5ch4        (*(vu32*)(bb_afio_mapr_base+4*16))
#define bb_afio_mapr_adc1_etrginj   (*(vu32*)(bb_afio_mapr_base+4*17))
#define bb_afio_mapr_adc1_etrgreg   (*(vu32*)(bb_afio_mapr_base+4*18))
#define bb_afio_mapr_adc2_etrginj   (*(vu32*)(bb_afio_mapr_base+4*19))
#define bb_afio_mapr_adc2_etrgreg   (*(vu32*)(bb_afio_mapr_base+4*20))
#define bm_afio_mapr_swj_cfg        0x07000000
#define bo_afio_mapr_spi1           0
#define bo_afio_mapr_i2c1           1
#define bo_afio_mapr_usart1         2
#define bo_afio_mapr_usart2         3
#define bo_afio_mapr_usart3         4
#define bo_afio_mapr_tim1           6
#define bo_afio_mapr_tim2           8
#define bo_afio_mapr_tim3           10
#define bo_afio_mapr_tim4           12
#define bo_afio_mapr_can            13
#define bo_afio_mapr_pd01           15
#define bo_afio_mapr_tim5ch4        16
#define bo_afio_mapr_adc1_etrginj   17
#define bo_afio_mapr_adc1_etrgreg   18
#define bo_afio_mapr_adc2_etrginj   19
#define bo_afio_mapr_adc2_etrgreg   20
#define bo_afio_mapr_swj_cfg        24
struct tagAFIO_StReg     //at 0x40010000;
{
  vu32 EVCR;
  vu32 MAPR;
  vu32 EXTICR[4];
} ;

extern struct tagGPIO_StReg volatile * const pg_gpio_rega;
extern struct tagGPIO_StReg volatile * const pg_gpio_regb;
extern struct tagGPIO_StReg volatile * const pg_gpio_regc;
extern struct tagGPIO_StReg volatile * const pg_gpio_regd;
extern struct tagGPIO_StReg volatile * const pg_gpio_rege;
extern struct tagGPIO_StReg volatile * const pg_gpio_regf;
extern struct tagGPIO_StReg volatile * const pg_gpio_regg;
extern struct tagAFIO_StReg volatile * const pg_afio_reg;
extern struct tagGPIO_StReg volatile * const pg_gpio_reg[];
void GPIO_CfgPinFunc(u32 port,u32 pinnum,u32 func_no);
u32 GPIO_GetData(u32 port);
void GPIO_OutData(u32 port,u32 data);
void GPIO_SettoHigh(u32 port,u32 msk);
void GPIO_SettoLow(u32 port,u32 msk);
void GPIO_PowerOn(u32 port);
void GPIO_PowerOff(u32 port);

#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
