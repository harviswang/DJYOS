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
//module:p1020-core0
//author��zqf
//version��V1.0.0 11:40:37 AM/Nov 4, 2013
//description:this is used for the ppc e500v2 core regs
//other description:for the asm using
//modified history: NULL
//------------------------------------------------------

#ifndef E500V2_CORE_REGS_H_
#define E500V2_CORE_REGS_H_
////////////////////////////////////////////////////////////////////////////////
//define for the general register
////////////////////////////////////////////////////////////////////////////////
#define	r0	0x0
#define	r1	0x1
#define sp  r1
#define	r2	0x2
#define	r3	0x3
#define	r4	0x4
#define	r5	0x5
#define	r6	0x6
#define	r7	0x7
#define	r8	0x8
#define	r9	0x9
#define	r10	0xa
#define	r11	0xb
#define	r12	0xc
#define	r13	0xd
#define	r14	0xe
#define	r15	0xf
#define	r16	0x10
#define	r17	0x11
#define	r18	0x12
#define	r19	0x13
#define	r20	0x14
#define	r21	0x15
#define	r22	0x16
#define	r23	0x17
#define	r24	0x18
#define	r25	0x19
#define	r26	0x1a
#define	r27	0x1b
#define	r28	0x1c
#define	r29	0x1d
#define	r30	0x1e
#define	r31	0x1f
////////////////////////////////////////////////////////////////////////////////
//define some special register
////////////////////////////////////////////////////////////////////////////////
#define hid0  	   0x3f0
#define hid1       0x3f1
#define xer        0x01
//you could use lr and cr in specific instructions
#define lr         0x08
#define cr
#define spefscr    0x200
#define ctr        0x09
#define bucsr      0x3f5

//register for configuration
#define SVR        0x3ff
#define PVR        0x11f
#define PIR        0x11e

//register for the interrupt
#define  srr0       0x1a
#define  srr1       0x1b
#define  csrr0      0x3a
#define  csrr1      0x3b
#define  mcsrr0     0x23a
#define  mcsrr1     0x23b
#define  mcsr       0x23c
#define  mcar       0x23d
#define  dear       0x3d
#define  esr        0x3e//exception state register
#define  IVPR       0x3f
#define  IVOR0      0x190   /* Interrupt Vector Offset Register 0 */
#define  IVOR1      0x191   /* Interrupt Vector Offset Register 1 */
#define  IVOR2      0x192   /* Interrupt Vector Offset Register 2 */
#define  IVOR3      0x193   /* Interrupt Vector Offset Register 3 */
#define  IVOR4      0x194   /* Interrupt Vector Offset Register 4 */
#define  IVOR5      0x195   /* Interrupt Vector Offset Register 5 */
#define  IVOR6      0x196   /* Interrupt Vector Offset Register 6 */
#define  IVOR7      0x197   /* Interrupt Vector Offset Register 7 */
#define  IVOR8      0x198   /* Interrupt Vector Offset Register 8 */
#define  IVOR9      0x199   /* Interrupt Vector Offset Register 9 */
#define  IVOR10     0x19a   /* Interrupt Vector Offset Register 10 */
#define  IVOR11     0x19b   /* Interrupt Vector Offset Register 11 */
#define  IVOR12     0x19c   /* Interrupt Vector Offset Register 12 */
#define  IVOR13     0x19d   /* Interrupt Vector Offset Register 13 */
#define  IVOR14     0x19e   /* Interrupt Vector Offset Register 14 */
#define  IVOR15     0x19f   /* Interrupt Vector Offset Register 15 */
#define  IVOR32     0x210   /* Interrupt Vector Offset Register 32 */
#define  IVOR33     0x211   /* Interrupt Vector Offset Register 33 */
#define  IVOR34     0x212   /* Interrupt Vector Offset Register 34 */
#define  IVOR35     0x213   /* Interrupt Vector Offset Register 35 */

//define for the timer
#define tbl_read   0x10c
#define tbl_write  0x11c
#define tbu_read   0x10d
#define tbu_write  0x11d
#define tcr        0x154  //time control register
#define tsr        0x150 //time state register
#define dec        0x16   //decrement timer register
#define decar      0x36   //decrement timer auto reload register

//register for the debug
#define dbcr0      308
#define dbcr1      309
#define dbcr2      310
#define dbsr       304
#define iac1       312
#define iac2       313
#define dac1       316
#define dac2       317

//register for the MMU
#define mmucsr0  0x3f4
#define  MAS0    0x270
#define  MAS1    0x271
#define  MAS2    0x272
#define  MAS3    0x273
#define  MAS4    0x274
#define  MAS6    0x276
#define  MAS7    0x3b0
#define  pid0    0x30
#define  pid1    0x279
#define  pid2    0x27a
#define  mmucfg    0x3f7
#define  tlb0cfg   0x2b0
#define  tlb1cfg   0x2b1

#define  l1csr0     0x3f2
#define  l1csr1     0x3f3
#define  l1cfg0     0x203
#define  l1cfg1     0x204

//registers for general special registers
#define  spr0   0x110
#define  spr1   0x111
#define  spr2   0x112
#define  spr3   0x113
#define  spr4   0x114
#define  spr5   0x115
#define  spr6   0x116
#define  spr7   0x117

#endif /* E500V2_REGS_H_ */
