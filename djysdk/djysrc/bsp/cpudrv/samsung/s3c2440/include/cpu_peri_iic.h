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

#define CN_IIC0  0

//offset of reg
typedef struct
{
  vu32 IICCON;
  vu32 IICSTAT;
  vu32 IICADD;
  vu32 IICDS;
  vu32 IICLC;
}tagI2CReg;

#define CN_IIC_ADDR_OFFSET  0x01		//���λΪ��д��־
#define CN_IIC_WRITE_BIT    0x00		//����־Ϊ0
#define CN_IIC_READ_BIT     0x01		//д��־Ϊ1

#define IICCON_ACKEN_MASK   ( 1 << 7)
#define IICCON_IICLK_MASK   ( 1 << 6)
#define IICCON_INT_MASK     ( 1 << 5)
#define IICCON_INTPENDINGFLAG_MASK (1 << 4)
#define IICCON_TXCLK_OFFSET (0)
#define IICCON_TXCLK(x)     ((x & 0xF) << IICCON_TXCLK_OFFSET)

#define IICCON_MS_OFFSET 	(6)
#define IICSTAT_MS(x)       ((x & 0x3) << IICCON_MS_OFFSET)
#define IICSTAT_MS_SR_MODE  (0 << IICCON_MS_OFFSET)
#define IICSTAT_MS_ST_MODE  (1 << IICCON_MS_OFFSET)
#define IICSTAT_MS_MR_MODE  (2 << IICCON_MS_OFFSET)
#define IICSTAT_MS_MT_MODE  (3 << IICCON_MS_OFFSET)
#define IICSTAT_ACK_MASK    (1 << 0)
#define IICSTAT_START_STOP_MASK (1 << 5)
#define IICSTAT_TXRXEN_MASK (1 << 4)
#define IICSTAT_BSY_MASK			0x20

#define CN_IIC_CLK_100K     (100*1000)
ptu32_t module_init_iic(ptu32_t para);


#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/

