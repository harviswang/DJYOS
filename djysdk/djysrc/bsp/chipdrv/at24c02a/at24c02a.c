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
// �ļ���     ��at24c02a.c
// ģ������: EEPROMоƬ������������IICBUS��������ģ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 14/01.2015
// =============================================================================

#include "stdint.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"

// =============================================================================
#define CN_AT24_CHIP_SIZE       (256)			//оƬ��С256 Bytes
#define CN_AT24_PAGE_SIZE       (8)				//оƬҳ��С8 Bytes
#define CN_AT24_PAGE_SUM    	(CN_AT24_CHIP_SIZE/CN_AT24_PAGE_SIZE)

#define AT24C_ADDRESS			0x50			//�豸��ַ
#define AT24C_CLK_FRE			(100*1000)		//�����ٶȣ���λHz


//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct tagIIC_Device *ps_AT24_Dev = NULL;
static u32 s_AT24_Timeout = CN_TIMEOUT_FOREVER;

// =============================================================================
// ���ܣ�AT24оƬWPд�������ų�ʼ��������Ϊ��ʱ������д��Ϊ��ʱ��д����
// ��������
// ���أ���
// =============================================================================
void __AT24_GpioInit(void)
{
	//AT24C��WP��������Ϊ�ͣ�����д
}

// =============================================================================
// ���ܣ�AT24оƬ��ҳд,��ַ��Χֻ�����ڸ�ҳ��
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u16 __AT24_PageProgram(u16 wAddr, u8 *pbyBuf, u16 wBytesNum)
{
	if(((wAddr%CN_AT24_PAGE_SIZE) + wBytesNum) > CN_AT24_PAGE_SIZE)
		return 0;

	IIC_Write(ps_AT24_Dev,wAddr,pbyBuf,wBytesNum,true,s_AT24_Timeout);

	//AT24�ڲ�дʱ�䣬���Ϊ10mS
	Djy_EventDelay(10*mS);

	return wBytesNum;
}

// =============================================================================
// ���ܣ�AT24оƬ��ҳ��,��ַ��Χֻ�����ڸ�ҳ��
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u32 __AT24_PageRead(u16 wAddr, u8 *pbyBuf, u16 wBytesNum)
{
	if(((wAddr%CN_AT24_PAGE_SIZE) + wBytesNum) > CN_AT24_PAGE_SIZE)
		return 0;

	IIC_Read(ps_AT24_Dev,wAddr,pbyBuf,wBytesNum,s_AT24_Timeout);

	//AT24�ڲ�дʱ�䣬���Ϊ10mS
	Djy_EventDelay(10*mS);

	return wBytesNum;
}

// =============================================================================
// ���ܣ�AT24оƬд������EEPROM��ҳ������ÿ�ζ�д���ܳ���ҳ��С
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u32 AT24_WriteBytes(u16 wAddr, u8 *pbyBuf, u16 wBytesNum)
{
	u32 offset = 0,wSize = 0;

	//�ж��Ƿ񳬳�оƬ��ַ��Χ
	if((wAddr + wBytesNum) >= CN_AT24_CHIP_SIZE)
		return false;

	//��ҳ����
	while(wBytesNum)
	{
		offset = wAddr % CN_AT24_PAGE_SIZE;				//��ҳƫ���ֽ���
		wSize = (wBytesNum > CN_AT24_PAGE_SIZE - offset)?
				(CN_AT24_PAGE_SIZE - offset): wBytesNum;//��ҳд�ֽ���
		__AT24_PageProgram(wAddr,pbyBuf,wSize);			//д��ҳ
		wAddr += wSize;									//д��ַ����
		pbyBuf += wSize;								//����������
		wBytesNum -= wSize;								//д���ȵݼ�
	}

	return true;
}

// =============================================================================
// ���ܣ�AT24оƬ��������EEPROM��ҳ������ÿ�ζ�д���ܳ���ҳ��С
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u16 AT24_ReadBytes(u16 wAddr, u8 *pbyBuf, u16 wBytesNum)
{
	u16 offset = 0,rSize = 0;

	//�ж��Ƿ񳬳�оƬ��ַ��Χ
	if((wAddr + wBytesNum) >= CN_AT24_CHIP_SIZE)
		return false;

	//��ҳ����
	while(wBytesNum)
	{
		offset = wAddr % CN_AT24_PAGE_SIZE;				//��ҳƫ���ֽ���
		rSize = (wBytesNum > CN_AT24_PAGE_SIZE - offset)?
				(CN_AT24_PAGE_SIZE - offset): wBytesNum;//��ҳ���ֽ���
		__AT24_PageRead(wAddr,pbyBuf,rSize);			//����ҳ
		wAddr += rSize;									//����ַ����
		pbyBuf += rSize;								//����������
		wBytesNum -= rSize;								//�����ȵݼ�
	}

	return true;
}

// =============================================================================
// ���ܣ�д�ֵ�ָ����ַ
// ������wAddr,��ַ
// 		 wValue,��ֵ��16������ֵ
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u16 AT24_WriteWord(u16 wAddr,u16 wValue)
{
	u8 pbyBuf[2];
	pbyBuf[0]=wValue;
	pbyBuf[1]=wValue>>8;
	if(true == AT24_WriteBytes(wAddr,pbyBuf,2))
		return true;
	else
		return false;
}

// =============================================================================
// ���ܣ�����
// ������wAddr,��ַ
// ���أ�wValue,�ɹ�;-1,ʧ��
// =============================================================================
s16 AT24_ReadWord(u16 wAddr)
{
	u8 pbyBuf[2];
	u16 wValue;

	if(true == AT24_ReadBytes(wAddr, pbyBuf,2))
	{
		wValue=pbyBuf[0]+(pbyBuf[1]<<8);
		return wValue;
	}
	else
		return -1;
}

// =============================================================================
// ���ܣ�AT24оƬ��ʼ��������WPд�����������ų�ʼ���ͼ����豸����Ӧ��IIC����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AT24_ModuleInit(void)
{
	bool_t result = false;
	static struct tagIIC_Device s_AT24_Dev;
	//GPIO��ʼ����SDA��SCL�Ѿ���IIC�г�ʼ���ˣ��˴�ֻ���ʼ��WP����
	__AT24_GpioInit();

	//��ʼ��IIC�豸�ṹ��
	s_AT24_Dev.DevAddr 					= AT24C_ADDRESS;
	s_AT24_Dev.BitOfMemAddr 			= 8;
	s_AT24_Dev.BitOfMemAddrInDevAddr 	= 0;

	//���AT24��IIC0����
	if(NULL != IIC_DevAdd_r("IIC0","IIC_Dev_AT24",&s_AT24_Dev))
	{
		ps_AT24_Dev = &s_AT24_Dev;
		IIC_BusCtrl(ps_AT24_Dev,CN_IIC_SET_CLK,AT24C_CLK_FRE,0);
		IIC_BusCtrl(ps_AT24_Dev,CN_IIC_DMA_USED,0,0);
		result = true;
	}

	return result;
}


