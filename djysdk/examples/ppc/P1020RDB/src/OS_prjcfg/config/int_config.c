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
//����ģ��:��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ���ļ������˾��幤�������õ��жϣ���ʹ���˵�ÿ���жϣ��������һ��
//      struct int_line�ṹ��������ڴ�Ƚϳ����ϵͳ��˵������Ϊ������������ϵ
//      ͳ����ϵͳ�����ж��߶����ϾͿ����ˡ�
//          �������ڴ�ǳ��������ж����ڶ�ĵ�Ƭ����˵�����ܸ��ж��Ƿ�ʹ�ã���
//      ����һ��struct int_line�ṹ�Ļ����ڴ��ʾ����ɽ��ʵ�ʹ��������õ�����
//      �����������࣬������ֻ��ʹ�õ��˵��ж�������һ�¾ͺá�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 20130907
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "int.h"
//this assay indicate which int line could be used by the user
//you must obey this, else due to yourself
const ufast_t tg_IntUsed[] =
{
//internal chip interrupt---64,also means the interrupt number

//external interrupt
//     cn_int_line_extbase                   (0x00)
     cn_int_line_irq0                      ,
     cn_int_line_irq1                      ,
     cn_int_line_irq2                      ,
     cn_int_line_irq3                      ,
     cn_int_line_irq4                      ,
     cn_int_line_irq5                      ,
     cn_int_line_irq6                      ,
     cn_int_line_irq7                      ,
     cn_int_line_irq8                      ,
     cn_int_line_irq9                      ,
     cn_int_line_irqa                      ,
     cn_int_line_irqb                      ,

//msg interrupt

     cn_int_line_msg0                      ,
     cn_int_line_msg1                      ,
     cn_int_line_msg2                      ,
     cn_int_line_msg3                      ,
     cn_int_line_msg4                      ,
     cn_int_line_msg5                      ,
     cn_int_line_msg6                      ,
     cn_int_line_msg7                      ,

//shared msg interrupt
     cn_int_line_smsg0                      ,
     cn_int_line_smsg1                     ,
     cn_int_line_smsg2                     ,
     cn_int_line_smsg3                    ,
     cn_int_line_smsg4                      ,
     cn_int_line_smsg5                     ,
     cn_int_line_smsg6                      ,
     cn_int_line_smsg7                      ,

//inter-processor interrupt,core0
     cn_int_line_c0_ipi0                    ,
     cn_int_line_c0_ipi1                   ,
     cn_int_line_c0_ipi2                    ,
     cn_int_line_c0_ipi3                   ,
     cn_int_line_c0_ipi4                    ,
//inter-processor interrupt,core1
     cn_int_line_c1_ipi0                    ,
     cn_int_line_c1_ipi1                    ,
     cn_int_line_c1_ipi2                    ,
     cn_int_line_c1_ipi3                    ,
     cn_int_line_c1_ipi4                    ,

//pic global timer interrupt
     cn_int_line_gbtimer_a0               ,
     cn_int_line_gbtimer_a1               ,
     cn_int_line_gbtimer_a2               ,
     cn_int_line_gbtimer_a3               ,

//     cn_int_line_gbtimer_bbase            (0x28)
     cn_int_line_gbtimer_b0               ,
     cn_int_line_gbtimer_b1               ,
     cn_int_line_gbtimer_b2               ,
     cn_int_line_gbtimer_b3,

//     CN_INT_LINE_LAST                  ,
    cn_int_line_error                     ,  //
    cn_int_line_etsec1_g1_transmit        ,  //
    cn_int_line_etsec1_g1_receive         ,  //
    cn_int_line_elbc_general_interrupt    ,  //
    cn_int_line_DMA_ch1                   ,  //
    cn_int_line_DMA_ch2                  ,  //
    cn_int_line_DMA_ch3                  , //
    cn_int_line_DMA_ch4                   , //
    cn_int_line_etsec1_g1_error           , //
    cn_int_line_etsec3_g3_transmit        ,  //
    cn_int_line_etsec3_g3_receive         , //
    cn_int_line_etsec3_g1_error           ,  //
    cn_int_line_usb1                     , //
    cn_int_line_etsec1_g0_transmit       ,  //
    cn_int_line_etsec1_g0_receive        ,  //
    cn_int_line_etsec3_g0_transmit       ,  //
    cn_int_line_etsec3_g0_receive        ,  //
    cn_int_line_etsec3_g0_error           ,  //
    cn_int_line_etsec1_g0_error           , //
    cn_int_line_etsec2_g0_transmit        ,  //
    cn_int_line_etsec2_g0_receive         ,  //
    cn_int_line_reserverd_21              ,  //
    cn_int_line_reserverd_22              ,  //
    cn_int_line_reserverd_23              , //
    cn_int_line_etsec2_g0_error           , //
    cn_int_line_reserverd_25              ,  //
    cn_int_line_duart                     ,  //
    cn_int_line_iic_controllers           ,  //
    cn_int_line_performance_monitor       , //
    cn_int_line_security_interrupt1       ,  //
    cn_int_line_usb2                      ,  //
    cn_int_line_gpio                     , //
    cn_int_line_reserverd_32             ,  //
    cn_int_line_reserverd_33             ,  //
    cn_int_line_reserverd_34             ,  //
    cn_int_line_etsec2_g1_transmit        , //
    cn_int_line_etsec2_g1_receive        , //
    cn_int_line_reserverd_37             ,  //
    cn_int_line_reserverd_38             , //
    cn_int_line_reserverd_39             , //
    cn_int_line_reserverd_40               , //
    cn_int_line_reserverd_41             , //
    cn_int_line_security_interrupt2       ,  //
    cn_int_line_espi                     ,  //
    cn_int_line_reserverd_44            ,  //
    cn_int_line_reserverd_45            ,//
    cn_int_line_TDM                     ,  //
    cn_int_line_TDM_error                ,  //
    cn_int_line_reserverd_48              ,  //
    cn_int_line_reserverd_49              , //
    cn_int_line_reserverd_50              ,  //
    cn_int_line_etsec2_g1_error           ,  //
    cn_int_line_etsec1_1588timer          , //
    cn_int_line_etsec2_1588timer          , //
    cn_int_line_etsec3_1588timer         ,  //
    cn_int_line_reserverd_55              , //
    cn_int_line_eSDHC                    ,  //
    cn_int_line_reserverd_57             ,  //
    cn_int_line_reserverd_58             , //
    cn_int_line_reserverd_59            ,  //
    cn_int_line_reserverd_60            ,  //
    cn_int_line_reserverd_61             ,  //
    cn_int_line_reserverd_62             , //
    cn_int_line_reserverd_63              //
};

const ufast_t tg_IntUsedNum = sizeof(tg_IntUsed)/sizeof(ufast_t);
//�����ж��߿������ݽṹ
struct tagIntLine tg_int_table[sizeof(tg_IntUsed)/sizeof(ufast_t)];


