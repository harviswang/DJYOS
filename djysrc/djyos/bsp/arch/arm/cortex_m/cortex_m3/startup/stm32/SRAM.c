#include "os_inc.h"
#include "stm32f10x.h"
#include "SRAM.h"


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

void SRAM_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF
                                                | RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = SRAM_D0_GPIOD14 | SRAM_D1_GPIOD15 | SRAM_D2_GPIOD0 | SRAM_D3_GPIOD1 | SRAM_D13_GPIOD8
                                                    | SRAM_D14_GPIOD9 | SRAM_D15_GPIOD10 | SRAM_WE_GPIOD5 | SRAM_RE_GPIOD4
                                                    | SRAM_CS_GPIOD7 | SRAM_A16_GPIOD11 | SRAM_A17_GPIOD12 | SRAM_A18_GPIOD13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SRAM_D4_GPIOE7 | SRAM_D5_GPIOE8 | SRAM_D6_GPIOE9 | SRAM_D7_GPIOE10 | SRAM_D8_GPIOE11
                                                    | SRAM_D9_GPIOE12 | SRAM_D10_GPIOE13 | SRAM_D11_GPIOE14 | SRAM_D12_GPIOE15
                                                    | SRAM_LB_GPIOE0 | SRAM_UB_GPIOE1 | SRAM_A19_GPIOE3 | SRAM_A20_GPIOE4 |SRAM_A21_GPIOE5;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SRAM_A0_GPIOF0 | SRAM_A1_GPIOF1 | SRAM_A2_GPIOF2 | SRAM_A3_GPIOF3 | SRAM_A4_GPIOF4
                                                        | SRAM_A5_GPIOF5 | SRAM_A6_GPIOF12 | SRAM_A7_GPIOF13 | SRAM_A8_GPIOF14 | SRAM_A9_GPIOF15;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SRAM_A10_GPIOG0 | SRAM_A11_GPIOG1 | SRAM_A12_GPIOG2 | SRAM_A13_GPIOG3 | SRAM_A14_GPIOG4
                                                           | SRAM_A15_GPIOG5;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    SRAM_FSMCConfig();
}

void SRAM_Init(void)
{
	SRAM_GPIO_Init();
}

