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
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//ϵͳ�쳣��
#define CN_SYSVECT_RESET                    1
#define CN_SYSVECT_NMI                      2
#define CN_SYSVECT_HARD_FAULT               3
#define CN_SYSVECT_MEMMANAGE_FAULT          4
#define CN_SYSVECT_BUS_FAULT                5
#define CN_SYSVECT_USAGE_FAULT              6
#define CN_SYSVECT_SVC                      11
#define CN_SYSVECT_DEBUG                    12
#define CN_SYSVECT_PENDSV                   14
#define CN_SYSVECT_SYSTICK                  15

//�쳣�������ƺͱ�־
#define CN_HARDEXP_DECODERNAME          "HARDEXP_DECODER"
#define CN_SYS_EXP_CPUINFO_VALIDFLAG    ((u32)(0x87654321))

//����DEBUG FAULT״̬λ
#define CN_EXP_DFSR_HALTED                  (0x01 << 0)
#define CN_EXP_DFSR_BKPT                    (0x01 << 1)
#define CN_EXP_DFSR_DWTTRAP                 (0x01 << 2)
#define CN_EXP_DFSR_VCATCH                  (0x01 << 3)
#define CN_EXP_DFSR_EXTERNAL                (0x01 << 4)

//����HARD FAULT״̬λ
#define CN_EXP_HFSR_VECTBL                  (0x01 << 1)
#define CN_EXP_HFSR_FORCED                  (0x01 << 30)
#define CN_EXP_HFSR_DEBUGEVT                (0x01 << 31)

//����USAGE FAULT״̬λ
#define CN_EXP_UFSR_UNDEFINSTR              (0x01 << 0)
#define CN_EXP_UFSR_INVSTATE                (0x01 << 1)
#define CN_EXP_UFSR_INVPC                   (0x01 << 2)
#define CN_EXP_UFSR_NOCP                    (0x01 << 3)
#define CN_EXP_UFSR_UNALIGNED               (0x01 << 8)
#define CN_EXP_UFSR_DIVBYZERO               (0x01 << 9)

//����BUS FAULT״̬λ
#define CN_EXP_BFSR_IBUSERR                 (0x01 << 0)
#define CN_EXP_BFSR_PRECISERR               (0x01 << 1)
#define CN_EXP_BFSR_IMPRECISERR             (0x01 << 2)
#define CN_EXP_BFSR_UNSTKERR                (0x01 << 3)
#define CN_EXP_BFSR_STKERR                  (0x01 << 4)
#define CN_EXP_BFSR_BFARVALID               (0x01 << 7)

//����MEM FAULT״̬λ
#define CN_EXP_MFSR_IACCVIOL                (0x01 << 0)
#define CN_EXP_MFSR_DACCVIOL                (0x01 << 1)
#define CN_EXP_MFSR_MUNSTKERR               (0x01 << 3)
#define CN_EXP_MFSR_MSTKERR                 (0x01 << 4)
#define CN_EXP_MFSR_MMARVALID               (0x01 << 7)


//���쳣��Ϣ������CPU ��CORE ��Ϣ������CORE���У��;���CPU�ͺ��޹�
struct SysExpCoreInfo
{
    u32 spr_sp;
    u32 ger_r4;
    u32 ger_r5;
    u32 ger_r6;
    u32 ger_r7;
    u32 ger_r8;
    u32 ger_r9;
    u32 ger_r10;
    u32 ger_r11;    //r4-r11��������ջҲ����
    u32 ger_r0;
    u32 ger_r1;
    u32 ger_r2;
    u32 ger_r3;
    u32 ger_r12;
    u32 spr_lr;
    u32 spr_pc;
    u32 spr_xpsr;   //r0-r3,r12-r15���Զ���ѹջ
};

struct SysExpNvicInfo
{
    u32 bfar;       //bus fault address register
    u32 mmar;       //memory manage fault address register
    u32 dfsr;       //debug fault state register
    u32 hfsr;       //hard fault state register
    u16 ufsr;       //usage fault state register
    u8  bfsr;       //bus fault state register
    u8  mfsr;       //memory manage fault state register
};

enum SysExpType
{
    CN_EXP_TYPE_MEM_FAULT = 0,
    CN_EXP_TYPE_BUS_FAULT,
    CN_EXP_TYPE_USG_FAULT,
    CN_EXP_TYPE_HARD_FAULT,
    CN_EXP_TYPE_DBG_FAULT
};

struct SysExceptionInfo
{
    u32 SysExpType;                     //�쳣��Ϣ����
    u32 SysExpCpuFlag;                 //���쳣��Ϣ����Ч����
    struct SysExpCoreInfo CoreInfo;
    struct SysExpNvicInfo NvicInfo;
};

extern u32 g_u32ExpTable[16];
void Exp_Init(void);
void Exp_ConnectSystick(void (*tick)(u32 inc_ticks));

#ifdef __cplusplus
}
#endif
#endif // _EXCEPTION_H_

