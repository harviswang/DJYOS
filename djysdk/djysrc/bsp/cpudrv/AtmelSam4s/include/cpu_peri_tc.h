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
// ģ������: ��ʱ��Ӳ���ӿڲ����ݶ����Լ��ӿ�����
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 18/12, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef CPU_PERI_TC_H_
#define CPU_PERI_TC_H_

#include "stdint.h"

#ifndef WoReg
#define WoReg vu32
#define RwReg vu32
#define RoReg vu32
#endif

typedef struct {
  RwReg      TC_CCR;        /**< \brief (TcChannel Offset: 0x0) Channel Control Register */
  RwReg      TC_CMR;        /**< \brief (TcChannel Offset: 0x4) Channel Mode Register */
  RwReg      TC_SMMR;       /**< \brief (TcChannel Offset: 0x8) Stepper Motor Mode Register */
  RoReg      Reserved1[1];
  RwReg      TC_CV;         /**< \brief (TcChannel Offset: 0x10) Counter Value */
  RwReg      TC_RA;         /**< \brief (TcChannel Offset: 0x14) Register A */
  RwReg      TC_RB;         /**< \brief (TcChannel Offset: 0x18) Register B */
  RwReg      TC_RC;         /**< \brief (TcChannel Offset: 0x1C) Register C */
  RwReg      TC_SR;         /**< \brief (TcChannel Offset: 0x20) Status Register */
  RwReg      TC_IER;        /**< \brief (TcChannel Offset: 0x24) Interrupt Enable Register */
  RwReg      TC_IDR;        /**< \brief (TcChannel Offset: 0x28) Interrupt Disable Register */
  RwReg      TC_IMR;        /**< \brief (TcChannel Offset: 0x2C) Interrupt Mask Register */
  RoReg      Reserved2[4];
} tagTcChannel;

#define TCCHANNEL_NUMBER 3
typedef struct {
  tagTcChannel  TC_CHANNEL[TCCHANNEL_NUMBER]; /**< \brief (Tc Offset: 0x0) channel = 0 .. 2 */
  WoReg      TC_BCR;        /**< \brief (Tc Offset: 0xC0) Block Control Register */
  RwReg      TC_BMR;        /**< \brief (Tc Offset: 0xC4) Block Mode Register */
  WoReg      TC_QIER;       /**< \brief (Tc Offset: 0xC8) QDEC Interrupt Enable Register */
  WoReg      TC_QIDR;       /**< \brief (Tc Offset: 0xCC) QDEC Interrupt Disable Register */
  RoReg      TC_QIMR;       /**< \brief (Tc Offset: 0xD0) QDEC Interrupt Mask Register */
  RoReg      TC_QISR;       /**< \brief (Tc Offset: 0xD4) QDEC Interrupt Status Register */
  RwReg      TC_FMR;        /**< \brief (Tc Offset: 0xD8) Fault Mode Register */
  RoReg      Reserved1[2];
  RwReg      TC_WPMR;       /**< \brief (Tc Offset: 0xE4) Write Protect Mode Register */
} tagTcReg;


#endif /* TIMER_HARD_H_ */
