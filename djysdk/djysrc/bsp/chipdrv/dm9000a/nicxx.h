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

#ifndef DM9000A_H_
#define DM9000A_H_

#define DM9000A_INDEX	0x6C000000
#define DM9000A_DATA	0x6C000002

#define DM9000A_VID_PID	0x0A469000	// Vendor ID and Produuct ID
// DM9000�ļĴ���
#define DM9000A_NCR		0x00
#define DM9000A_NSR		0x01
// TX/RX Control & Status
#define DM9000A_TCR		0x02
#define DM9000A_TSR1	0x03
#define DM9000A_TSR2	0x04
#define DM9000A_RCR		0x05
#define DM9000A_RSR		0x06
#define DM9000A_ROCR	0x07
#define DM9000A_BPTR	0x08
#define DM9000A_FCTR	0x09
#define DM9000A_FCR		0x0A
// EEPROM & PHY
#define DM9000A_EPCR	0x0B
#define DM9000A_EPAR	0x0C
#define DM9000A_EPDRL	0x0D
#define DM9000A_EPDRH	0x0E

#define DM9000A_WCR		0x0F
// Physical Address Control
#define DM9000A_PAR0	0x10
#define DM9000A_PAR1	0x11
#define DM9000A_PAR2	0x12
#define DM9000A_PAR3	0x13
#define DM9000A_PAR4	0x14
#define DM9000A_PAR5	0x15
// Multicast Address Control
#define DM9000A_MAR0	0x16
#define DM9000A_MAR1	0x17
#define DM9000A_MAR2	0x18
#define DM9000A_MAR3	0x19
#define DM9000A_MAR4	0x1A
#define DM9000A_MAR5	0x1B
#define DM9000A_MAR6	0x1C
#define DM9000A_MAR7	0x1D
// General Purpose Control
#define DM9000A_GPCR	0x1E
#define DM9000A_GPR		0x1F
// TX/RX SRAM Read/Write Pointer Address
#define DM9000A_TRPAL	0x22
#define DM9000A_TRPAH	0x23
#define DM9000A_RWPAL	0x24
#define DM9000A_RWPAH	0x25
// Chip ID
#define DM9000A_VIDL	0x28
#define DM9000A_VIDH	0x29
#define DM9000A_PIDL	0x2A
#define DM9000A_PIDH	0x2B
#define DM9000A_CHIPR	0x2C
#define DM9000A_TCR2	0x2D
#define DM9000A_OCR		0x2E
#define DM9000A_SMCR	0x2F
#define DM9000A_ETXCSR	0x30
#define DM9000A_TCSCR	0x31
#define DM9000A_RCSCCR	0x32
#define DM9000A_MPAR	0x33
#define DM9000A_LEDCR	0x34
#define DM9000A_BUSR	0x38
#define DM9000A_INTCR	0x39

#define DM9000A_SCCR	0x50
#define DM9000A_RSCCR	0x51
// Memory Data Read
#define DM9000A_MRCMDX	0xF0
#define DM9000A_MRCMDX1	0xF1
#define DM9000A_MRCMD	0xF2
#define DM9000A_MDRRL	0xF4
#define DM9000A_MDRRH	0xF5
// Memory Data Write
#define DM9000A_MWCMDX	0xF6
#define DM9000A_MWCMD	0xF8
#define DM9000A_MDWRL	0xFA
#define DM9000A_MDWRH	0xFB
#define DM9000A_TXPLL	0xFC
#define DM9000A_TXPLH	0xFD
// Interrupt
#define DM9000A_ISR		0xFE
#define DM9000A_IMR		0xFF

enum DM9000A_IO_mode { ENUM_DM9000A_IO_16BIT = 0, ENUM_DM9000A_IO_32BIT = 1,
	ENUM_DM9000A_IO_8BIT = 2};

#define DM9000A_PKT_RDY		0x01	/* Packet ready to receive */
#define DM9000A_PKT_ERR		0x02
#define DM9000A_PKT_MAX		1536	/* Received packet max size */

extern u8 enet_state_nic_init(struct _enet_state *pt_enet_st);
extern struct enet_rcv_packet* dm9000a_dump_data(u32 len);
extern void dm9000a_reset_to_new(void);


#endif /* DM9000A_H_ */
