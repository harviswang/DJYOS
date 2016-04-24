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
//1. ����: 2009-006-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GPIO_H__
#define __GPIO_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_reg         //��ַ��0x56000000
{
    vu32 GPACON;     //0x56000000
    vu32 GPADAT;     //0x56000004
    vu32 rev1;      //0x56000008
    vu32 rev2;      //0x5600000c
    vu32 GPBCON;     //0x56000010
    vu32 GPBDAT;     //0x56000014
    vu32 GPBUP ;     //0x56000018
    vu32 rev4;      //0x5600001c
    vu32 GPCCON;     //0x56000020
    vu32 GPCDAT;     //0x56000024
    vu32 GPCUP ;     //0x56000028
    vu32 rev5;      //0x5600002c
    vu32 GPDCON;     //0x56000030
    vu32 GPDDAT;     //0x56000034
    vu32 GPDUP ;     //0x56000038
    vu32 rev6;      //0x5600003c
    vu32 GPECON;     //0x56000040
    vu32 GPEDAT;     //0x56000044
    vu32 GPEUP ;     //0x56000048
    vu32 rev7;      //0x5600001c
    vu32 GPFCON;     //0x56000050
    vu32 GPFDAT;     //0x56000054
    vu32 GPFUP ;     //0x56000058
    vu32 rev8;      //0x5600005c
    vu32 GPGCON;     //0x56000060
    vu32 GPGDAT;     //0x56000064
    vu32 GPGUP ;     //0x56000068
    vu32 rev9;      //0x5600006c
    vu32 GPHCON;     //0x56000070
    vu32 GPHDAT;     //0x56000074
    vu32 GPHUP ;     //0x56000078
    vu32 rev10;      //0x5600007c
    vu32 MISCCR;     //0x56000080
    vu32 DCLKCON;     //0x56000084
    vu32 EXTINT0;     //0x56000088
    vu32 EXTINT1;      //0x5600008c
    vu32 EXTINT2;      //0x56000090
    vu32 EINTFLT0;     //0x56000094
    vu32 EINTFLT1;     //0x56000098
    vu32 EINTFLT2;      //0x5600009c
    //vu32 EINTFLT3;      //0x4C0000a0???
    vu32 rev11;         //0x560000a0
    vu32 EINTMASK;      //0X560000a4
    vu32 EINTPEND;      //0X560000a8
};

#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H__*/
