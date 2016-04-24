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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
// �ļ���     ��cpu_peri_spi.c
// ģ������: SPIģ��ײ�Ӳ������ģ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 9/12.2014
// =============================================================================

#include "config-prj.h"
#include "string.h"
#include "cpu_peri.h"
#include "spibus.h"

// =============================================================================
#define CN_SPI_BASE        0x40008000

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
//	struct semaphore_LCB *pDrvPostSemp;	//�ź���
	u32 SendDataLen;
	u32 RecvDataLen;
	u32 RecvOffset;
//	u8 CurrentCS;
//	u8 block_option;
};
// =============================================================================
//����SPI���ƿ���жϾ�̬��
#define CN_SPI_BUF_LEN      		128
#define CN_SPI_DMA_BUF_LEN  		128
static struct tagSPI_CB s_SPI_CB;
static u8 s_SPI_Buf[CN_SPI_BUF_LEN];
static u8 s_SPI_DmaSendBuf[CN_SPI_DMA_BUF_LEN];
static u8 s_SPI_DmaRecvBuf[CN_SPI_DMA_BUF_LEN];
struct SPI_IntParamSet IntParamset0;

static const Pin SPI_PINS[] = {
	{PIO_PA12A_MISO,  PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP},
	{PIO_PA13A_MOSI,  PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP},
	{PIO_PA14A_SPCK,  PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP},
	{PIO_PA11A_NPCS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP},
	{PIO_PB14A_NPCS1, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
};

//static u8 s_ChipSelect = 0x00;

// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
	Reg->SPI_IER = IntSrc;
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
	Reg->SPI_IDR = IntSrc;
}

// =============================================================================
// ���ܣ�SPIʱ�����ú�����ʱ����ԴΪ50M����SCR��CPSR��Ƶ�õ�ʱ�ӣ�ʱ�Ӽ��㹫ʽΪ��
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// ������spi_dev���豸���
//      spiclk�������õ�ʱ���ٶȣ���λΪHz
// ���أ�true=�ɹ���false=ʧ��
// ˵�����������̶�SCR = 1;�������ֲᣬCPSDVSR����Ϊ2-254��ż������ˣ�Ƶ�ʷ�ΧΪ
//		 12.5M ~ 100kHz
// =============================================================================
static void __SPI_SetClk(volatile tagSpiReg *Reg,u8 cs,u32 Fre)
{
	if(cs > CN_CS_MAX)
		return;

	Reg->SPI_CSR[cs] |= SPI_CSR_SCBR(CN_CFG_MCLK/Fre);
}

// =============================================================================
// ���ܣ�SPI���ų�ʼ������ʼ��Ϊ����ʹ�ã�����ʱ�Ӻ�����
// ������spi_no,SPI��������
// ���أ���
// =============================================================================
static void __SPI_GpioInit(void)
{
//	GPIO_CfgPinFunc(SPI_PINS,PIO_LISTSIZE(SPI_PINS));
	PIO_Configure(SPI_PINS,PIO_LISTSIZE(SPI_PINS));
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __SPI_Config(volatile tagSpiReg *Reg,u8 cs,tagSpiConfig *ptr)
{
	if(cs > CN_CS_MAX)
		return;
	__SPI_SetClk(Reg,cs,ptr->Freq);

	//set the PHA
	if(ptr->Mode & SPI_CPHA)
	{
		Reg->SPI_CSR[cs] |= SPI_CSR_NCPHA;
	}
	else
	{
		Reg->SPI_CSR[cs] &= ~SPI_CSR_NCPHA;
	}
	//set the cpol
	if(ptr->Mode & SPI_CPOL)
	{
		Reg->SPI_CSR[cs] |= SPI_CSR_CPOL;
	}
	else
	{
		Reg->SPI_CSR[cs] &= ~SPI_CSR_CPOL;
	}

	if(ptr->CharLen <= 16 && ptr->CharLen >= 4)
	{
		Reg->SPI_CSR[cs] &= ~SPI_CSR_BITS_Msk;
		Reg->SPI_CSR[cs] |= ptr->CharLen - 8;
	}
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __SPI_HardConfig(void)
{
    tagSpiReg *Reg;
    u32 temp,Fre;

    Reg = (tagSpiReg *)CN_SPI_BASE;
    //����SPIʹ��GPIO����
    __SPI_GpioInit();

    Reg->SPI_CR = SPI_CR_SPIDIS|SPI_CR_SWRST;			//��λ������
    Reg->SPI_MR = SPI_MR_MSTR|SPI_MR_MODFDIS|SPI_MR_DLYBCS(128);

	//Ĭ��ʹ��4M������
	Fre = 4*1000*1000;
    for(temp = 0; temp < 4; temp ++)
    {
    	Reg->SPI_CSR[temp] = SPI_CSR_CSAAT|SPI_CSR_SCBR(CN_CFG_MCLK/Fre);
    }
    //ʹ��DMA��ʽ
//    Reg->SPI_PTCR = SPI_PTCR_RXTEN | SPI_PTCR_TXTEN;
	Reg->SPI_TCR   = 0;
	Reg->SPI_TNCR  = 0;
    Reg->SPI_RCR   = 0;
	Reg->SPI_RNCR  = 0;

    Reg->SPI_CR = SPI_CR_SPIEN;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __SPI_IntConfig(void)
{
	u8 IntLine;

	IntLine = CN_INT_LINE_SPI;
    //�ж��ߵĳ�ʼ��
	u32 SPI_ISR(ufast_t IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_PRE);
    Int_IsrConnect(IntLine,SPI_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч,���������о��������Ƭѡ�Ĵ�������ֱ�ӷ���
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SPI��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
//	s_ChipSelect = cs;
	Reg->SPI_TDR |= SPI_TDR_PCS(cs);
	Reg->SPI_MR  &= ~SPI_MR_PCS(0x0F);
	Reg->SPI_MR  |= SPI_MR_PCS(cs);
	return true;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
	Reg->SPI_CR = SPI_CR_LASTXFER | SPI_CR_SPIDIS;
	return false;
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __SPI_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
	s32 result = 1;
	if((u32)Reg != CN_SPI_BASE)
		return 0;

	switch(cmd)
	{
	case CN_SPI_SET_CLK:
		__SPI_SetClk(Reg,data1,data2);
		break;
	case CN_SPI_CS_CONFIG:
		__SPI_Config(Reg,data1,(tagSpiConfig *)data2);
		break;
	case CN_SPI_SET_AUTO_CS_EN:
		// Ӳ����������Զ�����CS������
		break;
	case CN_SPI_SET_AUTO_CS_DIS:
		break;
	default:
		break;
	}
	return result;
}

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static u32 __SPI_Read( tagSpiReg * Reg )
{
    while ( (Reg->SPI_SR & SPI_SR_RDRF) == 0 ) ;

    return Reg->SPI_RDR & 0xFFFF ;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��dwNpcs,Ƭѡ��wData��д����
// ���أ���
// =============================================================================
static void __SPI_Write( tagSpiReg * Reg, uint32_t dwNpcs, uint16_t wData )
{
    /* Send data */
    while ( (Reg->SPI_SR & SPI_SR_TXEMPTY) == 0 ) ;
    Reg->SPI_TDR = wData | SPI_PCS( dwNpcs ) ;
    while ( (Reg->SPI_SR & SPI_SR_TDRE) == 0 ) ;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��д����
// ������Reg,�Ĵ�����ַ��srcAddr,����Դָ�룻wrSize��д��������destAddr������������
//       ��ַָ�룻rdSize������������recvoff,��ƫ�ƣ���δ��
// ���أ�true,��ȷ��false,����
// =============================================================================
static bool_t __SPI_TxRxPoll(tagSpiReg *Reg,u8 *srcAddr,u32 wrSize,
		u8 *destAddr, u32 rdSize,u32 recvoff)
{
	u32 i;
	if(!srcAddr)
		return false;

	Reg->SPI_CR |= SPI_CR_SPIEN;
	for (i=0;i<(wrSize+rdSize);i++)
	{
		if (i<wrSize)
		{
			__SPI_Write(Reg,0x0,srcAddr[i]);
			__SPI_Read(Reg);
		}
		else if((i>=wrSize)&&(i<(wrSize+rdSize)))
		{
		  __SPI_Write(Reg,0x0,0);
		  if(destAddr)
		  	destAddr[i-wrSize] = __SPI_Read(Reg);
		}
	}

	return true;
}

// =============================================================================
// ���ܣ�SPI�ײ��������ͽ��պ�������������ɵĹ������£�
//       1.����ṹ��̬�����������ж���ʹ�ã�
//       2.������صļĴ�����ʹ�䴦�ڷ��ͽ�������״̬;
//       3.�����жϣ����жϷ���������������շ���
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       sendlen,�������ݳ��ȣ��ֽ�
//       recvlen,�������ݳ��ȣ��ֽ�
//       recvoff,����������ϡ�����ӽ��յ��ĵڼ����ֽڿ�ʼ�������ݣ��ֽ�
//       block_opt,����ѡ�trueʱ��Ϊ��������������ʽ��ײ������ͷ��ź���������CS
//       spi_semp,�ײ������Ҫ�ͷŵ��ź������Ƿ��ͷ���block_opt����
// ���أ�true,�޴���;false,ʧ��
// =============================================================================
static bool_t __SPI_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
								u32 recvoff)
{
	struct SPI_IntParamSet *Param=NULL;
	u32 temp;

	if((u32)Reg != CN_SPI_BASE)
		return false;

	Param = &IntParamset0;
	Param->SendDataLen = sendlen;
	Param->RecvDataLen = recvlen;
	Param->RecvOffset  = recvoff;

    __SPI_IntDisable(Reg,SPI_IDR_TXBUFE|SPI_IDR_ENDRX);

    Reg->SPI_CR |= SPI_CR_SPIEN;
    //���������ͨ�ţ����Ƿ�������ٽ��գ�������ȷ���
    if(sendlen > 0)
    {
		if(sendlen > CN_SPI_DMA_BUF_LEN)
			temp = CN_SPI_DMA_BUF_LEN;
		else
			temp = sendlen;

		SPI_PortRead(&s_SPI_CB,s_SPI_DmaSendBuf,temp);
		Reg->SPI_TPR = (u32)s_SPI_DmaSendBuf;
		Reg->SPI_TCR = temp;
		Reg->SPI_RPR = (u32)s_SPI_DmaRecvBuf;
		Reg->SPI_RCR = temp;
		Reg->SPI_PTCR = SPI_PTCR_TXTEN|SPI_PTCR_RXTEN;
		__SPI_IntEnable(Reg,SPI_IER_TXBUFE);
    }
    else
    {
		if(recvlen > CN_SPI_DMA_BUF_LEN)
			temp = CN_SPI_DMA_BUF_LEN;
		else
			temp = recvlen;
		Reg->SPI_TPR = (u32)s_SPI_DmaSendBuf;
		Reg->SPI_TCR = temp;
		Reg->SPI_RPR = (u32)s_SPI_DmaRecvBuf;
		Reg->SPI_RCR = temp;
		Reg->SPI_PTCR = SPI_PTCR_TXTEN|SPI_PTCR_RXTEN;
		__SPI_IntEnable(Reg,SPI_IER_RXBUFF);
    }

	return true;
}
// =============================================================================
// ���ܣ�SPI�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.�����շ�һ�壬��˷��Ϳ��ж���Ҳ�յ������ݣ�
//       2.SPI_PortRead()�ж���������ʱ��˵�����͵������Ѿ���ɣ��ɹط����ж�
//       3.��param->RecvDataLen����0������Ҫ�������ݣ���ӽ��յ���������offset�ֽ�
//         ��ʼ�洢���ݣ�������SPI_PortRead()
//       4.
// ������spi_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
u32 SPI_ISR(ufast_t IntLine)
{
	struct tagSPI_CB *pSCB=NULL;
	struct SPI_IntParamSet *param;
	tagSpiReg *Reg;
    u32 spi_sr,spi_imr,temp;

    if(IntLine != CN_INT_LINE_SPI)
    	return 0;

    pSCB = &s_SPI_CB;
	Reg = (tagSpiReg *)CN_SPI_BASE;
	param = &IntParamset0;

	spi_sr = Reg->SPI_SR;
    spi_imr = Reg->SPI_IMR;
    //�����ж�
    if( (spi_imr & SPI_IMR_TXBUFE) && (spi_sr & SPI_SR_TXBUFE))	//ʹ���˷����жϣ���TXRIS
    {
    	Reg->SPI_PTCR = SPI_PTCR_TXTDIS|SPI_PTCR_RXTDIS;
    	temp = SPI_PortRead(pSCB,s_SPI_DmaSendBuf,CN_SPI_DMA_BUF_LEN);//�Ƿ���������Ҫ����

    	if(temp >0)
    	{
    		param->SendDataLen -= CN_SPI_DMA_BUF_LEN;
    		param->RecvOffset  -= CN_SPI_DMA_BUF_LEN;

    		Reg->SPI_TPR = (u32)s_SPI_DmaSendBuf;
    		Reg->SPI_TCR = temp;
    		Reg->SPI_RPR = (u32)s_SPI_DmaRecvBuf;
    		Reg->SPI_RCR = temp;
    		Reg->SPI_PTCR = SPI_PTCR_TXTEN|SPI_PTCR_RXTEN;
    	}
    	else											//�������ͽ���
    	{
    		//�ط����ж�
    		while(!(Reg->SPI_SR & SPI_SR_TXEMPTY));
    		while(Reg->SPI_SR & SPI_SR_RDRF)
    		{
    			temp = Reg->SPI_RDR;
    		}
    		__SPI_IntDisable(Reg,SPI_IDR_TXBUFE);
    		param->RecvOffset -= param->SendDataLen;
    		param->SendDataLen = 0;
    		//��������ˣ��ж��Ƿ���Ҫ����
    		if(param->RecvDataLen > 0)
    		{
    			//MASTERģʽ�£����ͺͽ���ͬʱ����
    			memset(s_SPI_DmaSendBuf,0x00,CN_SPI_DMA_BUF_LEN);
    			if(param->RecvDataLen > CN_SPI_DMA_BUF_LEN)
    				temp = CN_SPI_DMA_BUF_LEN;
    			else
    				temp = param->RecvDataLen;

    			Reg->SPI_TPR = (u32)s_SPI_DmaSendBuf;
    			Reg->SPI_TCR = temp;
    			Reg->SPI_RPR = (u32)s_SPI_DmaRecvBuf;
    			Reg->SPI_RCR = temp;

    			//ͬʱʹ�ܽ��պͷ���DMA
    			Reg->SPI_PTCR = SPI_PTCR_TXTEN | SPI_PTCR_RXTEN;
    			__SPI_IntEnable(Reg,SPI_IER_RXBUFF);	//ʹ�ܽ����ж�
    		}
    	}
    }
    else if((spi_imr & SPI_IMR_RXBUFF) && (spi_sr & SPI_SR_RXBUFF))    //�����ж�
    {
    	Reg->SPI_PTCR = SPI_PTCR_TXTDIS|SPI_PTCR_RXTDIS;
    	//��������˶�������
    	if(param->RecvDataLen > CN_SPI_DMA_BUF_LEN)
    		temp = CN_SPI_DMA_BUF_LEN;
    	else
    		temp = param->RecvDataLen;

    	SPI_PortWrite(pSCB,s_SPI_DmaRecvBuf,temp);
    	param->RecvDataLen -= temp;

    	//�ж��Ƿ����������
    	if(param->RecvDataLen > 0)
    	{
			if(param->RecvDataLen > CN_SPI_DMA_BUF_LEN)
				temp = CN_SPI_DMA_BUF_LEN;
			else
				temp = param->RecvDataLen;
			Reg->SPI_TPR = (u32)s_SPI_DmaSendBuf;
			Reg->SPI_TCR = temp;
			Reg->SPI_RPR = (u32)s_SPI_DmaRecvBuf;
			Reg->SPI_RCR = temp;

			//ͬʱʹ�ܽ��պͷ���DMA
			Reg->SPI_PTCR = SPI_PTCR_TXTEN | SPI_PTCR_RXTEN;
    	}
    	else 					//���������Ѿ����
    	{
    		param->RecvDataLen = 0;
    		__SPI_IntDisable(Reg,SPI_IDR_RXBUFF);
    		Reg->SPI_PTCR = SPI_PTCR_TXTDIS | SPI_PTCR_RXTDIS;
    	}
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {

    }

    return 0;
}

// =============================================================================
// ���ܣ�SPI�ײ������ĳ�ʼ�����������SPI���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�SPI_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��SPI�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����SPIBusAdd��SPIBusAdd_r�������߽�㣻
// ��������
// ���أ���ʼ���ɹ�����1��ʧ�ܷ���0
// =============================================================================
bool_t SPI_Init(void)
{
	struct tagSPI_Param SPI_Config;

	SPI_Config.BusName 		   	= "SPI";
	SPI_Config.SPIBuf 		   	= (u8*)&s_SPI_Buf;
	SPI_Config.SPIBufLen 	  	= CN_SPI_BUF_LEN;
	SPI_Config.SpecificFlag 	= CN_SPI_BASE;
	SPI_Config.MultiCSRegFlag   = true;
	SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
	SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
	SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
	SPI_Config.pCsInActive 		= (CsInActiveFunc)__SPI_BusCsInActive;
	SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

	__SPI_HardConfig();
	__SPI_IntConfig();
	PMC_EnablePeripheral(CN_INT_LINE_SPI);

	if(NULL == SPI_BusAdd_r(&SPI_Config,&s_SPI_CB))
		return 0;
	return 1;
}



