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
/*========================================================*
 * �ļ�ģ��˵��:                                                 *
 * omapl138��spi�������ļ����ṩ��spi�ڳ�ʼ���������������      *
 * �ļ��汾: v1.00                                               *
 * ������Ա: lst,kongds,huangcw                              *
 * ����ʱ��: 2011.06.01                                          *
 * Copyright(c) 2011-2011  ��������������޹�˾             *
 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):                               *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>              *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/
#include "stdint.h"
#include "cpu_peri.h"

// =========================================================================
// �������ܣ�spi�ڳ�ʼ��,�������У�cs��spiģ�鰴gpio��output��ʽ���ʣ���csһ����
//              ��Ϊgpio������ܡ�����l138�����Ź����ص㣬pinmux�е�����������
//              spiģ���е����ã������pinmux�а�spi0��scs0��������ΪGP2[14]��
//              ���������Ž���ϵͳgpioģ����ƣ�spiģ��������������úͲ�������
//              Ч����ˣ�����spi֮ǰ��Ӧ������pinmux�аѸ���������Ϊspi��cs��
//              �ܣ�ti���ĵ�����pinmux��ֻ�ܰ���Щ���ŷ����spiʹ�ã�������ָ
//              ����Ϊcs����gpio���Ƿ���Ϊcsʹ�ã���spiģ�����
// ���������tpSpi����������spi���ƽṹ�ĵ�ַ
//           tagpInConfig�����ýṹ������������Ϣ
// �����������
// ����ֵ  ��true=�ɹ���false=ʧ��
// =========================================================================
bool_t Spi_Init(volatile tagSpiReg * tpSpi, tagSpiConfig* tagpInConfig)
{
    u32 u32Prescaler;
    if(tpSpi == g_ptSpi0Reg)
        Cpucfg_EnableLPSC(cn_PSC0_SPI0);
    else if(tpSpi == g_ptSpi1Reg)
        Cpucfg_EnableLPSC(cn_PSC1_SPI1);
    else
        return false;
    if (tagpInConfig != NULL)
    {

      // reset tpSpi port.
      tpSpi->GCR0 = 0;

      tpSpi->GCR0 |= CN_SPI_RESET;

      // config master/slave mode.
      if (SPI_MODE_MASTER == tagpInConfig->tagMode)
      {
         // set clkmod and master for master mode.
         tpSpi->GCR1 = CN_SPI_CLKMOD | CN_SPI_MASTER;
      }
      else if (SPI_MODE_SLAVE == tagpInConfig->tagMode)
      {
         // clear spigcr1 for slave mode.
         tpSpi->GCR1 = 0;
      }
      else
      {
         goto error_exit;
      }

      // config pin options.
      switch (tagpInConfig->tagPinOption)
      {
         case SPI_3PIN:
            // enable tpSpi SOMI, SIMO, and CLK.
            tpSpi->PC0 = SOMI | SIMO | CLK;
            break;

         case SPI_4PIN_CS:
            // enable tpSpi SOMI, SIMO, CLK, and set cs[0~7] as gpio.
            tpSpi->PC0 = SOMI | SIMO | CLK;
            tpSpi->PC0 &= ~all_cs;        //����Ƭѡ����Ϊgpio
            tpSpi->PC4 = all_cs;          //��������Ƭѡ
            tpSpi->PC1 |= all_cs;         //����Ƭѡ����Ϊ���
            break;

         case SPI_4PIN_EN:
            // enable tpSpi SOMI, SIMO, CLK, and ENA.
            tpSpi->PC0 = SOMI | SIMO | CLK | ENA;
            break;

         case SPI_5PIN:
            // enable tpSpi SOMI, SIMO, CLK, SCS0, ENA and set cs[0~7] as gpio.
            tpSpi->PC0 = SOMI | SIMO | CLK | ENA;
            tpSpi->PC0 &= ~all_cs;        //����Ƭѡ����Ϊgpio
            tpSpi->PC4 = all_cs;          //��������Ƭѡ
            tpSpi->PC1 |= all_cs;         //����Ƭѡ����Ϊ���
            break;

         default:
             goto error_exit;
      }

      // config tpSpi direction, polarity, and phase.
      tpSpi->FMT0 = 0;

      if (SPI_SHIFT_LSB == tagpInConfig->tagShiftDir)
      {
         tpSpi->FMT0 |= SHIFTDIR;
      }

      if (tagpInConfig->polarity)
      {
         tpSpi->FMT0 |= POLARITY;
      }

      if (tagpInConfig->phase)
      {
         tpSpi->FMT0 |= PHASE;
      }

      // set the u32Prescaler and character length.
      u32Prescaler = (((CN_CFG_PLL0_SYSCLK2 / tagpInConfig->freq) - 1) & 0xFF);
      //u32Prescaler = 0x80;
      tpSpi->FMT0 |= u32Prescaler << SHIFT_PRESCALE;

      tpSpi->FMT0 |= (tagpInConfig->char_len&0x1f)<< SHIFT_CHARLEN;

      tpSpi->DELAY = (8 << 24) | (8 << 16);

      // disable interrupts.
      tpSpi->INT = 0x00;
      tpSpi->LVL = 0x00;

      // enable tpSpi.
      tpSpi->GCR1 |= CN_SPI_ENABLE;

    }
    return true;

error_exit:
    if(tpSpi == g_ptSpi0Reg)
        Cpucfg_DisableLPSC(cn_PSC0_SPI0);
    else if(tpSpi == g_ptSpi1Reg)
        Cpucfg_DisableLPSC(cn_PSC1_SPI1);
    return false;
}

// =========================================================================
// �������ܣ�����Ƭѡ��Ч
// ���������tpSpi����������spi���ƽṹ�ĵ�ַ
//           ufCsNo��Ƭѡ��
// �����������
// ����ֵ  ����
// =========================================================================
void Spi_ActiveCS(volatile tagSpiReg * const tagpSpi,ufast_t ufCsNo)
{
    if(tagpSpi != NULL)
        tagpSpi->PC5 = 1<<ufCsNo;                       //����Ƭѡ
    return;
}

// =========================================================================
// �������ƣ�Spi_InActiveCS
// �������ܣ�����Ƭѡ��Ч
// ���������tpSpi����������spi���ƽṹ�ĵ�ַ
//           ufCsNo��Ƭѡ��
// �����������
// ����ֵ  ����
// =========================================================================
void Spi_InActiveCS(volatile tagSpiReg * const tpSpi,ufast_t ufCsNo)
{
    if(tpSpi != NULL)
        tpSpi->PC4 = 1<<ufCsNo;                         //����Ƭѡ
    return;
}

// =========================================================================
// �������ƣ�Spi_TxRx
// �������ܣ���spi�ڷ������ݣ�ͬʱ�������ݡ���spi�������ԣ��շ���ͬʱ���еġ�
// ���������tpSpi����������spi���ƽṹ�ĵ�ַ
//           ufCsNo��Ƭѡ��
//           u8pSrc��Դ��������ַ����ΪNULL
//           u32SrcLen��Դ���������ȣ�����Ҫ���͵����ݳ���
//           u32DstLen�����ջ��������ȣ�Ҳ���������յ���������
//           u32DstOffset������ƫ�ƣ�������һ���������ݺ󣬲ſ�ʼ���գ������Ҫ
//              �ȷ���������������ٴӴ����������ݵķ����ǳ����ã���Ϊ������ʱ��
//              ���������Ǳ��붪���ġ�
// ���������u8pDst�����ջ�������ַ����ΪNULL
// ����ֵ  ���յ���������
// =========================================================================
u32 Spi_TxRx(volatile tagSpiReg * const tpSpi,u8 *u8pSrc,u32 u32SrcLen,
                u8 *u8pDst,u32 u32DstLen,u32 u32DstOffset)
{
   u32 u32Sent=0,u32Received=0;

    if(((u8pSrc==NULL) && (u32SrcLen != 0))
                || ((u8pDst==NULL) && (u32DstLen != 0)))
        return 0;
    if (tpSpi != NULL)
    {
        tpSpi->BUF;                               //��ս��ռĴ���
        while((u32Sent < u32SrcLen)
                    || (u32Received < (u32DstLen + u32DstOffset) ))
        {
            while (tpSpi->BUF & TXFULL) {}            //�ȴ����ͼĴ�����
            if(u32Sent < u32SrcLen)
            {
                tpSpi->DAT0 = u8pSrc[u32Sent];              //��������
                while(tpSpi->BUF & RXEMPTY) {}    //�ȴ�������Ч
                u32Sent++;
            }else
            {
                tpSpi->DAT0 = 0;                  //���Ϳ����ݣ����ڲ�������ʱ��
                while(tpSpi->BUF & RXEMPTY) {}    //�ȴ�������Ч
            }
            if(u32DstLen != 0)
            {
                //��Ҫ��������
                if((u32Received >= u32DstOffset)
                            &&(u32Received <(u32DstLen+u32DstOffset)))
                {
                    u8pDst[u32Received - u32DstOffset] = (u8)tpSpi->BUF;
                }
                u32Received++;
            }
        }
    }

    return (u32Received - u32DstOffset);
}

