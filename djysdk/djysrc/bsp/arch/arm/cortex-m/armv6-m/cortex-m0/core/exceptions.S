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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:  �쳣ģ��
//���ߣ�     lst
//�汾��      V1.00
//��ʼ�汾������ڣ�2009-08-30
//�ļ�����:  ϵͳ�쳣ģ���еĻ�ಿ��
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����:
//   ����:
//   �°汾�ţ�
//   �޸�˵��:
//------------------------------------------------------
.syntax         unified
.cpu            cortex-m0
.fpu            softvfp
.thumb
.file   "exceptions.S"

//���첽�ź�ʹ�ܲ��������ȣ����������л��лָ��첽�źżĴ�����ͬ��ʹ���첽�ź�
.macro restore_asyn_signal_reg
    bl Int_ContactAsynSignal
.endm

/**** ջ��С���� ****/
.section        .stack.msp, "aw", %note
.align          3
        .space  0x300

.extern  Exp_EsrTick
.extern  Int_ContactAsynSignal

/**** �쳣������ ****/
.section    .isr_vector, "ax", %progbits
.align          3
.global         isr_vector
isr_vector:
        .word   msp_top
        .word   Init_Cpu
        .word   nmi_handler
        .word   hardfault_handler
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   exp_svc_handler           @ SVCall Handler
    .word   0                         @ Reserved
    .word   0                         @ Reserved
    .word   0                         @ PendSV Handler
    .word   exp_systick_handler       @ SysTick Handler

    @ External Interrupts
    .word     __start_int_0           @ 0
    .word     __start_int_1           @ 1
    .word     __start_int_2           @ 2
    .word     __start_int_3           @ 3
    .word     __start_int_4           @ 4
    .word     __start_int_5           @ 5
    .word     __start_int_6           @ 6
    .word     __start_int_7           @ 7
    .word     __start_int_8           @ 8
    .word     __start_int_9           @ 9
    .word     __start_int_10          @ 10
    .word     __start_int_11          @ 11
    .word     __start_int_12          @ 12
    .word     __start_int_13          @ 13
    .word     __start_int_14          @ 14
    .word     __start_int_15          @ 15
    .word     __start_int_16          @ 16
    .word     __start_int_17          @ 17
    .word     __start_int_18          @ 18
    .word     __start_int_19          @ 19
    .word     __start_int_20          @ 20
    .word     __start_int_21          @ 21
    .word     __start_int_22          @ 22
    .word     __start_int_23          @ 23
    .word     __start_int_24          @ 24
    .word     __start_int_25          @ 25
    .word     __start_int_26          @ 26
    .word     __start_int_27          @ 27
    .word     __start_int_28          @ 28
    .word     __start_int_29          @ 29
    .word     __start_int_30          @ 30
    .word     __start_int_31          @ 31



/**** ������������ ****/
.text
.align 2
.global default_handler
.thumb
.thumb_func
default_handler:
        b               default_handler

.global nmi_handler
.thumb_set nmi_handler, default_handler

.global hardfault_handler
.thumb_set hardfault_handler, default_handler





.text
.align 2
.global exp_systick_handler
.thumb
.thumb_func
exp_systick_handler:
    push    {r0,lr}                             @r0ֻ��Ϊ������ջָ��8�ֽڶ���������
    ldr     r0,=0xe000e010
    ldr     r0,[r0]             @��һ������жϱ�־
    mrs     r0,psp              @ȡ�߳�ջָ��
    subs    r0,r0,#8*4          @����8���Ĵ���,���ڱ���r4~r11
    msr     psp,r0
    stmia   r0!,{r4-r7}         @�����Ĵ���,����tick�쳣���������߳��л���
    mov     r1,r8
    mov     r2,r9
    mov     r3,r10
    mov     r4,r11
    stmia   r0!,{r1-r4}         @�����Ĵ���,����tick�쳣���������߳��л���
    bl      Exp_EsrTick
    pop     {r0,r1}
    mov     lr,r1
    mrs     r0,psp
    ldmfd   r0!,{r4-r7}
    ldmfd   r0!,{r2,r3}
    mov     r8,r2
    mov     r9,r3
    ldmfd   r0!,{r2,r3}
    mov     r10,r2
    mov     r11,r3
    msr     psp,r0
    bx      lr

.text
.align 2
.global exp_svc_handler
.thumb
.thumb_func
exp_svc_handler:
    push    {r0,lr}
    restore_asyn_signal_reg
    pop     {r0,r1}
    mov     lr,r1
    movs    r0,#4
    TST     r1,r0             @����EXC_RETURN�ı���2
    bne     isne
    MRS     R0,MSP              @��ʹ�õ�����ջ���ʰ�MSP��ֵȡ��
    beq     iseq
isne:
    MRS   R0,PSP              @����, ʹ�õ����߳�ջ���ʰ�MSP��ֵȡ��
iseq:
    LDR     R1,[R0,#24]       @��ջ�ж�ȡPC��ֵ
    subs    r1,r1,#2
    LDRB    R1,[R1]           @��SVCָ���ж�ȡ�������ŵ�R1
    cmp     r1,#0
    bne     svc_error        @Ŀǰֻ֧��0��svc����
    adds     r0,#32              @0�ŵ��������������л�������pspָ��
    ldmfd   r0!,{r4-r7}        @�ֹ�����r4-r11
    ldmfd   r0!,{r2,r3}
    mov     r8,r2
    mov     r9,r3
    ldmfd   r0!,{r2,r3}
    mov     r10,r2
    mov     r11,r3
    msr     psp,r0              @pspָ��������������
svc_error:                       @ֱ�ӷ���
    bx      lr                  @���أ�ʵ�ʵ����Ľ��Ǵ�����������

.end