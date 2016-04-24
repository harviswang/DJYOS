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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��lcd_spidrv.h
// ģ������: ͼ����ʾ����ͨ�Žӿڣ�CPU������оƬͨ�Ų���SPIģʽ����Ϊ������оƬ��Ӳ��
//           ���٣�ÿһ���㶼��Ҫ����һ��SPIͨ�ţ�ͨ����С������DJYBUSģ����󽵵�
//           ͨ��Ч�ʣ���˱�д��LCDר��SPIͨ��ģ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 06/10.2014
// =============================================================================
#if 1
#include "cpu_peri.h"
#include "lcd_spidrv.h"

// =============================================================================
// ���ܣ���ʼ��SPIģ�飬����IO��SPIͨ�Ų���
// ��������
// ���أ���
// =============================================================================
static void __spi_init(void)
{
    // Connect MOSI, MISO, and SCK to SSP peripheral
    LPC_GPIO0->FIODIR    &= ~PIN_CLK;
    LPC_PINCON->PINSEL0  |= (2 << 18) | (2 << 16) | (2 << 14);
    LPC_PINCON->PINMODE0 &= ~0x000FF000;

    // Enable SPI in Master Mode, CPOL=1, CPHA=1
    // Max. 12.5 MBit used for Data Transfer @ 100MHz
    LPC_SSP1->CR0        = 0x01C7;
    LPC_SSP1->CPSR       = 0x02;
    LPC_SSP1->CR1        = 0x02;
}

// =============================================================================
// ���ܣ�����һ���ֽڵ����ݵ�SPI���豸��ͬʱ����һ���ֽڵ�����
// ������byte,������ֽ�
// ���أ��������ֽ�
// =============================================================================
unsigned char spi_tran (unsigned char byte)
{
    LPC_SSP1->DR = byte;
    while (!(LPC_SSP1->SR & RNE));        // Wait for send to finish
    return (LPC_SSP1->DR);
}

bool_t LCD_TxRx(u8* WrBuf, u32 WrLen, u8* RdBuf, u32 RdLen)
{
    u32 i;
    LCD_CS(0);
    for(i = 0; i < WrLen; i++)
    {
        spi_tran(WrBuf[i]);
    }
    for(i = 0; i < RdLen; i++)
    {
        RdBuf[i] = spi_tran(0);
    }
    LCD_CS(1);
    return true;
}

// =============================================================================
// ���ܣ�SPIӲ����ʼ������LCDģ�����
// ��������
// ���أ���
// =============================================================================
void LCD_HardInit(void)
{
    // Configure the LCD Control pins
    LPC_PINCON->PINSEL9 &= 0xF0FFFFFF;
    LPC_GPIO4->FIODIR   |= 0x30000000;
    LPC_GPIO4->FIOSET    = 0x20000000;

    // SSEL1 is GPIO output set to high
    LPC_GPIO0->FIODIR   |= 0x00000040;
    LPC_GPIO0->FIOSET    = 0x00000040;
    LPC_PINCON->PINSEL0 &= 0xFFF03FFF;
    LPC_PINCON->PINSEL0 |= 0x000A8000;

    __spi_init();
}
#else

#include "stdint.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "cpu_peri_ssp.h"
#include "lcd_spidrv.h"

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct tagSPI_Device *ps_LCD_Dev = NULL;
static u32 s_LCD_Timeout = CN_TIMEOUT_FOREVER;


bool_t LCD_TxRx(u8* WrBuf, u32 WrLen, u8* RdBuf, u32 RdLen)
{
    struct tagSPI_DataFrame spidata;
    spidata.SendBuf = WrBuf;
    spidata.SendLen = WrLen;
    spidata.RecvBuf = RdBuf;
    spidata.RecvLen = RdLen;
    LCD_CS(0);

    SPI_Transfer(ps_LCD_Dev,&spidata,true,s_LCD_Timeout);

    LCD_CS(1);
    return true;
}

static void __LCD_HardDrvInit(void)
{
    // Configure the LCD Control pins
    LPC_PINCON->PINSEL9 &= 0xF0FFFFFF;
    LPC_GPIO4->FIODIR   |= 0x30000000;
    LPC_GPIO4->FIOSET    = 0x20000000;

    // SSEL1 is GPIO output set to high
    LPC_GPIO0->FIODIR   |= 0x00000040;
    LPC_GPIO0->FIOSET    = 0x00000040;
    LPC_PINCON->PINSEL0 &= 0xFFF03FFF;
    LPC_PINCON->PINSEL0 |= 0x000A8000;
}

bool_t LCD_HardInit(void)
{
    bool_t result = false;
    static struct tagSPI_Device s_LCD_Dev;

    __LCD_HardDrvInit();

    //��ʼ��IIC�豸�ṹ��
    s_LCD_Dev.Cs = 0;
    s_LCD_Dev.AutoCs = false;
    s_LCD_Dev.CharLen = 8;
    s_LCD_Dev.Mode = SPI_MODE_3;
    s_LCD_Dev.ShiftDir = SPI_SHIFT_MSB;
    s_LCD_Dev.Freq = 10*1000*1000;      //10M

    //���LCDCH��IIC0����
    if(NULL != SPI_DevAdd_s("SSP1","IIC_Dev_LCD",&s_LCD_Dev))
    {
        ps_LCD_Dev = &s_LCD_Dev;
        SPI_BusCtrl(ps_LCD_Dev,CN_SPI_SET_POLL,0,0);
        result = true;
    }

    return result;
}
#endif
