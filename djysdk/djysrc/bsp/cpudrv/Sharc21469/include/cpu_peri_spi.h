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
//�ļ�����: spi�������ļ����ṩ��spi�ڳ�ʼ���������������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-10-06
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_SPI_H__
#define __CPU_PERI_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  vu32 rSPICTL;        	// 0x0000
  vu32 rSPIFLG;          // 0x0001
  vu32 rSPISTAT;         // 0x0002
  vu32 rTXSPI;         	// 0x0003
  vu32 rRXSPI;         	// 0x0004
  vu32 rSPIBAUD;         // 0x0005
  vu32 rRXSPI_SHADOW;	// 0x0006
  vu32 RESERVED0[128];
  vu32 rIISPI;        // 0x0000
  vu32 rIMSPI;        // 0x0001
  vu32 rCSPI;         // 0x0002
  vu32 rCPSPI;        // 0x0003
  vu32 rSPIDMAC;      // 0x0004
}tag_SpiReg;

#define g_ptSpi0Reg	(volatile tag_SpiReg *)cn_spi1_baddr
#define g_ptSpi1Reg	(volatile tag_SpiReg *)cn_spi2_baddr
 
//-----------------------------------------------------------------------------
// Public Typedefs
//-----------------------------------------------------------------------------

typedef enum
{
    SPI_CS0 = 0,
    SPI_CS1 = 1,
    SPI_CS2 = 2,
    SPI_CS3 = 3,
}enum_spi_csx;

typedef enum
{
   SPI_MODE_SLAVE = 0,
   SPI_MODE_MASTER
} tagSpiMode;

typedef enum
{
   SPI_SHIFT_MSB = 0,
   SPI_SHIFT_LSB
} tagSpiShiftDir;

typedef struct
{
   tagSpiMode tagMode;			//Slave or Master 
   tagSpiShiftDir tagShiftDir;	//msb or lsb
   u8 polarity;					//Clock Polarity
   u8 phase;					//Clock Phase
   u32 freq;					//SPI BAUD
   u8 char_len;					//word length
} tagSpiConfig;

extern tagSpiConfig	pg_spi_Config;

bool_t Spi_Init(volatile tag_SpiReg * tpSpi, tagSpiConfig *tagpInConfig);
bool_t Spi_ActiveCS(volatile tag_SpiReg * tpSpi,ufast_t ufCsNo);
bool_t Spi_InActiveCS(volatile tag_SpiReg * tpSpi,ufast_t ufCsNo);
u32 Spi_TxRx(volatile tag_SpiReg * const tpSpi,u8 *u8pSrc,u32 u32SrcLen,
                u8 *u8pDst,u32 u32DstLen,u32 u32DstOffset);
                
#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/

