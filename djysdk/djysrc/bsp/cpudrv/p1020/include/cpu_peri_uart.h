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
//Copyright (C), 2004-2009,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�����Ŷ�
//����ģ��: uart��������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------
#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#include "cpu_peri.h"


#ifdef __cplusplus
extern "C" {
#endif

//base address
//#define cn_ccsrbar_base 0x0ff700000


typedef struct         //offset
{
    vu8 URBR_THR_DLB;     //0x00    ULCR[DLAB]=0,RBRֻ����THRֻд��
                          //        ULCR[DLAB]=1,DLB�ɶ�д
    vu8 UDMB_IER;         //0x01    ULCR[DLAB]=0,IER�ɶ�д��
                          //        ULCR[DLAB]=1,DLB�ɶ�д
    vu8 UIIR_FCR_AFR;     //0x02    ULCR[DLAB]=0,IIRֻ��, FCRֻд��
                          //        ULCR[DLAB]=1,AFR�ɶ�д
    vu8 ULCR;             //0x03
    vu8 UMCR;             //0x04
    vu8 ULSR;               //0x05
    vu8 UMSR;               //0x06
    vu8 USCR;               //0x07
    vu8 RESERVED0;
    vu8 RESERVED1;
    vu8 UDSR;             //0x10
}tagUartReg;

#define CN_UART0   0
#define CN_UART1   1
#define CN_UART_NUM 2

ptu32_t ModuleInstall_UART(ptu32_t serial_no);//���ڳ�ʼ��
u32 Uart_SendServiceDirectly(char *str);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


