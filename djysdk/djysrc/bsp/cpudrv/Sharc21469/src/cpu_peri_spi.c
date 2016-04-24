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
//����ģ��: SPI����ģ��
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
#include "stdint.h"
#include "cpu_peri_spi.h"

#include <cdef21469.h>
#include <def21469.h>
#include <sru.h>
#include <sysreg.h>

//SPI default setting
 tagSpiConfig   pg_spi_Config = {   SPI_MODE_MASTER,    //Master Mode
                                    SPI_SHIFT_MSB,      //MSBF
                                    1,                  //CLKPL = 1
                                    1,                  //CPHASE = 1
                                    100000,             //SPI BAUD = 100k
                                    8};                 //char_len = 8bits

// ----��ʼ��SPIģ��---------------------------------------------
//���ܣ�Ŀǰ����֧�ֲ�����SPI1ΪMaster����DMA���ܣ���MODE3�¹�������
//      ���ò�������Ĭ�����ã���SPI�����ʿ�����Ӧ�ó���ͨ��tagspiConfig.spi_clk���޸�
//������SPIx            SPIģ��ѡ��
//      *tagpInConfig   SPI�������ýṹ��tagspiConfig
//���أ�true=�ɹ���false=ʧ��
// --------------------------------------------------------------
bool_t Spi_Init(volatile tag_SpiReg * tpSpi, tagSpiConfig *tagpInConfig)
{
    u32 spi_div16=0;

    if((tpSpi != g_ptSpi0Reg)&&(tpSpi != g_ptSpi1Reg))//SPI1δ����
        return false;

    if (tagpInConfig != NULL)
    {
            spi_div16 = (CN_CFG_PCLK1/(tagpInConfig->freq*4))&0x7fff;

            tpSpi->rSPIDMAC = 0;            //no DMA
            tpSpi->rSPIBAUD = spi_div16<<1; //SPI Baud
            tpSpi->rSPIFLG = 0xF80;         //reset value

            //����˵����
            //ģʽ3����DMA��CSƬѡλ��IO���ƣ���ʹ��SPIģ����Զ�Ƭѡ����
            //TIMOD=01��SPIMS-Master��WL8-8λ���ݣ�MSBF-MSBλ����
            //OPD-��©ģʽ��CLKPL-Clock Polarity��CPHASE-Clock Phase
            tpSpi->rSPICTL = SPIEN|SENDZ|TIMOD1;

            if(tagpInConfig->tagMode == SPI_MODE_MASTER)
                tpSpi->rSPICTL |= SPIMS;

            if(tagpInConfig->tagShiftDir == SPI_SHIFT_MSB)
                tpSpi->rSPICTL |= MSBF;

            if(tagpInConfig->polarity)
                tpSpi->rSPICTL |= CLKPL;

            if(tagpInConfig->phase)
                tpSpi->rSPICTL |= CPHASE;

            if(tagpInConfig->char_len == 8)
                tpSpi->rSPICTL |= WL8;
            else if(tagpInConfig->char_len == 16)
                tpSpi->rSPICTL |= WL16;
            else if(tagpInConfig->char_len == 32)
                tpSpi->rSPICTL |= WL32;
            else
                return false;

            return true;
    }
    return false;
}

// ----SPIģ���æ-----------------------------------------------
//���ܣ���SPIFλ
//������SPIx    SPIģ��ѡ��
//���أ�true=�ɹ���false=ʧ��
// --------------------------------------------------------------
bool_t SPI_WaitFor(volatile tag_SpiReg * tpSpi)
{
    int nTimeout = 10000;
    // status updates can be delayed up to 10 cycles
    // so wait at least 10 cycles before even
    // checking them
    int n;

    if((tpSpi != g_ptSpi0Reg)&&(tpSpi != g_ptSpi1Reg))
        return false;

    // make sure nothing is waiting to be sent
    while( !(SPIF & tpSpi->rSPISTAT) )
    {
        if( nTimeout-- < 0 )
        {
            return false;
        }
    }

    return true;

}
// ----SPIģ���д״̬-------------------------------------------
//���ܣ�ȷ��д���ݴ��ھ���״̬
//������SPIx    SPIģ��ѡ��
//���أ�true=�ɹ���false=ʧ��
// --------------------------------------------------------------
bool_t SPI_CheckTxReady(volatile tag_SpiReg * tpSpi)
{
    int nTimeout = 10000;
    // status updates can be delayed up to 10 cycles
    // so wait at least 10 cycles before even
    // checking them
    int n;

    if((tpSpi != g_ptSpi0Reg)&&(tpSpi != g_ptSpi1Reg))
        return false;

    // make sure nothing is waiting to be sent
    while( (TXS & tpSpi->rSPISTAT) )
    {
        if( nTimeout-- < 0 )
        {
            return false;
        }
    }

    return true;

}
// ----SPIģ����״̬-------------------------------------------
//���ܣ�ȷ��Ӳ�������������ݿɶ�
//������SPIx    SPIģ��ѡ��
//���أ�true=�ɹ���false=ʧ��
// --------------------------------------------------------------
bool_t SPI_CheckRxReady(volatile tag_SpiReg * tpSpi)
{
    int nTimeout = 10000;
    // status updates can be delayed up to 10 cycles
    // so wait at least 10 cycles before even
    // checking them
    int n;

    if((tpSpi != g_ptSpi0Reg)&&(tpSpi != g_ptSpi1Reg))
        return false;

    // make sure something is waiting to be read
    while( !(RXS & tpSpi->rSPISTAT) )
    {
        if( nTimeout-- < 0 )
        {
            return false;
        }
    }

    return true;

}

// ----SPIģ��Ƭѡ����-------------------------------------------
//���ܣ�Ŀǰ���ṩSPI1Ƭѡ��������WB9260��
//������SPIx    SPIģ��ѡ��
//      tgCsSel Ƭѡ�ź�
//���أ�true=�ɹ���false=ʧ��
// --------------------------------------------------------------
bool_t Spi_ActiveCS(volatile tag_SpiReg * tpSpi,ufast_t ufCsNo)
{
    if(tpSpi == g_ptSpi0Reg)
    {
        if( true != SPI_WaitFor(tpSpi) )
        {
            return false;
        }

        switch (ufCsNo)
        {
           case SPI_CS0:
              sysreg_bit_clr( sysreg_FLAGS, FLG4 ); //����ss0�� FLASH
              break;
           case SPI_CS1:
              sysreg_bit_clr( sysreg_FLAGS, FLG5 ); //����ss1�� FLASH
              break;
           case SPI_CS2:
              sysreg_bit_clr( sysreg_FLAGS, FLG6 ); //����ss2�� RTC
              break;
           default:
              return false;
        }
        return true;
    }
    else if(tpSpi == g_ptSpi1Reg)
    {
        return false;
    }
    else
        return false;

}

// ----SPIģ��Ƭѡ����-------------------------------------------
//���ܣ�Ŀǰ���ṩSPI1Ƭѡ��������WB9260��
//������SPIx    SPIģ��ѡ��
//      tgCsSel Ƭѡ�ź�
//���أ�true=�ɹ���false=ʧ��
// --------------------------------------------------------------
bool_t Spi_InActiveCS(volatile tag_SpiReg * tpSpi,ufast_t ufCsNo)
{
    if(tpSpi == g_ptSpi0Reg)
    {
        if( true != SPI_WaitFor(tpSpi) )
        {
            return false;
        }

        switch(ufCsNo)
        {
           case SPI_CS0:
              sysreg_bit_set( sysreg_FLAGS, FLG4 );    //����ss0��  FLASH
              break;
           case SPI_CS1:
              sysreg_bit_set( sysreg_FLAGS, FLG5 );    //����ss1��  FLASH
              break;
           case SPI_CS2:
              sysreg_bit_set( sysreg_FLAGS, FLG6 );    //����ss2��  RTC
              break;
           default:
              return false;
        }
    }
    else if(tpSpi == g_ptSpi1Reg)
    {
        return false;
    }
    else
        return false;

    return true;
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
u32 Spi_TxRx(volatile tag_SpiReg * const tpSpi,u8 *u8pSrc,u32 u32SrcLen,
                u8 *u8pDst,u32 u32DstLen,u32 u32DstOffset)
{
    u8 temp_data;
    u32 u32Sent=0,u32Received=0;

    if((tpSpi != g_ptSpi0Reg)&&(tpSpi != g_ptSpi1Reg))
        return false;

    if(((u8pSrc==NULL) && (u32SrcLen != 0))
                || ((u8pDst==NULL) && (u32DstLen != 0)))
        return false;

    if( true != SPI_WaitFor(tpSpi) )
    {
        return false;
    }

    while((u32Sent < u32SrcLen)
                    || (u32Received < (u32DstLen + u32DstOffset) ))
    {
        if( true != SPI_CheckTxReady(tpSpi) )
            return false;

        if(u32Sent < u32SrcLen)
        {
            //������Ч����
            tpSpi->rTXSPI = u8pSrc[u32Sent];
            u32Sent++;
        }
        else
            tpSpi->rTXSPI = 0xFF;

        if( true != SPI_CheckRxReady(tpSpi) )
            return false;

        temp_data = tpSpi->rRXSPI;

        if(u32DstLen != 0)
        {
            if((u32Received >= u32DstOffset)
                        &&(u32Received <(u32DstLen+u32DstOffset)))
            {
                //������Ч����
                u8pDst[u32Received - u32DstOffset] = temp_data;
            }
            u32Received++;
        }
    }

    return (u32Received - u32DstOffset);
}
