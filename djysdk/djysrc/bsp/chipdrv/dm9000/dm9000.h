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
// �ļ���     ��dm9000.h
// ģ������: DM9000������������DJYOS����̫��Э��ʵ��Ҫ��ʵ�ֵ������ײ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 18/09.2014
// =============================================================================

#ifndef DM9000_H_
#define DM9000_H_


#define DM9000_ID		    0x90000A46
#define DM9000_PKT_MAX		1536	/* Received packet max size */
#define DM9000_PKT_RDY		0x01	/* Packet ready to receive */

//����DM9000�Ĵ���
#define DM9000_NCR             0x00
#define DM9000_NSR             0x01
#define DM9000_TCR             0x02
#define DM9000_TSR1            0x03
#define DM9000_TSR2            0x04
#define DM9000_RCR             0x05
#define DM9000_RSR             0x06
#define DM9000_ROCR            0x07
#define DM9000_BPTR            0x08
#define DM9000_FCTR            0x09
#define DM9000_FCR             0x0A
#define DM9000_EPCR            0x0B
#define DM9000_EPAR            0x0C
#define DM9000_EPDRL           0x0D
#define DM9000_EPDRH           0x0E
#define DM9000_WCR             0x0F

#define DM9000_PAR             0x10
#define DM9000_MAR             0x16

#define DM9000_GPCR			   0x1e
#define DM9000_GPR             0x1f
#define DM9000_TRPAL           0x22
#define DM9000_TRPAH           0x23
#define DM9000_RWPAL           0x24
#define DM9000_RWPAH           0x25

#define DM9000_VIDL            0x28
#define DM9000_VIDH            0x29
#define DM9000_PIDL            0x2A
#define DM9000_PIDH            0x2B

#define DM9000_CHIPR           0x2C
#define DM9000_SMCR            0x2F

#define DM9000_PHY		       0x40	/* PHY address 0x01 */

#define DM9000_MRCMDX          0xF0
#define DM9000_MRCMD           0xF2
#define DM9000_MRRL            0xF4
#define DM9000_MRRH            0xF5
#define DM9000_MWCMDX		   0xF6
#define DM9000_MWCMD           0xF8
#define DM9000_MWRL            0xFA
#define DM9000_MWRH            0xFB
#define DM9000_TXPLL           0xFC
#define DM9000_TXPLH           0xFD
#define DM9000_ISR             0xFE
#define DM9000_IMR             0xFF

/*PHY registers*/
#define DM9000_BMCR				0x00
#define DM9000_BMSR				0x01
#define DM9000_PHYID1			0x02
#define DM9000_PHYID2			0x03
#define DM9000_ANAR				0x04
#define DM9000_ANLPAR			0x05
#define DM9000_ANER				0x06
#define DM9000_DSCR				0x16
#define DM9000_DSCSR			0x17
#define DM9000_10BTCSR			0x18

//�Ĵ�������

#define NCR_EXT_PHY		(1<<7)
#define NCR_WAKEEN		(1<<6)
#define NCR_FCOL		(1<<4)
#define NCR_FDX			(1<<3)
#define NCR_LBK			(3<<1)
#define NCR_LBK_INT_MAC	(1<<1)
#define NCR_LBK_INT_PHY	(2<<1)
#define NCR_RST			(1<<0)

#define NSR_SPEED		(1<<7)
#define NSR_LINKST		(1<<6)
#define NSR_WAKEST		(1<<5)
#define NSR_TX2END		(1<<3)
#define NSR_TX1END		(1<<2)
#define NSR_RXOV		(1<<1)

#define TCR_TJDIS		(1<<6)
#define TCR_EXCECM		(1<<5)
#define TCR_PAD_DIS2	(1<<4)
#define TCR_CRC_DIS2	(1<<3)
#define TCR_PAD_DIS1	(1<<2)
#define TCR_CRC_DIS1	(1<<1)
#define TCR_TXREQ		(1<<0)

#define TSR_TJTO		(1<<7)
#define TSR_LC			(1<<6)
#define TSR_NC			(1<<5)
#define TSR_LCOL		(1<<4)
#define TSR_COL			(1<<3)
#define TSR_EC			(1<<2)

#define RCR_WTDIS		(1<<6)
#define RCR_DIS_LONG	(1<<5)
#define RCR_DIS_CRC		(1<<4)
#define RCR_ALL			(1<<3)
#define RCR_RUNT		(1<<2)
#define RCR_PRMSC		(1<<1)
#define RCR_RXEN		(1<<0)

#define RSR_RF			(1<<7)
#define RSR_MF			(1<<6)
#define RSR_LCS			(1<<5)
#define RSR_RWTO		(1<<4)
#define RSR_PLE			(1<<3)
#define RSR_AE			(1<<2)
#define RSR_CE			(1<<1)
#define RSR_FOE			(1<<0)

#define EPCR_EPOS_PHY	(1<<3)
#define EPCR_EPOS_EE	(0<<3)
#define EPCR_ERPRR		(1<<2)
#define EPCR_ERPRW		(1<<1)
#define EPCR_ERRE		(1<<0)

#define FCTR_HWOT(ot)	(( ot & 0xf ) << 4 )
#define FCTR_LWOT(ot)	( ot & 0xf )

#define BPTR_BPHW(x)	((x) << 4)
#define BPTR_JPT_200US	(0x07)
#define BPTR_JPT_600US	(0x0f)

#define IMR_PAR			(1<<7)
#define IMR_ROOM		(1<<3)
#define IMR_ROM			(1<<2)
#define IMR_PTM			(1<<1)
#define IMR_PRM			(1<<0)

#define ISR_ROOS		(1<<3)
#define ISR_ROS			(1<<2)
#define ISR_PTS			(1<<1)
#define ISR_PRS			(1<<0)

#define GPCR_GPIO0_OUT	(1<<0)

#define GPR_PHY_PWROFF	(1<<0)

#endif /* DM9000_H_ */
