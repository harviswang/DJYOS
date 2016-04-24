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
#include "stdint.h"
#include "cpu_peri.h"
#include "stm32f10x.h"
#include "SRAM.h"


void SRAM_GPIO_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );

    GPIO_PowerOn(CN_GPIO_D);
    GPIO_PowerOn(CN_GPIO_E);
    GPIO_PowerOn(CN_GPIO_F);
    GPIO_PowerOn(CN_GPIO_G);

    GPIO_CfgPinFunc(CN_GPIO_D, 14, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d0
    GPIO_CfgPinFunc(CN_GPIO_D, 15, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d1
    GPIO_CfgPinFunc(CN_GPIO_D, 0, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d2
    GPIO_CfgPinFunc(CN_GPIO_D, 1, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d3
    GPIO_CfgPinFunc(CN_GPIO_E, 7, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d4
    GPIO_CfgPinFunc(CN_GPIO_E, 8, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d5
    GPIO_CfgPinFunc(CN_GPIO_E, 9, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d6
    GPIO_CfgPinFunc(CN_GPIO_E, 10, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d7
    GPIO_CfgPinFunc(CN_GPIO_E, 11, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d8
    GPIO_CfgPinFunc(CN_GPIO_E, 12, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d9
    GPIO_CfgPinFunc(CN_GPIO_E, 13, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d10
    GPIO_CfgPinFunc(CN_GPIO_E, 14, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d11
    GPIO_CfgPinFunc(CN_GPIO_E, 15, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d12
    GPIO_CfgPinFunc(CN_GPIO_D, 8, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d13
    GPIO_CfgPinFunc(CN_GPIO_D, 9, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //d14
    GPIO_CfgPinFunc(CN_GPIO_D, 10, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //d15

    GPIO_CfgPinFunc(CN_GPIO_F, 0, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a0
    GPIO_CfgPinFunc(CN_GPIO_F, 1, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a1
    GPIO_CfgPinFunc(CN_GPIO_F, 2, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a2
    GPIO_CfgPinFunc(CN_GPIO_F, 3, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a3
    GPIO_CfgPinFunc(CN_GPIO_F, 4, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a4
    GPIO_CfgPinFunc(CN_GPIO_F, 5, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a5
    GPIO_CfgPinFunc(CN_GPIO_F, 12, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a6
    GPIO_CfgPinFunc(CN_GPIO_F, 13, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a7
    GPIO_CfgPinFunc(CN_GPIO_F, 14, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a8
    GPIO_CfgPinFunc(CN_GPIO_F, 15, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a9
    GPIO_CfgPinFunc(CN_GPIO_G, 0, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a10
    GPIO_CfgPinFunc(CN_GPIO_G, 1, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a11
    GPIO_CfgPinFunc(CN_GPIO_G, 2, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a12
    GPIO_CfgPinFunc(CN_GPIO_G, 3, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a13
    GPIO_CfgPinFunc(CN_GPIO_G, 4, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a14
    GPIO_CfgPinFunc(CN_GPIO_G, 5, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a15
    GPIO_CfgPinFunc(CN_GPIO_D, 11, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a16
    GPIO_CfgPinFunc(CN_GPIO_D, 12, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a17
    GPIO_CfgPinFunc(CN_GPIO_D, 13, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //a18
    GPIO_CfgPinFunc(CN_GPIO_E, 3, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a19
    GPIO_CfgPinFunc(CN_GPIO_E, 4, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a20
    GPIO_CfgPinFunc(CN_GPIO_E, 5, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //a21

    GPIO_CfgPinFunc(CN_GPIO_D, 5, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //we
    GPIO_CfgPinFunc(CN_GPIO_D, 4, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //re
    GPIO_CfgPinFunc(CN_GPIO_D, 7, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //cs1 for sram
    GPIO_CfgPinFunc(CN_GPIO_G, 9, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //cs2 for nand
    GPIO_CfgPinFunc(CN_GPIO_G, 10, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //cs3 for lcd
    GPIO_CfgPinFunc(CN_GPIO_G, 12, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);   //cs4 for dm9000a


    GPIO_CfgPinFunc(CN_GPIO_E, 0, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //lb
    GPIO_CfgPinFunc(CN_GPIO_E, 1, CN_GPIO_MODE_PERI_OUT_PP_50Mhz);    //ub

}

void SRAM_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    /*-- FSMC Configuration ------------------------------------------------------*/
    /*----------------------- SRAM Bank 1 ----------------------------------------*/
      /* FSMC_Bank1_NORSRAM1 configuration */

    p.FSMC_AddressSetupTime = 1;
    p.FSMC_AddressHoldTime = 0;
    p.FSMC_DataSetupTime = 2;
    p.FSMC_BusTurnAroundDuration = 0;
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 1;
    p.FSMC_AccessMode = FSMC_AccessMode_A;

    /* Color LCD configuration ------------------------------------
       LCD configured as follow:
          - Data/Address MUX = Disable
          - Memory Type = SRAM
          - Data Width = 16bit
          - Write Operation = Enable
          - Extended Mode = Enable
          - Asynchronous Wait = Disable */
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_PSRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /* BANK1 (of NOR/SRAM Bank) is enabled */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
     FSMC_NORSRAMTimingInitTypeDef  p;
    /*-- FSMC Configuration ------------------------------------------------------*/
    /*----------------------- SRAM Bank 3 ----------------------------------------*/
      /* FSMC_Bank1_NORSRAM3 configuration */
      p.FSMC_AddressSetupTime = 1;
      p.FSMC_AddressHoldTime = 0;
      p.FSMC_DataSetupTime = 2;
      p.FSMC_BusTurnAroundDuration = 0;
      p.FSMC_CLKDivision = 0;
      p.FSMC_DataLatency = 0;
      p.FSMC_AccessMode = FSMC_AccessMode_A;
      /* Color LCD configuration ------------------------------------
         LCD configured as follow:
            - Data/Address MUX = Disable
            - Memory Type = SRAM
            - Data Width = 16bit
            - Write Operation = Enable
            - Extended Mode = Enable
            - Asynchronous Wait = Disable */
      FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
      FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
      FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
      FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
      FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
      FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
      FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
      FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
      FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
      FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
      FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
      FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
      FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
      FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
      FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
      FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
      /* BANK 3 (of NOR/SRAM Bank) is enabled */
      FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
}

void NandFlash_FSMCConfig(void)
{
  FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
  FSMC_NAND_PCCARDTimingInitTypeDef  p;

  /*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_SetupTime = 0x0;
  p.FSMC_WaitSetupTime = 0x3;
  p.FSMC_HoldSetupTime = 0x9;
  p.FSMC_HiZSetupTime = 0x0;

  FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank3_NAND;
  FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable;
  FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;
  FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;
  FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;
  FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;

  FSMC_NANDInit(&FSMC_NANDInitStructure);

  /*!< FSMC NAND Bank Cmd Test */
  FSMC_NANDCmd(FSMC_Bank3_NAND, ENABLE);
}

void Dm9000a_FSMCConfig()
{
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef p;

    // DM9000Aʹ�õĵ�ַ��BANK1��BANK1��4��Ƭѡ��DM9000Aʹ�õ�4��Ƭѡ��
    // ʱ������
    p.FSMC_AddressSetupTime = 1;
    p.FSMC_AddressHoldTime = 1;
    p.FSMC_DataSetupTime = 2;
    p.FSMC_BusTurnAroundDuration = 1;
    p.FSMC_CLKDivision = 1;
    p.FSMC_DataLatency = 1;
    p.FSMC_AccessMode = FSMC_AccessMode_A;
    // ����ģʽ
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_PSRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    // ʹ��DM9000A��FSMC����
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

void SRAM_Init(void)
{
    SRAM_GPIO_Init();
    SRAM_FSMCConfig();
    LCD_FSMCConfig();
    NandFlash_FSMCConfig();
    Dm9000a_FSMCConfig();
}

