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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_enet.c
// ģ������: ENETģ��ĳ�ʼ���Ĳ���������PHY��ʼ����MII��ʼ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 19/08.2014
// =============================================================================

#include "cpu_peri.h"
#include "string.h"

// ����MAC�������ã�Ĭ������ΪRMII��100M��ȫ˫��,����
static ENET_CONFIG s_EnetConfig = {MAC_RMII,AUTONEG_ON,MII_100BASET,MII_FDX,
                            NO_LOOPBACK,{0x1E,0x30,0x6C,0xA2,0x45,0x5E}};

// =============================================================================
// ���ܣ�MII�ӿ�д������ͨ��MDIO��PHYоƬͨ��
// ������phy_addr, PHY��ַ
//       PhyReg,�Ĵ�����ַ
//       Value,д�������
// ���أ�0,�������أ�1,ʧ��
// =============================================================================
static void __EMAC_PHY_Write (int PhyAddr,int PhyReg, int Value)
{
    u32 tout;

    LPC_EMAC->MADR = PhyAddr | PhyReg;
    LPC_EMAC->MWTD = Value;

    /* Wait utill operation completed */
    tout = 0;
    for (tout = 0; tout < MII_WR_TOUT; tout++)
    {
        if ((LPC_EMAC->MIND & MIND_BUSY) == 0)
        {
            break;
        }
    }
}

// =============================================================================
// ���ܣ�MII�ӿڶ����ݣ�ͨ��MDIO�ӿ���PHYоƬͨ��
// ������phy_addr, PHY��ַ
//       reg_addr,�Ĵ�����ַ
//       data,��ȡ��������ָ��
// ���أ�0,�������أ�1,ʧ��
// =============================================================================
static u16 __EMAC_PHY_Read (int PhyAddr,unsigned char PhyReg)
{
    u32 val,tout;

    LPC_EMAC->MADR = PhyAddr | PhyReg;
    LPC_EMAC->MCMD = MCMD_READ;

    /* Wait until operation completed */
    tout = 0;
    for (tout = 0; tout < MII_RD_TOUT; tout++)
    {
        if ((LPC_EMAC->MIND & MIND_BUSY) == 0)
        {
            break;
        }
    }
    LPC_EMAC->MCMD = 0;
    val = LPC_EMAC->MRDD;
    return (val);
}

// =============================================================================
// ���ܣ�PHYоƬ��ʼ����������ʹ������Ӧ��ʽ��ͨ����ȡPHYоƬ��SPEED��˫������ؼĴ�
//       �������������MAC����ز���
// ��������
// ���أ�true,link�ɹ���false,linkʧ��
// =============================================================================
static bool_t __EMAC_PHY_Init(void)
{
    u32 regv,tout;
    bool_t result = false;

    __EMAC_PHY_Write (DP83848C_DEF_ADR,PHY_REG_BMCR, 0x8000);//Reset PHY

    //Wait for hardware reset to end.
    for (tout = 0; tout < 0x100000; tout++)
    {
        regv = __EMAC_PHY_Read (DP83848C_DEF_ADR,PHY_REG_BMCR);
        if (!(regv & 0x8000))
            break;                          //Reset complete
    }

    if(s_EnetConfig.autoneg == AUTONEG_ON)
    {
        //Set Auto negotiation
        __EMAC_PHY_Write (DP83848C_DEF_ADR,PHY_REG_BMCR, PHY_AUTO_NEG);
        tout = 100;
        while(tout--)
        {
            regv = __EMAC_PHY_Read (DP83848C_DEF_ADR,PHY_REG_BMSR);
            if(regv & 0x0020)   break;          //Auto negotiation Complete.
            Djy_DelayUs(1*mS);
        }
    }
    else
    {
        //Manual Configure The Speed And Half/Full Duplex Mode
    }

    //Check the link status
    tout = 100;
    while(tout--)
    {
        regv = __EMAC_PHY_Read (DP83848C_DEF_ADR,PHY_REG_STS);
        if (regv & 0x0001)  break;              //Link is on
        Djy_DelayUs(1*mS);
    }
    if(tout != 0)
    {
        if (regv & 0x0004)
            s_EnetConfig.duplex = MII_FDX;
        else
            s_EnetConfig.duplex = MII_HDX;
        if (regv & 0x0002)
            s_EnetConfig.speed = MII_10BASET;
        else
            s_EnetConfig.speed = MII_100BASET;
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�����BD���ʼ�����������������顢�������������飬�Լ�����״̬���鶼������һ��
//       4 �ֽڣ�32  λ���ĵ�ַ�߽���룬������״̬�����������һ�� 8 �ֽڣ�64 λ����
//       ��ַ�߽����.ENET��BD��ʹ����SRAM�Ĵ�0x20080000��ʼ��16K������ַ�ռ�
// ��������
// ���أ���
// =============================================================================
static void __EMAC_RXBD_Init(void)
{
    u32 i;
    for (i = 0; i < NUM_RX_FRAG; i++)
    {
        RX_DESC_PACKET(i)  = RX_BUF(i);
        RX_DESC_CTRL(i)    = RCTRL_INT | (ETH_FRAG_SIZE-1);
        RX_STAT_INFO(i)    = 0;
        RX_STAT_HASHCRC(i) = 0;
    }

    /* Set EMAC Receive Descriptor Registers. */
    LPC_EMAC->RxDescriptor    = RX_DESC_BASE;
    LPC_EMAC->RxStatus        = RX_STAT_BASE;
    LPC_EMAC->RxDescriptorNumber = NUM_RX_FRAG-1;

    /* Rx Descriptors Point to 0 */
    LPC_EMAC->RxConsumeIndex  = 0;
}

// =============================================================================
// ���ܣ�����BD���ʼ�����������������顢�������������飬�Լ�����״̬���鶼������һ��
//       4 �ֽڣ�32  λ���ĵ�ַ�߽���룬������״̬�����������һ�� 8 �ֽڣ�64 λ����
//       ��ַ�߽����.ENET��BD��ʹ����SRAM�Ĵ�0x20080000��ʼ��16K������ַ�ռ�
// ��������
// ���أ���
// =============================================================================
static void __EMAC_TXBD_Init(void)
{
    u32 i;
    for (i = 0; i < NUM_TX_FRAG; i++)
    {
        TX_DESC_PACKET(i) = TX_BUF(i);
        TX_DESC_CTRL(i)   = 0;
        TX_STAT_INFO(i)   = 0;
    }

    /* Set EMAC Transmit Descriptor Registers. */
    LPC_EMAC->TxDescriptor    = TX_DESC_BASE;
    LPC_EMAC->TxStatus        = TX_STAT_BASE;
    LPC_EMAC->TxDescriptorNumber = NUM_TX_FRAG-1;

    /* Tx Descriptors Point to 0 */
    LPC_EMAC->TxProduceIndex  = 0;
}

// =============================================================================
// ���ܣ���̫��ģ��MAC��ʼ������Ҫ��MAC�Ĵ�����BD��ĳ�ʼ�����¼������裺
//       1.ENETģ��ʱ��ʹ�ܣ��������ã�����ENET��MDC�����ţ�
//       2.��λ��̫��ģ�飬��������Ĵ�������λ����TX��RX��
//       3.PHY��ʼ������Ҫ����LINK��SPEED��DUPLEX
//       4.��ʼ��BD������TCR��RCR�������ͽ��չ��ܣ�
//       3.MAC��ַ��ʼ����дMAC���Ĵ�����������Ϊ�Ƿ�ǿ���滻���Ͱ��е�Դ��ַ��
//       4.ʹ��ENET��
// ��������
// ���أ���
// =============================================================================
static void  __EMAC_Init(void)
{
    LPC_SC->PCONP |= (1<<30);                   //Power Up the EMAC controller.
    LPC_PINCON->PINSEL2 = 0x50150105;           //Enable P1 Ethernet Pins.
    LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0xF) | 0x5;   //MDC MDIO Pins

    //Reset all EMAC internal modules.
    LPC_EMAC->MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX
                    | MAC1_RES_MCS_RX | MAC1_SIM_RES | MAC1_SOFT_RES;
    LPC_EMAC->Command = CR_REG_RES | CR_TX_RES | CR_RX_RES | CR_PASS_RUNT_FRM;

    Djy_DelayUs(1*mS);                          //A short delay after reset.

    //Initialize MAC control registers.
    LPC_EMAC->MAC1 = MAC1_PASS_ALL;
    LPC_EMAC->MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
    LPC_EMAC->MAXF = ETH_MAX_FLEN;
    LPC_EMAC->CLRT = CLRT_DEF;
    LPC_EMAC->IPGR = IPGR_DEF;

    //Enable Reduced MII interface.
    if(s_EnetConfig.interface == MAC_RMII)
        LPC_EMAC->Command = CR_RMII | CR_PASS_RUNT_FRM;
    else
    {
        LPC_EMAC->Command &= ~CR_RMII;
        LPC_EMAC->Command |= CR_PASS_RUNT_FRM;
    }

    if(!__EMAC_PHY_Init())
        return;
    if(s_EnetConfig.duplex == MII_FDX)          //Full Duplex Mode
    {
        LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
        LPC_EMAC->Command |= CR_FULL_DUP;
        LPC_EMAC->IPGT     = IPGT_FULL_DUP;
    }
    else
    {
        LPC_EMAC->IPGT = IPGT_HALF_DUP;         //Half Duplex Mode
    }

    if(s_EnetConfig.speed == MII_100BASET)
        LPC_EMAC->SUPP = SUPP_SPEED;            //100Mbit Speed
    else
        LPC_EMAC->SUPP = 0;                     //10Mbit Speed

    //Set the Ethernet MAC Address registers
    LPC_EMAC->SA0 = (s_EnetConfig.mac[0] << 8) | s_EnetConfig.mac[1];
    LPC_EMAC->SA1 = (s_EnetConfig.mac[2] << 8) | s_EnetConfig.mac[3];
    LPC_EMAC->SA2 = (s_EnetConfig.mac[4] << 8) | s_EnetConfig.mac[5];

    //Initialize Tx and Rx DMA Descriptors
    __EMAC_RXBD_Init();
    __EMAC_TXBD_Init();

    //Receive Broadcast and Perfect Match Packets
    LPC_EMAC->RxFilterCtrl = RFC_BCAST_EN | RFC_PERFECT_EN;

    //Enable EMAC interrupts.
    LPC_EMAC->IntEnable = INT_RX_DONE | INT_TX_DONE;

    //Reset all interrupts
    LPC_EMAC->IntClear  = 0xFFFF;

    /* Enable receive and transmit mode of MAC Ethernet core */
    LPC_EMAC->Command  |= (CR_RX_EN | CR_TX_EN);
    LPC_EMAC->MAC1     |= MAC1_REC_EN;
}

// =============================================================================
// ���ܣ�Enet�жϳ�ʼ����ʹ���жϵķ�ʽ�������ݣ����������жϼĴ���
// ��������
// ���أ���
// =============================================================================
static void __EMAC_IntInit(void)
{
    ufast_t IntLine = CN_INT_LINE_ENET;
    u32 Enet_ISR(ufast_t IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_PRE);
    Int_IsrConnect(IntLine,Enet_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ���ܣ���̫��ģ���жϷ��������������͡�����֡��ɺʹ����жϣ��Դ���֡�Ĵ��������
//       �Ƿ����Ӳ����λ���������������ʱ��������������BD��
// ������int_line,�ж��ߺ�
// ���أ�0
// =============================================================================
u32 Enet_ISR(ufast_t IntLine)
{
    u32 idx;
    //Tx Frame Done Int
    if(LPC_EMAC->IntStatus & INT_TX_DONE)
    {

    }

    //Rx Frame Done Int
    if(LPC_EMAC->IntStatus & INT_RX_DONE)
    {
        idx = LPC_EMAC->RxConsumeIndex;
        if(++idx == NUM_RX_FRAG) idx = 0;
        LPC_EMAC->RxConsumeIndex = idx;
    }

    //Error Int
    if(LPC_EMAC->IntStatus & (INT_RX_ERR | INT_TX_ERR))
    {

    }

    LPC_EMAC->IntClear = 0xFFFF;        //Clear The INT Flag
    return 0;
}

u32 Enet_Ctrl(u32 cmd,ptu32_t data1, ptu32_t data2)
{
    return 0;
}

// =============================================================================
// ���ܣ���̫�����ݰ����ͺ���������⵽��ʱ��ͻ��BD����������������0����ʾ���η���ʧ
//       �ܣ�һ֡���ݷ�����ɺ󣬻����TX_DONE�ж�
// ������packet,���������ݰ�ָ��
//       length,���ݰ����ȣ��ֽڵ�λ�����ó���FRAME��󳤶ȣ�1536��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t Enet_SendPacket(u8 *packet,u32 length)
{
    u8 *tptr,idx;
    u32 FrameSize;

    FrameSize = length - 1;
    idx  = LPC_EMAC->TxProduceIndex;
    if(LPC_EMAC->TxProduceIndex == LPC_EMAC->TxConsumeIndex - 1)    //FULL
    {
        return false;
    }
    tptr = (u8 *)TX_DESC_PACKET(idx);
    TX_DESC_CTRL(idx) = FrameSize | TCTRL_LAST | TCTRL_INT;

    memcpy(tptr,packet,length);
    if (++idx == NUM_TX_FRAG)
        idx = 0;
    LPC_EMAC->TxProduceIndex = idx;
    return true;
}

// =============================================================================
// ���ܣ���̫�����ݰ���ȡ�������յ�һ�������̫������ȫ����֡����ж�
// ������packet,���ݴ洢ָ��
// ���أ�length,���ݰ����ȣ��ֽڵ�λ�����ó���FRAME��󳤶ȣ�1536��
// =============================================================================
u32 Enet_RecvPacket(u8* packet)
{
    u32 idx,len,length = 0;
    u8* rptr;
//  if(s_RxFrameDoneFlash == false)
//      return length;
//  s_RxFrameDoneFlash = false;
    while(LPC_EMAC->RxProduceIndex != LPC_EMAC->RxConsumeIndex)
    {
        idx = LPC_EMAC->RxConsumeIndex;
        rptr = (u8*)RX_DESC_PACKET(idx);
        len = RX_STAT_INFO(idx) & RINFO_SIZE;
        memcpy(packet,rptr,len);
        packet += len;                          //ָ������
        length += len;
        if(++idx == NUM_RX_FRAG)    idx = 0;
        LPC_EMAC->RxConsumeIndex = idx;
        //ÿ�ζ�һ֡����
        if(RX_STAT_INFO(idx) & RINFO_LAST_FLAG)
        {
            break;
        }
    }
    return length;
}

// =============================================================================
// ���ܣ���ʼ����̫��ģ�飬��ģ���ʼ��ʱ����Ĭ�ϵĲ������ã���PHY����Ӧģʽ��RMII��
//       �ڡ�100MBIT��ȫ˫����ʽ����ʼ��������PHY�ĳ�ʼ����ͨ�ź���̫��MAC�Ĵ�����ʼ
//       �������У�BD��������Ҫ�������жϷ�ʽ���ͺͽ��ա�
// ������para,δʹ��
// ���أ�0
// =============================================================================
ptu32_t ModuleInstall_Enet(ptu32_t para)
{
    __EMAC_Init();
    __EMAC_IntInit();
    return 0;
}
