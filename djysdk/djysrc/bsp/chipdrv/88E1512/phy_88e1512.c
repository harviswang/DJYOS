//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   ������.
//����ģ��: 88E1512 phyоƬ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: 88E1512 phyоƬ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-11-14
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri_tsec.h"
#include "phy_88e1512.h"
#include "stdio.h"

#define SET_PAGE_REG      (0x16)

#define PAGE_NUM(x)       (x)
#define REGISTER(x)       (x)

#define CON_PHY_RGMII_MODE(x)   ((x & 0xFFF8) | 0x2)

#define MARVEL_RESET      (0x8000)

 //----��ȡ�Ƿ�����-----------------------------------------------------------------
//���ܣ���ȡMarvell PHY��״̬�Ĵ�������ȡ����״̬
//������int addr��phyоƬ��ַ
//���أ�����link = 0,û������
//      ����link = 1,�Ѿ�����
//-----------------------------------------------------------------------------
s32 genphy_update_link_88E1512(u32 addr)
{
    u32 reg,linked;
    s32 timeout = 500;
    s32 devad = -1;

    phy_write(addr, devad, SET_PAGE_REG, PAGE_NUM(1));
    reg = phy_read(addr, devad, REGISTER(17));
    while (timeout--)
    {
        reg = tsec_phy_read(addr, devad, REGISTER(17));
        if(reg & 0x0400)        //�Ƿ�������
            break;              //��������
        Djy_DelayUs(1000);
    }
    if(reg & 0x0400)
        linked = 1;
    else
        linked = 0;

    return linked;
}

//----��ȡphy�������-------------------------------------------------------
//���ܣ���ȡ�����������Ҫ���������ٶȺ��Ƿ�Ϊȫ˫��
//������int addr��phyоƬ��ַ
//���أ��ṹ��phy_parameter,����������ٶȺ�ȫ/��˫��
//-----------------------------------------------------------------------------
struct phy_parameter genphy_parse_link__88E1512(u32 addr)
{
    u32 reg,temp;
    s32 devad = -1;
    struct phy_parameter priv;

    reg = tsec_phy_read(addr, devad, SET_PAGE_REG);
    if(reg != PAGE_NUM(1))
    {
        tsec_phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(1));
    }

    reg = tsec_phy_read(addr, devad, REGISTER(17));
    temp = reg >> 14;
    switch(temp)
    {
        case 2:
            priv.speed = 1000;
            break;
        case 1:
            priv.speed = 100;
            break;
        case 0:
            priv.speed = 10;
            break;
        default:
            break;
    }
    if(reg & 0x2000)
        priv.duplex = 0x01;      //full_duplex
    else
        priv.duplex = 0x00;      //half_duplex

    return priv;
}

//----phy��ʼ������-----------------------------------------------------------------
//���ܣ�����Marvell PHY�Ĵ��������оƬ88E1512
//������int addr��phyоƬ��ַ��int supported��phyоƬ֧�ֵ����ݴ���ģʽ����
//���أ�����0
//-----------------------------------------------------------------------------
s32 phy_config_88e1512(s32 addr,s32 supported)
{
    s32 reg;
    s32 timeout = 500;
    s32 devad = -1;

    //��PHYҳ����Ϊ18
    reg = tsec_phy_read(addr, devad, SET_PAGE_REG);
    if(reg != PAGE_NUM(18))
    {
        if (phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(18)) < 0)
        {
            printf("PHY reset failed\r\n");
            return -1;
        }
    }

    //����ΪRGMIIģʽ��18ҳ����20���Ĵ���
    reg = tsec_phy_read(addr, devad, REGISTER(20));
    reg = CON_PHY_RGMII_MODE(reg);//RGMII 1000M
    tsec_phy_write(addr,devad,REGISTER(20),reg);

    //update the PHY's MODE
    reg = tsec_phy_read(addr, devad, REGISTER(20));
    reg |= MARVEL_RESET;
    tsec_phy_write(addr,devad,REGISTER(20),reg);

    //�ȴ�PHY's MODE ��ɸ���
    while ((reg & MARVEL_RESET) && timeout--)
    {
        reg = tsec_phy_read(addr, devad, REGISTER(20));
        Djy_DelayUs(1000);
    }
    if (reg & MARVEL_RESET)
    {
        printf("PHY reset timed out\n");
        return -1;
    }

    //��ȡ����״̬��page1,register1
    if (phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(1)) < 0)
    {
        printf("PHY reset failed\r\n");
        return -1;
    }
    reg = tsec_phy_read(addr, devad, REGISTER(0));
    reg &= 0xEFFF;//��ֹ����Ӧ
    tsec_phy_write(addr,devad,REGISTER(0),reg);
    reg = tsec_phy_read(addr, devad, REGISTER(26));
    reg |= 0x4000;//link noisy
    tsec_phy_write(addr, devad, REGISTER(26),reg);

    return 0;
}

//----phyоƬ�ĸ�λ---------------------------------------------------------
//���ܣ�����Marvell PHY�Ĵ��������оƬ88E1512��88e1512оƬ��������Բ�ͬ��
//      ������ҪӲ�������ã����Ҳ��Ҫ���ã�����ʱ�������ò�����ҳ
//������int addr��phyоƬ��ַ��
//���أ�����0
//--------------------------------------------------------------------------
void phy_reset_88e1512(s32 addr)
{
    s32 reg;
    s32 timeout = 500;
    s32 devad = -1;

    reg = tsec_phy_read(addr, devad, SET_PAGE_REG);

    if(reg != PAGE_NUM(1))
    {
        //SET PAGE1
        if (phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(1)) < 0)
        {
            printf("PHY reset failed\r\n");
            return;
        }
    }

    reg = tsec_phy_read(addr, devad, REGISTER(0));
    reg |= MARVEL_RESET;
    if (phy_write(addr, devad, REGISTER(0),reg) < 0)
    {
        printf("PHY reset failed\r\n");
        return;
    }

    /*
     * Poll the control register for the reset bit to go to 0 (it is
     * auto-clearing).  This should happen within 0.5 seconds per the
     * IEEE spec.
     */
    while ((reg & MARVEL_RESET) && timeout--)
    {
        reg = tsec_phy_read(addr, devad, REGISTER(0));

        if (reg < 0)
        {
            printf("PHY status read failed\r\n");
            return;
        }
        Djy_DelayUs(1000);
    }

    if (reg & MARVEL_RESET)
    {
        printf("PHY reset timed out\n");
        return;
    }

    return;
}


