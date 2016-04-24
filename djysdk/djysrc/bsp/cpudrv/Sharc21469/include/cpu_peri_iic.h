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
//����ģ��: I2C����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: I2C�������ļ����ṩ��I2C�ڳ�ʼ���������������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-10-06
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_I2C_H__
#define __CPU_PERI_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif


//error code
#define ERR_NO_ERROR                    0
#define ERR_INVALID_PARAMETER           1
#define ERR_TIMEOUT                     2
#define ERR_WRITE_TIMEOUT               3
#define ERR_ACK_TIMEOUT                 4

//offset of reg
typedef struct
{
  vu8 rTWIDIV;         // 0x0000
  vu8 RESERVED0[3];
  vu8 rTWIMITR;        // 0x0004
  vu8 RESERVED1[3];
  vu8 rTWISCTL;        // 0x0008
  vu8 RESERVED2[3];
  vu8 rTWISSTAT;       // 0x000C
  vu8 RESERVED3[3];
  vu8 rTWISADDR;       // 0x0010
  vu8 RESERVED4[3];
  vu8 rTWIMCTL;        // 0x0014
  vu8 RESERVED5[3];
  vu8 rTWIMSTAT;       // 0x0018
  vu8 RESERVED6[3];
  vu8 rTWIMADDR;       // 0x001C
  vu8 RESERVED7[3];
//  vu8 rTWIIRPTL;	   // 0x0020
  vu8 RESERVED8[4];
  vu8 rTWIIMASK;	   // 0x0024
  vu8 RESERVED9[3];
  vu8 rTWIFIFOCTL;	   // 0x0028
  vu8 RESERVED10[3];
  vu8 rTWIFIFOSTAT;	   // 0x002C
}tagI2CReg;

#define rTWIIRPTL 	((volatile unsigned int *)CN_IIC0_BADDR+0x0020)
//fifo regs
#define rTXTWI8 	((volatile unsigned int *)CN_IIC0_BADDR+0x0080)
#define rTXTWI16 	((volatile unsigned int *)CN_IIC0_BADDR+0x0084)
#define rRXTWI8 	((volatile unsigned int *)CN_IIC0_BADDR+0x0088)
#define rRXTWI16 	((volatile unsigned int *)CN_IIC0_BADDR+0x008c)

typedef enum
{
    I2C_CLOCK_RATE_50K,
    I2C_CLOCK_RATE_100K,
    I2C_CLOCK_RATE_200K,
    I2C_CLOCK_RATE_400K
}enum_I2C_Clock;//����IIC���ߵ�ʱ��


bool_t IIC0_Init(void);




#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/

