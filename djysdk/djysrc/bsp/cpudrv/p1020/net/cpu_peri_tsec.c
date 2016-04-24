//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   ������.
//����ģ��: tsec��������ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: tsec���������ļ�,����etsec3���ڵ�phy�����ڵ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-11-14
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

#include "os.h"

#include <asm/io.h>
#include <cpu_peri.h>
#include "phy_88e1512.h"
#include "cpu_peri_tsec.h"
#include "pkg.h"

#define TSECDEBUG
#ifdef TSECDEBUG
#define _DEBUG    1
#else
#define _DEBUG    0
#endif


typedef struct _tagTsecSndPkgBuf
{
	struct _tagTsecSndPkgBuf *nxt;
	tagNetPkg *plst;
	u8 pkgnum;
}tagTsecSndPkgBuf;

struct tsec_info_struct
{
    tsec_t *regs;
    struct tsec_mii_mng *miiregs_sgmii;
    char *devname;
    char *mii_devname;
    phy_interface_t interface;
    u32 phyaddr;
    u32  flags;
    unsigned char enetaddr[6];
    void *priv;
    
    u8  devno;
    tagTsecSndPkgBuf   *sndpkgbufhead;
    tagTsecSndPkgBuf   *sndpkgbuftail;
};
/*
 * Output a debug text when condition "cond" is met. The "cond" should be
 * computed by a preprocessor in the best case, allowing for the best
 * optimization.
 */
#define debug_cond(cond, fmt, args...)        \
    do {                    \
        if (cond)            \
            printk(fmt, ##args);    \
    } while (0)

#define tsec_debug(fmt, args...)            \
    debug_cond(_DEBUG, fmt, ##args)

/*
 * AS.HARNOIS : Better to set PKTSIZE to maximum size because
 * traffic type is not always controlled
 * maximum packet size =  1518
 * maximum packet size and multiple of 32 bytes =  1536
 */
#define PKTSIZE            1518
#define PKTSIZE_ALIGN        1536

#define PKTBUFSRX    4
#define TX_BUF_CNT        2
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define CRCPOLY_LE 0xedb88320

static u32 rxIdx;        /* index of the current RX buffer */
static u32 txIdx;        /* index of the current TX buffer */

typedef volatile struct rtxbd
{
    txbd8_t txbd[TX_BUF_CNT];
    rxbd8_t rxbd[PKTBUFSRX];
} RTXBD;

#define MAXCONTROLLERS    (8)

#ifdef __GNUC__
static RTXBD rtx __attribute__ ((aligned(8)));
#else
#error "rtx must be 64-bit aligned"
#endif
//NetRxPacketsʹ�����洢���հ������飬һ��������󳤶�Ϊ1536��һ��4�����У�
//��tsec�е�RxBD��Ӧ����RxBD�е�bufָ��ָ��NetRxPackets[i]�ĵ�ַ����ÿ�ν���
//�����ݾʹӸõ�ַȥ��ȡ��
static u8 NetRxPackets[PKTBUFSRX][PKTSIZE_ALIGN]= {{0}};


/* Default initializations for TSEC controllers. */

static struct tsec_info_struct tsec_info[] =
{
#if    (CONFIG_TSEC1 == 1)
    STD_TSEC_INFO(1),    /* TSEC1 */
#endif
#if    (CONFIG_TSEC2 == 1)
    STD_TSEC_INFO(2),    /* TSEC2 */
#endif
#if    (CONFIG_TSEC3 == 1)
    STD_TSEC_INFO(3),    /* TSEC3 */
#endif
#if    (CONFIG_TSEC4 == 1)
    STD_TSEC_INFO(4),    /* TSEC4 */
#endif
};

#define TBIANA_SETTINGS ( \
        TBIANA_ASYMMETRIC_PAUSE \
        | TBIANA_SYMMETRIC_PAUSE \
        | TBIANA_FULL_DUPLEX \
        )

/* By default force the TBI PHY into 1000Mbps full duplex when in SGMII mode */
#ifndef CONFIG_TSEC_TBICR_SETTINGS
#define CONFIG_TSEC_TBICR_SETTINGS ( \
        TBICR_PHY_RESET \
        | TBICR_ANEG_ENABLE \
        | TBICR_FULL_DUPLEX \
        | TBICR_SPEED1_SET \
        )
#endif /* CONFIG_TSEC_TBICR_SETTINGS */

/* Enable or disable autonegotiation.  If this is set to enable,
 * the forced link modes above are completely ignored.
 */
#define AUTONEG_DISABLE        0x00
#define AUTONEG_ENABLE        0x01

#define SPEED_10    10
#define SPEED_100   100
#define SPEED_1000  1000
#define HALF_DUPLEX 1
#define FULL_DUPLEX 2

/* Duplex, half or full. */
#define DUPLEX_HALF        0x00
#define DUPLEX_FULL        0x01
#if 0
static s32 tsec_send(struct tsec_info_struct *tsec_info, void *packet, s32 length);
//----�������������շ�����-------------------------------------------------------
//���ܣ��ڳ�ʼ��ʱ���ó�loopbackģʽ���Ի��շ����۲����ڵ��շ��Ƿ�������
//      ������tsec�ĳ�ʼ��û��ʹ��
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//���أ���
//-----------------------------------------------------------------------------
void redundant_init(struct tsec_info_struct *tsec_info)
{
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;
    u32  t, count = 0;
    s32 fail = 1;
    static const u8 pkt[] =
    {
        0x00, 0x1e, 0x4f, 0x12, 0xcb, 0x2c, 0x00, 0x25,
        0x64, 0xbb, 0xd1, 0xab, 0x08, 0x00, 0x45, 0x00,
        0x00, 0x5c, 0xdd, 0x22, 0x00, 0x00, 0x80, 0x01,
        0x1f, 0x71, 0x0a, 0xc1, 0x14, 0x22, 0x0a, 0xc1,
        0x14, 0x6a, 0x08, 0x00, 0xef, 0x7e, 0x02, 0x00,
        0x94, 0x05, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
        0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
        0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
        0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
        0x71, 0x72
    };

    /* Enable promiscuous mode */
    setbits_be32(&regs->rctrl, 0x8);
    /* Enable loopback mode */
    setbits_be32(&regs->maccfg1, MACCFG1_LOOPBACK);
    /* Enable transmit and receive */
    setbits_be32(&regs->maccfg1, MACCFG1_RX_EN | MACCFG1_TX_EN);

    /* Tell the DMA it is clear to go */
    setbits_be32(&regs->dmactrl, DMACTRL_INIT_SETTINGS);
    out_be32(&regs->tstat, TSTAT_CLEAR_THALT);
    out_be32(&regs->rstat, RSTAT_CLEAR_RHALT);
    clrbits_be32(&regs->dmactrl, DMACTRL_GRS | DMACTRL_GTS);

    do
    {

        tsec_send(tsec_info, (void *)pkt, sizeof(pkt));

        /* Wait for buffer to be received */
        for (t = 0; rtx.rxbd[rxIdx].status & RXBD_EMPTY; t++)
        {
            if (t >= 10 * TOUT_LOOP)
            {
                printk("%s: tsec: rx error\r\n", priv->name);
                break;
            }
        }

        if (!memcmp(pkt, (void *)NetRxPackets[rxIdx], sizeof(pkt)))
        {
            fail = 0;
            printk("%s: tsec: loopback succed\r\n", priv->name);
        }

        rtx.rxbd[rxIdx].length = 0;
        rtx.rxbd[rxIdx].status =
            RXBD_EMPTY | (((rxIdx + 1) == PKTBUFSRX) ? RXBD_WRAP : 0);
        rxIdx = (rxIdx + 1) % PKTBUFSRX;

        if (in_be32(&regs->ievent) & IEVENT_BSY)
        {
            out_be32(&regs->ievent, IEVENT_BSY);
            out_be32(&regs->rstat, RSTAT_CLEAR_RHALT);
        }
        if (fail)
        {
            printk("loopback recv packet error!\r\n");
            clrbits_be32(&regs->maccfg1, MACCFG1_RX_EN);
            Djy_DelayUs(1000);
            setbits_be32(&regs->maccfg1, MACCFG1_RX_EN);
        }
    }
    while ((count++ < 10) && (fail == 1));

    if (fail)
        printk("eTSEC init fail!\r\n");
    /* Disable promiscuous mode */
    clrbits_be32(&regs->rctrl, 0x8);
    /* Disable loopback mode */
    clrbits_be32(&regs->maccfg1, MACCFG1_LOOPBACK);
}
#endif
/**
 * genphy_update_link - update link status in @phydev
 * @phydev: target phy_device struct
 *
 * Description: Update the value in phydev->link to reflect the
 *   current link value.  In order to do this, we need to read
 *   the status register twice, keeping the second value.
 */
//----phy�ӿ����Ӹ��º���--------------------------------------------------
//���ܣ�����PHYоƬΪ�Զ�ƥ��ģʽ����ʼ��ʱ���PHY���ӵ���·�����
//������struct tsec_private *priv��tsec����PHY���ýṹ��
//���أ�����0
//-------------------------------------------------------------------------
static s32 genphy_update_link(struct tsec_private *priv)
{
    u32 mii_reg;
    /*
     * Wait if the link is up, and autonegotiation is in progress
     * (ie - we're capable and it's not done)
     */
    mii_reg = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_BMSR);

    if (priv->link && mii_reg & BMSR_LSTATUS)
        return 0;

    if ((mii_reg & BMSR_ANEGCAPABLE) && !(mii_reg & BMSR_ANEGCOMPLETE))
    {
        s32 i = 0;

        printk("Waiting for PHY auto negotiation to complete");
        while (!(mii_reg & BMSR_ANEGCOMPLETE))
        {
            /*
             * Timeout reached ?
             */
            if (i > PHY_ANEG_TIMEOUT)
            {
                printk(" TIMEOUT !\r\n");
                priv->link = 0;
                return 0;
            }

            if ((i++ % 500) == 0)
                printk(".");

            Djy_DelayUs(1000);    /* 1 ms */
            mii_reg = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_BMSR);
        }
        printk(" done\r\n");
        priv->link = 1;
    }
    else
    {
        /* Read the link a second time to clear the latched state */
        mii_reg = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_BMSR);

        if (mii_reg & BMSR_LSTATUS)
            priv->link = 1;
        else
            priv->link = 0;
    }

    return 0;
}
/*
 * Generic function which updates the speed and duplex.  If
 * autonegotiation is enabled, it uses the AND of the link
 * partner's advertised capabilities and our advertised
 * capabilities.  If autonegotiation is disabled, we use the
 * appropriate bits in the control register.
 *
 * Stolen from Linux's mii.c and phy_device.c
 */
//----��������PHY״̬��Ϣ����-------------------------------------------
//���ܣ������Զ�ƥ��ģʽ����ȡƥ���phy��״̬�����ٶȡ�ȫ��˫��
//      ģʽ����Ϣ����priv�ṹ���С�
//������struct tsec_private *priv��tsec����PHY���ýṹ��
//���أ�����0
//----------------------------------------------------------------------
static s32 genphy_parse_link(struct tsec_private *priv)
{
    s32 mii_reg = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_BMSR);

    /* We're using autonegotiation */
    if (mii_reg & BMSR_ANEGCAPABLE)
    {
        u32 lpa = 0;
        u32 gblpa = 0;

        /* Check for gigabit capability */
        if (mii_reg & BMSR_ERCAP)
        {
            /* We want a list of states supported by
             * both PHYs in the link
             */
            gblpa = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_STAT1000);
            gblpa &= phy_read(priv->phyaddr,
                              MDIO_DEVAD_NONE, MII_CTRL1000) << 2;
        }

        /* Set the baseline so we only have to set them
         * if they're different
         */
        priv->speed = SPEED_10;
        priv->duplex = DUPLEX_HALF;

        /* Check the gigabit fields */
        if (gblpa & (PHY_1000BTSR_1000FD | PHY_1000BTSR_1000HD))
        {
            priv->speed = SPEED_1000;

            if (gblpa & PHY_1000BTSR_1000FD)
                priv->duplex = DUPLEX_FULL;

            /* We're done! */
            return 0;
        }

        lpa = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_ADVERTISE);
        lpa &= phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_LPA);

        if (lpa & (LPA_100FULL | LPA_100HALF))
        {
            priv->speed = SPEED_100;

            if (lpa & LPA_100FULL)
                priv->duplex = DUPLEX_FULL;

        }
        else if (lpa & LPA_10FULL)
            priv->duplex = DUPLEX_FULL;
    }
    else
    {
        u32 bmcr = phy_read(priv->phyaddr, MDIO_DEVAD_NONE, MII_BMCR);

        priv->speed = SPEED_10;
        priv->duplex = DUPLEX_HALF;

        if (bmcr & BMCR_FULLDPLX)
            priv->duplex = DUPLEX_FULL;

        if (bmcr & BMCR_SPEED1000)
            priv->speed = SPEED_1000;
        else if (bmcr & BMCR_SPEED100)
            priv->speed = SPEED_100;
    }

    return 0;
}
//----phy��ʼ������-----------------------------------------------
//���ܣ�����phy�Ĵ��������ó��Զ�ƥ��ģʽ��������������ӣ�
//      ����·���ӽ�������Ϣ�����ڽṹ����
//������struct tsec_private *priv��tsec����PHY���ýṹ��
//���أ�����0
//----------------------------------------------------------------
s32 phy_startup(struct tsec_private *priv)
{
    struct phy_parameter para;
    if(priv->interface == PHY_INTERFACE_MODE_RGMII) //tsec3
    {
        priv->link = genphy_update_link_88E1512(priv->phyaddr);
        para = genphy_parse_link__88E1512(priv->phyaddr);
        priv->speed = para.speed;
        priv->duplex = para.duplex;
    }
    else if(priv->interface == PHY_INTERFACE_MODE_SGMII)//tsec2
    {
        priv->link = 1;
        priv->speed = SPEED_1000;
        priv->duplex = DUPLEX_FULL;
    }
    else
    {
        genphy_update_link(priv);
        genphy_parse_link(priv);
    }
    return 0;
}

//----phy��λ����-----------------------------------------------------------
//���ܣ���λAtheros PHY�Ĵ�����ֻ���p1020rdb��etsec3�ڵ�ar8201 phyоƬ
//������int addr��phyоƬ��ַ
//���أ�����0��ʾ��λ�ɹ�
//----------------------------------------------------------------------------
s32 phy_reset(s32 addr)
{
    s32 reg;
    s32 timeout = 500;
    s32 devad = MDIO_DEVAD_NONE;

    reg = phy_read(addr, devad, MII_BMCR);
    if (reg < 0)
    {
        tsec_debug("PHY status read failed\n");
        return -1;
    }

    reg |= BMCR_RESET;

    if (phy_write(addr, devad, MII_BMCR, reg) < 0)
    {
        tsec_debug("PHY reset failed\n");
        return -1;
    }

    /*
     * Poll the control register for the reset bit to go to 0 (it is
     * auto-clearing).  This should happen within 0.5 seconds per the
     * IEEE spec.
     */
    while ((reg & BMCR_RESET) && timeout--)
    {
        reg = phy_read(addr, devad, MII_BMCR);

        if (reg < 0)
        {
            tsec_debug("PHY status read failed\n");
            return -1;
        }
        Djy_DelayUs(1000);
    }

    if (reg & BMCR_RESET)
    {
        printk("PHY reset timed out\n");
        return -1;
    }

    return 0;
}
/*configure MDIO*/
//----tsec MDIO�Ĵ���д����----------------------------------------------------
//���ܣ�дtsec MDIO�Ĵ���
//������struct tsec_mii_mng *phyregs, etsec��mii�Ĵ�����int port_addr,PHY��ַ
//s32 regnum,TBI�мĴ���ƫ������int value��д���ֵ
//���أ���
//-----------------------------------------------------------------------------
void tsec_local_mdio_write(struct tsec_mii_mng *phyregs, s32 port_addr,
                           s32 dev_addr, s32 regnum, s32 value)
{
    s32 timeout = 1000000;

    out_be32(&phyregs->miimadd, (port_addr << 8) | (regnum & 0x1f));
    out_be32(&phyregs->miimcon, value);
    asm("sync");

    while ((in_be32(&phyregs->miimind) & MIIMIND_BUSY) && timeout--)
        ;
}
//----tsec MDIO�Ĵ���������----------------------------------------------------
//���ܣ���tsec MDIO�Ĵ���
//������struct tsec_mii_mng *phyregs, etsec��mii�Ĵ���
//      int port_addr,PHY��ַ
//      s32 regnum,TBI�мĴ���ƫ����
//���أ�����ֵ
//-----------------------------------------------------------------------------
s32 tsec_local_mdio_read(struct tsec_mii_mng *phyregs, s32 port_addr,
                         s32 dev_addr, s32 regnum)
{
    s32 value;
    s32 timeout = 1000000;

    /* Put the address of the phy, and the register
     * number into MIIMADD */
    out_be32(&phyregs->miimadd, (port_addr << 8) | (regnum & 0x1f));

    /* Clear the command register, and wait */
    out_be32(&phyregs->miimcom, 0);
    asm("sync");

    /* Initiate a read command, and wait */
    out_be32(&phyregs->miimcom, MIIMCOM_READ_CYCLE);
    asm("sync");

    /* Wait for the the indication that the read is done */
    while ((in_be32(&phyregs->miimind) & (MIIMIND_NOTVALID | MIIMIND_BUSY))
            && timeout--)
        ;

    /* Grab the value read from the PHY */
    value = in_be32(&phyregs->miimstat);

    return value;
}

s32 tsec_phy_read(s32 addr, s32 dev_addr, s32 regnum)
{
    struct tsec_mii_mng *phyregs = (struct tsec_mii_mng *)CONFIG_SYS_MDIO_BASE_ADDR;

    return tsec_local_mdio_read(phyregs, addr, dev_addr, regnum);
}

s32 tsec_phy_write(s32 addr, s32 dev_addr, s32 regnum,
                   u16 value)
{
    struct tsec_mii_mng *phyregs = (struct tsec_mii_mng *)CONFIG_SYS_MDIO_BASE_ADDR;

    tsec_local_mdio_write(phyregs, addr, dev_addr, regnum, value);

    return 0;
}

/* Configure the TBI for SGMII operation */
//---����TBI�Ĵ�������-------------------------------------------
//���ܣ�p1020rdb�в���tsec��sgmillģʽ��phy�������ӣ�����
//      tbi�Ĵ��������ⲿphy
//������struct tsec_private *priv��tsec����PHY���ýṹ��
//���أ���
//---------------------------------------------------------------
static void tsec_configure_serdes(struct tsec_private *priv)
{
    /* Access TBI PHY registers at given TSEC register offset as
     *opposed to the register offset used for external PHY accesses */
    tsec_local_mdio_write(priv->phyregs_sgmii,
                          in_be32(&priv->regs->tbipa),
                          0, TBI_ANA, TBIANA_SETTINGS);
    tsec_local_mdio_write(priv->phyregs_sgmii,
                          in_be32(&priv->regs->tbipa),
                          0, TBI_TBICON, TBICON_CLK_SELECT);
    tsec_local_mdio_write(priv->phyregs_sgmii,
                          in_be32(&priv->regs->tbipa),
                          0, TBI_CR, CONFIG_TSEC_TBICR_SETTINGS);
}

/* Initialized required registers to appropriate values, zeroing
 * those we don't care about (unless zero is bad, in which case,
 * choose a more appropriate value)
 */
//---��ʼ��tsec�Ĵ�������---------------------------------------
//���ܣ���ʼ��tsec�Ĵ���������
//������struct tsec_t *regs��tsec���ڼĴ����ṹ��
//���أ���
//--------------------------------------------------------------
static void init_registers(tsec_t *regs)
{
    /* Clear IEVENT */
    out_be32(&regs->ievent, IEVENT_INIT_CLEAR);

    out_be32(&regs->imask, IMASK_INIT_CLEAR);

    out_be32(&regs->hash.iaddr0, 0);
    out_be32(&regs->hash.iaddr1, 0);
    out_be32(&regs->hash.iaddr2, 0);
    out_be32(&regs->hash.iaddr3, 0);
    out_be32(&regs->hash.iaddr4, 0);
    out_be32(&regs->hash.iaddr5, 0);
    out_be32(&regs->hash.iaddr6, 0);
    out_be32(&regs->hash.iaddr7, 0);

    out_be32(&regs->hash.gaddr0, 0);
    out_be32(&regs->hash.gaddr1, 0);
    out_be32(&regs->hash.gaddr2, 0);
    out_be32(&regs->hash.gaddr3, 0);
    out_be32(&regs->hash.gaddr4, 0);
    out_be32(&regs->hash.gaddr5, 0);
    out_be32(&regs->hash.gaddr6, 0);
    out_be32(&regs->hash.gaddr7, 0);

    out_be32(&regs->rctrl, 0x00000000);

    /* Init RMON mib registers */
    memset((void *)&(regs->rmon), 0, sizeof(rmon_mib_t));

    out_be32(&regs->rmon.cam1, 0xffffffff);
    out_be32(&regs->rmon.cam2, 0xffffffff);

    out_be32(&regs->mrblr, MRBLR_INIT_SETTINGS);

    out_be32(&regs->minflr, MINFLR_INIT_SETTINGS);

    out_be32(&regs->attr, ATTR_INIT_SETTINGS);
    out_be32(&regs->attreli, ATTRELI_INIT_SETTINGS);

}

/* Configure maccfg2 based on negotiated speed and duplex
 * reported by PHY handling code
 */
//---����maccfg2�Ĵ�������-----------------------------------------
//���ܣ�����֮ǰphy��·�������õ��ٶȺ�˫��ģʽ������maccfg2�Ĵ���
//������struct tsec_private *priv��tsec����PHY���ýṹ��
//���أ���
//----------------------------------------------------------------
static void adjust_link(struct tsec_private *priv)
{
    tsec_t *regs = priv->regs;
    u32  ecntrl, maccfg2;

    if (!priv->link)
    {
        printk("%s: No link.\r\n",priv->name);
        return;
    }

    /* clear all bits relative with interface mode */
    ecntrl = in_be32(&regs->ecntrl);
    ecntrl &= ~ECNTRL_R100;

    maccfg2 = in_be32(&regs->maccfg2);
    maccfg2 &= ~(MACCFG2_IF | MACCFG2_FULL_DUPLEX);

    if (priv->duplex)
        maccfg2 |= MACCFG2_FULL_DUPLEX;

    switch (priv->speed)
    {
    case 1000:
        maccfg2 |= MACCFG2_GMII;
        break;
    case 100:
    case 10:
        maccfg2 |= MACCFG2_MII;

        /* Set R100 bit in all modes although
         * it is only used in RGMII mode
         */
        if (priv->speed == 100)
            ecntrl |= ECNTRL_R100;
        break;
    default:
        printk("%s: Speed was bad\n", priv->name);
        break;
    }

    out_be32(&regs->ecntrl, ecntrl);
    out_be32(&regs->maccfg2, maccfg2);

    printk("Speed: %d, %s duplex\r\n", priv->speed,
               (priv->duplex) ? "full" : "half");
}
/* Set up the buffers and their descriptors, and bring up the
 * interface
 */
//---����tsec����-------------------------------------------------------
//���ܣ�����tsec�����շ����壬ʹ���շ�
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//���أ���
//----------------------------------------------------------------------
static void startup_tsec(struct tsec_info_struct *tsec_info)
{
    s32 i;
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;

    /* reset the indices to zero */
    rxIdx = 0;
    txIdx = 0;

    /* Point to the buffer descriptors */
    out_be32(&regs->tbase, (u32)(&rtx.txbd[txIdx]));
    out_be32(&regs->rbase, (u32)(&rtx.rxbd[rxIdx]));

    /* Initialize the Rx Buffer descriptors */
    for (i = 0; i < PKTBUFSRX; i++)
    {
        rtx.rxbd[i].status = RXBD_EMPTY;
        rtx.rxbd[i].length = 0;
        rtx.rxbd[i].bufPtr = (u32 )NetRxPackets[i] ;
    }
    rtx.rxbd[PKTBUFSRX - 1].status |= RXBD_WRAP;

    /* Initialize the TX Buffer Descriptors */
    for (i = 0; i < TX_BUF_CNT; i++)
    {
        rtx.txbd[i].status = 0;
        rtx.txbd[i].length = 0;
        rtx.txbd[i].bufPtr = 0;
    }
    rtx.txbd[TX_BUF_CNT - 1].status |= TXBD_WRAP;

//    redundant_init(tsec_info);//�ػ�����

    /* Enable Transmit and Receive */
    setbits_be32(&regs->maccfg1, MACCFG1_RX_EN | MACCFG1_TX_EN);
    /* Tell the DMA it is clear to go */
    setbits_be32(&regs->dmactrl, DMACTRL_INIT_SETTINGS);
    out_be32(&regs->tstat, TSTAT_CLEAR_THALT);
    out_be32(&regs->rstat, RSTAT_CLEAR_RHALT);
    clrbits_be32(&regs->dmactrl, DMACTRL_GRS | DMACTRL_GTS);
}

/* This returns the status bits of the device.    The return value
 * is never checked, and this is what the 8260 driver did, so we
 * do the same.     Presumably, this would be zero if there were no
 * errors
 */
//---tsec���ͺ���---------------------------------------------------------
//���ܣ�tsec���ڷ��ͺ���
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//void *packet,���Ͱ�ָ�룬int length�������ĳ���
//���أ����ط���״̬
//------------------------------------------------------------------------
static s32 tsec_send(struct tsec_info_struct *tsec_info,
                     void *packet, s32 length)
{
    s32 i;
    s32 result = 0;
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;

    /* Find an empty buffer descriptor */
    for (i = 0; rtx.txbd[txIdx].status & TXBD_READY; i++)
    {
        if (i >= TOUT_LOOP)
        {
            tsec_debug("%s: tsec: tx buffers full\n", tsec_info->devname);
            return result;
        }
    }

    rtx.txbd[txIdx].bufPtr = (u32) packet;
    rtx.txbd[txIdx].length = length;
    rtx.txbd[txIdx].status |=
        (TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT);

    /* Tell the DMA to go */
    out_be32(&regs->tstat, TSTAT_CLEAR_THALT);

    /* Wait for buffer to be transmitted */
    for (i = 0; rtx.txbd[txIdx].status & TXBD_READY; i++)
    {
        if (i >= TOUT_LOOP)
        {
            tsec_debug("%s: tsec: tx error\n", tsec_info->devname);
            return result;
        }
    }

    txIdx = (txIdx + 1) % TX_BUF_CNT;
    result = rtx.txbd[txIdx].status & TXBD_STATS;

    return result;
}
//---tsec���պ���------------------------------------------------------
//���ܣ�tsec���ڽ��պ���
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//u8 *packet,���հ�ָ��
//���أ����ؽ���״̬
//---------------------------------------------------------------------
static u32 tsec_recv(struct tsec_info_struct *tsec_info,u8* packet)
{
    s32 length = 0;
    s32 i;
    u8 *tempbuf = NULL;
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;

    while (!(rtx.rxbd[rxIdx].status & RXBD_EMPTY))
    {

        length = rtx.rxbd[rxIdx].length;
        /* Send the packet up if there were no errors */
        if (!(rtx.rxbd[rxIdx].status & RXBD_STATS))
        {
#if 1
            tempbuf = NetRxPackets[rxIdx];
            //NetReceive(NetRxPackets[rxIdx], length - 4);
            for(i = 0; i<(length - 4); i++)
            {
                packet[i]=tempbuf[i];
            }
            //DumpBuffer(packet,length-4);
#endif
        }
        else
        {
            printk("Got error %x\n",
                       (rtx.rxbd[rxIdx].status & RXBD_STATS));
        }

        rtx.rxbd[rxIdx].length = 0;

        /* Set the wrap bit if this is the last element in the list */
        rtx.rxbd[rxIdx].status =
            RXBD_EMPTY | (((rxIdx + 1) == PKTBUFSRX) ? RXBD_WRAP : 0);

        rxIdx = (rxIdx + 1) % PKTBUFSRX;
    }

    if (in_be32(&regs->ievent) & IEVENT_BSY)
    {
        out_be32(&regs->ievent, IEVENT_BSY);
        out_be32(&regs->rstat, RSTAT_CLEAR_RHALT);
    }
    if(length > 4)
        return (length - 4);
    else
        return 0;

}
/* Stop the interface */
//---tsec����ֹͣ����---------------------------------------------------
//���ܣ�ֹͣʹ��tsec���ڽӿ�
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//���أ���
//----------------------------------------------------------------------
static void tsec_halt(struct tsec_info_struct *tsec_info)
{
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;

    clrbits_be32(&regs->dmactrl, DMACTRL_GRS | DMACTRL_GTS);
    setbits_be32(&regs->dmactrl, DMACTRL_GRS | DMACTRL_GTS);
#if 1
    while ((in_be32(&regs->ievent) & (IEVENT_GRSC | IEVENT_GTSC))
            != (IEVENT_GRSC | IEVENT_GTSC))
        ;
#endif
    clrbits_be32(&regs->maccfg1, MACCFG1_TX_EN | MACCFG1_RX_EN);

    /* Shut down the PHY, as needed */
}

/* Initializes data structures and registers for the controller,
 * and brings the interface up.     Returns the link status, meaning
 * that it returns success if the link is up, failure otherwise.
 * This allows u-boot to find the first active controller.
 */
//---tsec���ڳ�ʼ������-------------------------------------------------
//���ܣ�tsec���ڽӿڳ�ʼ��
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//���أ�����tsec������·����״̬
//----------------------------------------------------------------------
static s32 tsec_init(struct tsec_info_struct *tsec_info)
{
    u32 tempval;
    char tmpbuf[MAC_ADDR_LEN];
    s32 i;
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;
    s32 ret;

    /* Make sure the controller is stopped */
    tsec_halt(tsec_info);

    /* Init MACCFG2.  Defaults to GMII */
    out_be32(&regs->maccfg2, MACCFG2_INIT_SETTINGS);

    /* Init ECNTRL */
    out_be32(&regs->ecntrl, ECNTRL_INIT_SETTINGS);

    /* Copy the station address into the address registers.
     * Backwards, because little endian MACS are dumb */
    for (i = 0; i < MAC_ADDR_LEN; i++)
        tmpbuf[MAC_ADDR_LEN - 1 - i] = tsec_info->enetaddr[i];

    tempval = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) |
              tmpbuf[3];

    out_be32(&regs->macstnaddr1, tempval);

    tempval = *((u32 *) (tmpbuf + 4));

    out_be32(&regs->macstnaddr2, tempval);

    /* Clear out (for the most part) the other registers */
    init_registers(regs);

    /* Ready the device for tx/rx */
    startup_tsec(tsec_info);

    /* Start up the PHY */
    ret = phy_startup(priv);
    if (ret)
    {
        printk("Could not initialize PHY %s\n",
                   tsec_info->devname);
        return ret;
    }

    adjust_link(priv);

    /* If there's no link, fail */
    return priv->link ? 0 : -1;
}
//---��ȡphy�ӿ����ͺ���------------------------------------------------
//���ܣ���ȡtsec��phy�ӿ����ӷ�ʽ������MII��GMII��RGMII��SGMII������ģʽ
//������struct tsec_private *priv��tsec����PHY���ýṹ��
//���أ�����tsec��PHY�ӿ�����ģʽ
//----------------------------------------------------------------------
static phy_interface_t tsec_get_interface(struct tsec_private *priv)
{
    tsec_t *regs = priv->regs;
    u32  ecntrl;

    ecntrl = in_be32(&regs->ecntrl);

    if (ecntrl & ECNTRL_SGMII_MODE)
        return PHY_INTERFACE_MODE_SGMII;

    if (ecntrl & ECNTRL_TBI_MODE)
    {
        if (ecntrl & ECNTRL_REDUCED_MODE)
            return PHY_INTERFACE_MODE_RTBI;
        else
            return PHY_INTERFACE_MODE_TBI;
    }

    if (ecntrl & ECNTRL_REDUCED_MODE)
    {
        if (ecntrl & ECNTRL_REDUCED_MII_MODE)
            return PHY_INTERFACE_MODE_RMII;
        else
        {
            phy_interface_t interface = priv->interface;

            /*
             * This isn't autodetected, so it must
             * be set by the platform code.
             */
            if ((interface == PHY_INTERFACE_MODE_RGMII_ID) ||
                    (interface == PHY_INTERFACE_MODE_RGMII_TXID) ||
                    (interface == PHY_INTERFACE_MODE_RGMII_RXID))
                return interface;

            return PHY_INTERFACE_MODE_RGMII;
        }
    }

    if (priv->flags & TSEC_GIGABIT)
        return PHY_INTERFACE_MODE_GMII;

    return PHY_INTERFACE_MODE_MII;
}


/* Discover which PHY is attached to the device, and configure it
 * properly.  If the PHY is not recognized, then return 0
 * (failure).  Otherwise, return 1
 */
//---phy��ʼ������-----------------------------------------------------
//���ܣ����PHY��·����״̬��������phy�Ĵ���
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//���أ�PHY�豸��������1������0
//---------------------------------------------------------------------
static s32 init_phy(struct tsec_info_struct *tsec_info)
{
    struct tsec_private *priv = (struct tsec_private *)tsec_info->priv;
    tsec_t *regs = priv->regs;
    u32  supported = (SUPPORTED_10baseT_Half |
                               SUPPORTED_10baseT_Full |
                               SUPPORTED_100baseT_Half |
                               SUPPORTED_100baseT_Full);

    if (priv->flags & TSEC_GIGABIT)
        supported |= SUPPORTED_1000baseT_Full;

    /* Assign a Physical address to the TBI */
    out_be32(&regs->tbipa, CONFIG_SYS_TBIPA_VALUE);

    priv->interface = tsec_get_interface(priv);

    if (priv->interface == PHY_INTERFACE_MODE_SGMII)
        tsec_configure_serdes(priv);

    /* Wait 15ms to make sure the PHY has come out of hard reset */
    Djy_DelayUs(15000);

    //config the PHY
    if(priv->interface == PHY_INTERFACE_MODE_GMII)//eTSEC1,RMII
    {
        /* Soft Reset the PHY */
        phy_reset(priv->phyaddr);
//        extern s32 phy_config_ar8021(s32 addr,s32 supported);
//        phy_config_ar8021(priv->phyaddr,supported);
    }
    else if(priv->interface == PHY_INTERFACE_MODE_RGMII)//eTSEC3,RGMII
    {
//        phy_reset_88e1512(priv->phyaddr);
//        phy_config_88e1512(priv->phyaddr,supported);
    }
    else if(priv->interface == PHY_INTERFACE_MODE_SGMII)//eTSEC2,SGMII
    {
        //eTSEC2ֱ�ӽӵ����ˣ�û����phy
    }
    return 1;
}
static struct tsec_private g_tsec_priv[3]= {{0}};
/* Initialize device structure. Returns success if PHY
 * initialization succeeded (i.e. if it recognizes the PHY)
 */
//---��ʼ��tsec���ڽṹ���������-------------------------------------
//���ܣ���ʼ��tsec���ڽṹ�����������tsec���ڵ�mac��ַ
//������struct tsec_info_struct *tsec_info��tsec�������ýṹ��
//      int num,tsec���ں�0-2
//      u8 last_mac,MAC��ַ�����һλ
//���أ�PHY�豸��������1������0
//--------------------------------------------------------------------

extern u8  sgNetHardMac[6];
static s32 tsec_initialize(struct tsec_info_struct *tsec_info,
                           s32 num,u8 last_mac)
{

    struct tsec_private *priv = &g_tsec_priv[num];

    priv->regs = tsec_info->regs;
    priv->phyregs_sgmii = tsec_info->miiregs_sgmii;

    priv->phyaddr = tsec_info->phyaddr;
    priv->flags = tsec_info->flags;
    sprintf(priv->name, tsec_info->devname);
    priv->interface = tsec_info->interface;
    tsec_info->priv = priv;


    /* Tell u-boot to get the addr from the env */
    tsec_info->enetaddr[0] = sgNetHardMac[0];
    tsec_info->enetaddr[1] = sgNetHardMac[1];
    tsec_info->enetaddr[2] = sgNetHardMac[2];
    tsec_info->enetaddr[3] = sgNetHardMac[3];
    tsec_info->enetaddr[4] = sgNetHardMac[4];
    tsec_info->enetaddr[5] = sgNetHardMac[5];


    /* Reset the MAC */
    setbits_be32(&priv->regs->maccfg1, MACCFG1_SOFT_RESET);
    Djy_DelayUs(2);  /* Soft Reset must be asserted for 3 TX clocks */
    clrbits_be32(&priv->regs->maccfg1, MACCFG1_SOFT_RESET);

    /* Try to initialize PHY here, and return */
    return init_phy(tsec_info);
}

//---tsec���ڽ��սӿں���-------------------------------------------
//���ܣ�tsec���ڽ��գ�����ӿ�
//������unsigned char  bychip,���յ����ں�
//      unsigned char* packet�����հ�ָ��
//���أ����հ�����
//------------------------------------------------------------------
u32 Net_RecvPacket(u8  bychip,u8* packet)
{
    s32 length=0;
    length = tsec_recv(&tsec_info[bychip],packet);

    return length;
}
//---tsec���ڷ��ͽӿں���-------------------------------------------
//���ܣ�tsec���ڷ��ͺ���������ӿ�
//������unsigned char  bychip,���յ����ں�
//      unsigned char* packet�����հ�ָ��
//      unsigned short length�����ͳ���
//���أ�����״̬
//-----------------------------------------------------------------
u32  Net_SendPacket(u8 bychip,u8* packet, u16 length)
{
    return tsec_send(&tsec_info[bychip],packet,length) ;
}
//---tsec���ڳ�ʼ������--------------------------------------------
//���ܣ�tsec���ڳ�ʼ����������ʼ��PHYоƬ����ʼ�����ö�Ӧ
//      ���ڵĽṹ�����
//������num,��ʼ�������ڱ�ţ�0-2
//      last_mac,���һ��MAC��ַ
//���أ�true,��ʼ���ɹ���false����ʼ��ʧ��
//-----------------------------------------------------------------
bool_t module_tsec_init(s32 num,u8 last_mac)
{
	s32 count;
	if(num > ARRAY_SIZE(tsec_info))
		return false;
	
	//��ʼ��tsec���ڽṹ���������
	if(tsec_initialize(&tsec_info[num],num,last_mac))	
	{
		//tsec����Ӳ����ʼ������
		count = tsec_init(&tsec_info[num]);	
		if(count == 0)
		return true;
	}
	
    return false;
}

