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
//����ģ��: linkport��������(������������)
//����:  �캣��.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2014-4-25
//   ����:  �캣��.
//   �°汾�ţ�1.0.0
//   �޸�˵��: 
//------------------------------------------------------
#ifndef __CPU_PERI_LINKPORT_H__
#define __CPU_PERI_LINKPORT_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct         //offset
{
    vu32 rLCTL;       		//0x00
    vu32 rLSTAT;      		//0x01
    vu32 RESERVED0;
    vu32 rLSTAT_SHADOW;		//0x03
    vu32 RESERVED1[4];
    vu32 rTXLB;       		//0x08
    vu32 rRXLB;       		//0x09
    vu32 rRXLB_OUT_SHADOW;	//0x0A
    vu32 rRXLB_IN_SHADOW;	//0x0B
    vu32 rTXLB_OUT_SHADOW;	//0x0C
    vu32 rTXLB_IN_SHADOW;	//0x0D
}tag_LinkPortReg;


typedef enum
{
    LinkPort1,
    LinkPort2
}enum_linkportx;


typedef enum
{
    SendMode,
    RecvMode
}enum_TransMode;



typedef struct         //offset
{
    u32 rIILB;				//0x00
    u32 rIMLB;				//0x01
    u32 rCLB;				//0x02
    u32 rCPLB;				//0x03
}tag_DMA_LinkPortReg;


struct t_LinkPortChainedDMA_TCB
{
	int CPLB;
	int CLB;
	int IMLB;
	int IILB;
};


bool_t  LinkPort_Init(u32 LinkPortNum,u8 TransMode,u8 DMAMode);
bool_t  LinkPort_Enable(u32 LinkPortNum);
bool_t  LinkPort_Disable(u32 LinkPortNum) ;
bool_t  LinkPort_EnableDMA(u32 LinkPortNum);
bool_t LinkPort_DisableDMA(u32 LinkPortNum);
bool_t LinkPort_InitDMA(u32 LinkPortNum,u32* pData,u32 DataLen);
u32  LinkPort_SendData(u32 LinkPortNum,u32 Val);
u32 LinkPort_RecvData(u32 LinkPortNum);
u32 LinkPort0_RecvISR(ufast_t linkport0_int_line);
u32 LinkPort0_SendISR(ufast_t linkport0_int_line);
u32  LinkPort1_RecvISR(ufast_t linkport1_int_line);
u32 LinkPort1_SendISR(ufast_t linkport1_int_line);
bool_t LinkPort_InterruptInit(u32 LinkPortNum,u32 TransMode);
void LinkPort_InitChainedDMA(int InitTCB_Addr);
bool_t  LinkPort_SetLinkPortChainedTCB(int Chained_Num);
bool_t LinkPort_SetRecvDataPoolLen(u8 Data_Pool_Num, u32 Len);
bool_t LinkPort_SetRecvDataPoolAddr(u8 Data_Pool_Num,u32 addr);
void  LinkPort_DefaultIntInit(void);
void LinkPort_EnableDMA_Send(u32 LinkPortNum);
bool_t SetDMAChainedTCB_Send(u32 addr,u32 Len);
void EnableLinkPortDMA_Recv(u32 LinkPortNum);



#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_LINKPORT_H__

