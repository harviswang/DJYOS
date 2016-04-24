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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: IICģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 05/12.2014
// =============================================================================
#include "cpu_peri.h"
#include <string.h>
#include "endian.h"
#include "iicbus.h"

//==============================================================================
#define CN_IIC_REGISTER_BADDR0        0x40018000
#define CN_IIC_REGISTER_BADDR1        0x4001C000

//�ж���ʹ�õ��������ͽṹ��
struct tagIIC_IntParamSet
{
	struct tagSemaphoreLCB *pDrvPostSemp;	//�ź���
	u32 TransCount;						//����������������
	u32 TransTotalLen;
};

//����IIC��GPIO��������
static const Pin IIC_GPIO_PIN[]={
		{PIO_PA3A_TWD0,PIOA,ID_PIOA,PIO_PERIPH_A,PIO_DEFAULT},
		{PIO_PA4A_TWCK0,PIOA,ID_PIOA,PIO_PERIPH_A,PIO_DEFAULT}
};

//���徲̬����
static struct tagIIC_CB s_tIIC0_CB;
#define IIC0_BUF_LEN  		128
#define IIC0_DMA_BUF_LEN 	64
static u8 s_IIC0Buf[IIC0_BUF_LEN];
static u8 s_IIC0DmaBuf[IIC0_DMA_BUF_LEN];
static struct tagIIC_IntParamSet IntParamset0;

static struct tagIIC_CB s_tIIC1_CB;
#define IIC1_BUF_LEN  		128
#define IIC1_DMA_BUF_LEN 	64
static u8 s_IIC1DmaBuf[IIC1_DMA_BUF_LEN];
static u8 s_IIC1Buf[IIC1_BUF_LEN];
static struct tagIIC_IntParamSet IntParamset1;

//��̬�������Ƿ�ʹ��DMA��ʽ���ͽ��գ�Ĭ��Ϊ��
static volatile bool_t s_IIC_DmaUsed[2] = {false,false};

// =============================================================================
// ���ܣ�����Ӧ��I2C������������λ
// ������iic��Ӧ����Ĵ����Ļ�ַ
// ���أ���
// =============================================================================
static void __IIC_Reset(volatile tagI2CReg *reg)
{
    reg->TWI_CR = TWI_CR_SWRST;
}

// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
//       IntSrc,�ж�Դ
// ����: ��
// =============================================================================
static void __IIC_IntDisable(volatile tagI2CReg *reg,u32 IntSrc)
{
    reg->TWI_IDR = IntSrc;
}

// =============================================================================
// ����: ��ֹiic�ж�
// ����: reg,�������ļĴ�����ָ��
//       IntSrc,�������ж�Դ
// ����: ��
// =============================================================================
static void __IIC_IntEnable(volatile tagI2CReg *reg,u32 IntSrc)
{
    reg->TWI_IER = IntSrc;
}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_GenerateStop(volatile tagI2CReg *reg)
{
	reg->TWI_CR = TWI_CR_STOP;
}

// =============================================================================
// ���ܣ�IICʱ�����ú�����ʱ����ԴΪMCK�����㷽���ɲο������ֲ�
// ������reg���豸���
//      iicclk�������õ�ʱ���ٶȣ���׼��ʱ����50KHz,100KHz,200KHz
// ���أ�true=�ɹ���false=ʧ��
// =============================================================================
static void __IIC_SetClk(volatile tagI2CReg *reg,u32 iicclk)
{
    uint32_t dwCkDiv = 0 ;
    uint32_t dwClDiv ;
    uint32_t dwOk = 0 ;

    /* Configure clock */
    while ( !dwOk )
    {
        dwClDiv = ((CN_CFG_MCLK/ (2 * iicclk)) - 4) / (1<<dwCkDiv) ;

        if ( dwClDiv <= 255 )
        {
            dwOk = 1 ;
        }
        else
        {
            dwCkDiv++ ;
        }
    }

    reg->TWI_CWGR = 0 ;
    reg->TWI_CWGR = (dwCkDiv << 16) | (dwClDiv << 8) | dwClDiv ;
}

// =============================================================================
// ���ܣ�IIC�����DMA���ú��������ÿ�����ʹ��DMA��ʽ
// ������
// ���أ�
// =============================================================================
static void __IIC_DMA_Config(volatile tagI2CReg* Reg,u32 cmd)
{
	u8 Port;
	if((u32)Reg == CN_IIC_REGISTER_BADDR0)
		Port = CN_IIC0;
	else
		Port = CN_IIC1;

	if(cmd == CN_IIC_DMA_USED)				//����ʹ��DMA��ʽ���ͺͽ���
	{
		s_IIC_DmaUsed[Port] = true;			//���ʹ��DMA
		//�رղ�ʹ��DMAʱ���ж�
		__IIC_IntDisable(Reg,TWI_IDR_RXRDY|TWI_IDR_TXRDY);
		Reg->TWI_PTCR = TWI_PTCR_RXTEN | TWI_PTCR_TXTEN;
	}
	else									//����ʹ�÷�DMA��ʽ���ͺͽ���
	{
		s_IIC_DmaUsed[Port] = false;
		__IIC_IntDisable(Reg,TWI_IDR_ENDTX|TWI_IDR_ENDRX);
		Reg->TWI_PTCR = TWI_PTCR_RXTDIS | TWI_PTCR_TXTDIS;
	}
	//����DMA����
	Reg->TWI_TCR   = 0;
	Reg->TWI_TNCR  = 0;
    Reg->TWI_RCR   = 0;
	Reg->TWI_RNCR  = 0;
}

// =============================================================================
// ���ܣ�IIC���ų�ʼ������ʼ��Ϊ����ʹ��
// ������iic_no,IIC��������
// ���أ���
// =============================================================================
static void __IIC_GpioInit(u32 iic_no)
{
    if(iic_no == CN_IIC0)
    {
//        GPIO_CfgPinFunc(IIC_GPIO_PIN,PIO_LISTSIZE(IIC_GPIO_PIN));
    	PIO_Configure(IIC_GPIO_PIN,PIO_LISTSIZE(IIC_GPIO_PIN));
    }
    else if(iic_no == CN_IIC1)
    {
    }
}

// =============================================================================
// ����: IICĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
//       iicno,IIC��������
// ����: ��
// =============================================================================
static void __IIC_HardConfig(ptu32_t RegBaseAddr,u8 iicno)
{
	volatile tagI2CReg *reg;
	reg = (volatile tagI2CReg *)RegBaseAddr;

    __IIC_GpioInit(iicno);

    reg->TWI_CR = TWI_CR_SWRST;//reset the twi
    reg->TWI_RHR;        //clear the rhr
    //config master mode
    reg->TWI_CR = TWI_CR_MSDIS;
    reg->TWI_CR = TWI_CR_SVDIS;

    /* Set master mode */
    reg->TWI_CR = TWI_CR_MSEN;

    //config the SCK
    reg->TWI_CWGR = 0 ;
    reg->TWI_CWGR = (1 << 16) | (248 << 8) | 248 ;//100KHz
}

// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       ISR,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntConfig(u32 IntLine,u32 (*ISR)(ufast_t))
{
    //�ж��ߵĳ�ʼ��
    Int_IsrConnect(IntLine,ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ�������������λ�Ѿ����£��ú����ڲ��轫�õ�ַ��
//               ��һλ�����޸����Ķ�д����λ�����һ����д0����ʾд�����һ����д
//               1,��ʾ��;
//      mem_addr,�洢���ڲ���ַ,�ú����跢�͵�maddr_len�ֽڵ�����
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��
//      length,���͵������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      iic_semp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __IIC_GenerateWriteStart(volatile tagI2CReg *Reg,
									   u8 dev_addr,
									   u32 mem_addr,
									   u8 maddr_len,
									   u32 length,
									   struct tagSemaphoreLCB *iic_semp)
{
    struct tagIIC_CB *IIC;
    struct tagIIC_IntParamSet *IntParam;
    u8 ch,*pDmaBuf,Port,DmaBufLen;

    //ͨ��specific_flag��������������
    if((u32)Reg == CN_IIC_REGISTER_BADDR0)
    {
    	IntParam = &IntParamset0;
    	IIC = &s_tIIC0_CB;
    	pDmaBuf = s_IIC0DmaBuf;
    	Port = CN_IIC0;
    	DmaBufLen = IIC0_DMA_BUF_LEN;
    }
    else if((u32)Reg == CN_IIC_REGISTER_BADDR1)
    {
    	IntParam = &IntParamset1;
    	IIC = &s_tIIC1_CB;
    	pDmaBuf = s_IIC1DmaBuf;
    	Port = CN_IIC1;
    	DmaBufLen = IIC1_DMA_BUF_LEN;
    }
    else
    {
    	return false;
    }
    IntParam->TransTotalLen = length;
    IntParam->TransCount = 0;
    IntParam->pDrvPostSemp = iic_semp;

    Reg->TWI_MMR = 0;
    Reg->TWI_MMR = (maddr_len<<8)|(dev_addr<<16);
    Reg->TWI_IADR = 0;
    Reg->TWI_IADR = TWI_IADR_IADR(mem_addr);
    Reg->TWI_CR = TWI_CR_START;

    if(s_IIC_DmaUsed[Port] == false)		//ʹ�÷�DMA��ʽ ����
    {
		//����һ���ֽڵ�����
		while(!(Reg->TWI_SR & TWI_SR_TXRDY));
		IIC_PortRead(IIC,&ch,1);

		Reg->TWI_THR = ch;
		IntParam->TransCount++;
	    __IIC_IntEnable(Reg,TWI_IER_TXRDY);
    }
    else									//����DMA����ʱ�Ĳ���
    {
    	while(!(Reg->TWI_SR & TWI_SR_TXRDY));
    	ch = IIC_PortRead(IIC,pDmaBuf,DmaBufLen);
		Reg->TWI_TPR  = (u32)pDmaBuf;
		Reg->TWI_TCR  = ch;
		Reg->TWI_PTCR = TWI_PTCR_TXTEN;
        __IIC_IntEnable(Reg,TWI_IER_ENDTX);
    }

    return true;
}

// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ�������������λ�Ѿ����£��ú����ڲ��轫�õ�ַ��
//               ��һλ�����޸����Ķ�д����λ�����һ����д0����ʾд�����һ����д
//               1,��ʾ��;
//      mem_addr,�洢���ڲ���ַ,�ú����跢�͵�maddr_len�ֽڵ�����
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��
//      len,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//          �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      iic_semp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __IIC_GenerateReadStart( volatile tagI2CReg *Reg,
									   u8 dev_addr,
									   u32 mem_addr,
									   u8 maddr_len,
									   u32 length,
									   struct tagSemaphoreLCB *iic_semp)
{
    u8 DmaRcvLen,*pDmaBuf,Port,DmaBufLen;

    if((u32)Reg == CN_IIC_REGISTER_BADDR0)
    {
    	IntParamset0.TransTotalLen = length;
    	IntParamset0.TransCount = 0;
    	IntParamset0.pDrvPostSemp = iic_semp;				//iic_buf_semp
    	pDmaBuf = s_IIC0DmaBuf;
    	Port = CN_IIC0;
    	DmaBufLen = IIC0_DMA_BUF_LEN;
    }
    else if((u32)Reg == CN_IIC_REGISTER_BADDR1)
    {
    	IntParamset1.TransTotalLen = length;
    	IntParamset1.TransCount = 0;
    	IntParamset1.pDrvPostSemp = iic_semp;
    	pDmaBuf = s_IIC1DmaBuf;
    	Port = CN_IIC1;
    	DmaBufLen = IIC1_DMA_BUF_LEN;
    }
    else
    {
    	return false;
    }
    Reg->TWI_RHR;

    //TWI_MMR_IADRSZ_2_BYTE; len ; device addr
    Reg->TWI_MMR = 0;
    Reg->TWI_MMR = (maddr_len<<8)|(TWI_MMR_MREAD)|(dev_addr<<16);
    Reg->TWI_IADR = 0;
    Reg->TWI_IADR = TWI_IADR_IADR(mem_addr);
    Reg->TWI_CR = TWI_CR_START;

    if(s_IIC_DmaUsed[Port] == false)
    	__IIC_IntEnable(Reg,TWI_IER_RXRDY|TWI_IER_TXRDY);
    else
    {
    	DmaRcvLen = length>DmaBufLen ? DmaBufLen:length;
		Reg->TWI_RPR  = (uint32_t)pDmaBuf;
		Reg->TWI_RCR  = DmaRcvLen;
		Reg->TWI_PTCR = TWI_PTCR_RXTEN;
        __IIC_IntEnable(Reg,TWI_IER_ENDRX);
    }

    return true;
}

// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateEnd(volatile tagI2CReg *Reg)
{
	if(((u32)Reg != CN_IIC_REGISTER_BADDR0) &&
			(u32)Reg == CN_IIC_REGISTER_BADDR1)
		return;

//    __IIC_IntDisable(Reg, TWI_IDR_TXRDY|TWI_IDR_RXRDY|TWI_IDR_TXCOMP);
	__IIC_IntDisable(Reg, TWI_IDR_TXRDY|TWI_IDR_RXRDY|
			TWI_IDR_ENDRX|TWI_IDR_ENDTX|TWI_IDR_TXCOMP);
    Djy_EventDelay(100);
    __IIC_GenerateStop(Reg);

    __IIC_Reset(Reg);
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __IIC_BusCtrl(volatile tagI2CReg *Reg,u32 cmd,u32 data1,u32 data2)
{
	if(((u32)Reg != CN_IIC_REGISTER_BADDR0) &&
			(u32)Reg != CN_IIC_REGISTER_BADDR1)
		return 0;

	switch(cmd)
	{
	case CN_IIC_SET_CLK:
		__IIC_SetClk(Reg,data1);
		break;
	case CN_IIC_DMA_USED:
	case CN_IIC_DMA_UNUSED:
		__IIC_DMA_Config(Reg,cmd);
		break;
	default:
		break;
	}
	return 1;
}

// =============================================================================
// ���ܣ�IIC�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.ÿ���������һ���������ֽڷ���һ���жϣ�
//       2.���ж���ж�ʹ��ͬһ���жϺţ�������ݾ����������ʹ�õ����ĸ��жϣ�
//       3.���жϱ�־�����ж�ACK�źţ�ÿ��д�ֽڣ�������������Ӧ�޸ģ�
//       4.���մﵽ������һ���ֽ�ʱ�������ò�����ACK�źţ�
//       5.���ջ������ʱ����post�ź���IntParam->pDrvPostSemp��
//       6.���ջ������ʱ�������ֹͣʱ��
// ������i2c_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
static u32 __IIC_ISR(ufast_t IntLine)
{
    static struct tagIIC_CB *ICB;
    static struct tagIIC_IntParamSet *IntParam;
    tagI2CReg *reg;

    u8 ch,Port,*PDmaBuf;
    u32 twi_sr,twi_imr,DmaBufLen,temp;

    if(IntLine == CN_INT_LINE_TWI0)
    {
        ICB = &s_tIIC0_CB;
        IntParam = &IntParamset0;
        reg = (tagI2CReg *)CN_IIC_REGISTER_BADDR0;
        Port = CN_IIC0;
        PDmaBuf = s_IIC0DmaBuf;
        DmaBufLen = IIC0_DMA_BUF_LEN;
    }
    else if(IntLine == CN_INT_LINE_TWI1)
    {
        ICB = &s_tIIC1_CB;
        IntParam = &IntParamset1;
        reg = (tagI2CReg *)CN_IIC_REGISTER_BADDR1;
        Port = CN_IIC1;
        PDmaBuf = s_IIC1DmaBuf;
        DmaBufLen = IIC1_DMA_BUF_LEN;
    }
    else
    {
        return 0;
    }

    twi_sr = reg->TWI_SR;
    twi_imr = reg->TWI_IMR;

    if(s_IIC_DmaUsed[Port] == false)		//ʹ�÷�DMA���ͺͽ���
    {
		if((twi_sr & TWI_SR_RXRDY) && (twi_imr & TWI_IMR_RXRDY))//�����ж�
		{
			ch = reg->TWI_RHR;
			if(IIC_PortWrite(ICB,&ch,1))
				IntParam->TransCount ++;
			if(IntParam->TransCount == IntParam->TransTotalLen - 1)
			{
				__IIC_GenerateStop(reg);   //������һ��ʱдstop
			}
			else if(IntParam->TransCount == IntParam->TransTotalLen)
			{
				__IIC_GenerateStop(reg); 
				__IIC_IntDisable(reg,TWI_IDR_RXRDY);
				__IIC_IntEnable(reg,TWI_IER_TXCOMP);
			}
		}
		else if((twi_sr & TWI_SR_TXRDY) && (twi_imr & TWI_IMR_TXRDY))//����
		{
			if(IIC_PortRead(ICB,&ch,1))
			{
				reg->TWI_THR = ch;
				IntParam->TransCount ++;
			}
			else if(IntParam->TransCount == IntParam->TransTotalLen)
			{
				__IIC_IntDisable(reg,TWI_IDR_TXRDY);
				__IIC_IntEnable(reg,TWI_IER_TXCOMP);
				__IIC_GenerateStop(reg);
			}
		}
		else if((twi_sr & TWI_SR_TXCOMP) && (twi_imr & TWI_IMR_TXCOMP))
		{
			__IIC_IntDisable(reg,TWI_IDR_TXCOMP);
			Lock_SempPost(IntParam->pDrvPostSemp);
		}
    }
    else			//ʹ��DMA��ʽ���ͺͽ�������
    {
    	if((twi_sr & TWI_SR_ENDRX) && (twi_imr & TWI_IMR_ENDRX))//�����ж�
    	{
    		reg->TWI_PTCR = TWI_PTCR_RXTDIS;
    		//���㱾�λ��������ж�������
    		if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
    		{
    			temp = DmaBufLen;
    			IIC_PortWrite(ICB,PDmaBuf,temp);
    			IntParam->TransCount += temp;
    			//�����´�DMA���յ����ݴ�С
    			if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
    				temp = DmaBufLen;
    			else
    				temp = IntParam->TransTotalLen - IntParam->TransCount;
    			//�����´�DMA����
    			reg->TWI_RPR = (u32)PDmaBuf;
    			reg->TWI_RCR = temp;
    			reg->TWI_PTCR = TWI_PTCR_RXTEN;
    		}
    		else					//�������յ���ȫ��������
    		{
    			temp = IntParam->TransTotalLen - IntParam->TransCount;
    			IIC_PortWrite(ICB,PDmaBuf,temp);
    			__IIC_GenerateStop(reg);

				__IIC_IntDisable(reg,TWI_IDR_ENDRX);
				__IIC_IntEnable(reg,TWI_IER_TXCOMP);
    		}
    	}
    	else if((twi_sr & TWI_SR_ENDTX) && (twi_imr & TWI_IMR_ENDTX))//����
    	{
    		reg->TWI_PTCR = TWI_PTCR_TXTDIS;
    		//���㷢�������жϣ�DMA���͵��ֽ���
    		if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
    		{
    			IntParam->TransCount += DmaBufLen;
    			//���㱾���跢�Ͷ�������
				if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
					temp = DmaBufLen;
				else
					temp = IntParam->TransTotalLen - IntParam->TransCount;
				IIC_PortRead(ICB,PDmaBuf,temp);
				//����DMA����
				reg->TWI_TPR = (u32)PDmaBuf;
				reg->TWI_TCR = temp;
    			reg->TWI_PTCR = TWI_PTCR_TXTEN;
    		}
    		else					//����DMA�Ѿ��������
    		{
    			IntParam->TransCount = IntParam->TransTotalLen;
				__IIC_IntDisable(reg,TWI_IDR_ENDTX);
				__IIC_IntEnable(reg,TWI_IER_TXCOMP);
				__IIC_GenerateStop(reg);
    		}

    	}
		else if((twi_sr & TWI_SR_TXCOMP) && (twi_imr & TWI_SR_TXCOMP))
		{
			__IIC_IntDisable(reg,TWI_IDR_TXCOMP);
			Lock_SempPost(IntParam->pDrvPostSemp);
		}
    }

    return 0;
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������para,�޾�������
// ���أ���
// =============================================================================
bool_t IIC0_Init(void)
{
	struct tagIIC_Param IIC0_Config;

	IIC0_Config.BusName 		   = "IIC0";
	IIC0_Config.IICBuf 		       = (u8*)&s_IIC0Buf;
	IIC0_Config.IICBufLen   	   = IIC0_BUF_LEN;
	IIC0_Config.SpecificFlag 	   = CN_IIC_REGISTER_BADDR0;
	IIC0_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
	IIC0_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
	IIC0_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
	IIC0_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;

	__IIC_HardConfig(CN_IIC_REGISTER_BADDR0,0);
	__IIC_IntConfig(CN_INT_LINE_TWI0,__IIC_ISR);

    PMC_EnablePeripheral(CN_INT_LINE_TWI0);

	if(NULL == IIC_BusAdd_r(&IIC0_Config,&s_tIIC0_CB))
		return 0;
	return 1;
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������para,�޾�������
// ���أ���
// =============================================================================
bool_t IIC1_Init(void)
{
	struct tagIIC_Param IIC1_Config;

	IIC1_Config.BusName 		   = "IIC1";
	IIC1_Config.IICBuf 		       = (u8*)&s_IIC1Buf;
	IIC1_Config.IICBufLen   	   = IIC1_BUF_LEN;
	IIC1_Config.SpecificFlag 	   = CN_IIC_REGISTER_BADDR1;
	IIC1_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
	IIC1_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
	IIC1_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
	IIC1_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;

	__IIC_HardConfig(CN_IIC_REGISTER_BADDR1,0);
	__IIC_IntConfig(CN_INT_LINE_TWI1,__IIC_ISR);

    PMC_EnablePeripheral(CN_INT_LINE_TWI1);

	if(NULL == IIC_BusAdd_r(&IIC1_Config,&s_tIIC1_CB))
		return 0;
	return 1;
}

