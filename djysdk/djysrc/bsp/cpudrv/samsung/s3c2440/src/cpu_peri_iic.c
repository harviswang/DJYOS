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
#include "cpu_peri_iic.h"
#include "SysTime.h"
#include <string.h>
#include "endian.h"
#include "iicbus.h"
#include "int.h"
#include "djyos.h"

#define CN_IIC_TIMEOUT  (1000 * mS)
//==============================================================================
#define CN_IIC_REGISTER_BADDR0        0x54000000

//�ж���ʹ�õ��������ͽṹ��
struct tagIIC_IntParamSet
{
	struct tagSemaphoreLCB *pDrvPostSemp;	//�ź���
	u32 TransCount;						//����������������
	u32 TransTotalLen;
};

//���徲̬����
static struct tagIIC_CB s_tIIC0_CB;
#define IIC0_BUF_LEN  		128
static u8 s_IIC0Buf[IIC0_BUF_LEN];
static struct tagIIC_IntParamSet IntParamset0;

// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
//       IntSrc,�ж�Դ
// ����: ��
// =============================================================================
static void __IIC_IntDisable(volatile tagI2CReg *reg)
{
	reg->IICCON &= ~IICCON_INT_MASK;
}

// =============================================================================
// ����: ��ֹiic�ж�
// ����: reg,�������ļĴ�����ָ��
//       IntSrc,�������ж�Դ
// ����: ��
// =============================================================================
static void __IIC_IntEnable(volatile tagI2CReg *reg)
{
    reg->IICCON |= IICCON_INT_MASK;
}

static void __IIC_IntPendingBitClear(volatile tagI2CReg *reg)
{
	reg->IICCON &= ~(IICCON_INTPENDINGFLAG_MASK);
}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_GenerateStop(volatile tagI2CReg *reg)
{
	u32 timeout = CN_IIC_TIMEOUT;
	if((reg->IICSTAT & IICSTAT_MS(3)) == IICSTAT_MS_MT_MODE)
		reg->IICSTAT = 0xD0;
	else if((reg->IICSTAT & IICSTAT_MS(3)) == IICSTAT_MS_MR_MODE)
		reg->IICSTAT = 0x90;

	while((reg->IICSTAT & IICSTAT_BSY_MASK) && (timeout > 0))
	{
		Djy_EventDelay(100);
		timeout -= 100;
	}
}

// =============================================================================
// ���ܣ�IICʱ�����ú�����ʱ����ԴΪMCK�����㷽���ɲο������ֲ�
// ������reg���豸���
//      iicclk�������õ�ʱ���ٶȣ���׼��ʱ����50KHz,100KHz,200KHz
// ���أ�true=�ɹ���false=ʧ��
// =============================================================================
static void __IIC_SetClk(volatile tagI2CReg *reg,u32 iicclk)
{
	u32 BaseClk = CN_CFG_PCLK/256,temp;

	if(iicclk < BaseClk)	//����100K
	{
		reg->IICCON |= IICCON_IICLK_MASK;		//IICCLK = CN_CFG_PCLK/16
		temp = (BaseClk/iicclk) - 1;
		if(temp < 16)
		{
			reg->IICCON &= ~ (0xF << IICCON_TXCLK_OFFSET);
			reg->IICCON |= IICCON_TXCLK(temp);
		}
	}
}


// =============================================================================
// ���ܣ�IIC���ų�ʼ������ʼ��Ϊ����ʹ��
// ������iic_no,IIC��������
// ���أ���
// =============================================================================
static void __IIC_GpioInit(void)
{
	u32 temp;
	temp = pg_gpio_reg->GPECON;
	temp &= ~(0x0F << 28);
	temp |= (0x0A << 28);		//����IO��ΪSDA��SCL
	pg_gpio_reg->GPECON = temp;
	//pg_gpio_reg->GPHUP
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

    __IIC_GpioInit();

    __IIC_SetClk(reg,CN_IIC_CLK_100K);

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
// ���ܣ���ѯ�Ƿ���յ�ACK�źţ�2440�ǲ�ѯ�жϱ�־��
// �������Ĵ�����ַָ��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
static bool_t __IIC_AckReceived(volatile tagI2CReg *Reg)
{
	u32 timeout = CN_IIC_TIMEOUT;
//    while(Reg->IICSTAT & IICSTAT_ACK_MASK);						//�ж�ACK
//    while(!(Reg->IICCON & IICCON_INTPENDINGFLAG_MASK));

    while((!(Reg->IICCON & IICCON_INTPENDINGFLAG_MASK)) && (timeout > 0))
    {
    	Djy_DelayUs(1);
		timeout -=1;
    }
    if(timeout == 0)
    {
    	return false;
    }
    return true;
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
    struct tagIIC_IntParamSet *IntParam;
    u8 i;u32 timeout = CN_IIC_TIMEOUT;

    if((u32)Reg != CN_IIC_REGISTER_BADDR0)
    	return false;

	IntParam = &IntParamset0;
	IntParam->TransTotalLen = length;
	IntParam->TransCount = 0;
	IntParam->pDrvPostSemp = iic_semp;

    //���ж�
    Int_CutLine(cn_int_line_iic);
    __IIC_IntEnable(Reg);

    while((Reg->IICSTAT & IICSTAT_BSY_MASK) &&(timeout > 0))	//check if busy
    {
		Djy_EventDelay(100);
		timeout -= 100;
    }
    if(timeout == 0)
    {
    	return false;
    }
    Reg->IICCON |= IICCON_ACKEN_MASK;
    Reg->IICSTAT = IICSTAT_MS_MT_MODE | IICSTAT_TXRXEN_MASK; //����Ϊ������ģʽ
    Reg->IICDS = (dev_addr<<CN_IIC_ADDR_OFFSET) | CN_IIC_WRITE_BIT;	//���豸��ַ
    __IIC_IntPendingBitClear(Reg);

    Reg->IICSTAT |= IICSTAT_START_STOP_MASK;					//��������
    if(! __IIC_AckReceived(Reg))
    {
    	__IIC_GenerateStop(Reg);
    	return false;
    }
    
	for(i = 0; i < maddr_len; i ++)
	{
		Reg->IICDS = (u8) (mem_addr >> ((maddr_len - i - 1) *8));
		__IIC_IntPendingBitClear(Reg);
	    if(! __IIC_AckReceived(Reg))
	    {
	    	__IIC_GenerateStop(Reg);
	    	return false;
	    }
	}
	Int_ContactLine(cn_int_line_iic);
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
    struct tagIIC_IntParamSet *IntParam;
    u32 i,temp,timeout = CN_IIC_TIMEOUT;

    if((u32)Reg != CN_IIC_REGISTER_BADDR0)
    	return false;

	IntParam = &IntParamset0;
	IntParam->TransTotalLen = length;
	IntParam->TransCount = 0;
	IntParam->pDrvPostSemp = iic_semp;

    //˵������оƬIIC���������Ƚ��������뵽CDS���������ͱ㷢������
    Int_CutLine(cn_int_line_iic);
    __IIC_IntEnable(Reg);			//��оƬ���뿪�жϣ������ж��Ƿ��ͽ�����

    while((Reg->IICSTAT & IICSTAT_BSY_MASK) &&(timeout > 0))	//check if busy
    {
		Djy_EventDelay(100);
		timeout -= 100;
    }
    if(timeout == 0)
    {
    	return false;
    }
    Reg->IICCON |= IICCON_ACKEN_MASK;	//��оƬ���뿪ACKENλ������������
    Reg->IICSTAT = IICSTAT_MS_MT_MODE | IICSTAT_TXRXEN_MASK; 		//������ģʽ
    Reg->IICDS = (dev_addr<<CN_IIC_ADDR_OFFSET) | CN_IIC_WRITE_BIT;	//���豸��ַ

    Reg->IICSTAT |= IICSTAT_START_STOP_MASK;					//��������
    if(! __IIC_AckReceived(Reg))
    {
    	__IIC_GenerateStop(Reg);
    	return false;
    }

    __IIC_IntPendingBitClear(Reg);
	for(i = 0; i < maddr_len; i ++)
	{
		Reg->IICDS = (u8) (mem_addr >> ((maddr_len - i - 1) *8));
//		__IIC_IntPendingBitClear(Reg);
	    if(! __IIC_AckReceived(Reg))
	    {
	    	__IIC_GenerateStop(Reg);
	    	return false;
	    }
	}
    //����Restart
	Reg->IICDS = (dev_addr<<CN_IIC_ADDR_OFFSET) | CN_IIC_READ_BIT;	//���豸��ַ
    Reg->IICSTAT = IICSTAT_MS_MR_MODE | IICSTAT_TXRXEN_MASK
    					| IICSTAT_START_STOP_MASK; 				//����Ϊ������ģʽ
    __IIC_IntPendingBitClear(Reg);
    if(! __IIC_AckReceived(Reg))
    {
    	__IIC_GenerateStop(Reg);
    	return false;
    }

//    temp = Reg->IICSTAT;				//�˴�����ʡ���൱����״̬
    temp = Reg->IICDS;
    temp = Reg->IICCON;
    temp = Reg->IICSTAT;
    __IIC_IntPendingBitClear(Reg);
    Int_ContactLine(cn_int_line_iic);

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
	if((u32)Reg != CN_IIC_REGISTER_BADDR0)
		return;

	__IIC_IntDisable(Reg);
    Djy_EventDelay(100);
    __IIC_GenerateStop(Reg);
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
	if((u32)Reg != CN_IIC_REGISTER_BADDR0)
		return 0;

	switch(cmd)
	{
	case CN_IIC_SET_CLK:
		__IIC_SetClk(Reg,data1);
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

    u8 ch;
    if(IntLine != cn_int_line_iic)
    	return 0;
	ICB = &s_tIIC0_CB;
	IntParam = &IntParamset0;
	reg = (tagI2CReg *)CN_IIC_REGISTER_BADDR0;

    if(reg->IICCON & IICCON_INTPENDINGFLAG_MASK)		//�Ƿ����жϱ�־
    {
    	if((reg->IICSTAT & IICSTAT_MS_MT_MODE) ==  IICSTAT_MS_MT_MODE)//����
    	{
    		if(!(reg->IICSTAT & IICSTAT_ACK_MASK))		//�Ƿ��յ�ACK�ź�
    		{
    			if(IIC_PortRead(ICB,&ch,1))
    			{
    				reg->IICDS = ch;
    				IntParam->TransCount ++;
    			}
    			else if(IntParam->TransCount == IntParam->TransTotalLen)
    			{
    				__IIC_IntDisable(reg);
    				__IIC_IntPendingBitClear(reg);
    				__IIC_GenerateStop(reg);
    				Lock_SempPost(IntParam->pDrvPostSemp);
    			}
    		}
    	}
    	else if((reg->IICSTAT & IICSTAT_MS_MR_MODE) ==  IICSTAT_MS_MR_MODE)
    	{
			ch = reg->IICDS;
			if(IIC_PortWrite(ICB,&ch,1))
				IntParam->TransCount ++;
			if(IntParam->TransCount == IntParam->TransTotalLen - 1)
			{
				reg->IICCON &= ~(IICCON_ACKEN_MASK);	//�����ڶ�����������ACK
			}
			else if(IntParam->TransCount == IntParam->TransTotalLen)
			{
				__IIC_IntPendingBitClear(reg);
				__IIC_IntDisable(reg);
				__IIC_GenerateStop(reg);
				Lock_SempPost(IntParam->pDrvPostSemp);
			}
    	}
    }
    else
    {
    	//....
    }
    __IIC_IntPendingBitClear(reg);
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
bool_t IIC_Init(void)
{
	struct tagIIC_Param IIC0_Config;

	IIC0_Config.BusName 		   = "IIC0";
	IIC0_Config.IICBuf 		       = (u8*)&s_IIC0Buf;
	IIC0_Config.IICBufLen   	   = IIC0_BUF_LEN;
	IIC0_Config.SpecificFlag 	   = CN_IIC_REGISTER_BADDR0;
	IIC0_Config.pWriteReadPoll      = NULL;
	IIC0_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
	IIC0_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
	IIC0_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
	IIC0_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;

	__IIC_HardConfig(CN_IIC_REGISTER_BADDR0,0);
	__IIC_IntConfig(cn_int_line_iic,__IIC_ISR);

	if(NULL == IIC_BusAdd_r(&IIC0_Config,&s_tIIC0_CB))
		return 0;
	return 1;
}

