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
//========================================================  
// �ļ�ģ��˵��:                                            
// omapl138��ʱ����������                         
// �ļ��汾: v1.00                                          
// ������Ա: lst                                            
// ����ʱ��: 2011.06.01                                     
// Copyright(c) 2011-2011  ��������������޹�˾            
//========================================================  
// �����޸ļ�¼(���µķ�����ǰ��):							
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>			
//========================================================  
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��

#ifndef __OMAPL138_TIMER_H__
#define __OMAPL138_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

//timer0:0x01c20000
//timer1:0x01c21000
//timer2:0x01f0c000
//timer3:0x01f0d000
//�ر�ע��:L138��timer������������е�out��inû�����壬���ž��������뻹�������
//GPDATA_GPDIR�Ĵ���ȷ��������TM64P_OUT34���ź�TM64P_IN34���ţ���λ��������ţ�
//����GPDATA_GPDIR������������Ϊ��������
typedef struct
{
    vu32 REV;             // 0x0000    0x44720211
    vu32 EMUMGT;          // 0x0004 bit0=0ʹ��bit1���Ʒ���ģʽ��
                                       // timer���з�ʽ��bit1=0ֹͣ��=1����ֹͣ
    vu32 GPINT_GPEN;      // 0x0008 timer�ⲿ���Ź���ģʽ
    vu32 GPDATA_GPDIR;    // 0x000C timer�ⲿ���Ź���ģʽ
    vu32 TIM12;           // 0x0010 �Ӽ�����
    vu32 TIM34;           // 0x0014 �Ӽ�����
    vu32 PRD12;           // 0x0018 ���ڼĴ���
    vu32 PRD34;           // 0x001C ���ڼĴ���
    vu32 TCR;             // 0x0020 
    vu32 TGCR;            // 0x0024
    vu32 WDTCR;           // 0x0028
    vu32 RSVD0[2];        // 0x002C
    vu32 REL12;           // 0x0034 ���ؼĴ���
    vu32 REL34;           // 0x0038 ���ؼĴ���
    vu32 CAP12;           // 0x003C ����Ĵ���
    vu32 CAP34;           // 0x0040 ����Ĵ���
    vu32 INTCTLSTAT;      // 0x0044
    vu32 RSVD1[6];        // 0x0048
    vu32 CMP0;            // 0x0060
    vu32 CMP1;            // 0x0064
    vu32 CMP2;            // 0x0068
    vu32 CMP3;            // 0x006C
    vu32 CMP4;            // 0x0070
    vu32 CMP5;            // 0x0074
    vu32 CMP6;            // 0x0078
    vu32 CMP7;            // 0x007C
}tagTimerReg;

// bitmask defines for GPINT_GPEN.
#define GPENO34         (0x02000000)   // bit 25
#define GPENI34         (0x01000000)   // bit 24
#define GPENO12         (0x00020000)   // bit 17
#define GPENI12         (0x00010000)   // bit 16
#define GPINT34INVO     (0x00002000)   // bit 13
#define GPINT34INVI     (0x00001000)   // bit 12
#define GPINT34ENO      (0x00000200)   // bit 9
#define GPINT34ENI      (0x00000100)   // bit 8
#define GPINT12INVO     (0x00000020)   // bit 5
#define GPINT12INVI     (0x00000010)   // bit 4
#define GPINT12ENO      (0x00000002)   // bit 1
#define GPINT12ENI      (0x00000001)   // bit 0

// bitmask defines for GPDATA_GPDIR.
#define GPDIRO34        (0x02000000)   // bit 25
#define GPDIRI34        (0x01000000)   // bit 24
#define GPDIRO12        (0x00020000)   // bit 17
#define GPDIRI12        (0x00010000)   // bit 16
#define GPDATAO34       (0x00000200)   // bit 9
#define GPDATAI34       (0x00000100)   // bit 8
#define GPDATAO12       (0x00000002)   // bit 1
#define GPDATAI12       (0x00000001)   // bit 0

// bitmask defines for TCR.
#define ENAMODE34_OFF   22          //t34�Ĺ���ģʽ,2bit
#define ENAMODE12_OFF   6           //t12�Ĺ���ģʽ,2bit

#define CLKSRC34_OFF    24          //t34��ʱ��Դ,1bit
#define CLKSRC12_64_OFF 8           //t12��64λtimerģʽ��ʱ��Դ,1bit

#define EVTINTEN34      (1<<18)     //t34�¼������ж�
#define PRDINTEN34      (1<<16)     //t34����ƥ���ж�
#define EVTINTEN12      (1<<2)      //t12�¼������ж�
#define PRDINTEN12      (1<<0)      //t12����ƥ���ж�

void timer_counter_len_set(u32 t64_no,ufast_t mode);
void timer_int_ctrl(u32 t64_no,u32 msk,bool_t en);
void timer_set_clksrc(u32 t64_no,u32 sub,u32 src);
void timer_set_mode(u32 t64_no,u32 sub,u32 mode);
void timer_set_Period(u32 t64_no,u32 sub,u32 value);
void timer_set_reload(u32 t64_no,u32 sub,u32 value);
void Timer_Stop(u32 t64_no,u32 sub);
void Timer_Start(u32 t64_no,u32 sub);
void timer_clear(u32 t64_no,u32 sub);
u32 Timer_Read(u32 t64_no,u32 sub);

#ifdef __cplusplus
}
#endif

#endif //__OMAPL138_TIMER_H__
