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
#ifndef _STACK_DEFINE_H
#define _STACK_DEFINE_H

//attention: this defines only for the powerpc p1020
// in the e500 core, for the ABI protocal, it's an empty decrement stack, so
// all the defines following is under this imagination

#define  _NORMAL_TOP_OFFSET       16
#define  _IRQ_TOP_OFFSET          32
#define  _VM_STACK_FROM_TOP       -4
#define  _IRQ_COUNTER_FROM_TOP    -8

#define CUR_STACK_SIZE  304
//GENERAL REGISTER FROM THE STACK TOP--number 31
#define R0_OFFSET    16
#define R2_OFFSET    24
#define R3_OFFSET    32
#define R4_OFFSET    40
#define R5_OFFSET    48
#define R6_OFFSET    56
#define R7_OFFSET    64
#define R8_OFFSET    72
#define R9_OFFSET    80
#define R10_OFFSET   88
#define R11_OFFSET   96
#define R12_OFFSET   104
#define R13_OFFSET   112
#define R14_OFFSET   120
#define R15_OFFSET   128
#define R16_OFFSET   136
#define R17_OFFSET    144
#define R18_OFFSET    152
#define R19_OFFSET    160
#define R20_OFFSET    168
#define R21_OFFSET    176
#define R22_OFFSET    184
#define R23_OFFSET    192
#define R24_OFFSET    200
#define R25_OFFSET    208
#define R26_OFFSET    216
#define R27_OFFSET    224
#define R28_OFFSET    232
#define R29_OFFSET    240
#define R30_OFFSET    248
#define R31_OFFSET    256
//SPECIAL REGISTER OFFSET FROM THE STACK TOP--number 9
#define CR_OFFSET     268
#define XER_OFFSET    272
#define CTR_OFFSET    276

#define LR_OFFSET     284
#define MSR_OFFSET    288

#define PC_OFFSET     300

#endif  //  _STACK_DEFINE_H
