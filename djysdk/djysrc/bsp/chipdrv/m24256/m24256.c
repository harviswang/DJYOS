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
// �ļ���     ��M24256.c
// ģ������: E2PROM����������ʹ����DJYBUS ��IICģ��
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 07/10.2014
// =============================================================================

#include "config-prj.h"
#if (CN_CFG_DJYBUS_IIC_BUS == 1)
#include "cpu_peri.h"
#include "iicbus.h"

#define E2ROM_Address                 	(0x50)
#define CN_E2ROM_MAX_SIZE              	(0x40000)	//256K BYTES
#define CN_E2ROM_PAGE_SIZE             	(64)		// BYTE
#define CN_E2ROM_MAX_PAGE               (CN_E2ROM_MAX_SIZE/CN_E2ROM_PAGE_SIZE)

#define CN_CHIP_WRITE_FINISHED_TIME            (6*mS)
static struct tagIIC_Device pg_E2ROM_Dev;
#define CN_E2ROM_OP_TIMEOUT             CN_TIMEOUT_FOREVER//2000*mS


// =============================================================================
// ���ܣ�������
// ������Addr,�����ݵĵ�ַ
//       pDstBuf,�����ݵĴ洢��ַ
//       DstLen,��ַ����
// ���أ���ȡ�����������ֽ�
// =============================================================================
u32 E2PROM_ReadData(u32 Addr,u8 *pDstBuf,u32 DstLen)
{
	u32 result = 0;
	if((Addr + DstLen > CN_E2ROM_MAX_SIZE) || (DstLen == 0))
		return result;
	
	result = IIC_Read(&pg_E2ROM_Dev,Addr,pDstBuf,DstLen,CN_E2ROM_OP_TIMEOUT);
	
	return result;
}

// =============================================================================
// ���ܣ���ָ����ַд��һ���ֽڵ�����
// ������Addr,д���ַ
//       Value,д�������
// ���أ�д���������
// =============================================================================
u32 E2PROM_WriteByte(u32 Addr,u8 Value)
{
	u32 result = 0;
	if(Addr > CN_E2ROM_MAX_SIZE)
		return result;
	result = IIC_Write(&pg_E2ROM_Dev,Addr,&Value,1,true,CN_E2ROM_OP_TIMEOUT);
	
	return result;
}

// =============================================================================
// ���ܣ���ҳд������ֽڲ��ܳ���һҳ��С������,WC��оƬ��д�������ţ�ÿ��д֮ǰ������
//       ��ƽ��д�������ߵ�ƽ�����Ǳ����WCһֱ�ǵ͵�ƽ����������д���оƬ�軨һ��
//       ʱ��ȥ�ж��Ƿ�д��
// ������PageNo,ҳ��
//       pSrcBuf,д�������
//       SrcLen,д���ֽ���������С��һҳ������
// ���أ�д����ֽ���
// =============================================================================
u32 E2PROM_WritePage(u32 PageNo,u8 *pSrcBuf,u32 SrcLen)
{
	u32 Addr,result=0;
	
	if((PageNo > CN_E2ROM_MAX_PAGE) || (SrcLen == 0)
			|| (SrcLen > CN_E2ROM_PAGE_SIZE) || (NULL == pSrcBuf))
		return result;
	
	Addr = PageNo * CN_E2ROM_PAGE_SIZE;
	result = IIC_Write(&pg_E2ROM_Dev,Addr,pSrcBuf,SrcLen,1,CN_E2ROM_OP_TIMEOUT);
	
	Djy_EventDelay(CN_CHIP_WRITE_FINISHED_TIME);
    return result;
}

// =============================================================================
// ���ܣ���ʼ��EEPROMоƬ��������ҽӵ�IIC�������棬ʹ����DJYBUS��ģ��
// ������para����ʱû�õ�
// ���أ�true,��ȷ;false,����
// =============================================================================
ptu32_t M24256_ModuleInit(ptu32_t para)
{
	pg_E2ROM_Dev.DevAddr = E2ROM_Address;
	pg_E2ROM_Dev.BitOfMemAddrInDevAddr = 0;
	pg_E2ROM_Dev.BitOfMemAddr = 16;
	
	IIC0_Init();

	if(NULL == IIC_DevAdd_r("IIC0","IICDev_M24256",&pg_E2ROM_Dev))
		return false;
	else
		return true;
}

#endif
