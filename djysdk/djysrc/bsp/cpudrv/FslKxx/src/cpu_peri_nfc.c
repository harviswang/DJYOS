// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_nfc.c
// ģ������: NFCģ��ĵײ㺯������Ҫ��ʼ��NFCģ�飬������NFC��������ȡ���ݵ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 02/08.2014
// =============================================================================

#include "cpu_peri.h"
#include "string.h"

// ��û�ж�NFC�������ã���ʹ��Ĭ������
#ifndef NFC_CONFIG
#define NFC_CE 			NFC_CE0			// Ƭѡʹ��CE0
#define DATA_WIDTH 		NFC_16BIT		// ���ݿ��16����
#define NFCDIV          0x05			// ʱ�ӷ�Ƶ
#define NFCFRAC         0x00			// ʱ�ӷ�Ƶ
#define PAGE_SIZE 		PAGE_2K			// ҳ��С
#define ECC_SIZE  		PAGE_64			// ECC��С
#endif

#define NFC_BASE_PTR NFC_BASE

// =============================================================================
// ���ܣ�NFCģ��ĳ�ʼ��������ʱ�ӡ�MPU��ʱ�ӷ�Ƶ��IO���úͼĴ����ĳ�ʼ��������ʱ�ӡ�
//       Ƭѡ�����ݿ�Ⱥ�ҳ��С��ECC���ݴ�С��Ӧ���ݾ����NANDоƬ��������Դ������ʹ
//       ��Ĭ�Ϻ궨��ķ�ʽ�������߿ɸ��ݾ���оƬ�޸Ļ��Լ����¶���
// ��������
// ���أ���
// =============================================================================
void NFC_ModuleInit(void)
{
    /* Enable the NFC clock gate in the SIM */
    SIM->SCGC3 |= SIM_SCGC3_NFC_MASK;

    /* Disable the MPU to allow NFC to access memory */
    MPU->CESR &= 0xFFFFFFFE;

    /* Set the NFC clock divder according to defines in nand.h */
    SIM->CLKDIV4 |= SIM_CLKDIV4_NFCDIV(NFCDIV) |SIM_CLKDIV4_NFCFRAC(NFCFRAC);

    /* Enable the chip select in the NFC and enable the pad for the CS */
    if(NFC_CE == NFC_CE0)
    {
        NFC->RAR |= (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
        PORTC->PCR[17] =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;  /* Enable NFC_CE0 pad */
    }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC->RAR |= (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
        PORTC->PCR[18] =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;  /* Enable NFC_CE0 pad */
    }

    /* Enable all of the NFC control signal pads */
    PORTC->PCR[16] =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_RB pad */
    PORTD->PCR[8] =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_CLE pad */
    PORTD->PCR[9] =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_ALE pad */
    PORTD->PCR[10] =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_RE pad */
    PORTC->PCR[11] =  PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_WE pad */

    /* Configure bit width in NFC and enable the NFC_IO pins based on the
     * defined port size in nand.h.*/

    if (DATA_WIDTH == NFC_16BIT)
    {
    	NFC->CFG |= NFC_CFG_BITWIDTH_MASK;

        PORTB->PCR[20] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA15 pad */
    	PORTB->PCR[21] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA14 pad */
    	PORTB->PCR[22] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA13 pad */
    	PORTB->PCR[23] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA12 pad */
    	PORTC->PCR[0] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA11 pad */
    	PORTC->PCR[1] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA10 pad */
    	PORTC->PCR[2] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA9 pad */
    	PORTC->PCR[4] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA8 pad */
    	PORTC->PCR[5] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA7 pad */
    	PORTC->PCR[6] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA6 pad */
    	PORTC->PCR[7] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA5 pad */
    	PORTC->PCR[8] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA4 pad */
    	PORTC->PCR[9] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA3 pad */
    	PORTC->PCR[10] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA2 pad */
    	PORTD->PCR[4] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA1 pad */
    	PORTD->PCR[5] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA0 pad */

        /* Set the sector size in the NFC to match the memory */
        NFC->SECSZ = (PAGE_SIZE|(ECC_SIZE+1));
    }
    else /* (DATA_WIDTH == NFC_8BIT) */
    {
    	NFC->CFG &= ~NFC_CFG_BITWIDTH_MASK;

    	PORTC->PCR[5] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA7 pad */
    	PORTC->PCR[6] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA6 pad */
    	PORTC->PCR[7] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA5 pad */
    	PORTC->PCR[8] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA4 pad */
    	PORTC->PCR[9] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA3 pad */
    	PORTC->PCR[10] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA2 pad */
    	PORTD->PCR[4] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA1 pad */
    	PORTD->PCR[5] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA0 pad */

        /* Set the sector size in the NFC to match the memory */
        NFC->SECSZ = (PAGE_SIZE | ECC_SIZE);
    }

    /* Configure NFC for EDO mode if defined in nand.h */
    #if defined(NFC_EDO)
        NFC->CFG |= NFC_CFG_FAST_MASK;
    #endif

    /* Disable auto address and buff num increment */
    NFC->CFG &= ~(NFC_CFG_AIAD_MASK | NFC_CFG_AIBN_MASK);

    /* Set NFC to STOP if a write error is detected */
    NFC->CFG |= NFC_CFG_STOPWERR_MASK;

    /* Disable row address increment */
    NFC->RAI = 0;

    /* Disable swap */
    NFC->SWAP = 0;
}

// =============================================================================
// ���ܣ��жϱ��β����Ƿ���ɣ����ж�R/B#�����Ƿ��Ѿ����ߣ���Ϊ�ͣ����ʾδ���
// ��������
// ���أ�true,���β����Ѿ����;false,δ���
// =============================================================================
bool_t NFC_OptionCompleted(void)
{
	return !(NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
}

// =============================================================================
// ���ܣ���NFC��Ӧ��Ƭѡ�ź�����Ϊ��Ч�����ر���ĳƬѡ�ϵ�NANDоƬͨ��
// ������CsNo,Ƭѡ�ţ�0��1
// ���أ���
// =============================================================================
void NFC_CsActive(u8 CsNo)
{
    if(CsNo == NFC_CE0)
    {
        NFC->RAR |= (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
    }
    else
    {
        NFC->RAR |= (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }
}

// =============================================================================
// ���ܣ���NFC��Ӧ��Ƭѡ�ź�����Ϊ��Ч�����ر���ĳƬѡ�ϵ�NANDоƬͨ��
// ������CsNo,Ƭѡ�ţ�0��1
// ���أ���
// =============================================================================
void NFC_CsInactive(u8 CsNo)
{
    if(CsNo == NFC_CE0)
    {
        NFC->RAR &= ~(NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
    }
    else
    {
        NFC->RAR &= ~(NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }
}

// =============================================================================
// ���ܣ����͸�λ���NANDоƬ����λоƬ���������غ󣬸�λʱ���Ѿ����
// ��������
// ���أ���
// =============================================================================
void NFC_Reset(void)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC->ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Write the NFC_CMD2 register with the command byte and code for a reset */
    NFC->CMD2 = NFC_CMD2_BYTE1(RESET_CMD_BYTE) | NFC_CMD2_CODE(NFC_RESET_CMD_CODE);

    /* Set Start Bit to send reset to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */
    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
}

// =============================================================================
// ���ܣ���Flash��ID�����ص�ID��оƬ�����ɸ��ݾ��������ȡ���õ���Ϣ
// ������FlashBufNo,ʹ�õ�NFC�������ݵ�BUFFER�ţ���0-3
//       FlashID0,ID�ŵĸ�8�ֽڣ��������
//       FlashID1,ID�ŵĵ�8�ֽڣ��������
// ���أ���
// =============================================================================
void NFC_FlashIDRead(u8 FlashBufNo, u32* FlashID0, u32* FlashID1)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC->ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Write the NFC_CMD2 register with the command byte and code for a reset */
    NFC->CMD2 = NFC_CMD2_BYTE1(READ_ID_CMD_BYTE)
              | NFC_CMD2_CODE(NFC_READ_ID_CMD_CODE)
              | NFC_CMD2_BUFNO(FlashBufNo);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */
    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);

    /* Load the flash ID values into fls_id0 and fls_id1 */
    if(FlashID0 != NULL)
    {
    	*((u32 *)(FlashID0)) = NFC->SR1;
    }
    if(FlashID1 != NULL)
    {
    	*((u32 *)(FlashID1)) = NFC->SR2;
    }
}

// =============================================================================
// ���ܣ���flash��״̬��״̬��Ϣ��ӳ�˱��β����Ľ������д��������Ƿ�ɹ�
// ��������
// ���أ�Status,״ֵ̬����һ��8���ص���ֵ����ֵ�������NANDоƬ����
// =============================================================================
u8 NFC_FlashStatusRead(void)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC->ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Write the NFC_CMD2 register with the command byte and code for a reset */
    NFC->CMD2 = NFC_CMD2_BYTE1(READ_STATUS_CMD_BYTE)
              | NFC_CMD2_CODE(NFC_READ_STATUS_CODE)
              | NFC_CMD2_BUFNO(0);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */
    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);

    /* Load the flash ID values into fls_id0 and fls_id1 */

    return (u8)NFC->SR2;
}

// =============================================================================
// ���ܣ�����ָ���Ŀ飬�����Ŀ�������ݶκ�ECC��
// ������RowAddr,��ż����е�ַ
// ���أ���
// =============================================================================
void NFC_BlockErase(u32 RowAddr)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC->ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Disable ECC during block erase */
    NFC->CFG &= ~NFC_CFG_ECCMODE(0x7);

    /* Make sure the column address is cleared - not needed for block erase */
    NFC->CAR = 0x0;

    /* Set the chip select to use */
    if(NFC_CE == NFC_CE0)
    {
        NFC->RAR = (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
     }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC->RAR = (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }

    /* Set the row address */
    NFC->RAR |= RowAddr;

    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC->CMD2 = (NFC_CMD2_BYTE1(BLOCK_ERASE_CMD_BYTE1)
              | NFC_CMD2_CODE(NFC_BLOCK_ERASE_CMD_CODE) );

    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC->CMD1 = NFC_CMD1_BYTE2(BLOCK_ERASE_CMD_BYTE2)
                | NFC_CMD1_BYTE3(READ_STATUS_CMD_BYTE);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */
    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
}

// =============================================================================
// ���ܣ����ָ����ҳ���ڵ��øú���ǰ����̵�����Ӧ���Ѿ�д�뵽���ݻ�������
// ������FlashBufNo,ʹ�õ�NFC�������ݵ�BUFFER�ţ���0-3
//       RowAddr,�е�ַ����������ҳ��
//       ColAddr,�е�ַ����ҳ�ڵ�ַ
// ���أ���
// =============================================================================
void NFC_PageProgram(u8 FlashBufNo, u32 RowAddr, u16 ColAddr)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC->ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Make sure ECC is enabled before reading */
    NFC->CFG &= ~NFC_CFG_ECCMODE(0x7);

    /* Set the chip select to use */
    if(NFC_CE == NFC_CE0)
    {
        NFC->RAR = (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
     }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC->RAR = (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }

    /* Set the row address */
    NFC->RAR |= RowAddr;

    /* Set the column address */
    NFC->CAR = ColAddr;

    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC->CMD2 = (NFC_CMD2_BYTE1(PROGRAM_PAGE_CMD_BYTE1)
              | NFC_CMD2_CODE(NFC_PROGRAM_PAGE_CMD_CODE)
              | NFC_CMD2_BUFNO(FlashBufNo));

    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC->CMD1 = NFC_CMD1_BYTE2(PROGRAM_PAGE_CMD_BYTE2)
                | NFC_CMD1_BYTE3(READ_STATUS_CMD_BYTE);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */
    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
}

// =============================================================================
// ���ܣ���ȡָ��ҳ���ݣ��ں������غ󣬵����߿ɶ�ȡ�������е�����
// ������FlashBufNo,ʹ�õ�NFC�������ݵ�BUFFER�ţ���0-3
//       RowAddr,�е�ַ����������ҳ��
//       ColAddr,�е�ַ����ҳ�ڵ�ַ
// ���أ���
// =============================================================================
void NFC_PageRead(u8 FlashBufNo, u32 RowAddr, u16 ColAddr)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC->ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Make sure ECC is enabled before reading */
    NFC->CFG &= ~NFC_CFG_ECCMODE(0x7);

    /* Set the chip select to use */
    if(NFC_CE == NFC_CE0)
    {
        NFC->RAR = (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
     }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC->RAR = (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }

    /* Set the row address */
    NFC->RAR |= RowAddr;

    /* Set the column address */
    NFC->CAR = ColAddr;

    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC->CMD2 = (NFC_CMD2_BYTE1(PAGE_READ_CMD_BYTE1)
              | NFC_CMD2_CODE(NFC_READ_PAGE_CMD_CODE)
              | NFC_CMD2_BUFNO(FlashBufNo) );

    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_READ_CMD_BYTE2)
                | NFC_CMD1_BYTE3(READ_STATUS_CMD_BYTE);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */
    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
}

// =============================================================================
// ���ܣ������ݴ�NADN�������Ļ������ж���
// ������BufferNo,ʹ�õ�NFC�������ݵ�BUFFER�ţ���0-3
//       pDstBuf,Ŀ�껺������ַ
//       offset,����BUFFER��ƫ����
//       len,��ȡ���ֽ���
// ���أ�len,�������ֽ���
// =============================================================================
u32 NFC_ReadBuffer(u8* pDstBuf,u32 offset,u32 len)
{
	u32 result = 0;
	if(len > 0)
	{
		memcpy(pDstBuf,(u8 *)(NFC_BASE_PTR+offset),len);
		result = len;
	}

	return result;
}

// =============================================================================
// ���ܣ������ݴ�ŵ�ָ��NAND��������BUFFER������
// ������BufferNo,ʹ�õ�NFC�������ݵ�BUFFER�ţ���0-3
//       pDstBuf,Դ���ݵ�ַ
//       offset,����BUFFER��ƫ����
//       len,�ֽ���
// ���أ�len,�ֽ���
// =============================================================================
u32 NFC_WriteBuffer(u8* pSrcBuf,u32 offset,u32 len)
{
	u32 result = 0;
	if(len > 0)
	{
		memcpy((u8 *)(NFC_BASE_PTR+offset),pSrcBuf,len);
		result = len;
	}

	return result;
}












